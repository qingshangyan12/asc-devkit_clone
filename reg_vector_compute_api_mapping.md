# Basic API 与 Tensor API 寄存器矢量计算接口对标表

## 说明

- 基线目录：`docs/zh/api/SIMD-API/basic_api/reg_vector_compute`；对标目录：`docs/zh/api/SIMD-API/tensor_api/reg_vector_compute`。
- 以 Basic API 的可调用接口文档为行基线；`overview.md`、各类别索引页、`key_features`、`register_data_types`、`aux_data_types`等说明性或类型定义文档不作为计算接口列入。
- “完整对标”表示主要运算语义和 Basic API 文档声明的数据类型均被覆盖；“部分对标”会注明缺失的类型、形式或行为；“无直接对标”表示 Tensor API 目录内没有等价的公开实现接口。
- `b8/b16/b32/b64`沿用 Basic API 文档的按位宽泛型记法。Tensor API 文档列出具体类型时，按其原文列举。类型很多的搬运/转换接口采用模式或类型转换对概括，完整组合仍以对应接口文档为准。
- 模板参数中的元素类型参数已体现在主表“支持的数据类型”列；由编译器从实参推导的寄存器类型参数（如`U/S/V`）不重复列出。其余会改变行为、布局、精度或搬运方式的模板参数及合法范围见“基础 API 额外模板参数”章节。

## 对标结果

| 基础 API 类别 | 基础 API 接口名称 | 基础 API 支持的数据类型 | Tensor API 接口名称 | Tensor API 支持的数据类型 | Tensor API 对标能力 |
| --- | --- | --- | --- | --- | --- |
| basic_arithmetic | `Abs` | `int8_t`、`int16_t`、`half`、`int32_t`、`float`、`int64_t`；`complex32→half`、`complex64→float` | `abs` | `int8_t`、`int16_t`、`half`、`int32_t`、`float` | 部分对标：逐元素绝对值；缺少`int64_t`和复数模值形式。 |
| basic_arithmetic | `Add` | 普通结果：`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`complex32`、`int64_t`、`uint64_t`、`complex64`；带进位：`int32_t`、`uint32_t` | `operator+`；`addc`（带进位） | `operator+`：`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`；`addc`：`int32_t`、`uint32_t` | 部分对标：普通加法和输出进位均有对应实现；普通加法缺少复数及64位整数。`addc`还支持输入进位。 |
| basic_arithmetic | `AddC` | `int32_t`、`uint32_t` | `addc` | `int32_t`、`uint32_t` | 完整对标：均支持输入进位并返回结果与输出进位。 |
| basic_arithmetic | `Adds` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`complex32`、`int64_t`、`uint64_t`、`complex64` | `operator+`（reg_tensor与标量） | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：覆盖矢量与标量加法，缺少复数及64位整数。 |
| basic_arithmetic | `Div` | 依算法模式：`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`、`complex32`、`int64_t`、`uint64_t`、`complex64`；高精度模式为`half/float/complex64`子集 | `operator/` | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` | 部分对标：逐元素除法；无Basic API算法模式，且缺少复数和64位整数。 |
| basic_arithmetic | `Exp` | `half`、`float` | `exp` | `half`、`float` | 部分对标：数学运算和类型对应，但Tensor API无算法精度及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `LeakyRelu` | `half`、`float` | `leaky_relu` | `half`、`float` | 完整对标。 |
| basic_arithmetic | `Ln` | `half`、`float` | `log` | `half`、`float` | 部分对标：均计算自然对数，但Tensor API无算法精度及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `Log` | `half`、`float` | `log` | `half`、`float` | 部分对标：Basic API的`Log`与`Ln`等价，但Tensor API无算法精度及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `Log10` | `half`、`float` | `log10` | `half`、`float` | 部分对标：数学运算和类型对应，但Tensor API无算法精度及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `Log2` | `half`、`float` | `log2` | `half`、`float` | 部分对标：数学运算和类型对应，但Tensor API无算法精度及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `Max` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t` | `max`（reg_tensor与reg_tensor） | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：缺少64位整数。 |
| basic_arithmetic | `Maxs` | 同`Max` | `max`（reg_tensor与标量） | 同Tensor API `max` | 部分对标：覆盖矢量与标量求最大值，缺少64位整数。 |
| basic_arithmetic | `Min` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t` | `min`（reg_tensor与reg_tensor） | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：缺少64位整数。 |
| basic_arithmetic | `Mins` | 同`Min` | `min`（reg_tensor与标量） | 同Tensor API `min` | 部分对标：覆盖矢量与标量求最小值，缺少64位整数。 |
| basic_arithmetic | `Mul` | `int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`complex32`、`int64_t`、`uint64_t`、`complex64` | `operator*`（reg_tensor与reg_tensor） | `int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：缺少复数及64位整数。 |
| basic_arithmetic | `Mull` | `int32_t`、`uint32_t` | `mull` | `int32_t`、`uint32_t` | 完整对标：返回乘积高、低32位寄存器。 |
| basic_arithmetic | `Muls` | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`、`complex32`、`int64_t`、`uint64_t`、`complex64` | `operator*`（reg_tensor与标量） | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` | 部分对标：缺少复数及64位整数。 |
| basic_arithmetic | `Neg` | `int8_t`、`int16_t`、`half`、`int32_t`、`float`、`int64_t` | 无 | — | 无直接对标：Tensor API仅文档化二元`operator-`，没有一元取负接口。 |
| basic_arithmetic | `Prelu` | `half`、`float` | `prelu` | `half`、`float` | 完整对标。 |
| basic_arithmetic | `Relu` | `half`、`int32_t`、`float`、`int64_t` | `relu` | `half`、`int32_t`、`float` | 部分对标：缺少`int64_t`。 |
| basic_arithmetic | `Sqrt` | `half`、`float` | `sqrt` | `half`、`float` | 部分对标：数学运算和类型对应，但Tensor API无0/1ulp算法、快速求逆及FTZ/Subnormal模板模式。 |
| basic_arithmetic | `Sub` | 普通结果：与`Add`普通结果相同；带借位：`int32_t`、`uint32_t` | `operator-`；`subc`（带借位） | `operator-`：`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`；`subc`：`int32_t`、`uint32_t` | 部分对标：普通减法和输出借位均有对应实现；普通减法缺少复数及64位整数。 |
| basic_arithmetic | `SubC` | `int32_t`、`uint32_t` | `subc` | `int32_t`、`uint32_t` | 完整对标：均支持输入借位并返回结果与借位。 |
| compare_and_select | `Compare` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`；输出Mask | `operator_compare`（`== != < <= > >=`） | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`；返回`reg_tensor<bool>` | 部分对标：覆盖寄存器间比较，缺少64位整数。 |
| compare_and_select | `Compares` | 同`Compare`，第二操作数为标量 | `operator_compare`（reg_tensor与标量） | 同Tensor API `operator_compare` | 部分对标：覆盖标量比较，缺少64位整数。 |
| compare_and_select | `Select` | `bool`、8/16/32/64位有符号与无符号整数、`half`、`bfloat16_t`、`float` | `select` | `bool`、`int8_t`、`uint8_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp8_e5m2_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：按mask选择语义一致；缺少64位整数，新增FP8/HIFLOAT8。 |
| compare_and_select | `Squeeze` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` | 无 | — | 无直接对标。 |
| composite_compute | `AbsSub` | `half`、`float`、`int64_t` | 无 | — | 无直接对标；可由减法与绝对值组合，但不等价于单接口。 |
| composite_compute | `Axpy` | `half`、`float`、`int64_t`、`uint64_t` | 无 | — | 无直接对标；可组合乘法和加法，非融合接口。 |
| composite_compute | `ExpSub` | `half→float`、`float→float` | 无 | — | 无直接对标；可组合减法和指数，非融合接口。 |
| composite_compute | `MulAddDst` | `int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t` | 无 | — | 无直接对标；Tensor API无对应融合累加接口。 |
| composite_compute | `MulDstAdd` | `half×half→float`、`bfloat16_t→bfloat16_t`、`float→float` | 无 | — | 无直接对标。 |
| composite_compute | `MulsCast` | `float×float→half` | 无 | — | 无直接对标。 |
| data_compression | `Pack` | `int16_t/uint16_t→uint8_t`、`int32_t/uint32_t→uint16_t`、`int64_t/uint64_t→uint32_t` | `store<store_sideband_mode::pack>` | `int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`half`、`bfloat16_t`、`float` | 部分对标：Tensor API在搬出时完成打包，调用形态和目的类型推导方式不同。 |
| data_compression | `UnPack` | `int8_t→int16_t`、`uint8_t→uint16_t`、`int16_t→int32_t`、`uint16_t→uint32_t`、`int32_t→int64_t`、`uint32_t→uint64_t` | `load<load_sideband_mode::unpack>` | 8/16/32bit元素类型（含整数、浮点和打包4bit类型） | 部分对标：Tensor API在搬入时展开并补零，不是独立寄存器转换接口。 |
| data_compression | `Unsqueeze` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t` | 无 | — | 无直接对标。`load`的upsample是搬入时重复元素，语义不等同。 |
| data_padding | `Duplicate` | `bool`、4/8位小数类型、8/16/32/64位整数、`half`、`bfloat16_t`、`float`、`complex32/64` | `fill` | `int8_t`、`uint8_t`、FP8类、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：均以标量填充寄存器；Tensor API缺少bool、4bit、64位和复数中的多种类型。 |
| data_reorder | `DeInterleave` | `bool`、8/16/32/64位整数、`half`、`bfloat16_t`、`float` | `deinterleave` | `int8_t`、`uint8_t`、FP8/HIFLOAT8、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` | 部分对标：数据解交织语义一致；缺少bool和64位整数，新增低精度浮点。 |
| data_reorder | `Interleave` | 同Basic API `DeInterleave` | `interleave` | 同Tensor API `deinterleave` | 部分对标：数据交织语义一致；缺少bool和64位整数，新增低精度浮点。 |
| histogram_compute | `Histograms` | `uint8_t→uint16_t` | 无 | — | 无直接对标。 |
| index_operations | `Arange` | `int8_t`、`int16_t`、`int32_t`、`half`、`float`、`int64_t` | 无 | — | 无直接对标。 |
| logical_compute | `And` | `bool`、8/16/32/64位有符号与无符号整数 | `operator&` | `bool`、8/16/32位有符号与无符号整数 | 部分对标：缺少64位整数。 |
| logical_compute | `Not` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t` | `operator!` | `bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` | 部分对标：均按二进制编码逐位取反；缺少64位整数，新增bool。 |
| logical_compute | `Or` | `bool`、8/16/32/64位有符号与无符号整数 | `operator or`（按位或） | `bool`、8/16/32位有符号与无符号整数 | 部分对标：缺少64位整数。 |
| logical_compute | `ShiftLeft` | 数据：8/16/32/64位有符号与无符号整数；寄存器移位量为匹配位宽的有符号整数 | `operator<<`（寄存器移位） | 数据：8/16/32位有符号与无符号整数；移位量：对应`reg_tensor<int8_t/int16_t/int32_t>` | 部分对标：缺少64位整数。 |
| logical_compute | `ShiftLefts` | 数据：8/16/32/64位有符号与无符号整数；标量移位量`int16_t` | `operator<<`（标量移位） | 数据：8/16/32位有符号与无符号整数；移位量`int16_t` | 部分对标：缺少64位整数。 |
| logical_compute | `ShiftRight` | 数据：8/16/32/64位有符号与无符号整数；寄存器移位量为匹配位宽的有符号整数 | `operator>>`（寄存器移位） | 数据：8/16/32位有符号与无符号整数；移位量：对应`reg_tensor<int8_t/int16_t/int32_t>` | 部分对标：缺少64位整数。 |
| logical_compute | `ShiftRights` | 数据：8/16/32/64位有符号与无符号整数；标量移位量`int16_t` | `operator>>`（标量移位） | 数据：8/16/32位有符号与无符号整数；移位量`int16_t` | 部分对标：缺少64位整数。 |
| logical_compute | `Xor` | `bool`、8/16/32/64位有符号与无符号整数 | `operator^` | `bool`、8/16/32位有符号与无符号整数 | 部分对标：缺少64位整数。 |
| MaskReg_compute | `DeInterleave` | `b8`、`b16`、`b32` | `deinterleave`（掩码） | `DataType`分组支持`b8`、`b16`、`b32`；数据为`reg_tensor<bool>` | 完整对标。 |
| MaskReg_compute | `Interleave` | `b8`、`b16`、`b32` | `interleave`（掩码） | `DataType`分组支持`b8`、`b16`、`b32`；数据为`reg_tensor<bool>` | 完整对标。 |
| MaskReg_compute | `Move` | Mask寄存器（文档未按元素类型区分） | `reg_tensor<bool>`赋值/传递，无独立接口页 | `bool` | 部分对标：可通过对象赋值表达，但Tensor API无同名公开计算接口。 |
| MaskReg_compute | `MoveMask` | `b16`、`b32` | 无 | — | 无直接对标。 |
| MaskReg_compute | `Pack` | Mask寄存器（文档未按元素类型区分） | 无 | — | 无直接对标。 |
| MaskReg_compute | `Select` | Mask寄存器（文档未按元素类型区分） | `select` | 条件为`reg_tensor<bool>`；数据类型见compare_and_select的`select` | 部分对标：可按布尔寄存器选择数据，但接口操作对象和Basic MaskReg版不同。 |
| MaskReg_compute | `UnPack` | Mask寄存器（文档未按元素类型区分） | 无 | — | 无直接对标。 |
| reduction_compute | `PairReduceElem` | `half`、`float` | 无 | — | 无直接对标。 |
| reduction_compute | `Reduce` | 求和：`half→half`、`int16_t→int32_t`、`uint16_t→uint32_t`、32/64位同型、`float`；最大/最小：16/32/64位整数、`half`、`float`同型 | 无 | — | 无直接对标。 |
| reduction_compute | `ReduceDataBlock` | 求和：`half→half`、`int16_t→int32_t`、`uint16_t→uint32_t`、32位同型、`float`；最大/最小：16/32位整数、`half`、`float`同型 | 无 | — | 无直接对标。 |
| reg_data_load | `GatherB`（离散搬入） | 数据`b8/b16/b32/b64`；索引`uint32_t` | 无 | — | 无直接对标。 |
| reg_data_load | `Gather`（离散搬入） | 数据为8/16/32/64位整数、`half`、`bfloat16_t`、`float`（部分8位源扩展到16位）；索引`uint16_t/uint32_t/uint64_t` | 无 | — | 无直接对标。 |
| reg_data_load | `Load` | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | `load<direct>` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`、`half`、`bfloat16_t`、`float`、HIFLOAT8/FP8/4bit打包类型 | 部分对标：均从UB连续搬入；Basic `Load`允许非32字节对齐，Tensor `load<direct>`要求32字节对齐，且不支持双寄存器复数形式。 |
| reg_data_load | `LoadAlign`（连续对齐搬入） | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | `load<direct>` | 同上 | 部分对标：连续对齐搬入能力对应；Tensor API不支持Basic的双寄存器复数形式。 |
| reg_data_load | `LoadAlign`（MaskReg搬入） | 源`b8/b16/b32/b64` | 无 | — | 无直接对标。 |
| reg_data_load | `LoadAlign`（非连续对齐搬入） | `b8/b16/b32` | `load`的`downsample/unpack/unpack4/upsample`模式（视步长语义） | 按模式支持8/16/32bit元素及部分低精度类型 | 部分对标：仅覆盖文档化的固定采样/展开排列，不能等价覆盖Basic通用非连续步长。 |
| reg_data_load | `LoadUnAlign`（连续非对齐搬入） | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | 无 | — | 无直接对标：Tensor API `load`要求其模式规定的地址对齐。 |
| reg_data_load | `MaskGenWithRegTensor`（MaskReg搬入） | 源`b16/b32` | 无 | — | 无直接对标。 |
| reg_data_load | `Move` | `bool`、8/16/32/64位整数、`half`、`bfloat16_t`、`float` | `reg_tensor`赋值/传递，无独立接口页 | 取决于`reg_tensor<T>` | 部分对标：可由Tensor对象赋值表达，但无同名公开搬运接口。 |
| reg_data_store | `Scatter`（离散搬出） | 数据为8/16/32/64位整数、`half`、`bfloat16_t`、`float`；索引`uint16_t/uint32_t/uint64_t` | 无 | — | 无直接对标。 |
| reg_data_store | `Store` | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | `store<direct>` | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`bfloat16_t`、`float`、HIFLOAT8/FP8/4bit打包类型 | 部分对标：均连续搬出到UB Tensor；Tensor API不含64位和双寄存器复数，且遵循Tensor布局/对齐约束。 |
| reg_data_store | `StoreAlign`（连续对齐搬出） | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | `store<direct>` | 同上 | 部分对标：连续对齐搬出对应；Tensor API缺少64位和双寄存器复数形式。 |
| reg_data_store | `StoreAlign`（MaskReg搬出） | 目的`b8/b16/b32/b64` | 无 | — | 无直接对标。 |
| reg_data_store | `StoreAlign`（非连续对齐搬出） | `b8/b16/b32` | `store`的`store_1st/intlv/pack/pack_quarter`模式（视布局语义） | 按模式支持8/16/32/64bit中的子集及部分浮点/低精度类型 | 部分对标：仅覆盖文档化的固定交织/打包模式，不能等价覆盖Basic通用非连续步长。 |
| reg_data_store | `StoreUnAlign`（连续非对齐搬出） | 单寄存器`b8/b16/b32/b64`；双寄存器`complex32/b64` | 无 | — | 无直接对标。 |
| reg_data_store | `StoreUnAlign`（MaskReg非对齐搬出） | 目的`b16/b32` | 无 | — | 无直接对标。 |
| scatter_operations | `Gather` | 数据`b8/b16/b32`；索引`uint8_t/uint16_t/uint32_t` | 无 | — | 无直接对标。 |
| sync_control | `LocalMemBar` | 无数据类型 | 无 | — | 无直接对标。 |
| system_variable_access | `ClearSpr` | 无数据类型 | 无 | — | 无直接对标。 |
| type_conversion | `Cast` | 文档列出的完整源→目的矩阵，覆盖4/8位低精度、8/16/32/64位整数、`half`、`bfloat16_t`、`float` | `cast` | 完整配置表覆盖上述大部分类型，并按`layout_mode`、`round_mode`、`saturation_mode`约束组合 | 部分对标：核心类型转换、舍入、饱和及布局能力对应；两侧合法类型对和模式组合并非完全相同，应按Tensor `cast`完整配置表迁移。 |
| type_conversion | `Truncate` | `half`、`bfloat16_t`、`float` | `trunc` | `half`、`bfloat16_t`、`float` | 部分对标：默认向零截断及类型对应，但Tensor API没有Basic API的可选舍入模板模式。 |
| vf_call | `asc_vf_call` | 无固定数据类型（由被调用VF签名决定） | 无 | — | 无直接对标。 |

## 基础 API 额外模板参数

### 通用Mask合并模式

`MaskMergeMode`不是所有接口都真正支持两个枚举值。下表按各接口文档声明的有效范围整理；默认值均为`ZEROING`，但`Move`例外，默认且仅支持`MERGING`。

| 参数范围 | 适用基础 API | 参数行为与Tensor API差异 |
| --- | --- | --- |
| `ZEROING`、`MERGING`均支持 | `Add`（不保留进位重载）、`Min`、`Max`、`Duplicate` | `ZEROING`把mask无效位置清零，`MERGING`保留原`dstReg`。Tensor API返回新`reg_tensor`，其无效位置置零，不提供保留旧目的值的`MERGING`模板模式。 |
| 仅`ZEROING`有效 | `Abs`、`Adds`、`Div`、`Exp`、`LeakyRelu`、`Ln`、`Log`、`Log10`、`Log2`、`Maxs`、`Mins`、`Mul`、`Muls`、`Neg`、`Prelu`、`Relu`、`Sqrt`、`Sub`（不保留借位重载）、`And/Or/Not/Xor`（RegTensor重载）、`ShiftLeft/ShiftLefts/ShiftRight/ShiftRights`、`Truncate`、`AbsSub`、`Axpy`、`ExpSub`、`MulAddDst`、`MulDstAdd`、`PairReduceElem`、`Reduce`、`ReduceDataBlock` | 文档虽把参数类型写为`MaskMergeMode`，但明确说明`MERGING`当前不支持。Tensor API相应计算接口同样对mask无效位置置零。 |
| 仅`MERGING`有效，默认`MERGING` | `Move`（带mask重载） | `ZEROING`当前不支持；Tensor API没有同名接口，也没有通过模板参数表达的等价模式。 |
| 不带该参数 | `Add/Sub`的进位/借位重载、`AddC/SubC`、`Mull`以及其他未在上面列出的接口 | 不适用。 |

> 注：`Add`的带进位重载与`Sub`的带借位重载没有`mode`模板参数。`And/Or/Not/Xor`的MaskReg重载也没有该参数。

### 算术算法、精度与Subnormal模式

| 基础 API | 额外模板参数及默认值 | 文档支持范围 | Tensor API对标影响 |
| --- | --- | --- | --- |
| `Div` | `auto mode = MaskMergeMode::ZEROING`；也可传指向`DivSpecificMode`常量对象的指针 | `mrgMode`仅有效支持`ZEROING`；`precisionMode=false/true`，`true`时启用差值补偿，仅支持`float/complex64`；`algo`支持`INTRINSIC`、`DIFF_COMPENSATION`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_0ULP_FTZ_TRUE`、`PRECISION_0ULP_FTZ_FALSE`、`PRECISION_1ULP_FTZ_FALSE`。不同算法的数据类型范围见主表，FTZ_FALSE模式支持Subnormal。 | Tensor `operator/`没有对应的算法、精度、FTZ模板参数，因此即使重合数据类型也只属于部分对标。 |
| `Exp` | `auto mode`；`ExpSpecificMode{mrgMode, algo}`，默认`ZEROING/INTRINSIC` | `algo`：`INTRINSIC`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_FALSE`；三者最大误差均1ulp，FTZ_FALSE保留Subnormal。 | Tensor `exp`无算法/FTZ选择；基本运算语义对标，但无法显式复刻这些模式。 |
| `Ln` | `auto mode`；`LnSpecificMode{mrgMode, algo}`，默认`ZEROING/INTRINSIC` | `algo`：`INTRINSIC`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_FALSE`；三者最大误差均1ulp。 | Tensor `log`无算法/FTZ选择。 |
| `Log` | `auto mode`；`LogSpecificMode{mrgMode, algo}`，默认`ZEROING/INTRINSIC` | `algo`：`INTRINSIC`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_FALSE`；三者最大误差均1ulp。 | Tensor `log`无算法/FTZ选择。 |
| `Log2` | `auto mode`；`Log2SpecificMode{mrgMode, algo}`，默认`ZEROING/INTRINSIC` | `algo`：`INTRINSIC`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_FALSE`；三者最大误差均1ulp。 | Tensor `log2`无算法/FTZ选择。 |
| `Log10` | `auto mode`；`Log10SpecificMode{mrgMode, algo}`，默认`ZEROING/INTRINSIC` | `algo`：`INTRINSIC`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_1ULP_FTZ_FALSE`；三者最大误差均1ulp。 | Tensor `log10`无算法/FTZ选择。 |
| `Sqrt` | `auto mode`；`SqrtSpecificMode{mrgMode, precisionMode, algo}`，默认`ZEROING/false/INTRINSIC` | `precisionMode`：`false/true`，`true`仅对`float`生效；`algo`：`INTRINSIC`、`FAST_INVERSE`、`PRECISION_1ULP_FTZ_TRUE`、`PRECISION_0ULP_FTZ_FALSE`、`PRECISION_1ULP_FTZ_FALSE`。`FAST_INVERSE`和`PRECISION_0ULP_FTZ_FALSE`仅支持`float`且误差0ulp；`PRECISION_1ULP_FTZ_FALSE`用于`half`并支持Subnormal。 | Tensor `sqrt`无精度算法/FTZ选择。 |

`INTRINSIC`模式下Subnormal是否保留受编译选项`--cce-ftz`影响（文档默认`true`）；显式`FTZ_TRUE`采用FTZ，`FTZ_FALSE`用于保留Subnormal。由此，主表中上述接口的“完整对标”仅指运算和数据类型，不代表算法配置能力完全一致。

### 比较、重排、归约和其他计算模式

| 基础 API | 额外模板参数 | 支持范围与约束 | Tensor API对标影响 |
| --- | --- | --- | --- |
| `Compare`、`Compares` | `CMPMODE mode = CMPMODE::EQ` | `LT`、`GT`、`GE`、`EQ`、`NE`、`LE` | Tensor以六个比较运算符表达相同范围，能力可对应。 |
| `Squeeze` | `GatherMaskMode store = NO_STORE_REG` | `NO_STORE_REG`、`STORE_REG`；后者把有效元素总字节数写入AR寄存器 | Tensor API无直接对标。 |
| `Arange` | `IndexOrder order = INCREASE_ORDER` | `INCREASE_ORDER`、`DECREASE_ORDER` | Tensor API无直接对标。 |
| `Histograms` | `HistogramsBinType mode`；`HistogramsType type`，均无默认值 | `mode`：`BIN0`统计`[0,127]`、`BIN1`统计`[128,255]`；`type`：`FREQUENCY`、`ACCUMULATE` | Tensor API无直接对标。 |
| `Pack`、`UnPack`（数据） | `HighLowPart part = LOWEST` | `LOWEST`、`HIGHEST`；当寄存器为`RegTraitNumTwo`时仅支持`LOWEST` | Tensor `load/store`侧带模式不能直接选择同样的寄存器半部。 |
| `Pack`、`UnPack`（MaskReg） | `HighLowPart part = LOWEST` | `LOWEST`、`HIGHEST` | Tensor API无直接Mask打包/解包接口。 |
| `Duplicate`的RegTensor源重载 | `HighLowPart pos = LOWEST` | 当前只支持`LOWEST`，即广播源寄存器最低位 | Tensor `fill`接收标量，不对应此重载。 |
| `ExpSub` | `RegLayout layout = ZERO` | 源为`half`时支持`ZERO`、`ONE`；源为`float`时参数不生效 | Tensor API无直接对标。 |
| `MulsCast` | `RegLayout layout = ZERO` | `ZERO`、`ONE` | Tensor API无直接对标。 |
| `PairReduceElem` | `PairReduce type = SUM` | 仅`SUM` | Tensor API无直接对标。 |
| `Reduce`、`ReduceDataBlock` | `ReduceType type = SUM` | `SUM`、`MAX`、`MIN`；数据类型组合随`type`变化，见主表 | Tensor API无直接对标。 |
| `MaskGenWithRegTensor` | `int16_t offset`，无默认值 | `b16`时`[0,15]`；`b32`时`[0,31]` | Tensor API无直接对标。 |
| `ClearSpr` | `SpecialPurposeReg spr`，无默认值 | 当前文档仅列`AR` | Tensor API无直接对标。 |
| `asc_vf_call` | `auto funcPtr`；`typename... Args` | `funcPtr`必须指向SIMD入口核函数；`Args`由调用参数包决定 | Tensor API无直接对标。 |

### 类型转换配置

| 基础 API | 额外模板参数 | 支持范围与关键约束 | Tensor API对标影响 |
| --- | --- | --- | --- |
| `Truncate` | `RoundMode roundMode = CAST_NONE`；`MaskMergeMode mode = ZEROING` | 文档列出的舍入取值为`CAST_RINT`、`CAST_ROUND`、`CAST_FLOOR`、`CAST_CEIL`、`CAST_TRUNC`；`mode`仅支持`ZEROING`。注意原型默认值`CAST_NONE`未出现在该接口的有效取值列表中。 | Tensor `trunc`固定执行向零截断，没有可选舍入模式；因此数据类型和默认截断语义可对标，模板配置能力不完整。 |
| `Cast` | `const CastTrait& trait = castTrait` | `CastTrait`含`layoutMode`、`satMode`、`mrgMode`、`roundMode`。整体枚举候选包括：布局`UNKNOWN/ZERO/ONE/TWO/THREE`；饱和`UNKNOWN/NO_SAT/SAT`；mask合并在当前合法转换表中为`ZEROING`；舍入按类型对支持`UNKNOWN/CAST_RINT/CAST_ROUND/CAST_FLOOR/CAST_CEIL/CAST_TRUNC/HYBRID`的子集。四个字段不能任意组合，必须使用Basic `Cast.md`数据类型配置表列出的组合；`SAT`是否生效还受CTRL[60]和`SetCtrlSpr`配置影响。 | Tensor `cast`也有layout、round、sat配置，但命名和合法组合表不同；迁移时必须按“源→目的类型+全部模式字段”逐行核对，不能仅按类型并集判断。 |

### 搬入搬出模板模式

| 基础 API | 额外模板参数 | 支持范围与参数约束 | Tensor API对标影响 |
| --- | --- | --- | --- |
| 连续`LoadAlign`（单搬入） | `LoadDist dist = DIST_NORM`；可选`PostLiteral postMode` | `DIST_NORM`、`DIST_BRC_B8/B16/B32`、`DIST_US_B8/B16`、`DIST_DS_B8/B16`、`DIST_UNPACK_B8/B16/B32`、`DIST_UNPACK4_B8`、`DIST_BLK`、`DIST_E2B_B16/B32`。不同模式起始地址对齐为1/2/4/32、`min(32,VL/2)`、`min(32,VL/4)`或`VL/16`、`VL/8`，以文档模式表为准。 | Tensor `load`仅以`direct/downsample/unpack/unpack4/upsample`覆盖其中一部分；广播另由`load_broadcast`覆盖，且模式集合并非一一对应。 |
| 连续`LoadAlign`（双搬入） | `LoadDist dist`，无默认值；可选`postMode` | `DIST_DINTLV_B8/B16/B32`，均要求32B对齐；一次读`2*VL`并按奇偶元素拆到两个寄存器 | Tensor `load<deintlv>`能力对应，但支持类型集合不同。 |
| 连续`StoreAlign`（单搬出） | `StoreDist dist = DIST_NORM`；可选`postMode` | `DIST_NORM`或`DIST_NORM_B8/B16/B32`、`DIST_FIRST_ELEMENT_B8/B16/B32`、`DIST_PACK_B16/B32/B64`、`DIST_PACK4_B32`；对齐随模式为1/2/4/32、`min(32,VL/2)`或`min(32,VL/4)`。 | Tensor `store`的`direct/store_1st/pack/pack_quarter`覆盖主要模式，但类型集合和接口约束不同。 |
| 连续`StoreAlign`（双搬出） | `StoreDist dist`，无默认值 | `DIST_INTLV_B8/B16/B32`，均要求32B对齐；忽略mask并交织两个寄存器至`2*VL`目的区域 | Tensor `store<intlv>`能力对应，但类型集合不同。 |
| MaskReg `LoadAlign/StoreAlign` | `MaskDist dist = DIST_NORM`；可选`postMode` | `DIST_NORM`、`DIST_US`、`DIST_DS`；对齐分别为`VL/8`、`VL/16`、`min(32,VL/4)` | Tensor API没有对应Mask内存搬运接口。 |
| 非连续`LoadAlign/StoreAlign` | `DataCopyMode dataMode`，无默认值；可选`postMode` | 当前仅`DATA_BLOCK_COPY`；`repeatStride`单位为32B DataBlock，且`repeatStride * sizeof(T)`需32B对齐 | Tensor API固定sideband模式不能覆盖通用repeat stride。 |
| `LoadUnAlign` | 部分重载带`PostLiteral postMode = POST_MODE_UPDATE` | `POST_MODE_NORMAL`不支持；使用post-update重载时仅`POST_MODE_UPDATE`有效 | Tensor `load`要求各模式规定的对齐，无直接对标。 |
| `StoreUnAlign`、MaskReg `StoreUnAlign` | `PostLiteral postMode = POST_MODE_UPDATE` | `POST_MODE_NORMAL`不支持；仅`POST_MODE_UPDATE`有效 | Tensor API无非对齐搬出对标。 |

`PostLiteral`的两个枚举候选是`POST_MODE_NORMAL`和`POST_MODE_UPDATE`，但上述带地址自动更新的重载均明确不支持`POST_MODE_NORMAL`。使用`POST_MODE_UPDATE`时，地址在搬运后按传入步长更新；使用`AddrReg`偏移的重载则不会自动更新基地址。

### 同步控制参数

`LocalMemBar<MemType src, MemType dst>`不是任意两种流水线的笛卡尔积。文档允许的组合仅为：

| `src` | 允许的`dst` |
| --- | --- |
| `VEC_STORE` | `VEC_LOAD`、`VEC_STORE`、`SCALAR_LOAD`、`SCALAR_STORE` |
| `VEC_LOAD` | `VEC_STORE`、`SCALAR_STORE` |
| `SCALAR_STORE` | `VEC_LOAD`、`VEC_STORE` |
| `SCALAR_LOAD` | `VEC_STORE` |
| `VEC_ALL` | `VEC_ALL`、`SCALAR_ALL` |
| `SCALAR_ALL` | `VEC_ALL` |

Tensor API目录没有`LocalMemBar`对标接口，因此这些同步范围不能由Tensor接口直接表达。

## 汇总

| 对标结论 | 数量 |
| --- | ---: |
| 完整对标 | 7 |
| 部分对标 | 45 |
| 无直接对标 | 30 |
| 合计 | 82 |

总体上，Tensor API已覆盖常用基础算术、比较选择、逻辑运算、类型转换以及连续对齐的寄存器搬入/搬出；主要缺口集中在64位与复数类型、归约/直方图/索引生成、离散及非对齐搬运、MaskReg专用操作，以及融合复合计算接口。
