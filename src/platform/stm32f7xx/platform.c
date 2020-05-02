#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

#include "platform/platform.h"

static UART_HandleTypeDef s_uart;

void SysTick_Handler(
    void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

int _write(
    int fd,
    uint8_t *buf,
    int size)
{
    /* stdout */
    if (fd == 1) {
        HAL_UART_Transmit(&s_uart, buf, size, HAL_MAX_DELAY);
        return size;
    }
    return -1;
}

void platform_init(
    void)
{

    HAL_Init();

    __USART3_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pin = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    s_uart.Instance = USART3;
    s_uart.Init = (UART_InitTypeDef) {
        .BaudRate = 115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = USART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_8,
        .OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLED
    };

    if (HAL_UART_Init(&s_uart) != HAL_OK) {
        asm ("bkpt 255");
    }
}