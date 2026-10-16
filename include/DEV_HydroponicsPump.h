#include "HomeSpan.h"

#define PUMP_INTERVAL_MINUTES 15
#define PUMP_DURATION_SECONDS 30

struct DEV_HydroponicsPump: Service::LightBulb {
    int relayPin;
    bool invert;
    SpanCharacteristic *timer_state;

    DEV_HydroponicsPump(int pin, bool defaultOn = true, bool invert = false)
    : Service::LightBulb()
    , relayPin(pin)
    , invert(invert) {
        this->timer_state = new Characteristic::On(defaultOn);
        pinMode(this->relayPin, OUTPUT);
        this->set_pump(false);
    }

    boolean update() {
        return (true);
    }

    void loop() {
        time_t now = time(NULL);
        long current_interval = now / (PUMP_INTERVAL_MINUTES * 60);

        // enable if not yet pumped in this interval and timer is on
        if (current_interval > this->last_pumped_interval && this->timer_state->getVal()) {
            this->last_pumped_interval = current_interval;
            this->last_pump_start = now;
            this->set_pump(true);
        }
        // disable if on and has been on for PUMP_DURATION_SECONDS
        if (this->pump_state && now - this->last_pump_start >= PUMP_DURATION_SECONDS) {
            this->set_pump(false);
        }
    }

private:
    long last_pumped_interval = 0;
    time_t last_pump_start = 0;
    bool pump_state = false;

    void set_pump(bool state) {
        this->pump_state = state;
        digitalWrite(this->relayPin, this->invert ^ state);
    }
};
