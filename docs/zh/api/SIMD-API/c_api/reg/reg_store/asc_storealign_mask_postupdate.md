# asc_storealign_mask_postupdate

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

当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。

掩码寄存器搬出场景提供以下一种模式：

- **立即数偏移搬出模式**：将掩码寄存器中的数据连续对齐搬出到当前目的地址，搬出完成后基于立即数累加目的地址指针，单位为字节。

本接口仅在AIV上生效。

## 函数原型

### 掩码寄存器立即数偏移搬出模式

```c
__simd_callee__ inline void asc_storealign_mask_postupdate(__ubuf__ <dtype>*& dst,
                                                            vector_bool src,
                                                            int32_t offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_storealign_mask_postupdate(__ubuf__ uint8_t*& dst,
                                                            vector_bool src,
                                                            int32_t offset)
```

## 参数说明

### 掩码寄存器立即数偏移搬出模式

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。 |
| src | 输入 | 源操作数（掩码寄存器）。 |
| offset | 输入 | 目的地址更新量，类型为`int32_t`，单位为字节。接口执行后，dst指针的底层UB地址偏移offset字节。 |

掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- dst起始地址需32字节对齐，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。目的操作数的实际访问地址和Post Update后的地址不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- Post Update后的dst新地址仍需32字节对齐，否则下一次调用本接口会报错。

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

__simd_vf__ inline void store_mask(__ubuf__ uint8_t* actual, __ubuf__ uint8_t* expected)
{
    vector_bool src = asc_create_mask_b8(PAT_ALL);
    asc_storealign_mask_postupdate(actual, src, 32);
    asc_storealign_postupdate(expected, src, 32);
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
