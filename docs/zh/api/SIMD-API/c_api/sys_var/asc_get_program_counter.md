# asc_get_program_counter

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

获取程序计数器（PC）的指针，程序计数器用于记录当前程序执行的位置。PC是一个只读特殊寄存器，宽度64 bit，复位值为0，由硬件随指令推进自动维护。

程序计数器指针的有效地址位为0~47位，使用时需通过掩码0xFFFFFFFFFFFF（低48位全1）将高位无效位清零，即只取低48位作为有效地址。典型使用场景为配合[asc_icache_preload](../cache_ctrl/asc_icache_preload.md)接口，以当前程序执行位置为起点预加载后续指令到ICache中，调用示例请参考[asc_icache_preload调用示例](../cache_ctrl/asc_icache_preload.md#调用示例)。

## 函数原型

```c
__aicore__ inline int64_t asc_get_program_counter()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的当前程序计数器值。计数器宽度为64 bit，复位值为0，返回值随指令推进不断变化，反映当前核正在执行的指令位置。

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，读取的是只读程序计数器寄存器，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- 返回值由硬件随指令推进自动维护，本接口仅读取当前核上下文中的PC值，调用方不可设置或修改该值。
- 计数器宽度为64 bit，但有效地址位为0~47位：硬件约束要求程序员保证PC值不超过2^48−1（即`PC[47:0]`的最大值），使用时需通过掩码0xFFFFFFFFFFFF将高位无效位清零。
- 本接口读取的是程序计数器（PC），与核ID、block索引、系统cycle计数器等其他系统变量语义不同，不可混用。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

</font>

## 调用示例

```cpp
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
```