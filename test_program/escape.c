#include <signal.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "../motor.h"

void handler(int signum)
{
	motor_stop();
	delay(100);
	exit(1);
}


int main()
{
	signal(SIGINT, handler);
	pwm_initialize();
	while(1)
	{
		motor_stack();
	}
}
