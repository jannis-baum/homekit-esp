#include "HomeSpan.h"
#include "DEV_SunriseLamp.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Sunrise Lamp");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_SunriseLamp(14, 12);
}

void loop() { homeSpan.poll(); }
