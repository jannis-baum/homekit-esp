#include "HomeSpan.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define DELAY 10000

struct DEV_Temperature: Service::TemperatureSensor {
    SpanCharacteristic *temperature;

    Adafruit_BME280 *bme;

    DEV_Temperature(Adafruit_BME280 *bme): bme(bme), Service::TemperatureSensor() {
        this->temperature = new Characteristic::CurrentTemperature();
    }

    void loop() {
        if (this->temperature->timeVal() < DELAY) return;
        this->temperature->setVal(this->bme->readTemperature());
    }
};

struct DEV_Humidity: Service::HumiditySensor {
    SpanCharacteristic *humidity;

    Adafruit_BME280 *bme;

    DEV_Humidity(Adafruit_BME280 *bme): bme(bme), Service::HumiditySensor() {
        this->humidity = new Characteristic::CurrentRelativeHumidity();
    }

    void loop() {
        if (this->humidity->timeVal() < DELAY) return;
        this->humidity->setVal(this->bme->readHumidity());
    }
};

struct DEV_Climate {
    Adafruit_BME280 bme;

    DEV_Climate() {
        bool status = this->bme.begin(0x76);
        if (!status) {
            LOG0("Could not find a valid BME280 sensor, check wiring!");
            while (1);
        }

        new DEV_Temperature(&this->bme);
        new DEV_Humidity(&this->bme);
    }
};
