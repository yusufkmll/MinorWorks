#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//char GPS[69] = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";

typedef struct GpsDataStruct {
	float GPS_latitude, GPS_longtitude, GPS_speed;
	char prefix[8];
	int veri1;
	char veri4;
	char veri5;
	char veri6;
	float veri7;
	int veri8;
	float veri9;
	char veri10[5];
}GpsDataStruct;

GpsDataStruct readGPS(char*);
char* readUntilComma(char*, int);
