---
name: c_api 
description: 通过AscendC C API编程语言编写对应的测试样例, 测试样例仅考虑单核上的数据搬运、计算、搬出，且除了必要的搬运操作外，每次只测试单独的一个API接口。本技能仅考虑NPU架构为3510场景的AscendC C API测试样例编写。当用户提到C API样例时，应使用此技能。
---

# Ascend 3510 NPU 架构 C API 技能文档

## 概述

3510 C API（适用于 Ascend 950PR/950DT）为 NPU（神经网络处理器）提供了底层的、面向硬件的编程接口。
这些 API 是对编译器 CCE（Cube Compute Engine）内部指令的薄封装，提供了 C 语言编程体验，支持数组分配内存，可直接控制
数据搬运、矢量/矩阵计算、同步以及系统状态。

本文档涵盖 3510 架构 C API（即 `npu_arch_3510`），兼容 Ascend 950PR 和 Ascend 950DT 硬件。

---

## 重要限制

- 注意函数标识符和变量类型的匹配
- vector_int, vector_float 等类型仅在__simd_vf__中使用，为寄存器RegTensor的数据类型
- __simd_vf__ 仅支持在vector core中执行
- vector_bool类型用于在__simd_vf__中控制参与计算的元素位置
- vector_bool类型采用正常的变量声明方式，使用方式参考[./examples/example.md](./examples/example.md)中的RegBase编程模板。
- 3510架构对于Vector类型的运算，均需要搬入到寄存器中进行计算，计算完成后再搬出到GM。即数据通路为gm -> ub -> reg -> ub -> gm。

## 快速入门

### 头文件引入

```c
#include "c_api/asc_simd.h"
```

单个头文件即可包含所有 C API 接口。也可以分别引入各类别头文件（如 `vector_compute.h`、`cube_datamove.h`、`sync.h`）。

### Kernel 函数结构

典型的 C API Kernel 遵循以下模式：

```c
__vector__ __global__ __aicore__ void kernel_name(__gm__ float* input, __gm__ float* output)
{
    // 步骤1：初始化 NPU 状态
    asc_init();

    // 步骤2：声明本地 UB 缓冲区（仅支持编译期常量大小）
    __ubuf__ float local_buf[BUFFER_SIZE];

    // 步骤3：计算每个 block 的工作量
    uint32_t blockLength = total_size / asc_get_block_num();

    // 步骤4：搬运数据 GM -> UB
    asc_copy_gm2ub_sync(local_buf, input + asc_get_block_idx() * blockLength,
                        blockLength * sizeof(float));

    // 步骤5：在 UB 中执行计算
    asc_add_sync(local_buf, local_buf, local_buf, blockLength);

    // 步骤6：搬运结果 UB -> GM
    asc_copy_ub2gm_sync(output + asc_get_block_idx() * blockLength, local_buf,
                        blockLength * sizeof(float));
}
```

### Host 侧启动流程

```c
// 初始化 ACL 运行时
aclInit(nullptr);
aclrtSetDevice(0);
aclrtCreateStream(&stream);

// 分配 device 内存
aclrtMalloc(&xDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
aclrtMalloc(&yDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);

// 拷贝数据 host -> device
aclrtMemcpy(xDevice, totalByteSize, xHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);

// 启动 kernel：<<<numBlocks, nullptr, stream>>>
kernel_name<<<numBlocks, nullptr, stream>>>(xDevice, yDevice);

// 同步并回读
aclrtSynchronizeStream(stream);
aclrtMemcpy(zHost, totalByteSize, zDevice, totalByteSize, ACL_MEMCPY_DEVICE_TO_HOST);

// 清理
aclrtFree(xDevice);
aclrtDestroyStream(stream);
aclrtResetDevice(0);
aclFinalize();
```

### CMakeLists.txt

```cmake
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(my_kernel my_kernel.asc)
target_compile_options(my_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>)
```

---

## 核心概念

### 存储修饰符

| 修饰符    | 地址空间               | 对齐要求 |
|-----------|------------------------|----------|
| `__gm__`  | Global Memory（DDR/HBM）| 无       |
| `__ubuf__`| Unified Buffer（片上）  | 32B      |
| `__cbuf__`| L1 Buffer（片上）       | 32B      |
| `__ca__`  | L0A Buffer（矩阵左操作数）| 512B    |
| `__cb__`  | L0B Buffer（矩阵右操作数）| 512B    |
| `__cc__`  | L0C Buffer（矩阵结果）   | 64B      |

### 流水线类型

NPU 具有多条独立的执行流水线：

| 流水线     | 用途                                                |
|------------|-----------------------------------------------------|
| `PIPE_S`   | 标量流水线：指令分发、标量计算                        |
| `PIPE_V`   | 矢量计算流水线                                       |
| `PIPE_M`   | 矩阵计算流水线（Cube 单元）                          |
| `PIPE_MTE1`| 数据搬运：L1->L0A/L0B、L1->UB、L0 初始化            |
| `PIPE_MTE2`| 数据搬运：GM->L1、GM->L0A/L0B、GM->UB、L1 初始化    |
| `PIPE_MTE3`| 数据搬运：UB->GM、UB->L1                             |
| `PIPE_FIX` | Fixpipe（量化、激活函数）                            |
| `PIPE_ALL` | 所有流水线                                           |

### 同步 vs 异步 API

大多数 API 有三种变体：

- **`asc_xxx(...)`** — 异步：发出指令但不等待完成。配合手动的 `asc_sync_notify`/`asc_sync_wait` 实现流水线级控制。
- **`asc_xxx_sync(...)`** — 同步：自动插入同步操作，更简单但灵活性较低。
- **高维切分** — 带跨步的数据访问，通过 `repeat`、`dataBlockStride` 和 `repeatStride` 参数控制。

### 三种 API 模式

1. **前 N 个数据计算**：从起始位置连续获取数据，通过 `count` 参数化。
2. **高维切分计算**：受控的跨步访问，由 `repeatTime`（最大 255）、`dataBlockStride`、`repeatStride` 控制。
3. **同步计算**：带 `_sync` 后缀的自动同步变体。

### DataBlock 与 Repeat

- 每条矢量指令每次迭代处理 **8 个 DataBlock**（每个 32B，共 256B）。
- `repeatTime` 控制迭代次数（1-255）。
- `dataBlockStride`：单次迭代内不同 DataBlock 之间的步长（1 = 连续）。
- `repeatStride`：相邻迭代间相同 DataBlock 位置的步长（8 = 连续）。

### 掩码操作

按位控制哪些元素参与计算：
- **16 位类型**：掩码使用 2 个 `uint64_t`（`mask[2]`）
- **32 位类型**：掩码使用 1 个 `uint64_t`（`mask[0]`）
- **64 位类型**：使用 `mask[0]` 的前 32 位

计算前设置掩码：
```c
uint64_t mask[2] = {0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL};
asc_set_vector_mask(mask[1], mask[0]);
```

### 内存分配

UB 缓冲区必须使用编译期常量数组大小：
```c
constexpr uint32_t SIZE = 1024;
__ubuf__ float buf[SIZE];  // 正确

// 不支持：
// __ubuf__ float buf[dynamic_size];    // 动态长度
// __ubuf__ float buf[rows][cols];      // 多维数组
// __ubuf__ float* ptr = asc_get_phy_buf_addr(...); // 与数组方式混用
```

### 地址重叠约束

- **单次迭代内**：源操作数与目的操作数必须 100% 完全重叠（不支持部分重叠）。
- **多次迭代间**：不支持前序迭代的目的操作数与后序迭代的源操作数重叠。
  - 例外：对于 half/int32_t/float 类型的 `asc_add`、`asc_sub`、`asc_mul`，
    当目的操作数与 `src1` 重叠，且 `src1RepStride` 或 `dstRepStride` 为 0 时，支持跨迭代重叠。

---

## API 分类概览

| 类别              | 头文件                 | 流水线          | 说明                                           |
|-------------------|------------------------|-----------------|-----------------------------------------------|
| 初始化            | `misc.h`               | PIPE_S          | `asc_init()` — 初始化 NPU 状态                 |
| 矢量计算          | `vector_compute.h`     | PIPE_V          | 逐元素运算：加、乘、relu、exp 等               |
| 矩阵计算          | `cube_compute.h`       | PIPE_M          | 矩阵乘加（`asc_mmad`）                         |
| 矢量数据搬运      | `vector_datamove.h`    | PIPE_MTE2/3     | GM<->UB、UB<->L1、寄存器加载/存储              |
| Cube 数据搬运     | `cube_datamove.h`      | PIPE_MTE1/2/3   | GM<->L1、L1<->L0A/L0B/L0C、初始化填充         |
| 标量计算          | `scalar_compute.h`     | PIPE_S          | 位操作：popc、clz、ffs 等                     |
| 同步控制          | `sync.h`               | PIPE_S          | 流水线同步、核间同步、屏障                     |
| 系统变量          | `sys_var.h`            | PIPE_S          | 读取芯片状态：核数、cycle、寄存器              |
| 缓存控制          | `cache_ctrl.h`         | PIPE_S          | 数据/指令缓存刷新和预取                        |
| 原子操作          | `atomic.h`             | PIPE_S          | GM 写入的原子加/最大/最小操作                  |

---

## 常用使用模式

### 模式 1：简单同步计算（推荐初学者）

```c
asc_init();
__ubuf__ float a[N], b[N], c[N];
asc_copy_gm2ub_sync(a, gm_a + offset, N * sizeof(float));
asc_copy_gm2ub_sync(b, gm_b + offset, N * sizeof(float));
asc_add_sync(c, a, b, N);
asc_copy_ub2gm_sync(gm_c + offset, c, N * sizeof(float));
```

### 模式 2：异步 + 手动同步

```c
asc_copy_gm2ub(ub_buf, gm_buf, size);             // PIPE_MTE2
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_add(ub_dst, ub_src0, ub_src1, count);          // PIPE_V
asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
asc_copy_ub2gm(gm_dst, ub_dst, size);              // PIPE_MTE3
```

### 模式 3：多核分块

```c
uint32_t blockLen = totalLen / asc_get_block_num();
uint32_t offset = asc_get_block_idx() * blockLen;
// 每个核处理自己的数据分片
```

### 模式 4：原子 GM 写入

```c
asc_copy_ub2gm(gm_dst, ub_src0, size);   // 普通写入
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_add_float();               // 开启原子加
asc_copy_ub2gm(gm_dst, ub_src1, size);   // 原子累加写入
asc_set_atomic_none();                    // 关闭原子模式
```

---

## 支持的数据类型

| C/C++ 类型   | 矢量类型别名           | NPU 类型    |
|--------------|------------------------|-------------|
| `half`       | `vector_half`           | `vector_f16`|
| `float`      | `vector_float`          | `vector_f32`|
| `bfloat16_t` | `vector_bfloat16_t`     | `vector_bf16`|
| `int8_t`     | `vector_int8_t`         | `vector_s8` |
| `int16_t`    | `vector_int16_t`        | `vector_s16`|
| `int32_t`    | `vector_int32_t`        | `vector_s32`|
| `int64_t`    | `vector_int64_t`        | `vector_s64`|
| `uint8_t`    | `vector_uint8_t`        | `vector_u8` |
| `uint16_t`   | `vector_uint16_t`       | `vector_u16`|
| `uint32_t`   | `vector_uint32_t`       | `vector_u32`|
| `fp8_e4m3`   | `vector_fp8_e4m3fn_t`  | `vector_f8e4m3`|
| `fp8_e5m2`   | `vector_fp8_e5m2_t`    | `vector_f8e5m2`|
| `int4x2_t`   | `vector_int4x2_t`       | `vector_s4x2`|
| `fp4x2_e2m1` | `vector_fp4x2_e2m1_t`  | `vector_f4e2m1x2`|
| `fp4x2_e1m2` | `vector_fp4x2_e1m2_t`  | `vector_f4e1m2x2`|
| `hifloat8_t` | `vector_hifloat8_t`     | `vector_hif8`|

---

完整的 API 参考请参见 [reference.md](reference.md)。
完整的可运行示例请参见 [example.md](example.md)。
