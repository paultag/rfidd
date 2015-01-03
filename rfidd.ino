#include <SoftwareSerial.h>

#define RFID_START       0x0A
#define RFID_STOP        0x0D
#define RFID_ENABLE_PIN  3
#define RFID_DATA_PIN    6

#define RFID_DISABLE     digitalWrite(RFID_ENABLE_PIN, HIGH)
#define RFID_ENABLE      digitalWrite(RFID_ENABLE_PIN, LOW)

#define RFID_RESPONSE_LENGTH   11


SoftwareSerial sserial =  SoftwareSerial(RFID_DATA_PIN, 0);

void setup() {
  pinMode(RFID_ENABLE_PIN, OUTPUT);
  pinMode(RFID_DATA_PIN, INPUT);
  
  RFID_DISABLE;
  
  Serial.begin(9600);
  while (!Serial);

  sserial.begin(2400);

  Serial.flush();
}

void loop() { 
  RFID_ENABLE;

  char data[RFID_RESPONSE_LENGTH];
  char offset = 0;
  data[0] = 0x00;
  while(true) {
    if (sserial.available()) {
      data[offset] = sserial.read();
      if (data[offset] == RFID_START) {
        offset = 0;
        continue;
      } else if (data[offset] == RFID_STOP) {
        data[offset] = 0;
        break;
      }
      offset = (offset + 1) % RFID_RESPONSE_LENGTH;
    }
  }
  
  RFID_DISABLE;
  Serial.println(data);
  Serial.flush();
  delay(500);
  RFID_ENABLE;
}
