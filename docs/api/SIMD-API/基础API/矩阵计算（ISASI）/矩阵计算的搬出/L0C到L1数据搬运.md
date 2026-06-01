# L0C到L1数据搬运<a id="ZH-CN_TOPIC_0000002538231198"></a>

## 产品支持情况<a id="zh-cn_topic_0000002511188540_section796754519912"></a>

|产品|是否支持|
|----------|:----------:|
|Ascend 950PR/Ascend 950DT|支持包含FixpipeParamsV220/FixpipeParamsArch3510参数的接口。|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|仅支持包含FixpipeParamsV220参数的接口。|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|仅支持包含FixpipeParamsV220参数的接口。|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Kirin X90|√|
|Kirin 9030|√|

## 功能说明<a id="zh-cn_topic_0000002511188540_section106841136114319"></a>

头文件路径为: "basic_api/kernel_operator_fixpipe_intf.h"、"basic_api/kernel_operator_data_copy_intf.h"。

矩阵计算的结果存放在L0C Buffer，需要将结果搬运至L1 Buffer中，并且在搬运过程中支持随路量化等操作。

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径如下图所示。

**图 1**  L0C2L1流程图<a id="zh-cn_topic_0000002511188540_fig8956371257"></a>  
![](../../../../figures/L0C2L1_Function_Combination.png)

</cann-filter>

## 函数原型<a id="zh-cn_topic_0000002511188540_section82039854412"></a>

<cann-filter npu_type = "950,A3,910b,310b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas 200I/500 A2 推理产品：

`DataCopy`矩阵搬出接口支持多种随路能力的组合，需要设置不同的寄存器，配合数据搬运指令开启不同的数据搬运能力，对应的接口如下：

- 数据搬运接口，通路L0C(CO1)->L1(C1)，配合设置寄存器实现量化和Relu激活

    ```cpp
    template <typename T, typename U>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
    ```

- 寄存器设置接口，通过调用该接口设置随路量化，随路pRelu参数（AscendC待实现），其中tensor的每个元素都代表一个量化参数，使用Vector量化、pRelu模式（AscendC待实现）时需要设置

    ```cpp
    template <typename T>
    __aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
    
    template <typename T, bool setRelu = false>
    __aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag = false)
    ```

- 寄存器设置接口，通过调用该接口设置随路量化参数，此元素代表整个输出矩阵使用的量化参数，使用Scalar量化时需要设置

    ```cpp
    __aicore__ inline void SetFixpipePreQuantFlag(uint64_t config)
    ```

- 寄存器设置接口，通过调用该接口设置随路NZ2ND格式转换配置，使用随路NZ2ND需要设置

    ```cpp
    __aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
    ```

</cann-filter>

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

- 寄存器设置接口，通过调用该接口设置随路Leaky Relu系数，使用随路Leaky Relu需要设置（注：非正式对外接口，正式待补充，头文件路径为："basic\_api/dav\_c220/kernel\_operator\_cube\_others\_impl.h"）

    ```cpp
    template<typename T>
    __aicore__ inline void SetLreluALPHA(T config)
    ```

</cann-filter>

<cann-filter npu_type = "310b">

针对Atlas 200I/500 A2 推理产品：

- 寄存器设置接口，通过调用该接口设置ClipRelu操作的最大值

    ```cpp
    uint64_t clipReluMaxVal = 0x3c00;
    SetFixPipeClipRelu(clipReluMaxVal); // 开启Relu的情况下，先进行Relu操作，再进行clip，clipReluMaxVal为通过该接口设置的最大值
    ```

- 寄存器设置接口，通过调用该接口设置Elementwise操作时LocalTensor的地址

    ```cpp
    template <typename T>
    __aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride)
    ```

</cann-filter>

为了方便用户使用，减少学习理解成本，针对用户经常使用的场景组合，提供了矩阵搬出的组合接口`Fixpipe`，接口内包含了设置寄存器与数据搬运能力，对应的接口如下：

<cann-filter npu_type = "950">

针对Ascend 950PR/Ascend 950DT：

- 通路L0C(CO1)->L1(C1)，不开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

- 通路L0C(CO1)->L1(C1)，开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

</cann-filter>

<cann-filter npu_type = "950,A3,910b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

- 通路L0C(CO1)->L1(C1)，不开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
    ```

- 通路L0C(CO1)->L1(C1)，开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams)
    ```

</cann-filter>

## 参数说明<a id="zh-cn_topic_0000002511188540_section16128134420472"></a>

**DataCopy相关参数说明如下：**

**表 1**  数据搬运DataCopy模板参数说明

| 参数名 | 描述 |
|----------|----------|
| T | 目的操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002511188540_section4219135304818)。 |
| U | 源操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002511188540_section4219135304818)。 |

**表 2**  数据搬运DataCopy接口参数说明

| 参数名称 | 输入/输出 | 含义 |
|----------|----------|----------|
| dst | 输出 | 目的操作数，类型为LocalTensor，数据格式为NZ格式，地址需要满足32B对齐。 |
| src | 输入 | 源操作数，类型为LocalTensor，支持的物理地址为L0C Buffer（TPosition为CO1），为Mmad接口计算的结果。数据格式为NZ格式，地址需要满足64B对齐。 |
| intriParams | 输入 | 搬运参数，类型为[DataCopyCO12DstParams](#zh-cn_topic_0000002511188540_table35908519282)。<br>具体定义请参考\${INSTALL_DIR}/include/ascendc/basic_api/interface/kernel_struct_data_copy.h，\${INSTALL_DIR}请替换为CANN软件安装后文件存储路径。 |

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

**表 3**  DataCopyCO12DstParams结构体参数定义<a id="zh-cn_topic_0000002511188540_table35908519282"></a>

| 参数名称 | 含义 |
|----------|----------|
| sid | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| nSize | 源NZ矩阵在N方向上的大小。取值范围nSize∈[0, 4095]，nSize必须为16的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| mSize | 源NZ矩阵在M方向上的大小。取值范围为mSize∈[0, 65535]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| dstStride | 目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0， 单位为datablock（32Bytes）。 |
| srcStride | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]，单位为C0_Size（16*sizeof(T)），T为src的数据类型，其值应填成mSize对16向上取整。 |
| unitFlag | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag。<br>&bull; 1（2'b01）：无效值。<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不复位单元标记位。<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将Mmad指令和Fixpipe指令的unitFlag值设置为2或3。<br>参数设置方案和特性细节可参考：Mmad计算中关键特性说明的UnitFlag特性说明章节。 |
| clipReluPre | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| eltWiseOp | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| quantPre | 用于控制量化模式，QuantMode_t类型，具体定义如下：<br>&bull; half/bfloat16_t输出，此参数需配置为QuantMode_t::F322F16/QuantMode_t::F322BF16。<br>&bull; 配置为scalar量化时，需要调用[SetFixpipePreQuantFlag](./寄存器配置说明/SetFixpipePreQuantFlag.md)接口来设置scalar量化参数。<br>&bull; 配置为tensor量化时，需要调用[SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)来设置tensor量化参数，其中tensor量化参数需要通过DataCopy从L1 Buffer搬运至Fixpipe Buffer。<br>注：此参数需要用户手动配置，不会自动推导配置对应量化模式。此通路不支持NoQuant模式。<br><pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能，此通路不支持<br>    F322F16,      // Float32_2_Float16：float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // Float32_2_BFloat16：float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // DeQuant_Float16：int32_t量化成half，scalar量化<br>    VDEQF16,      // Vector_DeQuant_Float16：int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // ReQuant_int8：int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // Vector_ReQuant_int8：int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre> |
| reluPre | 用于配置relu操作的模式，类型为uint8_t，取值如下：<br>配置为Leaky relu时，需要调用[SetLreluALPHA](./寄存器配置说明/SetLreluALPHA.md)（注：非正式对外接口，正式待补充）来设置leaky relu系数；配置为Prelu时，需要调用[SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)来设置Prelu参数（AscendC待实现）。<br>&bull; 0：不开启relu<br>&bull; 1：Normal relu<br>&bull; 2：Leaky relu（AscendC待实现）<br>&bull; 3：Prelu（AscendC待实现） |
| channelSplit | 仅在L0C(CO1) -> GM通路时生效，此通路设置为false即可。 |
| nz2ndEn | 仅在L0C(CO1) -> GM通路时生效，此通路设置为false即可。 |

</cann-filter>

<cann-filter npu_type = "950,310b">

针对Ascend 950PR/Ascend 950DT、Atlas 200I/500 A2 推理产品：

**表 4**  DataCopyCO12DstParams结构体参数定义（C0取值：一般情况下，C0 = 16；开启channelSplit（channel切分）时，C0 = 8）

|参数名称|含义|
|----------|----------|
|nSize|src横向方向的size大小。<br>&bull; 不开启NZ2ND功能，必须为C0的倍数，此时连续传输数据块的个数为nSize / C0。<br>&bull; 开启NZ2ND功能，不受限制。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|mSize|src纵向方向的size大小。<br>&bull; 不开启NZ2ND功能，连续传输数据块的大小为mSize \* C0个元素的长度。<br>&bull; 开启NZ2ND功能，NZ/ND矩阵的大小为mSize \* nSize。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|dstStride|&bull; 不开启NZ2ND功能<br>dst相邻连续数据片段间隔（前面一个数据块的头与后面数据块的头的间隔），取值不为0。单位为dataBlock（32字节）。<br>&bull; 开启NZ2ND功能<br>dst同一ND矩阵的相邻行的偏移（头与头），取值不为0， 单位为元素。|
|srcStride|&bull; 不开启NZ2ND功能<br>src相邻连续数据片段间隔（前面一个数据块的头与后面数据块的头的间隔），必须为16的倍数。取值范围：srcStride∈[0, 65535]， 单位：C0_Size(C0 * sizeof(U)，U为src的数据类型)。<br>&bull; 开启NZ2ND功能<br>src同一NZ矩阵的相邻Z排布的偏移（头与头），必须为16的倍数，取值范围：srcStride∈[0, 65535]，单位C0_size。|
|quantPre|用于控制量化模式，QuantMode_t类型，具体定义如下。默认值为QuantMode_t::NoQuant，即不开启量化功能。<br>配置为scalar量化时，需要调用[SetFixpipePreQuantFlag](./寄存器配置说明/SetFixpipePreQuantFlag.md)接口来设置scalar量化参数；配置为tensor量化时，需要调用[SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)来设置tensor量化参数。<pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能<br>    F322F16,      // float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // int32_t量化成half，scalar量化<br>    VDEQF16,      // int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre>|
|reluPre|用于配置relu操作的模式，类型为uint8_t，取值如下：<br>&bull; 0：不开启relu<br>&bull; 1：Normal relu|
|channelSplit|类型为bool，配置是否开启channel切分，对于float类型的dst生效。<br>&bull; false：不开启<br>&bull; true：开启|
|nz2ndEn|类型为bool，配置是否开启NZ2ND的格式转换，仅在CO1 -> GM通路生效。<br>如果要开启NZ2ND的功能需要同步调用[SetFixpipeNz2ndFlag](./寄存器配置说明/SetFixpipeNz2ndFlag.md)来设置格式转换的相关配置信息。<br>&bull; false：不开启<br>&bull; true：开启|
|<cann-filter npu_type = "310b"> clipReluPre <br>该参数仅在Atlas 200I/500 A2 推理产品支持。|用于配置是否开启ClipRelu操作，参数类型为uint8_t，取值如下：0，不开启ClipRelu；1，开启ClipRelu，此时需要调用[SetFixPipeClipRelu](./寄存器配置说明/SetFixPipeClipRelu.md)来设置clipRelu的最大值。<br>&bull; 该操作在随路量化后进行，quantPre配置后才能使用，当前支持的量化模式有F322F16/DEQF16/VDEQF16/QF322B8_PRE/VQF322B8_PRE/REQ8/VREQ8。 |
|eltWiseOp <br>该参数仅在Atlas 200I/500 A2 推理产品支持。|用于配置是否开启Elementwise操作及操作模式。Elementwise操作是指进行随路量化后，可以逐个元素加/减一个LocalTensor，大小为mSize * nSize，具体LocalTensor地址相关参数需要调用[SetFixPipeAddr](./寄存器配置说明/SetFixPipeAddr.md)来设置。<br>eltWiseOp参数类型为uint8_t，取值如下：<br>&bull; 0：不开启Elementwise<br>&bull; 1：Elementwise Addition<br>&bull; 2：Elementwise Subtraction<br> </cann-filter> |
|sid|预留参数，为后续的功能做保留，开发者暂时无需关注。|

</cann-filter>

**Fixpipe相关参数说明如下：**

**表 5**  Fixpipe模板参数说明

| 参数名 | 描述 |
|----------|----------|
| T/U | 目的操作数/源操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002511188540_section4219135304818)。 |
| config | Fixpipe相关配置参数，类型为FixpipeConfig。取值如下：<br>&bull; **CFG_ROW_MAJOR（默认取值）**：开启NZ2ND，输出数据格式为ND格式。<cann-filter npu_type = "A3,910b">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品，在L0C -> L1通路下不生效。</cann-filter><br>&bull; CFG_NZ：NZ2NZ，输出数据格式为NZ格式。<br><cann-filter npu_type = "950">&bull; CFG_COLUMN_MAJOR：针对Ascend 950PR/Ascend 950DT，开启NZ2DN，输出数据格式为DN格式。<br></cann-filter><pre>struct FixpipeConfig {<br>    CO2Layout format;<br>    bool isToUB; // 用于用户指定目的地址的位置是否是UB <br>};<br>enum class CO2Layout : uint8_t {<br>    NZ = 0, // 输出数据格式仍为NZ格式。<br>    ROW_MAJOR, // 开启NZ2ND，输出数据格式为ND格式。<br><cann-filter npu_type = "950">    COLUMN_MAJOR, // 仅Ascend 950PR/Ascend 950DT支持，开启NZ2DN，输出数据格式为DN格式。<br></cann-filter>};<br>constexpr FixpipeConfig CFG_NZ = {CO2Layout::NZ};<br>constexpr FixpipeConfig CFG_ROW_MAJOR = {CO2Layout::ROW_MAJOR};<cann-filter npu_type = "950"><br>constexpr FixpipeConfig CFG_COLUMN_MAJOR = {CO2Layout::COLUMN_MAJOR}; // 仅Ascend 950PR/Ascend 950DT支持</cann-filter></pre> |
| S | 参数cbufWorkspace的数据类型，即随路量化参数的数据类型。<br>&bull; 当目的操作数、源操作数、cbufWorkspace使用基础数据类型时，模板参数S必须为uint64_t类型，否则编译失败。<br>&bull; 当目的操作数、源操作数、cbufWorkspace使用TensorTrait类型时，模板参数S的LiteType必须为uint64_t类型，否则编译失败。<br>模板参数S后一个模板参数仅用于上述数据类型检查，用户无需关注。 |

**表 6**  Fixpipe参数说明

| 参数名称 | 输入/输出 | 含义 |
|----------|----------|----------|
| dst | 输出 | 目的操作数，类型为LocalTensor。数据格式为NZ格式。NZ地址需满足32B对齐。<cann-filter npu_type = "950"><br>针对Ascend 950PR/Ascend 950DT，还支持数据格式为ND、DN，地址需满足32B对齐。</cann-filter> |
| src | 输入 | 源操作数，类型为LocalTensor，支持的物理地址为L0C Buffer（TPosition为CO1），为Mmad接口计算的结果。数据格式为NZ格式，地址需要满足64B对齐。 |
| intriParams | 输入 | Fixpipe搬运参数，具体定义请参考 "basic_api/interface/kernel_struct_fixpipe.h" <br>参数说明请参考Fixpipe搬运参数（FixpipeParamsArch3510、FixpipeParamsV220）结构体说明。 |
| cbufWorkspace | 输入 | 量化参数，类型为`LocalTensor<uint64_t>`，支持的物理地址为L1 Buffer（TPosition为C1），地址需满足32B对齐。<br>&bull; 当quantPre为VDEQF16、VQF322B8_PRE、VREQ8时支持。<br><cann-filter npu_type = "950">&bull; 针对Ascend 950PR/Ascend 950DT，除上述外还有VQF322FP8_PRE、VQF322HIF8_PRE、VQF322HIF8_PRE_HYBRID、VQS322BF16_PRE、VQF322F16_PRE、VQF322BF16_PRE、VQF322F32_PRE支持。<br></cann-filter>quantPre介绍请参考Fixpipe搬运参数结构体中quantPre部分。 |

<cann-filter npu_type = "950">

**表 7**  Fixpipe搬运参数（FixpipeParamsArch3510）结构体说明

| 参数名称 | 数据类型 | 含义 |
|----------|----------|----------|
| nSize | 输入 | 源NZ矩阵在N方向上的大小，取值范围为nSize ∈[0, 4095]。<br>&bull; 若开启channelSplit功能，nSize必须为8的倍数。<br>&bull; 若不开启channelSplit功能，nSize必须为16的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| mSize | 输入 | 源NZ矩阵在M方向上的大小，取值范围为mSize∈[0, 65535]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| srcStride | 输入 | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]， 单位为C0_Size（16*sizeof(T)，T为src的数据类型）。 |
| dstStride | 输入 | &bull; 不开启NZ2ND功能：目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位为element。<br>&bull; 开启NZ2ND/NZ2DN功能：目的ND矩阵每一行中的元素个数，取值不为0，单位为element。 |
| quantPre | 输入 | QuantMode_t是一个枚举类型，用于控制量化模式，默认值为QuantMode_t::NoQuant，即不开启量化功能。QuantMode_t取值如下：<br>&bull; NoQuant，不开启量化功能<br>&bull; F322F16，float cast成half，cast mode为CAST_RINT模式<br>&bull; F322BF16，float cast成bfloat16_t，cast mode为CAST_RINT模式<br>&bull; DEQF16，int32_t量化成half， scalar量化<br>&bull; VDEQF16，int32_t量化成half，tensor量化<br>&bull; QF322B8_PRE，float量化成uint8_t/int8_t，scalar量化<br>&bull; VQF322B8_PRE，float量化成uint8_t/int8_t，tensor量化<br>&bull; REQ8，int32_t量化成uint8_t/int8_t，scalar量化<br>&bull; VREQ8，int32_t量化成uint8_t/int8_t，tensor量化<br>&bull; QF322FP8_PRE，float量化成fp8_e4m3fn_t，scalar量化<br>&bull; VQF322FP8_PRE，float量化成fp8_e4m3fn_t，tensor量化<br>&bull; QF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，scalar量化<br>&bull; VQF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，tensor量化<br>&bull; QF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，scalar量化<br>&bull; VQF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，tensor量化<br>&bull; QS322BF16_PRE，int32_t量化成bfloat16_t，scalar量化<br>&bull; VQS322BF16_PRE，int32_t量化成bfloat16_t，tensor量化<br>&bull; QF322F16_PRE，float量化成half，scalar量化<br>&bull; VQF322F16_PRE，float量化成half，tensor量化<br>&bull; QF322BF16_PRE，float量化成bfloat16_t，scalar量化<br>&bull; VQF322BF16_PRE，float量化成bfloat16_t，tensor量化<br>&bull; QF322F32_PRE，float量化成float，scalar量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../高阶API/量化操作/AscendDequant.md)高阶API。<br>&bull; VQF322F32_PRE，float量化成float，tensor量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../高阶API/量化操作/AscendDequant.md)高阶API。 |
| deqScalar | 输入 | scalar量化参数，表示单个scale值，quantPre量化模式为scalar量化时需要设置该参数。支持的数据类型为uint64_t。 |
| reluEn | 输入 | 是否开启relu的开关：<br>&bull; false：不开启relu功能；<br>&bull; true：开启relu功能。 |
| unitFlag | 输入 | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag；<br>&bull; 1（2'b01）：保留值；<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不会设置寄存器；<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，会将unitFlag关闭。 |
| TransformParams | 输入 | TransformParams结构体是一个基于模板参数的类型选择器，用于在编译时根据不同的CO2Layout布局类型，自动选择对应的参数类型。<br><pre>template \<CO2Layout format\><br>struct TransformParams {};<br>template \<\><br>struct TransformParams\<CO2Layout::NZ\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = uint8_t;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::ROW_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2NdParams;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::COLUMN_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2DnParams;<br>};</pre><br>CO2Layout布局类型为ROW_MAJOR时， 该指令被定义为从L0C到目标位置的数据移动，并附带NZ2ND转换，结构体参数如下：<br><pre>struct Nz2NdParams {<br>    uint16_t ndNum = 1;<br>    uint16_t srcNdStride = 0;<br>    uint32_t dstNdStride = 0; <br>};</pre>&bull; ndNum：源NZ矩阵的数目，也就是传输ND矩阵的数目，取值范围为ndNum∈[0, 65535]。**注：ndNum=0表示不执行搬运，该接口将被视为NOP（空操作）。**<br>&bull; srcNdStride：不同NZ矩阵起始地址之间的间隔，取值范围为srcNdStride∈[0, 65535]，单位为C0_SIZE。当ndNum配置为1时，srcNdStride配置为0即可，不生效。<br>&bull; dstNdStride：目的相邻ND矩阵起始地址之间的偏移，取值范围为dstNdStride∈[1, 2^32 -1]，单位为element。当ndNum配置为1时，dstNdStride配置为0即可，不生效。<br><br>CO2Layout布局类型为COLUMN_MAJOR时， 该指令被定义为从L0C到目标位置的数据移动，并附带NZ2DN转换，结构体参数如下：<br><pre>struct Nz2DnParams {<br>    uint16_t dnNum = 1;<br>    uint16_t srcNzMatrixStride = 0;<br>    uint32_t dstDnMatrixStride = 0;<br>    uint16_t srcNzC0Stride = 0; <br>};</pre>&bull; dnNum：传输DN矩阵的数目，取值范围为dnNum∈[0, 65535]。**注：dnNum=0表示不执行搬运，该接口将被视为NOP（空操作）。**<br>&bull; srcNzMatrixStride：不同源NZ矩阵的偏移（头与头），单位为C0_SIZE。<br>&bull; dstDnMatrixStride：目的相邻DN矩阵起始地址间的偏移，取值范围为dstDnMatrixStride∈[1, 2^32 -1]，单位为element。<br>&bull; srcNzC0Stride：源矩阵NZ分形中相邻行的地址偏移（头与头），单位为C0_SIZE。<br><br>CO2Layout布局类型为NZ时，为普通搬运DMA模式，表示从L0C到目标位置的正常数据移动。 |
| dualDstCtrl | 输入 | 双目标模式控制。当启用双目标模式控制时，L0C中的M×N矩阵将被分成两半，并同时写入两个子块（SUB BLOCK）的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。<br>&bull; 2'b00：单目标模式，将整个矩阵写入通过subBlockId参数配置的目标UB。<br>&bull; 2'b01：双目标模式，按M维度拆分，M / 2 \* N写入每个UB，M必须为2的倍数。<br>&bull; 2'b10：双目标模式，按N维度拆分，M \* N / 2写入每个UB，N须为32的倍数。<br>&bull; 2'b11：保留。<br>dualDstCtrl仅支持在普通搬运模式或NZ2ND搬运场景下使用，不支持随路功能场景。 |
| subBlockId | 输入 | 在启用单目标模式时指示目标UB的编号。 |
| isChannelSplit | 输入 | 是否开启通道拆分的功能。默认为false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启ChannelSplit和NZ2ND功能。 |

</cann-filter>

<cann-filter npu_type = "950,A3,910b">

**表 8**  Fixpipe搬运参数（FixpipeParamsV220，其中V220为Atlas A2 训练系列产品/Atlas A2 推理系列产品内部使用版本号）结构体说明

|参数名称|数据类型|含义|
|---|---|---|
|nSize|必选输入| 源NZ矩阵在N方向上的大小。取值范围nSize∈[0, 4095]，nSize必须为16的倍数。<br>&bull; 输出类型为int4类型（AscendC待实现）时，nSize必须为64的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|mSize|必选输入|源NZ矩阵在M方向上的大小。取值范围为mSize∈[0, 65535]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|srcStride|必选输入|源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]，单位为C0_Size（16*sizeof(T)），T为src的数据类型，其值应填成mSize对16向上取整。|
|dstStride|必选输入|目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位为datablock（32Bytes）。|
|[quantPre](./关键特性说明/随路量化.md)|可选输入|用于控制量化模式，QuantMode_t类型，默认值为`QuantMode_t::NoQuant`，具体定义如下：<br>&bull; half/bfloat16_t输出，此参数需配置为`QuantMode_t::F322F16`/`QuantMode_t::F322BF16`;<br>注：此参数需要用户手动配置，不会自动推导配置对应量化模式。此通路不支持NoQuant模式。<br><pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能，此通路不支持<br>    F322F16,      // Float32_2_Float16：float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // Float32_2_BFloat16：float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // DeQuant_Float16：int32_t量化成half，scalar量化<br>    VDEQF16,      // Vector_DeQuant_Float16：int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // ReQuant_int8：int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // Vector_ReQuant_int8：int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre>|
|deqScalar|可选输入|scalar量化参数，表示单个scale值，quantPre量化模式为[随路量化](./关键特性说明/随路量化.md)时需要设置该参数。支持的数据类型为`uint64_t`。|
|[reluEn](./关键特性说明/随路relu.md)|可选输入|是否开启NormRelu的开关：<br>&bull; `false`：不开启NormRelu功能， 默认为`false`；<br>&bull; `true`：开启NormRelu功能。|
|unitFlag|可选输入|unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag；<br>&bull; 1（2'b01）：保留值；<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不复位单元标记位；<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将Mmad指令和Fixpipe指令的unitFlag值设置为2或3。<br>参数设置方案和特性细节可参考：Mmad计算中关键特性说明的UnitFlag特性说明章节。|
|[channelSplit](./关键特性说明/F32-Channel-Split.md)|可选输入|仅在L0C(CO1) -> GM通路时生效，此通路设置为false即可。|
|ndNum|可选输入|仅在L0C(CO1) -> GM通路时生效，此通路无需额外设置。|
|srcNdStride|可选输入|仅在L0C(CO1) -> GM通路时生效，此通路无需额外设置。|
|dstNdStride|可选输入|仅在L0C(CO1) -> GM通路时生效，此通路无需额外设置。|

</cann-filter>

## 数据类型<a id="zh-cn_topic_0000002511188540_section4219135304818"></a>

**源矩阵与目的矩阵支持的精度类型组合**

**DataCopy接口支持的精度类型组合如下：**

<cann-filter npu_type = "A3,910b,310b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas 200I/500 A2 推理产品，DataCopy接口支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t |
| int32_t | int8_t、uint8_t、half |

</cann-filter>

<cann-filter npu_type = "950">

针对Ascend 950PR/Ascend 950DT，DataCopy接口支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、int32_t |

</cann-filter>

**Fixpipe接口支持的精度类型组合如下：**

<cann-filter npu_type = "950">

针对Ascend 950PR/Ascend 950DT，支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（L1） |
| ---------- | ---------- |
| float | int8_t、uint8_t、hifloat8_t、fp8_e4m3fn_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、bfloat16_t、int32_t |

</cann-filter>

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（L1） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t |
| int32_t | int8_t、uint8_t、half |

</cann-filter>

<cann-filter npu_type = "x90">

针对Kirin X90，支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（L1） |
| ---------- | ---------- |
| float | int8_t、half |
| int32_t | int8_t、half |

</cann-filter>

<cann-filter npu_type = "9030">

针对Kirin 9030，支持的精度类型组合如下：

| 源矩阵（L0C） | 目的矩阵（L1） |
| ---------- | ---------- |
| float | half |
| int32_t | half |

</cann-filter>

## 返回值说明

无

## 约束说明<a id="zh-cn_topic_0000002511188540_section2045914466492"></a>

- 对于量化输入为float32数据类型的说明如下：
    - 标准的IEEE 754 float32格式为：1bit符号位，8bits指数位，23bits尾数位；当前AI处理器支持的float32格式为：1bit符号位，8bits指数位，10bits尾数位。
    - 如果用户提供的是标准的IEEE 754 float32输入，API内部会处理成处理器支持的float32格式进行计算，此时如果golden数据生成过程中使用的是标准的IEEE 754 float32数据，则可能引入精度不匹配问题，需要修正golden数据的生成，将量化参数的23bits尾数位的低13bits数据位清零再参与量化计算。
- 源矩阵C支持位于L0C Buffer，目的矩阵支持位于L1 Buffer。
- 源矩阵NZ格式地址要求64B对齐，目的矩阵NZ格式地址需要满足32B对齐。
- 当搬出的mSize、nSize或者ndNum中的任意一个值为0时，该指令不会被执行。ndNum=0时会报warning。
- 当前不支持随路pRelu操作。
- 量化和Relu参数不能为INF/NAN和非规格化数。
- 目标数据不能有重叠。如果对目的地址有重叠写入，硬件不会报告任何警告和错误，也不保证重叠数据的写入顺序。
- unitFlag特性开启需要配合Mmad同时开启。

<cann-filter npu_type = "A3,910b">

- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：
    - nSize∈\[0, 4095\]，目的矩阵NZ输出，nSize必须为16的倍数，输出类型为int4类型时，nSize必须为64的倍数。
    - srcStride∈\[0, 65535\]， 单位：C0\_SIZE（16\*sizeof\(T\)），T为src的数据类型。
    - dstStride需要大于0。
    - 不支持开启随路NZ2ND功能、F32 Channel Split功能。
    - 此通路不支持NoQuant量化模式。

- Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品特殊值/边界值约束说明：

    对于浮点类型INF/NAN输入输出，可以通过CTRL寄存器（控制寄存器）的CTRL\[48\]比特位进行设置，控制浮点数量化搬出时的饱和模式：

    - 非饱和模式：CTRL\[48\]设置成1'b1，INF/NAN保持原输出。
    - 饱和模式：CTRL\[48\]设置成1'b0，INF输出会被饱和为±MAX， NaN输出会被饱和为0。

        ```cpp
        // 设置CTRL[48]为0，开启浮点数饱和模式
        AscendC::AscendCUtils::SetOverflow(0);
        ```

    对于整数类型只有饱和模式。

</cann-filter>

## 调用示例<a id="zh-cn_topic_0000002511188540_section088124295117"></a>

<cann-filter npu_type = "950,A3,910b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品，Fixpipe完整样例请参考[fixpipe\_l0c2l1样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1)；

- 示例：通路L0C->L1。输入A矩阵和B矩阵的数据类型为half，输出C矩阵为int8_t，不开启NZ2ND的格式转换，开启scalar量化。

    ```cpp
        AscendC::LocalTensor<outputType> c1Local(AscendC::TPosition::C1, c1Addr, cSizeAlignL1);
    
    #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
        uint16_t c0Size = 32;
        AscendC::FixpipeParamsV220 fixpipeParams;
        // NZ2NZ场景，FixpipeParamsV220的dstStride单位为datablock（32Bytes）
        fixpipeParams.dstStride = baseM * c0Size * sizeof(outputType) / AscendC::ONE_BLK_SIZE;
        fixpipeParams.mSize = baseM;
        fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
        fixpipeParams.nSize = baseN;
        // scalar quant
        fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE;
        uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
        constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
        deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
        fixpipeParams.deqScalar = deqScalar;
        AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
    
    #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
        uint16_t c0Size = 32;
        AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::NZ> fixpipeParams;
        // NZ2NZ场景，FixpipeParamsArch3510的dstStride单位为元素个数
        fixpipeParams.dstStride = baseM * c0Size;
        fixpipeParams.mSize = baseM;
        fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
        fixpipeParams.nSize = baseN;
        // scalar quant
        fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE;
        uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t *>(&quantScalar));
        constexpr bool sign = (AscendC::IsSameType<outputType, int8_t>::value) ? true : false;
        deqScalar = (deqScalar & ~(static_cast<uint64_t>(1) << 46)) | (static_cast<uint64_t>(sign) << 46);
        fixpipeParams.deqScalar = deqScalar;
        AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(c1Local, c, fixpipeParams);
    #endif
    ```

</cann-filter>
