# GM->UB非对齐数据搬运(DataCopyPad)<a name="ZH-CN_TOPIC_0000001894460401"></a>

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

头文件路径为：`"basic_api/kernel_operator_data_copy_intf.h"`。

该接口提供将数据从Global Memory非对齐搬运至Unified Buffer的功能，可以根据开发者的需要自行填充数据。

填充的数据有两种配置方式：

- 配置结构体[DataCopyPadExtParams](#zh-cn_topic_0000002565888823_table2036831862917)的参数paddingValue，此时DataCopyPadExtParams的参数isPad需要设置为true。
- 使用寄存器配置接口[SetPadValue](寄存器配置说明/SetPadValue(ISASI).md)在接口外部设置需要填充的数据，此时DataCopyPadExtParams的参数isPad需要设置为false。

## 函数原型<a name="section620mcpsimp"></a>

<cann-filter npu-type="950">
针对Ascend 950PR/Ascend 950DT，支持设置数据搬运模式mode（搬运模式包括单次搬运对齐和整块数据搬运对齐）：

```cpp
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
```

</cann-filter>

```cpp
template <typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 操作数以及paddingValue（待填充数据值）的数据类型。各产品支持的数据类型请参考[数据类型](#section4219135304818)。 |
| <cann-filter npu-type="950">mode | 该参数仅Ascend 950PR/Ascend 950DT支持，配置数据搬运模式。PaddingMode类型，定义如下：<br><pre>enum class PaddingMode : uint8_t {<br>    Normal = 0,  // 默认模式，与原有数据搬运格式保持一致，每次数据搬运都会补齐至32字节对齐<br>    Compact,     // 紧凑模式，允许单次搬运不对齐，统一在整块数据末尾补齐至32字节对齐<br>};</pre></cann-filter> |

**表 2**  接口参数说明

| 参数名称 | 输入/输出 | 含义 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为LocalTensor，存储位置为Unified Buffer，目的地址需要32B对齐。 |
| src | 输入 | 源操作数，类型为GlobalTensor，存储位置为Global Memory，源地址需要1B对齐。 |
| dataCopyParams | 输入 | 搬运参数。DataCopyExtParams类型，具体参数说明请参考表3。 |
| padParams | 输入 | 从Global Memory搬运数据至Local Memory时，可以根据开发者需要，在搬运数据左边或右边填充数据。padParams是用于控制数据填充过程的参数。DataCopyPadExtParams类型，具体参数请参考表4。 |

下文表格中列出的结构体参数定义请参考\$\{INSTALL\_DIR\}/include/ascendc/basic\_api/interface/kernel\_struct\_data\_copy.h，\$\{INSTALL\_DIR\}请替换为CANN软件安装后文件存储路径。

**表 3**  DataCopyExtParams结构体参数定义

| 参数名 | 描述 |
| :--- | :--- |
| blockCount | 搬运的数据块个数，uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。<br>**注：blockCount = 0表示不执行搬运，该接口将被视为NOP（空操作）**。 |
| blockLen | 搬运的每个数据块长度，数据类型为uint32_t，取值范围：blockLen∈[0, 2097151]，单位：1B。<br>**注：blockLen= 0表示不执行搬运，该接口将被视为NOP（空操作）**。**blockLen必须是sizeof(T)的整数倍，需要注意UB不要超限。** |
| srcStride | 源操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：srcStride∈[0, 2^32 - 1]，单位：1B。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[-blockLen, 2^40-1]。 |
| dstStride | 目的操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint32_t类型，取值范围：dstStride∈[0, 2^32 - 1]，单位：dataBlock（32B）。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[0, 65535]。<br>**注：需要注意UB不要超限**。 |
| rsv | 保留字段。uint32_t类型，默认值为0。 |

**表 4**  DataCopyPadExtParams&lt;T&gt;结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| isPad | 是否需要填充用户自定义的数据，取值范围：true，false。<br>true：填充padding value。<br>false：表示用户不需要指定填充值，会默认填充随机值。 |
| leftPadding | 连续搬运数据块左侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| rightPadding | 连续搬运数据块右侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| paddingValue | 左右两侧需要填充的数据值，需要保证在数据占用字节范围内。<br>数据类型和源操作数保持一致，数据类型为模板参数T。<br>**当数据类型长度为64位时，该参数只能设置为0。** |

### GM->VECIN/VECOUT配置示例<a name="li73127579197"></a>

- 参数解释
    - 当blockLen+leftPadding+rightPadding满足32字节对齐时，若isPad为false，左右两侧填充的数据值会默认为随机值；否则为paddingValue。
    - 当blockLen+leftPadding+rightPadding不满足32字节对齐时，框架会填充一些假数据dummy，保证左右填充的数据和blockLen、假数据为32字节对齐。若leftPadding、rightPadding都为0：dummy会默认填充待搬运数据块的第一个元素值；若leftPadding/rightPadding不为0：isPad为false，左右两侧填充的数据值和dummy值均为随机值；否则为paddingValue。

- 配置示例1：
    - blockLen为64，每个连续传输数据块包含64字节；srcStride为1，因为源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间间隔1字节；dstStride为1，因为目的操作数的逻辑位置为VECIN/VECOUT，dstStride的单位为DataBlock数量（每DataBlock为32字节），即目的操作数相邻数据块之间间隔1个dataBlock。
    - blockLen+leftPadding+rightPadding满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值；否则为paddingValue。此处示例中，leftPadding、rightPadding均为0，则不填充。
    - blockLen+leftPadding+rightPadding不满足32字节对齐时，框架会填充一些假数据dummy，保证左右填充的数据和blockLen、假数据为32字节对齐。leftPadding/rightPadding不为0：若isPad为false，左右两侧填充的数据值和dummy值均为随机值；否则为paddingValue。

    ![](../../../../../figures/datacopypad1.png)

- 配置示例2：
    - blockLen为47，每个连续传输数据块包含47字节；srcStride为1，表示源操作数相邻数据块之间间隔1字节；dstStride为1，表示目的操作数相邻数据块之间间隔1个dataBlock。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding、rightPadding均为0：dummy会默认填充待搬运数据块的第一个元素值。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding/rightPadding不为0：若isPad为false，左右两侧填充的数据值和dummy值均为随机值；否则为paddingValue。

    ![](../../../../../figures/datacopypad2.png)
<cann-filter npu-type="950">
- 配置示例3：
    - blockLen为48，每个连续传输数据块包含48字节；srcStride为-48，表示源操作数相邻数据块之间间隔-48字节，相当于每次传输的连续数据块都是同一块；dstStride为1，表示目的操作数相邻数据块之间间隔1个dataBlock。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding、rightPadding均为0：dummy会默认填充待搬运数据块的第一个元素值。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding/rightPadding不为0：若isPad为false，左右两侧填充的数据值和dummy值均为随机值；否则为paddingValue。

    ![](../../../../../figures/datacopypad.png)
</cann-filter>

### 搬运模式的配置示例

<cann-filter npu-type="950">

- Normal模式
    - blockLen为48，每个连续传输数据块包含48字节；srcStride为0，源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间紧密排列；dstStride为0，目的操作数的逻辑位置为VECIN/VECOUT，dstStride的单位为DataBlock（32字节），目的操作数相邻数据块之间无间隔，注意数据块包含leftPadding/rightPadding数据。
    - blockLen + leftPadding + rightPadding满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值，否则为paddingValue。此处示例中，leftPadding为0，rightPadding为16，每个连续传输数据块都会在右侧填充16字节。目的操作数的总长度为192字节。

- Compact模式
    - blockLen为48，每个连续传输数据块包含48字节；srcStride为0，源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间紧密排列；dstStride为0，目的操作数的逻辑位置为VECIN/VECOUT，dstStride的单位为DataBlock（32字节），目的操作数相邻数据块之间紧密排列，不会填充数据。
    - blockLen * blockCount + leftPadding + rightPadding 满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值，否则为paddingValue。此处示例中，leftPadding为0，rightPadding为16，在最后一个数据块右侧填充16字节。目的操作数的总长度为160字节。

    ![](../../../../../figures/paddingMode.png)

</cann-filter>

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

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 位于Global Memory的源地址必须1B对齐，位于Unified Buffer的目的地址必须32B对齐。
- leftPadding、rightPadding所占字节数均不能超过32B。
- blockLen必须是sizeof\(T\)的整数倍。
- 结构体DataCopyPadExtParams的参数paddingValue数据类型和源操作数保持一致。当数据类型为b64时，paddingValue只能设置为0。
- 结构体DataCopyPadExtParams的参数取值范围需要考虑UB超限问题。

## 调用示例<a name="section177231425115410"></a>

本示例实现了GM->VECIN的非对齐搬运过程，完整使用样例请参见[DataCopyPad样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm)。

```cpp
AscendC::DataCopyExtParams copyParams{1, 20 * sizeof(half), 0, 0, 0};
AscendC::DataCopyPadExtParams<half> padParams{true, 0, 2, 0};
// 从GM->VECIN搬运40字节。
AscendC::DataCopyPad(dstGlobal, src0Global, copyParams, padParams);
```

结果示例：

```text
输入数据src0Global: [1 2 3 ... 32]
输出数据dstGlobal:[1 2 3 ... 20]
```
