#include "Engine/Driver.h"
#include "Engine/Regulator.h"
#include "SerialPortHandler/Serial_port_handler.h"
#include "DynamixelSerial/DynamixelSerial.h"
#include "DynamixelSerial/DynamixelSerial.cpp"
#include "Error_codes.h"

//Driver.h
Driver D;

//Regulator.h
const byte interruptPin = 2;

//serial_port_handler.h
bool if_msg_ended;
double * msg;
serial_handler<15,2,&Serial> handler(9600,64);

//msg vars
char msg_back_buffer_1[50],msg_back_buffer_2[50],msg_back_buffer_3[50],msg_back_buffer_4[50];
uint16_t dynamixel_angle;
uint8_t error_code;

void setup() {
	Dynamixel.begin(100000, 2);
	delay(1000);
	pinMode(interruptPin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPin), compute_v, FALLING);
	time = millis();
 	handler.init();
 	D.init();
	char* msg_back;
}

void loop(){
	if(if_msg_ended){
		/*
		*
		*	DYNAMIXEL CODE GOES HERE
		*
		*/
		//If we send angle
		Dynamixel.moveSpeed(1,(int)msg[1], 400);
	}
	error_code = ERR_ALL_FINE;
	//dynamixel_angle = Dynamixel.readPosition(1);

	// convert all data to str and send back
	sprintf(msg_back_buffer_1, "%d", trans_vel);
	sprintf(msg_back_buffer_2, "%d", dynamixel_angle);
	//TODO: millis() or cur_time ??
	sprintf(msg_back_buffer_3, "%d", millis());
	sprintf(msg_back_buffer_4, "%d", error_code);

	char* msg_back[] = {"<",msg_back_buffer_1,";",msg_back_buffer_2,";",
													msg_back_buffer_3,";",msg_back_buffer_4,">"};

//	for(char* msg_part: msg_back){
//		Serial.print(msg_part);
//	}


}
//
void serialEvent(){
 if_msg_ended = handler.buffer_update();
 if(if_msg_ended){
   //get the msg from serial port
   msg = handler.get_msg();
   //Send data to engine driver
   Serial.println(msg[0]);
   Serial.println(msg[1]);
   //UNCOMMENT WHEN ENCODER IS ON
   //D.send(get_Up(msg[0]));
   D.send(msg[0]);

 }
}
