#include "Driver.h"
#include "Regulator.h"
#include "serial_port_handler.h"

//Driver.h
Driver D;

//Regulator.h
const byte interruptPin = 2;

//serial_port_handler.h
bool if_msg_ended;
double * msg;
serial_handler<15,2,&Serial> handler(9600);
 

void setup() {
	Serial.begin(9600);
  	pinMode(interruptPin, INPUT_PULLUP);
  	attachInterrupt(digitalPinToInterrupt(interruptPin), compute_v, FALLING);
  	time = millis();
 	handler.init();
 	D.init(); 
}

void loop(){}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    if_msg_ended = handler.buffer_update();
    if(if_msg_ended){
      msg = handler.get_msg();
      D.parse(get_Up(msg[0]));
    }
  }
}
