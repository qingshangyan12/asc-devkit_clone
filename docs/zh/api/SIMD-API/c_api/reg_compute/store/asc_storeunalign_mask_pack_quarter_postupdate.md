# asc_storeunalign_mask_pack_quarter_postupdate

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
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

将掩码寄存器中的数据连续搬出到8字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。

针对掩码寄存器，本接口提供一种模式：

- **掩码寄存器PACK_QUARTER搬出模式：** 硬件指令从每4bit数据中提取最低有效位（LSB），将掩码寄存器中32B数据打包成8B，写入UB。搬运完成后UB地址按8B偏移量更新。需配合后处理接口[asc_storeunalign_post](asc_storeunalign_post.md)使用。

本接口可以拼接上一段**连续的尾块**和本次搬运主块部分，并同时更新本次尾块数据至非对齐寄存器中。首次搬运可以使用未初始化的非对齐寄存器。若上一次搬运的尾块和本次搬运主块不连续，则每一次尾块处理都需要调用后处理接口。若连续，仅最后一次尾块处理需要调用后处理接口。

本接口仅在AIV上生效。

## 函数原型

### 掩码寄存器搬出模式

```c
__simd_callee__ inline void asc_storeunalign_mask_pack_quarter_postupdate(
    __ubuf__ <dtype>*& dst,
    vector_store_unalign& unalign_reg,
    vector_bool src)
```

#### dtype支持数据类型

`dtype`支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storeunalign_mask_pack_quarter_postupdate(
    __ubuf__ uint8_t*& dst,
    vector_store_unalign& unalign_reg,
    vector_bool src)
```

## 参数说明

### 掩码寄存器搬出模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的操作数在UB中的起始地址，类型为`__ubuf__ <dtype>*&`。`b8`、`b16`、`b32`类型目的地址均需8字节对齐。搬出后，`dst`自动更新至本次搬出的结束地址。 |
| unalign_reg | 输入/输出 | 非对齐寄存器，类型为`vector_store_unalign&`，长度为32字节。 |
| src | 输入 | 源操作数（掩码寄存器），类型为`vector_bool`。 |

掩码寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。目的操作数地址更新后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 指令约束

- 连续调用时须复用同一个`unalign_reg`。
- 掩码寄存器PACK_QUARTER搬出模式须使用[asc_storeunalign_post](asc_storeunalign_post.md)的立即数偏移搬出模式收尾，复用同一个`unalign_reg`。
- 掩码寄存器搬出场景，`b8`、`b16`、`b32`类型目的地址均需8字节对齐。

## 调用示例

以下示例分别调用新旧接口并比较搬出结果。将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
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
    asc_storeunalign_mask_pack_quarter_postupdate(actual, actual_state, src);
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
