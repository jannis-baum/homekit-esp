#include "HomeSpan.h"

#define PUMP_INTERVAL_MINUTES 10
#define PUMP_DURATION_SECONDS 30

struct DEV_HydroponicsPump: Service::IrrigationSystem {
    // timer schedule on vs. off
    SpanCharacteristic *program_mode;
    // mirrors valve
    SpanCharacteristic *in_use;

    // valve, the physical relay controller and manual switch in homekit app
    struct PumpValve: Service::Valve {
        int relay_pin;
        bool invert;

        time_t last_pump_start = 0;
        long last_pumped_interval = 0;
        bool pump_state = false;

        // writable via Homekit app, "requested state"
        SpanCharacteristic *active;
        // actual state, read-only
        SpanCharacteristic *in_use;
        SpanCharacteristic *in_use_mirror;

        PumpValve(int pin, bool invert, SpanCharacteristic *in_use_mirror)
        : Service::Valve()
        , relay_pin(pin)
        , invert(invert)
        , in_use_mirror(in_use_mirror) {
            new Characteristic::ValveType(Characteristic::ValveType::IRRIGATION);
            new Characteristic::Name("Pump");
            this->active = new Characteristic::Active(false);
            this->in_use = new Characteristic::InUse(false);

            pinMode(this->relay_pin, OUTPUT);
            // not set_pump, characteristics shouldn't be notified before HomeSpan runs
            digitalWrite(this->relay_pin, this->invert);
        }

        boolean update() {
            this->set_pump(this->active->getNewVal());
            return (true);
        }

        void set_pump(bool state) {
            if (state) {
                time_t now = time(NULL);
                this->last_pumped_interval = now / (PUMP_INTERVAL_MINUTES * 60);
                this->last_pump_start = now;
            }
            this->pump_state = state;
            digitalWrite(this->relay_pin, this->invert ^ state);
            this->in_use->setVal(state);
            if (this->in_use_mirror) {
                this->in_use_mirror->setVal(state);
            }
            // skip when the Home app is changing it right now (valve update), HomeSpan warns otherwise
            if (this->active->getNewVal() != state) this->active->setVal(state);
        }
    };
    PumpValve *valve;

    // switch that controls whether the timer schedule is enabled
    struct ScheduleSwitch: Service::Switch {
        // switch
        SpanCharacteristic *on;
        // external, program/schedule mode that the switch controls
        SpanCharacteristic *program_mode;

        ScheduleSwitch(SpanCharacteristic *program_mode, bool default_on)
        : Service::Switch()
        , program_mode(program_mode) {
            new Characteristic::Name("Pump Schedule");
            this->on = new Characteristic::On(default_on);
        }

        boolean update() {
            auto new_val = this->on->getNewVal();
            auto mode = DEV_HydroponicsPump::program_mode_for(new_val);
            this->program_mode->setVal(mode);
            return (true);
        }
    };
    ScheduleSwitch *schedule_switch;

    DEV_HydroponicsPump(int pin, bool default_on = true, bool invert = false)
    : Service::IrrigationSystem() {
        // required, but the Home app derives the system's state from its valves
        new Characteristic::Active(1);
        this->in_use = new Characteristic::InUse(0);

        this->program_mode = new Characteristic::ProgramMode(this->program_mode_for(default_on));
        this->valve = new PumpValve(pin, invert, this->in_use);
        this->schedule_switch = new ScheduleSwitch(this->program_mode, default_on);
        this->addLink(this->valve);
    }

    void loop() {
        // time gives uptime in seconds until NPT syncs, then seconds since UNIX
        // epoch. the first pump will usually happen as soon as it syncs
        // (initialized `last_pumped_interval` at 0 so syncing means it's now
        // bigger) or if no sync happens for 15 minutes then it starts pumping by
        // the uptime schedule and switches to time schedule with one extra pump
        // as soon as it does sync
        time_t now = time(NULL);
        long current_interval = now / (PUMP_INTERVAL_MINUTES * 60);

        // enable if not yet pumped in this interval and timer is on
        if (current_interval > this->valve->last_pumped_interval && this->program_mode->getVal() == Characteristic::ProgramMode::SCHEDULED) {
            this->valve->set_pump(true);
        }
        // disable if on and has been on for PUMP_DURATION_SECONDS
        if (this->valve->pump_state && now - this->valve->last_pump_start >= PUMP_DURATION_SECONDS) {
            this->valve->set_pump(false);
        }
    }

private:
    static uint8_t program_mode_for(bool timer_on) {
        return timer_on ? Characteristic::ProgramMode::SCHEDULED : Characteristic::ProgramMode::NONE;
    }
};
