#include "gps.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "nmea.h"
#include "serial.h"

static const int MAXIMUM_BUFFER_SIZE = 256;

extern void gps_init(void)
{
	serial_init();
	serial_config();
	//Write commands
}

extern void gps_on(void)
{
	//Write on
}

// Compute the GPS location using decimal scale
extern int gps_location(loc_t *coord)
{
	if(gps_avail()>MAXIMUM_BUFFER_SIZE)
	{
		serial_flush();
	}

	uint8_t status = _EMPTY;
	while(status != _COMPLETED)
	{
		gpgga_t gpgga;
		gprmc_t gprmc;
		char buffer[256];

		if(serial_readln(buffer, 256)==-1)
		{
			coord->time = 0;
			coord->latitude = 0;
			coord->longitude = 0;
			coord->altitude = 0;
			coord->speed = 0;
			coord->course = 0;
			return -1;
		}
		else
		{
			switch (nmea_get_message_type(buffer))
			{
			case NMEA_GPGGA:
				nmea_parse_gpgga(buffer, &gpgga);

				gps_convert_deg_to_dec(&(gpgga.latitude), gpgga.lat, &(gpgga.longitude), gpgga.lon);

				coord->latitude = gpgga.latitude;
				coord->longitude = gpgga.longitude;
				coord->altitude = gpgga.altitude;

				status |= NMEA_GPGGA;
				break;
			case NMEA_GPRMC:
				nmea_parse_gprmc(buffer, &gprmc);
				coord->time = gprmc.time;
				coord->speed = gprmc.speed;
				coord->course = gprmc.course;

				status |= NMEA_GPRMC;
				break;
			}
		}
	}
	return 0;
}

extern void gps_flush(void)
{
	serial_flush();
}

extern int gps_avail(void)
{
	int i = serial_data_avail();
	printf("the number of bytes of data avalable is %d\n",i);
	return i;
}

extern void gps_off(void) {
	//Write off
	serial_close();
}

// Convert lat e lon to decimals (from deg)
void gps_convert_deg_to_dec(double *latitude, char ns,  double *longitude, char we)
{
	double lat = (ns == 'N') ? *latitude : -1 * (*latitude);
	double lon = (we == 'E') ? *longitude : -1 * (*longitude);

	*latitude = gps_deg_dec(lat);
	*longitude = gps_deg_dec(lon);
}

double gps_deg_dec(double deg_point)
{
	double ddeg;
	double sec = modf(deg_point, &ddeg)*60;
	int deg = (int)(ddeg/100);
	int min = (int)(deg_point-(deg*100));

	double absdlat = round(deg * 1000000.);
	double absmlat = round(min * 1000000.);
	double absslat = round(sec * 1000000.);

	return round(absdlat + (absmlat/60) + (absslat/3600)) /1000000;
}
