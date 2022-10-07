#include "HomeSpan.h"
#include "DEV_Climate.h"
#include "DEV_LampRelay.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Papa's Lampe");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();

    new DEV_LampRelay(15, true);
    new DEV_Climate(18, 19);
}
