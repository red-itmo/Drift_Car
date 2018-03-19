#include "Driver.h"
#include "Regulator.h"
#include "serial_port_handler.h"
#include "Error_codes.h"

//Driver.h
Driver D;

//Regulator.h
const byte interruptPin = 2;

//serial_port_handler.h
bool if_msg_ended;
double * msg;
serial_handler<15,2,&Serial> handler(9600,64);


void setup() {
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
	}
// Serial.println("loop");

	//	msg_back = "<" + str(engine_velocity) + ";" + str(dynamixel_alpha or dynamixel_velocity) + ";" + str(current_time) + ";" + str(error_code) + ">";

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
