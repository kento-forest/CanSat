#include <stdio.h>
#include <math.h>
#include <wiringPiI2C.h>
#include "../acclgyro.h"

int main()
{
	Gyro gyro_data;
	acclGyro_initialize();
	int now = 0;
	int lastTime = millis();
	double delat_theta = 0;
	while(1)
	{
		Gyro_read(&gyro_data);
		now = millis();
		int delta_time = now-lastTime;
		printf("gyro_data.gyroZ_scaled %f\n",gyro_data.gyroZ_scaled);
		delat_theta += gyro_data.gyroZ_scaled*delta_time/1000;//convert2seconds
		printf("%f\n",delat_theta);
		lastTime = now;
	}

	return 0;
}
