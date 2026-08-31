# asc_get_squeeze_status

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

读取squeeze操作后保存至AR特殊寄存器的有效数据长度值，用于配合[asc_squeeze_with_status](../reg/compare_and_select/asc_squeeze_with_status.md)和[asc_storeunalign_postupdate](../reg/reg_store/asc_storeunalign_postupdate.md)接口完成不等长数据存储。

AR是32 bit只读特殊寄存器，作为矢量计算中按块归约存储场景下的地址寄存器使用，由硬件在每次矢量循环开始时自动复位为0，并在归约存储类指令执行过程中按本次归约写入的有效元素总长度累加更新。本接口固定读取当前核上下文中的AR寄存器值并按`int64_t`返回，调用方可在归约存储完成后读取该值以获知本次矢量循环内归约写入的累积字节数。

## 函数原型

```c
__aicore__ inline int64_t asc_get_squeeze_status()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的squeeze有效数据长度值。寄存器宽度为32 bit，硬件保证返回值低32 bit为有效AR值，高32 bit由`int64_t`符号扩展得到。复位值为0，每次矢量循环开始时由硬件自动复位为0，循环内随归约存储类指令的执行按写入有效长度累加。调用方解析返回值时按32 bit无符号整数处理即可。

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，仅读取AR特殊寄存器当前值，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- AR寄存器为只读寄存器，调用方不可直接设置或修改该值，仅能通过执行归约存储类指令由硬件自动累加更新。
- AR寄存器宽度为32 bit，复位值为0，由硬件在每个矢量循环开始时自动复位为0；调用方不应跨矢量循环依赖上一轮循环的AR残留值。
- 返回值按32 bit无符号整数解析即可，按`int64_t`返回时高32 bit为符号扩展，调用方若需按无符号32 bit整数解析应自行做位运算处理（如`& 0xFFFFFFFF`）。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

</font>

## 调用示例

无
