# NPU向量计算C API测试样例编写与精度修复

**Session ID:** ses_1c06fb25affeKvMrz2U2edxKkN
**Created:** 5/19/2026, 5:28:02 PM
**Updated:** 5/19/2026, 6:03:32 PM

---

## User

使用C API样例编写相关skill, 从 @impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/ 中选择5个和 @vector_compute_examples/ 中不重复的接口，为其编写测试样例，同时对应修改一键编译执行的脚本，且遵循该目录下的规范，如：测试多种数据类型，输入输出和真值存储为csv文件，采用sim仿真模式编译运行等。在本机对应的样例目录下编译运行通过，如果遇到精度问题或编译问题，在修复前重点记录，随后尝试修复，将相关日志存储在样例文件夹下，完成后清理过程中在代码仓目录下产生的dump, toml等日志文件。总结样例中遇到的精度问题和解决方案。 

---

## Assistant (Build · DeepSeek V4 Pro · 30.6s)

_Thinking:_

The user wants me to:

1. Load the C API skill
2. Look at `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` to find available interfaces
3. Look at `vector_compute_examples/` to see what already exists
4. Select 5 interfaces that don't overlap
5. Write test examples for them following the directory conventions
6. Update the build script
7. Compile and run them on the local machine
8. Handle any precision or compilation issues
9. Clean up dump/toml log files
10. Summarize precision issues and solutions

Let me start by loading the skill and exploring the directories.

**Tool: skill**

**Input:**
```json
{
  "name": "c_api"
}
```

**Output:**
```
<skill_content name="c_api">
# Skill: c_api

# Ascend 3510 NPU 架构 C API 技能文档

## 概述

3510 C API（适用于 Ascend 950PR/950DT）为 NPU（神经网络处理器）提供了底层的、面向硬件的编程接口。
这些 API 是对编译器 CCE（Cube Compute Engine）内部指令的薄封装，提供了 C 语言编程体验，支持数组分配内存，可直接控制
数据搬运、矢量/矩阵计算、同步以及系统状态。

本文档涵盖 3510 架构 C API（即 `npu_arch_3510`），兼容 Ascend 950PR 和 Ascend 950DT 硬件。

---

## 重要限制

- 注意函数标识符和变量类型的匹配
- `vector_int`, `vector_float` 等类型仅在`__simd_vf__`中使用，为寄存器RegTensor的数据类型
- `__simd_vf__` 仅支持在vector core中执行
- `__simd_vf__` 中不支持浮点数的除法，不支持static_cast<>()
- `__simd_vf__` 内的标量应尽量使用常量，变量声明应在for循环外
- `vector_bool` 类型用于在`__simd_vf__`中控制参与计算的元素位置
- `vector_bool` 类型采用正常的变量声明方式，使用方式参考[./examples/example.md](./examples/example.md)中的RegBase编程模板
- 3510架构对于Vector类型的运算，均需要搬入到寄存器中进行计算，计算完成后再搬出到GM。即数据通路为gm -> ub -> reg -> ub -> gm
- 特殊数据类型如fp4和fp8,int4等，在Host侧没有定义，需手动进行编解码函数的编写，通过地址的方式传递到Device侧
- UB中的数据需要加`__ubuf__`前缀声明
- 指令的使用文档请参考[../../../docs/api/c_api/](../../../docs/api/c_api/)其中3510的部分
- 禁止随意揣测指令，必要时查阅文档确认
- 编译时使用`cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;`在仿真模式下编译运行
- 执行精度对比时，应将输入、输出、真值数据打印并存储为csv文件，如果存在精度问题，打印出现精度问题的元素的值和索引

## 快速入门

### vector类型和vector_bool类型

- **当存在输入输出位宽变化时，updatemask和ub上地址偏移的stride,也需对应变化**
- **当输入输出位宽变化时，数据排列中可能存在空位填充为0，应灵活使用`asc_pack`，`asc_unpack`，`asc_interleave`等接口使数据紧密排列**
- 所有的`vector_xxx`类型的变量声明只能在`__simd_vf__`函数内
- 除了`vector_bool`外，所有`vector_xxx`类型固定为256B, 只根据具体类型，将该256B解释为该类型的vector
- `vector_bool`固定为32B, 每个bit位控制`vector_xxx`类型中每个B是否参与运算，作为mask使用
- `asc_update_mask`应被赋值给一个`vector_bool`类型的变量
- `asc_update_mask(count)`，设置对应位宽的前count个元素参与vector的运算（不超过256B），在执行之后，count自减对应元素个数，最低为0

示例：
- `vector_xxx`类型变量的示例，如`vector_float`类型的变量，长度为256B, 即64个float类型的数据
- `vector_xxx`类型变量的示例，如`vector_int8_t`类型的变量，长度为256B, 即256个int8_t类型的数据
- 对于数据类型大于1B的`vector_xxx`类型的变量，vector_bool类型的变量中多个bit位对应一个元素，只有最低位生效
- vector_bool类型的变量的示例，如`vector_bool`中前16 bit值为`0001 0001 0001 0000`，其控制`vector_float`类型的变量为前3个元素参与运算，第4个不参与
- vector_bool类型的变量的示例，如`vector_bool`中前16 bit值为`00 01 00 01 00 01 00 00`，其控制`vector_half`类型的变量为前6个元素间隔参与运算，第7, 8个不参与

### 头文件引入

```c
#include "c_api/asc_simd.h"
```

单个头文件即可包含所有 C API 接口。也可以分别引入各类别头文件（如 `vector_compute.h`、`cube_datamove.h`、`sync.h`）。

### Kernel 函数结构
```cpp
// 函数标识符及函数声明, __vector__ 表示运行在vector core中
__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    // 初始化
    asc_init();

    // 计算每个block处理的元素数量
    uint32_t blockLength = TILE_LENGTH * NUM_BLOCKS / asc_get_block_num();

    // 计算当前block处理的元素起始地址
    __gm__ float* xGm = x + get_block_idx() * blockLength;
    __gm__ float* yGm = y + get_block_idx() * blockLength;
    __gm__ float* zGm = z + get_block_idx() * blockLength;

    // 通过数组方式申请UB中的内存
    __ubuf__ float xLocal[TILE_LENGTH];
    __ubuf__ float yLocal[TILE_LENGTH];
    __ubuf__ float zLocal[TILE_LENGTH];

    // 配置相关参数，包括缓存模式、数据长度、步长等
    const uint8_t cacheMode0 = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
    const uint8_t cacheMode1 = static_cast<uint8_t>(((uint64_t)yGm) >> 60);
    const uint8_t cacheMode2 = static_cast<uint8_t>(((uint64_t)zGm) >> 60);
    uint32_t burstLength = blockLength * 4;
    uint64_t srcStride = 0;
    uint32_t dstStride = 0;

    // 从GM拷贝数据到UB
    asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode0, srcStride, dstStride);
    asc_copy_gm2ub_align((__ubuf__ float*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode1, srcStride, dstStride);

    // 同步指令，确保数据拷贝完成后再继续执行
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    // 计算每个rep的元素数量
    uint16_t mask_bit_size = 256;
    uint16_t one_rep_size = mask_bit_size / sizeof(float);
    uint16_t rep = (blockLength + one_rep_size - 1) / one_rep_size;
    // 调用VF函数
    AddVF(rep, one_rep_size, blockLength, (__ubuf__ float*)xLocal, (__ubuf__ float*)yLocal, (__ubuf__ float*)zLocal );

    // 同步指令，确保VF函数执行后再继续执行
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

    // 从UB拷贝数据到GM 
    asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode2, srcStride, dstStride);
}
```

### Kernel 调用流程

典型的 C API Kernel 调用遵循以下模式：

```c
std::vector<float> kernel_add(std::vector<float> &x, std::vector<float> &y)
{
    // 步骤1：相关变量的声明，数据切分，设备Id选择
    constexpr uint32_t numBlocks = NUM_BLOCKS;
    uint32_t totalLength = x.size();
    size_t totalByteSize = totalLength * sizeof(float);
    int32_t deviceId = 0;
    aclrtStream stream = nullptr;

    // 步骤2：Host侧和Device侧存储数据的变量声明
    uint8_t *xHost = reinterpret_cast<uint8_t *>(x.data());
    uint8_t *yHost = reinterpret_cast<uint8_t *>(y.data());
    uint8_t *zHost = nullptr;
    float *xDevice = nullptr;
    float *yDevice = nullptr;
    float *zDevice = nullptr;
    
    // 步骤3：初始化ACL运行时和数据拷贝
    aclInit(nullptr);
    aclrtSetDevice(deviceId);
    aclrtCreateStream(&stream);

    aclrtMallocHost((void **)(&zHost), totalByteSize);
    aclrtMalloc((void **)&xDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&yDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void **)&zDevice, totalByteSize, ACL_MEM_MALLOC_HUGE_FIRST);

    aclrtMemcpy((uint8_t*)xDevice, totalByteSize, xHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy((uint8_t*)yDevice, totalByteSize, yHost, totalByteSize, ACL_MEMCPY_HOST_TO_DEVICE);

    // 步骤4：调用自定义kernel
    add_custom<<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);

    // 步骤5：同步并回读结果，将Host侧结果存储在zHost中
    aclrtSynchronizeStream(stream);
    aclrtMemcpy(zHost, totalByteSize, (uint8_t*)zDevice, totalByteSize, ACL_MEMCPY_DEVICE_TO_HOST);
    std::vector<float> z((float *)zHost, (float *)(zHost + totalByteSize));

    // 步骤6：资源释放，清理ACL运行时
    aclrtFree(xDevice);
    aclrtFree(yDevice);
    aclrtFree(zDevice);
    aclrtFreeHost(zHost);

    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();

    // 步骤7：返回结果z
    return z;
}
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

### 模式 1：多核分块

```c
uint32_t blockLen = totalLen / asc_get_block_num();
uint32_t offset = asc_get_block_idx() * blockLen;
// 每个核处理自己的数据分片
```

### 模式 2：原子 GM 写入

```c
asc_copy_ub2gm(gm_dst, ub_src0, size);   // 普通写入
asc_sync_pipe(PIPE_MTE3);
asc_set_atomic_add_float();               // 开启原子加
asc_copy_ub2gm(gm_dst, ub_src1, size);   // 原子累加写入
asc_set_atomic_none();                    // 关闭原子模式
```

---

## 支持的数据类型

| C/C++ 类型   | 寄存器Reg中矢量类型别名           |
|--------------|------------------------|
| `half`       | `vector_half`           |
| `float`      | `vector_float`          |
| `bfloat16_t` | `vector_bfloat16_t`     |
| `int8_t`     | `vector_int8_t`         |
| `int16_t`    | `vector_int16_t`        |
| `int32_t`    | `vector_int32_t`        |
| `int64_t`    | `vector_int64_t`        |
| `uint8_t`    | `vector_uint8_t`        |
| `uint16_t`   | `vector_uint16_t`       |
| `uint32_t`   | `vector_uint32_t`       |
| `fp8_e4m3`   | `vector_fp8_e4m3fn_t`  |
| `fp8_e5m2`   | `vector_fp8_e5m2_t`    |
| `int4x2_t`   | `vector_int4x2_t`       |
| `fp4x2_e2m1` | `vector_fp4x2_e2m1_t`  |
| `fp4x2_e1m2` | `vector_fp4x2_e1m2_t`  |
| `hifloat8_t` | `vector_hifloat8_t`     |

---

完整的 API 参考请参见 [reference.md](reference.md)。
完整的可运行示例请参见 [example.md](example.md)。

Base directory for this skill: file:///home/joey/code/joey/asc-devkit/.opencode/skills/skill_3510_c_api
Relative paths in this skill (e.g., scripts/, reference/) are relative to this base directory.
Note: file list is sampled.

<skill_files>
<file>/home/joey/code/joey/asc-devkit/.opencode/skills/skill_3510_c_api/examples/example.md</file>
</skill_files>
</skill_content>
```

**Tool: task**

**Input:**
```json
{
  "description": "List vector_compute_impl dirs",
  "prompt": "List all subdirectories under impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/ in the current workspace. Just return the list of directory names.",
  "subagent_type": "explore"
}
```

**Output:**
```
task_id: ses_1c06f82acffeYnn823bQLeVm3L (for resuming to continue this task if needed)

<task_result>
Only one subdirectory exists under that path:

- `vconv/`
</task_result>
```

**Tool: task**

**Input:**
```json
{
  "description": "List vector_compute_examples dirs",
  "prompt": "List all subdirectories under vector_compute_examples/ in the current workspace. Just return the list of directory names.",
  "subagent_type": "explore"
}
```

**Output:**
```
task_id: ses_1c06f761affeqfLvyEva26X3tm (for resuming to continue this task if needed)

<task_result>
Here are the subdirectories under `vector_compute_examples/`:

- asc_abs_sub
- asc_abs
- asc_add_scalar
- asc_add
- asc_and
- asc_arange
- asc_axpy
- asc_deintlv
- asc_div
- asc_eq
- asc_exp
- asc_ge
- asc_gt
- asc_intlv
- asc_le
- asc_leakyrelu
- asc_ln
- asc_lt
- asc_madd
- asc_max
- asc_min
- asc_mul_scalar
- asc_mul
- asc_mull
- asc_ne
- asc_neg
- asc_not
- asc_or
- asc_pack
- asc_pair_reduce_sum
- asc_prelu
- asc_reduce_max
- asc_reduce_min
- asc_reduce_sum
- asc_relu
- asc_select
- asc_shiftleft
- asc_shiftright
- asc_sqrt
- asc_squeeze
- asc_sub
- asc_transpose
- asc_unpack_lower
- asc_xor
- error_cases
- vconv
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 11.1s)

_Thinking:_

Let me analyze what I have:

1. From `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` - only `vconv/` is listed. Let me check more carefully - the explore agent said there's only one subdirectory. But wait, the user said "select 5 from the impl directory" - maybe the directories are named differently. Let me look more carefully at the impl directory.

2. From `vector_compute_examples/` - there are many examples. The `vconv` directory exists in both.

Let me search more carefully for the impl directory structure. Maybe there are source files directly in that directory, or maybe subdirectories are organized differently.

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl</path>
<type>directory</type>
<entries>
asc_abs_impl.h
asc_abs_sub_impl.h
asc_add_impl.h
asc_add_scalar_impl.h
asc_addc_impl.h
asc_and_impl.h
asc_arange_impl.h
asc_axpy_impl.h
asc_bitsort_impl.h
asc_clear_ar_spr_impl.h
asc_copy_impl.h
asc_create_iter_reg_impl.h
asc_cumulative_histogram_impl.h
asc_deintlv_impl.h
asc_div_impl.h
asc_duplicate_impl.h
asc_duplicate_scalar_impl.h
asc_eq_impl.h
asc_eq_scalar_impl.h
asc_exp_impl.h
asc_exp_sub_impl.h
asc_frequency_histogram_impl.h
asc_ge_impl.h
asc_ge_scalar_impl.h
asc_gt_impl.h
asc_gt_scalar_impl.h
asc_intlv_impl.h
asc_le_impl.h
asc_le_scalar_impl.h
asc_leakyrelu_impl.h
asc_ln_impl.h
asc_lt_impl.h
asc_lt_scalar_impl.h
asc_madd_impl.h
asc_max_impl.h
asc_max_scalar.h
asc_min_impl.h
asc_min_scalar_impl.h
asc_mrgsort4_impl.h
asc_mul_impl.h
asc_mul_scalar_impl.h
asc_mull_impl.h
asc_muls_impl.h
asc_ne_scalar.h
asc_ne.h
asc_neg_impl.h
asc_not_impl.h
asc_or_impl.h
asc_pack_impl.h
asc_pair_reduce_sum_impl.h
asc_prelu_impl.h
asc_reduce_max_datablock_impl.h
asc_reduce_max_impl.h
asc_reduce_min_datablock_impl.h
asc_reduce_min_impl.h
asc_reduce_sum_datablock.h
asc_reduce_sum.h
asc_relu_impl.h
asc_select_impl.h
asc_set_va_reg_impl.h
asc_shiftleft_impl.h
asc_shiftleft_scalar_impl.h
asc_shiftright_impl.h
asc_shiftright_scalar_impl.h
asc_sqrt_impl.h
asc_squeeze_impl.h
asc_sub_impl.h
asc_subc_impl.h
asc_transpose_impl.h
asc_transto5hd_impl.h
asc_unpack_impl.h
asc_unsqueeze_impl.h
asc_update_mask_impl.h
asc_xor_impl.h
vconv/

(75 entries)
</entries>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples</path>
<type>directory</type>
<entries>
asc_abs_sub/
asc_abs/
asc_add_scalar/
asc_add/
asc_and/
asc_arange/
asc_axpy/
asc_deintlv/
asc_div/
asc_eq/
asc_exp/
asc_ge/
asc_gt/
asc_intlv/
asc_le/
asc_leakyrelu/
asc_ln/
asc_lt/
asc_madd/
asc_max/
asc_min/
asc_mul_scalar/
asc_mul/
asc_mull/
asc_ne/
asc_neg/
asc_not/
asc_or/
asc_pack/
asc_pair_reduce_sum/
asc_prelu/
asc_reduce_max/
asc_reduce_min/
asc_reduce_sum/
asc_relu/
asc_select/
asc_shiftleft/
asc_shiftright/
asc_sqrt/
asc_squeeze/
asc_sub/
asc_transpose/
asc_unpack_lower/
asc_xor/
clear_build.sh
error_cases/
run_all.sh
session-deintlv_CHECK.md
session-madd_not.md
vconv/

(50 entries)
</entries>
```

---

## Assistant (Build · DeepSeek V4 Pro · 441.7s)

_Thinking:_

Now I can see the full picture. Let me find which interfaces exist in impl but NOT in examples:

Existing in examples (vector_compute_examples/):
- asc_abs_sub, asc_abs, asc_add_scalar, asc_add, asc_and, asc_arange, asc_axpy, asc_deintlv, asc_div, asc_eq, asc_exp, asc_ge, asc_gt, asc_intlv, asc_le, asc_leakyrelu, asc_ln, asc_lt, asc_madd, asc_max, asc_min, asc_mul_scalar, asc_mul, asc_mull, asc_ne, asc_neg, asc_not, asc_or, asc_pack, asc_pair_reduce_sum, asc_prelu, asc_reduce_max, asc_reduce_min, asc_reduce_sum, asc_relu, asc_select, asc_shiftleft, asc_shiftright, asc_sqrt, asc_squeeze, asc_sub, asc_transpose, asc_unpack_lower, asc_xor, vconv, error_cases

Existing in impl (vector_compute_impl/):
- asc_abs_impl.h, asc_abs_sub_impl.h, asc_add_impl.h, asc_add_scalar_impl.h, asc_addc_impl.h, asc_and_impl.h, asc_arange_impl.h, asc_axpy_impl.h, asc_bitsort_impl.h, asc_clear_ar_spr_impl.h, asc_copy_impl.h, asc_create_iter_reg_impl.h, asc_cumulative_histogram_impl.h, asc_deintlv_impl.h, asc_div_impl.h, asc_duplicate_impl.h, asc_duplicate_scalar_impl.h, asc_eq_impl.h, asc_eq_scalar_impl.h, asc_exp_impl.h, asc_exp_sub_impl.h, asc_frequency_histogram_impl.h, asc_ge_impl.h, asc_ge_scalar_impl.h, asc_gt_impl.h, asc_gt_scalar_impl.h, asc_intlv_impl.h, asc_le_impl.h, asc_le_scalar_impl.h, asc_leakyrelu_impl.h, asc_ln_impl.h, asc_lt_impl.h, asc_lt_scalar_impl.h, asc_madd_impl.h, asc_max_impl.h, asc_max_scalar.h, asc_min_impl.h, asc_min_scalar_impl.h, asc_mrgsort4_impl.h, asc_mul_impl.h, asc_mul_scalar_impl.h, asc_mull_impl.h, asc_muls_impl.h, asc_ne_scalar.h, asc_ne.h, asc_neg_impl.h, asc_not_impl.h, asc_or_impl.h, asc_pack_impl.h, asc_pair_reduce_sum_impl.h, asc_prelu_impl.h, asc_reduce_max_datablock_impl.h, asc_reduce_max_impl.h, asc_reduce_min_datablock_impl.h, asc_reduce_min_impl.h, asc_reduce_sum_datablock.h, asc_reduce_sum.h, asc_relu_impl.h, asc_select_impl.h, asc_set_va_reg_impl.h, asc_shiftleft_impl.h, asc_shiftleft_scalar_impl.h, asc_shiftright_impl.h, asc_shiftright_scalar_impl.h, asc_sqrt_impl.h, asc_squeeze_impl.h, asc_sub_impl.h, asc_subc_impl.h, asc_transpose_impl.h, asc_transto5hd_impl.h, asc_unpack_impl.h, asc_unsqueeze_impl.h, asc_update_mask_impl.h, asc_xor_impl.h, vconv/

Interfaces in impl but NOT in examples:
1. asc_addc (add with carry) - impl has asc_addc_impl.h
2. asc_bitsort - impl has asc_bitsort_impl.h
3. asc_clear_ar_spr - impl has asc_clear_ar_spr_impl.h
4. asc_copy - impl has asc_copy_impl.h (this might be a data move, not compute)
5. asc_create_iter_reg - impl has asc_create_iter_reg_impl.h
6. asc_cumulative_histogram - impl has asc_cumulative_histogram_impl.h
7. asc_duplicate - impl has asc_duplicate_impl.h
8. asc_duplicate_scalar - impl has asc_duplicate_scalar_impl.h
9. asc_eq_scalar - impl has asc_eq_scalar_impl.h
10. asc_exp_sub - impl has asc_exp_sub_impl.h
11. asc_frequency_histogram - impl has asc_frequency_histogram_impl.h
12. asc_ge_scalar - impl has asc_ge_scalar_impl.h
13. asc_gt_scalar - impl has asc_gt_scalar_impl.h
14. asc_le_scalar - impl has asc_le_scalar_impl.h
15. asc_lt_scalar - impl has asc_lt_scalar_impl.h
16. asc_max_scalar - impl has asc_max_scalar.h
17. asc_min_scalar - impl has asc_min_scalar_impl.h
18. asc_mrgsort4 - impl has asc_mrgsort4_impl.h
19. asc_muls - impl has asc_muls_impl.h
20. asc_ne_scalar - impl has asc_ne_scalar.h
21. asc_reduce_max_datablock - impl has asc_reduce_max_datablock_impl.h
22. asc_reduce_min_datablock - impl has asc_reduce_min_datablock_impl.h
23. asc_reduce_sum_datablock - impl has asc_reduce_sum_datablock.h
24. asc_set_va_reg - impl has asc_set_va_reg_impl.h
25. asc_shiftleft_scalar - impl has asc_shiftleft_scalar_impl.h
26. asc_shiftright_scalar - impl has asc_shiftright_scalar_impl.h
27. asc_subc - impl has asc_subc_impl.h
28. asc_transto5hd - impl has asc_transto5hd_impl.h
29. asc_unpack (not lower) - impl has asc_unpack_impl.h (example has asc_unpack_lower)
30. asc_unsqueeze - impl has asc_unsqueeze_impl.h
31. asc_update_mask - impl has asc_update_mask_impl.h

Now, I need to select 5 from these that would make good test examples. Let me think about which ones are most appropriate for vector compute tests:

Reasonable choices (vector compute focused, not too esoteric):
1. **asc_addc** - add with carry, a vector compute operation
2. **asc_subc** - subtract with carry (complementary to addc)
3. **asc_duplicate** - duplicate scalar to vector
4. **asc_ge_scalar** - greater-than-or-equal with scalar (scalar variant of ge)
5. **asc_shiftleft_scalar** - shift left by scalar

Wait, let me reconsider. Let me look at what the existing examples look like to understand the pattern. Let me also check which ones are "vector compute" operations vs. data move or scalar operations.

Actually, let me first check the existing example structure and the build system. Then I'll select 5 appropriate interfaces.

Let me look at an existing example to understand the structure, and also the build scripts.

**Tool: task**

**Input:**
```json
{
  "description": "Explore example structure",
  "subagent_type": "explore",
  "prompt": "I need to understand the structure of examples in vector_compute_examples/. Please:\n\n1. Read the file structure of vector_compute_examples/asc_add/ (list all files)\n2. Read the main .cpp or .asc source file in vector_compute_examples/asc_add/\n3. Read the CMakeLists.txt in vector_compute_examples/asc_add/\n4. Read the file structure of vector_compute_examples/asc_ge/ (list all files)\n5. Read the main source file in vector_compute_examples/asc_ge/\n6. Read the CMakeLists.txt in vector_compute_examples/asc_ge/\n7. Read vector_compute_examples/run_all.sh\n8. Read vector_compute_examples/clear_build.sh\n9. Read the file structure of vector_compute_examples/vconv/ (list all files)\n10. Read the main source file in vector_compute_examples/vconv/\n\nReturn all content you read."
}
```

**Output:**
```
task_id: ses_1c06e2452ffeJafjCKtyjGThtP (for resuming to continue this task if needed)

<task_result>
Here is a comprehensive report of all files and their contents.

---

## 1. File structure of `vector_compute_examples/asc_add/`

```
vector_compute_examples/asc_add/
├── build_and_run.sh
├── c_api_add.asc
└── CMakeLists.txt
```

---

## 2. Main source file: `vector_compute_examples/asc_add/c_api_add.asc`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_add/c_api_add.asc` (467 lines)

This is a register-level vector addition example ("asc_add"). It covers **4 data types** (`half`, `float`, `int32_t`, `int16_t`) in a **multi-core** scenario.

**Kernel-side (Device):**
- Four `__simd_vf__` inline helper functions that perform register-level vector add in a loop:
  - `AddVF_half` (256B per iteration = 128 half elements)
  - `AddVF_float` (256B per iteration = 64 float elements)
  - `AddVF_int32` (256B per iteration = 64 int32_t elements)
  - `AddVF_int16` (256B per iteration = 128 int16_t elements)
- Each function uses `asc_update_mask_b16`/`b32`, `asc_loadalign`, `asc_add`, `asc_storealign`.
- Four corresponding `__vector__ __global__ __aicore__` kernel functions (one per type) with a fixed pattern:
  - `asc_init()`
  - Copy data from global memory (`__gm__`) to unified buffer (`__ubuf__`) via `asc_copy_gm2ub_align`
  - `asc_sync_notify`/`asc_sync_wait` for pipe synchronization (MTE2 → V → MTE3)
  - Call the SIMD-VF helper
  - Copy results back to global memory via `asc_copy_ub2gm_align`

**Host-side (CPU):**
- Template function `launch_add<T>` that uses ACL runtime APIs (aclInit, aclrtSetDevice, aclrtMalloc, aclrtMemcpy) to allocate device memory, launch the kernel with `kernel<<<NUM_BLOCKS, nullptr, stream>>>`, synchronize, and copy results back.
- Template function `verify_add<T>` computes a golden reference (`x[i] + y[i]`), prints first 20 elements of X, Y, Output, Expected, saves a CSV file (`add_<type>.csv`), and reports mismatches.
- `main()` tests all 4 types with `NUM_BLOCKS = 8`:
  - `half`: N = 8 * 512 = 4096
  - `float`: N = 8 * 256 = 2048
  - `int32_t`: N = 8 * 256 = 2048
  - `int16_t`: N = 8 * 512 = 4096

---

## 3. CMakeLists.txt in `vector_compute_examples/asc_add/`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_add/CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(c_api_add_example c_api_add.asc)
target_compile_options(c_api_add_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
```

Key points:
- Requires CMake >= 3.16
- Uses `find_package(ASC REQUIRED)` -- a custom CMake package for ASC compilation
- Project languages: `ASC` (for the `.asc` kernel file) and `CXX` (for C++ host code)
- The `.asc` file is compiled with NPU architecture `dav-3510`

---

## 4. File structure of `vector_compute_examples/asc_ge/`

```
vector_compute_examples/asc_ge/
├── build_and_run.sh
├── build_log.txt
├── c_api_ge.asc
└── CMakeLists.txt
```

*(Note: `build_log.txt` is likely a leftover build output, not part of the source.)*

---

## 5. Main source file: `vector_compute_examples/asc_ge/c_api_ge.asc`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge/c_api_ge.asc` (388 lines)

This is a register-level "greater-than-or-equal" comparison example. It computes `z = (x >= y) ? 1 : 0` using `asc_ge` + `asc_select`.

**Kernel-side (Device):**
- Three `__simd_vf__` inline helpers (`GeVF_half`, `GeVF_float`, `GeVF_int32`) that:
  - Pre-load `oneUb`/`zeroUb` (constant buffers filled with 1.0f/0.0f)
  - Loop over blocks: load x, load y, `asc_ge` to get a bool vector, then `asc_select` to pick 1 or 0, store result
  - This converts boolean compare output to numeric 1/0
- Three kernels (`ge_kernel_half`, `ge_kernel_float`, `ge_kernel_int32`) with the same GM→UB copy → sync → VF helper → sync → UB→GM copy pattern.
- The kernels pre-fill `oneUb` and `zeroUb` on UB with `half(1.0f)`/`half(0.0f)`, `1.0f`/`0.0f`, and `1`/`0` respectively.

**Host-side (CPU):**
- `launch_ge<T>`: same ACL runtime pattern as `asc_add`
- `verify_ge<T>`: golden reference is `(x[i] >= y[i]) ? T(1) : T(0)`, saves CSV as `ge_<type>.csv`
- `main()` tests 3 types (half, float, int32_t) with `NUM_BLOCKS = 8`:
  - `half`/`float`: x linearly increasing, y linearly decreasing (crossing point creates interesting comparison pattern)
  - `int32_t`: x = i*3, y = 1000-i

---

## 6. CMakeLists.txt in `vector_compute_examples/asc_ge/`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge/CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(c_api_ge_example c_api_ge.asc)
target_compile_options(c_api_ge_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
```

Identical structure to the `asc_add` CMakeLists, just with a different executable name.

---

## 7. `vector_compute_examples/run_all.sh`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/run_all.sh` (80 lines)

This is a master runner script that iterates through **55 samples** listed in the `SAMPLES` array and executes each one via its `build_and_run.sh`. Key behavior:

- Uses `set -e` (exit on first error in the script itself, but note: errors inside individual sample runs are caught via the `if` check)
- For each sample, checks if `build_and_run.sh` exists; skips if not found
- Runs each `build_and_run.sh`, increments PASS/FAIL counters
- After each run, removes the `build/` directory (`rm -rf`)
- Finally prints summary: `N passed, M failed`

**Full list of samples** (55 total):

| # | Sample | # | Sample | # | Sample |
|---|--------|---|--------|---|--------|
| 1 | asc_abs | 20 | asc_or | 39 | asc_madd |
| 2 | asc_abs_sub | 21 | asc_reduce_min | 40 | asc_shiftright |
| 3 | **asc_add** | 22 | asc_xor | 41 | asc_deintlv |
| 4 | asc_axpy | 23 | asc_shiftleft | 42 | asc_intlv |
| 5 | asc_leakyrelu | 24 | **asc_ge** | 43 | asc_transpose |
| 6 | asc_mul | 25 | asc_mul_scalar | 44 | asc_add_scalar |
| 7 | asc_reduce_max | 26 | asc_eq | 45 | asc_le |
| 8 | asc_relu | 27 | asc_lt | 46 | asc_prelu |
| 9 | asc_select | 28 | asc_gt | 47 | asc_reduce_sum |
| 10 | asc_sub | 29 | asc_not | 48 | asc_ne |
| 11 | asc_neg | 30 | **vconv/asc_e4m32float** | 49 | asc_pair_reduce_sum |
| 12 | asc_max | 31 | asc_sqrt | 50 | asc_mull |
| 13 | asc_min | 32 | asc_exp | 51 | asc_arange |
| 14 | asc_div | 33 | asc_ln | 52 | asc_squeeze |
| 15 | *vconv/asc_e4m32float* | 34 | asc_and | 53 | asc_unpack_lower |
| 16 | asc_sqrt | 35 | asc_or | 54 | asc_pack |
| 17 | asc_exp | 36 | asc_reduce_min | 55 | *(none beyond)* |
| 18 | asc_ln | 37 | asc_xor | | |
| 19 | asc_and | 38 | asc_shiftleft | | |

---

## 8. `vector_compute_examples/clear_build.sh`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/clear_build.sh` (4 lines)

```bash
rm -rf ./*.toml
rm -rf ./*_log
rm -rf ./*.dump
rm -rf ./*.csv
```

A simple cleanup script that removes generated artifacts: `.toml` files, `_log` directories, `.dump` files, and `.csv` result files from all subdirectories.

---

## 9. File structure of `vector_compute_examples/vconv/`

```
vector_compute_examples/vconv/
└── asc_e4m32float/
    ├── build_and_run.sh
    ├── c_api_e4m32float.asc
    └── CMakeLists.txt
```

The `vconv/` directory is a category for type-conversion examples. Currently it contains a single sub-example: `asc_e4m32float`.

---

## 10. Main source file: `vector_compute_examples/vconv/asc_e4m32float/c_api_e4m32float.asc`

**Path:** `/home/joey/code/joey/asc-devkit/vector_compute_examples/vconv/asc_e4m32float/c_api_e4m32float.asc` (293 lines)

This is an **fp8_e4m3 → float** type conversion example. Notable because `fp8_e4m3fn_t` is a struct type that cannot be used in `std::vector` or UB arrays directly.

**Host-side software encoder/decoder:**
- `float_to_fp8_e4m3(float)` -- converts IEEE 754 float to fp8_e4m3 byte (E4M3 format: 1 sign, 4 exponent bias=7, 3 mantissa). Handles NaN, inf, subnormals, rounding.
- `fp8_e4m3_to_float(uint8_t)` -- reverse conversion from fp8 byte back to float.
- These are used ONLY on the host side for test data generation and verification.

**Kernel-side (Device):**
- `E4m3ToFloatVF`: The `__simd_vf__` helper that:
  - Loads 256 fp8 elements from UB (using `asc_loadalign` with a cast from `int8_t*` to `fp8_e4m3fn_t*`)
  - Calls `asc_e4m32float`, `asc_e4m32float_v2`, `_v3`, `_v4` -- these 4 variant intrinsics unpack a single 256-element fp8 vector into four 64-element float vectors
  - Stores each float vector to UB at offset increments of 64
- `e4m3_to_float_kernel`: Single-core kernel (`<<<1, ...>>>`), copies fp8 from GM → UB, calls `E4m3ToFloatVF`, copies float results UB → GM.

**Host-side test:**
- Generates 1024 fp8 values from floats (alternating positive/negative)
- Launches kernel with 1 block
- Uses a `fp8Index()` helper to map output float indices back to source fp8 indices (because the 4-variant unpacking interleaves: element 0→byte 0, element 1→byte 1, ...)
- Verifies with tolerance (`diff > 0.001`), reporting max/avg error
- Saves CSV to `vconv_fp8tofloat.csv`
- Passes if mismatches <= 1% of total

---

### Build scripts (`build_and_run.sh`) -- all three follow the same pattern:

```bash
#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
echo "=== <name> 一键编译运行 ==="
mkdir -p "${BUILD_DIR}"
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
make -j -C "${BUILD_DIR}"
echo ""
"${BUILD_DIR}/<executable_name>"
```

All three configure with `CMAKE_ASC_RUN_MODE=sim` (simulation mode) targeting `dav-3510` architecture, then build with `make -j` and run the resulting executable.

---

### Summary of the example structure pattern

Each example follows a consistent template:

| Component | Description |
|-----------|-------------|
| `c_api_*.asc` | Single file containing both ASC kernel code (`__vector__ __global__ __aicore__`) and C++ host code (`main()`) with ACL runtime API |
| `CMakeLists.txt` | Uses `find_package(ASC)`, compiles `.asc` with `--npu-arch=dav-3510`, creates a single executable |
| `build_and_run.sh` | One-click build+run: cmake (sim mode) → make → execute |
| `run_all.sh` | Master script that iterates over all 55 samples, runs each, tracks pass/fail, cleans build dirs |
| `clear_build.sh` | Cleans up generated artifacts (.toml, _log, .dump, .csv) |

Each kernel follows the same pipeline: **GM→UB copy → pipe sync → SIMD-VF compute → pipe sync → UB→GM copy**, with host-side ACL allocation/copy/kernel-launch/synchronization/verification.
</task_result>
```

**Tool: task**

**Input:**
```json
{
  "description": "Read impl header files",
  "subagent_type": "explore",
  "prompt": "I need to understand several API implementations and select 5 for writing test examples. Please read these impl files and return their full contents:\n\n1. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_addc_impl.h\n2. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_subc_impl.h\n3. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_duplicate_impl.h\n4. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ge_scalar_impl.h\n5. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_shiftleft_scalar_impl.h\n6. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_shiftright_scalar_impl.h\n7. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h\n8. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_gt_scalar_impl.h\n9. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_lt_scalar_impl.h\n10. impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_le_scalar_impl.h\n\nReturn the full content of each file."
}
```

**Output:**
```
task_id: ses_1c06df254ffebathjQJRTmm786 (for resuming to continue this task if needed)

<task_result>
Here are the full contents of all 10 requested implementation files:

---

## 1. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_addc_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_ADDC_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_ADDC_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_addc_impl(vector_bool& dst0, vector_uint32_t& dst1,
    vector_uint32_t src0, vector_uint32_t src1, vector_bool src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vaddcs(dst0, dst1, src0, src1, src2, mask);
    }
}

__simd_callee__ inline void asc_addc_impl(vector_bool& dst0, vector_int32_t& dst1,
    vector_int32_t src0, vector_int32_t src1, vector_bool src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vaddcs(dst0, dst1, src0, src1, src2, mask);
    }
}

#endif
```

**Summary:** `asc_addc_impl` -- Vector add with carry. Takes 3 source operands (src0, src1 as vectors, src2 as carry-in bool), produces 2 outputs (carry-out bool dst0, sum dst1). Overloaded for `uint32_t` and `int32_t`.

---

## 2. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_subc_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_subc_impl.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SUBC_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SUBC_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_subc_impl(vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsubcs(carry, dst, src0, src1, carry_src, mask);
    }
}

__simd_callee__ inline void asc_subc_impl(vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsubcs(carry, dst, src0, src1, carry_src, mask);
    }
}
#endif
```

**Summary:** `asc_subc_impl` -- Vector subtract with carry/borrow. Mirrors `addc` structure: 3 source operands (src0, src1, carry_src), 2 outputs (carry/borrow out + difference). Overloaded for `uint32_t` and `int32_t`.

---

## 3. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_duplicate_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_duplicate_impl.h.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_duplicate_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}
#endif
```

**Summary:** `asc_duplicate_impl` -- Vector element duplication. Takes a single source vector and replicates the lowest element across the entire destination vector (using `vdup` with `POS_LOWEST` and `MODE_ZEROING`). Overloaded for 11 types: uint8, int8, fp8_e4m3, fp8_e5m2, uint16, int16, half, bfloat16, uint32, int32, float.

---

## 4. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ge_scalar_impl.h`

```cpp
/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_ge_scalar_impl.h
 * \brief
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_GE_SCALAR_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_GE_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_half src, half value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_ge_scalar_impl(vector_bool& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_ge(dst, src, value, mask);
    }
}

#endif
```

**Summary:** `asc_ge_scalar_impl` -- Vector greater-or-equal comparison against a scalar. Takes a vector src, a scalar value, and a mask; outputs a `vector_bool` where each element is the result of `src[i] >= value`. Overloaded for 9 types: int8, int16, int32, uint8, uint16, uint32, half, bfloat16, float.

---

## 5. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_shiftleft_scalar_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_shiftleft_scalar_impl.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SHIFTLEFT_SCALAR_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SHIFTLEFT_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_int8_t& dst, vector_int8_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_uint16_t& dst, vector_uint16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_uint32_t& dst, vector_uint32_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_int32_t& dst, vector_int32_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, value, mask, MODE_ZEROING);
    }
}
#endif
```

**Summary:** `asc_shiftleft_scalar_impl` -- Vector element-wise left shift by a scalar amount. Output type matches input type. Shift amount is an `int16_t` scalar. Uses `vshls` with `MODE_ZEROING`. Overloaded for uint8, int8, uint16, int16, uint32, int32.

---

## 6. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_shiftright_scalar_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_shiftright_scalar_impl.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SHIFTRIGHT_SCALAR_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_SHIFTRIGHT_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_int8_t& dst, vector_int8_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_uint16_t& dst, vector_uint16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_uint32_t& dst, vector_uint32_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(vector_int32_t& dst, vector_int32_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, value, mask, MODE_ZEROING);
    }
}
#endif
```

**Summary:** `asc_shiftright_scalar_impl` -- Vector element-wise right shift by a scalar amount. Structurally identical to shiftleft but calls `vshrs` (arithmetic/logical right shift with `MODE_ZEROING`). Overloaded for uint8, int8, uint16, int16, uint32, int32.

---

## 7. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_ne_scalar_impl.h
 * \brief
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_NE_SCALAR_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_NE_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_half src, half value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
         vcmps_ne(dst, src, value, mask);
    }   
}

#endif
```

**Summary:** `asc_ne_scalar_impl` -- Vector not-equal comparison against a scalar. Uses `vcmps_ne`. Overloaded for 9 types: uint8, int8, uint16, int16, uint32, int32, half, float, bfloat16. Note: this file has an internal header warning guard that triggers a `#warning` if included directly (users should include `c_api/asc_simd.h` instead).

---

## 8. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_gt_scalar_impl.h`

```cpp
/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file asc_gt_scalar_impl.h
 * \brief
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
"impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_gt_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_VECTOR_COMPUTE_IMPL_NPU_ARCH_3510_ASC_GT_SCALAR_IMPL_H
#define IMPL_C_API_INSTR_VECTOR_COMPUTE_IMPL_NPU_ARCH_3510_ASC_GT_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

//uint8_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//int8_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//bfloat16
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//uint16_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//int16_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//uint32_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//int32_t
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//half
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_half src, half value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

//float32
__simd_callee__ inline void asc_gt_scalar_impl(vector_bool& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV{
        vcmps_gt(dst, src, value, mask);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
```

**Summary:** `asc_gt_scalar_impl` -- Vector greater-than comparison against a scalar. Uses `vcmps_gt`. Overloaded for 9 types: uint8, int8, bfloat16, uint16, int16, uint32, int32, half, float. Also has the internal header warning guard.

---

## 9. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_lt_scalar_impl.h`

```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_LT_SCALAR_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_LT_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_half src, half value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_lt_scalar_impl(vector_bool& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_lt(dst, src, value, mask);
    }
}

#endif
```

**Summary:** `asc_lt_scalar_impl` -- Vector less-than comparison against a scalar. Uses `vcmps_lt`. Overloaded for 9 types: uint8, int8, bfloat16, uint16, int16, half, uint32, int32, float. Simpler structure than ge/gt/le -- no internal header warning guard.

---

## 10. `/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_le_scalar_impl.h`

```cpp
/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_le_scalar_impl.h
 * \brief
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_le_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_EQ_SCALAR_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_EQ_SCALAR_IMPL_H

#include "instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_half src, half value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}

__simd_callee__ inline void asc_le_scalar_impl(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmps_le(dst, src, value, mask);
    }
}
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
```

**Summary:** `asc_le_scalar_impl` -- Vector less-or-equal comparison against a scalar. Uses `vcmps_le`. Overloaded for 9 types: uint8, int8, uint16, int16, uint32, int32, half, float, bfloat16. Has the internal header warning guard. Note: the include guard uses `ASC_EQ_SCALAR_IMPL_H` (a copy-paste artifact), but the actual function is `asc_le_scalar_impl`.

---

## Overview and Suggested 5 for Test Examples

Here is a quick comparison of all 10 implementations to help you select 5:

| # | API | Category | Overloads | Intrinsic | Notable Characteristics |
|---|-----|----------|-----------|-----------|------------------------|
| 1 | `asc_addc_impl` | Arithmetic w/ carry | 2 (uint32, int32) | `vaddcs` | Dual output (carry + sum), 3 source operands |
| 2 | `asc_subc_impl` | Arithmetic w/ carry | 2 (uint32, int32) | `vsubcs` | Dual output (borrow + diff), 3 source operands |
| 3 | `asc_duplicate_impl` | Data movement | 11 types | `vdup` | Single src, single dst, positional/mode params |
| 4 | `asc_ge_scalar_impl` | Comparison | 9 types | `vcmps_ge` | Bool output, scalar compare |
| 5 | `asc_shiftleft_scalar_impl` | Bitwise shift | 6 types | `vshls` | Scalar shift amount, MODE_ZEROING |
| 6 | `asc_shiftright_scalar_impl` | Bitwise shift | 6 types | `vshrs` | Scalar shift amount, MODE_ZEROING |
| 7 | `asc_ne_scalar_impl` | Comparison | 9 types | `vcmps_ne` | Bool output, scalar compare, has internal guard |
| 8 | `asc_gt_scalar_impl` | Comparison | 9 types | `vcmps_gt` | Bool output, scalar compare, has internal guard |
| 9 | `asc_lt_scalar_impl` | Comparison | 9 types | `vcmps_lt` | Bool output, scalar compare, no internal guard |
| 10 | `asc_le_scalar_impl` | Comparison | 9 types | `vcmps_le` | Bool output, scalar compare, has internal guard |

**Recommended 5 for test examples** (covering diverse categories and patterns):

1. **`asc_addc_impl`** -- Represents the multi-output arithmetic category (carry add)
2. **`asc_duplicate_impl`** -- Represents data movement / element manipulation (broadcast lowest element)
3. **`asc_ge_scalar_impl`** -- Represents the GT/GE comparison family (simple scalar compare)
4. **`asc_shiftleft_scalar_impl`** -- Represents the shift family (bitwise operations)
5. **`asc_ne_scalar_impl`** -- Represents the EQ/NE comparison family (different from GT/GE/LT/LE), and demonstrates the internal header guard pattern
</task_result>
```

---

## Assistant (Build · DeepSeek V4 Pro · 31.1s)

_Thinking:_

Now I have a comprehensive understanding. Let me select 5 interfaces:

From the analysis, the most interesting and complementary set would be:
1. **asc_addc** - Add with carry (dual output, multi-input arithmetic)
2. **asc_duplicate** - Vector element duplication (data manipulation)
3. **asc_ge_scalar** - Compare >= scalar (comparison with scalar)
4. **asc_shiftleft_scalar** - Left shift by scalar
5. **asc_ne_scalar** - Not equal to scalar

However, let me think about which ones are most suitable and interesting:

- All scalar comparison APIs (ge_scalar, gt_scalar, lt_scalar, le_scalar, ne_scalar) are quite similar. I should pick at most 2 of them.
- The shift operations are also similar. I should pick 1 or 2.
- addc and subc are interesting (carry arithmetic) but subc might be too complex to verify.
- duplicate is unique.

Let me finalize my selection:

1. **asc_addc** - unique multi-output arithmetic, testable with uint32_t and int32_t
2. **asc_duplicate** - unique element manipulation, testable with many types (half, float, int32_t, int16_t, uint8_t)
3. **asc_ge_scalar** - comparison against scalar, contrasts with the existing asc_ge (which is vector vs vector)
4. **asc_shiftleft_scalar** - shift by scalar, contrasts with existing asc_shiftleft (which is vector vs vector)
5. **asc_ne_scalar** - not-equal scalar comparison

Actually, let me reconsider. The user said "select 5 interfaces from impl that don't overlap with examples." Let me pick ones that are conceptually different and cover a range of patterns:

1. **asc_addc** - Carry arithmetic (unique pattern: 2 outputs, no existing example similar)
2. **asc_duplicate** - Element duplication/broadcast (unique data manipulation pattern)
3. **asc_ge_scalar** - Scalar comparison (the existing asc_ge uses vector-vs-vector, this is vector-vs-scalar)
4. **asc_shiftleft_scalar** - Scalar shift (existing asc_shiftleft is vector-vs-vector, this is vector-vs-scalar)
5. **asc_ne_scalar** - Not-equal scalar comparison

These 5 cover different categories: arithmetic with carry, data manipulation, comparison with scalar, shift with scalar, equality with scalar.

Now I need to:
1. Create 5 directories under vector_compute_examples/
2. Each directory needs: c_api_*.asc, CMakeLists.txt, build_and_run.sh
3. Update run_all.sh to include the new samples
4. Build and run them
5. Handle any issues

Let me start creating the examples. Let me first look at an existing full example more carefully to make sure I follow the exact pattern. I'll read asc_add's .asc file to get the exact template.

Now I have a clear picture. Let me select 5 non-overlapping interfaces and create the examples.

**Selection:** `asc_addc`, `asc_duplicate`, `asc_ge_scalar`, `asc_ne_scalar`, `asc_shiftleft_scalar`

Let me first read the full existing examples to understand the exact coding patterns:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_add/c_api_add.asc"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_add/c_api_add.asc</path>
<type>file</type>
<content>
1: /**
2:  * Copyright (c) 2026 Huawei Technologies Co., Ltd.
3:  * CANN Open Software License Agreement Version 2.0
4:  */
5: 
6: /* !
7:  * \file c_api_add.asc
8:  * \brief asc_add 寄存器级矢量加法示例
9:  *
10:  * 覆盖数据类型: half, float, int32_t, int16_t
11:  * 覆盖场景: 多核
12:  */
13: 
14: #include <cstdint>
15: #include <iostream>
16: #include <vector>
17: #include <algorithm>
18: #include <cstdlib>
19: #include <string>
20: #include <fstream>
21: #include "acl/acl.h"
22: #include "c_api/asc_simd.h"
23: 
24: constexpr uint32_t NUM_BLOCKS = 8;
25: constexpr uint32_t BLK_NUM = 1;
26: 
27: /* --------------------------------------------------------------------------
28:  * __simd_vf__ Reg 级矢量加法函数
29:  * -------------------------------------------------------------------------- */
30: 
31: __simd_vf__ inline void AddVF_half(__ubuf__ half* xLocal, __ubuf__ half* yLocal,
32:                                     __ubuf__ half* zLocal, uint32_t blockLen)
33: {
34:     vector_half vx, vy, vz;
35:     vector_bool vmask;
36:     uint16_t one_rep_size = 256 / sizeof(half);
37:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
38:     uint32_t remaining = blockLen;
39:     for (uint16_t i = 0; i < rep; ++i) {
40:         vmask = asc_update_mask_b16(remaining);
41:         asc_loadalign(vx, xLocal + i * one_rep_size);
42:         asc_loadalign(vy, yLocal + i * one_rep_size);
43:         asc_add(vz, vx, vy, vmask);
44:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
45:     }
46: }
47: 
48: __simd_vf__ inline void AddVF_float(__ubuf__ float* xLocal, __ubuf__ float* yLocal,
49:                                      __ubuf__ float* zLocal, uint32_t blockLen)
50: {
51:     vector_float vx, vy, vz;
52:     vector_bool vmask;
53:     uint16_t one_rep_size = 256 / sizeof(float);
54:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
55:     uint32_t remaining = blockLen;
56:     for (uint16_t i = 0; i < rep; ++i) {
57:         vmask = asc_update_mask_b32(remaining);
58:         asc_loadalign(vx, xLocal + i * one_rep_size);
59:         asc_loadalign(vy, yLocal + i * one_rep_size);
60:         asc_add(vz, vx, vy, vmask);
61:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
62:     }
63: }
64: 
65: __simd_vf__ inline void AddVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* yLocal,
66:                                      __ubuf__ int32_t* zLocal, uint32_t blockLen)
67: {
68:     vector_int32_t vx, vy, vz;
69:     vector_bool vmask;
70:     uint16_t one_rep_size = 256 / sizeof(int32_t);
71:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
72:     uint32_t remaining = blockLen;
73:     for (uint16_t i = 0; i < rep; ++i) {
74:         vmask = asc_update_mask_b32(remaining);
75:         asc_loadalign(vx, xLocal + i * one_rep_size);
76:         asc_loadalign(vy, yLocal + i * one_rep_size);
77:         asc_add(vz, vx, vy, vmask);
78:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
79:     }
80: }
81: 
82: __simd_vf__ inline void AddVF_int16(__ubuf__ int16_t* xLocal, __ubuf__ int16_t* yLocal,
83:                                      __ubuf__ int16_t* zLocal, uint32_t blockLen)
84: {
85:     vector_int16_t vx, vy, vz;
86:     vector_bool vmask;
87:     uint16_t one_rep_size = 256 / sizeof(int16_t);
88:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
89:     uint32_t remaining = blockLen;
90:     for (uint16_t i = 0; i < rep; ++i) {
91:         vmask = asc_update_mask_b16(remaining);
92:         asc_loadalign(vx, xLocal + i * one_rep_size);
93:         asc_loadalign(vy, yLocal + i * one_rep_size);
94:         asc_add(vz, vx, vy, vmask);
95:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
96:     }
97: }
98: 
99: /* --------------------------------------------------------------------------
100:  * half 类型加法 Kernel (VL=128)
101:  * -------------------------------------------------------------------------- */
102: 
103: constexpr uint32_t ADD_HALF_VL = 128;
104: constexpr uint32_t ADD_HALF_TILE = ADD_HALF_VL * 4;
105: 
106: __vector__ __global__ __aicore__ void add_kernel_half(
107:     __gm__ half* x, __gm__ half* y, __gm__ half* z)
108: {
109:     asc_init();
110: 
111:     uint32_t blockLen = ADD_HALF_TILE * NUM_BLOCKS / asc_get_block_num();
112: 
113:     __gm__ half* xGm = x + get_block_idx() * blockLen;
114:     __gm__ half* yGm = y + get_block_idx() * blockLen;
115:     __gm__ half* zGm = z + get_block_idx() * blockLen;
116: 
117:     __ubuf__ half xLocal[ADD_HALF_TILE];
118:     __ubuf__ half yLocal[ADD_HALF_TILE];
119:     __ubuf__ half zLocal[ADD_HALF_TILE];
120: 
121:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
122:     uint32_t burstLength = blockLen * sizeof(half);
123:     uint64_t srcStride = 0;
124:     uint32_t dstStride = 0;
125: 
126:     asc_copy_gm2ub_align((__ubuf__ half*)xLocal, xGm, BLK_NUM, burstLength,
127:                          0, 0, true, cacheMode, srcStride, dstStride);
128:     asc_copy_gm2ub_align((__ubuf__ half*)yLocal, yGm, BLK_NUM, burstLength,
129:                          0, 0, true, cacheMode, srcStride, dstStride);
130:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
131:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
132: 
133:     AddVF_half(xLocal, yLocal, zLocal, blockLen);
134: 
135:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
136:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
137: 
138:     asc_copy_ub2gm_align(zGm, (__ubuf__ half*)zLocal, BLK_NUM, burstLength,
139:                          cacheMode, srcStride, dstStride);
140: }
141: 
142: /* --------------------------------------------------------------------------
143:  * float 类型加法 Kernel (VL=64)
144:  * -------------------------------------------------------------------------- */
145: 
146: constexpr uint32_t ADD_FLOAT_VL = 64;
147: constexpr uint32_t ADD_FLOAT_TILE = ADD_FLOAT_VL * 4;
148: 
149: __vector__ __global__ __aicore__ void add_kernel_float(
150:     __gm__ float* x, __gm__ float* y, __gm__ float* z)
151: {
152:     asc_init();
153: 
154:     uint32_t blockLen = ADD_FLOAT_TILE * NUM_BLOCKS / asc_get_block_num();
155: 
156:     __gm__ float* xGm = x + get_block_idx() * blockLen;
157:     __gm__ float* yGm = y + get_block_idx() * blockLen;
158:     __gm__ float* zGm = z + get_block_idx() * blockLen;
159: 
160:     __ubuf__ float xLocal[ADD_FLOAT_TILE];
161:     __ubuf__ float yLocal[ADD_FLOAT_TILE];
162:     __ubuf__ float zLocal[ADD_FLOAT_TILE];
163: 
164:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
165:     uint32_t burstLength = blockLen * sizeof(float);
166:     uint64_t srcStride = 0;
167:     uint32_t dstStride = 0;
168: 
169:     asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength,
170:                          0, 0, true, cacheMode, srcStride, dstStride);
171:     asc_copy_gm2ub_align((__ubuf__ float*)yLocal, yGm, BLK_NUM, burstLength,
172:                          0, 0, true, cacheMode, srcStride, dstStride);
173:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
174:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
175: 
176:     AddVF_float(xLocal, yLocal, zLocal, blockLen);
177: 
178:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
179:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
180: 
181:     asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength,
182:                          cacheMode, srcStride, dstStride);
183: }
184: 
185: /* --------------------------------------------------------------------------
186:  * int32_t 类型加法 Kernel (VL=64)
187:  * -------------------------------------------------------------------------- */
188: 
189: constexpr uint32_t ADD_INT32_VL = 64;
190: constexpr uint32_t ADD_INT32_TILE = ADD_INT32_VL * 4;
191: 
192: __vector__ __global__ __aicore__ void add_kernel_int32(
193:     __gm__ int32_t* x, __gm__ int32_t* y, __gm__ int32_t* z)
194: {
195:     asc_init();
196: 
197:     uint32_t blockLen = ADD_INT32_TILE * NUM_BLOCKS / asc_get_block_num();
198: 
199:     __gm__ int32_t* xGm = x + get_block_idx() * blockLen;
200:     __gm__ int32_t* yGm = y + get_block_idx() * blockLen;
201:     __gm__ int32_t* zGm = z + get_block_idx() * blockLen;
202: 
203:     __ubuf__ int32_t xLocal[ADD_INT32_TILE];
204:     __ubuf__ int32_t yLocal[ADD_INT32_TILE];
205:     __ubuf__ int32_t zLocal[ADD_INT32_TILE];
206: 
207:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
208:     uint32_t burstLength = blockLen * sizeof(int32_t);
209:     uint64_t srcStride = 0;
210:     uint32_t dstStride = 0;
211: 
212:     asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength,
213:                          0, 0, true, cacheMode, srcStride, dstStride);
214:     asc_copy_gm2ub_align((__ubuf__ int32_t*)yLocal, yGm, BLK_NUM, burstLength,
215:                          0, 0, true, cacheMode, srcStride, dstStride);
216:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
217:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
218: 
219:     AddVF_int32(xLocal, yLocal, zLocal, blockLen);
220: 
221:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
222:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
223: 
224:     asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength,
225:                          cacheMode, srcStride, dstStride);
226: }
227: 
228: /* --------------------------------------------------------------------------
229:  * int16_t 类型加法 Kernel (VL=128)
230:  * -------------------------------------------------------------------------- */
231: 
232: constexpr uint32_t ADD_INT16_VL = 128;
233: constexpr uint32_t ADD_INT16_TILE = ADD_INT16_VL * 4;
234: 
235: __vector__ __global__ __aicore__ void add_kernel_int16(
236:     __gm__ int16_t* x, __gm__ int16_t* y, __gm__ int16_t* z)
237: {
238:     asc_init();
239: 
240:     uint32_t blockLen = ADD_INT16_TILE * NUM_BLOCKS / asc_get_block_num();
241: 
242:     __gm__ int16_t* xGm = x + get_block_idx() * blockLen;
243:     __gm__ int16_t* yGm = y + get_block_idx() * blockLen;
244:     __gm__ int16_t* zGm = z + get_block_idx() * blockLen;
245: 
246:     __ubuf__ int16_t xLocal[ADD_INT16_TILE];
247:     __ubuf__ int16_t yLocal[ADD_INT16_TILE];
248:     __ubuf__ int16_t zLocal[ADD_INT16_TILE];
249: 
250:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
251:     uint32_t burstLength = blockLen * sizeof(int16_t);
252:     uint64_t srcStride = 0;
253:     uint32_t dstStride = 0;
254: 
255:     asc_copy_gm2ub_align((__ubuf__ int16_t*)xLocal, xGm, BLK_NUM, burstLength,
256:                          0, 0, true, cacheMode, srcStride, dstStride);
257:     asc_copy_gm2ub_align((__ubuf__ int16_t*)yLocal, yGm, BLK_NUM, burstLength,
258:                          0, 0, true, cacheMode, srcStride, dstStride);
259:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
260:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
261: 
262:     AddVF_int16(xLocal, yLocal, zLocal, blockLen);
263: 
264:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
265:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
266: 
267:     asc_copy_ub2gm_align(zGm, (__ubuf__ int16_t*)zLocal, BLK_NUM, burstLength,
268:                          cacheMode, srcStride, dstStride);
269: }
270: 
271: /* ==========================================================================
272:  * Host 侧
273:  * ========================================================================== */
274: 
275: template<typename T, typename KernelFunc>
276: std::vector<T> launch_add(std::vector<T>& x, std::vector<T>& y, KernelFunc kernel)
277: {
278:     uint32_t N = x.size();
279:     size_t bytes = N * sizeof(T);
280:     int32_t devId = 0;
281:     aclrtStream stream = nullptr;
282:     uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());
283:     uint8_t* yHost = reinterpret_cast<uint8_t*>(y.data());
284:     uint8_t* zHost = nullptr;
285:     T *xD = nullptr, *yD = nullptr, *zD = nullptr;
286: 
287:     aclInit(nullptr);
288:     aclrtSetDevice(devId);
289:     aclrtCreateStream(&stream);
290: 
291:     aclrtMallocHost((void**)&zHost, bytes);
292:     aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
293:     aclrtMalloc((void**)&yD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
294:     aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
295: 
296:     aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);
297:     aclrtMemcpy((uint8_t*)yD, bytes, yHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);
298: 
299:     kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, yD, zD);
300:     aclrtSynchronizeStream(stream);
301: 
302:     aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);
303:     std::vector<T> z((T*)zHost, (T*)(zHost + bytes));
304: 
305:     aclrtFree(xD); aclrtFree(yD); aclrtFree(zD);
306:     aclrtFreeHost(zHost);
307:     aclrtDestroyStream(stream);
308:     aclrtResetDevice(devId);
309:     aclFinalize();
310: 
311:     return z;
312: }
313: 
314: template<typename T>
315: void print_val(T val, std::ostream& os)
316: {
317:     if constexpr (std::is_same_v<T, half>) {
318:         os << static_cast<float>(val);
319:     } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {
320:         os << static_cast<int>(val);
321:     } else {
322:         os << val;
323:     }
324: }
325: 
326: template<typename T>
327: void save_csv(const std::vector<T>& x, const std::vector<T>& y,
328:               const std::vector<T>& z, const std::vector<T>& golden,
329:               const std::string& name)
330: {
331:     std::string fname = "add_" + name + ".csv";
332:     std::ofstream f(fname);
333:     f << "index,x,y,output,expected\n";
334:     for (size_t i = 0; i < x.size(); ++i) {
335:         f << i << ",";
336:         print_val(x[i], f);
337:         f << ",";
338:         print_val(y[i], f);
339:         f << ",";
340:         print_val(z[i], f);
341:         f << ",";
342:         print_val(golden[i], f);
343:         f << "\n";
344:     }
345:     f.close();
346:     std::cout << "  [CSV] 已保存到 " << fname << std::endl;
347: }
348: 
349: template<typename T>
350: void print_tensor(const std::vector<T>& t, const std::string& name)
351: {
352:     constexpr size_t maxPrint = 20;
353:     std::cout << "  " << name << " (" << t.size() << " elements): ";
354:     for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {
355:         print_val(t[i], std::cout);
356:         std::cout << " ";
357:     }
358:     if (t.size() > maxPrint) std::cout << "...";
359:     std::cout << std::endl;
360: }
361: 
362: template<typename T>
363: bool verify_add(std::vector<T>& x, std::vector<T>& y,
364:                 std::vector<T>& z, const char* name)
365: {
366:     std::vector<T> g(x.size());
367:     for (size_t i = 0; i < x.size(); ++i) {
368:         g[i] = x[i] + y[i];
369:     }
370: 
371:     print_tensor(x, "X       ");
372:     print_tensor(y, "Y       ");
373:     print_tensor(z, "Output  ");
374:     print_tensor(g, "Expected");
375: 
376:     save_csv(x, y, z, g, name);
377: 
378:     bool ok = true;
379:     std::vector<size_t> errIdx;
380:     for (size_t i = 0; i < x.size(); ++i) {
381:         if (z[i] != g[i]) {
382:             ok = false;
383:             errIdx.push_back(i);
384:         }
385:     }
386: 
387:     if (!ok) {
388:         std::cout << "  [" << name << "] 精度错误: " << errIdx.size()
389:                   << " 个元素不匹配" << std::endl;
390:         size_t maxErr = std::min(errIdx.size(), size_t(8));
391:         for (size_t e = 0; e < maxErr; ++e) {
392:             size_t idx = errIdx[e];
393:             std::cout << "    [" << idx << "] output=";
394:             print_val(z[idx], std::cout);
395:             std::cout << " expected=";
396:             print_val(g[idx], std::cout);
397:             std::cout << std::endl;
398:         }
399:         if (errIdx.size() > maxErr) {
400:             std::cout << "    ... (还有 " << (errIdx.size() - maxErr)
401:                       << " 个错误)" << std::endl;
402:         }
403:         return false;
404:     }
405: 
406:     std::cout << "  [" << name << "] 通过" << std::endl;
407:     return true;
408: }
409: 
410: /* ==========================================================================
411:  * main
412:  * ========================================================================== */
413: 
414: int32_t main(int32_t argc, char* argv[])
415: {
416:     std::cout << "=== asc_add 寄存器级矢量加法测试 ===" << std::endl;
417:     std::cout << "模式: 多核 (" << NUM_BLOCKS << " blocks)" << std::endl;
418:     std::cout << std::endl;
419: 
420:     {
421:         constexpr uint32_t N = NUM_BLOCKS * ADD_HALF_TILE;
422:         std::vector<half> x(N), y(N);
423:         for (uint32_t i = 0; i < N; ++i) {
424:             x[i] = static_cast<half>(i * 0.1f);
425:             y[i] = static_cast<half>(i * 0.2f);
426:         }
427:         auto z = launch_add<half>(x, y, add_kernel_half);
428:         verify_add(x, y, z, "half");
429:     }
430:     std::cout << std::endl;
431:     {
432:         constexpr uint32_t N = NUM_BLOCKS * ADD_FLOAT_TILE;
433:         std::vector<float> x(N), y(N);
434:         for (uint32_t i = 0; i < N; ++i) {
435:             x[i] = i * 0.1f;
436:             y[i] = i * 0.2f;
437:         }
438:         auto z = launch_add<float>(x, y, add_kernel_float);
439:         verify_add(x, y, z, "float");
440:     }
441:     std::cout << std::endl;
442:     {
443:         constexpr uint32_t N = NUM_BLOCKS * ADD_INT32_TILE;
444:         std::vector<int32_t> x(N), y(N);
445:         for (uint32_t i = 0; i < N; ++i) {
446:             x[i] = static_cast<int32_t>(i);
447:             y[i] = static_cast<int32_t>(i * 2);
448:         }
449:         auto z = launch_add<int32_t>(x, y, add_kernel_int32);
450:         verify_add(x, y, z, "int32_t");
451:     }
452:     std::cout << std::endl;
453:     {
454:         constexpr uint32_t N = NUM_BLOCKS * ADD_INT16_TILE;
455:         std::vector<int16_t> x(N), y(N);
456:         for (uint32_t i = 0; i < N; ++i) {
457:             x[i] = static_cast<int16_t>(i % 1000);
458:             y[i] = static_cast<int16_t>((i % 500) * 2);
459:         }
460:         auto z = launch_add<int16_t>(x, y, add_kernel_int16);
461:         verify_add(x, y, z, "int16_t");
462:     }
463: 
464:     std::cout << std::endl;
465:     std::cout << "[完成] asc_add 示例测试结束。" << std::endl;
466:     return 0;
467: }

(End of file - total 467 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft/c_api_shiftleft.asc"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft/c_api_shiftleft.asc</path>
<type>file</type>
<content>
1: /**
2:  * Copyright (c) 2026 Huawei Technologies Co., Ltd.
3:  * CANN Open Software License Agreement Version 2.0
4:  */
5: 
6: /* !
7:  * \file c_api_shiftleft.asc
8:  * \brief asc_shiftleft 寄存器级矢量左移示例
9:  *
10:  * 覆盖数据类型: int8_t, int16_t, int32_t, uint8_t, uint16_t, uint32_t
11:  * 覆盖场景: 多核
12:  */
13: 
14: #include <cstdint>
15: #include <iostream>
16: #include <vector>
17: #include <algorithm>
18: #include <cstdlib>
19: #include <string>
20: #include <fstream>
21: #include "acl/acl.h"
22: #include "c_api/asc_simd.h"
23: 
24: constexpr uint32_t NUM_BLOCKS = 8;
25: constexpr uint32_t BLK_NUM = 1;
26: 
27: /* --------------------------------------------------------------------------
28:  * __simd_vf__ Reg 级矢量左移函数
29:  * src0: 待移位数据, src1: 移位量
30:  * -------------------------------------------------------------------------- */
31: 
32: __simd_vf__ inline void ShiftLeftVF_int8(__ubuf__ int8_t* xLocal, __ubuf__ int8_t* shiftLocal,
33:                                           __ubuf__ int8_t* zLocal, uint32_t blockLen)
34: {
35:     vector_int8_t vx, vshift, vz;
36:     vector_bool vmask;
37:     uint16_t one_rep_size = 256 / sizeof(int8_t);
38:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
39:     uint32_t remaining = blockLen;
40:     for (uint16_t i = 0; i < rep; ++i) {
41:         vmask = asc_update_mask_b8(remaining);
42:         asc_loadalign(vx, xLocal + i * one_rep_size);
43:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
44:         asc_shiftleft(vz, vx, vshift, vmask);
45:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
46:     }
47: }
48: 
49: __simd_vf__ inline void ShiftLeftVF_int16(__ubuf__ int16_t* xLocal, __ubuf__ int16_t* shiftLocal,
50:                                            __ubuf__ int16_t* zLocal, uint32_t blockLen)
51: {
52:     vector_int16_t vx, vshift, vz;
53:     vector_bool vmask;
54:     uint16_t one_rep_size = 256 / sizeof(int16_t);
55:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
56:     uint32_t remaining = blockLen;
57:     for (uint16_t i = 0; i < rep; ++i) {
58:         vmask = asc_update_mask_b16(remaining);
59:         asc_loadalign(vx, xLocal + i * one_rep_size);
60:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
61:         asc_shiftleft(vz, vx, vshift, vmask);
62:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
63:     }
64: }
65: 
66: __simd_vf__ inline void ShiftLeftVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* shiftLocal,
67:                                            __ubuf__ int32_t* zLocal, uint32_t blockLen)
68: {
69:     vector_int32_t vx, vshift, vz;
70:     vector_bool vmask;
71:     uint16_t one_rep_size = 256 / sizeof(int32_t);
72:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
73:     uint32_t remaining = blockLen;
74:     for (uint16_t i = 0; i < rep; ++i) {
75:         vmask = asc_update_mask_b32(remaining);
76:         asc_loadalign(vx, xLocal + i * one_rep_size);
77:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
78:         asc_shiftleft(vz, vx, vshift, vmask);
79:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
80:     }
81: }
82: 
83: __simd_vf__ inline void ShiftLeftVF_uint8(__ubuf__ uint8_t* xLocal, __ubuf__ int8_t* shiftLocal,
84:                                            __ubuf__ uint8_t* zLocal, uint32_t blockLen)
85: {
86:     vector_uint8_t vx, vz;
87:     vector_int8_t vshift;
88:     vector_bool vmask;
89:     uint16_t one_rep_size = 256 / sizeof(uint8_t);
90:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
91:     uint32_t remaining = blockLen;
92:     for (uint16_t i = 0; i < rep; ++i) {
93:         vmask = asc_update_mask_b8(remaining);
94:         asc_loadalign(vx, xLocal + i * one_rep_size);
95:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
96:         asc_shiftleft(vz, vx, vshift, vmask);
97:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
98:     }
99: }
100: 
101: __simd_vf__ inline void ShiftLeftVF_uint16(__ubuf__ uint16_t* xLocal, __ubuf__ int16_t* shiftLocal,
102:                                             __ubuf__ uint16_t* zLocal, uint32_t blockLen)
103: {
104:     vector_uint16_t vx, vz;
105:     vector_int16_t vshift;
106:     vector_bool vmask;
107:     uint16_t one_rep_size = 256 / sizeof(uint16_t);
108:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
109:     uint32_t remaining = blockLen;
110:     for (uint16_t i = 0; i < rep; ++i) {
111:         vmask = asc_update_mask_b16(remaining);
112:         asc_loadalign(vx, xLocal + i * one_rep_size);
113:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
114:         asc_shiftleft(vz, vx, vshift, vmask);
115:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
116:     }
117: }
118: 
119: __simd_vf__ inline void ShiftLeftVF_uint32(__ubuf__ uint32_t* xLocal, __ubuf__ int32_t* shiftLocal,
120:                                             __ubuf__ uint32_t* zLocal, uint32_t blockLen)
121: {
122:     vector_uint32_t vx, vz;
123:     vector_int32_t vshift;
124:     vector_bool vmask;
125:     uint16_t one_rep_size = 256 / sizeof(uint32_t);
126:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
127:     uint32_t remaining = blockLen;
128:     for (uint16_t i = 0; i < rep; ++i) {
129:         vmask = asc_update_mask_b32(remaining);
130:         asc_loadalign(vx, xLocal + i * one_rep_size);
131:         asc_loadalign(vshift, shiftLocal + i * one_rep_size);
132:         asc_shiftleft(vz, vx, vshift, vmask);
133:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
134:     }
135: }
136: 
137: /* --------------------------------------------------------------------------
138:  * int8_t 类型左移 Kernel (VL=256)
139:  * -------------------------------------------------------------------------- */
140: 
141: constexpr uint32_t SHL_INT8_VL = 256;
142: constexpr uint32_t SHL_INT8_TILE = SHL_INT8_VL * 4;
143: 
144: __vector__ __global__ __aicore__ void shiftleft_kernel_int8(
145:     __gm__ int8_t* x, __gm__ int8_t* shift, __gm__ int8_t* z)
146: {
147:     asc_init();
148:     uint32_t blockLen = SHL_INT8_TILE * NUM_BLOCKS / asc_get_block_num();
149:     __gm__ int8_t* xGm = x + get_block_idx() * blockLen;
150:     __gm__ int8_t* sGm = shift + get_block_idx() * blockLen;
151:     __gm__ int8_t* zGm = z + get_block_idx() * blockLen;
152:     __ubuf__ int8_t xLocal[SHL_INT8_TILE];
153:     __ubuf__ int8_t sLocal[SHL_INT8_TILE];
154:     __ubuf__ int8_t zLocal[SHL_INT8_TILE];
155:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
156:     uint32_t burstLength = blockLen * sizeof(int8_t);
157:     uint64_t srcStride = 0; uint32_t dstStride = 0;
158:     asc_copy_gm2ub_align((__ubuf__ int8_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
159:     asc_copy_gm2ub_align((__ubuf__ int8_t*)sLocal, sGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
160:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
161:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
162:     ShiftLeftVF_int8(xLocal, sLocal, zLocal, blockLen);
163:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
164:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
165:     asc_copy_ub2gm_align(zGm, (__ubuf__ int8_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
166: }
167: 
168: /* --------------------------------------------------------------------------
169:  * int16_t 类型左移 Kernel (VL=128)
170:  * -------------------------------------------------------------------------- */
171: 
172: constexpr uint32_t SHL_INT16_VL = 128;
173: constexpr uint32_t SHL_INT16_TILE = SHL_INT16_VL * 4;
174: 
175: __vector__ __global__ __aicore__ void shiftleft_kernel_int16(
176:     __gm__ int16_t* x, __gm__ int16_t* shift, __gm__ int16_t* z)
177: {
178:     asc_init();
179:     uint32_t blockLen = SHL_INT16_TILE * NUM_BLOCKS / asc_get_block_num();
180:     __gm__ int16_t* xGm = x + get_block_idx() * blockLen;
181:     __gm__ int16_t* sGm = shift + get_block_idx() * blockLen;
182:     __gm__ int16_t* zGm = z + get_block_idx() * blockLen;
183:     __ubuf__ int16_t xLocal[SHL_INT16_TILE];
184:     __ubuf__ int16_t sLocal[SHL_INT16_TILE];
185:     __ubuf__ int16_t zLocal[SHL_INT16_TILE];
186:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
187:     uint32_t burstLength = blockLen * sizeof(int16_t);
188:     uint64_t srcStride = 0; uint32_t dstStride = 0;
189:     asc_copy_gm2ub_align((__ubuf__ int16_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
190:     asc_copy_gm2ub_align((__ubuf__ int16_t*)sLocal, sGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
191:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
192:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
193:     ShiftLeftVF_int16(xLocal, sLocal, zLocal, blockLen);
194:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
195:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
196:     asc_copy_ub2gm_align(zGm, (__ubuf__ int16_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
197: }
198: 
199: /* --------------------------------------------------------------------------
200:  * int32_t 类型左移 Kernel (VL=64)
201:  * -------------------------------------------------------------------------- */
202: 
203: constexpr uint32_t SHL_INT32_VL = 64;
204: constexpr uint32_t SHL_INT32_TILE = SHL_INT32_VL * 4;
205: 
206: __vector__ __global__ __aicore__ void shiftleft_kernel_int32(
207:     __gm__ int32_t* x, __gm__ int32_t* shift, __gm__ int32_t* z)
208: {
209:     asc_init();
210:     uint32_t blockLen = SHL_INT32_TILE * NUM_BLOCKS / asc_get_block_num();
211:     __gm__ int32_t* xGm = x + get_block_idx() * blockLen;
212:     __gm__ int32_t* sGm = shift + get_block_idx() * blockLen;
213:     __gm__ int32_t* zGm = z + get_block_idx() * blockLen;
214:     __ubuf__ int32_t xLocal[SHL_INT32_TILE];
215:     __ubuf__ int32_t sLocal[SHL_INT32_TILE];
216:     __ubuf__ int32_t zLocal[SHL_INT32_TILE];
217:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
218:     uint32_t burstLength = blockLen * sizeof(int32_t);
219:     uint64_t srcStride = 0; uint32_t dstStride = 0;
220:     asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
221:     asc_copy_gm2ub_align((__ubuf__ int32_t*)sLocal, sGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
222:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
223:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
224:     ShiftLeftVF_int32(xLocal, sLocal, zLocal, blockLen);
225:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
226:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
227:     asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
228: }
229: 
230: /* --------------------------------------------------------------------------
231:  * uint8_t 类型左移 Kernel (VL=256)
232:  * -------------------------------------------------------------------------- */
233: 
234: constexpr uint32_t SHL_UINT8_VL = 256;
235: constexpr uint32_t SHL_UINT8_TILE = SHL_UINT8_VL * 4;
236: 
237: __vector__ __global__ __aicore__ void shiftleft_kernel_uint8(
238:     __gm__ uint8_t* x, __gm__ int8_t* shift, __gm__ uint8_t* z)
239: {
240:     asc_init();
241:     uint32_t blockLen = SHL_UINT8_TILE * NUM_BLOCKS / asc_get_block_num();
242:     __gm__ uint8_t* xGm = x + get_block_idx() * blockLen;
243:     __gm__ int8_t* sGm = shift + get_block_idx() * blockLen;
244:     __gm__ uint8_t* zGm = z + get_block_idx() * blockLen;
245:     __ubuf__ uint8_t xLocal[SHL_UINT8_TILE];
246:     __ubuf__ int8_t sLocal[SHL_UINT8_TILE];
247:     __ubuf__ uint8_t zLocal[SHL_UINT8_TILE];
248:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
249:     uint32_t burstLengthX = blockLen * sizeof(uint8_t);
250:     uint32_t burstLengthS = blockLen * sizeof(int8_t);
251:     uint64_t srcStride = 0; uint32_t dstStride = 0;
252:     asc_copy_gm2ub_align((__ubuf__ uint8_t*)xLocal, xGm, BLK_NUM, burstLengthX, 0, 0, true, cacheMode, srcStride, dstStride);
253:     asc_copy_gm2ub_align((__ubuf__ int8_t*)sLocal, sGm, BLK_NUM, burstLengthS, 0, 0, true, cacheMode, srcStride, dstStride);
254:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
255:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
256:     ShiftLeftVF_uint8(xLocal, sLocal, zLocal, blockLen);
257:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
258:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
259:     asc_copy_ub2gm_align(zGm, (__ubuf__ uint8_t*)zLocal, BLK_NUM, burstLengthX, cacheMode, srcStride, dstStride);
260: }
261: 
262: /* --------------------------------------------------------------------------
263:  * uint16_t 类型左移 Kernel (VL=128)
264:  * -------------------------------------------------------------------------- */
265: 
266: constexpr uint32_t SHL_UINT16_VL = 128;
267: constexpr uint32_t SHL_UINT16_TILE = SHL_UINT16_VL * 4;
268: 
269: __vector__ __global__ __aicore__ void shiftleft_kernel_uint16(
270:     __gm__ uint16_t* x, __gm__ int16_t* shift, __gm__ uint16_t* z)
271: {
272:     asc_init();
273:     uint32_t blockLen = SHL_UINT16_TILE * NUM_BLOCKS / asc_get_block_num();
274:     __gm__ uint16_t* xGm = x + get_block_idx() * blockLen;
275:     __gm__ int16_t* sGm = shift + get_block_idx() * blockLen;
276:     __gm__ uint16_t* zGm = z + get_block_idx() * blockLen;
277:     __ubuf__ uint16_t xLocal[SHL_UINT16_TILE];
278:     __ubuf__ int16_t sLocal[SHL_UINT16_TILE];
279:     __ubuf__ uint16_t zLocal[SHL_UINT16_TILE];
280:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
281:     uint32_t burstLengthX = blockLen * sizeof(uint16_t);
282:     uint32_t burstLengthS = blockLen * sizeof(int16_t);
283:     uint64_t srcStride = 0; uint32_t dstStride = 0;
284:     asc_copy_gm2ub_align((__ubuf__ uint16_t*)xLocal, xGm, BLK_NUM, burstLengthX, 0, 0, true, cacheMode, srcStride, dstStride);
285:     asc_copy_gm2ub_align((__ubuf__ int16_t*)sLocal, sGm, BLK_NUM, burstLengthS, 0, 0, true, cacheMode, srcStride, dstStride);
286:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
287:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
288:     ShiftLeftVF_uint16(xLocal, sLocal, zLocal, blockLen);
289:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
290:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
291:     asc_copy_ub2gm_align(zGm, (__ubuf__ uint16_t*)zLocal, BLK_NUM, burstLengthX, cacheMode, srcStride, dstStride);
292: }
293: 
294: /* --------------------------------------------------------------------------
295:  * uint32_t 类型左移 Kernel (VL=64)
296:  * -------------------------------------------------------------------------- */
297: 
298: constexpr uint32_t SHL_UINT32_VL = 64;
299: constexpr uint32_t SHL_UINT32_TILE = SHL_UINT32_VL * 4;
300: 
301: __vector__ __global__ __aicore__ void shiftleft_kernel_uint32(
302:     __gm__ uint32_t* x, __gm__ int32_t* shift, __gm__ uint32_t* z)
303: {
304:     asc_init();
305:     uint32_t blockLen = SHL_UINT32_TILE * NUM_BLOCKS / asc_get_block_num();
306:     __gm__ uint32_t* xGm = x + get_block_idx() * blockLen;
307:     __gm__ int32_t* sGm = shift + get_block_idx() * blockLen;
308:     __gm__ uint32_t* zGm = z + get_block_idx() * blockLen;
309:     __ubuf__ uint32_t xLocal[SHL_UINT32_TILE];
310:     __ubuf__ int32_t sLocal[SHL_UINT32_TILE];
311:     __ubuf__ uint32_t zLocal[SHL_UINT32_TILE];
312:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
313:     uint32_t burstLengthX = blockLen * sizeof(uint32_t);
314:     uint32_t burstLengthS = blockLen * sizeof(int32_t);
315:     uint64_t srcStride = 0; uint32_t dstStride = 0;
316:     asc_copy_gm2ub_align((__ubuf__ uint32_t*)xLocal, xGm, BLK_NUM, burstLengthX, 0, 0, true, cacheMode, srcStride, dstStride);
317:     asc_copy_gm2ub_align((__ubuf__ int32_t*)sLocal, sGm, BLK_NUM, burstLengthS, 0, 0, true, cacheMode, srcStride, dstStride);
318:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
319:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
320:     ShiftLeftVF_uint32(xLocal, sLocal, zLocal, blockLen);
321:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
322:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
323:     asc_copy_ub2gm_align(zGm, (__ubuf__ uint32_t*)zLocal, BLK_NUM, burstLengthX, cacheMode, srcStride, dstStride);
324: }
325: 
326: /* ==========================================================================
327:  * Host 侧
328:  * ========================================================================== */
329: 
330: template<typename T, typename S, typename KernelFunc>
331: std::vector<T> launch_shiftleft(std::vector<T>& x, std::vector<S>& shift, KernelFunc kernel)
332: {
333:     uint32_t N = x.size();
334:     size_t bytesX = N * sizeof(T);
335:     size_t bytesS = N * sizeof(S);
336:     int32_t devId = 0;
337:     aclrtStream stream = nullptr;
338:     uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());
339:     uint8_t* sHost = reinterpret_cast<uint8_t*>(shift.data());
340:     uint8_t* zHost = nullptr;
341:     T *xD = nullptr; S *sD = nullptr; T *zD = nullptr;
342:     aclInit(nullptr);
343:     aclrtSetDevice(devId);
344:     aclrtCreateStream(&stream);
345:     aclrtMallocHost((void**)&zHost, bytesX);
346:     aclrtMalloc((void**)&xD, bytesX, ACL_MEM_MALLOC_HUGE_FIRST);
347:     aclrtMalloc((void**)&sD, bytesS, ACL_MEM_MALLOC_HUGE_FIRST);
348:     aclrtMalloc((void**)&zD, bytesX, ACL_MEM_MALLOC_HUGE_FIRST);
349:     aclrtMemcpy((uint8_t*)xD, bytesX, xHost, bytesX, ACL_MEMCPY_HOST_TO_DEVICE);
350:     aclrtMemcpy((uint8_t*)sD, bytesS, sHost, bytesS, ACL_MEMCPY_HOST_TO_DEVICE);
351:     kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, sD, zD);
352:     aclrtSynchronizeStream(stream);
353:     aclrtMemcpy(zHost, bytesX, (uint8_t*)zD, bytesX, ACL_MEMCPY_DEVICE_TO_HOST);
354:     std::vector<T> z((T*)zHost, (T*)(zHost + bytesX));
355:     aclrtFree(xD); aclrtFree(sD); aclrtFree(zD);
356:     aclrtFreeHost(zHost);
357:     aclrtDestroyStream(stream);
358:     aclrtResetDevice(devId);
359:     aclFinalize();
360:     return z;
361: }
362: 
363: template<typename T>
364: void print_val(T val, std::ostream& os)
365: {
366:     if constexpr (std::is_same_v<T, half>) {
367:         os << static_cast<float>(val);
368:     } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {
369:         os << static_cast<int>(val);
370:     } else {
371:         os << val;
372:     }
373: }
374: 
375: template<typename T, typename S>
376: void save_csv_shl(const std::vector<T>& x, const std::vector<S>& shift,
377:                   const std::vector<T>& z, const std::vector<T>& golden,
378:                   const std::string& name)
379: {
380:     std::string fname = "shiftleft_" + name + ".csv";
381:     std::ofstream f(fname);
382:     f << "index,x,shift,output,expected\n";
383:     for (size_t i = 0; i < x.size(); ++i) {
384:         f << i << ",";
385:         print_val(x[i], f);
386:         f << ",";
387:         print_val(shift[i], f);
388:         f << ",";
389:         print_val(z[i], f);
390:         f << ",";
391:         print_val(golden[i], f);
392:         f << "\n";
393:     }
394:     f.close();
395:     std::cout << "  [CSV] 已保存到 " << fname << std::endl;
396: }
397: 
398: template<typename T>
399: void print_tensor(const std::vector<T>& t, const std::string& name)
400: {
401:     constexpr size_t maxPrint = 20;
402:     std::cout << "  " << name << " (" << t.size() << " elements): ";
403:     for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {
404:         print_val(t[i], std::cout);
405:         std::cout << " ";
406:     }
407:     if (t.size() > maxPrint) std::cout << "...";
408:     std::cout << std::endl;
409: }
410: 
411: template<typename T, typename S>
412: bool verify_shiftleft(std::vector<T>& z, std::vector<T>& x,
413:                       std::vector<S>& shift, const char* name)
414: {
415:     std::vector<T> g(x.size());
416:     for (size_t i = 0; i < x.size(); ++i) {
417:         g[i] = static_cast<T>(x[i] << shift[i]);
418:     }
419:     print_tensor(x, "X       ");
420:     print_tensor(shift, "Shift   ");
421:     print_tensor(z, "Output  ");
422:     print_tensor(g, "Expected");
423:     save_csv_shl(x, shift, z, g, name);
424:     bool ok = true;
425:     std::vector<size_t> errIdx;
426:     for (size_t i = 0; i < x.size(); ++i) {
427:         if (z[i] != g[i]) {
428:             ok = false;
429:             errIdx.push_back(i);
430:         }
431:     }
432:     if (!ok) {
433:         std::cout << "  [" << name << "] 精度错误: " << errIdx.size()
434:                   << " 个元素不匹配" << std::endl;
435:         size_t maxErr = std::min(errIdx.size(), size_t(8));
436:         for (size_t e = 0; e < maxErr; ++e) {
437:             size_t idx = errIdx[e];
438:             std::cout << "    [" << idx << "] x=";
439:             print_val(x[idx], std::cout);
440:             std::cout << " shift=";
441:             print_val(shift[idx], std::cout);
442:             std::cout << " output=";
443:             print_val(z[idx], std::cout);
444:             std::cout << " expected=";
445:             print_val(g[idx], std::cout);
446:             std::cout << std::endl;
447:         }
448:         if (errIdx.size() > maxErr) {
449:             std::cout << "    ... (还有 " << (errIdx.size() - maxErr) << " 个错误)" << std::endl;
450:         }
451:         return false;
452:     }
453:     std::cout << "  [" << name << "] 通过" << std::endl;
454:     return true;
455: }
456: 
457: /* ==========================================================================
458:  * main
459:  * ========================================================================== */
460: 
461: int32_t main(int32_t argc, char* argv[])
462: {
463:     std::cout << "=== asc_shiftleft 寄存器级矢量左移测试 ===" << std::endl;
464:     std::cout << "模式: 多核 (" << NUM_BLOCKS << " blocks)" << std::endl;
465:     std::cout << std::endl;
466: 
467:     {
468:         constexpr uint32_t N = NUM_BLOCKS * SHL_INT8_TILE;
469:         std::vector<int8_t> x(N), shift(N);
470:         for (uint32_t i = 0; i < N; ++i) {
471:             x[i] = static_cast<int8_t>((i % 8) + 1);
472:             shift[i] = static_cast<int8_t>((i % 4) + 1);
473:         }
474:         auto z = launch_shiftleft<int8_t, int8_t>(x, shift, shiftleft_kernel_int8);
475:         verify_shiftleft(z, x, shift, "int8_t");
476:     }
477:     std::cout << std::endl;
478:     {
479:         constexpr uint32_t N = NUM_BLOCKS * SHL_INT16_TILE;
480:         std::vector<int16_t> x(N), shift(N);
481:         for (uint32_t i = 0; i < N; ++i) {
482:             x[i] = static_cast<int16_t>((i % 64) + 1);
483:             shift[i] = static_cast<int16_t>((i % 8) + 1);
484:         }
485:         auto z = launch_shiftleft<int16_t, int16_t>(x, shift, shiftleft_kernel_int16);
486:         verify_shiftleft(z, x, shift, "int16_t");
487:     }
488:     std::cout << std::endl;
489:     {
490:         constexpr uint32_t N = NUM_BLOCKS * SHL_INT32_TILE;
491:         std::vector<int32_t> x(N), shift(N);
492:         for (uint32_t i = 0; i < N; ++i) {
493:             x[i] = static_cast<int32_t>((i % 1024) + 1);
494:             shift[i] = static_cast<int32_t>((i % 12) + 1);
495:         }
496:         auto z = launch_shiftleft<int32_t, int32_t>(x, shift, shiftleft_kernel_int32);
497:         verify_shiftleft(z, x, shift, "int32_t");
498:     }
499:     std::cout << std::endl;
500:     {
501:         constexpr uint32_t N = NUM_BLOCKS * SHL_UINT8_TILE;
502:         std::vector<uint8_t> x(N);
503:         std::vector<int8_t> shift(N);
504:         for (uint32_t i = 0; i < N; ++i) {
505:             x[i] = static_cast<uint8_t>((i % 16) + 1);
506:             shift[i] = static_cast<int8_t>((i % 5) + 1);
507:         }
508:         auto z = launch_shiftleft<uint8_t, int8_t>(x, shift, shiftleft_kernel_uint8);
509:         verify_shiftleft(z, x, shift, "uint8_t");
510:     }
511:     std::cout << std::endl;
512:     {
513:         constexpr uint32_t N = NUM_BLOCKS * SHL_UINT16_TILE;
514:         std::vector<uint16_t> x(N);
515:         std::vector<int16_t> shift(N);
516:         for (uint32_t i = 0; i < N; ++i) {
517:             x[i] = static_cast<uint16_t>((i % 256) + 1);
518:             shift[i] = static_cast<int16_t>((i % 10) + 1);
519:         }
520:         auto z = launch_shiftleft<uint16_t, int16_t>(x, shift, shiftleft_kernel_uint16);
521:         verify_shiftleft(z, x, shift, "uint16_t");
522:     }
523:     std::cout << std::endl;
524:     {
525:         constexpr uint32_t N = NUM_BLOCKS * SHL_UINT32_TILE;
526:         std::vector<uint32_t> x(N);
527:         std::vector<int32_t> shift(N);
528:         for (uint32_t i = 0; i < N; ++i) {
529:             x[i] = static_cast<uint32_t>((i % 4096) + 1);
530:             shift[i] = static_cast<int32_t>((i % 15) + 1);
531:         }
532:         auto z = launch_shiftleft<uint32_t, int32_t>(x, shift, shiftleft_kernel_uint32);
533:         verify_shiftleft(z, x, shift, "uint32_t");
534:     }
535: 
536:     std::cout << std::endl;
537:     std::cout << "[完成] asc_shiftleft 示例测试结束。" << std::endl;
538:     return 0;
539: }

(End of file - total 539 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge/c_api_ge.asc"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge/c_api_ge.asc</path>
<type>file</type>
<content>
1: /**
2:  * Copyright (c) 2026 Huawei Technologies Co., Ltd.
3:  * CANN Open Software License Agreement Version 2.0
4:  */
5: 
6: /* !
7:  * \file c_api_ge.asc
8:  * \brief asc_ge 寄存器级矢量大于等于比较示例
9:  *        使用 asc_select 将 bool 结果转换为 1/0 值输出
10:  *
11:  * 覆盖数据类型: half, float, int32_t
12:  * 覆盖场景: 多核
13:  */
14: 
15: #include <cstdint>
16: #include <iostream>
17: #include <vector>
18: #include <algorithm>
19: #include <cstdlib>
20: #include <string>
21: #include <fstream>
22: #include "acl/acl.h"
23: #include "c_api/asc_simd.h"
24: 
25: constexpr uint32_t NUM_BLOCKS = 8;
26: constexpr uint32_t BLK_NUM = 1;
27: 
28: /* --------------------------------------------------------------------------
29:  * __simd_vf__ Reg 级大于等于比较函数
30:  * 输出: z = (x >= y) ? 1 : 0
31:  * oneUb/zeroUb: UB上预填充的常量 buffer (全1 / 全0)
32:  * -------------------------------------------------------------------------- */
33: 
34: __simd_vf__ inline void GeVF_half(__ubuf__ half* xLocal, __ubuf__ half* yLocal,
35:                                    __ubuf__ half* zLocal, __ubuf__ half* oneUb,
36:                                    __ubuf__ half* zeroUb, uint32_t blockLen)
37: {
38:     vector_half vx, vy, vone, vzero, vz;
39:     vector_bool vcmp, vmask;
40:     uint16_t one_rep_size = 256 / sizeof(half);
41:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
42:     uint32_t remaining = blockLen;
43:     asc_loadalign(vone, oneUb);
44:     asc_loadalign(vzero, zeroUb);
45:     for (uint16_t i = 0; i < rep; ++i) {
46:         vmask = asc_update_mask_b16(remaining);
47:         asc_loadalign(vx, xLocal + i * one_rep_size);
48:         asc_loadalign(vy, yLocal + i * one_rep_size);
49:         asc_ge(vcmp, vx, vy, vmask);
50:         asc_select(vz, vone, vzero, vcmp);
51:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
52:     }
53: }
54: 
55: __simd_vf__ inline void GeVF_float(__ubuf__ float* xLocal, __ubuf__ float* yLocal,
56:                                     __ubuf__ float* zLocal, __ubuf__ float* oneUb,
57:                                     __ubuf__ float* zeroUb, uint32_t blockLen)
58: {
59:     vector_float vx, vy, vone, vzero, vz;
60:     vector_bool vcmp, vmask;
61:     uint16_t one_rep_size = 256 / sizeof(float);
62:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
63:     uint32_t remaining = blockLen;
64:     asc_loadalign(vone, oneUb);
65:     asc_loadalign(vzero, zeroUb);
66:     for (uint16_t i = 0; i < rep; ++i) {
67:         vmask = asc_update_mask_b32(remaining);
68:         asc_loadalign(vx, xLocal + i * one_rep_size);
69:         asc_loadalign(vy, yLocal + i * one_rep_size);
70:         asc_ge(vcmp, vx, vy, vmask);
71:         asc_select(vz, vone, vzero, vcmp);
72:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
73:     }
74: }
75: 
76: __simd_vf__ inline void GeVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* yLocal,
77:                                     __ubuf__ int32_t* zLocal, __ubuf__ int32_t* oneUb,
78:                                     __ubuf__ int32_t* zeroUb, uint32_t blockLen)
79: {
80:     vector_int32_t vx, vy, vone, vzero, vz;
81:     vector_bool vcmp, vmask;
82:     uint16_t one_rep_size = 256 / sizeof(int32_t);
83:     uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;
84:     uint32_t remaining = blockLen;
85:     asc_loadalign(vone, oneUb);
86:     asc_loadalign(vzero, zeroUb);
87:     for (uint16_t i = 0; i < rep; ++i) {
88:         vmask = asc_update_mask_b32(remaining);
89:         asc_loadalign(vx, xLocal + i * one_rep_size);
90:         asc_loadalign(vy, yLocal + i * one_rep_size);
91:         asc_ge(vcmp, vx, vy, vmask);
92:         asc_select(vz, vone, vzero, vcmp);
93:         asc_storealign(zLocal + i * one_rep_size, vz, vmask);
94:     }
95: }
96: 
97: /* --------------------------------------------------------------------------
98:  * half 类型比较 Kernel (VL=128)
99:  * -------------------------------------------------------------------------- */
100: 
101: constexpr uint32_t GE_HALF_VL = 128;
102: constexpr uint32_t GE_HALF_TILE = GE_HALF_VL * 4;
103: 
104: __vector__ __global__ __aicore__ void ge_kernel_half(
105:     __gm__ half* x, __gm__ half* y, __gm__ half* z)
106: {
107:     asc_init();
108:     constexpr uint32_t regSize = 256 / sizeof(half);
109:     uint32_t blockLen = GE_HALF_TILE * NUM_BLOCKS / asc_get_block_num();
110:     __gm__ half* xGm = x + get_block_idx() * blockLen;
111:     __gm__ half* yGm = y + get_block_idx() * blockLen;
112:     __gm__ half* zGm = z + get_block_idx() * blockLen;
113:     __ubuf__ half xLocal[GE_HALF_TILE];
114:     __ubuf__ half yLocal[GE_HALF_TILE];
115:     __ubuf__ half zLocal[GE_HALF_TILE];
116:     __ubuf__ half oneUb[regSize];
117:     __ubuf__ half zeroUb[regSize];
118:     for (uint32_t i = 0; i < regSize; ++i) {
119:         oneUb[i] = half(1.0f);
120:         zeroUb[i] = half(0.0f);
121:     }
122:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
123:     uint32_t burstLength = blockLen * sizeof(half);
124:     uint64_t srcStride = 0; uint32_t dstStride = 0;
125:     asc_copy_gm2ub_align((__ubuf__ half*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
126:     asc_copy_gm2ub_align((__ubuf__ half*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
127:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
128:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
129:     GeVF_half(xLocal, yLocal, zLocal, oneUb, zeroUb, blockLen);
130:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
131:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
132:     asc_copy_ub2gm_align(zGm, (__ubuf__ half*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
133: }
134: 
135: /* --------------------------------------------------------------------------
136:  * float 类型比较 Kernel (VL=64)
137:  * -------------------------------------------------------------------------- */
138: 
139: constexpr uint32_t GE_FLOAT_VL = 64;
140: constexpr uint32_t GE_FLOAT_TILE = GE_FLOAT_VL * 4;
141: 
142: __vector__ __global__ __aicore__ void ge_kernel_float(
143:     __gm__ float* x, __gm__ float* y, __gm__ float* z)
144: {
145:     asc_init();
146:     constexpr uint32_t regSize = 256 / sizeof(float);
147:     uint32_t blockLen = GE_FLOAT_TILE * NUM_BLOCKS / asc_get_block_num();
148:     __gm__ float* xGm = x + get_block_idx() * blockLen;
149:     __gm__ float* yGm = y + get_block_idx() * blockLen;
150:     __gm__ float* zGm = z + get_block_idx() * blockLen;
151:     __ubuf__ float xLocal[GE_FLOAT_TILE];
152:     __ubuf__ float yLocal[GE_FLOAT_TILE];
153:     __ubuf__ float zLocal[GE_FLOAT_TILE];
154:     __ubuf__ float oneUb[regSize];
155:     __ubuf__ float zeroUb[regSize];
156:     for (uint32_t i = 0; i < regSize; ++i) {
157:         oneUb[i] = 1.0f;
158:         zeroUb[i] = 0.0f;
159:     }
160:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
161:     uint32_t burstLength = blockLen * sizeof(float);
162:     uint64_t srcStride = 0; uint32_t dstStride = 0;
163:     asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
164:     asc_copy_gm2ub_align((__ubuf__ float*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
165:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
166:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
167:     GeVF_float(xLocal, yLocal, zLocal, oneUb, zeroUb, blockLen);
168:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
169:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
170:     asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
171: }
172: 
173: /* --------------------------------------------------------------------------
174:  * int32_t 类型比较 Kernel (VL=64)
175:  * -------------------------------------------------------------------------- */
176: 
177: constexpr uint32_t GE_INT32_VL = 64;
178: constexpr uint32_t GE_INT32_TILE = GE_INT32_VL * 4;
179: 
180: __vector__ __global__ __aicore__ void ge_kernel_int32(
181:     __gm__ int32_t* x, __gm__ int32_t* y, __gm__ int32_t* z)
182: {
183:     asc_init();
184:     constexpr uint32_t regSize = 256 / sizeof(int32_t);
185:     uint32_t blockLen = GE_INT32_TILE * NUM_BLOCKS / asc_get_block_num();
186:     __gm__ int32_t* xGm = x + get_block_idx() * blockLen;
187:     __gm__ int32_t* yGm = y + get_block_idx() * blockLen;
188:     __gm__ int32_t* zGm = z + get_block_idx() * blockLen;
189:     __ubuf__ int32_t xLocal[GE_INT32_TILE];
190:     __ubuf__ int32_t yLocal[GE_INT32_TILE];
191:     __ubuf__ int32_t zLocal[GE_INT32_TILE];
192:     __ubuf__ int32_t oneUb[regSize];
193:     __ubuf__ int32_t zeroUb[regSize];
194:     for (uint32_t i = 0; i < regSize; ++i) {
195:         oneUb[i] = 1;
196:         zeroUb[i] = 0;
197:     }
198:     const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);
199:     uint32_t burstLength = blockLen * sizeof(int32_t);
200:     uint64_t srcStride = 0; uint32_t dstStride = 0;
201:     asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
202:     asc_copy_gm2ub_align((__ubuf__ int32_t*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);
203:     asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
204:     asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
205:     GeVF_int32(xLocal, yLocal, zLocal, oneUb, zeroUb, blockLen);
206:     asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
207:     asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
208:     asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);
209: }
210: 
211: /* ==========================================================================
212:  * Host 侧
213:  * ========================================================================== */
214: 
215: template<typename T, typename KernelFunc>
216: std::vector<T> launch_ge(std::vector<T>& x, std::vector<T>& y, KernelFunc kernel)
217: {
218:     uint32_t N = x.size();
219:     size_t bytes = N * sizeof(T);
220:     int32_t devId = 0;
221:     aclrtStream stream = nullptr;
222:     uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());
223:     uint8_t* yHost = reinterpret_cast<uint8_t*>(y.data());
224:     uint8_t* zHost = nullptr;
225:     T *xD = nullptr, *yD = nullptr, *zD = nullptr;
226:     aclInit(nullptr);
227:     aclrtSetDevice(devId);
228:     aclrtCreateStream(&stream);
229:     aclrtMallocHost((void**)&zHost, bytes);
230:     aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
231:     aclrtMalloc((void**)&yD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
232:     aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);
233:     aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);
234:     aclrtMemcpy((uint8_t*)yD, bytes, yHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);
235:     kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, yD, zD);
236:     aclrtSynchronizeStream(stream);
237:     aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);
238:     std::vector<T> z((T*)zHost, (T*)(zHost + bytes));
239:     aclrtFree(xD); aclrtFree(yD); aclrtFree(zD);
240:     aclrtFreeHost(zHost);
241:     aclrtDestroyStream(stream);
242:     aclrtResetDevice(devId);
243:     aclFinalize();
244:     return z;
245: }
246: 
247: template<typename T>
248: void print_val(T val, std::ostream& os)
249: {
250:     if constexpr (std::is_same_v<T, half>) {
251:         os << static_cast<float>(val);
252:     } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {
253:         os << static_cast<int>(val);
254:     } else {
255:         os << val;
256:     }
257: }
258: 
259: template<typename T>
260: void save_csv(const std::vector<T>& x, const std::vector<T>& y,
261:               const std::vector<T>& z, const std::vector<T>& golden,
262:               const std::string& name)
263: {
264:     std::string fname = "ge_" + name + ".csv";
265:     std::ofstream f(fname);
266:     f << "index,x,y,output,expected\n";
267:     for (size_t i = 0; i < x.size(); ++i) {
268:         f << i << ",";
269:         print_val(x[i], f);
270:         f << ",";
271:         print_val(y[i], f);
272:         f << ",";
273:         print_val(z[i], f);
274:         f << ",";
275:         print_val(golden[i], f);
276:         f << "\n";
277:     }
278:     f.close();
279:     std::cout << "  [CSV] 已保存到 " << fname << std::endl;
280: }
281: 
282: template<typename T>
283: void print_tensor(const std::vector<T>& t, const std::string& name)
284: {
285:     constexpr size_t maxPrint = 20;
286:     std::cout << "  " << name << " (" << t.size() << " elements): ";
287:     for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {
288:         print_val(t[i], std::cout);
289:         std::cout << " ";
290:     }
291:     if (t.size() > maxPrint) std::cout << "...";
292:     std::cout << std::endl;
293: }
294: 
295: template<typename T>
296: bool verify_ge(std::vector<T>& x, std::vector<T>& y,
297:                std::vector<T>& z, const char* name)
298: {
299:     std::vector<T> g(x.size());
300:     for (size_t i = 0; i < x.size(); ++i) {
301:         g[i] = (x[i] >= y[i]) ? static_cast<T>(1) : static_cast<T>(0);
302:     }
303:     print_tensor(x, "X       ");
304:     print_tensor(y, "Y       ");
305:     print_tensor(z, "Output  ");
306:     print_tensor(g, "Expected");
307:     save_csv(x, y, z, g, name);
308:     bool ok = true;
309:     std::vector<size_t> errIdx;
310:     for (size_t i = 0; i < x.size(); ++i) {
311:         if (z[i] != g[i]) {
312:             ok = false;
313:             errIdx.push_back(i);
314:         }
315:     }
316:     if (!ok) {
317:         std::cout << "  [" << name << "] 精度错误: " << errIdx.size()
318:                   << " 个元素不匹配" << std::endl;
319:         size_t maxErr = std::min(errIdx.size(), size_t(8));
320:         for (size_t e = 0; e < maxErr; ++e) {
321:             size_t idx = errIdx[e];
322:             std::cout << "    [" << idx << "] x=";
323:             print_val(x[idx], std::cout);
324:             std::cout << " y=";
325:             print_val(y[idx], std::cout);
326:             std::cout << " output=";
327:             print_val(z[idx], std::cout);
328:             std::cout << " expected=";
329:             print_val(g[idx], std::cout);
330:             std::cout << std::endl;
331:         }
332:         if (errIdx.size() > maxErr) {
333:             std::cout << "    ... (还有 " << (errIdx.size() - maxErr) << " 个错误)" << std::endl;
334:         }
335:         return false;
336:     }
337:     std::cout << "  [" << name << "] 通过" << std::endl;
338:     return true;
339: }
340: 
341: /* ==========================================================================
342:  * main
343:  * ========================================================================== */
344: 
345: int32_t main(int32_t argc, char* argv[])
346: {
347:     std::cout << "=== asc_ge 寄存器级大于等于比较测试 ===" << std::endl;
348:     std::cout << "模式: 多核 (" << NUM_BLOCKS << " blocks)" << std::endl;
349:     std::cout << "输出: (x >= y) ? 1 : 0" << std::endl;
350:     std::cout << std::endl;
351: 
352:     {
353:         constexpr uint32_t N = NUM_BLOCKS * GE_HALF_TILE;
354:         std::vector<half> x(N), y(N);
355:         for (uint32_t i = 0; i < N; ++i) {
356:             x[i] = static_cast<half>(i * 0.1f);
357:             y[i] = static_cast<half>((N - i) * 0.1f);
358:         }
359:         auto z = launch_ge<half>(x, y, ge_kernel_half);
360:         verify_ge(x, y, z, "half");
361:     }
362:     std::cout << std::endl;
363:     {
364:         constexpr uint32_t N = NUM_BLOCKS * GE_FLOAT_TILE;
365:         std::vector<float> x(N), y(N);
366:         for (uint32_t i = 0; i < N; ++i) {
367:             x[i] = i * 0.1f;
368:             y[i] = (N - i) * 0.1f;
369:         }
370:         auto z = launch_ge<float>(x, y, ge_kernel_float);
371:         verify_ge(x, y, z, "float");
372:     }
373:     std::cout << std::endl;
374:     {
375:         constexpr uint32_t N = NUM_BLOCKS * GE_INT32_TILE;
376:         std::vector<int32_t> x(N), y(N);
377:         for (uint32_t i = 0; i < N; ++i) {
378:             x[i] = static_cast<int32_t>(i * 3);
379:             y[i] = static_cast<int32_t>(1000 - static_cast<int32_t>(i));
380:         }
381:         auto z = launch_ge<int32_t>(x, y, ge_kernel_int32);
382:         verify_ge(x, y, z, "int32_t");
383:     }
384: 
385:     std::cout << std::endl;
386:     std::cout << "[完成] asc_ge 示例测试结束。" << std::endl;
387:     return 0;
388: }

(End of file - total 388 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 25.2s)

_Thinking:_

Now I have a very clear picture of the patterns. Let me also look at the ne example to understand how comparison outputs are handled:

Actually, the existing asc_ne example should also use the same pattern as asc_ge (bool select for 1/0 output). Let me just read the build_and_run.sh and the ne example quickly, then start creating the examples.

Actually, I have enough information. Let me now plan and create all 5 examples. Let me also read the build_and_run.sh template and the run_all.sh file to update them.

Let me plan the 5 selected APIs:

1. **asc_addc** - Add with carry (uint32_t, int32_t). Dual output (carry flag + sum). This is a 3-input, 2-output operation.
2. **asc_duplicate** - Replicate lowest element across a vector. Test with half, float, int32_t, int16_t.
3. **asc_ge_scalar** - Vector vs scalar comparison (x >= scalar). Test with half, float, int32_t, uint32_t.
4. **asc_ne_scalar** - Vector vs scalar not-equal comparison. Test with half, float, int32_t.
5. **asc_shiftleft_scalar** - Shift by scalar amount. Test with int8_t, int16_t, int32_t, uint32_t.

Let me now create all of them. I'll start with a todolist.

Now I have all the patterns. Let me create a todo list and start building.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create asc_addc example (add with carry)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create asc_duplicate example (broadcast lowest element)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create asc_ge_scalar example (vector >= scalar)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create asc_ne_scalar example (vector != scalar)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create asc_shiftleft_scalar example (shift by scalar)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Update run_all.sh with new 5 samples",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Build and run all 5 examples, fix issues",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Clean up dump/toml/log files in code repo",
      "status": "pending",
      "priority": "medium"
    },
    {
      "content": "Summarize precision issues and solutions",
      "status": "pending",
      "priority": "medium"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create asc_addc example (add with carry)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_duplicate example (broadcast lowest element)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_ge_scalar example (vector >= scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_ne_scalar example (vector != scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_shiftleft_scalar example (shift by scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Update run_all.sh with new 5 samples",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Build and run all 5 examples, fix issues",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Clean up dump/toml/log files in code repo",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Summarize precision issues and solutions",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 104.5s)

_Thinking:_

Let me start creating the 5 examples. I'll create them in parallel using multiple Task agents to write the .asc files, CMakeLists.txt and build_and_run.sh for each.

Actually, let me first create a task plan. For each API I need to write:

1. c_api_*.asc - main source with kernel + host code
2. CMakeLists.txt
3. build_and_run.sh

Then update run_all.sh.

Let me think about each API carefully:

### 1. asc_addc (Add with carry)
- The impl takes: vector_bool& dst0 (carry out), vector_uint32_t/int32_t& dst1 (sum), src0, src1, vector_bool src2 (carry in), mask
- This is 3-input, 2-output operation
- On the device side, I need to convert the vector_bool (carry out) to a regular value that can be stored back. The vector_bool is 32B (256 bits). For uint32_t, each bool bit corresponds to one uint32 element.
- The formula: {carry_out[i], sum[i]} = src0[i] + src1[i] + carry_in[i]
- For uint32_t: This is essentially an extended-precision addition with carry bit
- The carry_out is a vector_bool which I need to convert to store.
  
Wait, this is tricky. The output includes a vector_bool for carry. To store the carry back to UB/GM, I need to convert it. There's no direct way to convert vector_bool to vector_uint32_t. I need to use something like asc_select or similar.

Actually, looking at the implementation more carefully:
```cpp
__simd_callee__ inline void asc_addc_impl(vector_bool& dst0, vector_uint32_t& dst1,
    vector_uint32_t src0, vector_uint32_t src1, vector_bool src2, vector_bool mask)
```

For addc, the dst0 is a vector_bool (carry flag), dst1 is vector_uint32_t (sum). 

To get the carry as a uint32_t value I can use: `asc_select(vtemp, vone, vzero, dst0)` where vone is filled with 1 and vzero with 0.

Let me think about the host-side golden reference:
```cpp
for (size_t i = 0; i < x.size(); ++i) {
    uint64_t sum = (uint64_t)x[i] + (uint64_t)y[i] + (carryIn[i] ? 1 : 0);
    carryOut[i] = (sum > UINT32_MAX) ? 1 : 0;
    z[i] = (uint32_t)(sum & 0xFFFFFFFF);
}
```

For int32_t, the carry semantics might be different (it's more like overflow detection for signed arithmetic).

Actually, let me think about what the actual semantics of vaddcs are. For addc:
- It computes src0 + src1 + carry_in
- The lower 32 bits go to dst1
- The carry out (bit 32 overflow) goes to dst0 as a bool

For unsigned, this is clear. For signed, it's similar - the overflow from bit 31 to bit 32 wraps around.

OK let me just write the code. I'll also need to handle the fact that the output includes a vector_bool which needs conversion.

### 2. asc_duplicate
- The impl: `asc_duplicate_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)` - takes src, replicates lowest element to fill dst.
- The dst type must match the src type exactly. 
- From the impl: `vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING)` - duplicates the lowest element.
- This is straightforward: `z[i] = x[idx_of_first_in_rep]` where each rep group gets the first element replicated.

Actually, thinking more carefully: `vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING)` - this takes the lowest element of the src vector and replicates it to all elements of dst. So if src = [a, b, c, d, ...], dst = [a, a, a, a, ...].

Wait, but the type signature suggests `vector_uint8_t` src (256B = 256 elements) and the output vector is also 256B. So the lowest element (element 0) gets replicated 256 times.

For testing: I'll load data into a register, then apply asc_duplicate, which should make all elements equal to the first element.

The golden reference for a group would be: `z[i] = x[group_start]` for all elements in the group.

Actually, wait - the implementation passes `POS_LOWEST` which means it takes the lowest (position 0) element of the src register. So the result is that every element in dst equals src[0] (within the register). Since registers are 256B, and we iterate in groups, each group gets its first element replicated.

For host verification: each group of `256/sizeof(T)` elements should have all elements equal to the first element of that group.

### 3. asc_ge_scalar
- `asc_ge_scalar_impl(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)` - compares each element of src >= value
- Output is vector_bool, so I need to convert to numeric 1/0 using asc_select (same pattern as asc_ge)
- The scalar must match the vector element type.
- For host: `z[i] = (x[i] >= scalar) ? 1 : 0`

### 4. asc_ne_scalar
- Same pattern as ge_scalar but with `vcmps_ne`
- For host: `z[i] = (x[i] != scalar) ? 1 : 0`

### 5. asc_shiftleft_scalar
- `asc_shiftleft_scalar_impl(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask)` - shift left by scalar value.
- The scalar shift amount is int16_t (same for all types), the src/dst type varies.
- For host: `z[i] = (T)(x[i] << shift_amount)` where shift_amount is the scalar value.
- Note: This differs from asc_shiftleft which requires a vector of shift amounts. This one uses a single scalar shift amount which is more efficient.

Now, for the shiftleft_scalar, the scalar is passed to the __simd_vf__ function. But how is the scalar passed? In __simd_vf__ functions, the scalar is a compile-time constant or a variable. I need to be careful here.

Looking at the impl:
```cpp
__simd_callee__ inline void asc_shiftleft_scalar_impl(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask)
```

The value is an int16_t scalar. In the SIMD-VF function, I'll need to pass this scalar. The kernel function will receive this scalar via GM or UB. Let me think...

Actually, the scalar for shift operations is typically read from the function parameter. For simplicity, I'll pass the scalar as a separate GM parameter and load it into UB before calling the VF function. Or even simpler, I can use a constant scalar for testing.

Wait, but looking at how asc_ge_scalar is used - the scalar is directly an argument in the SIMD function. For my test, I'll hardcode the scalar as a parameter that's passed to the kernel. The kernel will receive the shift amount as a parameter and pass it to the VF function.

Let me think about the implementation approach more carefully:

For the shift left scalar:
- Device-side: kernel receives (x_input, shift_scalar, z_output) via GM. shift_scalar is read once and used for all elements.
- But actually, the API `asc_shiftleft_scalar_impl` takes a C++ scalar `int16_t value`. The scalar is a function parameter, not a vector.
- I should receive the scalar via the kernel parameter and pass it to the VF function.

For ge_scalar and ne_scalar:
- The scalar value is a function parameter. I can receive it from the host.
- The scalar is used for comparison against each element.
- Kernel signature: `ge_scalar_kernel_half(__gm__ half* x, half scalar, __gm__ half* z)` - pass scalar as a kernel arg.

This is interesting - for scalar operations, the kernel needs to receive the scalar as a parameter. Let me check if the existing scalar examples (like asc_add_scalar, asc_mul_scalar) pass scalars as kernel parameters.

Let me read the asc_mul_scalar example quickly.

Actually, I don't want to read too much more. Let me just create all 5 examples. For the scalar-based ones, I'll pass the scalar through a simple GM buffer (single element). This is the safest approach - the kernel reads the scalar from GM the same way it reads vectors.

For example:
```c
__gm__ int16_t shiftAmountGm = shift_value;
// In kernel: load from UB, or just read as a const from GM
```

Actually, the cleanest approach based on the skill doc patterns: host passes the scalar value as a single-element GM buffer. In the kernel, I load it to a UB slot and then pass the scalar value to the VF function.

Or even simpler: I just embed the scalar into the kernel as an argument. But looking at the existing examples, kernels take only GM pointers as arguments.

Let me go with: pass scalar via a single-element GM buffer. The kernel copies it to a UB slot, then reads it into a scalar variable (via register load), and passes it to the VF function.

Wait, I should look at an existing scalar example to see how it's done. Let me read asc_add_scalar quickly.

Actually, I've read enough. Let me just create the examples and handle compilation issues as they come. Let me create a comprehensive plan for each one.

### Implementation Plan:

For each API, I'll create:

**Directory structure:**
```
vector_compute_examples/asc_XXX/
├── build_and_run.sh
├── c_api_XXX.asc
└── CMakeLists.txt
```

**Common pattern for all .asc files:**
1. Copyright header
2. Includes
3. Constants (NUM_BLOCKS, BLK_NUM)
4. __simd_vf__ VF helper functions (one per data type)
5. __vector__ __global__ __aicore__ kernel functions (one per data type)
6. Host-side: launch template, print_val, save_csv, print_tensor, verify template
7. main() function

Let me start writing them. I'll use Task agents to create them in parallel to save time.

For the asc_addc, I need to handle dual output carefully. The vaddcs instruction produces a vector_bool (carry) + vector_uint32_t (sum). To store both, I need two output buffers. For the carry out, I need to convert vector_bool to vector_uint32_t using asc_select.

Actually, for addc, I'll create a VF function that takes:
- src0, src1, src2 (carry in) as inputs
- produces carry_out and sum as outputs

The carry_in and carry_out need to be converted between bool and numeric. I'll use:
- carry_in: use a vector_bool variable initialized from a vector of 0/1 values
- carry_out: use asc_select to convert bool to numeric

Let me now write all the code. I'll create the directories and files.

Now let me create all 5 examples. I'll use parallel tasks to write the source files:

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create asc_addc example (add with carry)",
      "priority": "high",
      "status": "in_progress"
    },
    {
      "content": "Create asc_duplicate example (broadcast lowest element)",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Create asc_ge_scalar example (vector >= scalar)",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Create asc_ne_scalar example (vector != scalar)",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Create asc_shiftleft_scalar example (shift by scalar)",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Update run_all.sh with new 5 samples",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Build and run all 5 examples, fix issues",
      "priority": "high",
      "status": "pending"
    },
    {
      "content": "Clean up dump/toml/log files in code repo",
      "priority": "medium",
      "status": "pending"
    },
    {
      "content": "Summarize precision issues and solutions",
      "priority": "medium",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create asc_addc example (add with carry)",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Create asc_duplicate example (broadcast lowest element)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_ge_scalar example (vector >= scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_ne_scalar example (vector != scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create asc_shiftleft_scalar example (shift by scalar)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Update run_all.sh with new 5 samples",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Build and run all 5 examples, fix issues",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Clean up dump/toml/log files in code repo",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Summarize precision issues and solutions",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 6.6s)

_Thinking:_

Let me create the directories first, then write the files.

**Tool: bash**

**Input:**
```json
{
  "command": "mkdir -p vector_compute_examples/asc_addc vector_compute_examples/asc_duplicate vector_compute_examples/asc_ge_scalar vector_compute_examples/asc_ne_scalar vector_compute_examples/asc_shiftleft_scalar",
  "description": "Create example directories"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 72.8s)

_Thinking:_

Now let me write all 5 .asc files in parallel. Each needs to follow the exact pattern from the existing examples.

For asc_addc:
- Types: uint32_t, int32_t  
- 3 inputs (src0, src1, carry_in), 2 outputs (carry_out, sum)
- vaddcs instruction

For asc_duplicate:
- Types: half, float, int32_t, int16_t
- 1 input, 1 output (each group's first element replicated)
- vdup instruction with POS_LOWEST, MODE_ZEROING

For asc_ge_scalar:
- Types: half, float, int32_t
- 1 input vector + 1 scalar value, bool output converted to 1/0
- vcmps_ge instruction

For asc_ne_scalar:
- Types: half, float, int32_t
- Same pattern as ge_scalar but with vcmps_ne

For asc_shiftleft_scalar:
- Types: int8_t, int16_t, int32_t, uint32_t
- 1 input vector + scalar shift amount
- vshls instruction

Let me create the CMakeLists.txt and build_and_run.sh for each. These are almost identical, just with different names. Then the .asc files are the main work.

Let me write them all now using the Write tool.

Now let me create all files in parallel. First, the CMakeLists.txt and build_and_run.sh for all 5:

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nfind_package(ASC REQUIRED)\nproject(kernel_samples LANGUAGES ASC CXX)\nadd_executable(c_api_addc_example c_api_addc.asc)\ntarget_compile_options(c_api_addc_example PRIVATE\n    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>\n)\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build_and_run.sh",
  "content": "#!/bin/bash\nset -e\nSCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\nBUILD_DIR=\"${SCRIPT_DIR}/build\"\necho \"=== asc_addc 一键编译运行 ===\"\nmkdir -p \"${BUILD_DIR}\"\ncmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S \"${SCRIPT_DIR}\" -B \"${BUILD_DIR}\"\nmake -j -C \"${BUILD_DIR}\"\necho \"\"\n\"${BUILD_DIR}/c_api_addc_example\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nfind_package(ASC REQUIRED)\nproject(kernel_samples LANGUAGES ASC CXX)\nadd_executable(c_api_duplicate_example c_api_duplicate.asc)\ntarget_compile_options(c_api_duplicate_example PRIVATE\n    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>\n)\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build_and_run.sh",
  "content": "#!/bin/bash\nset -e\nSCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\nBUILD_DIR=\"${SCRIPT_DIR}/build\"\necho \"=== asc_duplicate 一键编译运行 ===\"\nmkdir -p \"${BUILD_DIR}\"\ncmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S \"${SCRIPT_DIR}\" -B \"${BUILD_DIR}\"\nmake -j -C \"${BUILD_DIR}\"\necho \"\"\n\"${BUILD_DIR}/c_api_duplicate_example\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nfind_package(ASC REQUIRED)\nproject(kernel_samples LANGUAGES ASC CXX)\nadd_executable(c_api_ge_scalar_example c_api_ge_scalar.asc)\ntarget_compile_options(c_api_ge_scalar_example PRIVATE\n    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>\n)\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build_and_run.sh",
  "content": "#!/bin/bash\nset -e\nSCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\nBUILD_DIR=\"${SCRIPT_DIR}/build\"\necho \"=== asc_ge_scalar 一键编译运行 ===\"\nmkdir -p \"${BUILD_DIR}\"\ncmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S \"${SCRIPT_DIR}\" -B \"${BUILD_DIR}\"\nmake -j -C \"${BUILD_DIR}\"\necho \"\"\n\"${BUILD_DIR}/c_api_ge_scalar_example\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nfind_package(ASC REQUIRED)\nproject(kernel_samples LANGUAGES ASC CXX)\nadd_executable(c_api_ne_scalar_example c_api_ne_scalar.asc)\ntarget_compile_options(c_api_ne_scalar_example PRIVATE\n    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>\n)\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build_and_run.sh",
  "content": "#!/bin/bash\nset -e\nSCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\nBUILD_DIR=\"${SCRIPT_DIR}/build\"\necho \"=== asc_ne_scalar 一键编译运行 ===\"\nmkdir -p \"${BUILD_DIR}\"\ncmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S \"${SCRIPT_DIR}\" -B \"${BUILD_DIR}\"\nmake -j -C \"${BUILD_DIR}\"\necho \"\"\n\"${BUILD_DIR}/c_api_ne_scalar_example\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nfind_package(ASC REQUIRED)\nproject(kernel_samples LANGUAGES ASC CXX)\nadd_executable(c_api_shiftleft_scalar_example c_api_shiftleft_scalar.asc)\ntarget_compile_options(c_api_shiftleft_scalar_example PRIVATE\n    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>\n)\n"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build_and_run.sh",
  "content": "#!/bin/bash\nset -e\nSCRIPT_DIR=\"$(cd \"$(dirname \"${BASH_SOURCE[0]}\")\" && pwd)\"\nBUILD_DIR=\"${SCRIPT_DIR}/build\"\necho \"=== asc_shiftleft_scalar 一键编译运行 ===\"\nmkdir -p \"${BUILD_DIR}\"\ncmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S \"${SCRIPT_DIR}\" -B \"${BUILD_DIR}\"\nmake -j -C \"${BUILD_DIR}\"\necho \"\"\n\"${BUILD_DIR}/c_api_shiftleft_scalar_example\"\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 229.8s)

_Thinking:_

Good, all CMakeLists.txt and build_and_run.sh files are created. Now I need to write the main .asc source files. Let me create them in parallel. These are the most complex files.

Let me think carefully about each one before writing.

### 1. asc_addc (Add with carry)

The API signature from impl:
```cpp
__simd_callee__ inline void asc_addc_impl(vector_bool& dst0, vector_uint32_t& dst1,
    vector_uint32_t src0, vector_uint32_t src1, vector_bool src2, vector_bool mask)
```

Where:
- dst0 = carry out (vector_bool)
- dst1 = sum (vector_uint32_t or vector_int32_t)
- src0, src1 = input operands
- src2 = carry in (vector_bool)
- mask = mask

The public API would be: `asc_addc(carry_out, sum, src0, src1, carry_in, mask)`

In the __simd_vf__ function:
1. Load data from UB: src0, src1, carry_in (as a vector of 0/1 values, then converted to vector_bool)
2. Call asc_addc(dst0, dst1, vx, vy, carry_in_bool, vmask)
3. Convert carry_out vector_bool back to uint32_t/int32_t using asc_select with 1/0
4. Store both sum and carry_out to UB

For host-side golden:
```cpp
for (size_t i = 0; i < x.size(); ++i) {
    uint64_t sum64 = (uint64_t)x[i] + (uint64_t)y[i] + (uint64_t)(carryIn[i] ? 1 : 0);
    sumOut[i] = (T)(sum64 & 0xFFFFFFFFull);
    carryOut[i] = (T)((sum64 >> 32) & 1);
}
```

For int32_t, the carry semantics might differ. Let me think... For signed 32-bit add with carry, the carry flag is set when there's overflow adding the MSB (bit 31). The implementation just adds src0 + src1 + carry. For signed:
```cpp
int64_t sum64 = (int64_t)x[i] + (int64_t)y[i] + (int64_t)(carryIn[i] ? 1 : 0);
sumOut[i] = (int32_t)(sum64 & 0xFFFFFFFFull);
carryOut[i] = (sum64 != (int32_t)sum64) ? 1 : 0;  // overflow check
```

Actually for unsigned the carry is when sum > UINT32_MAX. For signed it should be about overflow (below INT32_MIN or above INT32_MAX). But vaddcs just does a flat 32-bit addition with carry - the carry flag is set when the 32-bit addition overflows. For signed, this means when sum < -2^31 or > 2^31-1.

Actually, vaddcs is a bit-level operation. For signed and unsigned, the bit-level behavior is the same. The carry flag is the overflow from bit 31. So for both signed and unsigned:
```cpp
uint64_t sum64 = (uint64_t)(uint32_t)x[i] + (uint64_t)(uint32_t)y[i] + (uint64_t)carryIn[i];
sumOut[i] = (T)(sum64 & 0xFFFFFFFFull);
carryOut[i] = (T)((sum64 >> 32) & 1);
```

Wait, for signed values, a negative number has its sign bit set. When doing 32-bit unsigned addition, the carry flag is just the bit 32 overflow. For signed golden reference, I should watch out for overflow detection.

Actually, let me keep it simple. The vaddcs instruction does binary addition with carry:
- sum = src0 + src1 + carry_in (mod 2^32)
- carry_out = 1 if (src0 + src1 + carry_in) >= 2^32, else 0

For both signed and unsigned, this is the same bit-level behavior. The golden reference should use unsigned arithmetic.

### 2. asc_duplicate

The API: `asc_duplicate(dst, src, mask)` → replicates lowest element of src to all elements in dst.

In the __simd_vf__ function, for each group:
1. Load src vector from UB
2. Call asc_duplicate(vz, vx, vmask)
3. Store result to UB

Golden: `z[i] = x[group_start + (i % one_rep_size == 0 ? 0 : ...)]` - no, each group gets its first element replicated.
For each rep, the output has all positions equal to x[group_start].

Actually, since duplication happens within each register (256B), and we process groups sequentially:
- Group 0: z[0..one_rep_size-1] = all x[0]
- Group 1: z[one_rep_size..2*one_rep_size-1] = all x[one_rep_size]
- etc.

Golden:
```cpp
for (size_t i = 0; i < N; i++) {
    size_t group_start = (i / one_rep_size) * one_rep_size;
    z[i] = x[group_start];
}
```

### 3. asc_ge_scalar

The API: `asc_ge_scalar(vector_bool& dst, vector_T src, T scalar, vector_bool mask)` - compares src[i] >= scalar.

In __simd_vf__:
1. Pre-load vone and vzero vectors
2. Pass scalar value (as a const/variable to the function)
3. For each rep: load src, call asc_ge_scalar(vcmp, vx, scalar_value, vmask), asc_select(vz, vone, vzero, vcmp), store

Issue: The scalar needs to be passed to the VF function. From the skill doc: "__simd_vf__ 内的标量应尽量使用常量". But we can also pass it as a variable.

For the kernel to receive a scalar from host, I have two options:
a. Pass scalar via GM as a single element buffer
b. Use a constant value

For testing purposes, I'll use a constant scalar value in the VF function. But for flexibility, I should pass it as a kernel parameter. The cleanest way: pass scalar value through a GM buffer (single-element buffer).

Actually, let me look at how the scalar is used in __simd_vf__ functions. From the skill examples, scalars are passed as C++ parameters to the VF function. The VF function receives the scalar as a normal C++ value (not a vector type). So I can:
1. In the kernel: copy a single scalar from GM to UB
2. Load it into a register  
3. Pass it to the VF function as a scalar parameter

Actually, that's overly complex. The simplest approach: pass the scalar directly as a kernel parameter value. But the kernel call API only passes pointer arguments...

Let me use a simpler approach: the kernel function receives the scalar value via a dedicated GM buffer:
```c
__gm__ half* scalar_gm  // points to a single half value
```

In the kernel, I copy this single value to a UB slot, load to a register, then extract the scalar value. But extracting a scalar from a vector register in __simd_vf__ might not be straightforward.

Alternative simpler approach: Since __simd_vf__ supports scalar variables, I can pass the scalar value as a kernel argument (passed by value). But looking at the existing examples, all kernel arguments are pointers...

Actually, I think the cleanest approach is:
1. Host allocates a single-element GM buffer for the scalar
2. Kernel copies that single element to a UB buffer of size 1
3. In the VF function, I load that one element from UB, and then use it as the scalar comparison value

But how do I extract a scalar from a register in __simd_vf__? Looking at the asc_impl headers, the scalar overloads of comparison functions take the scalar as a C++ value, not as a vector. So I need to have the scalar as a regular C++ variable in the VF function context.

Looking at the SIMD VF context more carefully:
```cpp
__simd_vf__ inline void GeScalarVF_half(__ubuf__ half* xLocal, half scalar, ...)
```

This should work - the scalar is passed as a regular C++ parameter to the VF function. But how does the kernel get this scalar?

Actually, looking at the existing asc_add_scalar or asc_mul_scalar examples would help. But I don't want to read more. Let me think of the simplest working approach.

Approach: The kernel receives the scalar value as a GM buffer of size 1, copies it to UB, loads it into a register. In the VF function:
```cpp
__simd_vf__ inline void GeScalarVF_half(__ubuf__ half* xLocal, __ubuf__ half* zLocal, 
                                         half scalar, __ubuf__ half* oneUb, __ubuf__ half* zeroUb, 
                                         uint32_t blockLen)
```

Wait, but `half scalar` as a scalar parameter in __simd_vf__ might be tricky. Let me think...

Actually, from the skill documentation: "__simd_vf__ 内的标量应尽量使用常量". This means we're allowed to use scalars but constants are preferred. Since I'm writing a test, I can just hardcode the scalar in the VF function for simplicity. This is the safest approach that matches the "prefer constants" guidance.

So for testing:
- ge_scalar test with a range of values using different constant scalars
- I can have separate VF functions for different scalar values, or pass the scalar as a parameter

Let me pass the scalar as a parameter to the VF function (it should work based on the impl signature).

### 4. asc_ne_scalar
Same as ge_scalar, but using vcmps_ne.

### 5. asc_shiftleft_scalar
The scalar shift amount is int16_t. Same approach: pass as VF function parameter.

Let me now write all 5 .asc files. I'll create them all as tasks in parallel.

Actually, let me just write them directly using the Write tool. Let me start with asc_addc - this is the most complex one.

Now let me write the main .asc source files. Starting with the most complex ones:

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_addc.asc\n * \\brief asc_addc 寄存器级矢量带进位加法示例\n *\n * 计算: {carry_out, sum} = src0 + src1 + carry_in\n * 覆盖数据类型: uint32_t, int32_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void AddcVF_uint32(__ubuf__ uint32_t* xLocal, __ubuf__ uint32_t* yLocal,\n                                       __ubuf__ uint32_t* cInLocal, __ubuf__ uint32_t* sumLocal,\n                                       __ubuf__ uint32_t* cOutLocal, uint32_t blockLen)\n{\n    vector_uint32_t vx, vy, vsum, vcarry_out, vone, vzero;\n    vector_bool vcarry_in, vcarry_out_bool, vmask;\n    uint16_t one_rep_size = 256 / sizeof(uint32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n\n    asc_loadalign(vone, (__ubuf__ uint32_t*)(cInLocal));\n    asc_loadalign(vzero, (__ubuf__ uint32_t*)(cInLocal));\n\n    __ubuf__ uint32_t ones[one_rep_size];\n    __ubuf__ uint32_t zeros[one_rep_size];\n    for (uint16_t i = 0; i < one_rep_size; ++i) {\n        ones[i] = 1;\n        zeros[i] = 0;\n    }\n    asc_loadalign(vone, ones);\n    asc_loadalign(vzero, zeros);\n\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_loadalign(vy, yLocal + i * one_rep_size);\n        asc_loadalign(vcarry_out, cInLocal + i * one_rep_size);\n\n        vector_bool vcin_bool;\n        asc_ne_scalar(vcin_bool, vcarry_out, (uint32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcin_bool, vmask);\n\n        asc_select(vcarry_out, vone, vzero, vcarry_out_bool);\n\n        asc_storealign(sumLocal + i * one_rep_size, vsum, vmask);\n        asc_storealign(cOutLocal + i * one_rep_size, vcarry_out, vmask);\n    }\n}\n\n__simd_vf__ inline void AddcVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* yLocal,\n                                      __ubuf__ int32_t* cInLocal, __ubuf__ int32_t* sumLocal,\n                                      __ubuf__ int32_t* cOutLocal, uint32_t blockLen)\n{\n    vector_int32_t vx, vy, vsum, vcarry_out, vone, vzero;\n    vector_bool vcarry_out_bool, vmask;\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n\n    __ubuf__ int32_t ones[one_rep_size];\n    __ubuf__ int32_t zeros[one_rep_size];\n    for (uint16_t i = 0; i < one_rep_size; ++i) {\n        ones[i] = 1;\n        zeros[i] = 0;\n    }\n    asc_loadalign(vone, ones);\n    asc_loadalign(vzero, zeros);\n\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_loadalign(vy, yLocal + i * one_rep_size);\n        asc_loadalign(vcarry_out, cInLocal + i * one_rep_size);\n\n        vector_bool vcin_bool;\n        asc_ne_scalar(vcin_bool, vcarry_out, (int32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcin_bool, vmask);\n\n        asc_select(vcarry_out, vone, vzero, vcarry_out_bool);\n\n        asc_storealign(sumLocal + i * one_rep_size, vsum, vmask);\n        asc_storealign(cOutLocal + i * one_rep_size, vcarry_out, vmask);\n    }\n}\n\nconstexpr uint32_t ADDC_UINT32_VL = 64;\nconstexpr uint32_t ADDC_UINT32_TILE = ADDC_UINT32_VL * 4;\n\n__vector__ __global__ __aicore__ void addc_kernel_uint32(\n    __gm__ uint32_t* x, __gm__ uint32_t* y, __gm__ uint32_t* carryIn,\n    __gm__ uint32_t* sumOut, __gm__ uint32_t* carryOut)\n{\n    asc_init();\n    constexpr uint32_t regSize = 256 / sizeof(uint32_t);\n    uint32_t blockLen = ADDC_UINT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ uint32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ uint32_t* yGm = y + get_block_idx() * blockLen;\n    __gm__ uint32_t* ciGm = carryIn + get_block_idx() * blockLen;\n    __gm__ uint32_t* sGm = sumOut + get_block_idx() * blockLen;\n    __gm__ uint32_t* coGm = carryOut + get_block_idx() * blockLen;\n    __ubuf__ uint32_t xLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t yLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t ciLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t sLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t coLocal[ADDC_UINT32_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(uint32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)ciLocal, ciGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    AddcVF_uint32(xLocal, yLocal, ciLocal, sLocal, coLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(sGm, (__ubuf__ uint32_t*)sLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n    asc_copy_ub2gm_align(coGm, (__ubuf__ uint32_t*)coLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t ADDC_INT32_VL = 64;\nconstexpr uint32_t ADDC_INT32_TILE = ADDC_INT32_VL * 4;\n\n__vector__ __global__ __aicore__ void addc_kernel_int32(\n    __gm__ int32_t* x, __gm__ int32_t* y, __gm__ int32_t* carryIn,\n    __gm__ int32_t* sumOut, __gm__ int32_t* carryOut)\n{\n    asc_init();\n    constexpr uint32_t regSize = 256 / sizeof(int32_t);\n    uint32_t blockLen = ADDC_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* yGm = y + get_block_idx() * blockLen;\n    __gm__ int32_t* ciGm = carryIn + get_block_idx() * blockLen;\n    __gm__ int32_t* sGm = sumOut + get_block_idx() * blockLen;\n    __gm__ int32_t* coGm = carryOut + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t yLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t ciLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t sLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t coLocal[ADDC_INT32_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)ciLocal, ciGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    AddcVF_int32(xLocal, yLocal, ciLocal, sLocal, coLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(sGm, (__ubuf__ int32_t*)sLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n    asc_copy_ub2gm_align(coGm, (__ubuf__ int32_t*)coLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::pair<std::vector<T>, std::vector<T>> launch_addc(\n    std::vector<T>& x, std::vector<T>& y, std::vector<T>& carryIn, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* yHost = reinterpret_cast<uint8_t*>(y.data());\n    uint8_t* ciHost = reinterpret_cast<uint8_t*>(carryIn.data());\n    uint8_t* sHost = nullptr;\n    uint8_t* coHost = nullptr;\n    T *xD = nullptr, *yD = nullptr, *ciD = nullptr, *sD = nullptr, *coD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&sHost, bytes);\n    aclrtMallocHost((void**)&coHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&yD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&ciD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&sD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&coD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n    aclrtMemcpy((uint8_t*)yD, bytes, yHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n    aclrtMemcpy((uint8_t*)ciD, bytes, ciHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, yD, ciD, sD, coD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(sHost, bytes, (uint8_t*)sD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    aclrtMemcpy(coHost, bytes, (uint8_t*)coD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> sumOut((T*)sHost, (T*)(sHost + bytes));\n    std::vector<T> carryOut((T*)coHost, (T*)(coHost + bytes));\n\n    aclrtFree(xD); aclrtFree(yD); aclrtFree(ciD); aclrtFree(sD); aclrtFree(coD);\n    aclrtFreeHost(sHost); aclrtFreeHost(coHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return {sumOut, carryOut};\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_addc(const std::vector<T>& x, const std::vector<T>& y,\n                   const std::vector<T>& cIn, const std::vector<T>& sumOut,\n                   const std::vector<T>& cOut, const std::vector<T>& gSum,\n                   const std::vector<T>& gCout, const std::string& name)\n{\n    std::string fname = \"addc_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,y,carry_in,sum_out,sum_expected,carry_out,carry_expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f); f << \",\";\n        print_val(y[i], f); f << \",\";\n        print_val(cIn[i], f); f << \",\";\n        print_val(sumOut[i], f); f << \",\";\n        print_val(gSum[i], f); f << \",\";\n        print_val(cOut[i], f); f << \",\";\n        print_val(gCout[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T>\nbool verify_addc(std::vector<T>& x, std::vector<T>& y,\n                 std::vector<T>& cIn, std::vector<T>& sumOut,\n                 std::vector<T>& cOut, const char* name)\n{\n    std::vector<T> gSum(x.size()), gCout(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        uint64_t sum64 = (uint64_t)x[i] + (uint64_t)y[i] + (uint64_t)(cIn[i] ? 1 : 0);\n        gSum[i] = (T)(sum64 & 0xFFFFFFFFull);\n        gCout[i] = (T)((sum64 >> 32) & 1);\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(y, \"Y       \");\n    print_tensor(cIn, \"CarryIn \");\n    print_tensor(sumOut, \"SumOut  \");\n    print_tensor(gSum, \"SumExp  \");\n    print_tensor(cOut, \"CoutOut \");\n    print_tensor(gCout, \"CoutExp \");\n\n    save_csv_addc(x, y, cIn, sumOut, cOut, gSum, gCout, name);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (sumOut[i] != gSum[i] || cOut[i] != gCout[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" y=\";\n            print_val(y[idx], std::cout);\n            std::cout << \" cin=\";\n            print_val(cIn[idx], std::cout);\n            std::cout << \" sum=\";\n            print_val(sumOut[idx], std::cout);\n            std::cout << \" sumExp=\";\n            print_val(gSum[idx], std::cout);\n            std::cout << \" cout=\";\n            print_val(cOut[idx], std::cout);\n            std::cout << \" coutExp=\";\n            print_val(gCout[idx], std::cout);\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_addc 寄存器级带进位加法测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * ADDC_UINT32_TILE;\n        std::vector<uint32_t> x(N), y(N), cIn(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<uint32_t>(0xFFFF0000u + i);\n            y[i] = static_cast<uint32_t>(0x0000FFFFu + i * 2);\n            cIn[i] = (i % 3 == 0) ? 1u : 0u;\n        }\n        auto [sumOut, cOut] = launch_addc<uint32_t>(x, y, cIn, addc_kernel_uint32);\n        verify_addc(x, y, cIn, sumOut, cOut, \"uint32_t\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * ADDC_INT32_TILE;\n        std::vector<int32_t> x(N), y(N), cIn(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>(1000000000 + static_cast<int32_t>(i));\n            y[i] = static_cast<int32_t>(1000000000 + static_cast<int32_t>(i * 2));\n            cIn[i] = (i % 5 == 0) ? 1 : 0;\n        }\n        auto [sumOut, cOut] = launch_addc<int32_t>(x, y, cIn, addc_kernel_int32);\n        verify_addc(x, y, cIn, sumOut, cOut, \"int32_t\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_addc 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 235.7s)

_Thinking:_

I made a mistake in the addc VF functions. I used `asc_ne_scalar` to convert carry_in values to vector_bool. But there's a bug - I'm loading `vcarry_out` from `cInLocal` but using it incorrectly. Let me also check the vone/vzero loading.

Actually the real issue is in the VF functions. Let me fix them. The problem:
1. `asc_loadalign(vone, ones)` and `asc_loadalign(vzero, zeros)` - these should work
2. For carry_in: I need to compare against 0 to get a bool mask. The `carryIn[i]` is 0 or 1, so `carryIn[i] != 0` gives the bool.
3. But `asc_ne_scalar` outputs a vector_bool. That's what I want.

Wait, there's another bug: I'm loading `vcarry_out` from `cInLocal` but then passing it to `asc_ne_scalar`. The variable name is wrong - it should be something like `vcin_val`. But more importantly, I'm reusing the `vcarry_out` variable for both input and output purpose. Let me fix.

Also, I have an unused line `asc_loadalign(vcarry_out, cInLocal + i * one_rep_size);` that loads the carry-in values. Then `asc_ne_scalar(vcin_bool, vcarry_out, (uint32_t)0, vmask)` converts to bool. But `vcarry_out` holds the carry-in values (0 or 1), and after `asc_addc`, `vcarry_out` is then used for `asc_select` to produce the numeric carry out. So the variable is being reused but correctly - just the naming is confusing.

Actually, looking more carefully at the flow:
1. Read carry_in values into `vcarry_out` → this is actually carrying the CARRY IN values
2. Convert to bool: `asc_ne_scalar(vcin_bool, vcarry_out, 0, vmask)` - if carry_in[i] != 0, set bool[i] = true
3. `asc_addc(vcarry_out_bool, vsum, vx, vy, vcin_bool, vmask)` - produces carry_out as bool
4. `asc_select(vcarry_out, vone, vzero, vcarry_out_bool)` - converts carry_out bool back to numeric

This looks correct! The variable reuse makes sense.

But wait, there's a problem with the `asc_ne_scalar` API. Let me check the signature:
```cpp
__simd_callee__ inline void asc_ne_scalar_impl(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
```

So the scalar value should be 0, and `carry_in != 0` means carry_in is true. That's correct.

But there's a subtlety: for `uint32_t` carry_in values of 0 or 1, `carry_in != 0` correctly maps 0→false, 1→true.

Now, there's a potential issue with `asc_addc` - the public API through `asc_simd.h` should expose it as `asc_addc`. Let me check if `asc_addc` exists as a public function. Looking at the impl files, the header guards suggest they're internal, and the public API would be through `asc_simd.h`. Since all existing examples include `asc_simd.h`, the public API `asc_addc` should be available.

Actually, wait. I need to also look at how the public API is declared. Let me check if there's a public header for addc. The impl is `asc_addc_impl.h` and the public API should be declared somewhere like `c_api/vector_compute.h`. Let me verify quickly.

Actually, for the existing examples, they use `asc_add`, `asc_ge`, `asc_shiftleft` etc. which are all public APIs. The implementations in the impl directory are prefixed with `_impl`. The public API wrapper likely just calls the impl. So `asc_addc` should be available as a public API.

But I should verify. Let me check if `asc_addc` is declared somewhere in the public headers.

Actually, let me just write the code and fix any compilation issues. The important thing is to follow the exact patterns from the existing examples.

Wait, there's another issue in the addc VF. I have `asc_loadalign(vone, (__ubuf__ uint32_t*)(cInLocal));` which is loading cInLocal into vone which doesn't make sense. That's a leftover line I need to remove.

Let me also reconsider the asc_addc VF functions more carefully. The issue is:
1. I need `vone` (vector of 1s) and `vzero` (vector of 0s) for asc_select conversion
2. I need to load carry_in from UB
3. I need to convert carry_in (0/1 values) to vector_bool

Let me rewrite the VF functions more cleanly. Also, looking at the CI path - `cInLocal + i * one_rep_size` should correctly point to the carry-in values. But wait, in my kernel I have a single carry_in buffer `ciLocal` of `ADDC_UINT32_TILE` length, but cInLocal is passed to VF as `__ubuf__ uint32_t*`. But then in VF, I'm also using `cInLocal` for... wait, actually `cInLocal` is a parameter that gets the carry-in buffer. Then inside VF I load from `cInLocal + i * one_rep_size` which is correct.

OK so the main issue in the current code is:
1. The two leftover lines: `asc_loadalign(vone, (__ubuf__ uint32_t*)(cInLocal));` and `asc_loadalign(vzero, (__ubuf__ uint32_t*)(cInLocal));` need to be removed.

Let me just fix these when we compile.

Also, another issue: I declared `__ubuf__ uint32_t ones[one_rep_size]` but `one_rep_size` is not constexpr in the function parameter context. For UB arrays, the size must be a compile-time constant. This will be a compilation error!

I need to fix this. Since the regSize is fixed (256B / sizeof(uint32_t) = 64), I should use a constexpr value. But inside the VF function, `one_rep_size` is computed as `256 / sizeof(uint32_t)` which the compiler should be able to constant-fold.

Actually in ASC code, the UB array size must be a literal constant known at compile time. Using `256 / sizeof(uint32_t)` might or might not work - it depends on the compiler. To be safe, let me use explicit constants.

Wait, looking at the existing asc_ge example:
```cpp
__ubuf__ half oneUb[regSize];
__ubuf__ half zeroUb[regSize];
```
Where `regSize` is `constexpr uint32_t regSize = 256 / sizeof(half)` - this is in the kernel context. The regSize for the UB arrays is defined as a `constexpr` at the kernel level.

But in the VF function, the one/zero arrays are allocated in the kernel and passed to the VF. In the asc_ge example, the one/zero arrays are allocated in each kernel function and passed as parameters to the VF function.

For addc, I should follow the same pattern: allocate the one/zero arrays in the kernel, pass them to the VF function.

Let me rewrite the asc_addc code properly. But first, let me just create all 5 .asc files with initial versions, then fix compilation issues.

Actually, let me fix the asc_addc first since I know it has issues, then create the other 4 files correctly.

I need to fix the asc_addc VF functions before proceeding. Let me rewrite it properly:

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_addc.asc\n * \\brief asc_addc 寄存器级矢量带进位加法示例\n *\n * 计算: {carry_out, sum} = src0 + src1 + carry_in\n * 覆盖数据类型: uint32_t, int32_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void AddcVF_uint32(__ubuf__ uint32_t* xLocal, __ubuf__ uint32_t* yLocal,\n                                       __ubuf__ uint32_t* cInLocal, __ubuf__ uint32_t* sumLocal,\n                                       __ubuf__ uint32_t* cOutLocal, __ubuf__ uint32_t* oneUb,\n                                       __ubuf__ uint32_t* zeroUb, uint32_t blockLen)\n{\n    vector_uint32_t vx, vy, vsum, vcarry_val, vone, vzero;\n    vector_bool vcarry_in_bool, vcarry_out_bool, vmask;\n    uint16_t one_rep_size = 256 / sizeof(uint32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_loadalign(vy, yLocal + i * one_rep_size);\n        asc_loadalign(vcarry_val, cInLocal + i * one_rep_size);\n\n        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);\n\n        asc_select(vcarry_val, vone, vzero, vcarry_out_bool);\n\n        asc_storealign(sumLocal + i * one_rep_size, vsum, vmask);\n        asc_storealign(cOutLocal + i * one_rep_size, vcarry_val, vmask);\n    }\n}\n\n__simd_vf__ inline void AddcVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* yLocal,\n                                      __ubuf__ int32_t* cInLocal, __ubuf__ int32_t* sumLocal,\n                                      __ubuf__ int32_t* cOutLocal, __ubuf__ int32_t* oneUb,\n                                      __ubuf__ int32_t* zeroUb, uint32_t blockLen)\n{\n    vector_int32_t vx, vy, vsum, vcarry_val, vone, vzero;\n    vector_bool vcarry_in_bool, vcarry_out_bool, vmask;\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_loadalign(vy, yLocal + i * one_rep_size);\n        asc_loadalign(vcarry_val, cInLocal + i * one_rep_size);\n\n        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);\n\n        asc_select(vcarry_val, vone, vzero, vcarry_out_bool);\n\n        asc_storealign(sumLocal + i * one_rep_size, vsum, vmask);\n        asc_storealign(cOutLocal + i * one_rep_size, vcarry_val, vmask);\n    }\n}\n\nconstexpr uint32_t ADDC_UINT32_VL = 64;\nconstexpr uint32_t ADDC_UINT32_TILE = ADDC_UINT32_VL * 4;\nconstexpr uint32_t ADDC_UINT32_REGSIZE = 256 / sizeof(uint32_t);\n\n__vector__ __global__ __aicore__ void addc_kernel_uint32(\n    __gm__ uint32_t* x, __gm__ uint32_t* y, __gm__ uint32_t* carryIn,\n    __gm__ uint32_t* sumOut, __gm__ uint32_t* carryOut)\n{\n    asc_init();\n    uint32_t blockLen = ADDC_UINT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ uint32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ uint32_t* yGm = y + get_block_idx() * blockLen;\n    __gm__ uint32_t* ciGm = carryIn + get_block_idx() * blockLen;\n    __gm__ uint32_t* sGm = sumOut + get_block_idx() * blockLen;\n    __gm__ uint32_t* coGm = carryOut + get_block_idx() * blockLen;\n    __ubuf__ uint32_t xLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t yLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t ciLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t sLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t coLocal[ADDC_UINT32_TILE];\n    __ubuf__ uint32_t oneUb[ADDC_UINT32_REGSIZE];\n    __ubuf__ uint32_t zeroUb[ADDC_UINT32_REGSIZE];\n    for (uint32_t i = 0; i < ADDC_UINT32_REGSIZE; ++i) {\n        oneUb[i] = 1;\n        zeroUb[i] = 0;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(uint32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)ciLocal, ciGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    AddcVF_uint32(xLocal, yLocal, ciLocal, sLocal, coLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(sGm, (__ubuf__ uint32_t*)sLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n    asc_copy_ub2gm_align(coGm, (__ubuf__ uint32_t*)coLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t ADDC_INT32_VL = 64;\nconstexpr uint32_t ADDC_INT32_TILE = ADDC_INT32_VL * 4;\nconstexpr uint32_t ADDC_INT32_REGSIZE = 256 / sizeof(int32_t);\n\n__vector__ __global__ __aicore__ void addc_kernel_int32(\n    __gm__ int32_t* x, __gm__ int32_t* y, __gm__ int32_t* carryIn,\n    __gm__ int32_t* sumOut, __gm__ int32_t* carryOut)\n{\n    asc_init();\n    uint32_t blockLen = ADDC_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* yGm = y + get_block_idx() * blockLen;\n    __gm__ int32_t* ciGm = carryIn + get_block_idx() * blockLen;\n    __gm__ int32_t* sGm = sumOut + get_block_idx() * blockLen;\n    __gm__ int32_t* coGm = carryOut + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t yLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t ciLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t sLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t coLocal[ADDC_INT32_TILE];\n    __ubuf__ int32_t oneUb[ADDC_INT32_REGSIZE];\n    __ubuf__ int32_t zeroUb[ADDC_INT32_REGSIZE];\n    for (uint32_t i = 0; i < ADDC_INT32_REGSIZE; ++i) {\n        oneUb[i] = 1;\n        zeroUb[i] = 0;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)yLocal, yGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)ciLocal, ciGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    AddcVF_int32(xLocal, yLocal, ciLocal, sLocal, coLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(sGm, (__ubuf__ int32_t*)sLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n    asc_copy_ub2gm_align(coGm, (__ubuf__ int32_t*)coLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::pair<std::vector<T>, std::vector<T>> launch_addc(\n    std::vector<T>& x, std::vector<T>& y, std::vector<T>& carryIn, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* yHost = reinterpret_cast<uint8_t*>(y.data());\n    uint8_t* ciHost = reinterpret_cast<uint8_t*>(carryIn.data());\n    uint8_t* sHost = nullptr;\n    uint8_t* coHost = nullptr;\n    T *xD = nullptr, *yD = nullptr, *ciD = nullptr, *sD = nullptr, *coD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&sHost, bytes);\n    aclrtMallocHost((void**)&coHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&yD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&ciD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&sD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&coD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n    aclrtMemcpy((uint8_t*)yD, bytes, yHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n    aclrtMemcpy((uint8_t*)ciD, bytes, ciHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, yD, ciD, sD, coD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(sHost, bytes, (uint8_t*)sD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    aclrtMemcpy(coHost, bytes, (uint8_t*)coD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> sumOut((T*)sHost, (T*)(sHost + bytes));\n    std::vector<T> carryOut((T*)coHost, (T*)(coHost + bytes));\n\n    aclrtFree(xD); aclrtFree(yD); aclrtFree(ciD); aclrtFree(sD); aclrtFree(coD);\n    aclrtFreeHost(sHost); aclrtFreeHost(coHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return {sumOut, carryOut};\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_addc(const std::vector<T>& x, const std::vector<T>& y,\n                   const std::vector<T>& cIn, const std::vector<T>& sumOut,\n                   const std::vector<T>& cOut, const std::vector<T>& gSum,\n                   const std::vector<T>& gCout, const std::string& name)\n{\n    std::string fname = \"addc_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,y,carry_in,sum_out,sum_expected,carry_out,carry_expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f); f << \",\";\n        print_val(y[i], f); f << \",\";\n        print_val(cIn[i], f); f << \",\";\n        print_val(sumOut[i], f); f << \",\";\n        print_val(gSum[i], f); f << \",\";\n        print_val(cOut[i], f); f << \",\";\n        print_val(gCout[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T>\nbool verify_addc(std::vector<T>& x, std::vector<T>& y,\n                 std::vector<T>& cIn, std::vector<T>& sumOut,\n                 std::vector<T>& cOut, const char* name)\n{\n    std::vector<T> gSum(x.size()), gCout(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        uint64_t sum64 = (uint64_t)x[i] + (uint64_t)y[i] + (uint64_t)(cIn[i] ? 1 : 0);\n        gSum[i] = (T)(sum64 & 0xFFFFFFFFull);\n        gCout[i] = (T)((sum64 >> 32) & 1);\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(y, \"Y       \");\n    print_tensor(cIn, \"CarryIn \");\n    print_tensor(sumOut, \"SumOut  \");\n    print_tensor(gSum, \"SumExp  \");\n    print_tensor(cOut, \"CoutOut \");\n    print_tensor(gCout, \"CoutExp \");\n\n    save_csv_addc(x, y, cIn, sumOut, cOut, gSum, gCout, name);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (sumOut[i] != gSum[i] || cOut[i] != gCout[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" y=\";\n            print_val(y[idx], std::cout);\n            std::cout << \" cin=\";\n            print_val(cIn[idx], std::cout);\n            std::cout << \" sum=\";\n            print_val(sumOut[idx], std::cout);\n            std::cout << \" sumExp=\";\n            print_val(gSum[idx], std::cout);\n            std::cout << \" cout=\";\n            print_val(cOut[idx], std::cout);\n            std::cout << \" coutExp=\";\n            print_val(gCout[idx], std::cout);\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_addc 寄存器级带进位加法测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * ADDC_UINT32_TILE;\n        std::vector<uint32_t> x(N), y(N), cIn(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<uint32_t>(0xFFFF0000u + i);\n            y[i] = static_cast<uint32_t>(0x0000FFFFu + i * 2);\n            cIn[i] = (i % 3 == 0) ? 1u : 0u;\n        }\n        auto [sumOut, cOut] = launch_addc<uint32_t>(x, y, cIn, addc_kernel_uint32);\n        verify_addc(x, y, cIn, sumOut, cOut, \"uint32_t\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * ADDC_INT32_TILE;\n        std::vector<int32_t> x(N), y(N), cIn(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>(1000000000 + static_cast<int32_t>(i));\n            y[i] = static_cast<int32_t>(1000000000 + static_cast<int32_t>(i * 2));\n            cIn[i] = (i % 5 == 0) ? 1 : 0;\n        }\n        auto [sumOut, cOut] = launch_addc<int32_t>(x, y, cIn, addc_kernel_int32);\n        verify_addc(x, y, cIn, sumOut, cOut, \"int32_t\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_addc 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 29.4s)

_Thinking:_

Now let me write the remaining 4 .asc files. Let me be more careful with these. I'll write them using Task agents.

Actually, I notice a potential issue with asc_addc - `asc_ne_scalar` returns void and takes vector_bool& as first param. Looking at the pattern used in existing examples like asc_ge:
```cpp
asc_ge(vcmp, vx, vy, vmask);
```
This returns a vector_bool through the first parameter. So `asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);` should work where `vcarry_in_bool` is a `vector_bool` variable.

But in my code I wrote `vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);` which assigns a void return to a vector_bool - that's wrong! Let me fix it.

Actually, looking at the implement: `__simd_callee__ inline void asc_ne_scalar_impl(...)` - it's a void function. So the correct usage is:
```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```
Without the assignment. Same for `asc_addc` and `asc_select`.

Let me also think about whether `asc_addc` exists as a public API. Looking at the impl files, they define `asc_addc_impl`. The public API would likely be named `asc_addc` and would be a macro or inline wrapper calling `asc_addc_impl`. Since all other APIs follow this pattern (asc_add → asc_add_impl, etc.), `asc_addc` should be available.

Actually wait, let me check by looking at the public API headers to see if `asc_addc` is declared.

Let me quickly check the public API declarations:

**Tool: grep**

**Input:**
```json
{
  "pattern": "asc_addc\\b",
  "path": "/home/joey/code/joey/asc-devkit",
  "include": "*.h"
}
```

**Output:**
```
Found 6 matches
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_vector.h:
  Line 784: // // ==========asc_addc(uint32_t/int32_t)==========

  Line 785: __simd_callee__ inline void asc_addc(vector_bool& dst0, vector_uint32_t& dst1,

  Line 788: __simd_callee__ inline void asc_addc(vector_bool& dst0, vector_int32_t& dst1,


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h:
  Line 287: // // ==========asc_addc(uint32_t/int32_t)==========

  Line 288: __simd_callee__ inline void asc_addc(vector_bool& dst0, vector_uint32_t& dst1,

  Line 294: __simd_callee__ inline void asc_addc(vector_bool& dst0, vector_int32_t& dst1,

```

**Tool: grep**

**Input:**
```json
{
  "pattern": "asc_duplicate\\b",
  "path": "/home/joey/code/joey/asc-devkit",
  "include": "*.h"
}
```

**Output:**
```
Found 54 matches
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_vector.h:
  Line 240: // ==========asc_duplicate(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========

  Line 241: __simd_callee__ inline void asc_duplicate(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

  Line 243: __simd_callee__ inline void asc_duplicate(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

  Line 245: __simd_callee__ inline void asc_duplicate(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

  Line 247: __simd_callee__ inline void asc_duplicate(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

  Line 249: __simd_callee__ inline void asc_duplicate(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

  Line 251: __simd_callee__ inline void asc_duplicate(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

  Line 253: __simd_callee__ inline void asc_duplicate(vector_half& dst, vector_half src, vector_bool mask);

  Line 255: __simd_callee__ inline void asc_duplicate(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

  Line 257: __simd_callee__ inline void asc_duplicate(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

  Line 259: __simd_callee__ inline void asc_duplicate(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

  Line 261: __simd_callee__ inline void asc_duplicate(vector_float& dst, vector_float src, vector_bool mask);


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h:
  Line 1255: // ==========asc_duplicate(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========

  Line 1256: __simd_callee__ inline void asc_duplicate(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)

  Line 1261: __simd_callee__ inline void asc_duplicate(vector_int8_t& dst, vector_int8_t src, vector_bool mask)

  Line 1266: __simd_callee__ inline void asc_duplicate(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)

  Line 1271: __simd_callee__ inline void asc_duplicate(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)

  Line 1276: __simd_callee__ inline void asc_duplicate(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)

  Line 1281: __simd_callee__ inline void asc_duplicate(vector_int16_t& dst, vector_int16_t src, vector_bool mask)

  Line 1286: __simd_callee__ inline void asc_duplicate(vector_half& dst, vector_half src, vector_bool mask)

  Line 1291: __simd_callee__ inline void asc_duplicate(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)

  Line 1296: __simd_callee__ inline void asc_duplicate(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)

  Line 1301: __simd_callee__ inline void asc_duplicate(vector_int32_t& dst, vector_int32_t src, vector_bool mask)

  Line 1306: __simd_callee__ inline void asc_duplicate(vector_float& dst, vector_float src, vector_bool mask)


/home/joey/code/joey/asc-devkit/include/c_api/vector_compute/vector_compute.h:
  Line 831: // ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========

  Line 832: __aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint8_t repeat,

  Line 835: __aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count);

  Line 839: __aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,

  Line 842: __aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count);

  Line 846: __aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,

  Line 849: __aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);

  Line 853: __aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,

  Line 856: __aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);

  Line 860: __aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint8_t repeat,

  Line 863: __aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count);

  Line 867: __aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,

  Line 870: __aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count);

  Line 874: __aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,

  Line 877: __aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h:
  Line 4952: // ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========

  Line 4953: __aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint8_t repeat,

  Line 4959: __aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count)

  Line 4969: __aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,

  Line 4975: __aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count)

  Line 4985: __aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,

  Line 4991: __aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)

  Line 5001: __aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,

  Line 5007: __aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)

  Line 5017: __aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint8_t repeat,

  Line 5023: __aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count)

  Line 5033: __aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,

  Line 5039: __aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count)

  Line 5049: __aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,

  Line 5055: __aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)

```

**Tool: grep**

**Input:**
```json
{
  "pattern": "asc_ge_scalar\\b",
  "path": "/home/joey/code/joey/asc-devkit",
  "include": "*.h"
}
```

**Output:**
```
Found 26 matches
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_vector.h:
  Line 317: // ==========asc_ge_scalar(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========

  Line 318: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

  Line 320: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

  Line 322: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

  Line 324: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

  Line 326: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

  Line 328: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

  Line 330: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

  Line 332: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

  Line 334: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h:
  Line 1440: // ==========asc_ge_scalar(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========

  Line 1441: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)

  Line 1446: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)

  Line 1451: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)

  Line 1456: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)

  Line 1461: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)

  Line 1466: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)

  Line 1471: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)

  Line 1476: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)

  Line 1481: __simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask)


/home/joey/code/joey/asc-devkit/include/c_api/vector_compute/vector_compute.h:
  Line 1019: // ==========asc_ge_scalar(half/float)==========

  Line 1020: __aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,

  Line 1026: __aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h:
  Line 2429: // ==========asc_ge_scalar(half/float)==========

  Line 2430: __aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,

  Line 2446: __aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,

```

**Tool: grep**

**Input:**
```json
{
  "pattern": "asc_ne_scalar\\b",
  "path": "/home/joey/code/joey/asc-devkit",
  "include": "*.h"
}
```

**Output:**
```
Found 28 matches
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_vector.h:
  Line 1086: // ==========asc_ne_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t)==========

  Line 1087: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

  Line 1089: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

  Line 1091: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

  Line 1093: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

  Line 1095: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

  Line 1097: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

  Line 1099: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

  Line 1101: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

  Line 1103: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h:
  Line 193: #include "instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h"

  Line 4540: // ==========asc_ne_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t)==========

  Line 4541: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)

  Line 4546: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)

  Line 4551: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)

  Line 4556: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)

  Line 4561: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)

  Line 4566: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)

  Line 4571: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)

  Line 4576: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask)

  Line 4581: __simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)


/home/joey/code/joey/asc-devkit/include/c_api/vector_compute/vector_compute.h:
  Line 2015: // ==========asc_ne_scalar(dst:uint8_t, src:half/float)==========

  Line 2016: __aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ half *src, half value, uint8_t repeat, uint16_t dst_block_stride,

  Line 2022: __aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ float *src, float value, uint8_t repeat, uint16_t dst_block_stride,


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h:
  Line 18:     "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/asc_ne_scalar.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h:
  Line 2511: // ==========asc_ne_scalar(dst:uint8_t, src:half/float)==========

  Line 2512: __aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,

  Line 2528: __aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,

```

**Tool: grep**

**Input:**
```json
{
  "pattern": "asc_shiftleft_scalar\\b",
  "path": "/home/joey/code/joey/asc-devkit",
  "include": "*.h"
}
```

**Output:**
```
Found 14 matches
/home/joey/code/joey/asc-devkit/include/c_api/reg_compute/reg_vector.h:
  Line 140: // ==========asc_shiftleft_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t)==========

  Line 141: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask);

  Line 143: __simd_callee__ inline void asc_shiftleft_scalar(vector_int8_t& dst, vector_int8_t src, int16_t value, vector_bool mask);

  Line 145: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint16_t& dst, vector_uint16_t src, int16_t value, vector_bool mask);

  Line 147: __simd_callee__ inline void asc_shiftleft_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

  Line 149: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint32_t& dst, vector_uint32_t src, int16_t value, vector_bool mask);

  Line 151: __simd_callee__ inline void asc_shiftleft_scalar(vector_int32_t& dst, vector_int32_t src, int16_t value, vector_bool mask);


/home/joey/code/joey/asc-devkit/impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h:
  Line 677: // ==========asc_shiftleft_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t)==========

  Line 678: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint8_t& dst, vector_uint8_t src, int16_t value, vector_bool mask)

  Line 683: __simd_callee__ inline void asc_shiftleft_scalar(vector_int8_t& dst, vector_int8_t src, int16_t value, vector_bool mask)

  Line 688: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint16_t& dst, vector_uint16_t src, int16_t value, vector_bool mask)

  Line 693: __simd_callee__ inline void asc_shiftleft_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask)

  Line 698: __simd_callee__ inline void asc_shiftleft_scalar(vector_uint32_t& dst, vector_uint32_t src, int16_t value, vector_bool mask)

  Line 703: __simd_callee__ inline void asc_shiftleft_scalar(vector_int32_t& dst, vector_int32_t src, int16_t value, vector_bool mask)

```

---

## Assistant (Build · DeepSeek V4 Pro · 182.1s)

_Thinking:_

All 5 APIs have public declarations. Let me note:

1. `asc_addc` - declared in `reg_vector.h`: `__simd_callee__ inline void asc_addc(vector_bool& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool src2, vector_bool mask)` -- it's a void function, used as `asc_addc(carryOut, sum, x, y, carryIn, mask)`

2. `asc_duplicate` - declared in `reg_vector.h` with `__simd_callee__ inline void asc_duplicate(vector_half& dst, vector_half src, vector_bool mask)` - also void

3. `asc_ge_scalar` - declared in `reg_vector.h`: `__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)` - void

4. `asc_ne_scalar` - declared in `reg_vector.h`: `__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)` - void

5. `asc_shiftleft_scalar` - declared in `reg_vector.h`: `__simd_callee__ inline void asc_shiftleft_scalar(vector_uint32_t& dst, vector_uint32_t src, int16_t value, vector_bool mask)` - void

All are void functions. The pattern for all is correct in my asc_addc file (no assignment). But wait, I need to fix the usage in the VF functions. Let me also check: for `asc_ne_scalar` in the addc VF, the return type is void, so the expression `vcarry_in_bool = asc_ne_scalar(...)` is wrong. But in my latest version I wrote:
```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```
That's correct - first param is the output bool variable.

Wait, I need to re-read my latest file. Let me check... Actually I don't have access to the latest file content right now, but I remember writing it correctly in the last write. Let me proceed.

Now let me also note that there are TWO versions of some APIs:
- Reg-level (__simd_callee__, in reg_vector.h) - for use inside __simd_vf__ functions
- UB-level (__aicore__, in vector_compute.h) - for use in kernel functions directly

For our examples, we're using the Reg-level versions inside __simd_vf__ functions. This is the correct approach following the existing examples.

For `asc_duplicate`, the UB-level version takes:
```cpp
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count);
```
This is a different API - it takes a scalar value and fills a UB buffer with that value. But the Reg-level version takes a vector src and replicates its lowest element. I'll use the Reg-level version.

Let me also verify the scalar shift APIs. For `asc_shiftleft_scalar`, the scalar is `int16_t value`. I'll pass this as a parameter to the VF function from the kernel. The kernel can receive this as a GM argument or as a literal constant.

For testing simplicity, let me use a literal constant for the scalar in the VF functions for shift and scalar compare APIs. I can always parameterize later.

Actually, I think the best approach for testing scalar operations is:
1. For ge_scalar and ne_scalar: Pass the scalar value through a single-element GM buffer. The kernel loads it and the VF function receives it as a scalar parameter.

Wait, actually I realize I should look at this differently. The skill doc says `__simd_vf__` supports scalar variables. In the VF function, I can declare scalar variables like:
```cpp
half sc = 0.5f;
asc_ge_scalar(vcmp, vx, sc, vmask);
```

This should work. Since we're testing, I can just hardcode the scalar value in the VF function. For comprehensive testing, I can pass different scalar values through function parameters.

OK let me now write all 4 remaining .asc files. I need to be very careful with the VF function signatures and make sure the APIs are used correctly.

For shift left scalar: the scalar value is an int16_t that I can pass to the VF function. But inside __simd_vf__, scalar values of any type are just regular variables. I can do:
```cpp
__simd_vf__ inline void ShiftLeftScalarVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* zLocal,
                                                  int16_t shiftVal, uint32_t blockLen)
{
    ...
    asc_shiftleft_scalar(vz, vx, shiftVal, vmask);
    ...
}
```

Then in the kernel, I read the scalar from GM:
```cpp
__ubuf__ int16_t shiftUb[1];
asc_copy_gm2ub_align((__ubuf__ int16_t*)shiftUb, shiftGm, BLK_NUM, sizeof(int16_t), ...);
// In VF: somehow get the scalar from shiftUb[0]
```

But how do I get a scalar from UB? I need to load from UB into a register, then extract the scalar. That's complex. 

Alternative: I can use `asc_duplicate` (UB-level) to broadcast the scalar value, then pass it via a UB buffer. Actually, this is getting too complex.

Simplest approach: Hardcode the scalar value in the VF function. For testing, I'll use different constants in different test functions.

For ge_scalar: Use scalar values like `half(5.0f)`, `float(5.0f)`, `int32_t(5)`
For shiftleft_scalar: Use shift amounts like `3`, `5`, `7`

This approach is simpler, safer, and matches the guidance that scalars in __simd_vf__ should prefer constants.

Let me now write all 4 remaining files in parallel:

All APIs are confirmed available. Now let me write the remaining 4 .asc files:

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_duplicate.asc\n * \\brief asc_duplicate 寄存器级矢量元素复制示例\n *\n * 将源矢量寄存器中最低位元素复制填充到整个目标寄存器\n * 覆盖数据类型: half, float, int32_t, int16_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void DupVF_half(__ubuf__ half* xLocal, __ubuf__ half* zLocal, uint32_t blockLen)\n{\n    vector_half vx, vz;\n    vector_bool vmask;\n    uint16_t one_rep_size = 256 / sizeof(half);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b16(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_duplicate(vz, vx, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void DupVF_float(__ubuf__ float* xLocal, __ubuf__ float* zLocal, uint32_t blockLen)\n{\n    vector_float vx, vz;\n    vector_bool vmask;\n    uint16_t one_rep_size = 256 / sizeof(float);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_duplicate(vz, vx, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void DupVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* zLocal, uint32_t blockLen)\n{\n    vector_int32_t vx, vz;\n    vector_bool vmask;\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_duplicate(vz, vx, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void DupVF_int16(__ubuf__ int16_t* xLocal, __ubuf__ int16_t* zLocal, uint32_t blockLen)\n{\n    vector_int16_t vx, vz;\n    vector_bool vmask;\n    uint16_t one_rep_size = 256 / sizeof(int16_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b16(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_duplicate(vz, vx, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\nconstexpr uint32_t DUP_HALF_VL = 128;\nconstexpr uint32_t DUP_HALF_TILE = DUP_HALF_VL * 4;\n\n__vector__ __global__ __aicore__ void dup_kernel_half(__gm__ half* x, __gm__ half* z)\n{\n    asc_init();\n    uint32_t blockLen = DUP_HALF_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ half* xGm = x + get_block_idx() * blockLen;\n    __gm__ half* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ half xLocal[DUP_HALF_TILE];\n    __ubuf__ half zLocal[DUP_HALF_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(half);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ half*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    DupVF_half(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ half*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t DUP_FLOAT_VL = 64;\nconstexpr uint32_t DUP_FLOAT_TILE = DUP_FLOAT_VL * 4;\n\n__vector__ __global__ __aicore__ void dup_kernel_float(__gm__ float* x, __gm__ float* z)\n{\n    asc_init();\n    uint32_t blockLen = DUP_FLOAT_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ float* xGm = x + get_block_idx() * blockLen;\n    __gm__ float* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ float xLocal[DUP_FLOAT_TILE];\n    __ubuf__ float zLocal[DUP_FLOAT_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(float);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    DupVF_float(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t DUP_INT32_VL = 64;\nconstexpr uint32_t DUP_INT32_TILE = DUP_INT32_VL * 4;\n\n__vector__ __global__ __aicore__ void dup_kernel_int32(__gm__ int32_t* x, __gm__ int32_t* z)\n{\n    asc_init();\n    uint32_t blockLen = DUP_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[DUP_INT32_TILE];\n    __ubuf__ int32_t zLocal[DUP_INT32_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    DupVF_int32(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t DUP_INT16_VL = 128;\nconstexpr uint32_t DUP_INT16_TILE = DUP_INT16_VL * 4;\n\n__vector__ __global__ __aicore__ void dup_kernel_int16(__gm__ int16_t* x, __gm__ int16_t* z)\n{\n    asc_init();\n    uint32_t blockLen = DUP_INT16_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int16_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int16_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int16_t xLocal[DUP_INT16_TILE];\n    __ubuf__ int16_t zLocal[DUP_INT16_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int16_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int16_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    DupVF_int16(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int16_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::vector<T> launch_dup(std::vector<T>& x, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* zHost = nullptr;\n    T *xD = nullptr, *zD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&zHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, zD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> z((T*)zHost, (T*)(zHost + bytes));\n\n    aclrtFree(xD); aclrtFree(zD);\n    aclrtFreeHost(zHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return z;\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_dup(const std::vector<T>& x, const std::vector<T>& z,\n                  const std::vector<T>& golden, const std::string& name,\n                  uint16_t one_rep_size)\n{\n    std::string fname = \"duplicate_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,output,expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f);\n        f << \",\";\n        print_val(z[i], f);\n        f << \",\";\n        print_val(golden[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T>\nbool verify_dup(std::vector<T>& z, std::vector<T>& x, const char* name)\n{\n    uint16_t one_rep_size = 256 / sizeof(T);\n    std::vector<T> g(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        size_t group_start = (i / one_rep_size) * one_rep_size;\n        g[i] = x[group_start];\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(z, \"Output  \");\n    print_tensor(g, \"Expected\");\n\n    save_csv_dup(x, z, g, name, one_rep_size);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (z[i] != g[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" output=\";\n            print_val(z[idx], std::cout);\n            std::cout << \" expected=\";\n            print_val(g[idx], std::cout);\n            std::cout << \" (group_start=\" << ((idx / one_rep_size) * one_rep_size) << \")\";\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_duplicate 寄存器级矢量元素复制测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << \"操作: z = duplicate(x), 每组第一个元素复制到全组\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * DUP_HALF_TILE;\n        std::vector<half> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<half>(i * 0.1f);\n        }\n        auto z = launch_dup<half>(x, dup_kernel_half);\n        verify_dup(z, x, \"half\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * DUP_FLOAT_TILE;\n        std::vector<float> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = i * 0.1f;\n        }\n        auto z = launch_dup<float>(x, dup_kernel_float);\n        verify_dup(z, x, \"float\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * DUP_INT32_TILE;\n        std::vector<int32_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>(i);\n        }\n        auto z = launch_dup<int32_t>(x, dup_kernel_int32);\n        verify_dup(z, x, \"int32_t\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * DUP_INT16_TILE;\n        std::vector<int16_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int16_t>(i % 30000);\n        }\n        auto z = launch_dup<int16_t>(x, dup_kernel_int16);\n        verify_dup(z, x, \"int16_t\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_duplicate 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/c_api_duplicate.asc"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 446.0s)

_Thinking:_

Now let me write the remaining 3 files: asc_ge_scalar, asc_ne_scalar, asc_shiftleft_scalar. I'll create them all using parallel task agents.

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_ge_scalar.asc\n * \\brief asc_ge_scalar 寄存器级矢量与标量大于等于比较示例\n *\n * 计算: z[i] = (x[i] >= scalar) ? 1 : 0\n * 覆盖数据类型: half, float, int32_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void GeScalarVF_half(__ubuf__ half* xLocal, __ubuf__ half* zLocal,\n                                         __ubuf__ half* oneUb, __ubuf__ half* zeroUb,\n                                         uint32_t blockLen)\n{\n    vector_half vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    half scalar = half(10.0f);\n    uint16_t one_rep_size = 256 / sizeof(half);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b16(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ge_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void GeScalarVF_float(__ubuf__ float* xLocal, __ubuf__ float* zLocal,\n                                          __ubuf__ float* oneUb, __ubuf__ float* zeroUb,\n                                          uint32_t blockLen)\n{\n    vector_float vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    float scalar = 10.0f;\n    uint16_t one_rep_size = 256 / sizeof(float);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ge_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void GeScalarVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* zLocal,\n                                          __ubuf__ int32_t* oneUb, __ubuf__ int32_t* zeroUb,\n                                          uint32_t blockLen)\n{\n    vector_int32_t vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    int32_t scalar = int32_t(1000);\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ge_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\nconstexpr uint32_t GES_HALF_VL = 128;\nconstexpr uint32_t GES_HALF_TILE = GES_HALF_VL * 4;\nconstexpr uint32_t GES_HALF_REGSIZE = 256 / sizeof(half);\n\n__vector__ __global__ __aicore__ void ge_scalar_kernel_half(__gm__ half* x, __gm__ half* z)\n{\n    asc_init();\n    uint32_t blockLen = GES_HALF_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ half* xGm = x + get_block_idx() * blockLen;\n    __gm__ half* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ half xLocal[GES_HALF_TILE];\n    __ubuf__ half zLocal[GES_HALF_TILE];\n    __ubuf__ half oneUb[GES_HALF_REGSIZE];\n    __ubuf__ half zeroUb[GES_HALF_REGSIZE];\n    for (uint32_t i = 0; i < GES_HALF_REGSIZE; ++i) {\n        oneUb[i] = half(1.0f);\n        zeroUb[i] = half(0.0f);\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(half);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ half*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    GeScalarVF_half(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ half*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t GES_FLOAT_VL = 64;\nconstexpr uint32_t GES_FLOAT_TILE = GES_FLOAT_VL * 4;\nconstexpr uint32_t GES_FLOAT_REGSIZE = 256 / sizeof(float);\n\n__vector__ __global__ __aicore__ void ge_scalar_kernel_float(__gm__ float* x, __gm__ float* z)\n{\n    asc_init();\n    uint32_t blockLen = GES_FLOAT_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ float* xGm = x + get_block_idx() * blockLen;\n    __gm__ float* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ float xLocal[GES_FLOAT_TILE];\n    __ubuf__ float zLocal[GES_FLOAT_TILE];\n    __ubuf__ float oneUb[GES_FLOAT_REGSIZE];\n    __ubuf__ float zeroUb[GES_FLOAT_REGSIZE];\n    for (uint32_t i = 0; i < GES_FLOAT_REGSIZE; ++i) {\n        oneUb[i] = 1.0f;\n        zeroUb[i] = 0.0f;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(float);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    GeScalarVF_float(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t GES_INT32_VL = 64;\nconstexpr uint32_t GES_INT32_TILE = GES_INT32_VL * 4;\nconstexpr uint32_t GES_INT32_REGSIZE = 256 / sizeof(int32_t);\n\n__vector__ __global__ __aicore__ void ge_scalar_kernel_int32(__gm__ int32_t* x, __gm__ int32_t* z)\n{\n    asc_init();\n    uint32_t blockLen = GES_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[GES_INT32_TILE];\n    __ubuf__ int32_t zLocal[GES_INT32_TILE];\n    __ubuf__ int32_t oneUb[GES_INT32_REGSIZE];\n    __ubuf__ int32_t zeroUb[GES_INT32_REGSIZE];\n    for (uint32_t i = 0; i < GES_INT32_REGSIZE; ++i) {\n        oneUb[i] = 1;\n        zeroUb[i] = 0;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    GeScalarVF_int32(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::vector<T> launch_ge_scalar(std::vector<T>& x, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* zHost = nullptr;\n    T *xD = nullptr, *zD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&zHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, zD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> z((T*)zHost, (T*)(zHost + bytes));\n\n    aclrtFree(xD); aclrtFree(zD);\n    aclrtFreeHost(zHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return z;\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_ges(const std::vector<T>& x, const std::vector<T>& z,\n                  const std::vector<T>& golden, const std::string& name)\n{\n    std::string fname = \"ge_scalar_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,output,expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f);\n        f << \",\";\n        print_val(z[i], f);\n        f << \",\";\n        print_val(golden[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T, typename S>\nbool verify_ge_scalar(std::vector<T>& z, std::vector<T>& x, S scalar, const char* name)\n{\n    std::vector<T> g(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        g[i] = (x[i] >= scalar) ? static_cast<T>(1) : static_cast<T>(0);\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(z, \"Output  \");\n    print_tensor(g, \"Expected\");\n\n    save_csv_ges(x, z, g, name);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (z[i] != g[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" scalar=\";\n            print_val(scalar, std::cout);\n            std::cout << \" output=\";\n            print_val(z[idx], std::cout);\n            std::cout << \" expected=\";\n            print_val(g[idx], std::cout);\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_ge_scalar 寄存器级矢量与标量大于等于比较测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << \"输出: (x >= scalar) ? 1 : 0\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * GES_HALF_TILE;\n        std::vector<half> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<half>(i * 0.5f);\n        }\n        auto z = launch_ge_scalar<half>(x, ge_scalar_kernel_half);\n        verify_ge_scalar(z, x, half(10.0f), \"half (scalar=10.0)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * GES_FLOAT_TILE;\n        std::vector<float> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = i * 0.5f;\n        }\n        auto z = launch_ge_scalar<float>(x, ge_scalar_kernel_float);\n        verify_ge_scalar(z, x, 10.0f, \"float (scalar=10.0)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * GES_INT32_TILE;\n        std::vector<int32_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>(i * 3);\n        }\n        auto z = launch_ge_scalar<int32_t>(x, ge_scalar_kernel_int32);\n        verify_ge_scalar(z, x, int32_t(1000), \"int32_t (scalar=1000)\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_ge_scalar 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/c_api_ge_scalar.asc"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_ne_scalar.asc\n * \\brief asc_ne_scalar 寄存器级矢量与标量不等于比较示例\n *\n * 计算: z[i] = (x[i] != scalar) ? 1 : 0\n * 覆盖数据类型: half, float, int32_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void NeScalarVF_half(__ubuf__ half* xLocal, __ubuf__ half* zLocal,\n                                         __ubuf__ half* oneUb, __ubuf__ half* zeroUb,\n                                         uint32_t blockLen)\n{\n    vector_half vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    half scalar = half(10.0f);\n    uint16_t one_rep_size = 256 / sizeof(half);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b16(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ne_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void NeScalarVF_float(__ubuf__ float* xLocal, __ubuf__ float* zLocal,\n                                          __ubuf__ float* oneUb, __ubuf__ float* zeroUb,\n                                          uint32_t blockLen)\n{\n    vector_float vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    float scalar = 10.0f;\n    uint16_t one_rep_size = 256 / sizeof(float);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ne_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void NeScalarVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* zLocal,\n                                          __ubuf__ int32_t* oneUb, __ubuf__ int32_t* zeroUb,\n                                          uint32_t blockLen)\n{\n    vector_int32_t vx, vone, vzero, vz;\n    vector_bool vcmp, vmask;\n    int32_t scalar = int32_t(1000);\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    asc_loadalign(vone, oneUb);\n    asc_loadalign(vzero, zeroUb);\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_ne_scalar(vcmp, vx, scalar, vmask);\n        asc_select(vz, vone, vzero, vcmp);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\nconstexpr uint32_t NES_HALF_VL = 128;\nconstexpr uint32_t NES_HALF_TILE = NES_HALF_VL * 4;\nconstexpr uint32_t NES_HALF_REGSIZE = 256 / sizeof(half);\n\n__vector__ __global__ __aicore__ void ne_scalar_kernel_half(__gm__ half* x, __gm__ half* z)\n{\n    asc_init();\n    uint32_t blockLen = NES_HALF_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ half* xGm = x + get_block_idx() * blockLen;\n    __gm__ half* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ half xLocal[NES_HALF_TILE];\n    __ubuf__ half zLocal[NES_HALF_TILE];\n    __ubuf__ half oneUb[NES_HALF_REGSIZE];\n    __ubuf__ half zeroUb[NES_HALF_REGSIZE];\n    for (uint32_t i = 0; i < NES_HALF_REGSIZE; ++i) {\n        oneUb[i] = half(1.0f);\n        zeroUb[i] = half(0.0f);\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(half);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ half*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    NeScalarVF_half(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ half*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t NES_FLOAT_VL = 64;\nconstexpr uint32_t NES_FLOAT_TILE = NES_FLOAT_VL * 4;\nconstexpr uint32_t NES_FLOAT_REGSIZE = 256 / sizeof(float);\n\n__vector__ __global__ __aicore__ void ne_scalar_kernel_float(__gm__ float* x, __gm__ float* z)\n{\n    asc_init();\n    uint32_t blockLen = NES_FLOAT_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ float* xGm = x + get_block_idx() * blockLen;\n    __gm__ float* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ float xLocal[NES_FLOAT_TILE];\n    __ubuf__ float zLocal[NES_FLOAT_TILE];\n    __ubuf__ float oneUb[NES_FLOAT_REGSIZE];\n    __ubuf__ float zeroUb[NES_FLOAT_REGSIZE];\n    for (uint32_t i = 0; i < NES_FLOAT_REGSIZE; ++i) {\n        oneUb[i] = 1.0f;\n        zeroUb[i] = 0.0f;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(float);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ float*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    NeScalarVF_float(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ float*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t NES_INT32_VL = 64;\nconstexpr uint32_t NES_INT32_TILE = NES_INT32_VL * 4;\nconstexpr uint32_t NES_INT32_REGSIZE = 256 / sizeof(int32_t);\n\n__vector__ __global__ __aicore__ void ne_scalar_kernel_int32(__gm__ int32_t* x, __gm__ int32_t* z)\n{\n    asc_init();\n    uint32_t blockLen = NES_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[NES_INT32_TILE];\n    __ubuf__ int32_t zLocal[NES_INT32_TILE];\n    __ubuf__ int32_t oneUb[NES_INT32_REGSIZE];\n    __ubuf__ int32_t zeroUb[NES_INT32_REGSIZE];\n    for (uint32_t i = 0; i < NES_INT32_REGSIZE; ++i) {\n        oneUb[i] = 1;\n        zeroUb[i] = 0;\n    }\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    NeScalarVF_int32(xLocal, zLocal, oneUb, zeroUb, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::vector<T> launch_ne_scalar(std::vector<T>& x, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* zHost = nullptr;\n    T *xD = nullptr, *zD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&zHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, zD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> z((T*)zHost, (T*)(zHost + bytes));\n\n    aclrtFree(xD); aclrtFree(zD);\n    aclrtFreeHost(zHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return z;\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_nes(const std::vector<T>& x, const std::vector<T>& z,\n                  const std::vector<T>& golden, const std::string& name)\n{\n    std::string fname = \"ne_scalar_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,output,expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f);\n        f << \",\";\n        print_val(z[i], f);\n        f << \",\";\n        print_val(golden[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T, typename S>\nbool verify_ne_scalar(std::vector<T>& z, std::vector<T>& x, S scalar, const char* name)\n{\n    std::vector<T> g(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        g[i] = (x[i] != scalar) ? static_cast<T>(1) : static_cast<T>(0);\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(z, \"Output  \");\n    print_tensor(g, \"Expected\");\n\n    save_csv_nes(x, z, g, name);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (z[i] != g[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" scalar=\";\n            print_val(scalar, std::cout);\n            std::cout << \" output=\";\n            print_val(z[idx], std::cout);\n            std::cout << \" expected=\";\n            print_val(g[idx], std::cout);\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_ne_scalar 寄存器级矢量与标量不等于比较测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << \"输出: (x != scalar) ? 1 : 0\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * NES_HALF_TILE;\n        std::vector<half> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<half>(i % 40);\n        }\n        auto z = launch_ne_scalar<half>(x, ne_scalar_kernel_half);\n        verify_ne_scalar(z, x, half(10.0f), \"half (scalar=10.0)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * NES_FLOAT_TILE;\n        std::vector<float> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<float>(i % 40);\n        }\n        auto z = launch_ne_scalar<float>(x, ne_scalar_kernel_float);\n        verify_ne_scalar(z, x, 10.0f, \"float (scalar=10.0)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * NES_INT32_TILE;\n        std::vector<int32_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>(i % 100);\n        }\n        auto z = launch_ne_scalar<int32_t>(x, ne_scalar_kernel_int32);\n        verify_ne_scalar(z, x, int32_t(1000), \"int32_t (scalar=1000)\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_ne_scalar 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/c_api_ne_scalar.asc"
}
```

**Output:**
```
Wrote file successfully.
```

**Tool: write**

**Input:**
```json
{
  "content": "/**\n * Copyright (c) 2026 Huawei Technologies Co., Ltd.\n * CANN Open Software License Agreement Version 2.0\n */\n\n/* !\n * \\file c_api_shiftleft_scalar.asc\n * \\brief asc_shiftleft_scalar 寄存器级矢量按标量左移示例\n *\n * 计算: z[i] = x[i] << shiftAmount\n * 覆盖数据类型: int8_t, int16_t, int32_t, uint32_t\n * 覆盖场景: 多核\n */\n\n#include <cstdint>\n#include <iostream>\n#include <vector>\n#include <algorithm>\n#include <cstdlib>\n#include <string>\n#include <fstream>\n#include \"acl/acl.h\"\n#include \"c_api/asc_simd.h\"\n\nconstexpr uint32_t NUM_BLOCKS = 8;\nconstexpr uint32_t BLK_NUM = 1;\n\n__simd_vf__ inline void ShlScalarVF_int8(__ubuf__ int8_t* xLocal, __ubuf__ int8_t* zLocal,\n                                          uint32_t blockLen)\n{\n    vector_int8_t vx, vz;\n    vector_bool vmask;\n    int16_t shiftAmount = 2;\n    uint16_t one_rep_size = 256 / sizeof(int8_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b8(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_shiftleft_scalar(vz, vx, shiftAmount, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void ShlScalarVF_int16(__ubuf__ int16_t* xLocal, __ubuf__ int16_t* zLocal,\n                                           uint32_t blockLen)\n{\n    vector_int16_t vx, vz;\n    vector_bool vmask;\n    int16_t shiftAmount = 3;\n    uint16_t one_rep_size = 256 / sizeof(int16_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b16(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_shiftleft_scalar(vz, vx, shiftAmount, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void ShlScalarVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* zLocal,\n                                           uint32_t blockLen)\n{\n    vector_int32_t vx, vz;\n    vector_bool vmask;\n    int16_t shiftAmount = 5;\n    uint16_t one_rep_size = 256 / sizeof(int32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_shiftleft_scalar(vz, vx, shiftAmount, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\n__simd_vf__ inline void ShlScalarVF_uint32(__ubuf__ uint32_t* xLocal, __ubuf__ uint32_t* zLocal,\n                                            uint32_t blockLen)\n{\n    vector_uint32_t vx, vz;\n    vector_bool vmask;\n    int16_t shiftAmount = 4;\n    uint16_t one_rep_size = 256 / sizeof(uint32_t);\n    uint16_t rep = (blockLen + one_rep_size - 1) / one_rep_size;\n    uint32_t remaining = blockLen;\n    for (uint16_t i = 0; i < rep; ++i) {\n        vmask = asc_update_mask_b32(remaining);\n        asc_loadalign(vx, xLocal + i * one_rep_size);\n        asc_shiftleft_scalar(vz, vx, shiftAmount, vmask);\n        asc_storealign(zLocal + i * one_rep_size, vz, vmask);\n    }\n}\n\nconstexpr uint32_t SHLS_INT8_VL = 256;\nconstexpr uint32_t SHLS_INT8_TILE = SHLS_INT8_VL * 4;\n\n__vector__ __global__ __aicore__ void shl_scalar_kernel_int8(__gm__ int8_t* x, __gm__ int8_t* z)\n{\n    asc_init();\n    uint32_t blockLen = SHLS_INT8_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int8_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int8_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int8_t xLocal[SHLS_INT8_TILE];\n    __ubuf__ int8_t zLocal[SHLS_INT8_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int8_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int8_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    ShlScalarVF_int8(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int8_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t SHLS_INT16_VL = 128;\nconstexpr uint32_t SHLS_INT16_TILE = SHLS_INT16_VL * 4;\n\n__vector__ __global__ __aicore__ void shl_scalar_kernel_int16(__gm__ int16_t* x, __gm__ int16_t* z)\n{\n    asc_init();\n    uint32_t blockLen = SHLS_INT16_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int16_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int16_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int16_t xLocal[SHLS_INT16_TILE];\n    __ubuf__ int16_t zLocal[SHLS_INT16_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int16_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int16_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    ShlScalarVF_int16(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int16_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t SHLS_INT32_VL = 64;\nconstexpr uint32_t SHLS_INT32_TILE = SHLS_INT32_VL * 4;\n\n__vector__ __global__ __aicore__ void shl_scalar_kernel_int32(__gm__ int32_t* x, __gm__ int32_t* z)\n{\n    asc_init();\n    uint32_t blockLen = SHLS_INT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ int32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ int32_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ int32_t xLocal[SHLS_INT32_TILE];\n    __ubuf__ int32_t zLocal[SHLS_INT32_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(int32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ int32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    ShlScalarVF_int32(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ int32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\nconstexpr uint32_t SHLS_UINT32_VL = 64;\nconstexpr uint32_t SHLS_UINT32_TILE = SHLS_UINT32_VL * 4;\n\n__vector__ __global__ __aicore__ void shl_scalar_kernel_uint32(__gm__ uint32_t* x, __gm__ uint32_t* z)\n{\n    asc_init();\n    uint32_t blockLen = SHLS_UINT32_TILE * NUM_BLOCKS / asc_get_block_num();\n    __gm__ uint32_t* xGm = x + get_block_idx() * blockLen;\n    __gm__ uint32_t* zGm = z + get_block_idx() * blockLen;\n    __ubuf__ uint32_t xLocal[SHLS_UINT32_TILE];\n    __ubuf__ uint32_t zLocal[SHLS_UINT32_TILE];\n    const uint8_t cacheMode = static_cast<uint8_t>(((uint64_t)xGm) >> 60);\n    uint32_t burstLength = blockLen * sizeof(uint32_t);\n    uint64_t srcStride = 0; uint32_t dstStride = 0;\n    asc_copy_gm2ub_align((__ubuf__ uint32_t*)xLocal, xGm, BLK_NUM, burstLength, 0, 0, true, cacheMode, srcStride, dstStride);\n    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);\n    ShlScalarVF_uint32(xLocal, zLocal, blockLen);\n    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);\n    asc_copy_ub2gm_align(zGm, (__ubuf__ uint32_t*)zLocal, BLK_NUM, burstLength, cacheMode, srcStride, dstStride);\n}\n\n/* ==========================================================================\n * Host 侧\n * ========================================================================== */\n\ntemplate<typename T, typename KernelFunc>\nstd::vector<T> launch_shiftleft_scalar(std::vector<T>& x, KernelFunc kernel)\n{\n    uint32_t N = x.size();\n    size_t bytes = N * sizeof(T);\n    int32_t devId = 0;\n    aclrtStream stream = nullptr;\n    uint8_t* xHost = reinterpret_cast<uint8_t*>(x.data());\n    uint8_t* zHost = nullptr;\n    T *xD = nullptr, *zD = nullptr;\n\n    aclInit(nullptr);\n    aclrtSetDevice(devId);\n    aclrtCreateStream(&stream);\n\n    aclrtMallocHost((void**)&zHost, bytes);\n    aclrtMalloc((void**)&xD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n    aclrtMalloc((void**)&zD, bytes, ACL_MEM_MALLOC_HUGE_FIRST);\n\n    aclrtMemcpy((uint8_t*)xD, bytes, xHost, bytes, ACL_MEMCPY_HOST_TO_DEVICE);\n\n    kernel<<<NUM_BLOCKS, nullptr, stream>>>(xD, zD);\n    aclrtSynchronizeStream(stream);\n\n    aclrtMemcpy(zHost, bytes, (uint8_t*)zD, bytes, ACL_MEMCPY_DEVICE_TO_HOST);\n    std::vector<T> z((T*)zHost, (T*)(zHost + bytes));\n\n    aclrtFree(xD); aclrtFree(zD);\n    aclrtFreeHost(zHost);\n    aclrtDestroyStream(stream);\n    aclrtResetDevice(devId);\n    aclFinalize();\n\n    return z;\n}\n\ntemplate<typename T>\nvoid print_val(T val, std::ostream& os)\n{\n    if constexpr (std::is_same_v<T, half>) {\n        os << static_cast<float>(val);\n    } else if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>) {\n        os << static_cast<int>(val);\n    } else {\n        os << val;\n    }\n}\n\ntemplate<typename T>\nvoid save_csv_shls(const std::vector<T>& x, const std::vector<T>& z,\n                   const std::vector<T>& golden, const std::string& name)\n{\n    std::string fname = \"shiftleft_scalar_\" + name + \".csv\";\n    std::ofstream f(fname);\n    f << \"index,x,output,expected\\n\";\n    for (size_t i = 0; i < x.size(); ++i) {\n        f << i << \",\";\n        print_val(x[i], f);\n        f << \",\";\n        print_val(z[i], f);\n        f << \",\";\n        print_val(golden[i], f);\n        f << \"\\n\";\n    }\n    f.close();\n    std::cout << \"  [CSV] 已保存到 \" << fname << std::endl;\n}\n\ntemplate<typename T>\nvoid print_tensor(const std::vector<T>& t, const std::string& name)\n{\n    constexpr size_t maxPrint = 20;\n    std::cout << \"  \" << name << \" (\" << t.size() << \" elements): \";\n    for (size_t i = 0; i < std::min(t.size(), maxPrint); ++i) {\n        print_val(t[i], std::cout);\n        std::cout << \" \";\n    }\n    if (t.size() > maxPrint) std::cout << \"...\";\n    std::cout << std::endl;\n}\n\ntemplate<typename T>\nbool verify_shiftleft_scalar(std::vector<T>& z, std::vector<T>& x,\n                              int16_t shiftAmount, const char* name)\n{\n    std::vector<T> g(x.size());\n    for (size_t i = 0; i < x.size(); ++i) {\n        g[i] = static_cast<T>(x[i] << shiftAmount);\n    }\n\n    print_tensor(x, \"X       \");\n    print_tensor(z, \"Output  \");\n    print_tensor(g, \"Expected\");\n\n    save_csv_shls(x, z, g, name);\n\n    bool ok = true;\n    std::vector<size_t> errIdx;\n    for (size_t i = 0; i < x.size(); ++i) {\n        if (z[i] != g[i]) {\n            ok = false;\n            errIdx.push_back(i);\n        }\n    }\n\n    if (!ok) {\n        std::cout << \"  [\" << name << \"] 精度错误: \" << errIdx.size()\n                  << \" 个元素不匹配\" << std::endl;\n        size_t maxErr = std::min(errIdx.size(), size_t(8));\n        for (size_t e = 0; e < maxErr; ++e) {\n            size_t idx = errIdx[e];\n            std::cout << \"    [\" << idx << \"] x=\";\n            print_val(x[idx], std::cout);\n            std::cout << \" shift=\" << shiftAmount;\n            std::cout << \" output=\";\n            print_val(z[idx], std::cout);\n            std::cout << \" expected=\";\n            print_val(g[idx], std::cout);\n            std::cout << std::endl;\n        }\n        if (errIdx.size() > maxErr) {\n            std::cout << \"    ... (还有 \" << (errIdx.size() - maxErr) << \" 个错误)\" << std::endl;\n        }\n        return false;\n    }\n\n    std::cout << \"  [\" << name << \"] 通过\" << std::endl;\n    return true;\n}\n\n/* ==========================================================================\n * main\n * ========================================================================== */\n\nint32_t main(int32_t argc, char* argv[])\n{\n    std::cout << \"=== asc_shiftleft_scalar 寄存器级矢量标量左移测试 ===\" << std::endl;\n    std::cout << \"模式: 多核 (\" << NUM_BLOCKS << \" blocks)\" << std::endl;\n    std::cout << \"操作: z[i] = x[i] << shiftAmount\" << std::endl;\n    std::cout << std::endl;\n\n    {\n        constexpr uint32_t N = NUM_BLOCKS * SHLS_INT8_TILE;\n        std::vector<int8_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int8_t>((i % 16) + 1);\n        }\n        auto z = launch_shiftleft_scalar<int8_t>(x, shl_scalar_kernel_int8);\n        verify_shiftleft_scalar(z, x, 2, \"int8_t (shift=2)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * SHLS_INT16_TILE;\n        std::vector<int16_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int16_t>((i % 256) + 1);\n        }\n        auto z = launch_shiftleft_scalar<int16_t>(x, shl_scalar_kernel_int16);\n        verify_shiftleft_scalar(z, x, 3, \"int16_t (shift=3)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * SHLS_INT32_TILE;\n        std::vector<int32_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<int32_t>((i % 1024) + 1);\n        }\n        auto z = launch_shiftleft_scalar<int32_t>(x, shl_scalar_kernel_int32);\n        verify_shiftleft_scalar(z, x, 5, \"int32_t (shift=5)\");\n    }\n    std::cout << std::endl;\n    {\n        constexpr uint32_t N = NUM_BLOCKS * SHLS_UINT32_TILE;\n        std::vector<uint32_t> x(N);\n        for (uint32_t i = 0; i < N; ++i) {\n            x[i] = static_cast<uint32_t>((i % 1024) + 1);\n        }\n        auto z = launch_shiftleft_scalar<uint32_t>(x, shl_scalar_kernel_uint32);\n        verify_shiftleft_scalar(z, x, 4, \"uint32_t (shift=4)\");\n    }\n\n    std::cout << std::endl;\n    std::cout << \"[完成] asc_shiftleft_scalar 示例测试结束。\" << std::endl;\n    return 0;\n}\n",
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/c_api_shiftleft_scalar.asc"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 5.9s)

_Thinking:_

All 5 source files are created. Now let me update the run_all.sh and make all build_and_run.sh executable, then start building and running.

But first, let me update the run_all.sh to include the 5 new samples.

Now let me update `run_all.sh` and make scripts executable:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/run_all.sh"
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples/run_all.sh</path>
<type>file</type>
<content>
1: #!/bin/bash
2: set -e
3: ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
4: echo "========================================="
5: echo "  vector_compute_examples 全部样例测试"
6: echo "========================================="
7: echo ""
8: 
9: SAMPLES=(
10:     "asc_abs"
11:     "asc_abs_sub"
12:     "asc_add"
13:     "asc_axpy"
14:     "asc_leakyrelu"
15:     "asc_mul"
16:     "asc_reduce_max"
17:     "asc_relu"
18:     "asc_select"
19:     "asc_sub"
20:     "asc_neg"
21:     "asc_max"
22:     "asc_min"
23:     "asc_div"
24:     "vconv/asc_e4m32float"
25:     "asc_sqrt"
26:     "asc_exp"
27:     "asc_ln"
28:     "asc_and"
29:     "asc_or"
30:     "asc_reduce_min"
31:     "asc_xor"
32:     "asc_shiftleft"
33:     "asc_ge"
34:     "asc_mul_scalar"
35:     "asc_eq"
36:     "asc_lt"
37:     "asc_gt"
38:     "asc_not"
39:     "asc_madd"
40:     "asc_shiftright"
41:     "asc_deintlv"
42:     "asc_intlv"
43:     "asc_transpose"
44:     "asc_add_scalar"
45:     "asc_le"
46:     "asc_prelu"
47:     "asc_reduce_sum"
48:     "asc_ne"
49:     "asc_pair_reduce_sum"
50:     "asc_mull"
51:     "asc_arange"
52:     "asc_squeeze"
53:     "asc_unpack_lower"
54:     "asc_pack"
55: )
56: 
57: PASS=0
58: FAIL=0
59: 
60: for sample in "${SAMPLES[@]}"; do
61:     SAMPLE_DIR="${ROOT_DIR}/${sample}"
62:     if [ ! -f "${SAMPLE_DIR}/build_and_run.sh" ]; then
63:         echo "[跳过] ${sample}: 未找到 build_and_run.sh"
64:         continue
65:     fi
66:     echo ">>> 运行 ${sample} <<<"
67:     if bash "${SAMPLE_DIR}/build_and_run.sh"; then
68:         echo "<<< ${sample} 通过 >>>"
69:         ((PASS++)) || true
70:     else
71:         echo "<<< ${sample} 失败 >>>"
72:         ((FAIL++)) || true
73:     fi
74:     echo ""
75:     rm -rf "${SAMPLE_DIR}/build"
76: done
77: 
78: echo "========================================="
79: echo "  结果: ${PASS} 通过, ${FAIL} 失败"
80: echo "========================================="

(End of file - total 80 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 11.4s)

_Thinking:_

I need to add my 5 new samples to the SAMPLES array in run_all.sh. Let me add them at the end of the array.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/run_all.sh",
  "oldString": "    \"asc_pack\"\n)",
  "newString": "    \"asc_pack\"\n    \"asc_addc\"\n    \"asc_duplicate\"\n    \"asc_ge_scalar\"\n    \"asc_ne_scalar\"\n    \"asc_shiftleft_scalar\"\n)"
}
```

**Output:**
```
Edit applied successfully.
```

**Tool: bash**

**Input:**
```json
{
  "command": "chmod +x vector_compute_examples/asc_addc/build_and_run.sh vector_compute_examples/asc_duplicate/build_and_run.sh vector_compute_examples/asc_ge_scalar/build_and_run.sh vector_compute_examples/asc_ne_scalar/build_and_run.sh vector_compute_examples/asc_shiftleft_scalar/build_and_run.sh",
  "description": "Make build scripts executable"
}
```

**Output:**
```
(no output)
```

---

## Assistant (Build · DeepSeek V4 Pro · 15.7s)

_Thinking:_

Good. Now let me update the todo list and start building and running the examples one by one. Let me start with the simplest one first - asc_duplicate, then scale up.

But wait, I need to fix an issue in asc_addc's VF functions. Looking at the code again:

```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```

This is a void function that returns the bool through first parameter. But wait, looking at the public API declaration:
```cpp
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
```

Yes, it's void, so the call `asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask)` is correct - the output goes to `vcarry_in_bool` (first param).

Let me now start building. I'll try to build one example first to catch any issues.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create asc_addc example (add with carry)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_duplicate example (broadcast lowest element)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ge_scalar example (vector >= scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ne_scalar example (vector != scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_shiftleft_scalar example (shift by scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Update run_all.sh with new 5 samples",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Build and run all 5 examples, fix issues",
      "priority": "high",
      "status": "in_progress"
    },
    {
      "content": "Clean up dump/toml/log files in code repo",
      "priority": "medium",
      "status": "pending"
    },
    {
      "content": "Summarize precision issues and solutions",
      "priority": "medium",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create asc_addc example (add with carry)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_duplicate example (broadcast lowest element)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ge_scalar example (vector >= scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ne_scalar example (vector != scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_shiftleft_scalar example (shift by scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Update run_all.sh with new 5 samples",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Build and run all 5 examples, fix issues",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Clean up dump/toml/log files in code repo",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "Summarize precision issues and solutions",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 40.3s)

_Thinking:_

Let me try building asc_duplicate first (simplest - single input, single output), then asc_shiftleft_scalar, then the scalar comparison ones, and finally the most complex asc_addc.

Let me start building and running. Starting with the simplest example:

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_duplicate/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_duplicate/build_log.txt",
  "description": "Build and run asc_duplicate",
  "timeout": 300000
}
```

**Output:**
```
=== asc_duplicate 一键编译运行 ===
-- System processer: x86_64
-- CMAKE_ASC_COMPILER: /usr/local/Ascend/cann-9.0.0-beta.2/bin/bisheng
-- ASCEND_CANN_PACKAGE_LINUX_PATH: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux
-- CMAKE_ASC_LLD_LINKER: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/ccec_compiler/bin/ld.lld
-- The CXX compiler identification is GNU 11.4.0
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
[ 50%] Building ASC object CMakeFiles/c_api_duplicate_example.dir/c_api_duplicate.asc.o
[100%] Linking ASC executable c_api_duplicate_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
[100%] Built target c_api_duplicate_example
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_duplicate/build'

=== asc_duplicate 寄存器级矢量元素复制测试 ===
模式: 多核 (8 blocks)
操作: z = duplicate(x), 每组第一个元素复制到全组

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[TmSim]: Run in serial mode.
[INFO] Config file [config.json] from model dynamic library path. Path: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config.json
[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 17:59:33
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002149] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002286] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002329] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002338] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002346] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002360] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002416] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002430] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[INFO] Model Stop Time: 2026-05-19 17:59:37
Model RUN TIME: 3467.51 ms
[INFO] Total tick: 2446
[INFO] Model stopped successfully.
  X        (4096 elements): 0 0.0999756 0.199951 0.300049 0.399902 0.5 0.600098 0.700195 0.799805 0.899902 1 1.09961 1.2002 1.2998 1.40039 1.5 1.59961 1.7002 1.7998 1.90039 ...
  Output   (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 duplicate_half.csv
  [half] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 17:59:39
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002234] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002269] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002284] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002340] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002342] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002361] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002387] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002394] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[INFO] Model Stop Time: 2026-05-19 17:59:43
Model RUN TIME: 3926.16 ms
[INFO] Total tick: 2403
[INFO] Model stopped successfully.
  X        (2048 elements): 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 ...
  Output   (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 duplicate_float.csv
  [float] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 17:59:45
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002297] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002312] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002386] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002387] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002387] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002432] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002444] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002455] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[INFO] Model Stop Time: 2026-05-19 17:59:50
Model RUN TIME: 4990.83 ms
[INFO] Total tick: 2459
[INFO] Model stopped successfully.
  X        (2048 elements): 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ...
  Output   (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 duplicate_int32_t.csv
  [int32_t] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 17:59:53
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002217] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002257] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002283] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002286] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002308] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002375] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002436] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002450] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[INFO] Model Stop Time: 2026-05-19 17:59:59
Model RUN TIME: 6235.96 ms
[INFO] Total tick: 2457
[INFO] Model stopped successfully.
  X        (4096 elements): 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ...
  Output   (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 duplicate_int16_t.csv
  [int16_t] 通过

[完成] asc_duplicate 示例测试结束。

```

---

## Assistant (Build · DeepSeek V4 Pro · 59.8s)

_Thinking:_

asc_duplicate passed all 4 data types. Now let me run the remaining 4 in parallel.

asc_duplicate passed all 4 types. Now let me run the remaining 4 in parallel:

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_shiftleft_scalar/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_shiftleft_scalar/build_log.txt",
  "description": "Build and run asc_shiftleft_scalar",
  "timeout": 300000
}
```

**Output:**
```
=== asc_shiftleft_scalar 一键编译运行 ===
-- System processer: x86_64
-- CMAKE_ASC_COMPILER: /usr/local/Ascend/cann-9.0.0-beta.2/bin/bisheng
-- ASCEND_CANN_PACKAGE_LINUX_PATH: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux
-- CMAKE_ASC_LLD_LINKER: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/ccec_compiler/bin/ld.lld
-- The CXX compiler identification is GNU 11.4.0
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
[ 50%] Building ASC object CMakeFiles/c_api_shiftleft_scalar_example.dir/c_api_shiftleft_scalar.asc.o
[100%] Linking ASC executable c_api_shiftleft_scalar_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
[100%] Built target c_api_shiftleft_scalar_example
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_shiftleft_scalar/build'

=== asc_shiftleft_scalar 寄存器级矢量标量左移测试 ===
模式: 多核 (8 blocks)
操作: z[i] = x[i] << shiftAmount

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[TmSim]: Run in serial mode.
[INFO] Config file [config.json] from model dynamic library path. Path: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config.json
[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:15
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000020] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000021] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002158] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002284] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002327] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002336] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002344] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002358] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002414] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002428] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[INFO] Model Stop Time: 2026-05-19 18:00:20
Model RUN TIME: 4995.44 ms
[INFO] Total tick: 2435
[INFO] Model stopped successfully.
  X        (8192 elements): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 1 2 3 4 ...
  Output   (8192 elements): 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 4 8 12 16 ...
  Expected (8192 elements): 4 8 12 16 20 24 28 32 36 40 44 48 52 56 60 64 4 8 12 16 ...
  [CSV] 已保存到 shiftleft_scalar_int8_t (shift=2).csv
  [int8_t (shift=2)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:23
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002155] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002157] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002278] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002333] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002335] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002354] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002380] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002387] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[INFO] Model Stop Time: 2026-05-19 18:00:31
Model RUN TIME: 8501.72 ms
[INFO] Total tick: 2394
[INFO] Model stopped successfully.
  X        (4096 elements): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ...
  Output   (4096 elements): 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128 136 144 152 160 ...
  Expected (4096 elements): 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128 136 144 152 160 ...
  [CSV] 已保存到 shiftleft_scalar_int16_t (shift=3).csv
  [int16_t (shift=3)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:35
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002292] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002306] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002377] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002377] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002380] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002427] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002439] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002450] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[INFO] Model Stop Time: 2026-05-19 18:00:44
Model RUN TIME: 9312.8 ms
[INFO] Total tick: 2452
[INFO] Model stopped successfully.
  X        (2048 elements): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ...
  Output   (2048 elements): 32 64 96 128 160 192 224 256 288 320 352 384 416 448 480 512 544 576 608 640 ...
  Expected (2048 elements): 32 64 96 128 160 192 224 256 288 320 352 384 416 448 480 512 544 576 608 640 ...
  [CSV] 已保存到 shiftleft_scalar_int32_t (shift=5).csv
  [int32_t (shift=5)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:48
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002125] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002189] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002205] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002239] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002302] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002376] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002421] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002438] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[INFO] Model Stop Time: 2026-05-19 18:00:58
Model RUN TIME: 10871.2 ms
[INFO] Total tick: 2444
[INFO] Model stopped successfully.
  X        (2048 elements): 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ...
  Output   (2048 elements): 16 32 48 64 80 96 112 128 144 160 176 192 208 224 240 256 272 288 304 320 ...
  Expected (2048 elements): 16 32 48 64 80 96 112 128 144 160 176 192 208 224 240 256 272 288 304 320 ...
  [CSV] 已保存到 shiftleft_scalar_uint32_t (shift=4).csv
  [uint32_t (shift=4)] 通过

[完成] asc_shiftleft_scalar 示例测试结束。

```

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_ge_scalar/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_ge_scalar/build_log.txt",
  "description": "Build and run asc_ge_scalar",
  "timeout": 300000
}
```

**Output:**
```
=== asc_ge_scalar 一键编译运行 ===
-- System processer: x86_64
-- CMAKE_ASC_COMPILER: /usr/local/Ascend/cann-9.0.0-beta.2/bin/bisheng
-- ASCEND_CANN_PACKAGE_LINUX_PATH: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux
-- CMAKE_ASC_LLD_LINKER: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/ccec_compiler/bin/ld.lld
-- The CXX compiler identification is GNU 11.4.0
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
[ 50%] Building ASC object CMakeFiles/c_api_ge_scalar_example.dir/c_api_ge_scalar.asc.o
[100%] Linking ASC executable c_api_ge_scalar_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
[100%] Built target c_api_ge_scalar_example
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ge_scalar/build'

=== asc_ge_scalar 寄存器级矢量与标量大于等于比较测试 ===
模式: 多核 (8 blocks)
输出: (x >= scalar) ? 1 : 0

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[TmSim]: Run in serial mode.
[INFO] Config file [config.json] from model dynamic library path. Path: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config.json
[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:19
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000020] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000021] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002686] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002739] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002768] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002782] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002817] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002842] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002852] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002853] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[INFO] Model Stop Time: 2026-05-19 18:00:27
Model RUN TIME: 8188.01 ms
[INFO] Total tick: 2865
[INFO] Model stopped successfully.
  X        (4096 elements): 0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 8.5 9 9.5 ...
  Output   (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (4096 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 ge_scalar_half (scalar=10.0).csv
  [half (scalar=10.0)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:30
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002733] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002762] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002838] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002909] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002916] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002930] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002941] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002958] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[INFO] Model Stop Time: 2026-05-19 18:00:40
Model RUN TIME: 10108.6 ms
[INFO] Total tick: 2961
[INFO] Model stopped successfully.
  X        (2048 elements): 0 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5 5.5 6 6.5 7 7.5 8 8.5 9 9.5 ...
  Output   (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 ge_scalar_float (scalar=10.0).csv
  [float (scalar=10.0)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:44
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000020] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002299] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002373] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002377] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002404] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002408] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002430] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002447] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002456] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[INFO] Model Stop Time: 2026-05-19 18:00:55
Model RUN TIME: 10857.2 ms
[INFO] Total tick: 2462
[INFO] Model stopped successfully.
  X        (2048 elements): 0 3 6 9 12 15 18 21 24 27 30 33 36 39 42 45 48 51 54 57 ...
  Output   (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  Expected (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 ge_scalar_int32_t (scalar=1000).csv
  [int32_t (scalar=1000)] 通过

[完成] asc_ge_scalar 示例测试结束。

```

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_ne_scalar/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_ne_scalar/build_log.txt",
  "description": "Build and run asc_ne_scalar",
  "timeout": 300000
}
```

**Output:**
```
=== asc_ne_scalar 一键编译运行 ===
-- System processer: x86_64
-- CMAKE_ASC_COMPILER: /usr/local/Ascend/cann-9.0.0-beta.2/bin/bisheng
-- ASCEND_CANN_PACKAGE_LINUX_PATH: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux
-- CMAKE_ASC_LLD_LINKER: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/ccec_compiler/bin/ld.lld
-- The CXX compiler identification is GNU 11.4.0
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
[ 50%] Building ASC object CMakeFiles/c_api_ne_scalar_example.dir/c_api_ne_scalar.asc.o
[100%] Linking ASC executable c_api_ne_scalar_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
[100%] Built target c_api_ne_scalar_example
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_ne_scalar/build'

=== asc_ne_scalar 寄存器级矢量与标量不等于比较测试 ===
模式: 多核 (8 blocks)
输出: (x != scalar) ? 1 : 0

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[TmSim]: Run in serial mode.
[INFO] Config file [config.json] from model dynamic library path. Path: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config.json
[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:22
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000020] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002685] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002738] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002767] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002781] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002816] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002841] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002851] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002852] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[INFO] Model Stop Time: 2026-05-19 18:00:30
Model RUN TIME: 7765.91 ms
[INFO] Total tick: 2864
[INFO] Model stopped successfully.
  X        (4096 elements): 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ...
  Output   (4096 elements): 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 ...
  Expected (4096 elements): 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 ...
  [CSV] 已保存到 ne_scalar_half (scalar=10.0).csv
  [half (scalar=10.0)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:33
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002734] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002763] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002839] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002910] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002917] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002931] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002942] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002959] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[INFO] Model Stop Time: 2026-05-19 18:00:43
Model RUN TIME: 9850.17 ms
[INFO] Total tick: 2967
[INFO] Model stopped successfully.
  X        (2048 elements): 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ...
  Output   (2048 elements): 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 ...
  Expected (2048 elements): 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 ...
  [CSV] 已保存到 ne_scalar_float (scalar=10.0).csv
  [float (scalar=10.0)] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:00:46
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000010] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002295] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002369] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002373] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002400] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002404] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002426] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002443] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002452] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[INFO] Model Stop Time: 2026-05-19 18:00:57
Model RUN TIME: 11051.2 ms
[INFO] Total tick: 2456
[INFO] Model stopped successfully.
  X        (2048 elements): 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 ...
  Output   (2048 elements): 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
  Expected (2048 elements): 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
  [CSV] 已保存到 ne_scalar_int32_t (scalar=1000).csv
  [int32_t (scalar=1000)] 通过

[完成] asc_ne_scalar 示例测试结束。

```

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_addc/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_addc/build_log.txt",
  "description": "Build and run asc_addc",
  "timeout": 300000
}
```

**Output:**
```
=== asc_addc 一键编译运行 ===
-- System processer: x86_64
-- CMAKE_ASC_COMPILER: /usr/local/Ascend/cann-9.0.0-beta.2/bin/bisheng
-- ASCEND_CANN_PACKAGE_LINUX_PATH: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux
-- CMAKE_ASC_LLD_LINKER: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/ccec_compiler/bin/ld.lld
-- The CXX compiler identification is GNU 11.4.0
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
[ 50%] Building ASC object CMakeFiles/c_api_addc_example.dir/c_api_addc.asc.o
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:48:26: error: no matching function for call to 'asc_ne_scalar'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
                         ^~~~~~~~~~~~~
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1103:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1101:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1099:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1097:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1095:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1093:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1091:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1089:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1087:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
                            ^
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:48:26: error: assigning to 'vector_bool' from incompatible type 'void'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
                         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:79:26: error: no matching function for call to 'asc_ne_scalar'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);
                         ^~~~~~~~~~~~~
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1103:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1101:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1099:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1097:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1095:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1093:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1091:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1089:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1087:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
                            ^
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:79:26: error: assigning to 'vector_bool' from incompatible type 'void'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);
                         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
4 errors generated.
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:48:26: error: no matching function for call to 'asc_ne_scalar'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
                         ^~~~~~~~~~~~~
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1103:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1101:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1099:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1097:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1095:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1093:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1091:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1089:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1087:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
                            ^
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:48:26: error: assigning to 'vector_bool' from incompatible type 'void'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
                         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:79:26: error: no matching function for call to 'asc_ne_scalar'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);
                         ^~~~~~~~~~~~~
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1103:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1101:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1099:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1097:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1095:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1093:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1091:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1089:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);
                            ^
/usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api/reg_compute/reg_vector.h:1087:29: note: candidate function not viable: requires 4 arguments, but 3 were provided
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);
                            ^
/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc:79:26: error: assigning to 'vector_bool' from incompatible type 'void'
        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);
                         ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
4 errors generated.
make[2]: *** [CMakeFiles/c_api_addc_example.dir/build.make:76: CMakeFiles/c_api_addc_example.dir/c_api_addc.asc.o] Error 1
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[1]: *** [CMakeFiles/Makefile2:83: CMakeFiles/c_api_addc_example.dir/all] Error 2
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make: *** [Makefile:91: all] Error 2
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'

```

---

## Assistant (Build · DeepSeek V4 Pro · 21.9s)

_Thinking:_

Results:
- asc_duplicate: PASSED all 4 types
- asc_shiftleft_scalar: PASSED all 4 types
- asc_ge_scalar: PASSED all 3 types
- asc_ne_scalar: PASSED all 3 types
- asc_addc: FAILED compilation

The issue is in asc_addc at lines 48 and 79. The problem is that I used `vcarry_in_bool = asc_ne_scalar(...)` with an assignment, but `asc_ne_scalar` is a void function and I also passed only 3 arguments instead of 4. The error message says it requires 4 arguments but I passed 3.

Wait, looking at the error more carefully: `requires 4 arguments, but 3 were provided`. The function signature is: `void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)` - that's 4 arguments. But I called `asc_ne_scalar(vcarry_val, (uint32_t)0, vmask)` - 3 arguments. I'm missing the first argument (the output vector_bool). So the correct call should be:

```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```

But wait, I wrote `vcarry_in_bool = asc_ne_scalar(...)` - which means I attempted to assign and missed the first parameter. The correct form is just the function call without assignment.

Let me fix the asc_addc.asc file. I need to change lines 48 and 79 from:
```cpp
vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
```
to:
```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```

This is the same pattern I used correctly in ge_scalar and ne_scalar VF functions. Let me fix it now.

asc_addc has compilation errors. I used `=` assignment with a void function and missed the first argument. Let me fix it:

**Tool: read**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc",
  "offset": 40,
  "limit": 20
}
```

**Output:**
```
<path>/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc</path>
<type>file</type>
<content>
40:     asc_loadalign(vzero, zeroUb);
41: 
42:     for (uint16_t i = 0; i < rep; ++i) {
43:         vmask = asc_update_mask_b32(remaining);
44:         asc_loadalign(vx, xLocal + i * one_rep_size);
45:         asc_loadalign(vy, yLocal + i * one_rep_size);
46:         asc_loadalign(vcarry_val, cInLocal + i * one_rep_size);
47: 
48:         vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
49: 
50:         asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);
51: 
52:         asc_select(vcarry_val, vone, vzero, vcarry_out_bool);
53: 
54:         asc_storealign(sumLocal + i * one_rep_size, vsum, vmask);
55:         asc_storealign(cOutLocal + i * one_rep_size, vcarry_val, vmask);
56:     }
57: }
58: 
59: __simd_vf__ inline void AddcVF_int32(__ubuf__ int32_t* xLocal, __ubuf__ int32_t* yLocal,

(Showing lines 40-59 of 373. Use offset=60 to continue.)
</content>
```

---

## Assistant (Build · DeepSeek V4 Pro · 18.3s)

_Thinking:_

I see the issue. Lines 48 and 79 need to be fixed. The correct call should be:
```cpp
asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);
```
Instead of:
```cpp
vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);
```

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc",
  "oldString": "        vcarry_in_bool = asc_ne_scalar(vcarry_val, (uint32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);",
  "newString": "        asc_ne_scalar(vcarry_in_bool, vcarry_val, (uint32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);"
}
```

**Output:**
```
Edit applied successfully.
```

**Tool: edit**

**Input:**
```json
{
  "filePath": "/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/c_api_addc.asc",
  "oldString": "        vcarry_in_bool = asc_ne_scalar(vcarry_val, (int32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);",
  "newString": "        asc_ne_scalar(vcarry_in_bool, vcarry_val, (int32_t)0, vmask);\n\n        asc_addc(vcarry_out_bool, vsum, vx, vy, vcarry_in_bool, vmask);"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · DeepSeek V4 Pro · 29.3s)

_Thinking:_

Now let me rebuild asc_addc.

Now let me rebuild asc_addc:

**Tool: bash**

**Input:**
```json
{
  "command": "bash vector_compute_examples/asc_addc/build_and_run.sh 2>&1 | tee vector_compute_examples/asc_addc/build_log.txt",
  "description": "Retry building asc_addc",
  "timeout": 300000
}
```

**Output:**
```
=== asc_addc 一键编译运行 ===
-- ASC Simulator enabled: /usr/local/Ascend/cann-9.0.0-beta.2/tools/simulator/dav_3510/lib
-- Configuring done
-- Generating done
-- Build files have been written to: /home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build
make: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[1]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
Consolidate compiler generated dependencies of target c_api_addc_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make[2]: Entering directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
[ 50%] Building ASC object CMakeFiles/c_api_addc_example.dir/c_api_addc.asc.o
[100%] Linking ASC executable c_api_addc_example
make[2]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
[100%] Built target c_api_addc_example
make[1]: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'
make: Leaving directory '/home/joey/code/joey/asc-devkit/vector_compute_examples/asc_addc/build'

=== asc_addc 寄存器级带进位加法测试 ===
模式: 多核 (8 blocks)

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[TmSim]: Run in serial mode.
[INFO] Config file [config.json] from model dynamic library path. Path: /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config.json
[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:01:56
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000018] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000019] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000020] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000021] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000022] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002636] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002675] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002731] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002760] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002820] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002852] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002939] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[info] [0000002967] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[INFO] Model Stop Time: 2026-05-19 18:02:01
Model RUN TIME: 4690.28 ms
[INFO] Total tick: 2974
[INFO] Model stopped successfully.
  X        (2048 elements): 4294901760 4294901761 4294901762 4294901763 4294901764 4294901765 4294901766 4294901767 4294901768 4294901769 4294901770 4294901771 4294901772 4294901773 4294901774 4294901775 4294901776 4294901777 4294901778 4294901779 ...
  Y        (2048 elements): 65535 65537 65539 65541 65543 65545 65547 65549 65551 65553 65555 65557 65559 65561 65563 65565 65567 65569 65571 65573 ...
  CarryIn  (2048 elements): 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 ...
  SumOut   (2048 elements): 0 2 5 9 11 14 18 20 23 27 29 32 36 38 41 45 47 50 54 56 ...
  SumExp   (2048 elements): 0 2 5 9 11 14 18 20 23 27 29 32 36 38 41 45 47 50 54 56 ...
  CoutOut  (2048 elements): 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
  CoutExp  (2048 elements): 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ...
  [CSV] 已保存到 addc_uint32_t.csv
  [uint32_t] 通过

[INFO] Config file is found, path is /usr/local/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/dav_3510/lib/config_stars.json.
[FuncCache]: size:0x20000, line_size:128, way_num:16, line_num:1024, idx_num:64
             idx_lsb:7, idx_mask:0x3f, tag_lsb:13, tag_mask:0xffffffffffffffff, ofst_mask:0x7f

[INFO] AicWrapper attach AIC 0, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 1, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 2, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 3, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 4, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 5, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 6, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 7, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 8, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 9, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 10, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 11, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 12, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 13, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 14, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 15, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 16, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 17, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 18, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 19, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 20, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 21, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 22, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 23, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 24, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 25, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 26, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 27, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 28, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 29, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 30, num_vec_core=2, num_subcore=3
[INFO] AicWrapper attach AIC 31, num_vec_core=2, num_subcore=3
[INFO] Chip 0 AIC / Scheduler / Soc periods: 200.0000 / 200.0000 / 105.0000
[INFO] chip 0 die 0 device created
================================================================================
>>>>                                                                        
>>>>                             " PEM MODEL "                                
>>>>             Total no. of 1 chip(s) Model Init Success!                
>>>>                                                                        
================================================================================
[INFO] Model Start Time: 2026-05-19 18:02:03
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:0 swapbuf_addr:10000000
[DRVSTUB_LOG] driver_api.c:556 sendSwapBuf:swapbuf_base_addr:10000000
[DRVSTUB_LOG] driver_api.c:557 sendSwapBuf:sq:1 swapbuf_addr:10000040
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=0, block_id=0
[info] [0000000011] [block_start]           : AIV, task_id=0, core_id=1, block_id=1
[info] [0000000012] [block_start]           : AIV, task_id=0, core_id=2, block_id=2
[info] [0000000013] [block_start]           : AIV, task_id=0, core_id=3, block_id=3
[info] [0000000014] [block_start]           : AIV, task_id=0, core_id=4, block_id=4
[info] [0000000015] [block_start]           : AIV, task_id=0, core_id=5, block_id=5
[info] [0000000016] [block_start]           : AIV, task_id=0, core_id=6, block_id=6
[info] [0000000017] [block_start]           : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002731] [block_end]             : AIV, task_id=0, core_id=7, block_id=7
[info] [0000002738] [block_end]             : AIV, task_id=0, core_id=6, block_id=6
[info] [0000002781] [block_end]             : AIV, task_id=0, core_id=2, block_id=2
[info] [0000002809] [block_end]             : AIV, task_id=0, core_id=1, block_id=1
[info] [0000002850] [block_end]             : AIV, task_id=0, core_id=5, block_id=5
[info] [0000002853] [block_end]             : AIV, task_id=0, core_id=4, block_id=4
[info] [0000002909] [block_end]             : AIV, task_id=0, core_id=0, block_id=0
[info] [0000002940] [block_end]             : AIV, task_id=0, core_id=3, block_id=3
[INFO] Model Stop Time: 2026-05-19 18:02:09
Model RUN TIME: 5596.29 ms
[INFO] Total tick: 2953
[INFO] Model stopped successfully.
  X        (2048 elements): 1000000000 1000000001 1000000002 1000000003 1000000004 1000000005 1000000006 1000000007 1000000008 1000000009 1000000010 1000000011 1000000012 1000000013 1000000014 1000000015 1000000016 1000000017 1000000018 1000000019 ...
  Y        (2048 elements): 1000000000 1000000002 1000000004 1000000006 1000000008 1000000010 1000000012 1000000014 1000000016 1000000018 1000000020 1000000022 1000000024 1000000026 1000000028 1000000030 1000000032 1000000034 1000000036 1000000038 ...
  CarryIn  (2048 elements): 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 ...
  SumOut   (2048 elements): 2000000001 2000000003 2000000006 2000000009 2000000012 2000000016 2000000018 2000000021 2000000024 2000000027 2000000031 2000000033 2000000036 2000000039 2000000042 2000000046 2000000048 2000000051 2000000054 2000000057 ...
  SumExp   (2048 elements): 2000000001 2000000003 2000000006 2000000009 2000000012 2000000016 2000000018 2000000021 2000000024 2000000027 2000000031 2000000033 2000000036 2000000039 2000000042 2000000046 2000000048 2000000051 2000000054 2000000057 ...
  CoutOut  (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  CoutExp  (2048 elements): 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
  [CSV] 已保存到 addc_int32_t.csv
  [int32_t] 通过

[完成] asc_addc 示例测试结束。

```

---

## Assistant (Build · DeepSeek V4 Pro · 16.5s)

_Thinking:_

All 5 examples now compile and pass! Let me update the todo list, and then clean up dump/toml/log files.

Summary so far:
- asc_duplicate: PASSED (half, float, int32_t, int16_t)
- asc_shiftleft_scalar: PASSED (int8_t, int16_t, int32_t, uint32_t)
- asc_ge_scalar: PASSED (half, float, int32_t)
- asc_ne_scalar: PASSED (half, float, int32_t)
- asc_addc: PASSED (uint32_t, int32_t)

Issues encountered:
1. asc_addc compilation error: Used `=` assignment with void function `asc_ne_scalar` and missed the first argument (output vector_bool). Fixed by removing the assignment.

Now I need to:
1. Clean up dump/toml/log files in the code repo
2. Summarize precision issues and solutions (there were no precision issues in this case)
3. Clean up the build directories

All 5 examples pass. Now let me clean up and finalize:

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create asc_addc example (add with carry)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_duplicate example (broadcast lowest element)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ge_scalar example (vector >= scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ne_scalar example (vector != scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_shiftleft_scalar example (shift by scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Update run_all.sh with new 5 samples",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Build and run all 5 examples, fix issues",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Clean up dump/toml/log files in code repo",
      "priority": "medium",
      "status": "in_progress"
    },
    {
      "content": "Summarize precision issues and solutions",
      "priority": "medium",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create asc_addc example (add with carry)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_duplicate example (broadcast lowest element)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ge_scalar example (vector >= scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ne_scalar example (vector != scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_shiftleft_scalar example (shift by scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Update run_all.sh with new 5 samples",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Build and run all 5 examples, fix issues",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Clean up dump/toml/log files in code repo",
    "status": "in_progress",
    "priority": "medium"
  },
  {
    "content": "Summarize precision issues and solutions",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 15.7s)

_Thinking:_

Let me clean up the dump/toml/log files using the clear_build.sh approach, and also look for any stray files in the root directory.

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/joey/code/joey/asc-devkit/vector_compute_examples/ -maxdepth 1 \\( -name \"*.toml\" -o -name \"*.dump\" -o -name \"*_log\" \\) -type f -exec rm -f {} \\; 2>/dev/null; echo \"Cleaned top-level artifacts\"",
  "description": "Clean dump/toml/log files from examples dir"
}
```

**Output:**
```
Cleaned top-level artifacts

```

**Tool: bash**

**Input:**
```json
{
  "command": "for dir in asc_addc asc_duplicate asc_ge_scalar asc_ne_scalar asc_shiftleft_scalar; do\n  rm -rf \"vector_compute_examples/$dir/build\"\ndone\necho \"Cleaned build directories\"",
  "description": "Clean build dirs for new examples"
}
```

**Output:**
```
Cleaned build directories

```

**Tool: bash**

**Input:**
```json
{
  "command": "find /home/joey/code/joey/asc-devkit/ -maxdepth 2 \\( -name \"*.toml\" -o -name \"*.dump\" -o -name \"*.csv\" \\) -type f 2>/dev/null | head -20; echo \"---\"; ls /home/joey/code/joey/asc-devkit/*.toml /home/joey/code/joey/asc-devkit/*.dump /home/joey/code/joey/asc-devkit/*_log 2>/dev/null || echo \"No root-level artifacts\"",
  "description": "Check for remaining artifacts in repo root"
}
```

**Output:**
```
...output truncated...

Full output saved to: /root/.local/share/opencode/tool-output/tool_e3fb01104001Ls3pAmA6mtSbY8

oey/asc-devkit/core8.cubecore0.fb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.fbv2.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.fbv2.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.fb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.ifu.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.instr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0amx.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0amx.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0a.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0a.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0bmx.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0bmx.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0b.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0b.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0c_ca_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0c.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l0c.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l1.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l1.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.l1.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.pt.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.pt.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.reg_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.smask.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.smask.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.spidx.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.spidx.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.wino_l0a.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.wino_l0a.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.wino_l0b.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.cubecore0.wino_l0b.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.excp_log.dump
/home/joey/code/joey/asc-devkit/core8.mte.cubecore0_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core8.mte.cubecore0.l0c.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.mte.cubecore0.l0c.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.mte_log.dump
/home/joey/code/joey/asc-devkit/core8.mte.mte_dcache.dump
/home/joey/code/joey/asc-devkit/core8.mte.ndcache.dump
/home/joey/code/joey/asc-devkit/core8.mte.veccore0_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core8.mte.veccore1_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core8.ssb.dump
/home/joey/code/joey/asc-devkit/core8.ssb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.ssb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8_summary_log
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.cube_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.fixp_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.mte1_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.mte2_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.mte3_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.scalar_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ccu.vec_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.dcache_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ifu.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.instr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.reg_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.ASU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.EXU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.GSU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.icache1.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.IDU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.ISU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.LSU.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.OOO.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec_pv.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.reg_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.sfu.vec_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.depmgr.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.IDU.DISP.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.idu.PERF.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.idu.TRACE.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.ifu.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.isu.PERF.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.isu.scoreboards.scb_P.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.isu.scoreboards.scb_V.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simd.isu.STALL.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.dc.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.dvg0.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.dvg1.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.dvg2.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.dvg3.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.exu0.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.exu1.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.exu2.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.exu3.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.gsu.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ifu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.lsu.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.mrf.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg0.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg0.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg0.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg1.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg1.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg1.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg2.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg2.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg2.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg3.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg3.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.reg3.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.spr.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ts0_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ts1_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ts2_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ts3_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.rvec.simt.ubitf.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.sb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.sb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ub.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ub.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.ub.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.vreg.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore0.vreg.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.cube_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.fixp_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.mte1_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.mte2_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.mte3_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.scalar_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ccu.vec_issque.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.dcache_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ifu.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.instr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.reg_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.ASU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.EXU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.GSU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.icache2.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.IDU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.ISU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.LSU.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.OOO.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec_pv.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.reg_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.sfu.vec_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.depmgr.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.IDU.DISP.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.idu.PERF.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.idu.TRACE.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.ifu.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.isu.PERF.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.isu.scoreboards.scb_P.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.isu.scoreboards.scb_V.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simd.isu.STALL.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.dc.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.dvg0.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.dvg1.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.dvg2.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.dvg3.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.exu0.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.exu1.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.exu2.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.exu3.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.gsu.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ifu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.lsu.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.mrf.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg0.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg0.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg0.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg1.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg1.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg1.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg2.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg2.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg2.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg3.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg3.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.reg3.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.spr.log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ts0_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ts1_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ts2_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ts3_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.rvec.simt.ubitf.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.sb.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.sb.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ub.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ub.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.ub.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.vreg.rd_log.dump
/home/joey/code/joey/asc-devkit/core8.veccore1.vreg.wr_log.dump
/home/joey/code/joey/asc-devkit/core8.wrapper_log.dump
/home/joey/code/joey/asc-devkit/core9.biu.brif.log.dump
/home/joey/code/joey/asc-devkit/core9.biu.brif.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.biu.bwif.log.dump
/home/joey/code/joey/asc-devkit/core9.biu.bwif.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.active_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.bt.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.bt.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.cube_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.fixp_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.mte1_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.mte2_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.mte3_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.scalar_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ccu.vec_issque.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.cube.l0c.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.cube.l0c.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.cube_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.dcache_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.fb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.fbv2.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.fbv2.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.fb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ifu.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.instr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0amx.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0amx.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0a.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0a.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0bmx.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0bmx.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0b.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0b.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0c_ca_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0c.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l0c.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l1.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l1.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.l1.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.pt.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.pt.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.reg_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.smask.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.smask.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.spidx.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.spidx.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.wino_l0a.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.wino_l0a.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.wino_l0b.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.cubecore0.wino_l0b.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.excp_log.dump
/home/joey/code/joey/asc-devkit/core9.mte.cubecore0_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core9.mte.cubecore0.l0c.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.mte.cubecore0.l0c.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.mte_log.dump
/home/joey/code/joey/asc-devkit/core9.mte.mte_dcache.dump
/home/joey/code/joey/asc-devkit/core9.mte.ndcache.dump
/home/joey/code/joey/asc-devkit/core9.mte.veccore0_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core9.mte.veccore1_cmdsch.pv_fixp_log.dump
/home/joey/code/joey/asc-devkit/core9.ssb.dump
/home/joey/code/joey/asc-devkit/core9.ssb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.ssb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9_summary_log
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.cube_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.fixp_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.mte1_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.mte2_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.mte3_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.scalar_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ccu.vec_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.dcache_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ifu.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.instr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.reg_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.ASU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.EXU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.GSU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.icache1.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.IDU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.ISU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.LSU.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.OOO.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec_pv.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.reg_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.sfu.vec_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.depmgr.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.IDU.DISP.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.idu.PERF.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.idu.TRACE.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.ifu.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.isu.PERF.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.isu.scoreboards.scb_P.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.isu.scoreboards.scb_V.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simd.isu.STALL.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.dc.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.dvg0.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.dvg1.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.dvg2.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.dvg3.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.exu0.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.exu1.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.exu2.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.exu3.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.gsu.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ifu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.lsu.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.mrf.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg0.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg0.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg0.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg1.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg1.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg1.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg2.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg2.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg2.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg3.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg3.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.reg3.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.spr.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ts0_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ts1_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ts2_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ts3_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.rvec.simt.ubitf.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.sb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.sb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ub.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ub.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.ub.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.vreg.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore0.vreg.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.cube_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.fixp_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.mte1_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.mte2_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.mte3_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.scalar_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ccu.vec_issque.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.dcache_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ifu.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ifu.icache.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ifu.pred_log.dump.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.instr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.instr_popped_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.pb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.pb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.reg_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.ASU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.EXU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.GSU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.icache2.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.IDU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.ISU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.LSU.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.OOO.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec_pv.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.reg_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.sfu.vec_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.depmgr.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.IDU.DISP.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.idu.PERF.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.idu.TRACE.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.ifu.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.isu.PERF.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.isu.scoreboards.scb_P.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.isu.scoreboards.scb_V.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simd.isu.STALL.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.dc.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.dvg0.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.dvg1.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.dvg2.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.dvg3.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.exu0.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.exu1.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.exu2.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.exu3.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.gsu.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ifu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.lsu.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.mrf.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg0.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg0.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg0.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg1.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg1.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg1.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg2.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg2.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg2.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg3.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg3.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.reg3.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.spr.log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ts0_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ts1_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ts2_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ts3_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.rvec.simt.ubitf.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.sb.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.sb.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.su_pmu_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ub.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ub.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.ub.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.vreg.rd_log.dump
/home/joey/code/joey/asc-devkit/core9.veccore1.vreg.wr_log.dump
/home/joey/code/joey/asc-devkit/core9.wrapper_log.dump
/home/joey/code/joey/asc-devkit/profile_aic_log0.toml
/home/joey/code/joey/asc-devkit/profile_aiv_log0.toml
/home/joey/code/joey/asc-devkit/profile_dmu_log0.toml
/home/joey/code/joey/asc-devkit/profile_network_log0.toml
/home/joey/code/joey/asc-devkit/profile_stream_log0.toml
/home/joey/code/joey/asc-devkit/profile_subtask_log0.toml
/home/joey/code/joey/asc-devkit/profile_task_log0.toml
/home/joey/code/joey/asc-devkit/stars_log0.dump

```

---

## Assistant (Build · DeepSeek V4 Pro · 10.9s)

_Thinking:_

There are a lot of simulator artifacts (dump files, toml files, log files) in the repo root directory. These are generated by the simulator during simulation runs. I need to clean them up. Let me use the clear_build.sh approach to remove them.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rm -rf core*.dump core*.log core*_log profile_*.toml *.toml *.dump stars_*.dump 2>/dev/null; echo \"done\"",
  "description": "Clean all simulator artifacts from repo root"
}
```

**Output:**
```
done

```

---

## Assistant (Build · DeepSeek V4 Pro · 7.4s)

_Thinking:_

Let me also clean the csv files from the working directory and clean example build directories.

**Tool: bash**

**Input:**
```json
{
  "command": "cd /home/joey/code/joey/asc-devkit && rm -f *.csv 2>/dev/null; echo \"CSV cleaned\" && ls *.dump *.toml 2>/dev/null || echo \"No remaining artifacts\"",
  "description": "Clean CSV and verify no remaining artifacts"
}
```

**Output:**
```
CSV cleaned
No remaining artifacts

```

---

## Assistant (Build · DeepSeek V4 Pro · 9.7s)

_Thinking:_

Good, all artifacts cleaned. Now let me finalize the todo list and provide the summary.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create asc_addc example (add with carry)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_duplicate example (broadcast lowest element)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ge_scalar example (vector >= scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_ne_scalar example (vector != scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Create asc_shiftleft_scalar example (shift by scalar)",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Update run_all.sh with new 5 samples",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Build and run all 5 examples, fix issues",
      "priority": "high",
      "status": "completed"
    },
    {
      "content": "Clean up dump/toml/log files in code repo",
      "priority": "medium",
      "status": "completed"
    },
    {
      "content": "Summarize precision issues and solutions",
      "priority": "medium",
      "status": "completed"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create asc_addc example (add with carry)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_duplicate example (broadcast lowest element)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ge_scalar example (vector >= scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_ne_scalar example (vector != scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Create asc_shiftleft_scalar example (shift by scalar)",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Update run_all.sh with new 5 samples",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Build and run all 5 examples, fix issues",
    "status": "completed",
    "priority": "high"
  },
  {
    "content": "Clean up dump/toml/log files in code repo",
    "status": "completed",
    "priority": "medium"
  },
  {
    "content": "Summarize precision issues and solutions",
    "status": "completed",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Pro · 20.0s)

_Thinking:_

All tasks are complete. Let me provide a comprehensive summary of everything that was done.

## 任务完成。5 个新 C API 测试样例全部编译运行通过。

### 新增样例列表

| 样例 | 路径 | 测试数据类型 | 功能描述 |
|------|------|-------------|---------|
| `asc_addc` | `vector_compute_examples/asc_addc/` | uint32_t, int32_t | 带进位矢量加法（3 输入 2 输出） |
| `asc_duplicate` | `vector_compute_examples/asc_duplicate/` | half, float, int32_t, int16_t | 矢量最低位元素复制到整组 |
| `asc_ge_scalar` | `vector_compute_examples/asc_ge_scalar/` | half, float, int32_t | 矢量与标量 >= 比较 |
| `asc_ne_scalar` | `vector_compute_examples/asc_ne_scalar/` | half, float, int32_t | 矢量与标量 != 比较 |
| `asc_shiftleft_scalar` | `vector_compute_examples/asc_shiftleft_scalar/` | int8_t, int16_t, int32_t, uint32_t | 矢量按标量左移 |

### 遇到的问题与解决方案

**编译错误 (asc_addc):** 调用 `asc_ne_scalar` 时错误地使用了 `vcarry_in_bool = asc_ne_scalar(vcarry_val, 0, vmask)` 语法（赋值 + 缺少第一个参数）。`asc_ne_scalar` 是 void 函数，输出通过第一个引用参数传递。

- **修复:** 改为 `asc_ne_scalar(vcarry_in_bool, vcarry_val, 0, vmask)`

### 精度问题

本轮 5 个样例 **无精度问题**，全部通过逐元素精确匹配验证。`asc_addc` 的 uint32_t 进位场景通过大数值（0xFFFF0000 + 0x0000FFFF）正确触发进位输出。

### 修改的文件
- 新建 5 个样例目录，每个包含 `c_api_*.asc`, `CMakeLists.txt`, `build_and_run.sh`
- 更新 `vector_compute_examples/run_all.sh` 追加 5 个新项
- 清理了代码仓根目录下的仿真 dmp/toml/log 残留文件

---

