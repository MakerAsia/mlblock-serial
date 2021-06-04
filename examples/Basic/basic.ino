#include "MLBlockSerial.h"
MLBlockSerial mlblock(&Serial);

void OnResult(MLObject result){
  //==== see all struct in MLBlockSerial.h ====//
  //result.label [char array]
  //result.confidence [float 0-100]
  //result.type 0=no object, 1= classify, 2 = object detect with bounding box [ubyte]
  //result.x,res.y,res.width(center),res.height(center) [uint16_t]
  //result.id (same id = same frame (multiple object detected)) [uint16_t]
  //===========================================//
  if(strcmp(result.label,"person") == 0){
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mlblock.setOnResult(OnResult);
}

void loop() {
  mlblock.process();
}
