#ifndef SIN_TABLE_H
#define SIN_TABLE_H

#define NUM_SINE_SAMPLES 3600 // Number of samples in the sine/cosine lookup table
#define SINE_TABLE_SCALE (NUM_SINE_SAMPLES / (2 * M_PI)) // Scale factor to convert angle to table index

extern float sin_table[NUM_SINE_SAMPLES];
extern float cos_table[NUM_SINE_SAMPLES];

void sin_table_init(void);

#endif
