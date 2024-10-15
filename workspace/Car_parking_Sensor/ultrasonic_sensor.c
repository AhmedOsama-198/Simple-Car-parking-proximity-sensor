

#include "ultrasonic_sensor.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>



static uint16 US_distance = 0;
static uint16 US_HighTime = 0;




void Ultrasonic_init(void)
{
	/* settin ICU call back function */
	ICU_setCallBack(Ultrasonic_edgeProcessing);

	/* initializing ICU with specific configurations*/
	ICU_ConfigType US_config = {US_SENSOR_ICU_PRESCALER, US_SENSOR_ICU_STARTING_EDGE};
	ICU_init(&US_config);

	/* configuring trigger pin as output pin */
	GPIO_setupPinDirection(US_SENSOR_TRIGGER_PORT_ID, US_SENSOR_TRIGGER_PIN_ID, PIN_OUTPUT);
}

void Ultrasonic_edgeProcessing(void)
{
	static uint8 US_EdgesCount = 0;
	US_EdgesCount++;
	if(US_EdgesCount == 1)
	{
		ICU_clearTimerValue();

		/* setting icu edge */
		ICU_setEdgeDetectionType(FALLING);
	}
	else if(US_EdgesCount == 2)
	{
		/* storing ICR1 value */
		US_HighTime = ICU_getInputCaptureValue();

		ICU_clearTimerValue();

		ICU_setEdgeDetectionType(RAISING);

		/* resetting edges counter variable */
		US_EdgesCount = 0;

	}
}

void Ultrasonic_Trigger(void)
{
	/* trigger for 10us */
	GPIO_writePin(US_SENSOR_TRIGGER_PORT_ID, US_SENSOR_TRIGGER_PIN_ID, TRIGGER);
	_delay_us(11);
	GPIO_writePin(US_SENSOR_TRIGGER_PORT_ID, US_SENSOR_TRIGGER_PIN_ID, NO_TRIGGER);
}

uint16 Ultrasonic_readDistance(void)
{

	Ultrasonic_Trigger();

	/* calculating distance */
	//US_distance = ((34000 * US_HighTime * TIME_OF_CLOCK_CYCLE_sec) / 2);
	US_distance = (uint16)((US_HighTime)/117.6)+1;


	return US_distance;
}

