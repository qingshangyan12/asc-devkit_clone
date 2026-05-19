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
