---
name: my_cube_c_api
description: 通过AscendC Cube C API编程语言编写对应的单API测试样例, 覆盖数据搬运（GM↔L1、L1↔L0A/L0B/L0C）和矩阵计算（asc_mmad）。支持 NPU 架构 2201 (Ascend 910B1) 和 3510 (Ascend 950PR/950DT)。每次只测试单独的一个 C API 接口，数据流遵循 Cube 核的搬运→计算→搬出模式。
---

# Ascend Cube C API 技能文档

## 概述

Cube C API 为 NPU 的 Cube（矩阵计算）核提供了底层、面向硬件的编程接口。
这些 API 是对编译器 CCE（Cube Compute Engine）内部指令的薄封装，可直接控制
**矩阵数据搬运**（GM↔L1↔L0A/L0B/L0C）和**矩阵乘加计算**（Mmad）。

支持的架构：
- **2201**（Ascend 910B1，对应芯片代号 A2/A3）
- **3510**（Ascend 950PR/Ascend 950DT，对应芯片代号 A5）

### 芯片代号与架构映射

| 芯片代号 | NPU 架构 | 芯片型号 | 编译选项 |
|----------|----------|----------|----------|
| A2/A3 | 2201 | Ascend 910B1 | `--npu-arch=dav-2201` |
| A5 | 3510 | Ascend 950PR/950DT | `--npu-arch=dav-3510` |

---

## 重要限制

- Cube C API 的 kernel 标识符为 `__cube__ __global__`（`__cube__` 展开为 `__attribute__((aic))`），Vector 核为 `__vector__ __global__`
- 注意：`__global__ __aicore__` 会导致链接器无法自动推导 kernel 类型，必须使用 `__cube__ __global__`
- 所有业务函数需加 `__aicore__ inline` 标识符
- Cube 核不使用 `__ubuf__`（UB 空间），而是使用 `__cbuf__`（L1）、`__ca__`（L0A）、`__cb__`（L0B）、`__cc__`（L0C）存储修饰符
- 不同架构（2201 vs 3510）的数据排布（Nz/Zz）和接口参数不同，需通过 `__NPU_ARCH__` 宏区分
- 所有 Cube C API 测试需设置 `core_type = CubeCore`（YAML 配置）或 `g_coreType = C_API_AIC_TYPE`（代码）
- `get_imm(offset)` 用于获取片内缓冲区的物理基地址（L1/L0A/L0B/L0C 均通过偏移量管理）
- 指令的使用文档请参考 `docs/api/c_api/` 目录
- 编译时注意架构选择：`--npu-arch=dav-2201` 或 `--npu-arch=dav-3510`
- 禁止随意揣测指令，必要时查阅文档或现有示例确认

## 快速入门

### 头文件引入

```c
#include "c_api/asc_simd.h"
```

单个头文件即可包含所有 C API 接口。也可以按类别引入：
```c
#include "c_api/cube_datamove.h"
#include "c_api/cube_compute.h"
```

### Cube Kernel 函数结构

典型 Cube 核 C API Kernel 遵循以下模式：

```c
// Cube 核数据通路：GM → L1(ND→NZ) → L0A/L0B → L0C(MMAD) → GM(NZ→ND)
// 关键：通过 get_imm(offset) 在不同存储层级上用偏移量管理缓冲区

// 1. 定义 Kernel 算子类
template <typename A_T, typename B_T, typename C_T>
class KernelMmad {
public:
    __aicore__ inline KernelMmad() {}

    __aicore__ inline void Init(
        __gm__ uint8_t* aGM, __gm__ uint8_t* bGM, __gm__ uint8_t* cGM,
        uint16_t mIn, uint16_t kIn, uint16_t nIn)
    {
        a_gm = (__gm__ A_T*)aGM;
        b_gm = (__gm__ B_T*)bGM;
        c_gm = (__gm__ C_T*)cGM;
        m = mIn; k = kIn; n = nIn;

        // 通过 get_imm(offset) 获取片上各层缓冲区基地址
        uint64_t offset = 0;
        a_l1 = (__cbuf__ A_T*)get_imm(0);             // L1 基地址偏移 0
        offset += m * k * sizeof(A_T);
        b_l1 = (__cbuf__ B_T*)get_imm(offset);         // L1 偏移 A 矩阵大小

        a_l0 = (__ca__ A_T*)get_imm(0);                // L0A 基地址
        b_l0 = (__cb__ B_T*)get_imm(0);                // L0B 基地址
        c_l0 = (__cc__ C_T*)get_imm(0);                // L0C 基地址
    }

    __aicore__ inline void Process() {
        CopyIn();   // GM → L1 (PIPE_MTE2)
        SplitA();   // L1 → L0A (PIPE_MTE1)
        SplitB();   // L1 → L0B (PIPE_MTE1, 带转置)
        Compute();  // MMAD (PIPE_M)
        CopyOut();  // L0C → GM (PIPE_FIX)
    }

private:
    // ... 见下文各步骤详解
};

// 2. 算子入口函数
template <typename A_T, typename B_T, typename C_T>
__aicore__ void kernel_capi_mmad_operator(
    __gm__ uint8_t* aGM, __gm__ uint8_t* bGM, __gm__ uint8_t* cGM,
    uint16_t m, uint16_t k, uint16_t n)
{
    KernelMmad<A_T, B_T, C_T> op;
    op.Init(aGM, bGM, cGM, m, k, n);
    op.Process();
}

// 3. 导出多个特化版本（分离编译模式）
#define KERNEL_CAPI_MMAD_TEST(A_T, B_T, C_T, M, K, N)                  \
    extern "C" __global__ __aicore__ void                               \
    kernel_capi_mmad_##A_T##_##B_T##_##C_T##_##M##_##K##_##N(          \
        __gm__ uint8_t* aGM, __gm__ uint8_t* bGM, __gm__ uint8_t* cGM, \
        uint16_t m, uint16_t k, uint16_t n) {                          \
        kernel_capi_mmad_operator<A_T, B_T, C_T>(aGM, bGM, cGM, M, K, N);\
    }

KERNEL_CAPI_MMAD_TEST(float, float, float, 32, 32, 32)
KERNEL_CAPI_MMAD_TEST(half, half, float, 64, 32, 64)
KERNEL_CAPI_MMAD_TEST(int8_t, int8_t, int32_t, 16, 32, 64)
```

### 融合编译模式（简化版）

如果使用融合编译，直接将模板参数写死在 kernel 中：

```c
extern "C" __global__ __aicore__ void matmul_kernel(
    __gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c)
{
    KernelMmad<float, float, float> op;
    op.Init(a, b, c, 32, 32, 32);
    op.Process();
}
```

### Host 侧启动流程

```c
aclInit(nullptr);
aclrtSetDevice(0);
aclrtCreateStream(&stream);

aclrtMalloc(&aDevice, totalSize, ACL_MEM_MALLOC_HUGE_FIRST);
aclrtMalloc(&bDevice, totalSize, ACL_MEM_MALLOC_HUGE_FIRST);
aclrtMalloc(&cDevice, totalSize, ACL_MEM_MALLOC_HUGE_FIRST);

aclrtMemcpy(aDevice, totalSize, aHost, totalSize, ACL_MEMCPY_HOST_TO_DEVICE);
aclrtMemcpy(bDevice, totalSize, bHost, totalSize, ACL_MEMCPY_HOST_TO_DEVICE);

matmul_kernel<<<1, nullptr, stream>>>(aDevice, bDevice, cDevice);

aclrtSynchronizeStream(stream);
aclrtMemcpy(cHost, totalSize, cDevice, totalSize, ACL_MEMCPY_DEVICE_TO_HOST);

aclrtFree(aDevice); aclrtFree(bDevice); aclrtFree(cDevice);
aclrtDestroyStream(stream);
aclrtResetDevice(0);
aclFinalize();
```

### CMakeLists.txt

```cmake
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(my_cube_kernel my_cube_kernel.asc)
target_compile_options(my_cube_kernel PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>)
```

---

## 核心概念

### 存储修饰符

| 修饰符    | 地址空间               | 对齐要求 | 用途                     |
|-----------|------------------------|----------|--------------------------|
| `__gm__`  | Global Memory（DDR/HBM）| 无       | 输入/输出数据             |
| `__cbuf__`| L1 Buffer（片上 Cache） | 32B      | GM↔L0 的中转缓冲区        |
| `__ca__`  | L0A Buffer（矩阵左操作数）| 512B    | Mmad 的 A 矩阵输入        |
| `__cb__`  | L0B Buffer（矩阵右操作数）| 512B    | Mmad 的 B 矩阵输入        |
| `__cc__`  | L0C Buffer（矩阵结果）   | 64B      | Mmad 的 C 矩阵输出/累加   |
| `__fbuf__`| FB Buffer（Fixpipe Buffer）| 32B  | 格式转换中间缓冲区         |

### Cube 数据流水线

| 流水线     | 用途                                                |
|------------|-----------------------------------------------------|
| `PIPE_M`   | 矩阵计算流水线（Cube 单元，`asc_mmad`）              |
| `PIPE_MTE1`| L1 → L0A/L0B/L0C 搬运（`asc_copy_l12l0a/b`）       |
| `PIPE_MTE2`| GM → L1 搬运（`asc_copy_gm2l1` / `asc_copy_gm2l1_nd2nz`） |
| `PIPE_MTE3`| L0C → GM/L1/UB 搬运                                  |
| `PIPE_FIX` | Fixpipe（L0C→GM 含量化/ReLU 等后处理）               |
| `PIPE_S`   | 标量流水线：指令分发、同步控制                       |
| `PIPE_ALL` | 所有流水线（用于全局同步）                            |

### Cube 核完整数据流与同步链

```
┌──────┐  PIPE_MTE2  ┌──────┐  PIPE_MTE1  ┌──────┐  PIPE_M  ┌──────┐  PIPE_FIX  ┌──────┐
│  GM  │ ──────────→ │  L1  │ ──────────→ │ L0A  │ ───────→ │ L0C  │ ─────────→ │  GM  │
│(ND)  │             │(NZ)  │             │ L0B  │          │      │ (NZ→ND)    │(ND)  │
└──────┘             └──────┘             └──────┘          └──────┘            └──────┘
                        ↑                    ↑                ↑                   ↑
                   asc_set_gm2l1_nz_para  asc_copy_l12l0a  asc_mmad          asc_set_l0c2gm_nz2nd
                   asc_copy_gm2l1_nd2nz   asc_copy_l12l0b                      asc_copy_l0c2gm
                                              _transpose

同步链:  notify/wait(MTE2, MTE1) → notify/wait(MTE1, M) → notify/wait(M, FIX) → notify/wait(FIX, MTE2)
```

### 同步机制

```c
// MTE2(数据搬入) → MTE1(L1→L0搬入)
asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

// MTE1 → M(矩阵计算)
asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

// M → FIX(结果搬出+量化)
asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

// FIX → MTE2(下一轮搬入)
asc_sync_notify(PIPE_FIX, PIPE_MTE2, EVENT_ID0);
asc_sync_wait(PIPE_FIX, PIPE_MTE2, EVENT_ID0);
```

### 同步 vs 异步 API

与 Vector C API 相同，Cube C API 也有两种变体：
- **`asc_xxx(...)`** — 异步：发出指令不等待完成
- **`asc_xxx_sync(...)`** — 同步：自动插入 `asc_sync_post_process()` 同步操作

---

## 分形（Fractal）详解

### 什么是分形

分形（Fractal）是 Cube 矩阵计算引擎中**最基本的数据原子单元**。
整个矩阵乘法管线（GM→L1→L0A/L0B→Mmad→L0C→GM）操作的都是切分成分形的矩阵数据。

### 分形结构

每个分形是一个 **16 行 × c0Size 列** 的 2D 数据块，固定 **512 字节**。

```
c0Size = 32 / sizeof(T)   // 列数（元素为单位）
BLOCK_CUBE = 16            // 行数（固定不变）
fractalSize = BLOCK_CUBE × c0Size  // 总元素数
fractalSize_bytes = 16 × 32 = 512  // 总字节数（固定）

物理存储于 L0A/L0B：行列连续的 512B 块
┌──────────────────────┐
│  row0: c0Size 个元素  │
│  row1: c0Size 个元素  │
│  ...                  │
│  row15: c0Size 个元素 │
└──────────────────────┘
   总大小 = 16 × c0Size × sizeof(T) = 512B
```

### 数据类型与分形参数

| 类型 | sizeof(T) | c0Size | BLOCK_CUBE | 分形元素数 | 分形字节 | fractalNum |
|------|-----------|--------|-----------|-----------|---------|------------|
| float32 | 4 | 8 | 16 | 128 | 512 | **2** |
| half | 2 | 16 | 16 | 256 | 512 | **1** |
| bfloat16 | 2 | 16 | 16 | 256 | 512 | **1** |
| int8_t | 1 | 32 | 16 | 512 | 512 | **2** |
| fp8_e4m3/e5m2 | 1 | 32 | 16 | 512 | 512 | **2** |
| hifloat8_t | 1 | 32 | 16 | 512 | 512 | **2** |
| int4b_t | 0.5 | 64 | 16 | 1024 | 512 | **4** |
| fp4x2(any) | 0.5 | 64 | 16 | 1024 | 512 | **4** |

### fractalNum 的作用

`fractalNum` 是**将多个分形拼成正方形所需的个数**。转置操作需要正方形数据块：

```
half  (16×16)  → 已是正方形，fractalNum=1
int8  (16×32)  → 2个并排成 32×32，fractalNum=2
float (16×8)   → 2个竖排成 16×16，fractalNum=2
int4  (16×64)  → 4个并排成 64×64，fractalNum=4
```

### 参数映射公式

在处理矩阵 C[M,N] = A[M,K] × B[K,N] 时，所有维度参数必须转换为**分形粒度**：

```
// M 方向的 L0A 分形数
m_step = M / 16                    // = CeilDiv(M, BLOCK_CUBE)

// K 方向的 L0A 分形数
k_step = K * sizeof(T) / 32        // = CeilDiv(K, c0Size)
                                    // c0Size = 32/sizeof(T), 所以 K/c0Size = K*sizeof(T)/32

// N 方向的 L0B 带转置后的分形数
k_step_B = K / 16                   // L1→L0B 转置后 m_step = K/16
k_step_B_after_transpose = N * sizeof(T) / 32  // 转置后 L0B 的 k_step = N/c0Size
```

实际代码示例：
```cpp
// L1 → L0A（不分形转置）
asc_copy_l12l0a(
    a_l0, a_l1,           // dst=L0A, src=L1
    0, 0,                 // m_start=0, k_start=0
    m / 16,               // M方向分形数 = M/BLOCK_CUBE
    k * sizeof(A_T) / 32, // K方向分形数 = K/c0Size（32B block数）
    m / 16,               // src_stride（L1 侧，分形为单位）
    m / 16                // dst_stride（L0A侧，分形为单位）
);

// L1 → L0B（带分形转置）
asc_copy_l12l0b_transpose(
    b_l0, b_l1,           // dst=L0B, src=L1
    0, 0,                 // m_start=0, k_start=0
    k / 16,               // 转置前 m_step = K/BLOCK_CUBE
    n * sizeof(B_T) / 32, // 转置前 k_step = N/c0Size
    k / 16,               // src_stride（L1 侧）
    n / 16                // dst_stride（L0B侧，转置后为 N/BLOCK_CUBE）
);
```

### ND / NZ / ZZ 数据排布

矩阵在 Cube 流水线中经历多次格式转换：

**ND（Normal Dense）** — 行优先标准存储（GM 侧）：
```
A[row][col]  → 连续行排列
```

**NZ（Nz）** — 分形块排列（L1 侧、3510 的 L0A）：
```
矩阵被分成 BLOCK_CUBE × c0Size 的分形块，各分形块按行优先排列
```

**ZZ（Zz）** — 分形块转置排列（2201 的 L0A）：
```
分形块按列优先排列，便于某些架构的矩阵计算
```

架构差异：
- **3510**: L1=Nz, L0A=Nz（直接搬运，用 `LoadData2DParamsV2`）
- **2201**: L1=Nz, L0A=Zz（需转置，用 `LoadData2DParams` + 循环）

---

## API 详解

### ND→NZ 配置与搬运（GM → L1）

**`asc_set_gm2l1_nz_para(uint64_t config)`** — 配置 ND→NZ 硬件转换参数：

```c
// config 打包方式：
// bit[0:15]   = ndNum           — ND 矩阵个数
// bit[16:31]  = loop2DstStride  — 第 2 层循环目的步长
// bit[32:47]  = loop3DstStride  — 第 3 层循环目的步长
// bit[48:63]  = loop4DstStride  — 第 4 层循环目的步长

// 对 A[M,K] 矩阵的典型配置：ndNum=1, M维度步长=m
uint64_t config = 1 | (1 << 16) | ((uint64_t)m << 32);
asc_set_gm2l1_nz_para(config);

// 对 B[K,N] 矩阵的典型配置：ndNum=1, K维度步长=k
config = 1 | (1 << 16) | ((uint64_t)k << 32);
asc_set_gm2l1_nz_para(config);
```

**`asc_copy_gm2l1_nd2nz(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en)`**

```
// 参数含义：
// loop1_src_stride — 第1层循环的源步长（字节单位，通常=k*sizeof(T)）
// l2_cache_ctl     — L2 缓存控制 (0=默认)
// n_value          — N 方向元素数（对于 A=M, 对于 B=K）
// d_value          — D 方向元素数（对于 A=K, 对于 B=N）
// loop4_src_stride — 第4层循环源步长（通常=0）
// smallc0_en       — 小 C0 使能

// A[M,K]: ND→NZ 搬运，每行 K 个元素
asc_set_gm2l1_nz_para(1 | (1 << 16) | ((uint64_t)m << 32));
asc_copy_gm2l1_nd2nz(a_l1, a_gm, k * sizeof(A_T), 0, m, k, 0, false);

// B[K,N]: ND→NZ 搬运，每行 N 个元素
asc_set_gm2l1_nz_para(1 | (1 << 16) | ((uint64_t)k << 32));
asc_copy_gm2l1_nd2nz(b_l1, b_gm, n * sizeof(B_T), 0, k, n, 0, false);
```

### L1 → L0A（SplitA，不分形转置）

**`asc_copy_l12l0a(dst, src, m_start, k_start, m_step, k_step, src_stride, dst_stride)`**

```c
asc_copy_l12l0a(
    a_l0,                    // 目的: L0A Buffer
    a_l1,                    // 源: L1 Buffer
    0,                       // m_start_position: M方向起始分形位置
    0,                       // k_start_position: K方向起始分形位置
    m / 16,                  // m_step: M方向分形数
    k * sizeof(A_T) / 32,    // k_step: K方向分形数（= K / c0Size）
    m / 16,                  // src_stride: L1 中行间步长（分形为单位）
    m / 16                   // dst_stride: L0A 中行间步长（分形为单位）
);
```

### L1 → L0B（SplitB，带分形转置）

**`asc_copy_l12l0b_transpose(dst, src, m_start, k_start, m_step, k_step, src_stride, dst_stride)`**

```c
asc_copy_l12l0b_transpose(
    b_l0,                    // 目的: L0B Buffer（转置后数据）
    b_l1,                    // 源: L1 Buffer（原始 NZ 排布）
    0,                       // m_start_position
    0,                       // k_start_position
    k / 16,                  // m_step: K方向分形数（转置前 m 方向）
    n * sizeof(B_T) / 32,    // k_step: N方向分形数（转置前 k 方向）
    k / 16,                  // src_stride: L1 侧分形步长
    n / 16                   // dst_stride: L0B 侧分形步长
);
```

### 矩阵乘加（Mmad）

**`asc_mmad(c, a, b, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source, c_matrix_init_val)`**

```c
// C[M,N] = A[M,K] × B[K,N]
// left_height  = M (A 行数)
// n_dim        = K (内维度)
// right_width  = N (B 列数)

asc_mmad(
    c_l0,                    // 结果: L0C Buffer
    a_l0,                    // A: L0A Buffer
    b_l0,                    // B: L0B Buffer
    m,                       // left_height: M（原始元素数）
    k,                       // n_dim: K（原始元素数）
    n,                       // right_width: N（原始元素数）
    0,                       // unit_flag: 0=关闭
    true,                    // disable_gemv: true=禁用 GEMV
    false,                   // c_matrix_source: false=不累加
    true                     // c_matrix_init_val: true=结果初始化为0
);
```

### NZ→ND 配置与结果搬出（L0C → GM）

**`asc_set_l0c2gm_nz2nd(nd_num, src_nd_stride, dst_nd_stride)`** — 配置 NZ→ND 输出转换：

```c
// 将 L0C 中的 NZ 排布结果转换为 GM 中的 ND（行优先）排布
asc_set_l0c2gm_nz2nd(1, 0, 0);  // 单个矩阵，无跨步
```

**`asc_copy_l0c2gm(dst, src, n_size, m_size, loop_dst_stride, loop_src_stride, ...)`** — 结果搬出：

```c
asc_copy_l0c2gm(
    c_gm,                    // 目的: GM（ND 格式）
    c_l0,                    // 源: L0C（NZ 格式）
    n,                       // n_size: N 维度大小（元素数）
    m,                       // m_size: M 维度大小（元素数）
    n,                       // loop_dst_stride: GM 侧行间步长（=N）
    m,                       // loop_src_stride: L0C 侧行间步长（=M）
    0, 0, 0, 0, 0,          // l2_cache_ctl, clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre
    false, true,             // split_en, nz2nd_en
    0, 0, 0,                 // quant_post, relu_post, clip_relu_post
    0,                       // eltwise_op
    false, false, false, false  // eltwise_antq_en, c0_pad_en, broadcast_en, nz2dn_en
);
```

### 缓冲区的地址管理

通过 `get_imm(offset)` 在不同存储层级上管理缓冲区地址：

```c
// L1 缓冲区管理（通过偏移量区分 A/B 矩阵）
uint64_t offset = 0;
__cbuf__ A_T* a_l1 = (__cbuf__ A_T*)get_imm(offset);         // A 矩阵在 L1
offset += m * k * sizeof(A_T);                                 // A 占用的空间
__cbuf__ B_T* b_l1 = (__cbuf__ B_T*)get_imm(offset);         // B 矩阵在 L1（偏移后）

// L0A/L0B/L0C 各层独立地址空间
__ca__ A_T* a_l0 = (__ca__ A_T*)get_imm(0);   // L0A 偏移 0
__cb__ B_T* b_l0 = (__cb__ B_T*)get_imm(0);   // L0B 偏移 0
__cc__ C_T* c_l0 = (__cc__ C_T*)get_imm(0);   // L0C 偏移 0
```

注意：
- `get_imm(0)` 返回各层基地址（L0A、L0B、L0C 各层独立寻址）
- L1 中若有多个矩阵，需用线性偏移区分
- 不支持通过指针加减（如 `a_l1 + offset`）来跨层寻址

---

## API 分类概览

### Cube 数据搬运（`cube_datamove.h`）

| API | 数据流 | 流水线 | 芯片 | 架构 | 说明 |
|-----|--------|--------|------|------|------|
| `asc_copy_gm2l1` | GM → L1 | MTE2 | A3/A5 | 2201/3510 | 基本 GM→L1 搬运，支持 pad 模式 |
| `asc_set_gm2l1_nz_para` | 配置 | — | A5 | 3510 | 配置 ND→NZ 转换参数 |
| `asc_copy_gm2l1_nd2nz` | GM → L1 | MTE2 | A5 | 3510 | ND→NZ 格式转换搬运 |
| `asc_copy_gm2l1_align` | GM → L1 | MTE2 | A5 | 3510 | 对齐模式 GM→L1 |
| `asc_copy_gm2l1_dn2nz` | GM → L1 | MTE2 | A5 | 3510 | DN→NZ 格式转换搬运 |
| `asc_copy_gm2l0a` | GM → L0A | MTE2 | A3 | 2201 | 直接 GM→L0A（2201 特有） |
| `asc_copy_gm2l0b` | GM → L0B | MTE2 | A3 | 2201 | 直接 GM→L0B（2201 特有） |
| `asc_copy_l12l0a` | L1 → L0A | MTE1 | A5 | 3510 | LoadData 2D，分形粒度参数 |
| `asc_copy_l12l0a_transpose` | L1 → L0A | MTE1 | A5 | 3510 | 带分形转置的 LoadData |
| `asc_copy_l12l0a_mx` | L1 → L0A | MTE1 | A5 | 3510 | MX 格式 scale 搬运 |
| `asc_copy_l12l0b` | L1 → L0B | MTE1 | A5 | 3510 | LoadData 2D，分形粒度参数 |
| `asc_copy_l12l0b_transpose` | L1 → L0B | MTE1 | A5 | 3510 | 带分形转置的 LoadData |
| `asc_copy_l12l0b_trans` | L1 → L0B | MTE1 | A5 | 3510 | LoadData with Transpose |
| `asc_copy_l12l0b_mx` | L1 → L0B | MTE1 | A5 | 3510 | MX 格式 scale 搬运 |
| `asc_copy_l12fb` | L1 → FB | MTE1 | A3/A5 | 2201/3510 | L1→Fixpipe Buffer |
| `asc_copy_l12bt` | L1 → BT | MTE1 | A3/A5 | 2201/3510 | L1→Tensor Buffer |
| `asc_copy_l12ub` | L1 → UB | MTE1 | A5 | 3510 | L1→UB 搬运 |
| `asc_copy_l12l0c` | L1 → L0C | MTE1 | A3 | 2201 | L1→L0C（用于 C 矩阵初始化） |
| `asc_copy_l12gm` | L1 → GM | — | A3 | 2201 | L1→GM 直接搬运 |
| `asc_set_l0c2gm_nz2nd` | 配置 | — | A5 | 3510 | 配置 NZ→ND 输出转换 |
| `asc_copy_l0c2gm` | L0C → GM | FIX | A5 | 3510 | 结果搬出，含量化/ReLU |
| `asc_copy_l0c2l1` | L0C → L1 | FIX | A3/A5 | 3510 | 中间结果搬出到 L1 |
| `asc_copy_l0c2ub` | L0C → UB | MTE3 | A5 | 3510 | 中间结果搬出到 UB |
| `asc_fill_l0a` | — | MTE1 | A3 | 2201 | 用常量/标量填充 L0A |
| `asc_fill_l0b` | — | MTE1 | A3 | 2201 | 用常量/标量填充 L0B |
| `asc_fill_l1` | — | MTE1 | A3/A5 | 2201/3510 | 用常量填充 L1 |
| `asc_set_gm2l1_loop_size` | 配置 | — | A5 | 3510 | 配置 GM→L1 循环大小 |
| `asc_set_gm2l1_loop1_stride` | 配置 | — | A5 | 3510 | 配置 loop1 步长 |
| `asc_set_gm2l1_loop2_stride` | 配置 | — | A5 | 3510 | 配置 loop2 步长 |
| `asc_set_gm2l1_pad` | 配置 | — | A5 | 3510 | 配置 pad 值 |
| `asc_set_l13d_rpt` | 配置 | — | A3/A5 | 2201/3510 | 配置 Load3D 重复参数 |
| `asc_set_l13d_fmatrix` | 配置 | — | A3/A5 | 2201/3510 | 配置 Load3D FMatrix |
| `asc_set_l13d_fmatrix_b` | 配置 | — | A3/A5 | 2201/3510 | 配置 Load3D FMatrix B |
| `asc_set_l0c2gm_config` | 配置 | — | A3/A5 | 2201/3510 | 配置 L0C→GM 输出参数 |
| `asc_set_l0c2gm_lrelu_alpha` | 配置 | — | A3/A5 | 2201/3510 | 配置 LeakyReLU alpha |

### Cube 矩阵计算（`cube_compute.h`）

| API | A类型×B类型→C类型 | 架构 | 说明 |
|-----|-------------------|------|------|
| `asc_mmad` | bf16×bf16→fp32 | 2201/3510 | 标准 BF16 矩阵乘加 |
| `asc_mmad` | half×half→fp32 | 2201/3510 | Half 矩阵乘加 |
| `asc_mmad` | fp32×fp32→fp32 | 2201/3510 | FP32 矩阵乘加 |
| `asc_mmad` | int8×int8→int32 | 2201/3510 | INT8 矩阵乘加 |
| `asc_mmad` | fp8_e4m3×fp8_e4m3/e5m2→fp32 | 3510 | FP8 矩阵乘加 |
| `asc_mmad` | fp8_e5m2×fp8_e4m3/e5m2→fp32 | 3510 | FP8 矩阵乘加 |
| `asc_mmad_mx` | fp4x2_*×fp4x2_*→fp32 | 3510 | MX FP4 矩阵乘加（4种组合） |
| `asc_mmad_mx` | fp8_e4m3/e5m2×fp8_e4m3/e5m2→fp32 | 3510 | MX FP8 矩阵乘加 |
| `asc_mmad_s4` | int4b×int4b→int32 | 2201 | Int4 矩阵乘加 |
| `asc_mmad_sparse` | int8 稀疏×int8→int32 | 2201 | 稀疏矩阵乘加 |

### Mmad 参数说明

```c
// 3510 架构签名
asc_mmad(
    __cc__ float* c_matrix,      // 结果矩阵 L0C
    __ca__ float* a_matrix,      // A 矩阵 L0A
    __cb__ float* b_matrix,      // B 矩阵 L0B
    uint16_t left_height,        // M（A 行数），范围[0,4095]
    uint16_t n_dim,              // K（内维度），范围[0,4095]
    uint16_t right_width,        // N（B 列数），范围[0,4095]
    uint8_t unit_flag,           // 0=关闭, 2=开启常驻, 3=开启单次
    bool disable_gemv,           // true=禁用 GEMV 优化
    bool c_matrix_source,        // true=C 矩阵来自 L0C 累加
    bool c_matrix_init_val       // true=初始化为0
);
```

### 架构差异（2201 vs 3510）

| 差异项 | 2201 (A2/A3, Ascend 910B1) | 3510 (A5, Ascend 950PR/950DT) |
|--------|----------------------------|-------------------------------|
| **L0A 数据排布** | Zz | Nz |
| **L0B 数据排布** | Zn | Zn |
| **len_burst 单位** | 64B | 32B |
| **GM→L1 方式** | `asc_copy_gm2l1`（burst模式） | `asc_copy_gm2l1_nd2nz`（ND→NZ） |
| **LoadData 接口** | `LoadData2DParams` | `LoadData2DParamsV2` |
| **Fixpipe 参数** | `FixpipeParamsV220` | `FixpipeParamsArch3510` |
| **L1→L0A LoadData** | Nz→Zz（循环+转置） | Nz→Nz（单次调用，可选转置） |
| **特有 API** | `asc_copy_gm2l0a/b`, `asc_mmad_sparse`, `asc_enable_hf32`, `asc_set_mmad_direction_m/n` | `asc_mmad_mx`, `asc_copy_l0c2l1`(多类型), `asc_set_l0c2gm_nz2nd`, `asc_set_gm2l1_nz_para`, `asc_copy_l12l0a/b_transpose` |
| **无实现 API** | — | `asc_copy_l12gm` 无 3510 实现 |

---

## 完整代码示例：C API Mmad（3510 架构）

此例来自 `cube_examples/asc_mmad/kernel_capi_mmad.cpp`，展示完整的 Cube 核 Mmad：

```c
#include "c_api/asc_simd.h"

template <typename A_T, typename B_T, typename C_T>
class KernelMmad {
public:
    __aicore__ inline KernelMmad() {}

    __aicore__ inline void Init(
        __gm__ uint8_t* aGM, __gm__ uint8_t* bGM, __gm__ uint8_t* cGM,
        uint16_t mIn, uint16_t kIn, uint16_t nIn)
    {
        a_gm = (__gm__ A_T*)aGM;
        b_gm = (__gm__ B_T*)bGM;
        c_gm = (__gm__ C_T*)cGM;
        m = mIn; k = kIn; n = nIn;

        uint64_t offset = 0;
        a_l1 = (__cbuf__ A_T*)get_imm(0);
        offset += m * k * sizeof(A_T);
        b_l1 = (__cbuf__ B_T*)get_imm(offset);
        a_l0 = (__ca__ A_T*)get_imm(0);
        b_l0 = (__cb__ B_T*)get_imm(0);
        c_l0 = (__cc__ C_T*)get_imm(0);
    }

    __aicore__ inline void Process() {
        CopyIn();   // GM → L1 (ND→NZ), PIPE_MTE2
        SplitA();   // L1 → L0A, PIPE_MTE1
        SplitB();   // L1 → L0B (transpose), PIPE_MTE1
        Compute();  // MMAD, PIPE_M
        CopyOut();  // L0C → GM (NZ→ND), PIPE_FIX
    }

private:
    __aicore__ inline void SetNd2NZPara(
        uint64_t ndNum, uint64_t loop2DstStride,
        uint64_t loop3DstStride, uint64_t loop4DstStride)
    {
        uint64_t config = ndNum;
        config |= (loop2DstStride << 16);
        config |= (loop3DstStride << 32);
        config |= (loop4DstStride << 48);
        asc_set_gm2l1_nz_para(config);
    }

    __aicore__ inline void CopyIn() {
        // A[M,K]: ND→NZ, loop1_src_stride=k*sizeof(A_T)
        SetNd2NZPara(1, 1, m, 0);
        asc_copy_gm2l1_nd2nz(a_l1, a_gm, k * sizeof(A_T), 0, m, k, 0, false);

        // B[K,N]: ND→NZ, loop1_src_stride=n*sizeof(B_T)
        SetNd2NZPara(1, 1, k, 0);
        asc_copy_gm2l1_nd2nz(b_l1, b_gm, n * sizeof(B_T), 0, k, n, 0, false);

        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    }

    __aicore__ inline void SplitA() {
        asc_copy_l12l0a(
            a_l0, a_l1,
            0, 0,
            m / 16,                    // m_step = M/BLOCK_CUBE
            k * sizeof(A_T) / 32,      // k_step = K/c0Size
            m / 16,                    // src_stride
            m / 16                     // dst_stride
        );
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    }

    __aicore__ inline void SplitB() {
        asc_copy_l12l0b_transpose(
            b_l0, b_l1,
            0, 0,
            k / 16,                    // m_step = K/BLOCK_CUBE (pre-transpose)
            n * sizeof(B_T) / 32,      // k_step = N/c0Size (pre-transpose)
            k / 16,                    // src_stride
            n / 16                     // dst_stride
        );
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    }

    __aicore__ inline void Compute() {
        asc_mmad(c_l0, a_l0, b_l0, m, k, n, 0, true, false, true);
        asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    }

    __aicore__ inline void CopyOut() {
        asc_set_l0c2gm_nz2nd(1, 0, 0);
        asc_copy_l0c2gm(
            c_gm, c_l0, n, m, n, m,
            0, 0, 0, 0, 0, false, true, 0, 0, 0, 0, false, false, false, false
        );
        asc_sync_notify(PIPE_FIX, PIPE_MTE2, EVENT_ID0);
        asc_sync_wait(PIPE_FIX, PIPE_MTE2, EVENT_ID0);
    }

private:
    __gm__ A_T* a_gm;
    __gm__ B_T* b_gm;
    __cbuf__ A_T* a_l1;
    __cbuf__ B_T* b_l1;
    __ca__ A_T* a_l0;
    __cb__ B_T* b_l0;
    __cc__ C_T* c_l0;
    __gm__ C_T* c_gm;
    uint16_t m, k, n;
};

// 多种数据类型特化
extern "C" __global__ __aicore__ void kernel_float32_32_32(
    __gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c,
    uint16_t m, uint16_t k, uint16_t n)
{
    KernelMmad<float, float, float> op;
    op.Init(a, b, c, 32, 32, 32);
    op.Process();
}
```

---

## 支持的数据类型

| C/C++ 类型   | Cube 分形参数（BLOCK_CUBE c0Size） | NPU 类型    |
|--------------|-----------------------------------|-------------|
| `float`      | 16 × 8 = 128 元素（512B）         | `vector_f32`|
| `half`       | 16 × 16 = 256 元素（512B）        | `vector_f16`|
| `bfloat16_t` | 16 × 16 = 256 元素（512B）        | `vector_bf16`|
| `int8_t`     | 16 × 32 = 512 元素（512B）        | `vector_s8` |
| `int16_t`    | 16 × 16 = 256 元素（512B）        | `vector_s16`|
| `int32_t`    | 16 × 8 = 128 元素（512B）         | `vector_s32`|
| `uint8_t`    | 16 × 32 = 512 元素（512B）        | `vector_u8` |
| `uint16_t`   | 16 × 16 = 256 元素（512B）        | `vector_u16`|
| `uint32_t`   | 16 × 8 = 128 元素（512B）         | `vector_u32`|
| `fp8_e4m3fn_t` | 16 × 32 = 512 元素（512B）      | `vector_f8e4m3`|
| `fp8_e5m2_t` | 16 × 32 = 512 元素（512B）        | `vector_f8e5m2`|
| `int4b_t`    | 16 × 64 = 1024 元素（512B）       | `vector_s4` |
| `fp4x2_e1m2_t` | 16 × 64 = 1024 元素（512B）     | `vector_f4e1m2x2`|
| `fp4x2_e2m1_t` | 16 × 64 = 1024 元素（512B）     | `vector_f4e2m1x2`|
| `hifloat8_t` | 16 × 32 = 512 元素（512B）        | `vector_hif8`|

---

## 常用使用模式

### 模式 1：完整 Cube 矩阵乘法（统一编译）

```c
extern "C" __global__ __aicore__ void simple_matmul(
    __gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c)
{
    // 直接用算子类
    KernelMmad<float, float, float> op;
    op.Init(a, b, c, 32, 32, 32);
    op.Process();
}
```

### 模式 2：GM → L1 ND→NZ 数据搬运

```c
// 配置 ND→NZ 参数（A 矩阵 M×K）
asc_set_gm2l1_nz_para(1 | (1 << 16) | ((uint64_t)M << 32));

// 执行搬运
asc_copy_gm2l1_nd2nz(
    l1_buf,           // 目的 L1
    gm_buf,           // 源 GM
    K * sizeof(T),    // loop1_src_stride (字节)
    0,                // l2_cache_ctl
    M,                // n_value (N 方向元素数)
    K,                // d_value (D 方向元素数)
    0,                // loop4_src_stride
    false             // smallc0_en
);

// 同步等待
asc_sync_notify(PIPE_MTE2, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_S, EVENT_ID0);
```

### 模式 3：L1 → L0A LoadData（分形粒度）

```c
asc_copy_l12l0a(
    l0a_buf,              // 目的: L0A
    l1_buf,               // 源: L1
    0,                    // m_start_position
    0,                    // k_start_position
    M / 16,               // m_step: M方向分形数
    K * sizeof(T) / 32,   // k_step: K方向分形数
    M / 16,               // src_stride（分形为单位）
    M / 16                // dst_stride（分形为单位）
);
```

### 模式 4：L0C → GM 结果搬出（NZ→ND）

```c
asc_set_l0c2gm_nz2nd(1, 0, 0);

asc_copy_l0c2gm(
    gm_output, l0c_buf,
    N, M,                 // n_size, m_size
    N, M,                 // loop_dst_stride, loop_src_stride
    0, 0, 0, 0, 0,       // 后处理参数关闭
    false, true,          // split_en, nz2nd_en(=true, 开启NZ→ND)
    0, 0, 0, 0, false, false, false, false
);
```

---

## 分形转置对齐约束

当使用 `asc_copy_l12l0a_transpose` 或 `asc_copy_l12l0b_transpose` 带转置时：

```
int4 (b4):   m_step 必须是 4 的倍数
int8 (b8):   m_step 必须是 2 的倍数
float (b32): k_step 必须是 2 的倍数
```

### int4 特殊处理

int4b_t 在搬运时，两个 int4 打包成一个 int8，所以 ND→NZ 时：
```c
// int4b_t 的 d_value（K维度）需减半
d_value = K >> 1;       // 两个 int4 合并成 int8
srcDValue = K >> 1;     // 同理
```

---

## 单 API 测试样例模板

以下为 Cube C API 单 API 测试样例的标准模板结构：

```c
#include <cstdint>
#include <vector>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t M = 32;
constexpr uint32_t K = 32;
constexpr uint32_t N = 32;
constexpr uint32_t NUM_BLOCKS = 1;  // 单核测试

// Device 侧 Kernel
extern "C" __global__ __aicore__ void test_api_kernel(
    __gm__ uint8_t* input, __gm__ uint8_t* output)
{
    asc_init();

    // 通过 get_imm 管理片上缓冲区
    __cbuf__ float* l1_buf = (__cbuf__ float*)get_imm(0);

    // === 1. 数据搬入 GM → L1 ===
    // 配置 ND→NZ 参数
    asc_set_gm2l1_nz_para(1 | (1 << 16) | ((uint64_t)M << 32));

    asc_copy_gm2l1_nd2nz(
        l1_buf, (__gm__ float*)input,
        K * sizeof(float), 0, M, K, 0, false
    );

    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

    // === 2. 单 API 测试点 ===
    // 在此处调用被测 API，如:
    //   asc_copy_l12l0a(...)
    //   asc_copy_l12fb(...)
    //   asc_fill_l1(...)
    //   asc_copy_l12l0b_transpose(...)

    // 同步等待 API 完成
    asc_sync_notify(PIPE_MTE1, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_FIX, EVENT_ID0);

    // === 3. 结果搬出 ===
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(
        (__gm__ float*)output, (__cc__ float*)get_imm(0),
        M, N, M, N,
        0, 0, 0, 0, 0, false, true, 0, 0, 0, 0, false, false, false, false
    );
    asc_sync_notify(PIPE_FIX, PIPE_MTE2, EVENT_ID0);
    asc_sync_wait(PIPE_FIX, PIPE_MTE2, EVENT_ID0);

    asc_sync_pipe(PIPE_ALL);
}

// Host 侧启动
int32_t main()
{
    aclInit(nullptr);
    aclrtSetDevice(0);
    aclrtCreateStream(&stream);

    uint32_t totalSize = M * K * sizeof(float);
    uint8_t* inputDevice = nullptr;
    uint8_t* outputDevice = nullptr;

    aclrtMalloc((void**)&inputDevice, totalSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void**)&outputDevice, totalSize, ACL_MEM_MALLOC_HUGE_FIRST);

    aclrtMemcpy(inputDevice, totalSize, inputHost, totalSize, ACL_MEMCPY_HOST_TO_DEVICE);

    test_api_kernel<<<1, nullptr, stream>>>(inputDevice, outputDevice);

    aclrtSynchronizeStream(stream);
    aclrtMemcpy(outputHost, totalSize, outputDevice, totalSize, ACL_MEMCPY_DEVICE_TO_HOST);

    // 精度比对
    // ...

    aclrtFree(inputDevice);
    aclrtFree(outputDevice);
    aclrtDestroyStream(stream);
    aclrtResetDevice(0);
    aclFinalize();
    return 0;
}
```

---

## 现有测试与示例参考

| 位置 | 内容 | 用途 |
|------|------|------|
| `cube_examples/asc_mmad/` | 完整 C API Mmad 示例 | **最推荐参考** — 纯 C API 的完整 Cube 矩阵乘法 |
| `tests/api/c_api/npu_arch_3510/cube_datamove/` | 25 个 C API Stub 测试 | 验证参数传递正确性（Mock 测试） |
| `tests/api/c_api/npu_arch_3510/cube_compute/` | 5 个 C API Stub 测试 | `asc_mmad` 等参数验证 |
| `tests/api/c_api/npu_arch_2201/cube_datamove/` | 24 个 C API Stub 测试 | 2201 架构 datamove 测试 |
| `tests/api/c_api/npu_arch_2201/cube_compute/` | 11 个 C API Stub 测试 | 2201 架构 compute 测试 |
| `examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/*/` | 11 个 Basic API .asc 示例 | 完整 Cube kernel（使用 Basic C++ API，非 C API） |
| `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/` | 26 个 impl 文件 | C API 底层实现参考 |
| `impl/c_api/instr_impl/npu_arch_3510/cube_compute_impl/` | 4 个 impl 文件 | `asc_mmad` 等实现 |
| `impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl/` | 29 个 impl 文件 | 2201 架构实现 |
| `impl/c_api/instr_impl/npu_arch_2201/cube_compute_impl/` | 9 个 impl 文件 | 2201 架构计算实现 |

---

## 编译注意事项

- Kernel 源文件使用 `.asc` 扩展名
- 目标架构：`--npu-arch=dav-3510`（3510 平台）或 `--npu-arch=dav-2201`（2201 平台）
- 项目语言必须包含 `ASC`
- 使用 `cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..` 在仿真模式下编译运行
- 执行精度比对时，应将输入、输出、真值数据打印并存储为 csv 文件
- 如果存在精度问题，打印出现精度问题的元素的值和索引
