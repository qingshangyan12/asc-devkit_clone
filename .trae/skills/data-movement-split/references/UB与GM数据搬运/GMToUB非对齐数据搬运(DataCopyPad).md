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

该接口提供数据非对齐搬运的功能，从Global Memory搬运数据至Local Memory时，可以根据开发者的需要自行填充数据。

## 函数原型<a name="section620mcpsimp"></a>

dataCopyParams为DataCopyExtParams类型时：

```
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
```

```
template <typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams)
```

dataCopyParams为DataCopyParams类型时：

```
template<typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
```

```
template<typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams)
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
| T | 操作数以及paddingValue（待填充数据值）的数据类型。各产品支持的数据类型请参考[数据类型](#section4219135304818)。 |
| mode | 配置数据搬运模式。PaddingMode类型，定义如下：<br><pre>enum class PaddingMode : uint8_t {<br>    Normal = 0,  // 默认模式，与原有数据搬运格式保持一致，每次数据搬运都会补齐至32字节对齐<br>    Compact,     // 紧凑模式，允许单次搬运不对齐，统一在整块数据末尾补齐至32字节对齐<br>};</pre><br>该参数仅Ascend 950PR/Ascend 950DT支持。 |

**表 3**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为LocalTensor。<br>起始地址需要保证32字节对齐。 |
| src | 输入 | 源操作数，类型为GlobalTensor。<br>起始地址无地址对齐约束。 |
| dataCopyParams | 输入 | 搬运参数。<br>&bull; DataCopyExtParams类型，具体参数说明请参考表4。<br>&bull; DataCopyParams类型，具体参数说明请参考表5。 |
| padParams | 输入 | 从Global Memory搬运数据至Local Memory时，可以根据开发者需要，在搬运数据左边或右边填充数据。padParams是用于控制数据填充过程的参数。<br>&bull; DataCopyPadExtParams类型，具体参数请参考表6。<br>&bull; DataCopyPadParams类型，具体参数请参考表7。 |

下文表格中列出的结构体参数定义请参考`INSTALL_DIR`/include/ascendc/basic_api/interface/kernel_struct_data_copy.h，`INSTALL_DIR`请替换为CANN软件安装后文件存储路径。

**表 4**  DataCopyExtParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 指定该指令包含的连续传输数据块个数，数据类型为uint16_t，取值范围：blockCount∈[1, 4095]。 |
| blockLen | 指定该指令每个连续传输数据块长度，**该指令支持非对齐搬运**，**每个连续传输数据块长度单位为字节**。数据类型为uint32_t，取值范围：blockLen∈[1, 2097151]**，blockLen必须是 sizeof(T) 的整数倍，需要注意UB不要超限**。 |
| srcStride | 源操作数，相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>源操作数的逻辑位置为GM，单位为字节。<br>数据类型为uint32_t，srcStride不要超出该数据类型的取值范围。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[-blockLen, 2^40-1]。 |
| dstStride | 目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>目的操作数的逻辑位置为VECIN/VECOUT，单位为dataBlock(32字节)。<br>数据类型为uint32_t，dstStride不要超出该数据类型的取值范围。<br>特别地，针对Ascend 950PR/Ascend 950DT，数据类型为int64_t，取值范围为[0, 65535]。 |
| rsv | 保留字段。 |

**表 5**  DataCopyParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 指定该指令包含的连续传输数据块个数，数据类型为uint16_t，取值范围：blockCount∈[1, 4095]。 |
| blockLen | 指定该指令每个连续传输数据块长度，**该指令支持非对齐搬运**，**每个连续传输数据块长度单位为字节**。数据类型为uint16_t，blockLen不要超出该数据类型的取值范围，**blockLen必须是 sizeof(T) 的整数倍**。 |
| srcStride | 源操作数，相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔），源操作数的逻辑位置为GM，单位为字节。数据类型为uint16_t，srcStride不要超出该数据类型的取值范围。 |
| dstStride | 目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔），目的操作数的逻辑位置为VECIN/VECOUT，单位为dataBlock(32字节)。数据类型为uint16_t，dstStride不要超出该数据类型的取值范围。 |

**表 6**  DataCopyPadExtParams&lt;T&gt;结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| isPad | 是否需要填充用户自定义的数据，取值范围：true，false。<br>true：填充padding value。<br>false：表示用户不需要指定填充值，会默认填充随机值。 |
| leftPadding | 连续搬运数据块左侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| rightPadding | 连续搬运数据块右侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| paddingValue | 左右两侧需要填充的数据值，需要保证在数据占用字节范围内。<br>数据类型和源操作数保持一致，T数据类型。<br>**当数据类型长度为64位时，该参数只能设置为0。** |

**表 7**  DataCopyPadParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| isPad | 是否需要填充用户自定义的数据，取值范围：true，false。<br>true：填充padding value。<br>false：表示用户不需要指定填充值，会默认填充随机值。 |
| leftPadding | 连续搬运数据块左侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| rightPadding | 连续搬运数据块右侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。** |
| paddingValue | 左右两侧需要填充的数据值，需要保证在数据占用字节范围内。<br>uint64_t数据类型，且该参数只能设置为0。 |

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

- 配置示例3：
    - blockLen为48，每个连续传输数据块包含48字节；srcStride为-48，表示源操作数相邻数据块之间间隔-48字节，相当于每次传输的连续数据块都是同一块；dstStride为1，表示目的操作数相邻数据块之间间隔1个dataBlock。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding、rightPadding均为0：dummy会默认填充待搬运数据块的第一个元素值。
    - blockLen+leftPadding+rightPadding不满足32字节对齐，leftPadding/rightPadding不为0：若isPad为false，左右两侧填充的数据值和dummy值均为随机值；否则为paddingValue。

    ![](../../../../../figures/datacopypad.png)

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

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- leftPadding、rightPadding的字节数均不能超过32字节。

## 调用示例<a name="section177231425115410"></a>

本示例实现了GM->VECIN的非对齐搬运过程，完整使用样例请参见[DataCopyPad样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm)。

```
AscendC::DataCopyExtParams copyParams{1, 20 * sizeof(half), 0, 0, 0};
AscendC::DataCopyPadExtParams<half> padParams{true, 0, 2, 0};
// 从GM->VECIN搬运40字节
AscendC::DataCopyPad(dstGlobal, src0Global, copyParams, padParams);
```

结果示例：

```
输入数据src0Global: [1 2 3 ... 32]
输出数据dstGlobal:[1 2 3 ... 20]
```
