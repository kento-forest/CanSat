#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

static const int angle_of_deviation = -7;
static const int devid = 0x1e; //I2C address
static const int mode_reg = 0x02;
static const int mode_continuous = 0x00;
static const int mode_single = 0x01;
static const int x_msb_reg = 0x03;
static const int x_lsb_reg = 0x04;
static const int z_msb_reg = 0x05;
static const int z_lsb_reg = 0x06;
static const int y_msb_reg = 0x07;
static const int y_lsb_reg = 0x08;
static const double PI = 3.14159265;
int fd;

static short read_out(int file,int msb_reg, int lsb_reg)
{
	uint8_t msb = wiringPiI2CReadReg8(file, msb_reg);
	uint8_t lsb = wiringPiI2CReadReg8(file, lsb_reg);
	short i = msb << 8| lsb;

	return i;
}

static double calc_compass_angle(short x,short y)
{
	double angle_calc1 = atan2((double)-y, (double)-x)*(180/PI) + 180;
	double angle_calc2 = angle_calc1 + angle_of_deviation;
	double angle_return;
	if (angle_calc2 > 360)
	{
		angle_return = angle_calc2 - 360;
	}
	else if(angle_calc2<0)
	{
		angle_return = angle_calc2 + 360;
	}
	else
	{
		angle_return = angle_calc2;
	}
	return angle_return;
}

int compass_initializer()
{
	/* WHO AM I */
	int fd = wiringPiI2CSetup(devid);
	/* start senser */
	if((wiringPiI2CWriteReg8(fd,mode_reg,mode_continuous))<0)
	{
		printf("write error register mode_reg");
	}
	printf("write register:mode_reg");
	return 0;
}

double compass_get_angle()
{
  /* read X_MSB */
	short x = read_out(fd, x_msb_reg, x_lsb_reg);
	short y = read_out(fd, y_msb_reg, y_lsb_reg);
	short z = read_out(fd, z_msb_reg, z_lsb_reg);
	double angle = calc_compass_angle(x,y);
	printf("x:%d,y:%d,z:%d,angle:%f\n",x,y,z,angle);
	return angle;
}
