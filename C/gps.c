/*
 * tampon.c
 *
 *  Created on: 9 Oca 2021
 *      Author: Yusuf
 */


#include "gps.h"

//char GPS[69] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";

char data[10],temp[10];
char *str = data;
int i = 0;

GpsDataStruct readGPS(char* gpsRawData) {
	GpsDataStruct gpsVeri;
    str = readUntilComma(gpsRawData, 1);
    if (!strcmp(str, "GPRMC")) {
        str = readUntilComma(gpsRawData, 3);// get the 3rd string which is Latitude
        gpsVeri.GPS_latitude = atof(str);
        str = readUntilComma(gpsRawData, 2);// get the 2nd string after Latitude which is longtitude
        gpsVeri.GPS_longtitude = atof(str);
        str = readUntilComma(gpsRawData, 2);// get the 2nd string after longtitude which is speed
        gpsVeri.GPS_speed = atof(str);
        i = 0;
        return gpsVeri;
    }
    else {
    	GpsDataStruct hataliVeri;
    	hataliVeri.GPS_latitude = 0;
    	hataliVeri.GPS_longtitude = 0;
    	hataliVeri.GPS_speed = 0;
    	return hataliVeri;
    }
}

char *readUntilComma(char *string, int times) {        // reads numerous times of data from string  until comma
    int n;
    int count = 0;
    for (n = 0;n < times;n++) {
        count = 0;
        while (string[i] != ',') {
            temp[count] = string[i];
            i++;
            count++;
        }
        i++;
    }
    return temp;
}

GpsDataStruct gps_oku(char* gps_rawdata)
{
	GpsDataStruct gps_data;
	sscanf(gps_rawdata, "%s,%d,%c,%f,%c,%f,%c,%f,%f,%d,%s",
						gps_data.prefix,
						&gps_data.veri1, 
						&gps_data.veri4,
						&gps_data.GPS_latitude,
						&gps_data.veri5,
						&gps_data.GPS_longtitude,
						&gps_data.veri6,
						&gps_data.GPS_speed,
						&gps_data.veri7,
						&gps_data.veri8,
						gps_data.veri10
						);
	return gps_data;
}

int main() {
	GpsDataStruct gpsVeri;
    gpsVeri = readGPS("GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A");
	printf("Lattitude: %f\n",gpsVeri.GPS_latitude);
    printf("Longtitude: %f\n",gpsVeri.GPS_longtitude);
    printf("Speed: %f\n",gpsVeri.GPS_speed);
	return 0;
}

