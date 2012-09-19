/*******************************************************************************
 * $Id: pll_drv.h 6 2012-02-03 23:42:58Z reviakinea@gmail.com $
 ******************************************************************************/

#ifndef PLL_DRV_H
#define PLL_DRV_H

#define Start_pll_64_mega()		(PLLCSR = 0x06)            // Start the PLL at 64MHz
#define Start_pll_32_mega()		(PLLCSR = 0x02)            // Start the PLL at 32MHz

// Start the PLL at the parameter frequency (only 64 or 32 MHz)
#define Start_pll(frequency)	(PLLCSR = ( (((frequency>>6)&1)<<2) | (1<<1) ) ) 
          
#define Is_pll_ready()			(PLLCSR & (1<<PLLOCK) )    // Return 1 if the PLL is ready
#define Wait_pll_ready()		while (!(PLLCSR & (1<<PLOCK)))
#define Stop_pll()				(PLLCSR = 0x00)            // Stop the PLL
       
#endif  // PLL_DRV_H
