#ifndef DUTY_CYCLE_H
#define DUTY_CYCLE_H

#include <stdint.h>

#define   MAX_THETA		120 // pi/2
#define   MAX_THETAx2   240
#define   MAX_THETAx3   360
#define   MAX_THETAx4   480

#define	  MAX_PWM       2666

uint16_t duty_cycle(uint16_t theta, uint16_t Vm);

#endif /* end of include guard: DUTY_CYCLE_H */
