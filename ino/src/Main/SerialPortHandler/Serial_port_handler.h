#include <stdlib.h>
#ifndef SERIAL_PORT_HANDLER_H
#define SERIAL_PORT_HANDLER_H


template <uint8_t size_of_msg,uint8_t n_of_fields,HardwareSerial* serial>
class serial_handler{


  uint8_t msg_buffer[size_of_msg];
  //the last element of array, that was added to buffer
  uint32_t last_access;
  uint32_t speed;
  uint8_t max_float_size;

  public:
    serial_handler(const int &def_speed, const int &float_size):
    last_access(0), speed(def_speed), max_float_size(float_size)
    {
      this->reset_buffer();
    }

    ~serial_handler(){
      serial->end();
    }

    void init(void){
      serial->begin(this->speed);
    }


    void reset_buffer(void){
      for (uint8_t i = 0; i < size_of_msg; ++i){
        this->msg_buffer[i] = -1;
      }

    }

    bool buffer_update(void){
      uint8_t incoming_byte = serial->read();

      if(incoming_byte == -1){
        return (this->msg_buffer[this->last_access] == '>') ? true : false;
      }

      this->msg_buffer[this->last_access] = incoming_byte;
      this->last_access++;

      // if not end of msg return false
      if(incoming_byte != '>'){
        return false;

      }

      return true;

    }

    double* get_msg(void){
      return this->parse(msg_buffer);
    }


    //parse bytes in buffer(and skip first, last, and separator bytes) and return array of floats
    double* parse(const uint8_t buf[size_of_msg]){
      static double answer[n_of_fields];
      uint8_t counter_for_data = 0;
      uint8_t i = 0;
      char* end_of_value;
      const char* char_buf = reinterpret_cast<const char*>(buf);

      // skip all until we reach header

      while(char_buf[i] != '<' ){
        ++i;
      }

      ++i;
      answer[0] = strtod(&char_buf[i],NULL);
      while(char_buf[i] != ';'){
        ++i;
      }
      ++i;
      answer[1] = strtod(&char_buf[i], NULL);

      //reset last_access indx to 0 and all values of buffer to -1
      this->last_access = 0;
      this->reset_buffer();
      return answer;
    }
};

#endif
