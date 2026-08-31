# asc_get_phy_buf_addr

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

基于偏移量offset获取片上实际物理地址，常用于在调试或地址推导场景下把一个相对偏移量转换为可在硬件上直接使用的物理地址形式。本接口将传入的偏移量作为64 bit立即数加载到标量X寄存器中返回，返回值即为传入的偏移量本身（按`uint64_t`返回）。该接口不读取任何特殊寄存器或存储状态，仅完成立即数到标量寄存器的搬运，可在AIC与AIV上下文中调用。

## 函数原型

```c
__aicore__ inline uint64_t asc_get_phy_buf_addr(uint64_t offset)
```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
| :------ | :--- | :------------ |
|offset   |输入   |片上内存的地址偏移量，范围是0~物理内存大小。参数类型为`uint64_t`，该值作为64 bit立即数加载到标量寄存器，返回值与传入值完全一致。|

## 返回值说明

返回`uint64_t`类型的物理内存地址，与传入的`offset`完全相等，代表按偏移量得到的物理地址形式。

## 流水类型

PIPE_S

## 约束说明

- 不能和[以数组方式申请内存的方法](../general_description_and_constraints.md#以数组方式申请内存)混用。
- 本接口仅完成立即数到标量寄存器的搬运，不读取或修改任何特殊寄存器、存储状态或缓冲内容，可在AIC与AIV上下文中调用。
- 返回值与传入的`offset`完全相等，接口本身不做地址合法性或对齐校验；调用方需自行保证返回值作为物理地址使用时满足对应缓冲的对齐与容量要求。

## 调用示例

```cpp
// 假设src0，src1，dst三个操作数均包含64个half类型的数据。
uint64_t offset = 0;                                   // 首先为src0申请内存，从0开始。
__ubuf__ half* src0 = (__ubuf__ half*)asc_get_phy_buf_addr(offset);    // 获取src0的地址，通过__ubuf__关键字指定该地址指向UB内存。
offset += 64 * sizeof(half);                           // 通过offset将src1的起始地址设置在src0之后。
__ubuf__ half* src1 = (__ubuf__ half*)asc_get_phy_buf_addr(offset);    // 获取src1的地址，通过__ubuf__关键字指定该地址指向UB内存。
offset += 64 * sizeof(half);                           // 通过offset将dst的起始地址设置在src1之后。
__ubuf__ half* dst = (__ubuf__ half*)asc_get_phy_buf_addr(offset);     // 获取dst的地址，通过__ubuf__关键字指定该地址指向UB内存。
...... // 使用src0、src1、dst中的数据进行后续计算或数据搬运操作。
```
