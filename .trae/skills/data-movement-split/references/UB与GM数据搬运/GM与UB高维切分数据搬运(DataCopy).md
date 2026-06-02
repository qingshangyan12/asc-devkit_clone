# GM与UB高维切分数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002382908021"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| :--- | :---: |
|<cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter>|
|<cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
|<cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | √ </cann-filter>|
|<cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | √ </cann-filter>|
|<cann-filter npu-type="910"> Atlas 训练系列产品 | √ </cann-filter>|
|<cann-filter npu-type="x90"> Kirin X90 | √ </cann-filter>|
|<cann-filter npu-type="9030"> Kirin 9030 | √ </cann-filter>|

## 功能说明<a name="section474617392321"></a>

提供高维切分数据搬运能力，支持GM与UB之间的双向数据搬运（GM->UB和UB->GM），通过配置DataCopyParams参数支持非连续和连续的数据搬运，数据在传输过程中保持原始格式和内容不变。

## 函数原型<a name="section1954364615315"></a>

```
// GM -> UB
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const DataCopyParams& repeatParams)

// UB -> GM
template <typename T>
__aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 操作数的数据类型。支持的数据类型请参考[数据类型](#section4219135304818)。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| :--- | :---: | :--- |
| dst | 输出 | 目的操作数。<br>&bull; 类型为LocalTensor时，存储位置为Unified Buffer，目的地址需要32B对齐。<br>&bull; 类型为GlobalTensor时，存储位置为Global Memory，目的地址需要1B对齐。 |
| src | 输入 | 源操作数。<br>&bull; 类型为GlobalTensor时，存储位置为Global Memory，源地址需要1B对齐。<br>&bull; 类型为LocalTensor时，存储位置为Unified Buffer，源地址需要32B对齐。 |
| repeatParams | 输入 | 搬运参数，[DataCopyParams](#table9182515919)类型。通过该参数可配置搬运的数据块大小、个数、间隔等信息，同时支持非连续和连续搬运。<br>具体定义请参考\${INSTALL_DIR}/include/ascendc/basic_api/interface/kernel_struct_data_copy.h，\${INSTALL_DIR}请替换为CANN软件安装后文件存储路径。 |

**表 3**  DataCopyParams结构体参数定义

| 参数名称 | 含义 |
| :--- | :--- |
| blockCount | 待搬运的连续传输数据块个数。uint16_t类型，取值范围：blockCount∈[1, 4095]。 |
| blockLen | 待搬运的每个连续传输数据块长度，单位为DataBlock（32字节）。uint16_t类型，取值范围：blockLen∈[1, 65535]。 |
| srcGap | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔），单位为DataBlock（32字节）。uint16_t类型，srcGap不要超出该数据类型的取值范围。 |
| dstGap | 目的操作数相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔），单位为DataBlock（32字节）。uint16_t类型，dstGap不要超出该数据类型的取值范围。 |

下面的样例呈现了DataCopyParams结构体参数的使用方法，样例中完成了2个连续传输数据块的搬运，每个数据块含有8个DataBlock，源操作数相邻数据块之间无间隔，目的操作数相邻数据块尾与头之间间隔1个DataBlock。

![](../../../../../figures/repeat-times.png)

## 数据类型<a name="section4219135304818"></a>

GM->UB和UB->GM两个数据通路对同一产品支持的数据类型相同，具体如下：

<cann-filter npu-type="950">

- Ascend 950PR/Ascend 950DT，支持的数据类型为：b8、b16、b32、b64。

</cann-filter>

<cann-filter npu-type="910">

- Atlas 训练系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="310p">

- Atlas 推理系列产品AI Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="310p">

- Atlas 推理系列产品Vector Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="910b">

- Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double。

</cann-filter>

<cann-filter npu-type="A3">

- Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double。

</cann-filter>

<cann-filter npu-type="310b">

- Atlas 200I/500 A2 推理产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t、float、double。

</cann-filter>

<cann-filter npu-type="x90">

- Kirin X90，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

<cann-filter npu-type="9030">

- Kirin 9030，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t、int64_t、uint64_t、half、float、double。

</cann-filter>

## 返回值说明<a name="section44801012174220"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 如果需要执行多个DataCopy指令，且DataCopy的目的地址存在重叠，需要通过调用[PipeBarrier(ISASI)](../../同步控制/核内同步/PipeBarrier(ISASI).md)来插入同步指令，保证多个DataCopy指令的串行化，防止出现异常数据。如下图左侧示意图，执行两个DataCopy指令，搬运的目的GM地址存在重叠，两条搬运指令之间需要通过调用PipeBarrier<PIPE_MTE3>()添加MTE3搬出流水的同步；如下图右侧示意图所示，搬运的目的地址Unified Buffer存在重叠，两条搬运指令之间需要调用PipeBarrier<PIPE_MTE2>()添加MTE2搬入流水的同步。

    ![](../../../../../figures/绘图4.png)

<cann-filter npu-type="910b,A3">

- 针对如下产品型号：Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品，在跨卡通信算子开发场景，DataCopy类接口支持跨卡数据搬运，仅支持HCCS物理链路，不支持其他通路；开发者开发过程中，需要关注涉及卡间通信的物理通路，可通过npu-smi info -t topo命令查询HCCS物理链路。

</cann-filter>

## 调用示例<a name="section122101199486"></a>

- GM -> UB

    ```
    // srcLocal为half类型的LocalTensor，srcGlobal为half类型的GlobalTensor
    // 使用传入DataCopyParams参数的搬运接口，支持连续和非连续搬运
    DataCopyParams intriParams;
    intriParams.blockCount = 1; // 连续数据块个数为1
    intriParams.blockLen = 512 * sizeof(half) / 32; // 连续数据块长度，单位为DataBlock，此处长度为512个half元素
    intriParams.srcGap = 0; // 源操作数做连续搬运
    intriParams.dstGap = 0; // 目的操作数连续排布
    AscendC::DataCopy(srcLocal, srcGlobal, intriParams);
    ```

    结果示例：

    ```
    输入数据srcGlobal：[1 2 3 ... 512]
    输出数据srcLocal：[1 2 3 ... 512]
    ```

- UB -> GM

    ```
    // dstLocal为half类型的LocalTensor，dstGlobal为half类型的GlobalTensor
    // 使用传入DataCopyParams参数的搬运接口，支持连续和非连续搬运
    DataCopyParams intriParams;
    intriParams.blockCount = 1; // 连续数据块个数为1
    intriParams.blockLen = 512 * sizeof(half) / 32; // 连续数据块长度，单位为DataBlock，此处长度为512个half元素
    intriParams.srcGap = 0; // 源操作数做连续搬运
    intriParams.dstGap = 0; // 目的操作数连续排布
    AscendC::DataCopy(dstGlobal, dstLocal, intriParams);
    ```

    结果示例：

    ```
    输入数据dstLocal：[1 2 3 ... 512]
    输出数据dstGlobal：[1 2 3 ... 512]
    ```
