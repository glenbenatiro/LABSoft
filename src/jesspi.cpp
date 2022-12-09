// #include "jesspi.h"

// JessPi::
// JessPi ()
// {

// }

// JessPi:: 
// ~JessPi ()
// {

// }

// int JessPi:: 
// initialize ()
// {
//   utility_devices_map ();
// }

// int JessPi:: 
// terminate ()
// {

// }

// // Utility
// void JessPi::
// utility_map_devices ()
// {
//   utility_map_peripheral (&(m_reg_aux),   (void *) AUX_BASE,  MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_gpio),  (void *) GPIO_BASE, MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_dma),   (void *) DMA_BASE,  MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_spi),   (void *) SPI0_BASE, MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_clk),   (void *) CLK_BASE,  MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_pwm),   (void *) PWM_BASE,  MEM_PAGE_SIZE);
//   utility_map_peripheral (&(m_reg_usec),  (void *) USEC_BASE, MEM_PAGE_SIZE);
// }

// void JessPi::
// utility_map_peripheral (MemoryMap *mem_map, 
//                         void      *phys_add,    
//                         int        size)
// {
//   mem_map->phys = phys_add;
//   mem_map->size = utility_page_roundup (size);
//   mem_map->bus  = (uint8_t *)phys_add - (uint8_t *)PHYS_REG_BASE +
//                         (uint8_t *)BUS_REG_BASE;
//   mem_map->virt = utility_map_segment (phys, mem_map->size);
// }

// int JessPi:: 
// utility_page_roundup (int size)
// {
//   return (size % MEM_PAGE_SIZE == 0 ? (size) : 
//     (((size) + MEM_PAGE_SIZE) & ~(MEM_PAGE_SIZE - 1)));
// }



// // eof