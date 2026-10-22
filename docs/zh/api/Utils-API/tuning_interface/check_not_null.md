# CHECK_NOT_NULL

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90处理器系列产品：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030处理器系列产品：支持
<!-- end id9 -->

## 功能说明

头文件路径为：`"utils/debug/check_not_null.h"`。

用于检查Host侧指针是否为空。指针非空时，程序继续执行；指针为空时，接口向标准错误流输出指针表达式、源文件名和行号，并调用`std::exit(EXIT_FAILURE)`终止当前进程，避免后续代码继续使用无效指针。

## 函数原型

```cpp
CHECK_NOT_NULL(ptr)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| ptr | 输入 | 待校验的Host侧对象指针。指针类型必须能够通过`static_cast`转换为`const void *`。 |

## 返回值说明

无

## 约束说明

- 仅支持在Host侧C++代码中调用，不支持在Device侧代码中调用。
- 指针为空时，接口终止当前进程，不返回调用方继续执行。`EXIT_FAILURE`的具体数值由C++标准库实现定义，调用方不能依赖其具体数值。
- `ptr`表达式只求值一次。为避免难以预期的行为，传入的表达式不应包含副作用。
- 不支持无法转换为`const void *`的指针或句柄类型。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id10 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id10 -->

```cpp
#include <iostream>
#include <vector>

#include "acl/acl.h"
#include "c_api/asc_simd.h"
#include "utils/debug/check_not_null.h"

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

    aclInit(nullptr);
    aclrtSetDevice(0);

    half* src0_device = nullptr;
    half* src1_device = nullptr;
    half* dst_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&src0_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    // 在申请设备内存后，使用`CHECK_NOT_NULL`检查设备指针。
    CHECK_NOT_NULL(src0_device);
    aclrtMalloc(reinterpret_cast<void**>(&src1_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    CHECK_NOT_NULL(src1_device);
    aclrtMalloc(reinterpret_cast<void**>(&dst_device), DATA_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    CHECK_NOT_NULL(dst_device);

    aclrtMemcpy(src0_device, DATA_BYTES, src0.data(), DATA_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(src1_device, DATA_BYTES, src1.data(), DATA_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    add_kernel<<<1, 0>>>(dst_device, src0_device, src1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(dst.data(), DATA_BYTES, dst_device, DATA_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);

    aclrtFree(dst_device);
    aclrtFree(src1_device);
    aclrtFree(src0_device);
    aclrtResetDevice(0);
    aclFinalize();
    return 0;
}
```

以`src0_device`为空为例，输出格式如下：

```text
CHECK_NOT_NULL failed: pointer 'src0_device' is null, file=example.asc, line=44
```

输出中的信息来源如下：

- `src0_device`：由宏参数`ptr`通过`#ptr`字符串化得到，即调用处传入的表达式文本。
- `example.asc`：由预定义宏`__FILE__`得到，即包含`CHECK_NOT_NULL`调用的源文件名。实际输出可能是文件名、相对路径或绝对路径，取决于编译命令。
- `44`：由预定义宏`__LINE__`得到，即`CHECK_NOT_NULL`在源文件中的调用行号。假设将以上代码块保存为`example.asc`，并以第一行`#include`作为第1行，则第一次调用位于第44行；代码增删后行号会相应变化。
