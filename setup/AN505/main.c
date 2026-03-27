//#include "RTE_Components.h"                 /* This mechanism is the standard way to include */
#include "ARMCM33_DSP_FP.h"              /* the device header file, in this case ARMCM33.h */
#include <stdio.h>
#include <stdint.h>

#define CONFIGURED_SIZE 3


struct address_info_t {
    unsigned mpu_region : 8;
    unsigned readwrite_ok : 1;
    unsigned : 8; // used for sau_region in Trust Zone implementation
    unsigned mpu_region_valid : 1;
    unsigned : 1; // sau_region_valid in Trust Zone implementation
    unsigned read_ok : 1;
    unsigned : 12; /* idau_region : 8
                    idau_region_valid : 1
                    secure : 1;
                    nonsecure_readwrite_ok : 1
                    nonsecure_read_ok : 1
                    */
}; 

/** 
* @var policy::WA Write Allocation: Set to 1 to use cache allocation on write miss.
* @var policy::RA Read Allocation: Set to 1 to use cache allocation on read miss.
* @var policy::WB Write-Back: Set to 1 to use write-back update policy .
* @var policy::NT  Non-Transient: Set to 1 for non-transient data.
*/
struct policy{
    uint32_t WA     : 1; // bit 0
    uint32_t RA     : 1; // bit 1
    uint32_t WB     : 1; // bit 2  
    uint32_t NT     : 1; // bit 3
};

struct normal_attribute {
    struct policy outer;
    struct policy inner;
};

/** 
 * G or nG – Gathering or non-Gathering. Multiple accesses to a device can be merged into a single
 * transaction except for operations with memory ordering semantics, for example, memory barrier
 * instructions, load acquire/store release.
 * R or nR – Reordering or Non-reordering.
 * E or nE – Early Write Acknowledge (similar to bufferable).
*/
struct device_attribute {
    uint32_t ignore         : 2; // bit 0 - 1
    uint32_t device_type    : 2; // bit 2 - 3 (00) nGnRnE , (01) nGnRE, (10) nGRE, (11) GRE
    uint32_t ignore2        : 4; // bit 4 - 7
};

struct rbar_s{
    uint32_t executable     :   1;  // bit 0      : (0) execution in this region allowed (1) not allowed
    uint32_t wr         :   2;  // bit 1 - 2  : (00) Read/write by privileged code only, (01) r/w by any level, (10)ro by privileged code only, (11) ro by any level
    uint32_t sharability    :   2;  // bit 3 - 4  : (00) Non-sharable, (01) Outershareable, (10) innershareable
    uint32_t base_address   :   27; // bit 5 - 31 : Starting address of the MPU region
};
struct rlar_s {
    uint32_t region_enable  :   1;  // bit 0      : (0) not enabled (1) enabled
    uint32_t attr_indx      :   3;  // bit 1 - 3  :  attribute sets from (0 - 3) MAIR0 and (4 - 7)MAIR1
    uint32_t ignored        :   1;  // bit 4      : reserved bit for future  (set  to 0)
    uint32_t limit_address  :   27; // bit 5 - 31 : Limiting address of the MPU region
};

struct region {
    struct rbar_s base;
    struct rlar_s limit;
};

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

int main( void )
{
    
    mpu_setup();

    /* Exercise 4 - Test the MPU */
    int x = 0;
    uint32_t *ptr = (uint32_t *) 0x00000001;
    
    x = *ptr;   /* Read test  */
    *ptr = 0xA;  /* Write test */
    unsigned int result = MPU->MAIR0;
    
    /* Exercise 5 - Test with TT instruction */

    // Currently it doesn't work because I am in Nonsecure priviledged mode
    // it only works properly if TrustZone Secure mode is enabled
    volatile uint32_t raw;
    asm volatile (
     
        "TT %1, %0\n\t"
        : "=r" (raw)
        : "r" (ptr)
        : 
    );

    struct address_info_t *test_result = (struct address_info_t *)&raw;

    while (1);
}



// Read fault status registers and loop forever
void HardFault_Handler(void) {
    volatile uint32_t hfsr  = *(volatile uint32_t*)0xE000ED2C; // HardFault Status
    volatile uint32_t mmfar = *(volatile uint32_t*)0xE000ED34; // MemManage Fault Address
    while(1);   
}

void MemManage_Handler(void) {
    volatile uint32_t mmfsr = *(volatile uint32_t*)0xE000ED28 & 0xFF; // bottom byte = MMFSR
    volatile uint32_t mmfar = *(volatile uint32_t*)0xE000ED34;         // faulting address


    while(1);   
}