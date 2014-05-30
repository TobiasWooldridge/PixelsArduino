#include <Adafruit_NeoPixel.h>

#define PIN 6
#define MESSAGE_LENGTH 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

byte message[MESSAGE_LENGTH];

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(35);
  strip.show();
}

byte checksum(byte bytes[], int length) {
  byte checksum = 0;

  for (int i = 0; i < length; i++) {
      byte b = bytes[i];
      checksum = ((checksum + b) << 1) ^ b;
  }

  return checksum;
}

long lastRequest = millis();

void loop() {
  while (Serial.available() < MESSAGE_LENGTH + 1) {
    if ((millis() - lastRequest) > 2) {
      Serial.print('i');
      lastRequest = millis();
    }
  }

  if (Serial.read() != 254) {
    return;
  }

  Serial.readBytes((char*)message, MESSAGE_LENGTH - 1);
  byte specifiedChecksum = Serial.read();

  byte actualChecksum = checksum(message, MESSAGE_LENGTH - 1);

  if (specifiedChecksum == actualChecksum) {
    strip.setPixelColor(message[0], message[1], message[2], message[3]);
    strip.show();
  }

  lastRequest = millis();
}
  
