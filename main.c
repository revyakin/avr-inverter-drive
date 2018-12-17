/*******************************************************************************
 * $Id: main.c 16 2012-02-08 01:27:53Z reviakinea@gmail.com $
 ******************************************************************************/
 
 /*
  * You Ad could be here! :-)
  *** copyleft (c) 2011 by Reviakin E. A. *************************************/

#include <inttypes.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "gpio.h"
#include "uart/uart.h"
#include "comm.h"
#include "pid.h"
#include "global_vars.h"

/* UART baudrate */
#define		UART_BAUDRATE	9600

/* one quarter of the circle */
#define		MAX_THETA		120		
#define		MAX_THETAx2		240
#define		MAX_THETAx3		360
#define		MAX_THETAx4		480

/* 64MHz (PLL frequency) / 2666 / 2 = 12 kHz (PWM frequency) */
#define		MAX_PWM			2666

/* used for the angle integrator */
#define		K_scal			16		

volatile uint8_t	Flag_IT_timer0 = 0;
volatile uint32_t	timer = 0;

volatile int16_t	Omega_meas = 0;

/* fuuuuuuking koefff = 7.8125 */

int16_t		pid_output;
int16_t		Omega_ref = 0; // Omega_ref = w * 1.223 = n * 0.128
int16_t		Command = 0;
uint16_t	theta1=0, theta2=160, theta3=320 ;

int16_t		amplitude , OmegaTe = 64 ;
uint8_t		direction = 0 ;
uint16_t    PWM0, PWM1, PWM2, DeadTime = 254 ;   // 254 => temps mort = 4 µs

void		init(void);
void		PSC_Init(unsigned int ot0,  unsigned int ot1);
void		PSC0_Load (unsigned int dt0, unsigned int dt1);
void		PSC1_Load (unsigned int dt0, unsigned int dt1);
void		PSC2_Load (unsigned int dt0, unsigned int dt1);
uint16_t	controlVF(uint16_t wTs);
uint16_t	duty_cycle(uint16_t theta, uint16_t Vm) ;


uint16_t freq_to_omega(uint16_t freq)
{
	return freq * 360 * K_scal / 1000; 
}	

void main(void)
{		
	init();
	PSC_Init(0x00, MAX_PWM); 
	uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUDRATE, F_CPU));
	
	/* !!! */
	Command = freq_to_omega(50);	// 50 Hz
	
	while(1)
	{
		/* Communicate with Big Brother :-) */
		// communication();
		
		/* Sampling */
		if (Flag_IT_timer0)
		{	
			if ( !(timer % 100) )
			{
				/* This led shows that program is running */
				GPIO_TOGGLE(WD_LED);
			}
			
			if ( !(timer % 10) )
			{
				GPIO_TOGGLE(LED);
				
				/* Read meashured speed from Timer2 */
				Omega_meas = TCNT1 * 10;
				TCNT1      = 0;
					
				/* PID controller */
				// pid_output = update_pid(reference_val.omega - Omega_meas, Omega_meas);
				// Command    = ((int32_t)pid_output * 131) >> 10;
					
				/* Update immediate vals */
				instant_val.time       = timer;
				instant_val.omega      = Omega_meas;
				instant_val.error      = reference_val.omega - Omega_meas;
				instant_val.pid_output = pid_output;
				instant_val.command	   = Command;
			}
			
			/* direction management : extract sign and absolute value */
			if (Command > (int16_t)(0))
			{
				direction = 0;
				OmegaTe   = Command;
			} else {
				direction = 1;
				OmegaTe   = (~Command) + 1;
			}
			
			if (OmegaTe > K_scal)
			{
			// ------------------------ V/f law --------------------------
				amplitude = controlVF(OmegaTe);
				
			// ------------------ natural PWN algorithm ------------------
				PWM0 = duty_cycle(theta1,amplitude);
				PWM1 = duty_cycle(theta2,amplitude);
				PWM2 = duty_cycle(theta3,amplitude);
			} else {
				PWM0 = 0;
				PWM1 = 0;
				PWM2 = 0;
			}
			
			// -------- load the PSCs with the new duty cycles -----------
			PSC0_Load(PWM0, PWM0 + DeadTime);
			if (!direction)
			{
				PSC1_Load(PWM1, PWM1 + DeadTime);
				PSC2_Load(PWM2, PWM2 + DeadTime);
			} else {
				PSC2_Load(PWM1, PWM1 + DeadTime);
				PSC1_Load(PWM2, PWM2 + DeadTime);
			}
			
			// 3 integrators for the evolution of the angles
			theta1 = (K_scal * theta1 + OmegaTe) / K_scal;
			theta2 = theta1 + 160;
			theta3 = theta1 + 320;
			
			if (theta1>=MAX_THETAx4) theta1 -= MAX_THETAx4;
			if (theta2>=MAX_THETAx4) theta2 -= MAX_THETAx4;
			if (theta3>=MAX_THETAx4) theta3 -= MAX_THETAx4;
			
			Flag_IT_timer0 = 0;
		}
	}
}

/*
 * Interrupt vector for the sampling period (Ts = 1 ms)
 *******************************************************************/
ISR(TIMER0_COMP_A_vect)
{
	/* time in ms */
	timer ++;
		
	/* Sampling */
	Flag_IT_timer0 = 1;
}

