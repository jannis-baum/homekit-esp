#include "DEV_LampRelay.h"
#include "DEV_HydroponicsPump.h"
#include "HomeSpan.h"

void setup() {
    Serial.begin(115200);
    // - pool.ntp.org is NTP Pool Project, a large, free, volunteer-run group
    //   of public time servers that speak NTP (Network Time Protocol)
    // - time zone doesn't matter because we just need multiples of 15 minutes
    homeSpan.enableWebLog(10, "pool.ntp.org", "UTC0", NULL);
    homeSpan.begin(Category::Sprinklers, "Hydroponics");

    new SpanAccessory();
    new Service::AccessoryInformation();
        new Characteristic::Identify();
    new DEV_LampRelay(26, false, true);
    new DEV_HydroponicsPump(27, true, true);
}
