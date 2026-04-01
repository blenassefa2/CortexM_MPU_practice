Just timing cpu 0 while it updates a shared counter

contention measurement
Expectation: there will be more contention and time difference visible when atomic and volatile are used
Did it occure: yes
Conclusion: together with sensitive operations these timing difference could be used for side channel attacks

Observation analysis: atomics introduce contention-dependent retry behavior, leading to observable timing differences


volatile atomic_uint_fast32_t shared_counter: expected = 003D090
min    : 00000000
max    : 0001791C
total  : 015500AA
avg    : 0000000B
contended: 000FAC67
cpu1 total ticks : 015500AA
final shared_counter value: 003D0900


atomic_uint_fast32_t shared_counter: expected = 003D090

min    : 00000000
max    : 00001234
total  : 0120AB60
avg    : 00000009
contended: 000E4270
cpu1 total ticks : 0120AB60
final shared_counter value: 003D0900


!Race-condition! volatile uint32_t shared_counter: expected = 003D090
min    : 00000000
max    : 00014A81
total  : 014F25B1
avg    : 0000000A
contended: 000FAC0A
cpu1 total ticks : 014F25B1
final shared_counter value: 003C98E8


!Race-condition! uint32_t shared_counter: expected = 003D090
min    : 00000000
max    : 0000EC61
total  : 0122CFBA
avg    : 00000009
contended: 000E4D66
cpu1 total ticks : 0122CFBA
final shared_counter value: 003CA6E9


