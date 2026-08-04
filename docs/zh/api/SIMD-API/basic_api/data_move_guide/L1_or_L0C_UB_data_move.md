# L1_or_L0C_UB_data_move

## overall_description<a name="ZH-CN_TOPIC_0000002574022823"></a>

L1 Buffer/L0C Buffer与Unified Buffer之间的data_move提供了灵活的接口支持，能够充分适配多样化的计算场景（见[表1](#zh-cn_topic_0000002534897870_table1417935217588)）。其中，Unified Buffer-\>L1 Buffer的data_move用于cube_compute（参考[cube_compute_load](../cube_compute_ISASI/cube_compute_load/cube_compute_load.md)），L0C Buffer-\>Unified Buffer、L1 Buffer-\>Unified Buffer用于将cube_compute结果搬出，完成cube_compute的输出流程（参考[cube_compute_store](../cube_compute_ISASI/cube_compute_store/cube_compute_store.md)）。

**表 1**  L1 Buffer/L0C Buffer与Unified Bufferdata_move接口overview<a name="zh-cn_topic_0000002534897870_table1417935217588"></a>

| data_path | 功能 | 描述 |
|----------|------|------|
| Unified Buffer->L1 Buffer | 连续data_move(DataCopy) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续data_move。 |
| Unified Buffer->L1 Buffer | high_dim_splitdata_move(DataCopy) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的data_move。 |
| Unified Buffer->L1 Buffer | 随路转换-[ND2NZ](<../cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md#ZH-CN_TOPIC_0000002568950893>)搬运(DataCopy) | 支持在data_move时进行[ND](<../cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md#zh-cn_topic_0000002545089965_section958745018719>)到[NZ](<../cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md#zh-cn_topic_0000002545089965_section363412741113>)格式的转换。 |
| Unified Buffer->L1 Buffer | 非对齐data_move(DataCopyPad) | 支持数据非对齐搬运，可以根据开发者的需要自行填充数据。 |
| L0C Buffer->Unified Buffer | 随路转换与量化(DataCopy) | 支持多种随路能力的组合，DataCopy接口内包含了设置寄存器与data_move能力。 |
| L0C Buffer->Unified Buffer | accompanying_quantization激活搬运(FixPipe) | 支持多种随路能力的组合，FixPipe接口内包含了设置寄存器与data_move能力。 |
| L1 Buffer->Unified Buffer | 连续data_move(DataCopyL1ToUB) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续data_move。 |
| L1 Buffer->Unified Buffer | high_dim_splitdata_move(DataCopyL1ToUB) | 提供基础的data_move能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的data_move。 |

## DataCopy（UBToL1连续data_move）

DataCopy能够实现Unified Buffer到L1 Buffer的连续data_move，data_move时格式和内容保持不变。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopy（UBToL1连续data_move）](../cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md)。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

## DataCopy（UBToL1high_dim_splitdata_move）

DataCopy能够实现Unified Buffer到L1 Buffer的连续data_move和非连续data_move，data_move时格式和内容保持不变。

high_dim_split是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopy（UBToL1high_dim_splitdata_move）](../cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_highdim_split.md)。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```

## DataCopy（UBToL1随路转换-ND2NZ搬运）

该接口主要实现将数据从Unified Buffer搬运至L1 Buffer，并支持在data_move时进行[ND](../cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md)到[NZ](../cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md)格式的转换。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopy（UBToL1随路转换-ND2NZ搬运）](../cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_ND2NZ.md)。

src和dst分别为源操作数和目的操作数；intriParams为ND2NZ搬运配置参数。

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Nd2NzParams& intriParams)
```

## DataCopyPad（UBToL1非对齐data_move）

该接口主要实现将数据从Unified Buffer搬运至L1 Buffer，并支持在data_move时提供非对齐data_move功能，可以根据开发者的需要自行填充数据。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- Unified Buffer -> L1 Buffer
    - UB -> C1

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopyPad（UBToL1非对齐data_move）](../cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md)。

src和dst分别为源操作数和目的操作数；dataCopyParams和nd2nzParams为搬运参数。

```cpp
template <typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)
```

<!-- npu="310p" id1 -->
## DataCopy（L0CToUBaccompanying_quantization激活搬运）

该接口主要实现将数据从L0C Buffer搬运至Unified Buffer，并支持多种随路能力的组合，接口内包含了设置寄存器与data_move能力。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L0C Buffer -> Unified Buffer
    - CO1 -> UB

搬运的数据为[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)的结果，接口具体介绍请参考：[DataCopy（L0C到UBdata_move）](../cube_compute_ISASI/cube_compute_store/DataCopy_L0CToUB.md)。

该接口仅Atlas 推理系列产品AI Core支持。

src和dst分别为源操作数和目的操作数；intriParams和enhancedParams为搬运参数。

- 源操作数和目的操作数类型一致

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
    ```

- 源操作数和目的操作数类型不一致

    ```cpp
    template <typename T, typename U>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
    ```
<!-- end id1 -->

## Fixpipe（L0CToUBaccompanying_quantization激活搬运）

该接口主要实现将数据从L0C Buffer搬运至Unified Buffer，并支持多种随路能力的组合，接口内包含了设置寄存器与data_move能力。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L0C Buffer -> Unified Buffer
    - CO1 -> UB

搬运的数据为[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)的结果，以Ascend 950PR/Ascend 950DT为例，接口示例如下：

注意，不同产品型号的接口原型可能不同，具体介绍请参考：[Fixpipe（L0C到UBdata_move）](../cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)。

src和dst分别为源操作数和目的操作数；intriParams为搬运参数，cbufWorkspace为开启tensor量化时所需的量化参数。

- 不开启随路[tensor量化](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

- 开启随路[tensor量化](../cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

<!-- npu="950" id2 -->
## DataCopyL1ToUB（L1ToUB连续data_move）

DataCopyL1ToUB能够实现L1 Buffer到Unified Buffer的连续data_move，data_move时格式和内容保持不变。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L1 Buffer -> Unified Buffer
    - C1 -> UB

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopyL1ToUB（L1到UBdata_move）](../cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md)。

该接口仅Ascend 950PR/Ascend 950DT支持。

src和dst分别为源操作数和目的操作数；count为连续搬运的元素个数。

```cpp
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```
<!-- end id2 -->

<!-- npu="950" id3 -->
## DataCopyL1ToUB（L1ToUBhigh_dim_splitdata_move）

DataCopyL1ToUB能够实现L1 Buffer到Unified Buffer的连续data_move和非连续data_move，data_move时格式和内容保持不变。

high_dim_split是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。

具体支持的data_path为（以[逻辑位置TPosition](../../basic_api/aux_data_structures/TPosition.md)表示）：

- L1 Buffer -> Unified Buffer
    - C1 -> UB

搬运的数据用于[cube_compute](../cube_compute_ISASI/cube_compute_ISASI.md)，接口具体介绍请参考：[DataCopyL1ToUB（L1到UBdata_move）](../cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md)。

该接口仅Ascend 950PR/Ascend 950DT支持。

src和dst分别为源操作数和目的操作数；repeatParams为搬运参数。

```cpp
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
```
<!-- end id3 -->
