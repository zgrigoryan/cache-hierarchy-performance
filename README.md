# Stride-Based Cache Performance Test

This repository contains a simple C++ program to illustrate how memory access *stride* affects cache performance. By fixing the total number of memory accesses (`NUM_ACCESSES`) and varying only the stride size, we can observe how spatial locality and cache line behavior change as we jump further and further in memory.

## Overview

- **Code**:  
  - Dynamically allocates an integer array of size `ARRAY_SIZE`.  
  - Randomly initializes its elements.  
  - Performs a warm-up pass.  
  - Repeatedly measures the time to do exactly `NUM_ACCESSES` increments in the array, with each access jumping by `stride` in memory.  
  - Prints the average time (in microseconds) for multiple trials (default: 5) for each stride.

- **Goal**:  
  Demonstrate how larger strides lead to worse performance due to higher cache miss rates (less spatial locality).

## Build & Run

1. **Open a terminal** and navigate to the folder containing the source file (e.g., `main.cpp`).

2. **Compile** with:
   ```bash
   g++ -std=c++11 -O3 main.cpp -o main
   ```
   - `-std=c++11` ensures C++11 features are recognized.  
   - `-O3` enables higher optimization (can yield more realistic performance measurements).

3. **Run** with:
   ```bash
   ./main
   ```

4. **Observe Output**: The output lists each stride and the corresponding average time in microseconds.

Example output:

```
    Stride | Avg Time (us)
-------------------------
         1 |         6053
         2 |         5819
         4 |         5839
         8 |         8243
        16 |        14236
        64 |        48320
```

## Explanation of Results

1. **Small Strides (1, 2, 4):**  
   - Each access is relatively close to the previous one in memory.  
   - This allows the CPU’s cache and hardware prefetcher to work efficiently.  
   - Fewer cache misses → lower overall time.

2. **Medium Strides (8, 16):**  
   - At stride 8 or 16 (which typically corresponds to jumps of 32 or 64 bytes per access if `sizeof(int)=4`), each access frequently lands on a *new* cache line.  
   - This raises the miss rate and increases the overall time.

3. **Large Strides (64 or higher):**  
   - Each access jumps well beyond one cache line (64 bytes).  
   - Almost every access triggers a cache miss, often leading to main memory accesses.  
   - The total time increases dramatically.

As stride grows, you lose the benefit of *spatial locality*—the idea that consecutive (or nearby) memory locations can be fetched into cache as part of the same line. Larger strides skip many potential in-line cache fetches, resulting in more misses and higher access times.

---

Feel free to adjust `ARRAY_SIZE` or `NUM_ACCESSES` in the source to suit your needs and compare how the performance changes as the working set size approaches or exceeds your system’s L1, L2, or L3 cache sizes.
