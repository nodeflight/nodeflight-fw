/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2020  Max Sikström
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/module.h"
#include "core/peripheral.h"
#include "core/interface_types.h"
#include "core/scheduler.h"
#include "core/variable.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stddef.h>
#include <math.h>

/* Specificataions from datasheet */
#define MPU6500_SPI_FREQ_STARTUP_HZ  1000000UL
#define MPU6500_SPI_FREQ_RUNNING_HZ  5000000UL
#define MPU6500_SPI_MODE             SPI_MODE_TRAILING_HIGH

/* mpu6500 configuration */
#define MPU6500_SAMPLE_RATE          8000

/* Implementation specific parameter */
#define MPU6500_TASK_PRIORITY        7
#define MPU6500_TASK_STACK_WORDS     256

#define MPU6500_NOTIFY_INT           0x00000001
#define MPU6500_NOTIFY_TIMEOUT       pdMS_TO_TICKS(2)

typedef struct mpu6500_s mpu6500_t;

struct mpu6500_s {
    if_spi_t *spi;
    if_gpio_t *pin_cs;
    if_gpio_t *pin_int;

    TaskHandle_t task;

    sc_t *sched;

    float out_gyro[3];
    float out_acc[3];
    float out_temp;
};

static int mpu6500_init(
    const char *name,
    md_arg_t *args);

static void mpu6500_task(
    void *storage);

static void mpu6500_wakeup(
    void *storage);

MD_DECL(motion_mpu6500, "ppps", mpu6500_init);

int mpu6500_init(
    const char *name,
    md_arg_t *args)
{
    mpu6500_t *mpu;
    int status;

    if (args[0].iface->peripheral->decl->type != PP_SPI) {
        return -1;
    }
    if (args[1].iface->peripheral->decl->type != PP_GPIO) {
        return -1;
    }
    if (args[2].iface->peripheral->decl->type != PP_GPIO) {
        return -1;
    }

    mpu = pvPortMalloc(sizeof(mpu6500_t));
    if (mpu == NULL) {
        return -1;
    }

    mpu->spi = IF_SPI(args[0].iface);
    mpu->pin_cs = IF_GPIO(args[1].iface);
    mpu->pin_int = IF_GPIO(args[2].iface);
    mpu->sched = args[3].sched;

    status = mpu->spi->configure(mpu->spi, &(const if_spi_cf_t) {
        .max_baud_rate = MPU6500_SPI_FREQ_STARTUP_HZ,
        .mode = MPU6500_SPI_MODE
    });
    if (status != 0) {
        return -1;
    }

    status = mpu->pin_cs->configure(mpu->pin_cs, &(const if_gpio_cf_t) {
        .dir = GPIO_DIR_OUT,
        .pull = GPIO_PULL_NONE
    });
    if (status != 0) {
        return -1;
    }
    mpu->pin_cs->set_value(mpu->pin_cs, true);

    if (0 != sc_configure_source(mpu->sched, 1.0f / MPU6500_SAMPLE_RATE)) {
        return -1;
    }

    vr_register(name, "fffffff",
        &mpu->out_gyro[0], &mpu->out_gyro[1], &mpu->out_gyro[2],
        &mpu->out_acc[0], &mpu->out_acc[1], &mpu->out_acc[2],
        &mpu->out_temp
    );

    xTaskCreate(
        mpu6500_task,
        "motion_mpu6500",
        MPU6500_TASK_STACK_WORDS,
        mpu,
        MPU6500_TASK_PRIORITY,
        &mpu->task);

    /* Configure interrupt pin after task is started, to reduce risk of early interrupt triggers */
    status = mpu->pin_int->configure(mpu->pin_int, &(const if_gpio_cf_t) {
        .dir = GPIO_DIR_IN,
        .pull = GPIO_PULL_NONE,
        .edge = GPIO_EDGE_FALLING,
        .edge_callback = mpu6500_wakeup,
        .edge_storage = mpu
    });
    if (status != 0) {
        return -1;
    }
    return 0;
}

static void mpu6500_wakeup(
    void *storage)
{
    mpu6500_t *mpu = storage;
    BaseType_t should_switch = pdFALSE;
    xTaskNotifyFromISR(mpu->task, MPU6500_NOTIFY_INT, eSetBits, &should_switch);
    portYIELD_FROM_ISR(should_switch);
}

static void mpu6500_write_reg(
    mpu6500_t *mpu,
    uint8_t reg,
    uint8_t value)
{
    uint8_t rx_buf[2];
    mpu->pin_cs->set_value(mpu->pin_cs, false);
    mpu->spi->transfer(mpu->spi, (uint8_t[]) {0x00 | reg, value}, rx_buf, 2);
    mpu->pin_cs->set_value(mpu->pin_cs, true);
}

static void mpu6500_startup(
    mpu6500_t *mpu)
{
    /* Power on */

    /* If the board has diffent power rails, give the system some margin to power up */
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Reset device */

    /* PWM_MGMT_1, DEVICE_RESET = 1 */
    mpu6500_write_reg(mpu, 107, 0x80);
    vTaskDelay(pdMS_TO_TICKS(100));

    /* SIGNAL_PATH_RESET, reset all */
    mpu6500_write_reg(mpu, 104, 0x07);
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Enable all parts */

    /* PWM_MGMT_1: no standby, internal oscillator */
    mpu6500_write_reg(mpu, 107, 0x00);
    /* PWM_MGMT_2: enable gyro and accelerometer */
    mpu6500_write_reg(mpu, 108, 0x00);
    /* PWM_MGMT_2: enable gyro and accelerometer */
    mpu6500_write_reg(mpu, 108, 0x00);

    /* Configure */

    /* SMPLRT_DIV: sample rate divider: /1 */
    mpu6500_write_reg(mpu, 25, 0x00);
    /* CONFIG: config, fifo mode = 0, ext_sync_set = 0, dlpf_cfg = 7 (3600Hz bw, 0.17ms delay) */
    mpu6500_write_reg(mpu, 26, 0x07);
    /* GYRO_CONFIG: gyro conf: no self test, +/- 2000deg/s, dlpf = 3600Hz */
    mpu6500_write_reg(mpu, 27, 0x18);
    /* ACCEL_CONFIG: accel conf: no self test, +/- 8g (0x18 for 16g) */
    mpu6500_write_reg(mpu, 28, 0x10);
    /* ACCEL_CONFIG 2: accel conf: no filtering, 0.75 ms delay */
    mpu6500_write_reg(mpu, 29, 0x00);
    /* LP_ACCEL_ODR: accel low power config: don't care (lowest filter) */
    mpu6500_write_reg(mpu, 30, 0x0b);

    /* Enable interrupt output */

    /*
     * INT_PIN_CFG:
     * bit 7 - 1 = int Active low
     * bit 6 - 0 = int push-pull
     * bit 5 - 1 = int held until cleared, 0 = pulse at 50us
     * bit 4 - 1 = int clear on read
     * bit 3-0 - fsync/bypass 0=disable
     */
    mpu6500_write_reg(mpu, 55, 0xb0);
    /* INT_ENABLE: RAW_RDY_EN=1 */
    mpu6500_write_reg(mpu, 56, 0x01);
}

void mpu6500_task(
    void *storage)
{
    mpu6500_t *mpu = storage;
    uint8_t tx_buf[16];
    uint8_t rx_buf[16];
    volatile float tmp_f;
    uint32_t notify_value;
    int16_t tmp_i16;
    int i;

    for (i = 0; i < 16; i++) {
        tx_buf[i] = 0;
    }

    mpu6500_startup(mpu);

    /*
     * Reconfigure driver for higher speed
     */
    mpu->spi->configure(mpu->spi, &(const if_spi_cf_t) {
        .max_baud_rate = MPU6500_SPI_FREQ_RUNNING_HZ,
        .mode = MPU6500_SPI_MODE
    });

    for (;;) {
        xTaskNotifyWait(0x00, UINT32_MAX, &notify_value, MPU6500_NOTIFY_TIMEOUT);

        /* Abort request if interrupt hasn't triggered (active low). failsafe */
        if (!(notify_value & MPU6500_NOTIFY_INT) && mpu->pin_int->get_value(mpu->pin_int)) {
            continue;
        }
        /*
         * Read all values:
         *
         * 59 - accel x high
         * 60 - accel x low
         * 61 - accel y high
         * 62 - accel y low
         * 63 - accel z high
         * 64 - accel z low
         * 65 - temperature high
         * 66 - temperature low
         * 67 - gyro x high
         * 68 - gyro x low
         * 69 - gyro y high
         * 70 - gyro y low
         * 71 - gyro z high
         * 72 - gyro z low
         */
        tx_buf[0] = 0x80 | 59;
        tx_buf[1] = 0;
        mpu->pin_cs->set_value(mpu->pin_cs, false);
        mpu->spi->transfer(mpu->spi, tx_buf, rx_buf, 15);
        mpu->pin_cs->set_value(mpu->pin_cs, true);

        /* Accel X */
        tmp_i16 = ((int16_t) rx_buf[1]) << 8 | rx_buf[2];
        tmp_f = tmp_i16 * (float) (8.0f / 32768.0f);
        mpu->out_acc[0] = tmp_f;

        /* Accel Y */
        tmp_i16 = ((int16_t) rx_buf[3]) << 8 | rx_buf[4];
        tmp_f = tmp_i16 * (float) (8.0f / 32768.0f);
        mpu->out_acc[1] = tmp_f;

        /* Accel Z */
        tmp_i16 = ((int16_t) rx_buf[5]) << 8 | rx_buf[6];
        tmp_f = tmp_i16 * (float) (8.0f / 32768.0f);
        mpu->out_acc[2] = tmp_f;

        /* Temperature */
        tmp_i16 = ((int16_t) rx_buf[7]) << 8 | rx_buf[8];
        tmp_f = tmp_i16 / 32768.0f;
        mpu->out_temp = tmp_f;

        /* Gyro X */
        tmp_i16 = ((int16_t) rx_buf[9]) << 8 | rx_buf[10];
        tmp_f = tmp_i16 * (float) (2000.0f * (float) M_PI / (32768.0f * 180.0f));
        mpu->out_gyro[0] = tmp_f;

        /* Gyro Y */
        tmp_i16 = ((int16_t) rx_buf[11]) << 8 | rx_buf[12];
        tmp_f = tmp_i16 * (float) (2000.0f * (float) M_PI / (32768.0f * 180.0f));
        mpu->out_gyro[1] = tmp_f;

        /* Gyro Z */
        tmp_i16 = ((int16_t) rx_buf[13]) << 8 | rx_buf[14];
        tmp_f = tmp_i16 * (float) (2000.0f * (float) M_PI / (32768.0f * 180.0f));
        mpu->out_gyro[2] = tmp_f;

        sc_trigger(mpu->sched);
    }
}
