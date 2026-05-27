#include "calculationStore.h"
#include <LittleFS.h>
#include "../config.h"

void saveCalculationMethod(const String& method) {
    File f = LittleFS.open(CALCULATION_METHOD_FILE, "w");
    if (f) {
        f.print(method);
        f.close();
    }
}

String loadCalculationMethod() {
    if (!LittleFS.exists(CALCULATION_METHOD_FILE)) {
        return DEFAULT_CALCULATION_METHOD;
    }
    File f = LittleFS.open(CALCULATION_METHOD_FILE, "r");
    if (!f) return DEFAULT_CALCULATION_METHOD;
    String method = f.readString();
    f.close();
    return method;
}