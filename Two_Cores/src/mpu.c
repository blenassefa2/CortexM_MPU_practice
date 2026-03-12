//#include "RTE_Components.h"                 /* This mechanism is the standard way to include */
#include "ARMCM33_DSP_FP.h"              /* the device header file, in this case ARMCM33.h */
#include <stdio.h>
#include <stdint.h>
#include "mpu.h"

struct region regions[CONFIGURED_SIZE];
uint32_t convert_r(struct rlar_s *rl, struct rbar_s *rb)
{
    if (rl != NULL)
    {
        return (rl->limit_address << 5 ) | (rl->attr_indx << 1) | rl->region_enable;
    }
    else if (rb != NULL)
    {
        return (rb->base_address << 5) | (rb->sharability << 3) | (rb->wr << 1) | rb->executable;
    }
}



void mpu_setup( void ) {
   __DMB(); /* Force any outstanding transfers to complete before disabling MPU */
    /* Disable MPU */
   
    MPU->CTRL = 0;
    /* Exercise 2 -  Initialize two Memory Attributes  */
   
    /* Configure memory types */
    
    volatile unsigned int result = MPU->MAIR0;
    // Set Memory Attr 0
    
    ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(
        ARM_MPU_ATTR_NON_CACHEABLE, // 0100 if write through and no allocation it is considered non cachable
        ARM_MPU_ATTR_NON_CACHEABLE
    ));
    
    // Set Memory Attr 1
    ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(
        ARM_MPU_ATTR_DEVICE,
        ARM_MPU_ATTR_DEVICE_nGnRnE
    ));

    result = MPU->MAIR0;
  
    
    /* Set up Attr 2-7 for further Normal and Device memory configurations: pass */

    /* Exercise 3 -  Initialize memory regions */
    MPU->RNR = 0; // for all 3 regions 

    /* Configure region 0 - Mappable memory */
    // instead of defining local struct I'll use global array regions
    // struct region mappable_memory; 

    // if it was  a local struct I might get garbage values so I must set each attribute
    // but now since i am using global variable I can assim all are initially set to 0
    regions[0].base.wr = 2; // RO priv only

    regions[0].limit.region_enable = 1;
    regions[0].limit.limit_address = 0x3FFF >> 5; 

    MPU->RBAR = convert_r(NULL, &regions[0].base);
    MPU->RLAR = convert_r(&regions[0].limit, NULL);


    /* Configure region 1 - ZBTSRAM 1 */
    // struct region zbtsram1;

    // since it is a local struct I might get garbage values so I'll set each attribute
   
    regions[1].base.wr = 1;
    regions[1].base.base_address = 0x4000 >> 5;


    regions[1].limit.region_enable = 1;
    regions[1].limit.limit_address = 0x7FFFFF >> 5; 

    MPU->RBAR_A1 = convert_r(NULL, &regions[1].base);
    MPU->RLAR_A1 = convert_r(&regions[1].limit, NULL);

    /* Configure region 2 - ZBTSRAM 2 & 3 (2x32-bit) */
    regions[2].base.wr = 1;
    regions[2].base.base_address = 0x20000000 >> 5; 


    regions[2].limit.region_enable = 1;
    regions[2].limit.limit_address = 0x207FFFFF >> 5; 

    MPU->RBAR_A2 = convert_r(NULL, &regions[2].base);
    MPU->RLAR_A2 = convert_r(&regions[2].limit, NULL);

    
    MPU->CTRL |= 5; /* Enable the MPU */
    SCB->SHCSR |= (1 << 16);  // enable MemManage fault
    
    __DSB(); /* Force memory writes before continuing */
    __ISB(); /* Flush and refill pipeline with updated permissions */
    return;
}
