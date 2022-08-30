#ifndef DEV_SUNRISE_ALARM_H
#define DEV_SUNRISE_ALARM_H

#include "HomeSpan.h"
#include "DEV_SunriseLamp.h"

#define WAKE_TIME (60 * 1000 * 30)
#define WAKE_STEPS (COLOR_WARM - COLOR_COLD)

#define COLOR_INC (float(COLOR_WARM - COLOR_COLD) / WAKE_STEPS)
#define BRIGHT_INC (float(BRIGHT_MAX - BRIGHT_MIN) / WAKE_STEPS)

struct DEV_SunriseAlarm: Service::Outlet {
    DEV_SunriseLamp *lamp;
    SpanCharacteristic *waking;

    int step;
    
    DEV_SunriseAlarm(DEV_SunriseLamp *lamp): Service::Outlet() {
        this->lamp = lamp;
        this->waking = new Characteristic::On();
        this->step = 0;
    }

    boolean update() {
        if (this->waking->getNewVal()) {
            this->step = 0;
            this->lamp->set(1, COLOR_WARM, BRIGHT_MIN);
        }
        return true;
    }

    void loop() {
        if (this->waking->getVal()) this->incrementAlarm();
    }

    void incrementAlarm() {
        if (this->lamp->brightness->timeVal() < float(WAKE_TIME) / WAKE_STEPS) return;
        if (this->step == WAKE_STEPS) {
            this->waking->setVal(false);
            return;
        }
        this->step++;

        this->lamp->set(
            1,
            COLOR_WARM - this->step * COLOR_INC,
            BRIGHT_MIN + this->step * BRIGHT_INC,
            true
        );
    }
};

#endif
