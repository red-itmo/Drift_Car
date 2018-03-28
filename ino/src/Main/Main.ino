#include "Engine/Driver.h"
#include "Engine/Regulator.h"
#include "SerialPortHandler/Serial_port_handler.h"
#include "AX12A.h"
#include "Error_codes.h"
#define DirectionPin  (2u)
#define BaudRate      (100000ul)
#define ID            (1u)


//Driver.h
Driver D;
int reg = 0;
//Regulator.h
const byte interruptPin = 2;

//serial_port_handler.h
bool if_msg_ended;
double * msg;
serial_handler<64, 2, &Serial> handler(9600, 128);

//msg vars
uint16_t dynamixel_angle;
uint8_t error_code;

void setup() {
  
  ax12a.begin(BaudRate, DirectionPin, &Serial3);
	pinMode(interruptPin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(interruptPin), compute_v, FALLING);

	time = millis();
 	handler.init();
 	D.init();
	char* msg_back;
}

void loop(){
	error_code = ERR_ALL_FINE;
	// convert all data to str and send back
  //send_back(trans_vel, dynamixel_angle, error_code);
}

void send_back(int vel, int dynamixel_angle, int error_code){
    // convert all data to str and send back
  char msg_back_buffer_1[50],msg_back_buffer_2[50],msg_back_buffer_3[50],msg_back_buffer_4[50];
  sprintf(msg_back_buffer_1, "%d", vel);
  sprintf(msg_back_buffer_2, "%d", dynamixel_angle);
  //TODO: millis() or cur_time ??
  sprintf(msg_back_buffer_3, "%d", millis());
  sprintf(msg_back_buffer_4, "%d", error_code);

  char* msg_back[] = {"<",msg_back_buffer_1,";",msg_back_buffer_2,";",
                          msg_back_buffer_3,";",msg_back_buffer_4,">"};

  for(char* msg_part: msg_back){
    Serial.print(msg_part);
  }
   Serial.print("\n");
  
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
   ax12a.move(ID,(int)msg[1]);

 }
}

