# UB->GM非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000001894460401"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| :--- | :---: |
|<cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter>|
|<cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter>|
|<cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter>|

## 功能说明<a name="section618mcpsimp"></a>

该接口提供数据非对齐搬运的功能，从Local Memory搬运数据至Global Memory时，支持非对齐搬运。

## 函数原型<a name="section620mcpsimp"></a>

dataCopyParams为DataCopyExtParams类型时：

```
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
```

```
template <typename T>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams)
```

dataCopyParams为DataCopyParams类型时：

```
template<typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& dataCopyParams)
```

```
template<typename T>
__aicore__ inline void DataCopyPad(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& dataCopyParams)
```

<cann-filter npu-type="950">不同产品型号对函数原型的支持存在差异，Ascend 950PR/Ascend 950DT支持设置数据搬运模式mode（搬运模式包括单次搬运对齐和整块数据搬运对齐）。</cann-filter>

## 数据类型<a name="section4219135304818"></a>

<cann-filter npu-type="950">

- Ascend 950PR/Ascend 950DT，支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64。

</cann-filter>

<cann-filter npu-type="A3">

- Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double。

</cann-filter>

<cann-filter npu-type="910b">

- Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double。

</cann-filter>

<cann-filter npu-type="310b">

- Atlas 200I/500 A2 推理产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

</cann-filter>

## 参数说明<a name="section622mcpsimp"></a>

**表 2**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 操作数的数据类型。各产品支持的数据类型请参考[数据类型](#section4219135304818)。 |
| mode | 配置数据搬运模式。PaddingMode类型，定义如下：<br><pre>enum class PaddingMode : uint8_t {<br>    Normal = 0,  // 默认模式，与原有数据搬运格式保持一致，每次数据搬运都会补齐至32字节对齐<br>    Compact,     // 紧凑模式，允许单次搬运不对齐，统一在整块数据末尾补齐至32字节对齐<br>};</pre><br>该参数仅Ascend 950PR/Ascend 950DT支持。 |

**表 3**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为GlobalTensor。<br>起始地址无地址对齐约束。 |
| src | 输入 | 源操作数，类型为LocalTensor。<br>起始地址需要保证32字节对齐。 |
| dataCopyParams | 输入 | 搬运参数。<br>&bull; DataCopyExtParams类型，具体参数说明请参考表4。<br>&bull; DataCopyParams类型，具体参数说明请参考表5。 |

下文表格中列出的结构体参数定义请参考`INSTALL_DIR`/include/ascendc/basic_api/interface/kernel_struct_data_copy.h，`INSTALL_DIR`请替换为CANN软件安装后文件存储路径。

**表 4**  DataCopyExtParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 指定该指令包含的连续传输数据块个数，数据类型为uint16_t，取值范围：blockCount∈[1, 4095]。 |
| blockLen | 指定该指令每个连续传输数据块长度，**该指令支持非对齐搬运**，**每个连续传输数据块长度单位为字节**。数据类型为uint32_t，取值范围：blockLen∈[1, 2097151]**，blockLen必须是 sizeof(T) 的整数倍，需要注意UB不要超限**。 |
| srcStride | 源操作数，相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>源操作数的逻辑位置为VECIN/VECOUT，单位为dataBlock(32字节)。<br>数据类型为uint32_t，srcStride不要超出该数据类型的取值范围。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[0, 65535]。 |
| dstStride | 目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>目的操作数的逻辑位置为GM，单位为字节。<br>数据类型为uint32_t，dstStride不要超出该数据类型的取值范围。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[0, 2^40-1]。 |
| rsv | 保留字段。 |

**表 5**  DataCopyParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 指定该指令包含的连续传输数据块个数，数据类型为uint16_t，取值范围：blockCount∈[1, 4095]。 |
| blockLen | 指定该指令每个连续传输数据块长度，**该指令支持非对齐搬运**，**每个连续传输数据块长度单位为字节**。数据类型为uint16_t，blockLen不要超出该数据类型的取值范围，**blockLen必须是 sizeof(T) 的整数倍**。 |
| srcStride | 源操作数，相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔），源操作数的逻辑位置为VECIN/VECOUT，单位为dataBlock(32字节)。数据类型为uint16_t，srcStride不要超出该数据类型的取值范围。 |
| dstStride | 目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔），目的操作数的逻辑位置为GM，单位为字节。数据类型为uint16_t，dstStride不要超出该数据类型的取值范围。 |

### VECIN/VECOUT->GM配置示例<a name="li1526352412213"></a>

- 当每个连续传输数据块长度blockLen为32字节对齐时，下图呈现了需要传入的DataCopyParams示例，blockLen为64，每个连续传输数据块包含64字节；srcStride为1，源操作数的逻辑位置为VECIN/VECOUT，srcStride的单位为dataBlock(32字节)，即源操作数相邻数据块之间间隔1个dataBlock；dstStride为1，目的操作数的逻辑位置为GM，dstStride的单位为字节，即目的操作数相邻数据块之间间隔1字节。

    ![](../../../../../figures/datacopypad3.png)

- 当每个连续传输数据块长度blockLen不满足32字节对齐，由于Unified Buffer要求32字节对齐，框架在搬出时会自动补充一些假数据来保证对齐，但在当搬到GM时会自动将填充的假数据丢弃掉。下图呈现了该场景下需要传入的DataCopyParams示例和假数据补齐的原理。blockLen为47，每个连续传输数据块包含47字节，不满足32字节对齐；srcStride为1，表示源操作数相邻数据块之间间隔1个dataBlock；dstStride为1，表示目的操作数相邻数据块之间间隔1字节。框架在搬出时会自动补充17字节的假数据来保证对齐，搬到GM时再自动将填充的假数据丢弃掉。

    ![](../../../../../figures/datacopypad4.png)

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section177231425115410"></a>

本示例实现了UB->GM的非对齐搬运过程，完整使用样例请参见[DataCopyPad样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm)。

```
AscendC::DataCopyExtParams copyParams{1, 20 * sizeof(half), 0, 0, 0};
// 从VECIN->GM搬运40字节
AscendC::DataCopyPad(dstGlobal, srcLocal, copyParams);
```

结果示例：

```
输入数据srcLocal: [1 2 3 ... 32]
输出数据dstGlobal:[1 2 3 ... 20]
```
