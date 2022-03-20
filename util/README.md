# util

Various utilities.

## redis-stream-in and redis-stream-out

These are benchmarking programs. 

### Hardware and Payload

Given my 12/24 core AMD Ryzen where these two clients run:

```
Architecture:                    x86_64                                                                                                                        
CPU op-mode(s):                  32-bit, 64-bit                                                                                                                
Byte Order:                      Little Endian                                                                                                                 
Address sizes:                   48 bits physical, 48 bits virtual                                                                                             
CPU(s):                          24                                                                                                                            
On-line CPU(s) list:             0-23                                                                                                                          
Thread(s) per core:              2                                                                                                                             
Core(s) per socket:              12                                                                                                                            
Socket(s):                       1
Vendor ID:                       AuthenticAMD
CPU family:                      23
Model:                           113
Model name:                      AMD Ryzen 9 3900X 12-Core Processor
Stepping:                        0
CPU MHz:                         3792.900
BogoMIPS:                        7585.80
Virtualization:                  AMD-V
Hypervisor vendor:               Microsoft
Virtualization type:             full
L1d cache:                       384 KiB
L1i cache:                       384 KiB
L2 cache:                        6 MiB
L3 cache:                        16 MiB
```

And given my Linux hardware which runs redis:

```
Architecture:                    x86_64                                                                                           
CPU op-mode(s):                  32-bit, 64-bit                                                                                   
Byte Order:                      Little Endian                                                                                    
Address sizes:                   36 bits physical, 48 bits virtual                                                                
CPU(s):                          4                                                                                                
On-line CPU(s) list:             0-3                                                                                              
Thread(s) per core:              1                                                                                                
Core(s) per socket:              4
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       GenuineIntel
CPU family:                      6
Model:                           58
Model name:                      Intel(R) Core(TM) i5-3570K CPU @ 3.40GHz
Stepping:                        9
CPU MHz:                         1800.000
CPU max MHz:                     4200.0000
CPU min MHz:                     1600.0000
BogoMIPS:                        6800.32
Virtualization:                  VT-x
L1d cache:                       128 KiB
L1i cache:                       128 KiB
L2 cache:                        1 MiB
L3 cache:                        6 MiB                                                                                            ──────────────────────────────────────────────────────
NUMA node0 CPU(s):               0-3
```

And given the following payload from redis examples:

    XADD {} * sensor-id 1234 temperature {}

### Benchmarks

Here are some benchmarks.

When source/sink are running on my Windows desktop, running in a WSL2 Debian container, we get:

    ~ 770.1650232441272mps

This was a bit underwhelming. I checked my CPU and it was mostly idle. I checked the CPU on the Redis server, and it was likewise idle. This means my home network, despite being a wired 1Gbps, isn't that fast.

When I run source/sink on the redis hardware, the results are much better:

    ~ 7011.405656912864mps
    