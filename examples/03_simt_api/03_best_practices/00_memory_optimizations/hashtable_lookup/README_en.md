# SIMT SoA vs AoS Hash Table Lookup Performance Tuning Example

## Overview

Taking flat open-addressing hash table lookup as an example, this example introduces how to optimize hash table lookup by modifying the hash table structure under the Ascend C SIMT programming model.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR&950DT products | >= CANN 9.2.0 |

## Directory Structure

```text
├── hashtable_lookup
│   ├── figures                          // Image resources for README
│   ├── CMakeLists.txt                   // CMake build project file
│   ├── hashtable_lookup.asc             // SIMT hash table lookup optimization path implementation
│   ├── README.md                        // Sample documentation
│   └── README_en.md                     // English sample documentation
```

## Example Description

- Example Function: Implement a hash table lookup operator. The hash table resolves hash collisions based on open addressing; for details, see [Background](../../../../../examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README_en.md#background). This example looks up the hash table through different hash table structure designs and thread access patterns, and compares their performance, demonstrating the impact of different optimization methods on performance.

- Example Specifications:

  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">hashtable_lookup</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">keys</td><td align="center">[1, 1048576]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">indices</td><td align="center">[1, 1048576]</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Preset Data</td><td colspan="4" align="center">AoS table 40MB / SoA table 40MB / SoA+digest table 41MB — build one of them per Case</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">lookup_aos_kernel / lookup_soa_kernel / lookup_soa_digest_swar_kernel / lookup_coop_swar_kernel</td></tr>
  </table>

## Example Implementation

### Case Implementation Description

This example builds an optimization path with 4 Cases. The kernel functions and main changes of each Case are shown in the following table.

| Case | Kernel Function | Change Introduced in This Step |
| ---- | ------ | -------------- |
| 0    | `lookup_aos_kernel` | Uses the basic hash table lookup implementation as the baseline |
| 1    | `lookup_soa_kernel` | Key/value stored in separate partitions |
| 2    | `lookup_soa_digest_swar_kernel` | Digest pre-filtering + vcmp_eq4 batch comparison |
| 3    | `lookup_coop_swar_kernel` | Thread group cooperative probing |

### SoA and AoS Memory Layouts

AoS (Array of Structures) and SoA (Structure of Arrays) are two different memory layouts:
- AoS layout: Each structure contains the complete information of one object, and multiple structures are arranged into an array stored in memory.
- SoA layout: The same kind of information of objects is arranged into an array, and the same kind of information of multiple objects is stored in adjacent memory.

**Figure 1 Memory layout diagram**

![Memory layout diagram](figures/soa_aos_layout.png)

Figure 1 uses the hash table structure of this example to illustrate the different memory layouts. The hash table structure code is implemented as follows:

```cpp
struct AoSBucket {
  uint64_t key;
  float value[DIM];
};
AoSBucket table[CAPACITY];

struct SoaTable {
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};
SoaTable table;
```

### Performance Metrics Description

| Metric | Description |
| ---- | ---- |
| Task Duration(μs) | Total task duration, including scheduling time to the accelerator, execution time on the accelerator, and response completion time |
| read_main_memory_data(KB) | Total amount of data read from GM by each block during task execution |

### Case 0: AoS Slot-by-Slot Probing Baseline

**Optimization Objective**: Establish a performance baseline using the basic hash table lookup implementation.

**Core Implementation**: A Bucket is 40B in total. AoS stores the 8B key and the 32B value in the same Bucket, and the lookup logic reads the 8B key bucket by bucket for comparison. Each thread independently handles one query key.

**Key Code**:

```cpp
struct AoSBucket {
  uint64_t key;
  float value[DIM];
};

__aicore__ inline uint32_t find_aos(const AoSBucket* table, uint64_t target_key, uint32_t start) {
  ...
  const uint64_t bucket_key = table[idx].key;
  if (bucket_key == target_key) {
      return idx;
  }
  if (bucket_key == EMPTY_KEY) {
      return NOT_FOUND;
  }
  idx = (idx + 1) & (CAPACITY - 1);
  ...
}
```

**Performance Data**:

| Case | Task Duration(μs) | read_main_memory_data_all(MB) |
| ---- | :---------------: | :------------------------: |
| 0    |     1311.297      |          672.645           |

> read_main_memory_data_all(MB) is the sum of read_main_memory_datas(KB) of all blocks in `Memory.csv`.

**Analysis**:

Data load and store are performed in units of Cache Lines. AoS stores the key and value in the same Bucket, where the value alone occupies 32B, so a 128B Cache Line can only hold 3.2 Buckets. The value field is not needed before the target key is found, yet it occupies every slot on the probing path as invalid payload. Probing accesses the entire table bucket by bucket, making data transfer the dominant overhead. What probing actually needs is only the 8B key — if the key is separated into a compact key region, the probing span can be compressed to 8B and no invalid value bytes are transferred, significantly improving memory access locality. Therefore, the next step of optimization changes the AoS layout to the SoA layout.

---

### Case 1: Pure SoA Layout Refactoring

**Optimization Objective**: Adopt SoA (Structure of Arrays) to optimize the memory layout and improve the memory access efficiency of key lookup.

**Core Implementation**: The algorithm is identical to Case 0; the only difference is the memory layout. SoA stores all 8MB of keys contiguously together and all 32MB of values contiguously together, with keys and values stored separately.

**Key Code**:

```cpp
struct SoaTable {
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};

__aicore__ inline uint32_t find_soa(const SoaTable* table, uint64_t target_key, uint32_t start)
{
    uint32_t idx = start;
    for (uint32_t i = 0; i < CAPACITY; ++i) {
        const uint64_t bucket_key = table->key[idx];
        if (bucket_key == target_key) {
            return idx;
        }
        if (bucket_key == EMPTY_KEY) {
            return NOT_FOUND;
        }
        idx = (idx + 1) & (CAPACITY - 1);
    }
    return NOT_FOUND;
}
```

**Performance Data**:

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | Performance Comparison |
| ---- | :---------------: | :------------------------: | :--: |
| 0    |    1311.297    |          672.645           |  -   |
| 1    |    783.029    |          239.765           | 1.67x |

**Analysis**:

With the same algorithm, Task Duration drops from 1311.297μs to 783.029μs. Case 1 delivers a x1.67 improvement over Case 0; the key to the performance gain lies in how the two layouts utilize Cache Lines:

**Figure 2 Cache Line diagram**

![Cache Line diagram](figures/cacheline.png)

Figure 2 compares the Cache Line utilization of the AoS and SoA layouts: in AoS, keys and values are stored alternately, so querying a key also loads the adjacent values; in SoA, all keys are stored contiguously, and a single 128B Cache Line can hold more keys, for example 16 8B keys, thereby reducing unnecessary value data loading during lookup, lowering Cache Line transfer overhead, and improving cache utilization.

At this point, if the read width of the probing lookup can be further reduced, lowering the number of bytes needed per probe, performance will improve further. Therefore we introduce Case 2.

---

### Case 2: Digest Pre-Filtering

**Optimization Objective**: Extract local information of the key as a pre-filtering field, quickly ruling out most keys through pre-filtering, reducing the number of bytes needed for probing, and improving memory access efficiency.

**Core Implementation**: Take the high 8 bits of the key's hash value as the pre-filtering field (digest). The hash value is a deterministic function of the key — the same key produces the same hash value during table building and lookup — so a Bucket whose digest does not match definitely does not store the target key and can be skipped directly. Each round loads 4 consecutive digests aligned to 4B and performs matching, and `__ffs` extracts the bucket positions with matching digests one by one — since equal digests do not mean equal keys, the key is finally confirmed based on the matching result. Thread handling is the same as before.

**Key Code**:

```cpp
struct SoaTableWithDigest {
  uint8_t digest[CAPACITY];
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};

// digest: the high 8 bits of the key's hash value
__host__ __aicore__ inline uint8_t digest_of(uint64_t hashed) {
  return static_cast<uint8_t>(hashed >> 32);
}

// Lookup side: the target digest is replicated 4 times into a 4B word for vcmp_eq4 four-way byte-parallel comparison
__host__ __aicore__ inline uint32_t digest_word_of(uint64_t hashed) {
  return static_cast<uint32_t>(digest_of(hashed)) * 0x01010101u;
}

__aicore__ inline uint32_t find_soa_digest_vcmp(const SoaTableWithDigest* table,
                                             uint64_t target_key,
                                             uint32_t start,
                                             uint32_t target_word) {
  const uint32_t empty_word = digest_word_of(murmur3_hash(EMPTY_KEY));
  const uint32_t first_pos = start & (CAPACITY - STRIDE);  // align the starting bucket to a word
  for (uint32_t offset = 0; offset < CAPACITY + STRIDE; offset += STRIDE) {
    const uint32_t pos = (first_pos + offset) & (CAPACITY - 1);

    // Compare target digests: __ffs extracts the bucket positions with matching digests one by one —
    // equal digests do not mean equal keys, the 8B key must still be read for final confirmation
    const uint32_t probe =
        *reinterpret_cast<const uint32_t*>(table->digest + pos);
    uint32_t cmp = vcmp_eq4(probe, target_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (table->key[idx] == target_key) {
        return idx;  // Hit: return the bucket number where the key resides
      }
    }

    cmp = vcmp_eq4(probe, empty_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (offset == 0 && idx < start) {
        continue;
      }
      if (table->key[idx] == EMPTY_KEY) {
        return NOT_FOUND;
      }
    }
  }
  return NOT_FOUND;
}
```

**Performance Data**:

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | Performance Comparison |
| ---- | :---------------: | :------------------------: | :--: |
| 1    |    783.029    |          239.765           |  -   |
| 2    |    483.379     |          279.439           | 1.62x |

**Analysis**:

By adding a 1B digest field to the SoA layout and storing digests contiguously, a single Cache Line can hold more digests, reducing Cache Line transfers during lookup. Case 2 delivers a x1.62 performance improvement over Case 1. However, a memory access efficiency problem remains: each lane queries its own key, and the 4B digests it reads are scattered across different Cache Lines, making it difficult to coalesce memory access requests. The entire warp must wait for the slowest request to complete, while handling a large number of independent memory access transactions. Therefore, we introduce the cooperative probing of Case 3, letting multiple lanes team up to query the same key, reducing the number of requests through contiguous memory access and improving memory access efficiency.


---

### Case 3: Cooperative Probing

**Optimization Objective**: Combine thread group cooperative probing with the struct design of Case 2 to achieve memory access coalescing, thereby improving overall performance.

**Core Implementation**: The 32 threads of each warp are divided into 8 cooperative groups, each with 4 threads, which jointly query one key.

**Figure 3 Cooperative group lookup diagram**

![Cooperative group lookup diagram](figures/cooperative_group.png)

Figure 3 shows the process of a cooperative group querying a key: multiple threads read the contiguously stored candidate keys into registers, then generate a match bitmap through intra-group voting and select the hit thread, which is responsible for handling the hit key, for example key3, thereby reducing memory access overhead and avoiding multiple threads repeatedly processing the same result.

**Key Code**:

The following code introduces the cooperative group mechanism on top of digest pre-filtering: every 4 threads cooperatively query the same key, aggregate the matching results through intra-group voting, and the hit thread completes the subsequent processing.

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void lookup_coop_swar_kernel(
    const SoaTableWithDigest* table, const uint64_t* keys, uint32_t* indices, uint32_t query_num) {
    ...
    const uint32_t word = *reinterpret_cast<const uint32_t*>(table->digest + pos);
    uint32_t tmask = vcmp_eq4(word, target_word) & 0x01010101u;
    uint32_t emask = vcmp_eq4(word, empty_word) & 0x01010101u;
    if (round == 0) {
      const uint32_t shift = (start > pos)
          ? (kBucketsPerLane < start - pos ? kBucketsPerLane : start - pos) : 0u;
      emask &= ~static_cast<uint32_t>((1ull << (shift * 8)) - 1ull);
    }
    ...
    const uint32_t match_ballot = asc_ballot(match);
    if (((match_ballot >> group_shift) & kGroupMask) != 0) {
      found = true;
      done = true;
    }
    ...
}
```

**Performance Data**:

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | Performance Comparison |
| ---- | :---------------: | :------------------------: | :--: |
| 2    |    483.379     |          279.439           |  -   |
| 3    |    423.152    |          283.177           | 1.14x |

**Analysis**:

The performance data of the preceding Cases was measured at a load factor of 0.9, and the performance of each optimization method also varies with the load factor:

**Figure 4 Performance comparison diagram**

![Performance comparison diagram](figures/performance_comparison.png)

Figure 4 shows how the lookup performance of different optimization methods changes with the load factor. When the load factor is low, the lookup performance of cooperative probing is always lower than the other Cases; but as the load factor increases, hash collisions increase and probing sequences become longer. At this point, cooperative probing mitigates the performance degradation under high load by expanding the per-round probing range and coalescing memory access requests.

The key to Case 3 is reducing the actual number of memory access transactions while keeping the number of memory access instructions unchanged. The two probing methods differ, resulting in different memory access coalescing efficiency. In Case 2, each thread queries a different key and the accessed addresses are scattered, so 32 loads may produce up to 32 independent memory access transactions; in Case 3, each group of 4 threads accesses contiguous addresses, and 4 4B loads cover the same 16B segment of data, which can be coalesced into a single memory access transaction. Therefore, Case 3 reduces the number of memory access transactions and ultimately becomes the implementation with the lowest latency in this example.


## Performance Comparison Summary

| Case | Kernel Function | Task Duration(μs) | read_main_memory_data_all(MB) | vs. Case 0 |
| ---- | ------ | :---------------: | :------------------------: | :--------: |
| 0    | `lookup_aos_kernel` | 1311.297 | 672.645 | 1.00x |
| 1    | `lookup_soa_kernel` | 783.029 | 239.765 | 1.67x |
| 2    | `lookup_soa_digest_swar_kernel` | 483.379 | 279.439 | 2.71x |
| 3    | `lookup_coop_swar_kernel` | 423.152 | 283.177 | 3.10x |

## Tuning Recommendations

1. Memory layout optimization: When an operator involves batch access to the same local field, prefer SoA over AoS, storing the same kind of fields contiguously to reduce the loading of irrelevant data and improve Cache Line utilization.
2. Digest pre-filtering: It essentially trades a small amount of extra space for lookup time. For scenarios where a digest can be extracted, such as hash values, you can first compare digests in parallel, filter out most non-matching items, and then read the complete key for exact confirmation, reducing unnecessary memory access.
3. Cooperative probing: When queries involve long probing sequences, multiple threads can cooperatively query the same key so that each thread accesses contiguous addresses, merging scattered memory access requests into fewer transactions, thereby shortening the serial probing time and improving memory access efficiency.

## Build and Run

Run the following steps in the root directory of this example to build and execute the example.

- Configure Environment Variables

  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the Example

  Run the following commands in this example directory.

  ```bash
  SCENARIO_NUM=3                                                                     # Select the execution scenario, options 0-3
  mkdir -p build && cd build;                                                        # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project
  ./hashtable_lookup                                                                  # Run the example
  ```

  The runtime environment variable `SOA_AOS_DEVICE=<id>` can be used to specify the execution device, defaulting to 0.

  When using NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;  # NPU simulation mode
  ```

  > **Note:** `SCENARIO_NUM` is a CMake cache variable. Clear the cmake cache before switching Cases or build modes. Execute `rm CMakeCache.txt` in the build directory, then run cmake again.

- Build Options Description

  | Option                        | Values       | Description                                                       |
  | --------------------------- | ------------ | ---------------------------------------------------------- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR&950DT products) |
  | `SCENARIO_NUM`            | `0`-`3`  | Example type, default is 3. 0: AoS slot-by-slot probing baseline; 1: pure SoA layout refactoring; 2: digest pre-filtering with word batch comparison; 3: cooperative probing x SWAR combination |

  - `--cce-res-usage` outputs compilation resource information such as register and stack usage, making resource usage easier to analyze.

- Execution Result

  The execution result is as follows (the output is the same for all Cases), and the last line indicates that the accuracy verification passed.

  ```text
  [Success] Case accuracy verification passed.
  ```

## Performance Debugging

### Introduction to the msOpProf Tool

`msOpProf` is a single-operator performance analysis tool. It offers two usage methods: `msopprof` and `msopprof simulator`. The tool helps users identify anomalies in operator memory, operator code, and operator instructions, enabling comprehensive operator tuning. It currently supports performance data collection and automatic parsing for different run modes (on-device or simulation) and different file types (executables or operator binary `.o` files).

Use the `msOpProf` tool to collect detailed performance data:

```bash
msopprof ./hashtable_lookup   # Analyze case performance
```

After the command completes, a folder named `OPPROF_{timestamp}_XXX` is generated in the output directory. The performance data folder structure example is as follows:

```text
├── dump                       // Raw performance data
├── ArithmeticUtilization.csv  # cube/vector instruction cycle ratio
├── L2Cache.csv                # L2 Cache hit rate
├── Memory.csv                 # UB, L1, and main memory read/write bandwidth
├── MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth
├── MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth
├── OpBasicInfo.csv            # Operator basic information, including Task Duration(μs)
├── PipeUtilization.csv        # Compute unit and transfer unit duration and ratio
├── ResourceConflictRatio.csv  # UB bank group, bank conflict, and resource conflict ratio
└── visualize_data.bin         # MindStudio Insight presentation file
```

The Task Duration in this README is taken from `OpBasicInfo.csv`:

```bash
cat ./OPPROF_*/OpBasicInfo.csv
```

- Simulation performance collection

  Use `msopprof simulator` to perform simulation performance analysis and generate visualized instruction pipeline diagrams. The commands are as follows:

  ```bash
  SCENARIO_NUM=3                                                                     # Select the execution scenario, options 0-3
  mkdir -p build && cd build;                                                        # Create and enter the build directory
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project
  msopprof simulator --soc-version=<soc_version> ./hashtable_lookup
  ```

> Before using simulation tuning, add the `-g` compilation option to CMakeLists.txt to generate debug information, enabling the simulator to collect instruction pipeline diagrams. For how to obtain `soc_version` and for more simulation tuning information, see the [simulator sample](../../../01_utilities/07_simulator).

  After the command completes, a folder named `OPPROF_{timestamp}_XXX` is generated in the current directory. The artifact structure is as follows:

  ```text
  OPPROF_{timestamp}_XXX/
  ├── dump                    // Raw performance data, which does not require user attention
  └── simulator
      ├── core*.veccore*/     // Simulation instruction pipeline diagram files of each vector core
      └── visualize_data.bin  // MindStudio Insight presentation file
  ```

  After the command completes, open `visualize_data.bin` in **MindStudio Insight** to view the visualized instruction pipeline diagrams.

For more information about how to use `msOpProf`, see [MindStudio Tool Tuning (msOpProf) Quick Start](https://www.hiascend.com/document/detail/zh/canncommercial/900/devaids/optool/docs/zh/quick_start/msopprof_quick_start.md).
