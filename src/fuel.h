#pragma once
// Fuel selection (SETUP DASHBOARD -> FUEL, or serial fuel=): the stoichiometric AFR and
// density of what is actually in the tank. The AFR panel scaling (lambda -> AFR, rich/lean
// words, bar range) and the MAF-derived fuel rate all follow the choice at runtime.
#include <stdint.h>
#include <ctype.h>

enum FuelType : uint8_t { FUEL_G91, FUEL_G95, FUEL_E10, FUEL_E20, FUEL_COUNT };

struct FuelSpec { const char *key; const char *label; float stoich; float density; };

// Approximate blend properties, not a fuel analysis: stoich AFR and density in g/L.
// G91/G95 mean Thai GASOHOL 91/95 (E10), NOT ethanol-free petrol.
// Octane rating does not change the ethanol fraction; both use the E10 estimate.
static const FuelSpec kFuels[FUEL_COUNT] = {
    { "g91", "G91", 14.1f, 749.0f },
    { "g95", "G95", 14.1f, 749.0f },
    { "e10", "E10", 14.1f, 749.0f },
    { "e20", "E20", 13.5f, 754.0f },
};

// Exact, case-insensitive setting value; FUEL_COUNT means invalid (or numeric telemetry).
inline uint8_t fuelTypeFromKey(const char *value) {
    if (!value) return FUEL_COUNT;
    for (int i = 0; i < FUEL_COUNT; i++) {
        const char *p = value, *key = kFuels[i].key;
        while (*key && tolower((unsigned char)*p) == *key) { p++; key++; }
        if (*key) continue;
        while (isspace((unsigned char)*p)) p++;
        if (!*p) return (uint8_t)i;
    }
    return FUEL_COUNT;
}
