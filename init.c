#include "init.h"

#include <avr/io.h>
#include "gpio.h"

void init(void) {
	/*************************************************************************************/
	/*           ports direction configuration                                           */
	/*************************************************************************************/
	
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	DDRE = 0;
	
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	/* PWM outputs */
	GPIO_DIRECTION(PSCOUT00, OUTPUT);
	GPIO_DIRECTION(PSCOUT01, OUTPUT);
	GPIO_DIRECTION(PSCOUT10, OUTPUT);
	GPIO_DIRECTION(PSCOUT11, OUTPUT);
	GPIO_DIRECTION(PSCOUT20, OUTPUT);
	GPIO_DIRECTION(PSCOUT21, OUTPUT);

	/* Leds */
	GPIO_DIRECTION(LED,    OUTPUT);
	GPIO_DIRECTION(WD_LED, OUTPUT);

	/* Encoder */
	GPIO_DIRECTION(ENCODER_A, INPUT_PULLUP);
	GPIO_DIRECTION(ENCODER_B, INPUT_PULLUP);
	
    /*************************************************************************************/
    /*     Timer 0 Configuration : generates the sampling frequency                      */
    /*************************************************************************************/
	TCCR0A = (1<<WGM01);   // mode CTC : Clear Timer on Compare
	TCCR0B = (1<<CS02);    // f_quartz = 12 MHz / 256 = 48 kHz
	OCR0A = 0x4;          // one interruption every ms
	TIMSK0 = (1<<OCIE0A);  // allow interruption when timer=compare

	/**************************************************************************************/
	/* Timer1: normal mode, ext. clock on rising edge. Used to calculate the motor speed  */
	/**************************************************************************************/
	TCCR1A = 0;
	TCCR1B = _BV(CS10) | _BV(CS11) | _BV(CS12);
	TCCR1C = 0;
	
	SREG |= 0x80;            // allow interruptions,
}
