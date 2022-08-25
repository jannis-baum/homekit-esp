#include "DEV_DimmableLamp.h"
#include "HomeSpan.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Sunrise Lamp");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_DimmableLamp(12);

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_DimmableLamp(14);
}

void loop() { homeSpan.poll(); }
