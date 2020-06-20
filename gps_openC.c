//This is the STAND-ALSONE version of a gps driver for a certain multirotor, but may work on others as well, if you want more information please message v.i.kiriouchine*at*alumnus.tue.nl
// 2017 - 2020, Vsevolod I. Kiriouchine , https://github.com/Kiriouchine/
//Also include, arduino-serial-lib.c and .h in same folder 
//arduino-serial-lib by T.Kurt from https://github.com/todbot/arduino-serial/  by 2006-2013, Tod E. Kurt, http://todbot.com/blog/ under the MIT Licence 

//This stand-alone version can be put on the drone via FTP or SCP software onto the drone, 
//Monitor the outputs via a Telnet program like PuTTY on ip 192.168.1.1 and port 23 or 5555

//Compile this program manually with a gcc compiler like: 
	// arm-none-linux-gnueabi-gcc -Wall gps_openC.c c:/Users/vsevolod/arduino-serial-lib.c -o gpsOpenC.elf
//your linux working file is now gpsOpenC.elf 

	
//to run this in command line via PuTTY go to the update folder:
	// cd update/ 
	// ls 
	// chmod 777 gpsOpenC.elf 
	// ./gpsOpenC.elf  

// re-run this program for better results often 

//For fast reading you can configure the GPS module to only send GNGLL messages at 5Hz for higher accuracy and estimation

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>

#include "arduino-serial-lib.h" //header of arduino-serial-lib library 

int GPS_fd;
char testChar = 'P';
char stringRead[100];
int stringIndex; 
int stopInt;  
char tag[10];

int main()
{
	// arm-none-linux-gnueabi-gcc -Wall gps_openC.c c:/Users/vsevolod/arduino-serial-lib.c -o gpsOpenC.elf
	// chmod 777 gpsOpenC.elf 
	// ./gpsOpenC.elf  
	//int main(int argc, char **argv)
	
	GPS_fd = serialport_init("/dev/ttyUSB0", 9600); //using arduino-serial-lib library  . set to 4800 or 9600 BAUD dependant onyour GPS module setting
	
	
    printf("open errno code is  = %d \n", errno);
	 printf("GPS_fd file descriptor is  = %d \n", GPS_fd);

	char tmp; 
	// char tagEnd;
	int tmp1; 
	
	 int i =0;
	size_t nbytes;
	nbytes = sizeof(tmp);
	printf("nbytes to read is  = %d \n", nbytes); 
	
	//do { //do executes at least one, before checking while, thus read is executed at least one regardless of datas initialization 
	//ssize_t datas = read (GPS_fd, &tmp, 1); // read from file descriptor GPS_fd and put in buffer tmp (with length 'sizeof tmp') datas returns size of read bytes?   
	
	ssize_t datas = read (GPS_fd, &tmp, 1);
	
	printf("					the read char tmp is = %c \n", tmp);
	
	stringIndex = 0;
	stopInt = 0;
	strcpy(stringRead, "");
	int lenst = 0; 
	char endst; 
	int stopMark = 0; 
	 
	//while( tmp1 != '$' && stopInt < 400) {
		
		//printf("$ sign not found, found =  %c , %d \n", tmp, stopInt); 
		
		//ssize_t datas = read (GPS_fd, &tmp, 1);
		
		//tmp1 = fgetc(GPS_fd); 
		
	do{
		
		while(tmp != '$')
			datas = read (GPS_fd, &tmp, 1);
		
		
		
		do{ //Carrige return CR is 0x0d
			datas = read (GPS_fd, tag, 5);
			
			// if (datas != 0){
				// tagEnd = tag[datas - 1];
				// printf("$tagEnd =%c \n", tagEnd);
			// }
			
			tag[datas] = '\0';
			
			if (datas != 0){
					//printf("$tag found =%s= number=%d \n", tag, datas); 
				
				strcat(stringRead, tag);
				strcpy(tag, "");
				
				lenst =  strlen(stringRead);
				endst = stringRead[lenst - 1];
				
			}
			

			stopInt++; 
			
			
	}while(endst != '*' && lenst < 100);
	
	//printf("%s\n", stringRead); 
	
	double longiD;
	double longiM;
	double latiD;
	double latiM;
	
	char longistD[3];
	char longistM[9];
	double longiNum;
	
	char latistD[4];
	char latistM[9];
	double latiNum;
	
	char longist[11];
	char latist[12]; 
	
	if( stringRead[0] == 'G' && stringRead[1] == 'N' && stringRead[2] == 'G' && stringRead[3] == 'L' && stringRead[4] == 'L'){
		
		printf("%s\n", stringRead); 
		
		printf("GNGLL string found \n");  
		
		/*extract longitude from GNGLL*/  
		strncpy(longist, stringRead+6, 10);
		
		longist[11] = '\0';
		
		printf("%s\n", longist);
		
		/* get degrees of longitude*/ 
		strncpy(longistD, longist, 2);
		
		longistD[3] = '\0';
		
		printf("%s\n", longistD); 
		
		longiD = atof(longistD);
		
		printf("longi Degree %f \n", longiD); 
		
		/* get minutes of longitude*/ 
		strncpy(longistM, longist+2, 8);
		
		longistM[9] = '\0';
		
		printf("%s\n", longistM); 
		
		longiM = atof(longistM);
		
		printf("longi Minute %f \n", longiM); 
		
		/* Longitude total degrees*/ 
		longiNum = longiD + longiM/60; 
		printf("GPS longitude = %f \n", longiNum); 
		
		
		
		/*extract latitude from GNGLL*/  
		strncpy(latist, stringRead+19, 11);
		
		latist[12] = '\0';
		
		printf("%s\n", latist);
		
		/* get degrees of latitude*/ 
		strncpy(latistD, latist, 3);
		
		latistD[4] = '\0';
		
		printf("%s\n", latistD); 
		
		latiD = atof(latistD);
		
		printf("lati Degree %f \n", latiD); 
		
		/* get minutes of latitude*/ 
		strncpy(latistM, latist+3, 8);
		
		latistM[9] = '\0';
		
		printf("%s\n", latistM); 
		
		latiM = atof(latistM);
		
		printf("lati Minute %f \n", latiM); 
		
		
		/* Latitude total degrees*/ 
		latiNum = latiD + latiM/60; 
		printf("GPS latitude = %f \n", latiNum); 
		
		
		
		stopMark = 1; 
	}
	
	
	strcpy(stringRead, "");
	lenst = 0;
	usleep(10000);
	
	
	}while(stopMark == 0); 
	
	stopInt = 0;
	
	return 0;
}