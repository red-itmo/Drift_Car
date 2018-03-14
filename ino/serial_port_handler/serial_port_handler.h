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
      if(incoming_byte!='>'){
        return false;

      }

      return true;

    }

    double* get_msg(void){
      uint8_t temp_buff[size_of_msg];
      memcpy(&temp_buff, &msg_buffer, sizeof(temp_buff));
      this->reset_buffer();
      return this->parse(temp_buff);
    }


    //parse bytes in buffer(and skip first, last, and separator bytes) and return array of floats
    double* parse(const uint8_t buf[size_of_msg]){
      serial->flush();
      static double answer[n_of_fields];
      char char_buff[this->last_access - 1];
      char value[this->max_float_size];
      uint8_t counter;
      uint8_t counter_for_data = 0;

      for(uint8_t i = 0; i < max_float_size; i++){
        value[i] = '0';
      }

      for(uint8_t i = 0; i < this->last_access; ++i){
        char_buff[i] = buf[i];
      }

      for (uint8_t i = 0; i < this->last_access; ++i){
        if(char_buff[i] == '<')
          continue;
        counter = 0;
        while(char_buff[i] != ';' && char_buff[i] != '>')
        {
          if(char_buff[i] != ' ' && char_buff[i] != '\n'){
            value[counter] = char_buff[i];
            counter++;
          }
          i++;
        }
        answer[counter_for_data++] = strtod(value,NULL);
        for(uint8_t i=0; i < max_float_size; i++){
          value[i] = '0';
        }
        if(char_buff[i] == '>'){
          break;
        }
      }
      this->last_access = 0;

      return answer;
    }
};

#endif
