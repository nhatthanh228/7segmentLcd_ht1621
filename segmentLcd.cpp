//tran nhat thanh
//version 1.3 fixed the error when display time format in previous version 1.2
//version 1.3 got issue when switching from time to integer number (fixed)
//version 1.3 got issue when use float number with 4 number behind dot signal

#include <Arduino.h>
#include "segmentLcd.h"

//============DEFINE=============
#define  BIAS     0x52             //0b1000 0101 0010 
#define  SYSDIS   0X00             //0b1000 0000 0000
#define  SYSEN    0X02             //0b1000 0000 0010
#define  LCDOFF   0X04             //0b1000 0000 0100
#define  LCDON    0X06             //0b1000 0000 0110
#define  XTAL     0x28             //0b1000 0010 1000
#define  RC256    0X30             //0b1000 0011 0000
#define  TONEON   0X12             //0b1000 0001 0010
#define  TONEOFF  0X10             //0b1000 0001 0000
#define  WDTDIS1  0X0A             //0b1000 0000 1010

#define TRACE1 //this is for debuging

//initiate the LCD.
void segmentLcd::begin(int csPin, int wrPin, int dataPin){
	//====TRACE====
	#ifdef TRACE
		Serial.begin(9600);
		Serial.println("====begin method====");
	#endif
	
	pinMode(csPin, OUTPUT);
	pinMode(wrPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	_csPin = csPin;
	_wrPin = wrPin;
	_dataPin = dataPin;	
	
	//setup the LCD
	#ifdef TRACE
		Serial.println("====setup BIAS====");
	#endif
	wrCMD(BIAS);
	#ifdef TRACE
		Serial.println("====setup SYSDIS====");
	#endif
	wrCMD(SYSDIS);
	#ifdef TRACE
		Serial.println("====setup SYSEN====");
	#endif
	wrCMD(SYSEN);
	#ifdef TRACE
		Serial.println("====setup RC256====");
	#endif
	wrCMD(RC256);
	#ifdef TRACE
		Serial.println("====setup WDTDIS1====");
	#endif
	wrCMD(WDTDIS1);
	#ifdef TRACE
		Serial.println("====setup LCDON====");
	#endif
	wrCMD(LCDON);
}

//sending command to LCD.
void segmentLcd::wrCMD(byte command){
	digitalWrite(_csPin, LOW);
	uint16_t cmd = 0x800;
	cmd = cmd | command;
	
	#ifdef TRACE
		Serial.println("====wrCMD method====");
		Serial.print("sending data to HT1621: "); Serial.print(cmd, BIN); 
		Serial.print(" - "); Serial.print ("0x"); Serial.println(cmd, HEX);
		Serial.print("Actual data send to HT1621: ");
	#endif
	
	for (int i =0; i <12; i ++){
		digitalWrite(_wrPin, LOW);
		if (cmd & 0x800){
			digitalWrite(_dataPin, HIGH);
			
			#ifdef TRACE
				Serial.print('1');
			#endif
		}
		else {
			digitalWrite(_dataPin, LOW);
			
			#ifdef TRACE
				Serial.print('0');
			#endif
		}
		cmd <<=1;
		digitalWrite(_wrPin, HIGH);
	}
	
	#ifdef TRACE
		Serial.println();
		Serial.println();
	#endif
	
	digitalWrite(_csPin, HIGH);
}

//Clear all number on LCD.
void segmentLcd::clear(){
	uint16_t cmd = 0x140;
	#ifdef TRACE
		Serial.println("====clear method=====");
		Serial.print("sending data to HT1621: "); Serial.print(cmd, BIN); 
		Serial.print(" - "); Serial.print ("0x"); Serial.println(cmd, HEX);
		Serial.print("Actual data send to HT1621: ");
	#endif
	
	digitalWrite(_csPin, LOW);
	for (int i =0; i<9; i++){
		digitalWrite(_wrPin, LOW);
		if (cmd & 0x100){
			digitalWrite(_dataPin, HIGH);
			#ifdef TRACE
				Serial.print('1');
			#endif
		}
		else {
			digitalWrite(_dataPin, LOW);
			#ifdef TRACE
				Serial.print('0');
			#endif
		}
		cmd <<=1;
		digitalWrite(_wrPin, HIGH);
	}
	#ifdef TRACE
		Serial.print(" ");
	#endif
	
	for (int i =0 ; i<32; i++){
		digitalWrite(_wrPin, LOW);
		digitalWrite(_dataPin, LOW);
		#ifdef TRACE
			Serial.print('0');
		#endif
		digitalWrite(_wrPin, HIGH);
	}
	digitalWrite(_csPin, HIGH);
	#ifdef TRACE
		Serial.println();
		Serial.println();
	#endif
}

//diplay integer number.
void segmentLcd::print(long number){
	#ifdef TRACE
		Serial.println("====print integer method====");
		Serial.print("number entered: "); Serial.println(number);
	#endif
	
	//check the condition.
	if (number > 9999){
		number = 9999;
	}
	if (number < -999){
		number = -999;
	}
	
	snprintf(localBuffer, 5, "%4li", number); //convert to char.
	
	//check the data structure after store in localBuffer array.
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =0 ; i <4;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
	#endif
	
	update(1);
}
//display Floating number.
void segmentLcd::printFloat(float number){
	
	#ifdef TRACE
		Serial.println("====print float method====");
		Serial.print("number entered: "); Serial.println(number);
	#endif
	
	//check the condition
	if(number > 99.99){
		number = 99.99;
	}
	if(number < -9.99){
		number = -9.99;
	}	
	
	dtostrf(number,5,2,localBuffer);//convert to string
	
	//check the data structure after store in localBuffer array.
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =0 ; i <5;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
		Serial.println();
	#endif
	
	//find dot position
	for (int i =0; i<5;i++){
		if(localBuffer[i] == '.'){
			_dotPosVal = i;
			#ifdef TRACE
				Serial.print("dot Position @: ");
				Serial.println(_dotPosVal);
			#endif
		}
	}
	
	//remove "." from string
	for (int i =_dotPosVal; i< 5; i ++){
		localBuffer[i] = localBuffer[i+1];
	}
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =0 ; i <5;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
	#endif
	update(2);
}

//display time format.
void segmentLcd::printTime(int firstNum, int secondNum){
	#ifdef TRACE
		Serial.println("====print time method====");
	#endif
	
	//check the condition.
	if (firstNum > 99){
		firstNum = 99;
	}
	if (secondNum > 99){
		secondNum = 99;
	}

	long tempNum = firstNum * 100 + secondNum;
	#ifdef TRACE
		Serial.print("time: "); Serial.println(tempNum);
	#endif
	
	//snprintf(localBuffer, 5, "%4li", tempNum); //convert to char.
	snprintf(localBuffer,5, "%04d", tempNum);
	
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =0 ; i <4;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
	#endif
	
	update(3);


}

//display temperature in C
void segmentLcd::printC(float number){
	#ifdef TRACE
		Serial.println("====print temperature in C method====");
		Serial.print("number entered: "); Serial.println(number);
	#endif
	
	dtostrf(number,5,3,localBuffer);//convert to string
	
	//check the data structure after store in localBuffer array.
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =1 ; i <5;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
		Serial.println();
	#endif
	//find dot position
	for (int i =0; i<5;i++){
		if(localBuffer[i] == '.'){
			_dotPosVal = i;
			#ifdef TRACE
				Serial.print("dot Position @: ");
				Serial.println(_dotPosVal);
			#endif
		}
	}
	
	//remove "." from string
	for (int i =_dotPosVal; i< 5; i ++){
		localBuffer[i] = localBuffer[i+1];
	}
	#ifdef TRACE
		Serial.print("Number after converted: ");
		for (int i =0 ; i <5;i ++){
			if (localBuffer[i]){
				Serial.print(localBuffer[i]);
				Serial.print(",");
			}
			else {
				Serial.print("|");
			}
		}
		Serial.println();
	#endif
	update(4);
}

void segmentLcd::printF(float number){
}

//update the LCD segment
void segmentLcd::update(int type){
	//clear array 
	for (int i =0; i< 4; i ++){
		dispNumArray[i] =0;
	}
	
	#ifdef TRACE
		Serial.println("====update method====");
	#endif
	
	//maping number to lcd segment.
	for (int i = 0 ; i < 4; i ++){
		switch(localBuffer[i]){
		  case '0':
			dispNumArray[i] = 0xFC;
			break;
		  case '1':
			dispNumArray[i] = 0x60;
			break;
		  case '2':
			dispNumArray[i] = 0xDA;
			break;  
		  case '3':
			dispNumArray[i] = 0xF2;
			break;
		  case '4':
			dispNumArray[i] = 0x66;
			break;
		  case '5':
			dispNumArray[i] = 0xB6;
			break;
		  case '6':
			dispNumArray[i] = 0xBE;
			break;
		  case '7':
			dispNumArray[i] = 0xE0;
			break;
		  case '8':
			dispNumArray[i] = 0xFE;
			break;
		  case '9':
			dispNumArray[i] = 0xF6;
			break;
		  case '-':
			dispNumArray[i] = 0x02;
			break;
		}
	}	
	
	switch (type){
		case 1: //for integer number
			#ifdef TRACE
				Serial.println("update integer number");
			#endif
			//nothing to do in this block when number is integer.
			break;
			
		case 2: //for floating number
			#ifdef TRACE
				Serial.println("update floating number");
			#endif
			dispNumArray[_dotPosVal-1] |= 0x01;			
			break;
			
		case 3: //for time format
			#ifdef TRACE
				Serial.println("update time format");
			#endif
			dispNumArray[1] |= 0x01;
			dispNumArray[2]	|= 0x01;
			break;
			
		case 4: //for temp C format
			#ifdef TRACE
				Serial.println("update temperature in C");
			#endif
			dispNumArray[_dotPosVal-1] |= 0x01;	
			dispNumArray[3] = 0x9D; //print *C
			break;		
		
		case 5:
			#ifdef TRACE
				Serial.println("update temperature in F");
			#endif
			dispNumArray[3] = 0x8F; //print *F
			break;
	}
	#ifdef TRACE
		Serial.print("sending command + address to HT1621: ");
	#endif
	
	
	//sending 101 to inform HT1621 that MCU send data then follow by address 000000.
	uint16_t cmd =0x140; 
	digitalWrite(_csPin, LOW);
	for(int i = 0 ;i < 9;i ++){
		digitalWrite(_wrPin, LOW);
		if (cmd & 0x100){
			digitalWrite(_dataPin, HIGH);
			#ifdef TRACE
				Serial.print('1');
			#endif
		}
		else {
		  digitalWrite(_dataPin, LOW);
			#ifdef TRACE
				Serial.print('0');
			#endif
		}
		cmd <<=1;
		digitalWrite(_wrPin, HIGH);
	}
	#ifdef TRACE
		Serial.println();
	#endif	
	
	#ifdef TRACE
		Serial.println("sending data to HT1621:");
	#endif
	
	//sending 32 bit of segment to LCD RAM.
	for (int i =0; i < 8; i++){
		for (int j =0; j<4;j++){
			digitalWrite(_wrPin,LOW);
			bufferMem =dispNumArray[j]<<i;
			if (bufferMem & 0x80){
				digitalWrite(_dataPin, HIGH);
				#ifdef TRACE
					Serial.print('1');
				#endif
			}
			else {
				digitalWrite(_dataPin, LOW);
				#ifdef TRACE
					Serial.print('0');
				#endif				
			}
			digitalWrite(_wrPin, HIGH);
		}
		#ifdef TRACE
			Serial.println();
		#endif
	}
	digitalWrite(_csPin, HIGH);
	
}

