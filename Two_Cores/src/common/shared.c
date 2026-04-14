#include "../../include/shared.h"
#include <stdatomic.h>

atomic_uint_fast32_t shared_counter = 0;

volatile atomic_bool cpu1_running = false;
volatile atomic_bool cpu0_running = false;