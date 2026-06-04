# UB->L1非对齐数据搬运（DataCopyPad）

## 产品支持情况<a name="section1550532418810"></a>

|产品|是否支持|
|---|:---:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|√|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|√|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品(AI Core)|x|
|Atlas 推理系列产品(Vector Core)|x|
|Atlas 训练系列产品|x|

## 功能说明<a name="section618mcpsimp"></a>

> [!NOTE]说明 
> 本接口为软件仿真实现，是在Matmul高阶API的基础上，利用Matmul高阶API中的workspace GM空间作为数据中转空间，数据先搬入GM，再搬入L1。因此，在使用本接口时，需要先使用REGISSTER_MATMUL注册高阶API。

该接口提供从Unified Buffer到L1 Buffer的数据非对齐搬运功能。

## 函数原型<a name="section620mcpsimp"></a>

- 通路：Local Memory-\>Local Memory，实际搬运过程是VECIN/VECOUT-\>GM-\>TSCM

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

<a name="table4835205712588"></a>

|参数名|描述|
|---|---|
|T|操作数以及paddingValue（待填充数据值）的数据类型。<br>Ascend 950PR/Ascend 950DT，支持的数据类型为：bool、int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double|

**表 2**  接口参数说明

<a name="zh-cn_topic_0235751031_table33761356"></a>

|参数名|输入/输出|描述|
|---|---|---|
|dst|输出|目的操作数，类型为LocalTensor。<br>LocalTensor的起始地址需要保证32字节对齐。|
|src|输入|源操作数，类型为LocalTensor。<br>LocalTensor的起始地址需要保证32字节对齐。<br>GlobalTensor的起始地址无地址对齐约束。|
|dataCopyParams|输入|搬运参数。<br>&bull; DataCopyExtParams类型，具体参数说明请参考[表4](#table10572141063919)。<br>&bull;|
|nd2nzParams|输入|从VECIN/VECOUT->TSCM进行数据搬运时，可以进行ND到NZ的数据格式转换。nd2nzParams是用于控制数据格式转换的参数，Nd2NzParams类型，具体参数请参考[UB->L1随路转换-ND2NZ搬运（DataCopy）](UB--L1随路转换-ND2NZ搬运（DataCopy）.md)中的介绍。<br>**注意：本接口Nd2NzParams的ndNum仅支持设置为1**。|

**表 3**  DataCopyExtParams结构体参数定义

<a name="table10572141063919"></a>

|参数名称|含义|
|---|---|
|blockCount|指定该指令包含的连续传输数据块个数，数据类型为uint16_t，取值范围：blockCount∈[1, 4095]。|
|blockLen|指定该指令每个连续传输数据块长度，**该指令支持非对齐搬运**，**每个连续传输数据块长度单位为字节**。数据类型为uint32_t，取值范围：blockLen∈[1, 2097151]**，blockLen必须是 sizeof(T) 的整数倍，需要注意UB不要超限**。|
|srcStride|源操作数，相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>**如果源操作数的逻辑位置为VECIN/VECOUT，则单位为dataBlock(32字节)。如果源操作数的逻辑位置为GM，则单位为字节**。<br>数据类型为uint32_t，srcStride不要超出该数据类型的取值范围。<br>特别地，针对如下型号：<br>Ascend 950PR/Ascend 950DT<br>数据类型为int64_t，如果源操作数的逻辑位置为VECIN/VECOUT，取值范围为[0, 65535]；如果源操作数的逻辑位置为GM，取值范围为[-blockLen, 2^40-1]。|
|dstStride|目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔）。<br>**如果目的操作数的逻辑位置为VECIN/VECOUT，则单位为dataBlock(32字节)，如果目的操作数的逻辑位置为GM，则单位为字节**。<br>数据类型为uint32_t，dstStride不要超出该数据类型的取值范围。<br>特别地，针对如下型号：<br>Ascend 950PR/Ascend 950DT<br>数据类型为int64_t，如果目的操作数的逻辑位置为VECIN/VECOUT，取值范围为[0, 65535]；如果目的操作数的逻辑位置为GM，取值范围为[0, 2^40-1]。|
|rsv|保留字段。|

**表 4**  DataCopyPadParams结构体参数定义

<a name="table990103991413"></a>

|参数名称|含义|
|---|---|
|isPad|是否需要填充用户自定义的数据，取值范围：true，false。<br>true：填充padding value。<br>false：表示用户不需要指定填充值，会默认填充随机值。|
|leftPadding|连续搬运数据块左侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。**|
|rightPadding|连续搬运数据块右侧需要补充的数据范围，单位为元素个数。<br>**leftPadding、rightPadding所占的字节数均不能超过32字节。**|
|paddingValue|左右两侧需要填充的数据值，需要保证在数据占用字节范围内。<br>uint64_t数据类型，且该参数只能设置为0。|

下面分别给出如下场景的配置示例：

- [VECIN/VECOUT-\>TSCM](#li1475016332217)
- [搬运模式的配置示例](#li1975762118172)

- <a name="li1475016332217"></a>**VECIN/VECOUT-\>TSCM**

    **注意：**内部实现涉及AIC和AIV之间的通信，实际搬运路径为VECIN/VECOUT-\>GM-\>TSCM，**发送通信消息会有开销，性能会受到影响**。

    如[图1 VECIN/VECOUT-\>TSCM搬运示意图](#fig9329040132719)所示，展示了从VECIN/VECOUT搬运到GM，再搬运到TSCM的过程：示例中数据类型为half，单个datablock（32字节）含有16个half元素，源操作数中的A1\~A6、B1\~B6、C1\~C6为需要进行搬运的数据。

    - 从VECIN/VECOUT-\>GM的搬运，数据存储格式没有发生转变，依然是ND。
        - **blockCount**为需要搬运的连续传输数据块个数，设置为3；
        - **blockLen**为一个连续传输数据块的大小（单位为字节），设置为6 \* 32 = 192；
        - **srcStride**为源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面数据块的头的间隔），源操作数逻辑位置为VECIN/VECOUT，其单位为datablock，两个连续传输数据块（A1\~A6、B1\~B6）中间相隔1个A7，因此srcStride设置为1；
        - **dstStride**为目的操作数，相邻连续数据块间的间隔（前面一个数据块的尾与后面数据块的头的间隔），目的操作数逻辑位置为GM，其单位为字节，两个连续传输数据块（A1\~A6、B1\~B6）中间相隔2个空白的datablock，因此dstStride设置为64字节。

    - 从GM-\>TSCM的搬运，数据存储格式由ND转换为NZ。
        - **ndNum**固定为1，即A1\~A6、B1\~B6、C1\~C6视作一整个ndMatrix；
        - **nValue**为ndMatrix的行数，即为3行；
        - **dValue**为ndMatrix中一行包含的元素个数，即为6 \* 16 = 96个元素；
        - **srcNdMatrixStride**为相邻ndMatrix之间的距离，因为仅涉及1个ndMatrix，所以可填为0；
        - **srcDValue**表明ndMatrix的第x行和第x+1行所相隔的元素个数，如A1\~B1的距离，即为8个datablock，8 \* 16 = 128个元素；
        - **dstNzC0Stride**为src同一行的相邻datablock在NZ矩阵中相隔datablock数，如A1\~A2的距离，即为7个datablock （A1 + 空白 + B1 + 空白 + C1 + 空白 \* 2）；
        - **dstNzNStride**为src中ndMatrix的相邻行在NZ矩阵中相隔多少个datablock，如A1\~B1的距离，即为2个datablock（A1 + 空白）；
        - **dstNzMatrixStride**为相邻NZ矩阵之间的元素个数，因为仅涉及1个NZ矩阵，所以可以填为1。

    **图 1**  VECIN/VECOUT-\>TSCM搬运示意图<a name="fig9329040132719"></a>  
    ![](../../../../../figures/VECIN-VECOUT--TSCM搬运示意图.png "VECIN-VECOUT--TSCM搬运示意图")

- <a name="li1975762118172"></a>搬运模式的配置示例
    - Normal模式

        blockLen为48，每个连续传输数据块包含48字节；srcStride为0，因为源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间紧密排列；dstStride为0，因为目的操作数的逻辑位置为VECIN、VECOUT，dstStride的单位为DataBlock（32字节），目的操作数相邻数据块之间无间隔，注意数据块包含leftPadding/rightPadding数据。

        blockLen + leftPadding + rightPadding满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值，否则为paddingValue。此处示例中，leftPadding为0，rightPadding为16，每个连续传输数据块都会在右侧填充16字节。目的操作数的总长度为192字节。

    - Compact模式

        blockLen为48，每个连续传输数据块包含48字节；srcStride为0，因为源操作数的逻辑位置为GM，srcStride的单位为字节，即源操作数相邻数据块之间紧密排列；dstStride为0，因为目的操作数的逻辑位置为VECIN、VECOUT，dstStride的单位为DataBlock（32字节），目的操作数相邻数据块之间紧密排列，不会填充数据

        blockLen \* blockCount + leftPadding + rightPadding 满足32字节对齐，isPad为false，左右两侧填充的数据值会默认为随机值，否则为paddingValue。此处示例中，leftPadding为0，rightPadding为16，在最后一个数据块右侧填充16字节。目的操作数的总长度为160字节。

        ![](../../../../../figures/paddingMode.png)

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- leftPadding、rightPadding的字节数均不能超过32字节。
- 针对Ascend 950PR/Ascend 950DT，VECIN/VECOUT-\>TSCM通路，仅支持如下数据类型：half、bfloat16\_t、int16\_t、uint16\_t、float、int32\_t、uint32\_t、int8\_t、uint8\_t、int64\_t、uint64\_t、double。

## 调用示例<a name="section177231425115410"></a>

无
