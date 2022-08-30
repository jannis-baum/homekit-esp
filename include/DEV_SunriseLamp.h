#ifndef DEV_SUNRISE_LAMP_H
#define DEV_SUNRISE_LAMP_H

#define COLOR_COLD 140
#define COLOR_WARM 500

#define BRIGHT_MAX 100
#define BRIGHT_MIN 1

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

        this->brightness = new Characteristic::Brightness(20);
        this->brightness->setRange(BRIGHT_MIN, BRIGHT_MAX, 1);

        this->colorTemp = new Characteristic::ColorTemperature();

        this->pinWarm = new LedPin(pinWarm);
        this->pinCold = new LedPin(pinCold);
    }
    
    boolean update() {
        return this->set(
            this->state->getNewVal(),
            this->colorTemp->getNewVal(),
            this->brightness->getNewVal()
        );
    }  

    boolean set(int state, float colorTemp, float brightness, boolean update = false) {
        if (update) {
            this->state->setVal(state);
            this->colorTemp->setVal(colorTemp);
            this->brightness->setVal(brightness);
        }

        float tempScalar = (float)(colorTemp - COLOR_COLD) / (COLOR_WARM - COLOR_COLD);
        int power = state * brightness;

        this->pinWarm->set(power * tempScalar);
        this->pinCold->set(power * abs(1 - tempScalar));

        return true;
    }
};

#endif
