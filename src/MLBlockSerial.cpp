#include "MLBlockSerial.h"

MLBlockSerial::MLBlockSerial(Stream *serial){
  this->serial = serial;
}

void MLBlockSerial::setOnResult(void (*onResultCallback)(MLObject)){
  _onResultCallback = onResultCallback;
}

void MLBlockSerial::process(){
  if(this->serial->available() >= 4){
    uint8_t start = this->serial->read();
    if(start == MLB_START_CHAR){
      uint16_t hiLen = this->serial->read();
      uint16_t loLen = this->serial->read();
      uint16_t packetLen = ((uint16_t)hiLen << 8) | loLen;
      if(packetLen > MLB_MAX_LENGTH){
        return;
      }
      uint8_t buff[packetLen - 3]; 
      size_t buffSize = this->serial->readBytes(buff,packetLen - 3); //minus 4 bytes,start(1),packetlen(2)
      if(buffSize != (packetLen - 3)){
        return; //readed not eq same size
      }
      if(buff[buffSize - 1] != MLB_END_CHAR){
        return; // ending not match
      }
      uint8_t checkSum = 0x00;
      checkSum ^= MLB_START_CHAR;
      checkSum ^= hiLen;
      checkSum ^= loLen;
      for(int i=0;i<buffSize - 2; i++){ //2 end bytes checksum and MLB_END_CHAR not included
        checkSum ^= buff[i];
      }
      if(checkSum != buff[buffSize - 2]){
        return;
      }
      MLObject result;
      result.type     = buff[0];
      result.id       = ((uint16_t)buff[1] << 8) | buff[2];
      result.classId  = ((uint16_t)buff[3] << 8) | buff[4];
      uint16_t labelLen = ((uint16_t)buff[5] << 8) | buff[6];
      memcpy(&result.label,&buff[7],labelLen + 1);
      result.label[labelLen] = 0x0; //str terminate
      uint16_t nextPos = 7 + labelLen;
      result.confidence = *(float *)&buff[nextPos];
      nextPos += 4;
      if(result.type == 2){
        result.x = ((uint16_t)buff[nextPos] << 8) | buff[nextPos+1];
        result.y = ((uint16_t)buff[nextPos+2] << 8) | buff[nextPos+3];
        result.width = ((uint16_t)buff[nextPos+4] << 8) | buff[nextPos+5];
        result.height = ((uint16_t)buff[nextPos+6] << 8) | buff[nextPos+7];
      }
      result.checkSum = checkSum;
//      Serial.println("==");
//      Serial.println(result.type);
//      Serial.println(result.id);
//      Serial.println(result.classId);
//      Serial.println(result.label);
//      Serial.println(result.confidence);
//      Serial.println(result.x);
//      Serial.println(result.y);
//      Serial.println(result.width);
//      Serial.println(result.height);
//      Serial.println(result.checkSum);
//      Serial.println("==");
      if(this->_onResultCallback){
        _onResultCallback(result);
      }
    }
  }
}
