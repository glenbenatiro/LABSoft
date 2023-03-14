#ifndef AUX_H
#define AUX_H

#include <cstdint>

#include "LabelValue.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

typedef struct 
{
  int fd,     // File descriptor
      h,      // Memory handle
      size;   // Memory size in bytes
    
  void *bus,  // Bus address
       *virt, // Virtual address
       *phys; // Physical address
} AP_MemoryMap;

enum WaveType 
{ 
  SINE, 
  SQUARE, 
  TRIANGLE, 
  DC,
  SAWTOOTH,
  NOISE,
  OTHER
};

enum LE_GRAPH_DISP_MODE 
{
  LE_GRAPH_DISP_MODE_REPEATED,
  SHIFT,
  LE_GRAPH_DISP_MODE_SCREEN
};

volatile uint32_t* g_reg32 (AP_MemoryMap mem_map, uint32_t offset);
void g_reg_write (volatile uint32_t *reg, uint32_t value, uint32_t mask, int shift);
void g_reg_write (AP_MemoryMap mem_map, uint32_t offset, uint32_t value, uint32_t mask, int shift);

void g_reg32_peek (AP_MemoryMap mem_map, uint32_t offset);
void g_reg32_peek (char const *name, AP_MemoryMap mem_map, uint32_t offset);

double  g_get_unit_prefix_scaler          (char unit_prefix);
int     g_get_unit_prefix_power_exponent  (char unit_prefix);
char    g_get_unit_prefix                 (int power_scaler);
char    g_get_label_unit_prefix           (const char* label);
double g_get_actual_value_from_label (const char *label);
double aux_unit_label_to_unit_scaler (const char *label);
double aux_unit_label_to_unit_power (const char *label);

#endif

