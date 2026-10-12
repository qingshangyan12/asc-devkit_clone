# asc_add

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/vector_compute/compute/vector_arith.h"`。

执行矢量加法运算。计算公式如下：

$$
dst_i = src0_i + src1_i
$$

## 函数原型

- 前n个数据计算

```cpp
__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
```

- 高维切分计算

```cpp
__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                               uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                               uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                               uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                               uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                               uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
```

## 参数说明

**表1** 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数。            |
| src0、src1 | 输入    | 源操作数（矢量）的起始地址。             |
| repeat     | 输入    | 迭代次数。        |
| dst_block_stride |输入| 目的操作数单次迭代内不同DataBlock间地址步长。 |
| src0_block_stride |输入| 源操作数0单次迭代内不同DataBlock间地址步长。 |
| src1_block_stride |输入| 源操作数1单次迭代内不同DataBlock间地址步长。 |
| dst_repeat_stride |输入| 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| src0_repeat_stride |输入| 源操作数0相邻迭代间相同DataBlock的地址步长。 |
| src1_repeat_stride |输入| 源操作数1相邻迭代间相同DataBlock的地址步长。 |
| count     | 输入    | 参与计算的元素个数。        |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。
- dst、src0、src1的起始地址需要32字节对齐。

## 调用示例

```cpp
#include <cstdlib>
#include <iostream>
#include <vector>

#include "acl/acl.h"
#include "c_api/asc_simd.h"

#define CHECK_ACL(x)                                                                        \
    do {                                                                                    \
        aclError __ret = x;                                                                 \
        if (__ret != ACL_ERROR_NONE) {                                                      \
            std::cerr << __FILE__ << ":" << __LINE__ << " aclError:" << __ret << std::endl; \
        }                                                                                   \
    } while (0);

#define CHECK_NOT_NULL(ptr) do { \
    if ((ptr) == nullptr) { \
        std::cerr << #ptr << " is nullptr" << std::endl; \
        std::exit(1); \
    } \
} while (0)

constexpr uint64_t TOTAL_LENGTH = 64;
constexpr uint64_t DATA_BYTES = TOTAL_LENGTH * sizeof(half);

__global__ __vector__ void add_kernel(__gm__ half* dst, __gm__ half* src0, __gm__ half* src1)
{
    asc_init();
    __ubuf__ half src0_ub[TOTAL_LENGTH];
    __ubuf__ half src1_ub[TOTAL_LENGTH];
    __ubuf__ half dst_ub[TOTAL_LENGTH];

    asc_copy_gm2ub_align(src0_ub, src0, DATA_BYTES);
    asc_copy_gm2ub_align(src1_ub, src1, DATA_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    asc_add(dst_ub, src0_ub, src1_ub, TOTAL_LENGTH);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_ub, DATA_BYTES);
    asc_sync();
}

int main()
{
    std::vector<half> src0(TOTAL_LENGTH, half(1.0f));
    std::vector<half> src1(TOTAL_LENGTH, half(2.0f));
    std::vector<half> dst(TOTAL_LENGTH, half(0.0f));

    CHECK_ACL(aclInit(nullptr));
    CHECK_ACL(aclrtSetDevice(0));

    half* src0_device = nullptr;
    half* src1_device = nullptr;
    half* dst_device = nullptr;
    CHECK_ACL(aclrtMalloc(reinterpret_cast<void**>(&src0_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_NOT_NULL(src0_device);
    CHECK_ACL(aclrtMalloc(reinterpret_cast<void**>(&src1_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_NOT_NULL(src1_device);
    CHECK_ACL(aclrtMalloc(reinterpret_cast<void**>(&dst_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST));
    CHECK_NOT_NULL(dst_device);

    CHECK_ACL(aclrtMemcpy(src0_device, DATA_BYTES, src0.data(), DATA_BYTES, ACL_MEMCPY_HOST_TO_DEVICE));
    CHECK_ACL(aclrtMemcpy(src1_device, DATA_BYTES, src1.data(), DATA_BYTES, ACL_MEMCPY_HOST_TO_DEVICE));
    add_kernel<<<1, 0>>>(dst_device, src0_device, src1_device);
    CHECK_ACL(aclrtSynchronizeDevice());
    CHECK_ACL(aclrtMemcpy(dst.data(), DATA_BYTES, dst_device, DATA_BYTES, ACL_MEMCPY_DEVICE_TO_HOST));

    CHECK_ACL(aclrtFree(dst_device));
    CHECK_ACL(aclrtFree(src1_device));
    CHECK_ACL(aclrtFree(src0_device));
    CHECK_ACL(aclrtResetDevice(0));
    CHECK_ACL(aclFinalize());
    return 0;
}
```
