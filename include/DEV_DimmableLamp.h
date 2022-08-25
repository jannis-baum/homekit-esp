#include "HomeSpan.h"
#include "extras/PwmPin.h"

struct DEV_DimmableLamp: Service::LightBulb {
    LedPin *pin;
    SpanCharacteristic *state;
    SpanCharacteristic *level;
    
    DEV_DimmableLamp(int pin): Service::LightBulb() {
        this->state = new Characteristic::On();
        this->level = new Characteristic::Brightness(50);
        this->level->setRange(5, 100, 1);

        this->pin = new LedPin(pin);
    }
    
    boolean update() {
        this->pin->set(this->state->getNewVal() * this->level->getNewVal());
        return (true);
    }  
};
