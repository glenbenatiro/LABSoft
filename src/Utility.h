#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>

#include "LabelValue.h"

enum WaveType 
{ 
  SINE,
  SQUARE,
  SQUARE_HALF,
  SQUARE_FULL,
  TRIANGLE,
  DC
};

typedef struct 
{
  int fd,     // File descriptor
      h,      // Memory handle
      size;   // Memory size in bytes
    
  void *bus,  // Bus address
       *virt, // Virtual address
       *phys; // Physical address
} MemoryMap;

enum DisplayMode 
{
  REPEATED,
  SHIFT,
  SCREEN
};

class Utility 
{
  public:
    
              static uint32_t  get_bits   (uint32_t input, unsigned shift, uint32_t mask);
    volatile  static uint32_t *get_reg32 (MemoryMap mem_map, uint32_t offset);

              static void      reg_write  (MemoryMap mem_map, uint32_t offset, uint32_t value, uint32_t mask, unsigned shift);
              static void      reg_write  (volatile uint32_t *reg, uint32_t value, uint32_t mask, unsigned shift);

              static void      print_bits (int bits, unsigned size = 1);
};

volatile uint32_t* g_reg32 (MemoryMap mem_map, uint32_t offset);
void g_reg_write (volatile uint32_t *reg, uint32_t value, uint32_t mask, int shift);
void g_reg_write (MemoryMap mem_map, uint32_t offset, uint32_t value, uint32_t mask, int shift);
uint32_t g_reg_read (MemoryMap mem_map, uint32_t offset, uint32_t mask, int shift);

void g_reg32_peek (MemoryMap mem_map, uint32_t offset);
void g_reg32_peek (char const *name, MemoryMap mem_map, uint32_t offset);



double  g_get_unit_prefix_scaler          (char unit_prefix);
int     g_get_unit_prefix_power_exponent  (char unit_prefix);
char    g_get_unit_prefix                 (int power_scaler);
char    g_get_label_unit_prefix           (const char* label);
double g_get_actual_value_from_label (const char *label);
double aux_unit_label_to_unit_scaler (const char *label);
double aux_unit_label_to_unit_power (const char *label);

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

#endif