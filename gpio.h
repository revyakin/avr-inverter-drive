#ifndef GPIO_H
#define GPIO_H

#include "gpio_macros.h"

/* PWM outputs */
#define	PSCOUT00	D, 0
#define	PSCOUT01	B, 7
#define	PSCOUT10	C, 0
#define	PSCOUT11	B, 6
#define	PSCOUT20	B, 0
#define	PSCOUT21	B, 1

/* Watchdog LED */
#define WD_LED		C, 1

/* LED */
#define LED			C, 2

/* Encoder */
#define ENCODER_A	C, 3
#define ENCODER_B	B, 5


#endif  /* GPIO_H */