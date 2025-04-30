#include "HomeSpan.h"

struct DEV_LampRelay: Service::LightBulb {
    int relayPin;
    bool invert;
    SpanCharacteristic *state;
    
    DEV_LampRelay(int pin, bool defaultOn = false, bool invert = false)
    : Service::LightBulb()
    , relayPin(pin)
    , invert(invert) {
        this->state = new Characteristic::On(defaultOn);
        pinMode(this->relayPin, OUTPUT);
        this->write(defaultOn);
    }
    
    boolean update() {
        this->write(this->state->getNewVal());
        return (true);
    }

private:
    void write(bool state) {
        digitalWrite(this->relayPin, this->invert ^ state);
    }
};
