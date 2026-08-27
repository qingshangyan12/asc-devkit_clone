# asc_storeunalign_mask_postupdate

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910" id4 -->
- Atlas 训练系列产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品 AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id7 -->

## 功能说明

reg计算数据搬运接口，适用于从掩码寄存器连续非32B对齐的起始地址连续搬出到Unified Buffer（UB）的场景。

该接口每调用一次接口会更新目的操作数在UB上的地址。

## 函数原型

### 掩码寄存器连续非对齐搬出

```c
__simd_callee__ inline void asc_storeunalign_mask_postupdate(__ubuf__ <dtype>*& dst,
                                                              vector_store_unalign& src0,
                                                              vector_bool src1)
```

#### dtype支持数据类型

dtype支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_storeunalign_mask_postupdate(__ubuf__ uint8_t*& dst,
                                                              vector_store_unalign& src0,
                                                              vector_bool src1)
```

## 参数说明

### 掩码寄存器连续非对齐搬出

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入/输出 | 非对齐寄存器，用于保存非对齐数据，长度32B。 |
| src1 | 输入 | 源操作数（掩码寄存器）。 |

掩码寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 该接口中的dst不需要32B对齐。
- 本接口应与[asc_storeunalign_post](asc_storeunalign_post.md)接口配合使用。asc_storeunalign_post接口用于处理该场景下非对齐搬出的尾块。

## 调用示例

以下示例分别调用新旧接口并比较搬出结果。将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng examples.asc -o main --npu-arch=dav-3510; ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t BUFFER_BYTES = 64;

__simd_vf__ inline void store_mask(__ubuf__ uint8_t* actual_base, __ubuf__ uint8_t* expected_base)
{
    __ubuf__ uint8_t* actual = actual_base + 8;
    __ubuf__ uint8_t* expected = expected_base + 8;
    vector_store_unalign actual_state;
    vector_store_unalign expected_state;
    vector_bool src = asc_create_mask_b8(PAT_ALL);
    asc_storeunalign_mask_postupdate(actual, actual_state, src);
    asc_storeunalign_postupdate(expected, expected_state, src);
    asc_storeunalign_post(actual, actual_state, 0);
    asc_storeunalign_post(expected, expected_state, 0);
}

__global__ __vector__ void store_mask_kernel(__gm__ uint8_t* actual, __gm__ uint8_t* expected)
{
    asc_init();
    __ubuf__ uint8_t actual_local[BUFFER_BYTES];
    __ubuf__ uint8_t expected_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(actual_local, actual, BUFFER_BYTES);
    asc_copy_gm2ub_align(expected_local, expected, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    store_mask(actual_local, expected_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(actual, actual_local, BUFFER_BYTES);
    asc_copy_ub2gm_align(expected, expected_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> actual(BUFFER_BYTES, 0);
    std::vector<uint8_t> expected(BUFFER_BYTES, 0);
    uint8_t* actual_device = nullptr;
    uint8_t* expected_device = nullptr;
    aclInit(nullptr);
    aclrtSetDevice(0);
    aclrtMalloc(reinterpret_cast<void**>(&actual_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&expected_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(actual_device, BUFFER_BYTES, actual.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(expected_device, BUFFER_BYTES, expected.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    store_mask_kernel<<<1, 0>>>(actual_device, expected_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(actual.data(), BUFFER_BYTES, actual_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    aclrtMemcpy(expected.data(), BUFFER_BYTES, expected_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    const bool passed = actual == expected;
    aclrtFree(actual_device);
    aclrtFree(expected_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
