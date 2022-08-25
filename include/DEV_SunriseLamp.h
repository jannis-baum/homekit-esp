#include "HomeSpan.h"
#include "extras/PwmPin.h"

struct DEV_SunriseLamp: Service::LightBulb {
    LedPin *pinWarm;
    LedPin *pinCold;

    SpanCharacteristic *state;
    SpanCharacteristic *brightness;
    SpanCharacteristic *colorTemp;
    
    DEV_SunriseLamp(int pinWarm, int pinCold): Service::LightBulb() {
        this->state = new Characteristic::On();

        this->brightness = new Characteristic::Brightness(50);
        this->brightness->setRange(5, 100, 1);

        this->colorTemp = new Characteristic::ColorTemperature();

        this->pinWarm = new LedPin(pinWarm);
        this->pinCold = new LedPin(pinCold);
    }
    
    boolean update() {
        float warmth = (float)(this->colorTemp->getNewVal() - 140) / 360;
        int brightness = this->state->getNewVal() * this->brightness->getNewVal();

        this->pinWarm->set(brightness * warmth);
        this->pinCold->set(brightness * abs(1 - warmth));

        return (true);
    }  
};
