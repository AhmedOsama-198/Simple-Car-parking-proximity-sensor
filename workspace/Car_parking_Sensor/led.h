#ifndef LED_H_
#define LED_H_

#include "gpio.h"
#include "std_types.h"


#define LED_CONNCETION_TYPE POSTIVE_LOGIC

#define LED_BLUE_PORT_ID	PORTC_ID
#define LED_RED_PORT_ID		PORTC_ID
#define LED_GREEN_PORT_ID	PORTC_ID

#define LED_BLUE_PIN_ID		PIN2_ID
#define LED_RED_PIN_ID		PIN0_ID
#define LED_GREEN_PIN_ID	PIN1_ID

#define LED_TIMER_INTERRUPT	TRUE

#if(LED_TIMER_INTERRUPT == TRUE)



#define F_CPU_HZ						16000000
#define LEDS_PRESCALER					PRESCALER_1024

#endif

typedef enum{
	NEGATIVE_LOGIC,POSTIVE_LOGIC
}LEDs_connectionType;

typedef enum{
	BLUE,RED,GREEN
}LED_ID	;

void LEDS_init(void);
void LED_on(LED_ID id);
void LED_off(LED_ID id);

#if(LED_TIMER_INTERRUPT == TRUE)

void LED_interruptInit(uint16 durationInMsec);
void LED_interruptDeInit();

#endif

#endif /* LED_H_ */
