#ifndef segmentLcd_h
#define segmentLcd_h
#include "Arduino.h"

class segmentLcd
{
public:
	//public functions
	void begin(int csPin, int wrPin, int dataPin); //initiate the LCD
	void clear();	//Clear all number on LCD
	void print(long number); //diplay integer number
	void printFloat(float number); //display floating number
	void printTime(int firstNum, int secondNum); //dipslay the time with colon
	void printC(float number); //display the temperature in C with floating number
	void printF(float number); //display the temperature in F with floating number

private:
	//private functions
	void wrCMD(byte command); //sending command to LCD
	void update(int type); //sending data to LCD. Type = 1; send integer number; type =2 send floating number; type =3 sending time format; type =4 send temp in C.
	int dotPos(float number);
	
	//private variable
	char localBuffer[4]={}; //store the mapping segment number
	byte dispNumArray[4]={}; //store the data that will use to send to LCD RAM
	byte bufferMem;
	int _csPin;
	int _wrPin;
	int _dataPin;
	int _dotPosVal;
};
#endif