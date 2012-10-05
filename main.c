#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "gpio.h"
#include "init.h"
#include "psc_initialisation.h"
#include "controlVF.h"
#include "duty_cycle.h"

#include "sin.h"

/* used for the angle integrator */
#define		K_scal			16		

volatile uint8_t	Flag_IT_timer0 = 0;

int16_t		Command = 0;
int16_t		amplitude , OmegaTe = 64 ;
uint8_t		direction = 0 ;
uint16_t	theta1=0, theta2=160, theta3=320 ;
uint16_t    PWM0, PWM1, PWM2, DeadTime = 254 ;   // 254 => temps mort = 4 µs

int main(void)
{		
	init();
	PSC_Init(0x00, MAX_PWM); 
	
	while(1)
	{
//		/* Sampling */
//		if (Flag_IT_timer0)
//		{	
//			/* direction management : extract sign and absolute value */
//			if (Command > (int16_t)(0))
//			{
//				direction = 0;
//				OmegaTe   = Command;
//			} else {
//				direction = 1;
//				OmegaTe   = (~Command) + 1;
//			}
//			
//			if (OmegaTe > K_scal)
//			{
//			// ------------------------ V/f law --------------------------
//				amplitude = controlVF(OmegaTe);
//				
//			// ------------------ natural PWN algorithm ------------------
//				PWM0 = duty_cycle(theta1,amplitude);
//				PWM1 = duty_cycle(theta2,amplitude);
//				PWM2 = duty_cycle(theta3,amplitude);
//			} else {
//				PWM0 = 0;
//				PWM1 = 0;
//				PWM2 = 0;
//			}
//			
//			// -------- load the PSCs with the new duty cycles -----------
//			PSC0_Load(PWM0, PWM0 + DeadTime);
//			if (!direction)
//			{
//				PSC1_Load(PWM1, PWM1 + DeadTime);
//				PSC2_Load(PWM2, PWM2 + DeadTime);
//			} else {
//				PSC2_Load(PWM1, PWM1 + DeadTime);
//				PSC1_Load(PWM2, PWM2 + DeadTime);
//			}
//			
//			// 3 integrators for the evolution of the angles
//			theta1 = (K_scal * theta1 + OmegaTe) / K_scal;
//			theta2 = theta1 + 160;
//			theta3 = theta1 + 320;
//			
//			if (theta1>=MAX_THETAx4) theta1 -= MAX_THETAx4;
//			if (theta2>=MAX_THETAx4) theta2 -= MAX_THETAx4;
//			if (theta3>=MAX_THETAx4) theta3 -= MAX_THETAx4;
//			
//			Flag_IT_timer0 = 0;
//		}
	}

    return 0;
}

volatile uint16_t deltaPhase = 1092;
volatile int16_t  sineAmp = 1000;

volatile uint16_t phase = 0;
volatile int32_t value;

/*
 * Interrupt vector for the sampling period (Ts = 1 ms)
 *******************************************************************/
ISR(TIMER0_COMP_A_vect)
{
    phase += deltaPhase;

    value = (int32_t)sineAmp * (int8_t)pgm_read_byte(SINE + (uint8_t)(phase >> 8));
    value /= 128;
    value += 1333;
   
    PSC0_Load(value, value + DeadTime);
    PSC1_Load(value, value + DeadTime);
    PSC2_Load(value, value + DeadTime);
}

