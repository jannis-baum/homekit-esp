#include "DEV_LampRelay.h"
#include "HomeSpan.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Living Room Ceiling Lights");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_LampRelay(19);
    new DEV_LampRelay(18);
    new DEV_LampRelay(5);
    new DEV_LampRelay(17);
}
