#include "esphome.h"
#include <Wire.h>

#define I2C_ADDR 0x2D
#define REG_CTRL_2 0x03
#define PWDN_PIN 12

class TAS5805 : public Component, public Switch  {
  public:
    void setup() override {
      pinMode(PWDN_PIN, OUTPUT);
      digitalWrite(PWDN_PIN, LOW);
      delay(200);
      digitalWrite(PWDN_PIN, HIGH);

      Wire.begin();
      Wire.beginTransmission(I2C_ADDR);
      if (Wire.endTransmission() != 0) {
        ESP_LOGE("TAS5805", "TAS5805 not found at address 0x2D");
        return;
      }
      Wire.beginTransmission(I2C_ADDR);
      Wire.write(REG_CTRL_2);
      Wire.write(0x02);
      Wire.endTransmission();
      delay(50);
      Wire.beginTransmission(I2C_ADDR);
      Wire.write(REG_CTRL_2);
      Wire.write(0x03);
      Wire.endTransmission();

      ESP_LOGI("TAS5805", "TAS5805 initialized.");
    }

    void write_state(bool state) override {
      uint8_t value = state ? 0x03 : 0x00;
      Wire.beginTransmission(I2C_ADDR);
      Wire.write(REG_CTRL_2);
      Wire.write(value);
      Wire.endTransmission();
      publish_state(state);
    }
};
