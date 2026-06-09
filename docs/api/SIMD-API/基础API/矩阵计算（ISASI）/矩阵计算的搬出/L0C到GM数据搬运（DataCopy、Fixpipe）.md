# L0C Buffer到GM数据搬运（DataCopy、Fixpipe）<a id="ZH-CN_TOPIC_0000002569070977"></a>

## 产品支持情况<a id="zh-cn_topic_0000002542828493_section796754519912"></a>

|产品|是否支持|
|----------|:----------:|
|Ascend 950PR/Ascend 950DT|支持包含FixpipeParamsV220/FixpipeParamsArch3510参数的接口，支持DataCopy接口。|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|仅支持包含FixpipeParamsV220参数的接口，支持DataCopy接口。|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|仅支持包含FixpipeParamsV220参数的接口，支持DataCopy接口。|
|Atlas 200I/500 A2 推理产品|仅支持包含FixpipeParamsM300参数的接口，支持DataCopy接口。|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Kirin X90|√|
|Kirin 9030|√|

## 功能说明<a id="zh-cn_topic_0000002542828493_section106841136114319"></a>

头文件路径为："basic\_api/kernel\_operator\_fixpipe\_intf.h"、"basic\_api/kernel\_operator\_data\_copy\_intf.h"。

矩阵计算的结果存放在L0C Buffer，DataCopy和Fixpipe接口用于将结果搬运至Global Memory（GM）中，并且在搬运过程中支持随路格式转换等操作。

<cann-filter npu_type = "A3,910b">

以Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品为例，下图展示了随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径。下图中的F32-\>F16与F32-\>BF16为非量化模式，仅为Cast，其余为随路scalar/tensor量化模式。

**图 1**  L0C2GM流程图<a id="zh-cn_topic_0000002542828493_fig542810249417"></a>  

![](../../../../figures/L0C2GM_Function_Combination.png)

</cann-filter>

## 函数原型<a id="zh-cn_topic_0000002542828493_section82039854412"></a>

### DataCopy

<cann-filter npu_type = "950,A3,910b,310b">

DataCopy矩阵搬出接口支持多种随路能力的组合，需要设置不同的寄存器，配合数据搬运指令开启不同的数据搬运能力，对应的接口如下：

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas 200I/500 A2 推理产品：

- 数据搬运接口，通路L0C Buffer（CO1）-\>GM，配合设置寄存器实现量化和ReLU激活，NZ到ND格式的转换，函数原型为：

    ```cpp
    template <typename T, typename U>
    __aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
    ```

- [SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)：寄存器设置接口，通过调用该接口设置Vector随路量化，其中tensor的每个元素都代表一个量化参数，启用tensor量化时需要设置。

- [SetFixpipePreQuantFlag](./寄存器配置说明/SetFixpipePreQuantFlag.md)：寄存器设置接口，通过调用该接口设置Scalar随路量化参数，此元素代表整个输出矩阵使用的量化参数，启用Scalar量化时需要设置。

- [SetFixpipeNz2ndFlag](./寄存器配置说明/SetFixpipeNz2ndFlag.md)：寄存器设置接口，通过调用该接口设置随路NZ2ND格式转换配置，使用随路NZ2ND需要设置。

</cann-filter>

<cann-filter npu_type = "310b">

针对Atlas 200I/500 A2 推理产品：

- [SetFixPipeClipRelu](./寄存器配置说明/SetFixPipeClipRelu.md)：寄存器设置接口，通过调用该接口设置ClipReLU操作的最大值。

- [SetFixPipeAddr](./寄存器配置说明/SetFixPipeAddr.md)：寄存器设置接口，通过调用该接口设置Elementwise操作时LocalTensor的地址。

</cann-filter>

### Fixpipe

为了方便用户使用，减少学习理解成本，针对用户经常使用的场景组合，提供了矩阵搬出的组合接口Fixpipe，接口内包含了设置寄存器与数据搬运能力，对应的接口如下：

<cann-filter npu_type = "950">

针对Ascend 950PR/Ascend 950DT：

- 通路L0C Buffer（CO1）-\>GM，不开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

- 通路L0C Buffer（CO1）-\>GM，开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

</cann-filter>

<cann-filter npu_type = "950,A3,910b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

- 通路L0C Buffer（CO1）-\>GM，不开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
    ```

- 通路L0C Buffer（CO1）-\>GM，开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams)
    ```

</cann-filter>

<cann-filter npu_type = "310b">

针对Atlas 200I/500 A2 推理产品：

- 通路L0C Buffer（CO1）-\>GM，不开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams)
    ```

- 通路L0C Buffer（CO1）-\>GM，开启随路[tensor量化](./关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsM300& intriParams)
    ```

</cann-filter>

## 参数说明<a id="zh-cn_topic_0000002542828493_section16128134420472"></a>

**DataCopy相关参数说明如下：**

**表 1**  数据搬运DataCopy模板参数说明

| 参数名 | 描述 |
|----------|----------|
| T | 目的操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002542828493_section4219135304818)。 |
| U | 源操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002542828493_section4219135304818)。 |

**表 2**  数据搬运DataCopy接口参数说明

| 参数名称 | 输入/输出 | 含义 |
|----------|----------|----------|
| dst | 输出 | 目的操作数，类型为GlobalTensor，数据格式为NZ、ND格式，ND地址要求满足1字节对齐，NZ地址需要满足32字节对齐。 |
| src | 输入 | 源操作数，类型为LocalTensor，支持的物理地址为L0C Buffer（TPosition为CO1），为Mmad接口计算的结果。数据格式为NZ格式，地址需要满足6对齐。 |
| intriParams | 输入 | 搬运参数，类型为[DataCopyCO12DstParams](#zh-cn_topic_0000002542828493_table35908519282)。 <br>具体定义请参考\$\{INSTALL\_DIR\}/include/ascendc/basic\_api/interface/kernel\_struct\_data_copy.h，\$\{INSTALL\_DIR\}请替换为CANN软件安装后文件存储路径。|

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

**表 3**  DataCopyCO12DstParams结构体参数定义<a id="zh-cn_topic_0000002542828493_table35908519282"></a>

| 参数名称 | 含义 |
|----------|----------|
| sid | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| nSize | 源NZ矩阵在N方向上的大小。取值范围nSize∈[0, 4095]，nSize必须为16的倍数<br>&bull; 对于目的矩阵NZ输出：输出类型为float类型时，若开启[channelSplit功能](./关键特性说明/F32-Channel-Split.md)，nSize必须为8的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| mSize | 源NZ矩阵在M方向上的大小。<br>&bull; 不开启随路NZ2ND功能（[NZ2NZ搬运](./关键特性说明/NZ2NZ.md)）：取值范围为mSize∈[0, 65535]。<br>&bull; 开启随路[NZ2ND](./关键特性说明/NZ2ND.md)功能：取值范围为mSize∈[0, 8192]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| dstStride | &bull; 不开启NZ2ND功能（[NZ2NZ搬运](./关键特性说明/NZ2NZ.md)）：目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0， 单位为datablock（32字节）。<br>&bull; 开启随路[NZ2ND](./关键特性说明/NZ2ND.md)功能：目的ND矩阵每一行中的元素个数，取值不为0 ，单位为element。 |
| srcStride | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]，单位为C0_Size（16*sizeof(T)），T为src的数据类型，其值应填成mSize对16向上取整。 |
| unitFlag | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag。<br>&bull; 1（2'b01）：无效值。<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不复位单元标记位。<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将Mmad指令和Fixpipe指令的unitFlag值设置为2或3。<br>参数设置方案和特性细节可参考：Mmad计算中关键特性说明的[UnitFlag](../矩阵计算/关键特性说明/UnitFlag.md)章节。 |
| clipReluPre | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| eltWiseOp | 此参数用户无需关注，设置为0即可。<br>注：兼容旧款产品接口传入，Atlas A2 训练系列产品/Atlas A2 推理系列产品不做处理 |
| quantPre | 用于控制量化模式，QuantMode_t类型，具体定义如下：<br>&bull; float/int32_t输出此需配置为QuantMode_t::NoQuant。<br>&bull; half/bfloat16_t输出，此参数需配置为QuantMode_t::F322F16/QuantMode_t::F322BF16。<br>&bull; 配置为scalar量化时，需要调用[SetFixpipePreQuantFlag](./寄存器配置说明/SetFixpipePreQuantFlag.md)接口来设置scalar量化参数。<br>&bull; 配置为tensor量化时，需要调用[SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)来设置tensor量化参数，其中tensor量化参数需要通过DataCopy从L1 Buffer搬运至Fixpipe Buffer。<br>注：此参数需要用户手动配置，不会自动推导配置对应量化模式。<br><pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能<br>    F322F16,      // Float32_2_Float16：float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // Float32_2_BFloat16：float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // DeQuant_Float16：int32_t量化成half，scalar量化<br>    VDEQF16,      // Vector_DeQuant_Float16：int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // ReQuant_int8：int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // Vector_ReQuant_int8：int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre> |
| reluPre | 用于配置ReLU操作的模式，类型为uint8_t，取值如下：。<br>&bull; 0：不开启ReLU<br>&bull; 1：Normal ReLU |
| channelSplit | 类型为bool，配置是否开启通道切分功能，仅在L0C Buffer(CO1) -> GM通路下NZ格式float类型输出时生效。<br>&bull; false：不开启<br>&bull; true：开启 |
| nz2ndEn | 类型为bool，配置是否开启NZ2ND的格式转换，仅在L0C Buffer(CO1) -> GM通路生效。<br>如果要开启NZ2ND的功能需要同步调用[SetFixpipeNz2ndFlag](./寄存器配置说明/SetFixpipeNz2ndFlag.md)来设置格式转换的相关配置信息<br>&bull; false：不开启<br>&bull; true：开启 |

</cann-filter>

<cann-filter npu_type = "950,310b">

针对Ascend 950PR/Ascend 950DT、Atlas 200I/500 A2 推理产品：

**表 4**  DataCopyCO12DstParams结构体参数定义（C0取值：一般情况下，C0 = 16；开启channelSplit（channel切分）时，C0 = 8）

|参数名称|含义|
|----------|----------|
|nSize|src横向方向的size大小。<br>&bull; 不开启NZ2ND功能，必须为C0的倍数，此时连续传输数据块的个数为nSize / C0。<br>&bull; 开启NZ2ND功能，不受限制。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|mSize|src纵向方向的size大小。<br>&bull; 不开启NZ2ND功能，连续传输数据块的大小为mSize \* C0个元素的长度。<br>&bull; 开启NZ2ND功能，NZ/ND矩阵的大小为mSize \* nSize。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|dstStride|&bull; 不开启NZ2ND功能<br>dst相邻连续数据片段间隔（前面一个数据块的头与后面数据块的头的间隔），取值不为0。单位为DataBlock（32字节）。<br>&bull; 开启NZ2ND功能<br>dst同一ND矩阵的相邻行的偏移（头与头），取值不为0， 单位为元素。|
|srcStride|&bull; 不开启NZ2ND功能<br>src相邻连续数据片段间隔（前面一个数据块的头与后面数据块的头的间隔），必须为16的倍数。取值范围：srcStride∈[0, 65535]， 单位：C0_Size(C0 * sizeof(U)，U为src的数据类型)。<br>&bull; 开启NZ2ND功能<br>src同一NZ矩阵的相邻Z排布的偏移（头与头），必须为16的倍数，取值范围：srcStride∈[0, 65535]，单位C0_size。|
|quantPre|用于控制量化模式，QuantMode_t类型，具体定义如下。默认值为QuantMode_t::NoQuant，即不开启量化功能。<br>配置为scalar量化时，需要调用[SetFixpipePreQuantFlag](./寄存器配置说明/SetFixpipePreQuantFlag.md)接口来设置scalar量化参数；配置为tensor量化时，需要调用[SetFixPipeConfig](./寄存器配置说明/SetFixPipeConfig.md)来设置tensor量化参数。<pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能<br>    F322F16,      // float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // int32_t量化成half，scalar量化<br>    VDEQF16,      // int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre>|
|reluPre|用于配置ReLU操作的模式，类型为uint8_t，取值如下：<br>&bull; 0：不开启ReLU<br>&bull; 1：Normal ReLU|
|channelSplit|类型为bool，配置是否开启channel切分，对于float类型的dst生效。<br>&bull; false：不开启<br>&bull; true：开启|
|nz2ndEn|类型为bool，配置是否开启NZ2ND的格式转换，仅在CO1 -> GM通路生效。<br>如果要开启NZ2ND的功能需要同步调用[SetFixpipeNz2ndFlag](./寄存器配置说明/SetFixpipeNz2ndFlag.md)来设置格式转换的相关配置信息。<br>&bull; false：不开启<br>&bull; true：开启|
|clipReluPre|<cann-filter npu_type = "310b">用于配置是否开启ClipReLU操作，参数类型为uint8_t，取值如下：0，不开启ClipReLU；1，开启ClipReLU，此时需要调用[SetFixPipeClipRelu](./寄存器配置说明/SetFixPipeClipRelu.md)来设置clipReLU的最大值。<br>&bull; 该操作在随路量化后进行，quantPre配置后才能使用，当前支持的量化模式有F322F16/DEQF16/VDEQF16/QF322B8_PRE/VQF322B8_PRE/REQ8/VREQ8。<br></cann-filter><cann-filter npu_type = "950">该参数仅在Atlas 200I/500 A2 推理产品支持。</cann-filter>|
|eltWiseOp|<cann-filter npu_type = "310b">用于配置是否开启Elementwise操作及操作模式。Elementwise操作是指进行随路量化后，可以逐个元素加/减一个LocalTensor，大小为mSize * nSize，具体LocalTensor地址相关参数需要调用[SetFixPipeAddr](./寄存器配置说明/SetFixPipeAddr.md)来设置。<br>eltWiseOp参数类型为uint8_t，取值如下：<br>&bull; 0：不开启Elementwise<br>&bull; 1：Elementwise Addition<br>&bull; 2：Elementwise Subtraction<br></cann-filter><cann-filter npu_type = "950">该参数仅在Atlas 200I/500 A2 推理产品支持。</cann-filter>|
|sid|预留参数，为后续的功能做保留，开发者暂时无需关注。|

</cann-filter>

**Fixpipe相关参数说明如下：**

**表 5**  Fixpipe模板参数说明

| 参数名 | 描述 |
|----------|----------|
| T/U | 目的操作数/源操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002542828493_section4219135304818)。 |
| config | Fixpipe相关配置参数，类型为FixpipeConfig。取值如下：<br>&bull; **CFG_ROW_MAJOR（默认取值）**：开启NZ2ND，输出数据格式为ND格式。<br>&bull; CFG_NZ：NZ2NZ，输出数据格式为NZ格式。<br><cann-filter npu_type = "950">&bull; CFG_COLUMN_MAJOR：针对Ascend 950PR/Ascend 950DT，开启NZ2DN，输出数据格式为DN格式。<br></cann-filter><pre>struct FixpipeConfig {<br>    CO2Layout format;<br>    bool isToUB; // 用于用户指定目的地址的位置是否是UB <br>};<br>enum class CO2Layout : uint8_t {<br>    NZ = 0, // 输出数据格式仍为NZ格式。<br>    ROW_MAJOR, // 开启NZ2ND，输出数据格式为ND格式。<br><cann-filter npu_type = "950">    COLUMN_MAJOR, // 仅Ascend 950PR/Ascend 950DT支持，开启NZ2DN，输出数据格式为DN格式。<br></cann-filter>};<br>constexpr FixpipeConfig CFG_NZ = {CO2Layout::NZ};<br>constexpr FixpipeConfig CFG_ROW_MAJOR = {CO2Layout::ROW_MAJOR};<cann-filter npu_type = "950"><br>constexpr FixpipeConfig CFG_COLUMN_MAJOR = {CO2Layout::COLUMN_MAJOR}; // 仅Ascend 950PR/Ascend 950DT支持</cann-filter></pre> |
| S | 参数cbufWorkspace的数据类型，即随路量化参数的数据类型。<br>&bull; 当目的操作数、源操作数、cbufWorkspace使用基础数据类型时，模板参数S必须为uint64_t类型，否则编译失败。<br>&bull; 当目的操作数、源操作数、cbufWorkspace使用TensorTrait类型时，模板参数S的LiteType必须为uint64_t类型，否则编译失败。<br>模板参数S后一个模板参数仅用于上述数据类型检查，用户无需关注。 |

**表 6**  Fixpipe参数说明

| 参数名称 | 输入/输出 | 含义 |
|----------|----------|----------|
| dst | 输出 | 目的操作数，类型为GlobalTensor。数据格式为NZ、ND格式。ND地址需要满足1字节对齐，NZ地址需满足32字节对齐。<cann-filter npu_type = "950"><br>针对Ascend 950PR/Ascend 950DT，还支持数据格式为DN，地址需满足32字节对齐。</cann-filter> |
| src | 输入 | 源操作数，类型为LocalTensor，支持的物理地址为L0C Buffer（TPosition为CO1），为Mmad接口计算的结果。数据格式为NZ格式，地址需要满足6对齐。 |
| intriParams | 输入 | Fixpipe搬运参数，具体定义请参考 "basic_api/interface/kernel_struct_fixpipe.h" <br>参数说明请参考Fixpipe搬运参数（FixpipeParamsArch3510、FixpipeParamsV220、FixpipeParamsM300）结构体说明。 |
| cbufWorkspace | 输入 | 量化参数，类型为`LocalTensor<uint64_t>`，支持的物理地址为L1 Buffer（TPosition为C1），地址需满足32字节对齐。<br>&bull; 当quantPre为VDEQF16、VQF322B8_PRE、VREQ8时支持。<br><cann-filter npu_type = "950">&bull; 针对Ascend 950PR/Ascend 950DT，除上述外还有VQF322FP8_PRE、VQF322HIF8_PRE、VQF322HIF8_PRE_HYBRID、VQS322BF16_PRE、VQF322F16_PRE、VQF322BF16_PRE、VQF322F32_PRE支持。<br></cann-filter>quantPre介绍请参考Fixpipe搬运参数结构体中quantPre部分。 |

<cann-filter npu_type = "950">

**表 7**  Fixpipe搬运参数（FixpipeParamsArch3510）结构体说明

| 参数名称 | 数据类型 | 含义 |
|----------|----------|----------|
| nSize | 输入 | 源NZ矩阵在N方向上的大小，取值范围为nSize ∈[0, 4095]。<br>&bull; 若开启channelSplit功能，nSize必须为8的倍数。<br>&bull; 若不开启channelSplit功能，nSize必须为16的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| mSize | 输入 | 源NZ矩阵在M方向上的大小，取值范围为mSize∈[0, 65535]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| srcStride | 输入 | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]， 单位为C0_Size（16\*sizeof(T)，T为src的数据类型）。 |
| dstStride | 输入 | &bull; 不开启NZ2ND功能：目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位为element。<br>&bull; 开启NZ2ND/NZ2DN功能：目的ND矩阵每一行中的元素个数，取值不为0，单位为element。 |
| quantPre | 输入 | QuantMode_t是一个枚举类型，用于控制量化模式，默认值为QuantMode_t::NoQuant，即不开启量化功能。QuantMode_t取值如下：<br>&bull; NoQuant，不开启量化功能<br>&bull; F322F16，float cast成half，cast mode为CAST_RINT模式<br>&bull; F322BF16，float cast成bfloat16_t，cast mode为CAST_RINT模式<br>&bull; DEQF16，int32_t量化成half， scalar量化<br>&bull; VDEQF16，int32_t量化成half，tensor量化<br>&bull; QF322B8_PRE，float量化成uint8_t/int8_t，scalar量化<br>&bull; VQF322B8_PRE，float量化成uint8_t/int8_t，tensor量化<br>&bull; REQ8，int32_t量化成uint8_t/int8_t，scalar量化<br>&bull; VREQ8，int32_t量化成uint8_t/int8_t，tensor量化<br>&bull; QF322FP8_PRE，float量化成fp8_e4m3fn_t，scalar量化<br>&bull; VQF322FP8_PRE，float量化成fp8_e4m3fn_t，tensor量化<br>&bull; QF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，scalar量化<br>&bull; VQF322HIF8_PRE，float量化成hifloat8_t(Half to Away Round)，tensor量化<br>&bull; QF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，scalar量化<br>&bull; VQF322HIF8_PRE_HYBRID，float量化成hifloat8_t(Hybrid Round)，tensor量化<br>&bull; QS322BF16_PRE，int32_t量化成bfloat16_t，scalar量化<br>&bull; VQS322BF16_PRE，int32_t量化成bfloat16_t，tensor量化<br>&bull; QF322F16_PRE，float量化成half，scalar量化<br>&bull; VQF322F16_PRE，float量化成half，tensor量化<br>&bull; QF322BF16_PRE，float量化成bfloat16_t，scalar量化<br>&bull; VQF322BF16_PRE，float量化成bfloat16_t，tensor量化<br>&bull; QF322F32_PRE，float量化成float，scalar量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../高阶API/量化操作/AscendDequant.md)高阶API。<br>&bull; VQF322F32_PRE，float量化成float，tensor量化，该量化模式精度无法达到双万分之一，可以达到双千分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../高阶API/量化操作/AscendDequant.md)高阶API。 |
| deqScalar | 输入 | scalar量化参数，表示单个scale值，quantPre量化模式为scalar量化时需要设置该参数。支持的数据类型为uint64_t。 |
| reluEn | 输入 | 是否开启ReLU的开关：<br>&bull; false：不开启ReLU功能；<br>&bull; true：开启ReLU功能。 |
| unitFlag | 输入 | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag；<br>&bull; 1（2'b01）：保留值；<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不会设置寄存器；<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，会将unitFlag关闭。 |
| params | 输入 | params是类型为TransformParams的成员变量，TransformParams结构体是一个基于模板参数的类型选择器，用于在编译时根据不同的CO2Layout布局类型，自动选择对应的参数类型。<br><pre>template \<CO2Layout format\><br>struct TransformParams {};<br>template \<\><br>struct TransformParams\<CO2Layout::NZ\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = uint8_t;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::ROW_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2NdParams;<br>};<br>template \<\><br>struct TransformParams\<CO2Layout::COLUMN_MAJOR\> {<br>    \_\_aicore\_\_ inline TransformParams(){};<br>    using PARAMS = Nz2DnParams;<br>};</pre><br>CO2Layout布局类型为ROW_MAJOR时， 该指令被定义为从L0C Buffer到目标位置的数据移动，并附带NZ2ND转换，结构体参数如下：<br><pre>struct Nz2NdParams {<br>    uint16_t ndNum = 1;<br>    uint16_t srcNdStride = 0;<br>    uint32_t dstNdStride = 0; <br>};</pre>&bull; ndNum：源NZ矩阵的数目，也就是传输ND矩阵的数目，取值范围为ndNum∈[0, 65535]。**注：ndNum=0表示不执行搬运，该接口将被视为NOP（空操作）。**<br>&bull; srcNdStride：不同NZ矩阵起始地址之间的间隔，取值范围为srcNdStride∈[0, 65535]，单位为C0_SIZE。当ndNum配置为1时，srcNdStride配置为0即可，不生效。<br>&bull; dstNdStride：目的相邻ND矩阵起始地址之间的偏移，取值范围为dstNdStride∈[1, 2^32 -1]，单位为element。当ndNum配置为1时，dstNdStride配置为0即可，不生效。<br><br>CO2Layout布局类型为COLUMN_MAJOR时， 该指令被定义为从L0C Buffer到目标位置的数据移动，并附带NZ2DN转换，结构体参数如下：<br><pre>struct Nz2DnParams {<br>    uint16_t dnNum = 1;<br>    uint16_t srcNzMatrixStride = 0;<br>    uint32_t dstDnMatrixStride = 0;<br>    uint16_t srcNzC0Stride = 0; <br>};</pre>&bull; dnNum：传输DN矩阵的数目，取值范围为dnNum∈[0, 65535]。**注：dnNum=0表示不执行搬运，该接口将被视为NOP（空操作）。**<br>&bull; srcNzMatrixStride：不同源NZ矩阵的偏移（头与头），单位为C0_SIZE。<br>&bull; dstDnMatrixStride：目的相邻DN矩阵起始地址间的偏移，取值范围为dstDnMatrixStride∈[1, 2^32 -1]，单位为element。<br>&bull; srcNzC0Stride：源矩阵NZ分形中相邻行的地址偏移（头与头），单位为C0_SIZE。<br><br>CO2Layout布局类型为NZ时，为普通搬运DMA模式，表示从L0C Buffer到目标位置的正常数据移动。 |
| dualDstCtrl | 输入 | 双目标模式控制。当启用双目标模式控制时，L0C Buffer中的M×N矩阵将被分成两半，并同时写入两个子块（SUB BLOCK）的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。<br>&bull; 2'b00：单目标模式，将整个矩阵写入通过subBlockId参数配置的目标UB。<br>&bull; 2'b01：双目标模式，按M维度拆分，M / 2 \* N写入每个UB，M必须为2的倍数。<br>&bull; 2'b10：双目标模式，按N维度拆分，M \* N / 2写入每个UB，N须为32的倍数。<br>&bull; 2'b11：保留。<br>dualDstCtrl仅支持在普通搬运模式或NZ2ND搬运场景下使用，不支持随路功能场景。 |
| subBlockId | 输入 | 在启用单目标模式时指示目标UB的编号。 |
| isChannelSplit | 输入 | 是否开启通道拆分的功能。默认为false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启ChannelSplit和NZ2ND功能。 |

</cann-filter>

<cann-filter npu_type = "950,A3,910b">

**表 8**  Fixpipe搬运参数（FixpipeParamsV220）结构体说明

|参数名称|数据类型|含义|
|---|---|---|
|nSize|必选输入| 源NZ矩阵在N方向上的大小。取值范围nSize∈[0, 4095]，nSize必须为16的倍数。<br>对于目的矩阵NZ输出：输出类型为float类型时，若开启[channelSplit功能](./关键特性说明/F32-Channel-Split.md)，nSize必须为8的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|mSize|必选输入|源NZ矩阵在M方向上的大小。<br>&bull; 不开启随路NZ2ND功能（[NZ2NZ搬运](./关键特性说明/NZ2NZ.md)），取值范围为mSize∈[0, 65535]。<br>&bull; 开启随路[NZ2ND](./关键特性说明/NZ2ND.md)功能，取值范围为mSize∈[0, 8192]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|srcStride|必选输入|源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]，单位为C0_Size（16*sizeof(T)），T为src的数据类型，其值应填成mSize对16向上取整。|
|dstStride|必选输入|&bull; 不开启NZ2ND功能（[NZ2NZ搬运](./关键特性说明/NZ2NZ.md)）：目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位为datablock（32字节）。<br>&bull; 开启随路[NZ2ND](./关键特性说明/NZ2ND.md)功能：目的ND矩阵每一行中的元素个数，取值不为0，单位为element。|
|[quantPre](./关键特性说明/随路量化.md)|可选输入|用于控制量化模式，QuantMode_t类型，默认值为`QuantMode_t::NoQuant`，具体定义如下：<br>&bull; float/int32_t输出此需配置为`QuantMode_t::NoQuant`;<br>&bull; half/bfloat16_t输出，此参数需配置为`QuantMode_t::F322F16`/`QuantMode_t::F322BF16`;<br>注：此参数需要用户手动配置，不会自动推导配置对应量化模式。<br><pre>enum QuantMode_t<br>{<br>    NoQuant,      // 不开启量化功能<br>    F322F16,      // Float32_2_Float16：float cast成half，cast mode为CAST_RINT模式<br>    F322BF16,     // Float32_2_BFloat16：float cast成bfloat16_t，cast mode为CAST_RINT模式<br>    DEQF16,       // DeQuant_Float16：int32_t量化成half，scalar量化<br>    VDEQF16,      // Vector_DeQuant_Float16：int32_t量化成half，tensor量化<br>    QF322B8_PRE,  // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化<br>    VQF322B8_PRE, // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化<br>    REQ8,         // ReQuant_int8：int32_t量化成int8_t/uint8_t，scalar量化<br>    VREQ8,        // Vector_ReQuant_int8：int32_t量化成int8_t/uint8_t，tensor量化<br>};</pre>|
|deqScalar|可选输入|scalar量化参数，表示单个scale值，quantPre量化模式为[随路量化](./关键特性说明/随路量化.md)时需要设置该参数。支持的数据类型为`uint64_t`。|
|[reluEn](./关键特性说明/随路ReLU.md)|可选输入|是否开启NormReLU的开关：<br>&bull; `false`：不开启NormReLU功能， 默认为`false`；<br>&bull; `true`：开启NormReLU功能。|
|unitFlag|可选输入|unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag；<br>&bull; 1（2'b01）：保留值；<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不复位单元标记位；<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将Mmad指令和Fixpipe指令的unitFlag值设置为2或3。<br>参数设置方案和特性细节可参考：Mmad计算中关键特性说明的[UnitFlag](../矩阵计算/关键特性说明/UnitFlag.md)章节。|
|[channelSplit](./关键特性说明/F32-Channel-Split.md)|可选输入|仅在L0C Buffer(CO1) -> GM NZ float输出时生效，是否开启通道拆分的功能。默认为`false`，不开启该功能。|
|ndNum|可选输入|L0C Buffer上源NZ矩阵的数目，也就是传输ND矩阵的数目，默认为1，取值范围为ndNum∈[0, 65535]，[NZ2ND](./关键特性说明/NZ2ND.md)多个NZ非连续矩阵的个数。<br>**注：ndNum=0表示不执行搬运，该接口将被视为NOP（空操作）。**|
|srcNdStride|可选输入|不同NZ矩阵起始地址之间的间隔，取值范围为srcNdStride∈[0, 512]，单位为数据块16*C0_Size。当ndNum配置为1时，srcNdStride配置为0即可，不生效。|
|dstNdStride|可选输入|目的相邻ND矩阵起始地址之间的偏移，取值范围为dstNdStride∈[0, 65535]，单位为element。当ndNum配置为1时，dstNdStride配置为0即可，不生效。|

</cann-filter>

<cann-filter npu_type = "310b">

**表 9**  Fixpipe搬运参数（FixpipeParamsM300）结构体说明

| 参数名称 | 数据类型 | 含义 |
|----------|----------|----------|
| nSize | 输入 | 源NZ矩阵在N方向上的大小，取值范围为nSize ∈[0, 4095]。<br>&bull; 若开启channelSplit功能，nSize必须为8的倍数。<br>&bull; 若不开启channelSplit功能，nSize必须为16的倍数。<br>**注：nSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| mSize | 输入 | 源NZ矩阵在M方向上的大小。<br>&bull; 不开启NZ2ND功能，取值范围为mSize∈[0, 65535]。<br>&bull; 开启NZ2ND功能，取值范围为mSize∈[0, 8192]。<br>**注：mSize=0表示不执行搬运，该接口将被视为NOP（空操作）。** |
| srcStride | 输入 | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围为srcStride∈[0, 65535]， 单位为C0_Size(16\*sizeof(T)，T为src的数据类型)。 |
| dstStride | 输入 | &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0， 单位为datablock(32字节)。<br>&bull; 开启NZ2ND功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位为element。 |
| quantPre | 输入 | QuantMode_t是一个枚举类型，用于控制量化模式，默认值为QuantMode_t::NoQuant，即不开启量化功能。QuantMode_t取值如下：<br>&bull; NoQuant，不开启量化功能<br>&bull; F322F16，float cast成half，cast mode为CAST_RINT模式<br>&bull; F322BF16，float cast成bfloat16_t，cast mode为CAST_RINT模式<br>&bull; DEQF16，int32_t量化成half，scalar量化<br>&bull; VDEQF16， int32_t量化成half，tensor量化<br>&bull; QF322B8_PRE，float量化成uint8_t/int8_t，scalar量化<br>&bull; VQF322B8_PRE，float量化成uint8_t/int8_t，tensor量化<br>&bull; REQ8，int32_t量化成uint8_t/int8_t，scalar量化<br>&bull; VREQ8，int32_t量化成uint8_t/int8_t，tensor量化 |
| deqScalar | 输入 | scalar量化参数，表示单个scale值，quantPre量化模式为scalar量化时需要设置该参数。支持的数据类型为uint64_t。 |
| ndNum | 输入 | 源NZ矩阵的数目，也就是传输ND矩阵的数目，取值范围为ndNum∈[1, 65535] |
| srcNdStride | 输入 | 不同NZ矩阵起始地址之间的间隔，取值范围为srcNdStride∈[1, 512]，单位为数据块（16 \* C0_Size）。当ndNum配置为1时，srcNdStride配置为0即可，不生效。 |
| dstNdStride | 输入 | 目的相邻ND矩阵起始地址之间的偏移，取值范围为dstNdStride∈[1, 65535]，单位为element。当ndNum配置为1时，dstNdStride配置为0即可，不生效。 |
| reluEn | 输入 | 是否开启ReLU的开关：<br>&bull; false：不开启ReLU功能；<br>&bull; true：开启ReLU功能。 |
| unitFlag | 输入 | unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于在L0C Buffer累加的场景。取值说明如下：<br>&bull; 0（2'b00）：不开启unitFlag；<br>&bull; 1（2'b01）：保留值；<br>&bull; 2（2'b10）：开启unitFlag，硬件执行完指令之后，不会设置寄存器；<br>&bull; 3（2'b11）：开启unitFlag，硬件执行完指令之后，会将unitFlag关闭。 |
| isChannelSplit | 输入 | 是否开启通道拆分的功能。默认为false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启ChannelSplit和NZ2ND功能。 |

</cann-filter>

## 数据类型<a id="zh-cn_topic_0000002542828493_section4219135304818"></a>

**源矩阵与目的矩阵支持的数据类型组合**

**DataCopy接口支持的数据类型组合如下：**

<cann-filter npu_type = "950,A3,910b,310b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas 200I/500 A2 推理产品，DataCopy接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、int32_t |

</cann-filter>

**Fixpipe接口支持的数据类型组合如下：**

<cann-filter npu_type = "950">

针对Ascend 950PR/Ascend 950DT，Fixpipe接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、hifloat8_t、fp8_e4m3fn_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、bfloat16_t、int32_t |

</cann-filter>

<cann-filter npu_type = "A3,910b">

针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，Fixpipe接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、int32_t |

</cann-filter>

<cann-filter npu_type = "310b">

针对Atlas 200I/500 A2 推理产品，Fixpipe接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、uint8_t、half、bfloat16_t、float |
| int32_t | int8_t、uint8_t、half、int32_t |

</cann-filter>

<cann-filter npu_type = "x90">

针对Kirin X90，Fixpipe接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | int8_t、half |
| int32_t | int8_t、half、int32_t |

</cann-filter>

<cann-filter npu_type = "9030">

针对Kirin 9030，Fixpipe接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（GM） |
| ---------- | ---------- |
| float | half |
| int32_t | half |

</cann-filter>

## 返回值说明

无

## 约束说明<a id="zh-cn_topic_0000002542828493_section2045914466492"></a>

- 对于量化输入为float32数据类型的说明如下：
    - 标准的IEEE 754 float32格式为：1bit符号位，8bits指数位，23bits尾数位；当前AI处理器支持的float32格式为：1bit符号位，8bits指数位，10bits尾数位。
    - 如果用户提供的是标准的IEEE 754 float32输入，API内部会处理成处理器支持的float32格式进行计算，此时如果golden数据生成过程中使用的是标准的IEEE 754 float32数据，则可能引入精度不匹配问题，需要修正golden数据的生成，将量化参数的23bits尾数位的低13bits数据位清零再参与量化计算。
- 源矩阵NZ格式地址要求6对齐，目的矩阵ND格式地址要求满足1字节对齐，NZ格式地址需要满足32字节对齐。
- 当搬出的mSize、nSize或者ndNum中的任意一个值为0时，该指令不会被执行。ndNum=0时会报warning。
- 当前不支持随路pReLU操作。
- cbufWorkspace量化Tensor位于L1 Buffer，地址要求32字节对齐。
- 量化和ReLU参数不能为INF/NAN和非规格化数。
- 目标数据不能有重叠。如果对目的地址有重叠写入，硬件不会报告任何警告和错误，也不保证重叠数据的写入顺序。
- unitFlag特性开启需要配合Mmad同时开启。

<cann-filter npu_type = "A3,910b">

- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：
    - nSize∈\[0, 4095\]，目的矩阵NZ输出，nSize必须为16的倍数，并且输出类型为float类型开启channelSplit功能，nSize必须为8的倍数， 输出类型为int4类型时，nSize必须为64的倍数。
    - srcStride∈\[0, 65535\]， 单位：C0\_SIZE（16\*sizeof\(T\)），T为src的数据类型。
    - dstStride需要大于0。
    - ndNum∈\[1, 65535\]。
    - 开启随路NZ2ND功能，mSize∈\[0, 8192\]， 不开启随路NZ2ND功能，mSize∈\[0, 65535\]。
    - srcNdStride∈\[1, 512\]，单位：数据块 16\* C0\_Size。当ndNum配置为1时，srcNdStride配置为0即可，参数不生效。
    - dstNdStride∈\[1, 65535\]，单位：element。当ndNum配置为1时，dstNdStride配置为0即可，参数不生效。
    - 当前quantPre只支持设置QuantMode\_t::NoQuant、QuantMode\_t::F322F16、QuantMode\_t::F322BF16、QuantMode\_t::DEQF16、QuantMode\_t::VDEQF16、QuantMode\_t::QF322B8\_PRE、QuantMode\_t::VQF322B8\_PRE、QuantMode\_t::REQ8、QuantMode\_t::VREQ8。

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

## 调用示例<a id="zh-cn_topic_0000002542828493_section088124295117"></a>

<cann-filter npu_type = "950,A3,910b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品，Fixpipe完整样例请参考[fixpipe\_l0c2gm样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm)；

- 示例：通路L0C Buffer-\>GM。输入A矩阵和B矩阵的数据类型为half，输出C矩阵为float，配置开启NZ2ND的格式转换。

    ```cpp
        AscendC::GlobalTensor<outputType> cGM;
        cGM.SetGlobalBuffer((__gm__ outputType *)c);
    
    #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
        AscendC::FixpipeParamsV220 fixpipeParams;
        fixpipeParams.ndNum = 1;
        fixpipeParams.srcNdStride = 0;
        fixpipeParams.dstNdStride = 0;
    #elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
        AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
    #endif
        fixpipeParams.mSize = baseM;
        fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
        fixpipeParams.nSize = baseN;
        fixpipeParams.dstStride = baseN;
        AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>(cGM, c, fixpipeParams);
    ```

</cann-filter>

<cann-filter npu_type = "950,A3,910b">

针对Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品，DataCopy完整样例请参考[data_copy_l0c2gm](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm)；

- 示例：Mmad含有矩阵乘偏置，左矩阵和右矩阵的数据类型为int8\_t，结果矩阵的数据类型为int32\_t。量化模式DEQF16，Scalar量化参数为2.0，将Mmad计算出的结果由int32\_t量化成half并搬出。

    ```cpp
    // Scalar量化，量化参数为2.0
    float quantScalar = 2.0;
    uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t*>(&quantScalar));
    // 将量化参数的标量写入寄存器，供后续DataCopy指令使用
    AscendC::SetFixpipePreQuantFlag(deqScalar);
    // 创建DataCopy的参数
    AscendC::DataCopyCO12DstParams intriParams;
    intriParams.nSize = n;
    intriParams.mSize = m;
    intriParams.srcStride = CeilAlign(m, CUBE_BLOCK);
    intriParams.dstStride = n;
    intriParams.quantPre = QuantMode_t::DEQF16;
    intriParams.reluPre = 1; // 开启ReLU
    intriParams.nz2ndEn = true; // 开启NZ2ND格式转换
    // 根据intriParams中的参数，执行最终的数据搬运
    AscendC::DataCopy(cGM, cLocal, intriParams);
    ```

- 示例：Mmad含有矩阵乘偏置，左矩阵和右矩阵的数据类型为int8\_t，结果矩阵的数据类型为int32\_t。量化模式VDEQF16，Tensor量化，将Mmad计算出的结果由int32\_t量化成half并搬出。

    ```cpp
    // CeilAlign定义如下
    __aicore__ inline uint16_t CeilAlign(uint16_t numerator, uint16_t denominator) 
    {
        return (numerator + denominator - 1) / denominator * denominator;
    }
    // 将GM中的量化数据 (quantAlphaGM) 拷贝到C1（quantAlphaTensor）
    uint16_t burstLen = CeilAlign(n * sizeof(uint64_t), 128) / AscendC::ONE_BLK_SIZE;
    AscendC::DataCopyParams intriParams{ 1, burstLen, 0, 0 };
    AscendC::DataCopy(quantAlphaTensor, quantAlphaGM, intriParams);
    // 设置同步，确保量化数据拷贝到C1后，执行后续DataCopy指令
    AscendC::SetFlag<AscendC::HardEvent::MTE2_FIX>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_FIX>(EVENT_ID0);
    // 将C1中的量化数据（quantAlphaTensor）拷贝到C2PIPE2GM（fbTensor）
    uint16_t fbufBurstLen = CeilAlign(deqDataSize, 128) / 128;
    AscendC::DataCopyParams dataCopyParams(1, fbufBurstLen, 0, 0);
    AscendC::DataCopy(fbTensor, quantAlphaTensor, dataCopyParams);
    // 将量化参数数据写入寄存器，供后续DataCopy指令使用
    AscendC::SetFixPipeConfig(fbTensor);
    // 创建DataCopy的参数, 
    AscendC::DataCopyCO12DstParams intriParams;
    intriParams.nSize = CeilAlign(n, CUBE_BLOCK);
    intriParams.mSize = m;
    intriParams.srcStride = CeilAlign(m, CUBE_BLOCK);
    intriParams.dstStride = m * C0_SIZE / AscendC::ONE_BLK_SIZE; // C0_SIZE = 32
    intriParams.quantPre = QuantMode_t::VDEQF16;
    intriParams.reluPre = 1; // 开启ReLU
    // 根据intriParams中的参数，执行最终的数据搬运
    AscendC::DataCopy(cGM, cLocal, intriParams);
    ```

</cann-filter>

<cann-filter npu_type = "310b">

针对Atlas 200I/500 A2 推理产品：

- 示例：Mmad含有矩阵乘偏置，左矩阵和右矩阵的数据类型为int8\_t，结果矩阵的数据类型为int32\_t。量化模式DEQF16，scalar量化参数为0.5，将Mmad计算出的结果由int32\_t量化成half并搬出。

    ```cpp
    #ifdef ASCENDC_CPU_DEBUG
    #include "tikicpulib.h"
    #endif
    #include "kernel_operator.h"
    #include "../../instrs/common_utils/register_utils.h"
    template <typename dst_T, typename fmap_T, typename weight_T, typename dstCO1_T> class KernelCubeDataCopy{
    public:
        __aicore__ inline KernelCubeDataCopy(uint16_t CoutIn, uint8_t dilationHIn, uint8_t dilationWIn, QuantMode_t deqModeIn)
        {
            // ceiling of 16
            Cout = CoutIn;
            dilationH = dilationHIn;
            dilationW = dilationWIn;
            C0 = 32 / sizeof(fmap_T);
            C1 = channelSize / C0;
            coutBlocks = (Cout + 16 - 1) / 16;
            ho = H - dilationH * (Kh - 1);
            wo = W - dilationW * (Kw - 1);
            howo = ho * wo;
            howoRound = ((howo + 16 - 1) / 16) * 16;
            featureMapA1Size = C1 * H * W * C0;      // shape: [C1, H, W, C0]
            weightA1Size = C1 * Kh * Kw * Cout * C0; // shape: [C1, Kh, Kw, Cout, C0]
            featureMapA2Size = howoRound * (C1 * Kh * Kw * C0);
            weightB2Size = (C1 * Kh * Kw * C0) * coutBlocks * 16;
            m = howo;
            k = C1 * Kh * Kw * C0;
            n = Cout;
            biasSize = Cout;                  // shape: [Cout]
            dstSize = coutBlocks * howo * 16; // shape: [coutBlocks, howo, 16]
            dstCO1Size = coutBlocks * howoRound * 16;
            fmRepeat = featureMapA2Size / (16 * C0);
            weRepeat = weightB2Size / (16 * C0);
            deqMode = deqModeIn;
        }
        __aicore__ inline void Init(__gm__ uint8_t* fmGm, __gm__ uint8_t* weGm, __gm__ uint8_t* biasGm, __gm__ uint8_t* deqGm, __gm__ uint8_t* eleWiseGm, __gm__ uint8_t* dstGm)
        {
            fmGlobal.SetGlobalBuffer((__gm__ fmap_T*)fmGm);
            weGlobal.SetGlobalBuffer((__gm__ weight_T*)weGm);
            biasGlobal.SetGlobalBuffer((__gm__ dstCO1_T*)biasGm);
            deqGlobal.SetGlobalBuffer((__gm__ uint64_t*)deqGm);
            dstGlobal.SetGlobalBuffer((__gm__ dst_T*)dstGm);
            eleWiseGlobal.SetGlobalBuffer((__gm__ half*)eleWiseGm);
            pipe.InitBuffer(inQueueFmA1, 1, featureMapA1Size * sizeof(fmap_T));
            pipe.InitBuffer(inQueueFmA2, 1, featureMapA2Size * sizeof(fmap_T));
            pipe.InitBuffer(inQueueWeB1, 1, weightA1Size * sizeof(weight_T));
            pipe.InitBuffer(inQueueWeB2, 1, weightB2Size * sizeof(weight_T));
            pipe.InitBuffer(inQueueBiasA1, 1, biasSize * sizeof(dstCO1_T));
            pipe.InitBuffer(inQueueDeqA1, 1, dstCO1Size * sizeof(uint64_t));
            pipe.InitBuffer(inQueueDeqFB, 1, dstCO1Size * sizeof(uint64_t));
            pipe.InitBuffer(outQueueCO1, 1, dstCO1Size * sizeof(dstCO1_T));
            pipe.InitBuffer(inQueueC1, 1, dstSize * sizeof(half));
        }
        __aicore__ inline void Process()
        {
            CopyIn();
            Split();
            Compute();
            CopyOut();
        }
    private:
        __aicore__ inline void CopyIn()
        {
            AscendC::LocalTensor<fmap_T> featureMapA1 = inQueueFmA1.AllocTensor<fmap_T>();
            AscendC::LocalTensor<weight_T> weightB1 = inQueueWeB1.AllocTensor<weight_T>();
            AscendC::LocalTensor<dstCO1_T> biasA1 = inQueueBiasA1.AllocTensor<dstCO1_T>();
            AscendC::DataCopy(featureMapA1, fmGlobal, { 1, static_cast<uint16_t>(featureMapA1Size * sizeof(fmap_T) / 32), 0, 0 });
            AscendC::DataCopy(weightB1, weGlobal, { 1, static_cast<uint16_t>(weightA1Size * sizeof(weight_T) / 32), 0, 0 });
            AscendC::DataCopy(biasA1, biasGlobal, { 1, static_cast<uint16_t>(biasSize * sizeof(dstCO1_T) / 32), 0, 0 });
            inQueueFmA1.EnQue(featureMapA1);
            inQueueWeB1.EnQue(weightB1);
            inQueueBiasA1.EnQue(biasA1);
        }
        __aicore__ inline void Split()
        {
            AscendC::LocalTensor<fmap_T> featureMapA1 = inQueueFmA1.DeQue<fmap_T>();
            AscendC::LocalTensor<weight_T> weightB1 = inQueueWeB1.DeQue<weight_T>();
            AscendC::LocalTensor<fmap_T> featureMapA2 = inQueueFmA2.AllocTensor<fmap_T>();
            AscendC::LocalTensor<weight_T> weightB2 = inQueueWeB2.AllocTensor<weight_T>();
            uint8_t padList[] = {0, 0, 0, 0};
            // load3dv2
            AscendC::LoadData(featureMapA2, featureMapA1, { padList, H, W, channelSize, k, howoRound, 0, 0, 1, 1, Kw, Kh, dilationW, dilationH, false, false, 0 });
            // load2d
            AscendC::LoadData(weightB2, weightB1, { 0, weRepeat, 1, 0, 0, false, 0 });
            inQueueFmA2.EnQue<fmap_T>(featureMapA2);
            inQueueWeB2.EnQue<weight_T>(weightB2);
            inQueueFmA1.FreeTensor(featureMapA1);
            inQueueWeB1.FreeTensor(weightB1);
        }
        __aicore__ inline void Compute()
        {
            AscendC::LocalTensor<fmap_T> featureMapA2 = inQueueFmA2.DeQue<fmap_T>();
            AscendC::LocalTensor<weight_T> weightB2 = inQueueWeB2.DeQue<weight_T>();
            AscendC::LocalTensor<dstCO1_T> dstCO1 = outQueueCO1.AllocTensor<dstCO1_T>();
            AscendC::LocalTensor<dstCO1_T> biasA1 = inQueueBiasA1.DeQue<dstCO1_T>();
            // C = A * B + bias
            // m：左矩阵Height，k：左矩阵Width，n：右矩阵Width
            AscendC::Mmad(dstCO1, featureMapA2, weightB2, biasA1, { m, n, k, true, 0, false, false, false });
            outQueueCO1.EnQue<dstCO1_T>(dstCO1);
            inQueueFmA2.FreeTensor(featureMapA2);
            inQueueWeB2.FreeTensor(weightB2);
        }
        __aicore__ inline void CopyOut()
        {
            AscendC::LocalTensor<dstCO1_T> dstCO1 = outQueueCO1.DeQue<dstCO1_T>();
            // 开启DEQF16量化，量化参数设置为0.5
            float tmp = (float)0.5;
            // 将float的tmp转换成uint64_t的deqScalar
            uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t*>(&tmp));
            bool nz2ndEn = false;
            // nz2nd不开启时，nSize必须为16的倍数
            uint16_t nSize = coutBlocks * 16;
            uint16_t mSize = m;
            // srcStride必须为16的倍数
            uint16_t srcStride = (m + 16 - 1) / 16 * 16;
            // nz2nd不开启时，dstStride为burst头到头的距离，且为32字节对齐
            uint32_t dstStride = m * sizeof(dst_T) * 16 / 32;
            if (nz2ndEn) {
                // nd矩阵的数量为1，src_nd_stride与dst_nd_stride填1
                AscendC::SetFixpipeNz2ndFlag(1, 1, 1);
                // nz2nd开启时，nSize可以不为16的倍数，与Mmad的n保持一致
                nSize = n;
                // nz2nd开启时，dstStride表示同一nd矩阵的相邻连续行的间隔，与n保持一致
                dstStride = nSize;
            };
            // 不开启ReLU与channelSplit
            AscendC::DataCopyCO12DstParams intriParams(nSize, mSize, dstStride, srcStride, deqMode, 0, false, nz2ndEn);
           
            // mov l0c to gm, deq scalar quant
            AscendC::SetFixpipePreQuantFlag(deqScalar);  // 设置量化参数
            AscendC::PipeBarrier<PIPE_FIX>();
            AscendC::DataCopy(dstGlobal, dstCO1, intriParams);
            // // mov l0c to gm, deq tensor quant
            // // 需要额外申请deq tensor的gm空间，将值搬运到workA1
            // AscendC::LocalTensor<uint64_t> workA1 = inQueueDeqA1.AllocTensor<uint64_t>();
            // // deq tensor的size
            // uint16_t deqSize = 128;
            // AscendC::DataCopy(workA1, deqGlobal, deqSize);
            // // deq tensor在fix上的地址
            // AscendC::LocalTensor<uint64_t> deqFB = inQueueDeqFB.AllocTensor<uint64_t>();
            // // l1->fix, burst_len unit is 128Bytes
            // uint16_t fbufBurstLen = deqSize / 128;
            // AscendC::DataCopyParams dataCopyParams(1, fbufBurstLen, 0, 0);
            // AscendC::DataCopy(deqFB, workA1, dataCopyParams);
            // // 设置量化tensor
            // AscendC::SetFixPipeConfig(deqFB);
            // AscendC::PipeBarrier<PIPE_FIX>();
            // // mov l0c to gm，量化操作后开启ClipReLU操作
            // intriParams.clipReluPre = 1; 
            // // 设置clip ReLU的值到寄存器
            // uint64_t clipReluVal = 0x3c00; // value 1, half
            // SetFixPipeClipRelu(clipReluVal);
            // //mov l0c to gm，量化操作后，设置element-wise操作，Add
            // intriParams.eltWiseOp = 1;
            // // 需要额外申请element-wise tensor的gm空间，将值搬到eleWiseTensor
            // AscendC::LocalTensor<half> eleWiseTensor = inQueueC1.AllocTensor<half>();
            // DataCopy(eleWiseTensor, eleWiseGlobal, { 1, static_cast<uint16_t>(sizeof(half) * dst_size / 32), 0, 0 });
            // AscendC::PipeBarrier<PIPE_ALL>();
            // // 将存放element-wise tensor的地址设置到寄存器里
            // SetFixPipeAddr(eleWiseTensor, 1);
    
            // AscendC::DataCopy(dstGlobal, dstCO1, intriParams);
            // inQueueDeqA1.FreeTensor(workA1);
            // inQueueDeqFB.FreeTensor(deqFB);
            // outQueueCO1.FreeTensor(dstCO1);
            // inQueueC1.FreeTensor(eleWiseTensor);
         }
    private:
        AscendC::TPipe pipe;
        // feature map queue
        AscendC::TQue<AscendC::TPosition::A1, 1> inQueueFmA1;
        AscendC::TQue<AscendC::TPosition::A2, 1> inQueueFmA2;
        // weight queue
        AscendC::TQue<AscendC::TPosition::B1, 1> inQueueWeB1;
        AscendC::TQue<AscendC::TPosition::B2, 1> inQueueWeB2;
        // bias queue
        AscendC::TQue<AscendC::TPosition::A1, 1> inQueueBiasA1;
        // deq tensor queue
        AscendC::TQue<AscendC::TPosition::A1, 1> inQueueDeqA1;
        // fb dst of deq tensor
        AscendC::TQue<AscendC::TPosition::C2PIPE2GM, 1> inQueueDeqFB;
        // dst queue
        AscendC::TQue<AscendC::TPosition::CO1, 1> outQueueCO1;
        // element-wise tensor
        AscendC::TQue<AscendC::TPosition::C1, 1> inQueueC1;
        AscendC::GlobalTensor<fmap_T> fmGlobal;
        AscendC::GlobalTensor<weight_T> weGlobal;
        AscendC::GlobalTensor<dst_T> dstGlobal;
        AscendC::GlobalTensor<uint64_t> deqGlobal;
        AscendC::GlobalTensor<dstCO1_T> biasGlobal;
        AscendC::GlobalTensor<half> eleWiseGlobal;
        uint16_t channelSize = 32;
        uint16_t H = 4, W = 4;
        uint8_t Kh = 2, Kw = 2;
        uint16_t Cout;
        uint16_t C0, C1;
        uint8_t dilationH, dilationW;
        uint16_t coutBlocks, ho, wo, howo, howoRound;
        uint32_t featureMapA1Size, weightA1Size, featureMapA2Size, weightB2Size, biasSize, dstSize, dstCO1Size;
        uint16_t m, k, n;
        uint8_t fmRepeat, weRepeat;
        QuantMode_t deqMode = QuantMode_t::NoQuant;
    };
    #define KERNEL_CUBE_DATACOPY(dst_type, fmap_type, weight_type, dstCO1_type, CoutIn, dilationHIn, dilationWIn, deqModeIn)  \
        extern "C" __global__ __aicore__ void cube_datacopy_kernel_##fmap_type(__gm__ uint8_t* fmGm, __gm__ uint8_t* weGm,    \
            __gm__ uint8_t* biasGm, __gm__ uint8_t* deqGm, __gm__ uint8_t* eleWiseGm, __gm__ uint8_t* dstGm)                                             \
        {                                                                                                                     \
            if (g_coreType == AscendC::AIV) {                                                                                 \
                return;                                                                                                       \
            }                                                                                                                 \
            KernelCubeDataCopy<dst_type, fmap_type, weight_type, dstCO1_type> op(CoutIn, dilationHIn, dilationWIn,            \
                deqModeIn);                                                                                                   \
            op.Init(fmGm, weGm, biasGm, deqGm, eleWiseGm, dstGm);                                                                        \
            op.Process();                                                                                                     \
        }
    KERNEL_CUBE_DATACOPY(half, int8_t, int8_t, int32_t, 128, 1, 1, QuantMode_t::DEQF16);
    ```

</cann-filter>
