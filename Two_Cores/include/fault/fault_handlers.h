#ifndef FAULT_HANDLERS_H
#define FAULT_HANDLERS_H

#include <stdint.h>

// Cortex-M exception handlers
void HardFault_Handler(void);
void MemManage_Handler(void);

//  Optional helpers (can be reused elsewhere)
// uint32_t fault_get_hfsr(void);
// uint32_t fault_get_mmfar(void);
// uint32_t fault_get_mmfsr(void);

#endif