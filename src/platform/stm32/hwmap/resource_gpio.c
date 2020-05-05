#include "hwacc/resource.h"

#include "platform/stm32/hwacc/gpio.h"

#include "stm32.h"

/* Not used, but keep for reference */
#define RESOURCE_TYPE_NONE 0
#define RESOURCE_TYPE_GPIO 1

#ifdef GPIOA
RESOURCE_DECL(pin_a00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a00));
RESOURCE_DECL(pin_a01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a01));
RESOURCE_DECL(pin_a02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a02));
RESOURCE_DECL(pin_a03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a03));
RESOURCE_DECL(pin_a04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a04));
RESOURCE_DECL(pin_a05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a05));
RESOURCE_DECL(pin_a06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a06));
RESOURCE_DECL(pin_a07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a07));
RESOURCE_DECL(pin_a08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a08));
RESOURCE_DECL(pin_a09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a09));
RESOURCE_DECL(pin_a10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a10));
RESOURCE_DECL(pin_a11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a11));
RESOURCE_DECL(pin_a12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a12));
RESOURCE_DECL(pin_a13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a13));
RESOURCE_DECL(pin_a14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a14));
RESOURCE_DECL(pin_a15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(a15));
#endif

#ifdef GPIOB
RESOURCE_DECL(pin_b00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b00));
RESOURCE_DECL(pin_b01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b01));
RESOURCE_DECL(pin_b02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b02));
RESOURCE_DECL(pin_b03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b03));
RESOURCE_DECL(pin_b04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b04));
RESOURCE_DECL(pin_b05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b05));
RESOURCE_DECL(pin_b06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b06));
RESOURCE_DECL(pin_b07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b07));
RESOURCE_DECL(pin_b08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b08));
RESOURCE_DECL(pin_b09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b09));
RESOURCE_DECL(pin_b10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b10));
RESOURCE_DECL(pin_b11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b11));
RESOURCE_DECL(pin_b12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b12));
RESOURCE_DECL(pin_b13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b13));
RESOURCE_DECL(pin_b14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b14));
RESOURCE_DECL(pin_b15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(b15));
#endif

#ifdef GPIOC
RESOURCE_DECL(pin_c00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c00));
RESOURCE_DECL(pin_c01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c01));
RESOURCE_DECL(pin_c02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c02));
RESOURCE_DECL(pin_c03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c03));
RESOURCE_DECL(pin_c04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c04));
RESOURCE_DECL(pin_c05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c05));
RESOURCE_DECL(pin_c06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c06));
RESOURCE_DECL(pin_c07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c07));
RESOURCE_DECL(pin_c08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c08));
RESOURCE_DECL(pin_c09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c09));
RESOURCE_DECL(pin_c10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c10));
RESOURCE_DECL(pin_c11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c11));
RESOURCE_DECL(pin_c12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c12));
RESOURCE_DECL(pin_c13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c13));
RESOURCE_DECL(pin_c14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c14));
RESOURCE_DECL(pin_c15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(c15));
#endif

#ifdef GPIOD
RESOURCE_DECL(pin_d00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d00));
RESOURCE_DECL(pin_d01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d01));
RESOURCE_DECL(pin_d02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d02));
RESOURCE_DECL(pin_d03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d03));
RESOURCE_DECL(pin_d04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d04));
RESOURCE_DECL(pin_d05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d05));
RESOURCE_DECL(pin_d06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d06));
RESOURCE_DECL(pin_d07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d07));
RESOURCE_DECL(pin_d08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d08));
RESOURCE_DECL(pin_d09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d09));
RESOURCE_DECL(pin_d10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d10));
RESOURCE_DECL(pin_d11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d11));
RESOURCE_DECL(pin_d12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d12));
RESOURCE_DECL(pin_d13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d13));
RESOURCE_DECL(pin_d14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d14));
RESOURCE_DECL(pin_d15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(d15));
#endif

#ifdef GPIOE
RESOURCE_DECL(pin_e00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e00));
RESOURCE_DECL(pin_e01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e01));
RESOURCE_DECL(pin_e02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e02));
RESOURCE_DECL(pin_e03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e03));
RESOURCE_DECL(pin_e04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e04));
RESOURCE_DECL(pin_e05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e05));
RESOURCE_DECL(pin_e06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e06));
RESOURCE_DECL(pin_e07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e07));
RESOURCE_DECL(pin_e08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e08));
RESOURCE_DECL(pin_e09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e09));
RESOURCE_DECL(pin_e10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e10));
RESOURCE_DECL(pin_e11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e11));
RESOURCE_DECL(pin_e12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e12));
RESOURCE_DECL(pin_e13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e13));
RESOURCE_DECL(pin_e14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e14));
RESOURCE_DECL(pin_e15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(e15));
#endif

#ifdef GPIOF
RESOURCE_DECL(pin_f00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f00));
RESOURCE_DECL(pin_f01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f01));
RESOURCE_DECL(pin_f02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f02));
RESOURCE_DECL(pin_f03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f03));
RESOURCE_DECL(pin_f04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f04));
RESOURCE_DECL(pin_f05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f05));
RESOURCE_DECL(pin_f06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f06));
RESOURCE_DECL(pin_f07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f07));
RESOURCE_DECL(pin_f08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f08));
RESOURCE_DECL(pin_f09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f09));
RESOURCE_DECL(pin_f10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f10));
RESOURCE_DECL(pin_f11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f11));
RESOURCE_DECL(pin_f12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f12));
RESOURCE_DECL(pin_f13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f13));
RESOURCE_DECL(pin_f14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f14));
RESOURCE_DECL(pin_f15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(f15));
#endif

#ifdef GPIOG
RESOURCE_DECL(pin_g00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g00));
RESOURCE_DECL(pin_g01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g01));
RESOURCE_DECL(pin_g02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g02));
RESOURCE_DECL(pin_g03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g03));
RESOURCE_DECL(pin_g04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g04));
RESOURCE_DECL(pin_g05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g05));
RESOURCE_DECL(pin_g06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g06));
RESOURCE_DECL(pin_g07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g07));
RESOURCE_DECL(pin_g08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g08));
RESOURCE_DECL(pin_g09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g09));
RESOURCE_DECL(pin_g10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g10));
RESOURCE_DECL(pin_g11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g11));
RESOURCE_DECL(pin_g12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g12));
RESOURCE_DECL(pin_g13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g13));
RESOURCE_DECL(pin_g14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g14));
RESOURCE_DECL(pin_g15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(g15));
#endif

#ifdef GPIOH
RESOURCE_DECL(pin_h00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h00));
RESOURCE_DECL(pin_h01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h01));
RESOURCE_DECL(pin_h02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h02));
RESOURCE_DECL(pin_h03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h03));
RESOURCE_DECL(pin_h04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h04));
RESOURCE_DECL(pin_h05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h05));
RESOURCE_DECL(pin_h06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h06));
RESOURCE_DECL(pin_h07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h07));
RESOURCE_DECL(pin_h08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h08));
RESOURCE_DECL(pin_h09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h09));
RESOURCE_DECL(pin_h10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h10));
RESOURCE_DECL(pin_h11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h11));
RESOURCE_DECL(pin_h12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h12));
RESOURCE_DECL(pin_h13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h13));
RESOURCE_DECL(pin_h14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h14));
RESOURCE_DECL(pin_h15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(h15));
#endif

#ifdef GPIOI
RESOURCE_DECL(pin_i00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i00));
RESOURCE_DECL(pin_i01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i01));
RESOURCE_DECL(pin_i02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i02));
RESOURCE_DECL(pin_i03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i03));
RESOURCE_DECL(pin_i04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i04));
RESOURCE_DECL(pin_i05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i05));
RESOURCE_DECL(pin_i06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i06));
RESOURCE_DECL(pin_i07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i07));
RESOURCE_DECL(pin_i08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i08));
RESOURCE_DECL(pin_i09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i09));
RESOURCE_DECL(pin_i10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i10));
RESOURCE_DECL(pin_i11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i11));
RESOURCE_DECL(pin_i12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i12));
RESOURCE_DECL(pin_i13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i13));
RESOURCE_DECL(pin_i14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i14));
RESOURCE_DECL(pin_i15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(i15));
#endif

#ifdef GPIOJ
RESOURCE_DECL(pin_j00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j00));
RESOURCE_DECL(pin_j01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j01));
RESOURCE_DECL(pin_j02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j02));
RESOURCE_DECL(pin_j03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j03));
RESOURCE_DECL(pin_j04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j04));
RESOURCE_DECL(pin_j05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j05));
RESOURCE_DECL(pin_j06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j06));
RESOURCE_DECL(pin_j07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j07));
RESOURCE_DECL(pin_j08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j08));
RESOURCE_DECL(pin_j09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j09));
RESOURCE_DECL(pin_j10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j10));
RESOURCE_DECL(pin_j11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j11));
RESOURCE_DECL(pin_j12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j12));
RESOURCE_DECL(pin_j13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j13));
RESOURCE_DECL(pin_j14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j14));
RESOURCE_DECL(pin_j15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(j15));
#endif

#ifdef GPIOK
RESOURCE_DECL(pin_k00, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k00));
RESOURCE_DECL(pin_k01, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k01));
RESOURCE_DECL(pin_k02, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k02));
RESOURCE_DECL(pin_k03, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k03));
RESOURCE_DECL(pin_k04, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k04));
RESOURCE_DECL(pin_k05, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k05));
RESOURCE_DECL(pin_k06, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k06));
RESOURCE_DECL(pin_k07, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k07));
RESOURCE_DECL(pin_k08, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k08));
RESOURCE_DECL(pin_k09, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k09));
RESOURCE_DECL(pin_k10, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k10));
RESOURCE_DECL(pin_k11, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k11));
RESOURCE_DECL(pin_k12, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k12));
RESOURCE_DECL(pin_k13, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k13));
RESOURCE_DECL(pin_k14, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k14));
RESOURCE_DECL(pin_k15, RESOURCE_TYPE_GPIO, 1, gpio_alloc, GPIO_ID(k15));
#endif