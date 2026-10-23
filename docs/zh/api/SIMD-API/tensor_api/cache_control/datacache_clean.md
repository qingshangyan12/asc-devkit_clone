# datacache_clean

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
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

头文件路径为：`tensor_api/tensor.h`。

将整个Data Cache的所有缓存行标记为无效。本接口不回写脏数据，未同步到主存的修改会被丢弃。调用后，下一次访问相关地址时从Global Memory重新加载数据。

## 函数原型

```cpp
__aicore__ inline void datacache_clean()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

在调用本接口之前，必须先调用[sync_data_barrier](../sync/data_barrier.md)，并将参数设置为`data_barrier_scope::all`，等待此前的内存访问指令执行结束。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void dci_example()
{
    sync_data_barrier(data_barrier_scope::all);
    datacache_clean();
}
```
