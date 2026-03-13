#ifndef MPU_H
#define MPU_H

#define CONFIGURED_SIZE 3

#include <stdint.h>
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



void mpu_setup(void);
uint32_t convert_r(struct rlar_s *rl, struct rbar_s *rb);
#endif /* MPU_H */