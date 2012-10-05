#include <avr/io.h>

.section .text

.global TIMER0_COMP_A_vect 

TIMER0_COMP_A_vect:

    ; phase += delta
    lds     r16, phase
    lds     r17, phase+1

    lds     r18, delta
    lds     r19, delta+1

    add     r16, r18
    adc     r17, r19

    sts     phase, r16
    sts     phase+1, r17

    lds     r18, amplitude
    lds     r19, amplitude+1

    ; r16 = SINE[phaseH & 0x7f]
    clr     r31
    mov     r30, r17
    lsr     r30

    addiw   r30, SINE
    lpm     r16, Z

    mul     r18, r16
    mov     r30, r1

    mul     r19, r16
    add     r30, r0
    adc     r31, r1

    sbrc    r16, 7
    sbwi




    
