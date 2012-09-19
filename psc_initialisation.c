/*******************************************************************************
 * $Id: psc_initialisation.c 6 2012-02-03 23:42:58Z reviakinea@gmail.com $
 ******************************************************************************/

#include <inttypes.h>

#include <avr/io.h>
#include "mcu/pll/pll_drv.h"

#define LOW(x)		((uint8_t)(x))
#define HIGH(x)		((uint8_t)((x) >> 8))

/**********************************************************************************/
/*                   PSC Initialization                                           */
/***********************************************************************************/
/*!
 * This function initializes the PSCs :
   *    - centered aligned mode,
   *    - fast clock input (the PWM clock (64 MHz) comes from the PLL)
*/
/**********************************************************************************/
void PSC_Init(unsigned int ot0,  unsigned int ot1)
{
	Start_pll_64_mega();	// start the PLL at 64 MHz
	Wait_pll_ready();

	OCR0RAH = HIGH(ot0);
	OCR0RAL = LOW(ot0);
	OCR0RBH = HIGH(ot1);
	OCR0RBL = LOW(ot1);
	PCNF0 = (1<<PMODE01) | (1<<PMODE00) | (1<<PCLKSEL0) ;  /* fast clock input used */
	PFRC0A = 0;
	PFRC0B = 0;
	PCTL0 = (1<<PARUN0) | (1<<PCCYC0);

	OCR1RAH = HIGH(ot0);
	OCR1RAL = LOW(ot0);
	OCR1RBH = HIGH(ot1);
	OCR1RBL =  LOW(ot1);
	PCNF1 = (1<<PMODE11) | (1<<PMODE10) | (1<<PCLKSEL1);   /* fast clock input used */
	PFRC1A = 0;
	PFRC1B = 0;
	PCTL1 = (1<<PARUN1) | (1<<PCCYC1);

	OCR2RAH = HIGH(ot0);
	OCR2RAL = LOW(ot0);
	OCR2RBH = HIGH(ot1);
	OCR2RBL = LOW(ot1);
	PCNF2 = (1<<PMODE21) | (1<<PMODE20) | (1<<PCLKSEL2);   /* fast clock input used */
	PFRC2A = 0;
	PFRC2B = 0;
	PCTL2 = (1<<PCCYC2) | (1<<PRUN2) ;

	// connect the PSC waveform generator outputs to the port outputs
	PSOC0 = (1<<POEN0B) | (1<<POEN0A) ;
	PSOC1 = (1<<POEN1B) | (1<<POEN1A) ;
	PSOC2 = (1<<POEN2B) | (1<<POEN2A) ;
}

/**********************************************************************************/
/*                                  PSC0                                          */
/**********************************************************************************/
/**
* @brief PSC0_Load: load the OCR0SA et OCR0SB registers with new values
*/
/**********************************************************************************/
void PSC0_Load (unsigned int dt0,  unsigned int dt1)
{
	OCR0SAH = HIGH(dt0);
	OCR0SAL = LOW(dt0);
	OCR0SBH = HIGH(dt1);
	OCR0SBL = LOW(dt1);
}

/**********************************************************************************/
/*                                  PSC1                                          */
/**********************************************************************************/
/**
* @brief PSC1_Load: load the OCR1SA et OCR1SB registers with new values
*/
/**********************************************************************************/
void PSC1_Load (unsigned int dt0, unsigned int dt1)
{
	OCR1SAH = HIGH(dt0);
	OCR1SAL = LOW(dt0);
	OCR1SBH = HIGH(dt1);
	OCR1SBL = LOW(dt1);
}

/**********************************************************************************/
/*                                  PSC2                                          */
/**********************************************************************************/
/**
* @brief PSC2_Load: load the OCR2SA et OCR2SB registers with new values
*/
/**********************************************************************************/
void PSC2_Load (unsigned int dt0, unsigned int dt1)
{
	OCR2SAH = HIGH(dt0);
	OCR2SAL = LOW(dt0);
	OCR2SBH = HIGH(dt1);
	OCR2SBL = LOW(dt1);
}
/**********************************************************************************/
