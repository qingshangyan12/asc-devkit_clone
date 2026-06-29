# Reg矢量计算接口MergeMode支持情况汇总

**表1**  数据搬入接口MergeMode支持情况

| 接口                 | 硬件支持模式           | 软件支持模式 |
| :------------------- | :--------------------- | :----------- |
| LoadAlign            | -                      | -            |
| LoadUnAlign          | -                      | -            |
| MaskGenWithRegTensor | -                      | -            |
| Gather               | -                      | -            |
| GatherB              | -                      | -            |
| Load                 | -                      | -            |
| Move                 | MaskMergeMode::MERGING | -            |

**表2**  数据搬出接口MergeMode支持情况

| 接口         | 硬件支持模式 | 软件支持模式 |
| :----------- | :----------- | :----------- |
| StoreAlign   | -            | -            |
| StoreUnAlign | -            | -            |
| Scatter      | -            | -            |
| Store        | -            | -            |

**表3**  基础算术接口MergeMode支持情况

| 接口      | 硬件支持模式           | 软件支持模式           |
| :-------- | :--------------------- | :--------------------- |
| Add       | MaskMergeMode::ZEROING | -                      |
| AddC      | -                      | -                      |
| Sub       | MaskMergeMode::ZEROING | -                      |
| SubC      | -                      | -                      |
| Exp       | MaskMergeMode::ZEROING | -                      |
| Sqrt      | MaskMergeMode::ZEROING | -                      |
| Max       | MaskMergeMode::ZEROING | MaskMergeMode::MERGING |
| Min       | MaskMergeMode::ZEROING | MaskMergeMode::MERGING |
| Mul       | MaskMergeMode::ZEROING | -                      |
| Mull      | -                      | -                      |
| Div       | MaskMergeMode::ZEROING | -                      |
| Adds      | MaskMergeMode::ZEROING | -                      |
| Maxs      | MaskMergeMode::ZEROING | -                      |
| Mins      | MaskMergeMode::ZEROING | -                      |
| Muls      | MaskMergeMode::ZEROING | -                      |
| Abs       | MaskMergeMode::ZEROING | -                      |
| Ln        | MaskMergeMode::ZEROING | -                      |
| Log       | MaskMergeMode::ZEROING | -                      |
| Log2      | -                      | MaskMergeMode::ZEROING |
| Log10     | -                      | MaskMergeMode::ZEROING |
| Neg       | MaskMergeMode::ZEROING | -                      |
| Relu      | MaskMergeMode::ZEROING | -                      |
| Prelu     | MaskMergeMode::ZEROING | -                      |
| LeakyRelu | MaskMergeMode::ZEROING | -                      |

**表4**  逻辑计算接口MergeMode支持情况

| 接口                   | 硬件支持模式           | 软件支持模式 |
| :--------------------- | :--------------------- | :----------- |
| Not（对RegTensor操作） | MaskMergeMode::ZEROING | -            |
| And（对RegTensor操作） | MaskMergeMode::ZEROING | -            |
| Or（对RegTensor操作）  | MaskMergeMode::ZEROING | -            |
| Xor（对RegTensor操作） | MaskMergeMode::ZEROING | -            |
| ShiftRight             | MaskMergeMode::ZEROING | -            |
| ShiftLefts             | MaskMergeMode::ZEROING | -            |
| ShiftLeft              | MaskMergeMode::ZEROING | -            |
| ShiftRights            | MaskMergeMode::ZEROING | -            |

**表5**  复合计算接口MergeMode支持情况

| 接口      | 硬件支持模式           | 软件支持模式 |
| :-------- | :--------------------- | :----------- |
| AbsSub    | MaskMergeMode::ZEROING | -            |
| ExpSub    | MaskMergeMode::ZEROING | -            |
| Axpy      | MaskMergeMode::ZEROING | -            |
| MulDstAdd | MaskMergeMode::ZEROING | -            |
| MulAddDst | MaskMergeMode::ZEROING | -            |
| MulsCast  | MaskMergeMode::ZEROING | -            |

**表6**  比较与选择接口MergeMode支持情况

| 接口     | 硬件支持模式 | 软件支持模式 |
| :------- | :----------- | :----------- |
| Compare  | -            | -            |
| Compares | -            | -            |
| Select   | -            | -            |
| Squeeze  | -            | -            |

**表7**  类型转换接口MergeMode支持情况

| 接口     | 硬件支持模式           | 软件支持模式 |
| :------- | :--------------------- | :----------- |
| Cast     | MaskMergeMode::ZEROING | -            |
| Truncate | MaskMergeMode::ZEROING | -            |

**表8**  归约计算接口MergeMode支持情况

| 接口            | 硬件支持模式           | 软件支持模式 |
| :-------------- | :--------------------- | :----------- |
| Reduce          | MaskMergeMode::ZEROING | -            |
| ReduceDataBlock | MaskMergeMode::ZEROING | -            |
| PairReduceElem  | MaskMergeMode::ZEROING | -            |

**表9**  数据填充接口MergeMode支持情况

| 接口      | 硬件支持模式                                   | 软件支持模式 |
| :-------- | :--------------------------------------------- | :----------- |
| Duplicate | MaskMergeMode::ZEROING、MaskMergeMode::MERGING | -            |

**表10**  离散操作接口MergeMode支持情况

| 接口   | 硬件支持模式 | 软件支持模式 |
| :----- | :----------- | :----------- |
| Gather | -            | -            |

**表11**  数据重排接口MergeMode支持情况

| 接口         | 硬件支持模式 | 软件支持模式 |
| :----------- | :----------- | :----------- |
| Interleave   | -            | -            |
| DeInterleave | -            | -            |

**表12**  数据压缩接口MergeMode支持情况

| 接口      | 硬件支持模式 | 软件支持模式 |
| :-------- | :----------- | :----------- |
| Unsqueeze | -            | -            |
| Pack      | -            | -            |
| UnPack    | -            | -            |

**表13**  直方图计算接口MergeMode支持情况

| 接口       | 硬件支持模式 | 软件支持模式 |
| :--------- | :----------- | :----------- |
| Histograms | -            | -            |

**表14**  索引操作接口MergeMode支持情况

| 接口   | 硬件支持模式 | 软件支持模式 |
| :----- | :----------- | :----------- |
| Arange | -            | -            |

**表15**  同步控制接口MergeMode支持情况

| 接口        | 硬件支持模式 | 软件支持模式 |
| :---------- | :----------- | :----------- |
| LocalMemBar | -            | -            |

**表16**  系统变量访问接口MergeMode支持情况

| 接口     | 硬件支持模式 | 软件支持模式 |
| :------- | :----------- | :----------- |
| ClearSpr | -            | -            |
