#include "lcd.h"
#include "buzzer.h"
#include "ultrasonic_sensor.h"
#include "led.h"


uint16 distance = 0;	/* variable to store distance */

typedef enum{
	SAFE, ALARM
}State;

State alarmState = SAFE;


void handleDisplay(void);

void handleAlarm(void);

void handleControl(void);


int main()
{

	LEDS_init();
	Ultrasonic_init();		/* initializing ultrasonic sensor */
	LCD_init();				/* initializing LCD driver */
	BUZZER_init();


	LCD_displayString("Distance = ");

	while(1)
	{

		distance = Ultrasonic_readDistance();

		handleDisplay();

		handleAlarm();

		handleControl();
	}
}

void handleDisplay(void)
{


	LCD_moveCursor(0,11);
	LCD_intgerToString(distance);
	LCD_displayString(" Cm");

}
void handleAlarm(void)
{

	if(distance <= 5 && alarmState == SAFE)
	{
		LED_off(RED);
		LED_off(BLUE);
		LED_off(GREEN);
		BUZZER_on();
		alarmState = ALARM;
		LCD_displayStringRowColumn(1,0,"STOP");

		LED_interruptInit(500);

	}
	else if(distance > 5 && alarmState == ALARM)
	{
		BUZZER_off();
		LED_interruptDeInit();
		alarmState = SAFE;
		LCD_displayStringRowColumn(1,0,"    ");

	}
}

void handleControl(void)
{
	if(distance <= 10 && distance >= 6)
	{
		LED_on(RED);
		LED_on(BLUE);
		LED_on(GREEN);
	}
	else if(distance <= 15 && distance >= 11)
	{
		LED_on(RED);
		LED_off(BLUE);
		LED_on(GREEN);

	}
	else if(distance <= 20 && distance >= 16)
	{
		LED_on(RED);
		LED_off(BLUE);
		LED_off(GREEN);

	}
	else if(distance > 20)
	{

		LED_off(RED);
		LED_off(BLUE);
		LED_off(GREEN);
	}
}
