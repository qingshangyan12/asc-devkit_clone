# Add Performance Optimization Example

## Overview

This example uses addition to demonstrate performance optimization methods based on static Tensor programming. The entire optimization process contains seven steps (case 0-6), showing the complete optimization path from scalar operation to vector operation, from single-core to multi-core, from basic implementation to deep optimization.

**Optimization Path**:
- Case 0: Single-core scalar version (baseline)
- Case 1: Single-core vector version
- Case 2: Multi-core uniform partitioning + small block transfer
- Case 3: Multi-core uniform partitioning + large block transfer
- Case 4: Multi-core uniform partitioning + double buffer optimization
- Case 5: Multi-core uniform partitioning + double buffer + L2Cache bypass
- Case 6: Multi-core uniform partitioning + double buffer + L2Cache bypass + avoid Bank Conflict

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series/Atlas A3 Inference Series
- Atlas A2 Training Series/Atlas A2 Inference Series

## Directory Structure

```
├── add_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Golden data comparison file
│   ├── CMakeLists.txt          // Build configuration file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Diagrams
│   └── add.asc                 // Ascend C example implementation (contains 7 optimization cases)
```

## Example Description

**Example Function**:

  The example implements addition of two fixed-shape matrices 8192×8192.

  Add computation formula:

$$
  z = x + y
$$

  - x: input, shape [8192, 8192], data type half
  - y: input, shape [8192, 8192], data type half
  - z: output, shape [8192, 8192], data type half

## Example Implementation

### Performance Indicator Description

**Table 1: AI Core Performance Indicator Field Description**
| Field Name | Field Meaning |
|:---:|:---|
| Task Duration(μs)|Task total duration, including time to schedule to accelerator, execution time on accelerator, and response end time.|
| aiv_time|Task theoretical execution time on AI Vector Core, unit μs.|
| aiv_vec_time(μs) | vec type instruction (vector computation instruction) duration, unit μs. |
| aiv_vec_ratio | vec type instruction (vector computation instruction) cycle count ratio in total cycle count. |
| aiv_scalar_time(μs) | scalar type instruction (scalar computation instruction) duration, unit μs. |
| aiv_scalar_ratio | scalar type instruction (scalar computation instruction) cycle count ratio in total cycle count. |
| aiv_mte2_time(μs) | mte2 type instruction (GM->UB transfer instruction) duration, unit μs. |
| aiv_mte2_ratio | mte2 type instruction (GM->UB transfer instruction) cycle count ratio in total cycle count. |
| aiv_mte3_time(μs) | mte3 type instruction (UB->GM transfer instruction) duration, unit μs. |
| aiv_mte3_ratio | mte3 type instruction (UB->GM transfer instruction) cycle count ratio in total cycle count. |

This section performance data runs on Atlas A2 Training Series products.

### Case 0: Single-core Scalar Version (Baseline Program)

**Implementation**: Refer to `KernelAdd::ProcessScalar()` function implementation

Baseline program implements addition of two groups of `half` type input data, using `for` loop `scalar` operation for computation.

**Key Code**:
```cpp
for (uint32_t i = 0; i < curLen; i++) {
      float xVal = (float)xLocal.GetValue(i);
      float yVal = (float)yLocal.GetValue(i);
      zLocal.SetValue(i, (half)(xVal + yVal));
    }
```

**Example Configuration**:
- Single-core scalar operation
- `dataCopyLen = 4096` is data amount element count per transfer
- Single transfer data amount is 4096 * 2B = 8192 Byte, single scalar processing data amount is 4 Byte

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1239689.1 | 1239688.63 | 0.015 | 0 | 1233742.494 | 0.995 | 5916.341 | 0.005 | 2485.465 | 0.002 |

**Optimization Effect Analysis**:
- End-to-end duration: **1239689.1μs** (approximately 1.24 seconds)
- Scalar instruction duration: 1233742.494μs, ratio **99.5%**
- Vector instruction duration: 0.015μs, ratio close to 0%
- Performance bottleneck: Scalar operation executes serially, cannot utilize hardware parallel capability. This scenario serves only as Add operation performance comparison example. Users should not use Scalar operation in actual business scenarios.

**Principle Explanation**:
- Scalar operation can only process 1 data element at a time, requires element-by-element loop
- AI Core hardware advantage lies in vector/matrix parallel computation, scalar operation cannot leverage hardware capability

**Performance Optimization Recommendation**:
> ⚠️ **Avoid scalar loops, use vector instructions**
> 
> In Ascend C programming, avoid using `for` loop with `GetValue/SetValue` scalar operation. Use vector instructions such as `AscendC::Add` to achieve orders-of-magnitude performance improvement.

---

### Case 1: Single-core Vector Version

**Implementation**: Refer to `KernelAdd::ProcessSingle()` function implementation

Convert scalar operation to vector operation, use `AscendC::Add` vector instruction to replace scalar loop, significantly improving computational efficiency.

**Key Code**:
```cpp
AscendC::Add(zLocal, xLocal, yLocal, curLen);
```

**Example Configuration**:
- Single-core operation
- `dataCopyLen = 4096` is data amount element count per transfer
- Single `DataCopy` transfer data amount is 8192 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 16384 Byte


**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 6909.6 | 6909.14 | 761.65 | 0.11 | 231.166 | 0.033 | 6208.762 | 0.873 | 2613.205 | 0.378 |

**Optimization Effect Analysis**:
- End-to-end performance: 6909.6μs, improved **99.4%** compared to Case 0
- Scalar instruction duration: from 1233742.494μs to 231.166μs, significantly reduced scalar instructions
- Vector instruction duration: 761.65μs, ratio 11%
- Data transfer duration: 6208.762μs, ratio 87.3%, transfer pipeline serial improvement

**Principle Explanation**:
- Vector instruction can process multiple data elements at a time (this example processes 4096*2 half elements at a time)
- Vector unit parallel computation capability far exceeds scalar unit
- But data transfer becomes bottleneck, indicating computation speed exceeds data supply. Single-core has insufficient transfer request amount, causing bandwidth underutilization.

**Performance Optimization Recommendation**:
> 💡 **Use vector instructions to replace scalar loops**
> 
> Use vector APIs such as `AscendC::Add`, `AscendC::Mul` to replace element-by-element scalar loops. Fully utilize AI Core vector computation unit to achieve hundred-fold or more performance improvement.

> 💡 **Users should not use single-core only**

**Next Optimization Direction**:
- Data transfer (MTE2) ratio 87.3%, becomes main bottleneck
- Need to improve bandwidth utilization through multi-core parallelism and larger transfer granularity

---

### Case 2: Multi-core Uniform Partitioning + Small Block Transfer

**Implementation**: Refer to `KernelAdd::Process()` function implementation

Enable multi-core parallel computation, partition 8192×8192 matrix to multiple AIV Core for parallel processing, using uniform partitioning strategy.

**Example Configuration**:
- Row direction partition into 48 parts, evenly distribute data to 48 cores for computation
- `dataCopyLen = 4096` is data amount element count per transfer
- Single `DataCopy` transfer data amount is 4096 * 2B = 8192 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 16384 Byte


**Key Code**:
```cpp
// Uniform partitioning calculates rows per core (M direction)
uint32_t baseCoreM = totalM / splitM;
uint32_t remainderM = totalM % splitM;
if (blockIdxM < remainderM) {
    actualCoreM = baseCoreM + 1;  // Evenly distribute remainder
    startM = blockIdxM * actualCoreM;
} else {
    actualCoreM = baseCoreM;
    startM = remainderM * (baseCoreM + 1) + (blockIdxM - remainderM) * baseCoreM;
}
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 306.58 | 297.14 | 15.885 | 0.053 | 8.219 | 0.028 | 215.485 | 0.725 | 54.33 | 0.183 |

**Optimization Effect Analysis**:
- End-to-end task duration 306.58μs, reduced **95.5%** compared to Case 1
- Data transfer MTE2 duration 215.485μs, ratio 72.5%

**Principle Explanation**:
- 48 AIV Core parallel processing, theoretically can achieve 48x acceleration
- Reasons actual acceleration below theoretical value:
  - Data transfer is still bottleneck (mte2 ratio 72.5%)
- Uniform partitioning ensures each core has balanced load, avoids load imbalance where some cores idle and some cores busy

- This example chooses M direction partition into 48 parts (`splitM=48, splitN=1`) instead of M partition 8, N partition 6. The core purpose is to maintain data per core continuous on GM, enabling single `DataCopy` to transfer `dataCopyLen` in block loop.

  <img src="figures/SplitCoreM.png" width="50%">

**Performance Optimization Recommendation**:
> 💡 **Fully utilize multi-core parallelism, adopt uniform partitioning strategy**
> 
> 1. Evenly partition data to multiple AIV Core for parallel computation
> 2. Use uniform partitioning strategy (remainder distributed to first few cores), ensure load balance
> 3. Partition granularity needs to consider: core count, data amount, UB space size

**Next Optimization Direction**:
- MTE2 ratio 72.5%, MTE3 ratio 18.3%, transfer still bottleneck
- Computation only 5.3%, indicating "computation fast, transfer slow"
- Can improve bandwidth utilization by increasing single transfer data amount
---

### Case 3: Multi-core Uniform Partitioning + Large Block Data Transfer

**Implementation**: Refer to `KernelAdd::Process()` function implementation

To fully utilize bandwidth resources, increase transfer instruction data amount.

**Example Configuration**:
- Row direction partition into 48 parts, evenly distribute data to 48 cores for computation
- `dataCopyLen = 16384` is data amount element count per partition (4x compared to Case2)
- Single `DataCopy` transfer data amount is 32768 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 65536 Byte


**Performance Data**:

| Task Duration(μs)  | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 268.5 |261.5 | 12.845 | 0.049 | 2.833 | 0.011 | 184.369 | 0.705 | 57.354 | 0.219 |

**Optimization Effect Analysis**:
- End-to-end performance: 268.5μs, reduced **12.5%** compared to Case 2
- By increasing single transfer data amount, MTE2 duration from 215.485μs to 184.369μs, reduced **14.4%**

**Principle Explanation**:
- Increasing single transfer data amount reduces transfer count
- Larger continuous data blocks better utilize memory bandwidth
- But data amount is limited by UB space size (this example UB needs to hold x, y, z three copies of data)

**Performance Optimization Recommendation**:
> 💡 **Increase single data transfer amount, reduce transfer count**
> 
> 1. Within UB space allowance, maximize `dataCopyLen`
> 2. Use continuous large block data transfer, avoid frequent small data block transfer
> 3. Need to balance UB space usage and transfer efficiency


> ⚠️ **Note: dataCopyLen is not necessarily larger is better**
> 
> Based on Case 3, if further increase dataCopyLen (for example from 16384 to 16512), end-to-end performance basically unchanged (268.5μs vs 267.76μs). Recommend combining total data amount, UB space, alignment requirements to determine optimal dataCopyLen value.

---

### Case 4: Double Buffer Optimization

**Implementation**: Refer to `KernelAdd::ProcessDoubleBuffer()` function implementation

Adopt double buffer technique to achieve pipeline parallelism between data transfer and computation, hiding memory access latency.

**Key Code**:
```cpp
// Ping-Pong double buffer addresses
static constexpr uint32_t xAddrPing = 0;
static constexpr uint32_t yAddrPing = MAX_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t zAddrPing = yAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t xAddrPong = zAddrPing + MAX_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t yAddrPong = xAddrPong + MAX_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t zAddrPong = yAddrPong + MAX_DATA_COPY_LEN * sizeof(half);

// Double buffer pipeline: alternate using two event IDs and two buffer groups
for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
    int32_t eventID = (loopIdx % 2 == 0 ? EVENT_ID0 : EVENT_ID1);
    AscendC::LocalTensor<half> &xLocal = (loopIdx % 2 == 0 ? xPing : xPong);
    // ... data transfer and computation, use corresponding eventID for synchronization
    AscendC::Add(zLocal, xLocal, yLocal, curLen);
}
```

**Example Configuration**:
- Row direction partition into 48 parts, evenly distribute data to 48 cores for computation
- `dataCopyLen = 16384` is data amount element count per partition
- Single `DataCopy` transfer data amount is 32768 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 65536 Byte
- Divide pending data into two parts, enabling parallel execution of data transfer and Vector computation

**Memory Layout**:

```
UB Memory Allocation (Double Buffer):
┌──────────────┐
│  xPing       │  0x00000
│  16384*2B    │
├──────────────┤
│  yPing       │  0x08000 (32768)
│  16384*2B    │
├──────────────┤
│  zPing       │  0x10000 (65536)
│  16384*2B    │
├──────────────┤
│  xPong       │  0x18000 (98304)
│  16384*2B    │
├──────────────┤
│  yPong       │  0x20000 (131072)
│  16384*2B    │
├──────────────┤
│  zPong       │  0x28000 (163840)
│  16384*2B    │
└──────────────┘
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 264.02 | 257.56 | 12.846 | 0.05 | 2.796 | 0.011 | 250.528 | 0.973 | 84.988 | 0.33 |

**Optimization Effect Analysis**:
- End-to-end performance: 264.02μs, reduced **1.7%** compared to Case 3
- MTE2 duration from 184.369μs to 250.528μs (+35.9%), MTE3 duration from 57.354μs to 84.988μs (+48.2%). This changes from serial pure read bandwidth to mixed read-write bandwidth, so duration increases. Users should focus more on end-to-end duration reduction.
- Due to enabling double buffer, transfer and computation execute in parallel in pipeline, hiding data transfer time and reducing Vector instruction wait time.

**Principle Explanation**:
- **Ping-Pong Mechanism**:
  - When Ping buffer performs computation, Pong buffer performs data transfer
  - Alternate execution, achieving pipeline parallelism between computation and transfer, as shown below
    <img src="figures/DoubleBuffer.png" width="50%">

**Performance Optimization Recommendation**:
> 💡 **Use double buffer to achieve transfer and computation parallelism**
> 
> 1. Double buffer has maximum benefit when computation and transfer times are similar
> 2. Requires sufficient UB space (approximately 2x single buffer space)
> 3. Use independent Event ID to manage synchronization of two buffer groups

**Next Optimization Direction**:
- Double buffer benefit is limited, indicating bottleneck lies in transfer speed itself
- Can try L2 Cache optimization to improve transfer efficiency
---

### Case 5: Double Buffer + L2 Cache bypass

**Implementation**: Refer to `KernelAdd::ProcessDoubleBufferL2Bypass()` function implementation (first set `SetL2CacheHint(CACHE_MODE_DISABLE)` inside this function, then call `ProcessDoubleBuffer()`)

On double buffer basis, for data that needs loading only once, set L2 Cache bypass to load directly from GM to UB.

**Key Code**:
```cpp
  xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
  yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
```

**Example Configuration**:
- Row direction partition into 48 parts, evenly distribute data to 48 cores for computation
- `dataCopyLen = 16384` is data amount element count per partition
- Single `DataCopy` transfer data amount is 32768 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 65536 Byte
- Divide pending data into two parts, enabling parallel execution of data transfer and Vector computation

**L2 Cache Strategy**:
- xGm: Disable L2 Cache (one-time read)
- yGm: Disable L2 Cache (one-time read)

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 187.68 | 185.15 | 12.846 | 0.069 | 4.479 | 0.024 | 175.997 | 0.951 | 160.291 | 0.866 |

**Optimization Effect Analysis**:
- End-to-end performance: 187.68μs, reduced **28.9%** compared to Case 4
- MTE2 duration: from 250.528μs to 175.997μs, reduced **29.8%**
- Vector instruction duration: 12.846μs, unchanged

**Principle Explanation**:
- **L2 Cache Function**:
  - L2 Cache is cache layer between AI Core and HBM
  - Repeatedly accessed data can be read from L2 Cache, faster speed
- **Stream Access Characteristic**:
  - Add input data is read only once, no data reuse

**Performance Optimization Recommendation**:
> 💡 **Reasonably adopt L2 Cache bypass**
> 
> 1. For input data read only once (such as x, y in this example), set `SetL2CacheHint(CACHE_MODE_DISABLE)`
> 2. For data requiring repeated access (such as convolution weights), retain L2 Cache
> 3. Recommend users configure optimization based on actual test data. In actual model and training scenarios, need to reasonably configure considering upstream and downstream

**Next Optimization Direction**:
- Transfer efficiency improved, but vector instruction efficiency still has optimization space
- Can try optimizing UB memory layout to avoid Bank Conflict
---

### Case 6: Double Buffer + L2 Cache bypass + Avoid Bank Conflict

**Implementation**: Refer to `KernelAdd::ProcessDoubleBufferBankConflict()` function implementation

On double buffer + L2 Cache bypass basis, optimize memory address layout to avoid UB (Unified Buffer) Bank Conflict, achieving optimal performance.

**Key Code**:
```cpp
// Set L2 Cache bypass
xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);

// Optimized address layout (avoid Bank Conflict)
static constexpr uint32_t xAddrPingBC = 0;
static constexpr uint32_t yAddrPingBC = BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t xAddrPongBC = MAX_DATA_COPY_LEN * sizeof(half) * 2;
static constexpr uint32_t yAddrPongBC = xAddrPongBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
static constexpr uint32_t zAddrPingBC = MAX_DATA_COPY_LEN * sizeof(half) * 4;
static constexpr uint32_t zAddrPongBC = zAddrPingBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
```

**Example Configuration**:
- Row direction partition into 48 parts, evenly distribute data to 48 cores for computation
- `dataCopyLen = 16256` is data amount element count per partition
- Single `DataCopy` transfer data amount is 32512 Byte
- Single `Add` processes two input `Tensor`, total processed data amount is 65204 Byte
- Divide pending data into two parts, enabling parallel execution of data transfer and Vector computation

**Memory Layout Optimization**:

For Atlas A2/A3 series products, UB size is 192KB, containing 16 Bank Groups. Each Bank Group contains 3 Banks. Each Bank size is 4KB, composed of 128 rows. Each row length is 32B.

UB Bank memory layout before optimization (that is case5)
<img src="figures/UBBankConflict.png" width="100%">

This shows simultaneous read-write conflict within one bank, read-read conflict and write-write conflict within one bankgroup.

UB Bank memory layout after optimization (case6)
<img src="figures/UBBankConflictResolution.png" width="100%">

Since vec instruction reads 256B data in one cycle (that is simultaneously reads 8 block data), as shown above xping and yping starting addresses are offset by 256B, effectively eliminating ub bank conflict.

**Bank Conflict Explanation**:
- UB is divided into multiple Bank Groups. Simultaneous read/write to same Bank Group causes conflict
- By adjusting dataCopyLen (16384→16256), data starting address has offset
- Ensure data accessed by vec instruction in one cycle distributes across different Bank Groups

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 184.52 | 178.49 | 6.954 | 0.039 | 3.424 | 0.019 | 171.611 | 0.961 | 121.442 | 0.68 |

**Optimization Effect Analysis**:
- End-to-end performance: 184.52μs, reduced **1.7%** compared to Case 5
- Vector instruction duration: from 12.846μs to 6.954μs, reduced **45.9%**
- MTE3 duration: from 160.291μs to 121.442μs, reduced **24.2%**

**Principle Explanation**:
- **Bank Conflict Problem**:
  - UB (Unified Buffer) is divided into multiple Bank Groups
  - If data read/written by vector instruction at one time falls in same Bank, causes read-write conflict
  - If data read/written by vector instruction at one time falls in same Bank Group, causes read-read conflict or write-write conflict
  - Bank Conflict causes memory access serialization, reducing vector instruction efficiency
- **Solution**:
  - Reduce dataCopyLen (16384→16256), making data starting address have offset
  - Redesign memory layout, ensuring data accessed at same time distributes across different Banks

**Performance Optimization Recommendation**:
> 💡 **Optimize UB memory layout, avoid Bank Conflict**
> 
> 1. When `aiv_vec_time` is abnormally high, may have Bank Conflict
> 2. By adjusting dataCopyLen or memory layout offset, make data distribute across different Bank Groups
> 3. Bank Conflict optimization has obvious benefit in vector-bound scenarios

**Final Performance Summary**:
- Compared to baseline Case 0: Performance improvement **6711x** (1239689.1μs → 184.52μs)
- Compared to single-core vector Case 1: Performance improvement **37.3x** (6909.6μs → 184.52μs)


---

## Performance Comparison Summary

### Atlas A2 Training Series Performance Comparison

The following table shows performance data comparison for this example on Atlas A2 Training Series products:

| Case | Optimization Strategy | Core Count | dataCopyLen | Task Duration(μs) | aiv_vec_time(μs) | Theoretical vector duration(μs) | Read Bandwidth(TB/S) | Read-write Mixed Bandwidth(TB/S) | End-to-end Duration Relative to Case 0 |
|:---:|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0 | Single-core scalar (baseline) | 1 | 4096 | 1239689.1 | NA | 283.405 | 0.0454 | NA | 1x |
| 1 | Single-core vector | 1 | 4096 | 6909.6 | 761.65 | 283.405 | 0.0432 | NA | 179.4x |
| 2 | Multi-core uniform partitioning | 48 | 4096 | 306.58 | 15.885 | 5.904 | 1.2457 | NA | 4043.6x |
| 3 | Increase transfer granularity | 48 | 16384 | 268.5 | 12.845 | 5.904 | 1.4560 | NA | 4617.1x |
| 4 | Double buffer | 48 | 16384 | 264.02 | 12.846 | 5.904 | NA | 1.6072 | 4695.4x |
| 5 | L2 Cache bypass | 48 | 16384 | 187.68 | 12.846 | 5.904 | NA | 2.2755 | 6612.2x |
| 6 | Bank Conflict optimization | 48 | 16256 | 184.52 | 6.954 | 5.904 | NA | 2.3456 | 6710.7x |

The "theoretical vector duration" in the table represents theoretical execution time considering only Vector computation itself under current core count configuration. This example performance data runs on Atlas A2 Training Series products. This processor processes 128 half data per cycle, with main frequency 1.85GHz. Theoretical vector duration calculation formula is
$$
T_{\text{theory}} = \frac{M \times N}{128 \times 1.85 \times 10^9 \times \text{core count}}
$$

For example, in 48-core scenario:
$$
T_{\text{theory}} = \frac{8192 \times 8192}{128 \times 1.85 \times 10^9 \times 48} = \frac{67108864}{1.13664 \times 10^{13}} \approx 5.904 \times 10^{-6} \text{ s} = 5.904 \text{ μs}
$$

Case 6 aiv_vec_time is 6.954 μs, already very close to theoretical duration in 48-core scenario.

Case 0-3 does not enable double buffer, data transfer executes serially, use read bandwidth to measure performance. Starting from Case 4, double buffer is enabled, then mte2 utilization is high, read-write behavior occurs in parallel, therefore read-write mixed bandwidth is estimated by total read-write data amount divided by $T_{mte2}$. Read bandwidth calculation formula is:
$$
BW_{read} = \frac{D_{read}}{T_{mte2}}
$$

Read-write mixed bandwidth calculation formula is:
$$
BW_{rw} = \frac{D_{read} + D_{write}}{T_{mte2}}
$$

Where:
- $D_{read} = M \times N \times sizeof(half) \times 2$ is total read data amount (x and y two input matrices)
- $D_{write} = M \times N \times sizeof(half)$ is total write data amount (z output matrix)
- $T_{mte2}$ is aiv_mte2_time (GM→UB transfer duration, μs)
- After enabling double buffer, mte2 and mte3 pipeline parallel; from this example data, Case 4-6 mte2 utilization is 97.3%, 95.3%, 95.2% respectively, therefore here use $T_{mte2}$ as main path time for estimation

Take Case 3 as example ($M=N=8192$, $T_{mte2}=184.369\mu s$):
$$
BW_{read} = \frac{8192 \times 8192 \times 2 \times 2}{184.369 \times 10^{-6}} = \frac{268435456}{184.369 \times 10^{-6}} \approx 1.4560 \times 10^{12} \text{ B/s} \approx 1.4560 \text{ TB/s}
$$

Take Case 6 as example ($T_{mte2}=171.611\mu s$):
$$
BW_{rw} = \frac{8192 \times 8192 \times (2+1) \times 2}{171.611 \times 10^{-6}} = \frac{402653184}{171.611 \times 10^{-6}} \approx 2.3456 \times 10^{12} \text{ B/s} \approx 2.3456 \text{ TB/s}
$$

The reason read-write mixed bandwidth exceeds 1.8 TB/s is because this statistics is not pure read bandwidth, but "read + write" mixed bandwidth. After enabling double buffer, read-write pipeline is parallel; meanwhile, z write data hits L2 Cache, and L2 bandwidth is high, so write bandwidth is high. Therefore, when using mte2 as read-write mixed time for estimation, numerator counts read-write total data amount, final mixed bandwidth exceeds 1.8 TB/s.

### Ascend 950 Series Performance Comparison

The following table shows performance data comparison for this example on Ascend 950 series products:

| Case | Optimization Strategy | Core Count | dataCopyLen | Task Duration(μs) | aiv_vec_time(μs) | Theoretical vector duration(μs) | Read Bandwidth(TB/S) | Read-write Mixed Bandwidth(TB/S) | End-to-end Duration Relative to Case 0 |
|:---:|:---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0 | Single-core scalar (baseline) | 1 | 4096 | 1228475.435 | NA | 317.750 | 0.034 | NA | 1x |
| 1 | Single-core vector | 1 | 4096 | 8924.021 | 943.362 | 317.750 | 0.035 | NA | 137.7x |
| 2 | Multi-core uniform partitioning | 64 | 4096 | 310.489 | 15.465 | 4.965 | 1.184 | NA | 3956.6x |
| 3 | Increase transfer granularity | 64 | 21760 | 251.684 | 11.121 | 4.965 | 1.454 | NA | 4881.0x |
| 4 | Double buffer | 64 | 21760 | 247.665 | 10.976 | 4.965 | NA | 1.712 | 4960.2x |
| 5 | L2 Cache bypass | 64 | 21760 | 182.04 | 10.997 | 4.965 | NA | 2.256 | 6748.4x |

The main reason for increased `dataCopyLen` on Ascend 950 series is UB capacity increased from 192KB to 256KB. This example needs to simultaneously place `x/y/z` Ping-Pong total 6 buffer blocks in double buffer scenario, so single block available space can be approximated as `UBSIZE/6`:
- Atlas A2/A3 series: `192KB / 6 = 32KB`, corresponding to `32KB / 2B = 16384` `half`
- Ascend 950 series: `256KB / 6 ≈ 42.67KB`, corresponding to approximately `21840` `half`

Therefore on Ascend 950 series, based on capacity upper limit estimation, single block `dataCopyLen` can be increased to approximately `21840` `half` data.
In actual implementation, recommend users align transfer granularity and computation granularity to 512B for more stable performance, so case3-5 uses `dataCopyLen=21760`, corresponding byte count is `21760 * 2B = 43520B = 85 * 512B`.

On Ascend 950 series products, BANK arrangement is different, this example temporarily does not need to consider Bank conflict, so only lists performance data for case0~case5. This processor processes 128 half data per cycle, with main frequency 1.65GHz, AIV core count is 64, so theoretical vector duration is
$$
T_{\text{theory}} = \frac{8192 \times 8192}{128 \times 1.65 \times 10^9 \times 64} = \frac{67108864}{1.35168 \times 10^{13}} \approx 4.965 \times 10^{-6} \text{ s} = 4.965 \text{ μs}
$$

Reasons Ascend 950 series does not reach theoretical vector duration peak:

- This theoretical value only counts "pure Vector computation" time, does not include data transfer, event synchronization, pipeline scheduling and other overhead; while actual Case 5 `aiv_vec_time=10.997μs`, Task Duration is `182.04μs`, `aiv_mte2_reatio` is as high as 98.3%, limited by bandwidth constraint, indicating end-to-end bottleneck is not pure computation itself.
- Ascend 950 series RegBase main benefit comes from "reduce redundant Load/Store, register reuse, increase no-dependency concurrent instruction ratio". Current Add example core computation is basically single Add, computation chain is short, limited fusion/dual-issue space, difficult to fully release RegBase advantage.

### Optimization Key Points Summary

| Optimization Method | Core Principle | Applicable Scenario |
|:---|:---:|:---|
| Scalar→Vector | Vector instruction parallel processes multiple elements | All computation-intensive operators |
| Single-core→Multi-core | Multi-core parallelism, load balance | Large data amount scenarios |
| Increase transfer granularity | Reduce transfer count, amortize startup overhead | Transfer-intensive scenarios |
| Double buffer | Transfer and computation pipeline parallelism | Computation and transfer time similar |
| L2 Cache bypass | Avoid Cache pollution, reduce overhead | Stream access (read once only) |
| Bank Conflict optimization | Optimize memory layout, avoid memory access conflict | Vector-bound scenarios |

---

## Build and Run

- Switch Case

  Specify case to build through `-DSCENARIO=N` during cmake compilation:

  ```bash
  cmake -DSCENARIO=6 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # Build case 6 (can replace with 0-6)
  ```

  Each case description:
  - `0`: Single-core scalar version
  - `1`: Single-core vector version
  - `2`: Multi-core uniform partitioning (dataCopyLen=4096)
  - `3`: Multi-core uniform partitioning (large block data transfer)
  - `4`: Double buffer optimization
  - `5`: Double buffer + L2Cache bypass
  - `6`: Double buffer + L2Cache bypass + avoid Bank Conflict

- Configure Environment Variables
  Choose the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
  - Default path, root user installation
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installation
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, CANN software package installation
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- Example Execution
  ```bash 
  mkdir -p build && cd build;   # Create and enter build directory
  cmake -DSCENARIO_NUM=6 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # Build project, default npu mode
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                           # Execute (use case specified at build time)
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output result correctness, confirm algorithm logic correctness
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DSCENARIO_NUM=6 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DSCENARIO_NUM=6 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear cmake cache before switching build modes. Execute `rm CMakeCache.txt` in build directory and re-run cmake.
  

- Build Option Description
  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series/Atlas A2 Inference Series and Atlas A3 Training Series/Atlas A3 Inference Series, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0`(default), `1`, `2`, `3`, `4`, `5`, `6` | Performance optimization case number |
  
  The following execution result indicates successful precision comparison.
  ```bash
  error ratio: 0.0000, tolerance:0.0001
  test pass!
  ```

## Performance Analysis

Use `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze performance
```

Current directory generates PROF_-prefix folder. `mindstudio_profiler_output` directory saves Host and each Device performance data summary. Performance data analysis recommends viewing files in this directory.

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Saves Host and each Device performance data summary
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
View specific performance analysis results:
```
# View Task Duration and various data
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```