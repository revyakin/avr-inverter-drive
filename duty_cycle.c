/*******************************************************************************
 * $Id: duty_cycle.c 5 2012-02-03 23:38:47Z reviakinea@gmail.com $
 ******************************************************************************/

#include <inttypes.h>
#include "table_sin120.h"

#define   MAX_THETA		120 // pi/2
#define   MAX_THETAx2   240
#define   MAX_THETAx3   360
#define   MAX_THETAx4   480
#define	  MAX_PWM       2666

/****************************************************************************/
/*                       duty cycle                                         */
/****************************************************************************/
/*!
 * This function computes the PWM duty cycles :
   *    - use the sine table to compute sin(theta),
   *    - multiply by Vm (comming from the V/f law)
   *    - shift by MAX_PWM/2 to generate positive and negative voltages
   *    - only one quarter of the period is stored in the sine table
*/

/******************************************************************************
 *
 * В этой функции происходит умножение напряжения статора на синус theta
 *                     VmxSin = Vm * sin(theta)
 *
 *      И преобразование этого значения для передачи модулю PSC
 *                     PWM = (MAX_PWM / 2) + VmxSin
 *
 ******************************************************************************/

uint16_t duty_cycle(uint16_t theta, uint16_t Vm)
{
  int8_t	sinus ;
  int32_t	Vmxsin ;

	if (theta <= MAX_THETA)                     		// x in [0;pi/2]
		sinus = tab_sin[theta];                 		// sin(x)
	else
    {
		if (theta <= MAX_THETAx2)                		// x in [pi/2;pi]
			sinus = tab_sin[MAX_THETAx2 - theta];		// sin(pi-x)
		else
		{
			if (theta <= MAX_THETAx3)              		// x in [pi;3*pi/2]
				sinus = - tab_sin[theta - MAX_THETAx2]; // sin(x-pi)
			else                                   		// x in [3*pi/2;2*pi]
				sinus = - tab_sin[MAX_THETAx4 - theta]; // sin(2*pi-x)
		}
	}
	
	Vmxsin = sinus;
	Vmxsin = (Vmxsin * Vm) / 128 ;
	
	return ((unsigned int) (MAX_PWM / 2 + Vmxsin));
}
