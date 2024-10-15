#include "timer0.h"

#include <avr/io.h>
#include "common_macros.h"
#include "gpio.h"
#include <avr/interrupt.h>


static volatile void(*g_callBackPtr)(void) = NULL_PTR;


void TIMER0_init(TIMER0_configType *config)
{

	/* clearing TIMER0 registers */
	TCCR0 = 0;
	TIMSK &= 0b11111100;


	/* configuring FOC0 bit */
	if(config -> mode == PWM_PHASE_CORRECT || config -> mode == FAST_PWM)
	{
		TCCR0 &=~(1<<FOC0);

		/* setting duty cycle */
		OCR0 = ((config -> outputCompareRegister) / 100) * 255;
	}
	else
	{
		TCCR0 |=(1<<FOC0);
	}

	if(config -> compareOutputMode != DISCONNECTED)
	{
		/* setting direction of OC0 pin */
		GPIO_setupPinDirection(TIMER0_OUTPUT_COMPARE_PORT_ID,TIMER0_OUTPUT_COMPARE_PIN_ID,PIN_OUTPUT);
	}

	/* configuring wave generation mode */
	switch(config ->mode)
	{
	case OVERFLOW:
		TCCR0 &=~(1<<WGM00) &~(1<<WGM01);
		break;
	case PWM_PHASE_CORRECT:
		TCCR0 &=~(1<<WGM01);
		TCCR0 |=(1<<WGM00);
		break;
	case CTC:
		TCCR0 &=~(1<<WGM00);
		TCCR0 |=(1<<WGM01);
		break;
	case FAST_PWM:
		TCCR0 |=(1<<WGM01);
		TCCR0 |=(1<<WGM00);
		break;
	}

	/* configuring compare output mode */
	TCCR0 = (TCCR0 & 0b11001111) | ((config -> compareOutputMode) << 4);

	/* configuring TIMER0 prescaler */
	TCCR0 = (TCCR0 & 0b11111000) | (config -> prescaler);

	/* enable interrupt bit if needed */
	switch(config -> interruptMask)
	{
	case TOIE0_bit:
		TIMSK|=(1<<TOIE0);
		SREG |= (1<<7);
		break;
	case OCIE0_bit:
		TIMSK|=(1<<OCIE0_bit);
		OCR0= config -> outputCompareRegister;
		SREG |= (1<<7);
		break;
	default:
		TIMSK &= 0b11111100;
	}

	/* setting timer counter initial value */
	TCNT0= config-> timerCounterRegister;
}

void TIME0_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;	/* saving address of call back function */
}

void TIMER0_deInit(void)
{
	/* clearing all TIMER0 registers and bits */
	TCCR0 = 0;
	TIMSK &= 0b11111100;
	TCNT0= 0;
	OCR0 = 0;

	/* resetting pointer to point to NULL */
	g_callBackPtr = NULL_PTR;
}


ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();		/* calling call back function */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		(*g_callBackPtr)();		/* calling call back function */
	}
}
