#include "HomeSpan.h"
#include "DEV_SunriseLamp.h"
#include "DEV_SunriseAlarm.h"

void setup() {
    homeSpan.begin(Category::Lighting, "Sunrise Lamp");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();

    DEV_SunriseLamp *lamp = new DEV_SunriseLamp(14, 12);
    new DEV_SunriseAlarm(lamp);
}
