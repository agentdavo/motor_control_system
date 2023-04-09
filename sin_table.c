#include "sin_table.h"
#include <math.h>

float sin_table[NUM_SINE_SAMPLES];
float cos_table[NUM_SINE_SAMPLES];

void sin_table_init(void) {
    // Generate sine/cosine lookup table
    for (int i = 0; i < NUM_SINE_SAMPLES; i++) {
        float angle = i / SINE_TABLE_SCALE;
        sin_table[i] = sinf(angle);
        cos_table[i] = cosf(angle);
    }
}
