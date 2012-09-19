 /*
  * You Ad could be here! :-)
  *** copyleft (c) 2011 by Reviakin E. A. *************************************/
 
#include "pid.h"

int16_t	p_gain = 1024;
int16_t	i_gain = 0;
int16_t	d_gain = 0;
 
int32_t update_pid(int32_t error, int32_t omega)
{
	int32_t	p_term = 0;
	int32_t	i_term = 0;
	int32_t	d_term = 0;
	
	p_term = (p_gain * (int32_t)error) >> 10;
	
	//p_term >>= 10;
	i_term >>= 10;
	d_term >>= 10;
	return (int32_t)(p_term + i_term + d_term);
}
