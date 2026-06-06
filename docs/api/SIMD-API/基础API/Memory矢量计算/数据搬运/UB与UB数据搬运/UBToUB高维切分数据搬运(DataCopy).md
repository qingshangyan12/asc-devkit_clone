# UB->UB高维切分数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002382908021"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| :--- | :---: |
|<cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter>|
|<cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | √ </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter>|
|<cann-filter npu-type="910"> Atlas 训练系列产品 | √ </cann-filter>|
|<cann-filter npu-type="x90"> Kirin X90 | x </cann-filter>|
|<cann-filter npu-type="9030"> Kirin 9030 | x </cann-filter>|

## 功能说明<a name="section474617392321"></a>

头文件路径为：`"basic_api/kernel_operator_data_copy_intf.h"`。

支持UB与UB之间的高维切分数据搬运，数据在传输过程中保持原始格式和内容不变。

高维切分数据搬运可通过配置数据块个数、单个数据块长度、地址偏移等搬运参数，同时支持非连续和连续的数据搬运。

## 函数原型<a name="section1954364615315"></a>

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 操作数的数据类型。支持的数据类型请参考[数据类型](#section4219135304818)。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数，类型为LocalTensor，存储位置为UB (Unified Buffer)，目的地址需要32B对齐。 |
| src | 输入 | 源操作数，类型为LocalTensor，存储位置为UB，源地址需要32B对齐。 |
| repeatParams | 输入 | 搬运参数，DataCopyParams类型，参数说明请参考表3。通过该参数可配置搬运的数据块大小、个数、间隔等信息，同时支持非连续和连续搬运。<br>具体定义请参考\$\{INSTALL\_DIR\}/include/ascendc/basic\_api/interface/kernel\_struct\_data\_copy.h，\$\{INSTALL\_DIR\}请替换为CANN软件安装后文件存储路径。 |

**表 3**  DataCopyParams结构体参数定义

| 参数名称 | 含义 |
| --- | --- |
| blockCount | 搬运的数据块个数。uint16_t类型，取值范围：blockCount∈[0, 4095]，默认值为1。<cann-filter npu-type="910b,A3"><br>**注：blockCount = 0表示不执行搬运，该接口将被视为NOP（空操作）。该说明针对如下型号生效：<br><cann-filter npu-type = "A3">&bull; Atlas A3 训练系列产品/Atlas A3 推理系列产品</cann-filter><br><cann-filter npu-type = "910b">&bull; Atlas A2 训练系列产品/Atlas A2 推理系列产品**</cann-filter> </cann-filter>|
| blockLen | 搬运的每个数据块长度。uint16_t类型，取值范围：blockLen∈[0, 65535]，单位：DataBlock（32B）。<br>**注：blockLen = 0表示不执行搬运，该接口将被视为NOP（空操作）。该说明针对如下型号生效：<br><cann-filter npu-type = "A3">&bull; Atlas A3 训练系列产品/Atlas A3 推理系列产品</cann-filter><br><cann-filter npu-type = "910b">&bull; Atlas A2 训练系列产品/Atlas A2 推理系列产品**</cann-filter> |
| srcGap | 源操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint16_t类型，取值范围：srcGap∈[0, 65535]，单位：DataBlock（32B）。<br>&bull; blockCount = 1时，srcGap无意义，设置为0即可。 |
| dstGap | 目的操作数相邻数据块之间的间隔（即前一个数据块**结束地址**与后一个数据块**起始地址**的差值），uint16_t类型，取值范围：dstGap∈[0, 65535]，单位：DataBlock（32B）。<br>&bull; blockCount = 1时，dstGap无意义，设置为0即可。 |

下面的样例通过两个示例展示连续搬运和非连续搬运的使用方式。

针对连续搬运场景，推荐使用[UB与UB连续数据搬运\(DataCopy\)](UBToUB连续数据搬运(DataCopy).md#ZH-CN_TOPIC_0000002382908021)，以下示例仅展示高维切分数据搬运接口具有连续搬运能力。

以half数据类型为例，源操作数的shape为1 \* 128。

以图1为例，将源操作数中128个元素**连续**搬运至目的操作数。DataCopyParams结构体参数配置如下：

- blockCount = 1，搬运1个数据块，表示连续搬运。
- blockLen = 8，一个数据块长度为8个DataBlock。
- srcGap = 0，源操作数相邻数据块之间的间隔为0。
- dstGap = 0，目的操作数相邻数据块之间的间隔为0。

**图 1**  连续搬运示意图<a name="zh-cn_topic_0000002535088926_fig4621152141711"></a>  
![](../../../../../figures/continuous_data_copy_diagram.png "连续搬运示意图")

以half数据类型为例，源操作数的shape为1 \* 112。

以图2为例，将源操作数中数据**非连续**搬运至目的操作数。DataCopyParams结构体参数配置如下：

- blockCount = 2，搬运2个数据块。
- blockLen = 3，一个数据块的长度为3个DataBlock。
- srcGap = 1，源操作数相邻数据块之间的间隔为1。
- dstGap = 2，目的操作数相邻数据块之间的间隔为2。

**图 2**  非连续搬运示意图<a name="zh-cn_topic_0000002535088926_fig422819202304"></a>  
![](../../../../../figures/discontinuous_data_copy_diagram.png "非连续搬运示意图")

## 数据类型<a name="section4219135304818"></a>

<cann-filter npu-type="950">

- Ascend 950PR/Ascend 950DT，支持的数据类型为：bool、int8_t、uint8_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64。

</cann-filter>

<cann-filter npu-type="910">

- Atlas 训练系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="310p">

- Atlas 推理系列产品AI Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="910b">

- Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double。

</cann-filter>

<cann-filter npu-type="A3">

- Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double。

</cann-filter>

## 返回值说明<a name="section44801012174220"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 位于UB的地址必须32B对齐。
- <cann-filter npu-type = "A3,910b">当DataCopyParams结构体参数blockCount、blockLen任意一个值为0时，该接口将被视为NOP（空操作）。该说明针对如下型号生效：
  - <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品</cann-filter>
  - <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>
</cann-filter>
- DataCopyParams结构体参数的值需在取值范围内：

    **表 4**  DataCopyParams结构体参数取值范围

    | 参数名称 | 取值范围 |
    | --- | --- |
    | blockCount | [0, 4095] |
    | blockLen | [0, 65535] |
    | srcGap | [0, 65535] |
    | dstGap | [0, 65535] |

<cann-filter npu-type="910b,A3">

- 针对如下产品型号：Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品，在跨卡通信算子开发场景，DataCopy类接口支持跨卡数据搬运，仅支持HCCS物理链路，不支持其他通路；开发者开发过程中，需要关注涉及卡间通信的物理通路，可通过`npu-smi info -t topo`命令查询HCCS物理链路。

</cann-filter>

## 调用示例<a name="section122101199486"></a>

- 场景1：[1, 512]数据搬运，mask连续模式，源和目的空间一样大

    ```cpp
    // mask=64表示每次迭代处理64个int32元素
    // repeatTime=8表示迭代8次，共处理512个元素
    // srcStride=1, dstStride=1：同一迭代内DataBlock无间隙
    // srcRepeatSize=8, dstRepeatSize=8：相邻迭代间步长为8个block（每个block 8个int32元素）
    uint64_t mask = 64;
    AscendC::Copy(dstLocal, srcLocal, mask, 8, {1, 1, 8, 8});
    ```

- 场景2：从[18, 64]的数据里搬运[18, 8]的数据量

    ```cpp
    // 源数据shape: [18, 64]，共1152个元素
    // 目的数据shape: [18, 8]，共144个元素
    // 每次迭代处理一行中的8个元素，共18次迭代
    // mask=8：每次迭代处理8个元素
    // repeatTime=18：迭代18次（对应18行）
    // srcStride=1, dstStride=1：同一迭代内无间隙
    // srcRepeatSize=8：源每次迭代后跳过64个元素（跳到下一行起始位置）
    // dstRepeatSize=1：目的每次迭代后跳过8个元素（紧凑排列下一行）
    uint64_t mask = 8;
    AscendC::Copy(dstLocal, srcLocal, mask, 18, {1, 1, 1, 8});
    ```

- 场景3：从[18, 64]搬运[18, 8]数据，Counter模式

    ```cpp
    // 源数据shape: [18, 64]，共1152个元素
    // 目的数据shape: [18, 8]，共144个元素
    // Counter模式下，Mask代表每次Repeat中处理的元素个数
    // mask=144：每次迭代处理144个元素
    // repeatTime=1：迭代1次（对应18行）
    // srcStride=8：源数据两个DataBlock地址步长为8（每次选择每一行的起始DataBlock）
    // dstStride=1：目的数据连续存放无间隙
    // srcRepeatSize=8, dstRepeatSize=8：repeatTime=1场景下该参数无效
    AscendC::SetMaskCount();
    AscendC::SetVectorMask<int32_t, AscendC::MaskMode::COUNTER>(144);
    AscendC::Copy<int32_t, false>(dstLocal, srcLocal, AscendC::MASK_PLACEHOLDER, 1, {1, 8, 8, 8});
    AscendC::SetMaskNorm();
    AscendC::ResetMask();
    ```

完整样例请参考[Copy样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub)。
