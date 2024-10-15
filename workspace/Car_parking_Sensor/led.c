#include "led.h"
#include "timer0.h"

#if(LED_TIMER_INTERRUPT == TRUE)

uint16 overflowsNeeded = 0;
uint16 g_interruptCount = 0;

#endif

void LEDS_init(void)
{
	GPIO_setupPinDirection(LED_BLUE_PORT_ID,LED_BLUE_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LED_RED_PORT_ID,LED_RED_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LED_GREEN_PORT_ID,LED_GREEN_PIN_ID,PIN_OUTPUT);

	if(LED_CONNCETION_TYPE == POSTIVE_LOGIC){
		GPIO_writePin(LED_BLUE_PORT_ID,LED_BLUE_PIN_ID,LOGIC_LOW);
		GPIO_writePin(LED_RED_PORT_ID,LED_RED_PIN_ID,LOGIC_LOW);
		GPIO_writePin(LED_GREEN_PORT_ID,LED_GREEN_PIN_ID,LOGIC_LOW);
	}
	else if(LED_CONNCETION_TYPE == NEGATIVE_LOGIC){
		GPIO_writePin(LED_BLUE_PORT_ID,LED_BLUE_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(LED_RED_PORT_ID,LED_RED_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(LED_GREEN_PORT_ID,LED_GREEN_PIN_ID,LOGIC_HIGH);
	}

}

#if(LED_TIMER_INTERRUPT == TRUE)

void LED_flash(void)
{
	static char flag = 0;

	g_interruptCount++;
	if(g_interruptCount == overflowsNeeded){
		if(flag == 0){
			flag = 1;
			LED_on(BLUE);
			LED_on(RED);
			LED_on(GREEN);

		}
		else
		{
			flag = 0;
			LED_off(BLUE);
			LED_off(RED);
			LED_off(GREEN);
		}
		g_interruptCount = 0;
	}
}


void LED_interruptInit(uint16 durationInMsec)
{
	uint16 prescaler = 0;

	switch(LEDS_PRESCALER){
	case PRESCALER_0:
		prescaler = 0;
		break;
	case PRESCALER_1:
		prescaler = 1;
		break;
	case PRESCALER_8:
		prescaler = 8;
		break;
	case PRESCALER_64:
		prescaler = 64;
		break;
	case PRESCALER_256:
		prescaler = 256;
		break;
	case PRESCALER_1024:
		prescaler = 1024;
		break;
	}
	//uint32 totalClocks = (uint32)((((uint64)durationInMsec * F_CPU_HZ) / (prescaler * 1000)));

	uint32 durationInMicroseconds = durationInMsec * 1000UL;  // Convert to microseconds
	uint32 timePerTick = prescaler * 1000000UL / F_CPU_HZ;  // Time per tick in microseconds
	uint32 timePerOverflow = 256 * timePerTick;  // Time per overflow in microseconds

	// Calculate total overflows needed
	overflowsNeeded = durationInMicroseconds / timePerOverflow;

	TIMER0_configType config= {OVERFLOW,DISCONNECTED,LEDS_PRESCALER,TOIE0_bit,0,0};
	TIME0_setCallBack(LED_flash);
	TIMER0_init(&config);

}

void LED_interruptDeInit(){
	TIMER0_deInit();
}

#endif

void LED_on(LED_ID id)
{
	switch(id){
	case BLUE:
		GPIO_writePin(LED_BLUE_PORT_ID,LED_BLUE_PIN_ID,LOGIC_HIGH);
		break;
	case RED:
		GPIO_writePin(LED_RED_PORT_ID,LED_RED_PIN_ID,LOGIC_HIGH);
		break;
	case GREEN:
		GPIO_writePin(LED_GREEN_PORT_ID,LED_GREEN_PIN_ID,LOGIC_HIGH);
		break;
	}
}
void LED_off(LED_ID id)
{
	switch(id){
	case BLUE:
		GPIO_writePin(LED_BLUE_PORT_ID,LED_BLUE_PIN_ID,LOGIC_LOW);
		break;
	case RED:
		GPIO_writePin(LED_RED_PORT_ID,LED_RED_PIN_ID,LOGIC_LOW);
		break;
	case GREEN:
		GPIO_writePin(LED_GREEN_PORT_ID,LED_GREEN_PIN_ID,LOGIC_LOW);
		break;
	}
}

