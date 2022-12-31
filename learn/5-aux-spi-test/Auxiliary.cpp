

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

// get virtual 32-bit pointer to a register
volatile uint32_t*
g_reg32 (MemoryMap  mem_map, 
         uint32_t   offset)
{
  return (volatile uint32_t *)((uint32_t)(mem_map.virt) + (uint32_t)(offset));
}

void 
g_reg_write (volatile uint32_t *reg,
             uint32_t  value, 
             uint32_t  mask, 
             int       shift)
{
  *reg = (*reg & ~(mask << shift)) | (value << shift);
}

void 
g_reg_write (MemoryMap mem_map,
             uint32_t  offset,
             uint32_t  value, 
             uint32_t  mask, 
             int       shift)
{
  volatile uint32_t* reg = g_reg32 (mem_map, offset);

  g_reg_write (reg, value, mask, shift);
}

// print the contents of the 32-bit register
void g_reg32_peek (char const *name, MemoryMap mem_map, uint32_t offset)
{
  uint32_t reg = *(g_reg32 (mem_map, offset));

  printf ("\n");
  printf ("**********\n");
  printf ("%s\n", name);
  printf ("reg: %08X\n", reg);

  printf ("bin: " BYTE_TO_BINARY_PATTERN " " 
  BYTE_TO_BINARY_PATTERN " " 
  BYTE_TO_BINARY_PATTERN " " 
  BYTE_TO_BINARY_PATTERN "\n",
  BYTE_TO_BINARY(reg >> 24), BYTE_TO_BINARY(reg >> 16), 
  BYTE_TO_BINARY(reg >> 8), BYTE_TO_BINARY(reg));

  printf ("**********\n\n");
}

void g_reg32_peek (MemoryMap mem_map, uint32_t offset)
{
  g_reg32_peek ("no name given", mem_map, offset);
}


char g_get_label_unit_prefix (const char *label)
{
  double value      = atof (strtok (strdup (label), " "));
  char unit_prefix  = strtok (NULL, "/")[0];

  return unit_prefix;
}

double g_get_unit_prefix_scaler (char unit_prefix)
{
  switch (unit_prefix)
  {
    case 'G':
      return 1'000'000'000;
      break;
    case 'M':
      return 1'000'000;
      break;
    case 'k':
      return 1'000;
      break;
    case 'm':
      return 0.001;
      break;
    case 'u':
      return 0.000'001;
      break;
    case 'n':
      return 0.000'000'001;
      break;
    default:
      return 0;
      break;
  }
}

int g_get_unit_prefix_power_exponent (char unit_prefix)
{
  switch (unit_prefix)
  {
    case 'G':
      return 9;
    case 'M':
      return 6;
    case 'k':
      return 3;
    case 'm':
      return -3;
    case 'u':
      return -6;
    case 'n':
      return -9;
    default:
      return 0;
  }
}

double g_get_actual_value_from_label (const char *label)
{
  double  value              = atof (strtok (strdup (label), " "));
  char    unit_prefix        = strtok (NULL, "/")[0];
  double  unit_prefix_scaler = 1.0;
 
  switch (unit_prefix)
    {
      case 'G':
          unit_prefix_scaler = 1'000'000'000.0;
          break;
      case 'M':
          unit_prefix_scaler = 1'000'000.0;
          break;
      case 'k':
          unit_prefix_scaler = 1'000.0;
          break;
      case 'm':
          unit_prefix_scaler = 0.001;
          break;
      case 'u':
          unit_prefix_scaler = 0.000'001;
          break;
      case 'n':
          unit_prefix_scaler = 0.000'000'001;
          break;
      default:
          unit_prefix_scaler = 1.0;
          break;
    }
  
  return value * unit_prefix_scaler;
}

char g_get_unit_prefix (int power_scaler)
{
  char label;

  switch (power_scaler)
    {
      case 9:
        label = 'G';
        break;
      case 6:
        label = 'M';
        break;
      case 3:
        label = 'k';
        break;
      case -3:
        label = 'm';
        break;
      case -6:
        label = 'u';
        break;
      case -9:
        label = 'n';
        break;
      default: 
        label = '\b';
        break;
    }

  return label;
}

double aux_unit_label_to_unit_scaler (const char *label)
{
  int a;
  char prefix = label[0];
 
  switch (prefix)
    {
      case ('G'):
        a = 3;
        break;
      case ('M'):
        a = 2;
        break;
      case ('k'):
        a = 1;
        break;
      case ('m'):
        a = -1;
        break;
      case ('u'):
        a = -2;
        break;
      case ('n'):
        a = -3;
        break;
      default:
        a = 0;
        break;
    }
  
  return pow (10, (3 * (a * -1)));
}

double aux_unit_label_to_unit_power (const char *label)
{
  int a;
  char prefix = label[0];
 
  switch (prefix)
    {
      case ('G'):
        a = 3;
        break;
      case ('M'):
        a = 2;
        break;
      case ('k'):
        a = 1;
        break;
      case ('m'):
        a = -1;
        break;
      case ('u'):
        a = -2;
        break;
      case ('n'):
        a = -3;
        break;
      default:
        a = 0;
        break;
    }
  
  return a;
}

