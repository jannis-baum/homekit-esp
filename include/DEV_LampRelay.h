#include "HomeSpan.h"

struct DEV_LampRelay: Service::LightBulb {
    int relayPin;
    bool invert;
    SpanCharacteristic *state;
    
    DEV_LampRelay(int pin, bool defaultOn = false, bool invert = false)
    : Service::LightBulb()
    , invert(invert) {
        this->state = new Characteristic::On(defaultOn);
        this->relayPin = pin;
        pinMode(this->relayPin, OUTPUT);
        digitalWrite(this->relayPin, 1);
    }
    
    boolean update() {
        digitalWrite(
            this->relayPin,
            this->invert ^ this->state->getNewVal()
        );
        return (true);
    }  
};
