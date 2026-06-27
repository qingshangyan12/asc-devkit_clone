# Scatter\(ISASI\)<a name="ZH-CN_TOPIC_0000001862055393"></a>

## 产品支持情况<a name="section1550532418810"></a>

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
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section17600329101418"></a>

头文件路径为："basic_api/kernel_operator_vec_scatter_intf.h"。

给定一个连续的输入张量和一个目的地址偏移张量，Scatter指令根据偏移地址生成新的结果张量后将输入张量分散到结果张量中。

将源操作数src中的元素按照指定的位置（由dst\_offset和base\_addr共同作用）分散到目的操作数dst中。

## 函数原型<a name="section15660625202219"></a>

-   tensor前n个数据计算

    ```
    template <typename T>
    __aicore__ inline void Scatter(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint32_t>& dstOffset, const uint32_t dstBaseAddr, const uint32_t count)
    ```

-   tensor高维切分计算
    -   mask逐bit模式

        ```
        template <typename T>
        __aicore__ inline void Scatter(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint32_t>& dstOffset, const uint32_t dstBaseAddr, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
        ```

    -   mask连续模式

        ```
        template <typename T>
        __aicore__ inline void Scatter(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint32_t>& dstOffset, const uint32_t dstBaseAddr, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
        ```

## 参数说明<a name="section1619484392111"></a>

**表1**  模板参数说明

| 参数名 | 描述 |
| :----- | :--- |
| T | 操作数数据类型。 |


**表2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| :------- | :-------- | :--- |
| dst | 输出 | 目的操作数，类型为LocalTensor。LocalTensor的起始地址需要32字节对齐。 |
| src | 输入 | 源操作数，类型为LocalTensor。数据类型需与dst保持一致。 |
| dstOffset | 输入 | 用于存储源操作数的每个元素在dst中对应的地址偏移。偏移基于dst的基地址dstBaseAddr计算，以字节为单位，取值应保证按dst数据类型位宽对齐，否则会导致非预期行为。<br>针对以下型号，地址偏移的取值范围不超出uint32_t的范围即可。<br>Atlas 推理系列产品AI Core<br>针对以下型号，地址偏移的取值范围如下：当操作数为8位时，取值范围为[0, 2^16-1]；当操作数为16位时，取值范围为[0, 2^17-1]，当操作数为32位或者64位时，不超过uint32_t的范围即可。超出取值范围可能导致非预期输出。<br>Atlas 200I/500 A2 推理产品<br>Ascend 950PR/Ascend 950DT |
| dstBaseAddr | 输入 | dst的起始偏移地址，单位是字节。取值应保证按dst数据类型位宽对齐，否则会导致非预期行为。 |
| count | 输入 | 执行处理的数据个数。 |
| mask/mask[] | 输入 | [mask](../SIMD计算说明/高维切分.md)用于控制每次迭代内参与计算的元素。<br>连续模式：表示前面连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为8位或16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]；当操作数为64位时，mask∈[1, 32]。<br>逐bit模式：可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。参数类型为长度为2的uint64_t类型数组。例如，mask=[8, 0]，8=0b1000，表示仅第4个元素参与计算。参数取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为8位或16位时，mask[0]、mask[1]∈[0, 2^64-1]并且不同时为0；当操作数为32位时，mask[1]为0，mask[0]∈(0, 2^64-1]；当操作数为64位时，mask[1]为0，mask[0]∈(0, 2^32-1]。 |
| repeatTime | 输入 | 指令迭代次数，每次迭代完成8个datablock的数据收集，数据范围：repeatTime∈[0,255]。<br>特别地，针对以下型号：Atlas 200I/500 A2 推理产品、Ascend 950PR/Ascend 950DT，操作数为**8位**时，每次迭代完成**4个datablock**（32Bytes）的数据收集。 |
| srcRepStride | 输入 | 相邻迭代间的地址步长，单位是datablock。 |

## 数据类型
<!-- npu="950" id8 -->
- 针对Ascend 950PR/Ascend 950DT，T支持的数据类型为：uint8_t、int8_t、uint16_t、int16_t、half、bfloat16_t、uint32_t、int32_t、float、uint64_t、int64_t。
<!-- end id8 -->
<!-- npu="310b" id9 -->
- 针对Atlas 200I/500 A2 推理产品，T支持的数据类型为：uint8_t、int8_t、uint16_t、int16_t、half、uint32_t、int32_t、float。
<!-- end id9 -->
<!-- npu="310p" id10 -->
- 针对Atlas 推理系列产品AI Core，T支持的数据类型为：uint16_t、uint32_t、float、half。
<!-- end id10 -->

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
-   操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。

-   dstOffset中的偏移地址不能有相同值，如果存在2个或者多个偏移重复的情况，行为是不可预期的，即无法确定谁对这个地址进行了最后写入。
-   针对Ascend 950PR/Ascend 950DT，uint8\_t/int8\_t数据类型仅支持tensor前n个数据计算接口。
<!-- npu="950" id8 -->
-   该接口通过VF调用[Reg矢量计算](../../Reg矢量计算)API实现兼容，当参数count或repeatTime取值为0时，不保证该接口被视为NOP（空操作）。
<!-- end id8 -->

## 调用示例<a name="section11276201527"></a>

完整使用样例请参见[Scatter兼容性样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/06_compatibility_guide/scatter)。

```
uint32_t COUNT = 128;
AscendC::Scatter(dstLocal, srcLocal, dstOffsetLocal, (uint32_t)0, COUNT); // dstOffsetLocal用于存储源操作数的每个元素在dst中对应的地址偏移
```

结果示例：

```
输入数据dstOffsetLocal:
[254 252 250 ... 4 2 0]
输入数据srcLocal（128个half类型数据）: 
[0 1 2 ... 125 126 127]
输出数据dstGlobal:
[127 126 125 ... 2 1 0]
```
