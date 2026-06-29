# 接口MergeMode支持情况汇总

**表 1**  Reg矢量计算接口MergeMode支持情况

| 接口 | 硬件支持模式 | 软件支持模式 |
| :--- | :----------- | :----------- |
| **数据搬入** | | |
| LoadAlign | / | / |
| LoadUnAlign | / | / |
| MaskGenWithRegTensor | / | / |
| Gather | / | / |
| GatherB | / | / |
| Load | / | / |
| Move | MaskMergeMode::MERGING | / |
| **数据搬出** | | |
| StoreAlign | / | / |
| StoreUnAlign | / | / |
| Scatter | / | / |
| Store | / | / |
| **基础算术** | | |
| Add | MaskMergeMode::ZEROING | / |
| AddC | / | / |
| Sub | MaskMergeMode::ZEROING | / |
| SubC | / | / |
| Exp | MaskMergeMode::ZEROING | / |
| Sqrt | MaskMergeMode::ZEROING | / |
| Max | MaskMergeMode::ZEROING | MaskMergeMode::MERGING |
| Min | MaskMergeMode::ZEROING | MaskMergeMode::MERGING |
| Mul | MaskMergeMode::ZEROING | / |
| Mull | / | / |
| Div | MaskMergeMode::ZEROING | / |
| Adds | MaskMergeMode::ZEROING | / |
| Maxs | MaskMergeMode::ZEROING | / |
| Mins | MaskMergeMode::ZEROING | / |
| Muls | MaskMergeMode::ZEROING | / |
| Abs | MaskMergeMode::ZEROING | / |
| Ln | MaskMergeMode::ZEROING | / |
| Log | MaskMergeMode::ZEROING | / |
| Log2 | / | MaskMergeMode::ZEROING |
| Log10 | / | MaskMergeMode::ZEROING |
| Neg | MaskMergeMode::ZEROING | / |
| Relu | MaskMergeMode::ZEROING | / |
| Prelu | MaskMergeMode::ZEROING | / |
| LeakyRelu | MaskMergeMode::ZEROING | / |
| **逻辑计算** | | |
| Not（对RegTensor操作） | MaskMergeMode::ZEROING | / |
| And（对RegTensor操作） | MaskMergeMode::ZEROING | / |
| Or（对RegTensor操作） | MaskMergeMode::ZEROING | / |
| Xor（对RegTensor操作） | MaskMergeMode::ZEROING | / |
| ShiftRight | MaskMergeMode::ZEROING | / |
| ShiftLefts | MaskMergeMode::ZEROING | / |
| ShiftLeft | MaskMergeMode::ZEROING | / |
| ShiftRights | MaskMergeMode::ZEROING | / |
| **复合计算** | | |
| AbsSub | MaskMergeMode::ZEROING | / |
| ExpSub | MaskMergeMode::ZEROING | / |
| Axpy | MaskMergeMode::ZEROING | / |
| MulDstAdd | MaskMergeMode::ZEROING | / |
| MulAddDst | MaskMergeMode::ZEROING | / |
| MulsCast | MaskMergeMode::ZEROING | / |
| **比较与选择** | | |
| Compare | / | / |
| Compares | / | / |
| Select | / | / |
| Squeeze | / | / |
| **类型转换** | | |
| Cast | MaskMergeMode::ZEROING | 待补充 |
| Truncate | MaskMergeMode::ZEROING | / |
| **归约计算** | | |
| Reduce | MaskMergeMode::ZEROING | / |
| ReduceDataBlock | MaskMergeMode::ZEROING | / |
| PairReduceElem | MaskMergeMode::ZEROING | / |
| **数据填充** | | |
| Duplicate | MaskMergeMode::ZEROING、MaskMergeMode::MERGING | / |
| **离散操作** | | |
| Gather | / | / |
| **数据重排** | | |
| Interleave | / | / |
| DeInterleave | / | / |
| **数据压缩** | | |
| Unsqueeze | / | / |
| Pack | / | / |
| UnPack | / | / |
| **直方图计算** | | |
| Histograms | / | / |
| **索引操作** | | |
| Arange | / | / |
| **同步控制** | | |
| LocalMemBar | / | / |
| **系统变量访问** | | |
| ClearSpr | / | / |
