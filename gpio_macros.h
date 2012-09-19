 /*
  * You Ad could be here! :-)
  *** copyleft (c) 2011 by Reviakin E. A. *************************************/

/*
	#define LED 	A, 0	// port A, bit 0
	#defint KEY 	B, 5	// port B, bit 5
	
	...
	
	void foo()
	{
		GPIO_DIRECTION(LED, OUTPUT);
		GPIO_DIRECTION(KEY, INPUT_PULLUP);
		
		GPIO_CLR(LED);
		
		if (GPIO_IS_CLR(KEY))
			GPIO_SET(LED);
		
	}
 
	...
 
 */

#ifndef GPIO_MACROS_H
#define GPIO_MACROS_H

#define __GPIO_IS_SET(port,pin) (PIN##port & (1 << pin))
#define __GPIO_IS_CLR(port,pin) (!(PIN##port & (1 << pin)))

#define __GPIO_CLR(port,pin) 	do{ PORT##port &= ~(1 << pin); }while(0)
#define __GPIO_SET(port,pin) 	do{ PORT##port |=  (1 << pin); }while(0)
#define __GPIO_TOGGLE(port,pin) do{ PORT##port ^=  (1 << pin); }while(0)

#define __GPIO_DIR_INPUT(port, pin)			do{ DDR##port &= ~(1 << pin); PORT##port &= ~(1 << pin); }while(0)
#define __GPIO_DIR_INPUT_PULLUP(port, pin)	do{ DDR##port &= ~(1 << pin); PORT##port |=  (1 << pin); }while(0)
#define __GPIO_DIR_OUTPUT(port, pin)		do{ DDR##port |=  (1 << pin);                            }while(0)

#define GPIO_DIRECTION(x, dir)	__GPIO_DIR_##dir(x)

#define GPIO_CLR(x)			__GPIO_CLR(x)
#define GPIO_SET(x)			__GPIO_SET(x)
#define GPIO_TOGGLE(x)		__GPIO_TOGGLE(x)

#define GPIO_IS_SET(x)		__GPIO_IS_SET(x)
#define GPIO_IS_CLR(x)		__GPIO_IS_CLR(x)

#endif  /* GPIO_MACROS_H */