# Reg矢量计算接口MergeMode支持情况汇总

不支持MergeMode模板参数的接口，下文不罗列。

**表1**  数据搬入接口MergeMode支持情况

| 接口 | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :--- | :----------------------- | :----------------------- |
| Move | MaskMergeMode::MERGING   | 无                       |

**表2**  基础算术接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------- | :----------------------- | :----------------------- |
| Add       | <!-- npu="950" id1 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::ZEROING。<br><!-- end id1 --><!-- npu="960" id2 -->针对Ascend 960PR/Ascend 960DT：<br>MaskMergeMode::ZEROING、MaskMergeMode::MERGING。<br><!-- end id2 -->   | <!-- npu="950" id3 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::MERGING。<br><!-- end id3 --><!-- npu="960" id4 -->针对Ascend 960PR/Ascend 960DT：<br>无。<br><!-- end id4 -->   |
| Sub       | MaskMergeMode::ZEROING   | 无                       |
| Exp       | MaskMergeMode::ZEROING   | 无                       |
| Sqrt      | MaskMergeMode::ZEROING   | 无                       |
| Max       | <!-- npu="950" id5 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::ZEROING。<br><!-- end id5 --><!-- npu="960" id6 -->针对Ascend 960PR/Ascend 960DT：<br>MaskMergeMode::ZEROING、MaskMergeMode::MERGING。<br><!-- end id6 -->   | <!-- npu="950" id7 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::MERGING。<br><!-- end id7 --><!-- npu="960" id8 -->针对Ascend 960PR/Ascend 960DT：<br>无。<br><!-- end id8 -->   |
| Min       | <!-- npu="950" id9 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::ZEROING。<br><!-- end id9 --><!-- npu="960" id10 -->针对Ascend 960PR/Ascend 960DT：<br>MaskMergeMode::ZEROING、MaskMergeMode::MERGING。<br><!-- end id10 -->   | <!-- npu="950" id11 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::MERGING。<br><!-- end id11 --><!-- npu="960" id12 -->针对Ascend 960PR/Ascend 960DT：<br>无。<br><!-- end id12 -->   |
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

**表3**  逻辑计算接口MergeMode支持情况

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

**表4**  复合计算接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------- | :----------------------- | :----------------------- |
| AbsSub    | MaskMergeMode::ZEROING   | 无                       |
| ExpSub    | MaskMergeMode::ZEROING   | 无                       |
| Axpy      | MaskMergeMode::ZEROING   | 无                       |
| MulDstAdd | MaskMergeMode::ZEROING   | 无                       |
| MulAddDst | MaskMergeMode::ZEROING   | 无                       |

**表5**  类型转换接口MergeMode支持情况

| 接口     | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :------- | :----------------------- | :----------------------- |
| Cast     | <!-- npu="950" id13 -->针对Ascend 950PR/Ascend 950DT：<br>MaskMergeMode::ZEROING。<br><!-- end id13 --><!-- npu="960" id14 -->针对Ascend 960PR/Ascend 960DT：<br>仅f162s8、f162s4、f322f16、f322bf16、f322HiF8、f322e4m3、f322e5m2、f162e4m3、f162e5m2、bf162e4m3、bf162e5m2、bf162e2m1、bf162e1m2、s162s4支持MaskMergeMode::ZEROING。<br><!-- end id14 -->   | 无                       |
| Truncate | MaskMergeMode::ZEROING   | 无                       |

**表6**  归约计算接口MergeMode支持情况

| 接口            | 硬件支持的模板参数枚举值 | 软件支持的模板参数枚举值 |
| :-------------- | :----------------------- | :----------------------- |
| Reduce          | MaskMergeMode::ZEROING   | 无                       |
| ReduceDataBlock | MaskMergeMode::ZEROING   | 无                       |
| PairReduceElem  | MaskMergeMode::ZEROING   | 无                       |

**表7**  数据填充接口MergeMode支持情况

| 接口      | 硬件支持的模板参数枚举值                       | 软件支持的模板参数枚举值 |
| :-------- | :--------------------------------------------- | :----------------------- |
| Duplicate | MaskMergeMode::ZEROING、MaskMergeMode::MERGING | 无                       |
