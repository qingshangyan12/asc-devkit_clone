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

从指令所在GM地址预加载数据到对应的cacheline中。

开发者手动调用该接口，能够从指令所在GM地址预加载指令到ICache中。预加载地址通常通过[asc_get_program_counter](../sys_var/asc_get_program_counter.md)接口获取，即以当前程序执行位置为起点，预加载后续指令到ICache中，减少取指时的ICache Miss。

<font color="red">

头文件路径：`"c_api/cache_ctrl/cache_ctrl.h"`。

开发者手动调用本接口，从指令所在的Global Memory地址预加载指令到ICache（指令缓存）中，避免后续取指时出现ICache未命中。ICache用于缓存Scalar单元最近使用或频繁使用的指令，通过本接口预取后，Scalar单元读取GM指令时优先从ICache中读取。

调用时传入预取起始地址与以2KB为单位的预取数量，硬件按128B对齐地址后。预取数量为0时不产生任何预取请求，等价于空操作。

本接口在AI Core与Vector Core上均生效，SIMT编译模式下整个`cache_ctrl.h`头文件不可用。

</font>

## 函数原型

```cpp
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| addr | 输入 | 预加载数据的地址。 |
| prefetch_len | 输入 | 预加载数据的长度，单位为2K Byte，取值需满足 $\text{prefetch\_len} < \dfrac{\text{ICache大小}}{2K}$。其中，AIC和AIV的ICache大小分别为32KB和16KB。 |

<font color="red">

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `addr` | 输入 | 预取指令的Global Memory起始地址。取值范围：任意有效的Global Memory指针。硬件执行时按128B对齐取有效位，调用方传入非128B对齐地址不会报错但实际预取起始Cache Line由对齐结果决定。 |
| `prefetch_len` | 输入 | 以2KB为单位的预取数量（非字节数）。取值范围：[0, 16]（AI Core，对应32KB ICache）；[0, 8]（Vector Core，对应16KB ICache）。传入超过有效上界的值会被封顶处理，不报错。传入0时不产生任何预取请求，等价于空操作，不触发异常。 |

</font>

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

prefetch_len参数单位为2K Byte，取值需满足 $\text{prefetch\_len} < \dfrac{\text{ICache大小}}{2K}$。其中，AIC和AIV的ICache大小分别为32KB和16KB。

<font color="red">

### 通用约束

- 预取起始地址按128B对齐取有效位。调用方传入非128B对齐地址不会报错但实际预取起始Cache Line由对齐结果决定。
- prefetch_len是以2KB为单位的预取数量（非字节数），实际展开的Cache Line数为`预取数量 × 16`，并封顶为ICache总容量对应的Cache Line数。AI Core上ICache总容量32KB，预取数量有效上界为16；Vector Core上ICache总容量16KB，预取数量有效上界为8。传入超过有效上界的值会被封顶处理，不报错。
- 若值为0则等价于空操作。
- prefetch_len传入0时不产生任何预取请求，等价于空操作，不触发异常。
- 本接口在AI Core与Vector Core上均生效。SIMT编译模式下整个`cache_ctrl.h`头文件不可用，调用本接口会触发编译错误。

</font>

## 调用示例

```cpp
int64_t prefetch_length = 32;
// 通过asc_get_program_counter获取当前程序计数器（PC）值。
// PC为64位返回值，有效地址位为0~47位，通过掩码0xFFFFFFFFFFFF（低48位全1）将高位无效位清零，
// 得到当前指令在GM中的有效48位地址，作为预加载的起始地址。
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```
