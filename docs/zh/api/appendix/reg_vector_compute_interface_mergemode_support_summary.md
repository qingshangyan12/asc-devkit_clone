# reg_vector_compute_interface_mergemode_support_summary

不支持MergeMode模板参数的接口，下文不罗列。

**表1**  数据搬入接口MergeMode支持情况

| 接口 | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :--- | :----------------------- | :----------------------- |
| Move | MaskMergeMode::MERGING   | 无                       |

**表2**  basic_arithmetic接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------- | :----------------------- | :----------------------- |
| Add       | MaskMergeMode::ZEROING   | MaskMergeMode::MERGING   |
| Sub       | MaskMergeMode::ZEROING   | 无                       |
| Exp       | MaskMergeMode::ZEROING   | 无                       |
| Sqrt      | MaskMergeMode::ZEROING   | 无                       |
| Max       | MaskMergeMode::ZEROING   | MaskMergeMode::MERGING   |
| Min       | MaskMergeMode::ZEROING   | MaskMergeMode::MERGING   |
| Mul       | MaskMergeMode::ZEROING   | 无                       |
| Div       | MaskMergeMode::ZEROING   | 无                       |
| Adds      | MaskMergeMode::ZEROING   | 无                       |
| Maxs      | MaskMergeMode::ZEROING   | 无                       |
| Mins      | MaskMergeMode::ZEROING   | 无                       |
| Muls      | MaskMergeMode::ZEROING   | 无                       |
| Abs       | MaskMergeMode::ZEROING   | 无                       |
| Ln        | MaskMergeMode::ZEROING   | 无                       |
| Log       | MaskMergeMode::ZEROING   | 无                       |
| Log2      | 无                       | MaskMergeMode::ZEROING   |
| Log10     | 无                       | MaskMergeMode::ZEROING   |
| Neg       | MaskMergeMode::ZEROING   | 无                       |
| Relu      | MaskMergeMode::ZEROING   | 无                       |
| Prelu     | MaskMergeMode::ZEROING   | 无                       |
| LeakyRelu | MaskMergeMode::ZEROING   | 无                       |

**表3**  logical_compute接口MergeMode支持情况

| 接口                   | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :--------------------- | :----------------------- | :----------------------- |
| Not（对RegTensor操作） | MaskMergeMode::ZEROING   | 无                       |
| And（对RegTensor操作） | MaskMergeMode::ZEROING   | 无                       |
| Or（对RegTensor操作）  | MaskMergeMode::ZEROING   | 无                       |
| Xor（对RegTensor操作） | MaskMergeMode::ZEROING   | 无                       |
| ShiftRight             | MaskMergeMode::ZEROING   | 无                       |
| ShiftLefts             | MaskMergeMode::ZEROING   | 无                       |
| ShiftLeft              | MaskMergeMode::ZEROING   | 无                       |
| ShiftRights            | MaskMergeMode::ZEROING   | 无                       |

**表4**  composite_compute接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------- | :----------------------- | :----------------------- |
| AbsSub    | MaskMergeMode::ZEROING   | 无                       |
| ExpSub    | MaskMergeMode::ZEROING   | 无                       |
| Axpy      | MaskMergeMode::ZEROING   | 无                       |
| MulDstAdd | MaskMergeMode::ZEROING   | 无                       |
| MulAddDst | MaskMergeMode::ZEROING   | 无                       |

**表5**  type_conversion接口MergeMode支持情况

| 接口     | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :------- | :----------------------- | :----------------------- |
| Cast     | MaskMergeMode::ZEROING   | 无                       |
| Truncate | MaskMergeMode::ZEROING   | 无                       |

**表6**  reduction_compute接口MergeMode支持情况

| 接口            | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------------- | :----------------------- | :----------------------- |
| Reduce          | MaskMergeMode::ZEROING   | 无                       |
| ReduceDataBlock | MaskMergeMode::ZEROING   | 无                       |
| PairReduceElem  | MaskMergeMode::ZEROING   | 无                       |

**表7**  data_padding接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值                       | 软件支持的模板参数枚举值 |
| :-------- | :--------------------------------------------- | :----------------------- |
| Duplicate | MaskMergeMode::ZEROING、MaskMergeMode::MERGING | 无                       |

