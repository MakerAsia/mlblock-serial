#ifndef _MLBLOCKSERIAL_H_
#define _MLBLOCKSERIAL_H_
#include <Arduino.h>
#include <Stream.h>
#define MLB_MAX_LENGTH 512
#define MLB_START_CHAR 0x24
#define MLB_END_CHAR 0xFE
struct __attribute__((__packed__)) MLObject {
  uint8_t type;
  uint16_t id;
  uint16_t classId;
  char label[256];
  float confidence;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint8_t checkSum;
};

class MLBlockSerial {
  public:
    MLBlockSerial(Stream *serial = &Serial);
    void setOnResult(void (*onResultCallback)(MLObject));
    void process(void);
  private:
    void (*_onResultCallback)(MLObject);
    Stream *serial;
};
#endif
