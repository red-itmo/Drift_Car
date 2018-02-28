
#ifndef SERIAL_PORT_HANDLER_H
#define SERIAL_PORT_HANDLER_H

template <uint8_t size_of_msg,uint8_t n_of_fields>
class serial_handler{


  uint8_t msg_buffer[size_of_msg];
  //the last element of array, that was added to buffer
  uint32_t last_access;
  //byte that we need to save, if buffer is already full
  uint8_t lost_byte;

  public:
    serial_handler(const int &speed):
    last_access(0)
    {
      Serial.begin(speed);
      this->reset_buffer();
    }

    ~serial_handler(){
      Serial.end();
    }



    void reset_buffer(void){
      for (uint8_t i = 0;i<size_of_msg;++i){
        this->msg_buffer[i] = -1;
      }

    }

    bool buffer_update(void){
      uint8_t incoming_byte = Serial.read();
      if(incoming_byte == -1){
        return (this->last_access < size_of_msg) ? false : true;
      }
      //if last append indx was < size_of_msg, then append msg to buf
      // and return false, because buffer not full
      if(this->last_access < size_of_msg){

        this->msg_buffer[last_access] = incoming_byte;
        ++last_access;
        return false;
      }

      return true;

    }

    float* get_msg(void){
      uint8_t temp_buff[size_of_msg];
      temp_buff = msg_buffer;
      this->reset_buffer();
      this->last_access = 0;
      return this->parse(temp_buff);
    }

    //parse bytes in buffer(and skip first, last, and separator bytes) and return array of floats
    float* parse(const uint8_t buf[size_of_msg]){
      float* answer[n_of_fields];
      uint8_t first_part[] = {buf[1],buf[2]};
      uint8_t second_part[] = {buf[4],buf[5]};
      for (uint8_t i=0; i<n_of_fields; i+=3){
        uint8_t temp[] = {buf[i+1],buf[i+2]};
        memcpy(&answer[i],&temp,sizeof(answer[i]));
      }
      return answer;
    }



};

#endif
