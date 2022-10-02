#include "HomeSpan.h"

struct DEV_LampRelay: Service::LightBulb {
    int relayPin;
    SpanCharacteristic *state;
    
    DEV_LampRelay(int pin): Service::LightBulb() {
        this->state = new Characteristic::On();
        this->relayPin = pin;
        pinMode(this->relayPin, OUTPUT);
        digitalWrite(this->relayPin, 1);
    }
    
    boolean update() {
        digitalWrite(this->relayPin, !this->state->getNewVal());
        return (true);
    }  
};
