/*******************************************************************************
 * $Id: controlVF.c 6 2012-02-03 23:42:58Z reviakinea@gmail.com $
 ******************************************************************************/

#include <inttypes.h>

#define Vf_SLOPE		(uint16_t)(220/50)		// (было 5) 	наклон V/f ..  Vном.двиг. / Fном.двиг.
#define OMEGA_TS_MAX	(uint16_t)(1650*0.128)	// (было 192) 	номинальная частота вращения двигателя - OMEGA_TS_MAX = Nном.двиг * 0.128
#define OMEGA_TS_MIN	(uint16_t)(264*0.128)	// (было 32)	250 об./мин. - почему ?? (16% от номинальной мля..  )

#define	MAX_PWM			2666	// макс. значение ШИМ
#define	VCC				220		// напряжение питания инвертора

#define	PWM_Koeff		(uint16_t)(VCC / (MAX_PWM / 2))

/*
   w_max.Te_scal = (480/2*pi) * Te * w_max = 12
     Te = 1/1000
     w_max = 1500 .(pi/30) = 50.pi

    => w_max = 16 * w_max.Te_scal = 192

*/

// V/f law
uint16_t controlVF(uint16_t wTs)
{
	uint16_t amp;
	
	// boost frequency
	if (wTs <= OMEGA_TS_MIN )
	{
		// boost voltage
		amp = Vf_SLOPE * OMEGA_TS_MIN; // (160 было)
	}
    else
		if ( (wTs > OMEGA_TS_MIN) & (wTs < OMEGA_TS_MAX) )
			// V/f law
			amp = Vf_SLOPE * wTs ;  
		else
			// rated value
			amp = Vf_SLOPE * OMEGA_TS_MAX; //(было 960) номинальное напряжение двигателя
			
    return amp ;
}
