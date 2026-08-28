# asc_get_store_atomic_config

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

获取标量原子操作的配置，包括原子操作的类型以及数据类型。接口支持通过函数返回值返回配置，也支持通过引用和参数输出配置：前者直接返回配置，后者将配置写入`config`。

该接口可用于校验[asc_set_store_atomic_config_v1](asc_set_store_atomic_config_v1.md)设置的原子操作启用位和原子操作类型是否符合预期。

<!-- npu="950" id8 -->
**Ascend 950PR/Ascend 950DT产品上该接口已废弃。原子加操作可以直接使用[asc_atomic_add](asc_atomic_add.md)实现。无需再对原子操作启用位和原子操作类型的值进行配置和校验。**
<!-- end id8 -->

## 函数原型

```c
// 通过引用和参数输出配置
__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config)

// 通过函数返回值返回配置，仅Atlas A3训练系列产品/Atlas A3推理系列产品和Atlas A2训练系列产品/Atlas A2推理系列产品支持
__aicore__ inline asc_store_atomic_config asc_get_store_atomic_config()
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入/输出 | 用于通过引用和参数输出配置。调用前无需设置，调用后保存获取到的原子操作启用位和原子操作类型，详细说明请参考[asc_store_atomic_config](../../defs/union/asc_store_atomic_config.md)。 |

## 返回值说明

- 通过函数返回值返回配置：返回`asc_store_atomic_config`类型的配置。
- 通过引用和参数输出配置：无返回值，配置写入`config`。

## 流水类型

PIPE_S

## 约束说明

需配合[asc_set_store_atomic_config_v1](asc_set_store_atomic_config_v1.md)接口使用。上述接口用于设置原子操作启用位和原子操作类型，本接口用于获取配置。

## 调用示例

本示例在2201架构下设置并获取数据搬运原子操作配置，演示通过引用和参数以及函数返回值两种方式获取配置。

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`指定为`dav-2201`：

```bash
bisheng example.asc -o main --npu-arch=dav-2201 && ./main
```

```cpp
#include <cstdint>
#include <iostream>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

constexpr uint32_t OUTPUT_COUNT = 2;

__global__ __vector__ void asc_get_store_atomic_config_kernel(__gm__ uint64_t* output)
{
    asc_init();
    asc_set_store_atomic_config_v1(1, 0);

    // 无返回值原型：通过引用参数输出配置。
    asc_store_atomic_config config;
    asc_get_store_atomic_config(config);
    output[0] = config.config;

    // 返回值原型：仅Atlas A3训练系列产品/Atlas A3推理系列产品和Atlas A2训练系列产品/Atlas A2推理系列产品支持，Ascend 950PR/Ascend 950DT产品上该原型不支持。
    asc_store_atomic_config config_return = asc_get_store_atomic_config();
    output[1] = config_return.config;

    asc_set_atomic_none();
    asc_sync();
}

int main()
{
    uint64_t output[OUTPUT_COUNT] = {0, 0};
    const uint64_t golden = 1;
    uint64_t* output_device = nullptr;

    aclInit(nullptr);
    aclrtSetDevice(0);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), sizeof(output), ACL_MEM_MALLOC_HUGE_FIRST);
    asc_get_store_atomic_config_kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output, sizeof(output), output_device, sizeof(output), ACL_MEMCPY_DEVICE_TO_HOST);

    const bool passed = output[0] == golden && output[1] == golden;
    std::cout << (passed ? "[Success] asc_get_store_atomic_config passed."
                         : "[Failed] asc_get_store_atomic_config failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
