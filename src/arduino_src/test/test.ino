#include "serial_port_handler/serial_port_handler.h"

bool if_msg_ended;
float* msg;
serial_handler<15,2,&Serial> handler(9600);
 

void setup() {
 handler.init();  
}


void loop() {
  // print the string when a newline arrives:
   //handler.serial_print();
  //Serial.write("<1.5;2.6>\n");
}



void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    if_msg_ended = handler.buffer_update();
    if(if_msg_ended){
      
      //Serial.print("a");
      msg = handler.get_msg();
      //Serial.print(msg[0]);
      //Serial.print(msg[1]);
    }
  }
}
