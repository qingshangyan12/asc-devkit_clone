# asc_icache_preload

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

开发者手动调用本接口，从调用本接口所在的Global Memory地址预加载指令到ICache（指令缓存）中，避免后续取指时出现ICache未命中。ICache用于缓存Scalar单元最近使用或频繁使用的指令，通过本接口预取后，Scalar单元读取GM指令时优先从ICache中读取。

预加载地址通常通过[asc_get_program_counter](../sys_var/asc_get_program_counter.md)接口获取。

## 函数原型

```c
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `addr` | 输入 | 预取指令的Global Memory起始地址。 |
| `prefetch_len` | 输入 | 预加载数据的长度，单位为2K Byte。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- `addr`预取地址硬件按128B对齐取有效位，调用方传入非128B对齐地址不会报错，但实际预取起始Cache Line由对齐结果决定。
- `prefetch_len`的取值范围为[0, 16]（AIC，对应32KB ICache）或[0, 8]（AIV，对应16KB ICache）。
- 调用本接口时的预加载地址需通过[asc_get_program_counter](../utils/sys_var/asc_get_program_counter.md)接口获取当前程序计数器（PC）值，获取后必须通过掩码`0xFFFFFFFFFFFF`（低48位全1）将高位无效位清零，以得到当前指令在GM中的有效48位地址。这是预加载地址的固定获取流程，示例代码如下：

    ```cpp
    int64_t prefetch_length = 6;
    // 通过asc_get_program_counter获取当前程序计数器（PC）值。
    // PC为64位返回值，有效地址位为0~47位，通过掩码0xFFFFFFFFFFFF（低48位全1）将高位无效位清零，
    // 得到当前指令在GM中的有效48位地址，作为预加载的起始地址。
    int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
    asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
    ```
## 调用示例

```cpp
int64_t prefetch_length = 7;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```
