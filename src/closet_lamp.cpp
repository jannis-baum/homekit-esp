#include "DEV_LampRelay.h"
#include "HomeSpan.h"

void setup() {
    Serial.begin(115200);
    homeSpan.begin(Category::Lighting, "Closet Lamp");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_LampRelay(13, false, true);
}
