#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

/* Immadiate values */
struct instant_val
{
	uint32_t	time;
	int16_t		omega;
	int16_t		error;
	int16_t		pid_output;
	int16_t		command;
} instant_val;

/* Reference values */
struct reference_val
{
	int16_t		omega;
} reference_val;

#endif /* GLOBAL_VARS_H */