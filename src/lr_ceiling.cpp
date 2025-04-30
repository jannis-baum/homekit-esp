#include "DEV_LampRelay.h"
#include "HomeSpan.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Living Room Ceiling Lights");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_LampRelay(19, false, true);
    new DEV_LampRelay(18, false, true);
    new DEV_LampRelay(5, false, true);
    new DEV_LampRelay(17, false, true);
}
