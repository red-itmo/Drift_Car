
#ifndef SERIAL_PORT_HANDLER_H
#define SERIAL_PORT_HANDLER_H


template <uint8_t size_of_msg,uint8_t n_of_fields,HardwareSerial* serial>
class serial_handler{


  uint8_t msg_buffer[size_of_msg];
  //the last element of array, that was added to buffer
  uint32_t last_access;
  //byte that we need to save, if buffer is already full
  uint8_t lost_byte;
  uint32_t speed;

  public:
    serial_handler(const int &def_speed):
    last_access(0),speed(def_speed)
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
      for (uint8_t i = 0;i<size_of_msg;++i){
        this->msg_buffer[i] = -1;
      }

    }

    bool buffer_update(void){
      uint8_t incoming_byte = serial->read();
      if(incoming_byte == -1){
        serial->print("a");
        return ((char)&this->msg_buffer[this->last_access] == ">") ? true : false;
      }
      //if last append indx was < size_of_msg, then append msg to buf
      // and return false, because buffer not full
      //serial->print((char)incoming_byte);
      this->msg_buffer[this->last_access] = incoming_byte;
      this->last_access++;
      //serial->print("i am in upd");
      if((char)incoming_byte!='>'){

        serial->print("n");
        return false;

      }
      serial->print("e");
      return true;

    }

    float* get_msg(void){
      uint8_t temp_buff[size_of_msg];
      memcpy(&temp_buff, &msg_buffer, sizeof(temp_buff));
      //temp_buff = msg_buffer;
      this->reset_buffer();
      serial->print("g");
      //this->last_access = 0;
      return this->parse(temp_buff);
    }

    //parse bytes in buffer(and skip first, last, and separator bytes) and return array of floats
    float* parse(const uint8_t buf[size_of_msg]){
      static float answer[n_of_fields];
      char* char_buff;
      char* value;
      uint8_t counter;
      uint8_t counter_for_data=0;
      //serial->print("p");
      //now cast to list of chars
      char_buff = (char*)buf;
      for (uint8_t i=0; i<this->last_access; ++i){
        if(char_buff[i] == '<')
        {
          continue;
        }
        counter = 0;
        while(char_buff[i+counter] != ';' && char_buff[i+counter] != '>')
        {
          value[counter] = char_buff[i+counter];
          serial->println(value[counter]);
          ++counter;

        }
        i+=counter;
        answer[counter_for_data++] = atof(value);

        if(char_buff[i] == '>'){
          break;
        }
      }
      serial->println(answer[0]);
      serial->println(answer[1]);
      this->last_access = 0;

      return answer;
    }



};

#endif
