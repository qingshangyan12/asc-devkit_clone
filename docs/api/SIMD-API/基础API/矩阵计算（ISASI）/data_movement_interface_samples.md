# 数据搬运接口样例核对与调用片段

本文按接口名称和功能描述核对 `examples` 目录中的现有样例。已经有样例的条目给出可跳转路径和关键代码片段；当前仓没有完整样例但 API 资料说明支持的条目，给出最小 C++ 调用片段和参数说明；资料没有说明支持指定通路的条目，明确标注为不支持。

> 说明：本文中的“L1”按基础 API 的逻辑位置理解，通常对应 `A1`、`B1`、`C1` 或 `TSCM`。具体产品支持范围以对应 API 文档的“支持的通路和数据类型”为准。

## 总览

| 条目 | 当前仓情况 | 处理 |
|---|---|---|
| GM->L1 连续数据搬运 `DataCopy` | 未找到该 `count` 重载的完整样例 | 补 `half`、`count = 512` 的 GM->A1 调用片段 |
| GM->L1 高维切分数据搬运 `DataCopy` | 未找到该非连续 `DataCopyParams` 场景的完整样例 | 补 `half`、`blockCount = 2` 的 GM->A1 非连续搬运片段 |
| GM->L1 DN2NZ `DataCopy` | 有相同原型调用样例 | 粘贴 `data_copy_gm2l1` A1 使用示例片段，并给出完整示例路径 |
| GM->L1 非对齐 `DataCopyPad` | 未找到 GM->A1/B1 同原型样例；资料仅说明 Ascend 950PR/Ascend 950DT 支持 | 按资料搬运模式示例补 Normal/Compact 两个 GM->A1 调用片段 |
| GM->L1 Load2D | 未找到 GM->A1/B1 同原型样例；当前仓只有 L1->L0 相关样例 | 补 `m = 32`、`k = 32` 对齐 shape 的 GM->A1 调用片段 |
| GM->L1 Load2DV2 | 未找到 GM->A1/B1 同原型样例；当前仓只有 L1->L0 相关样例 | 按资料图示参数场景补 GM->A1 调用片段 |
| UB->L1 高维切分 `DataCopy` | 未找到 UB->A1/B1 高维切分同原型样例；当前仓只有连续 UB->L1 相关样例 | 按资料图示参数补 `half`、`blockCount = 2` 的 UB->A1 调用片段 |
| UB->L1 ND2NZ `DataCopy` | 未找到 UB->TSCM 同原型完整样例；当前仓只有 GM->L1 ND2NZ 和 TSCM 连续搬运相关样例 | 按资料图示参数补 `half`、`ndNum = 2` 的 UB->TSCM 调用片段 |
| UB->L1 非对齐 `DataCopyPad` | 未找到 UB->TSCM 同原型完整样例；当前仓只有非 L1 的 DataCopyPad 和 TSCM 连续搬运相关样例 | 按资料 VECIN/VECOUT->TSCM 场景补 `half` 调用片段，并补 Normal/Compact 模式配置片段 |
| `Load2DBitMode` | 未找到 `Load2DBitModeParam` 同原型 examples 样例；资料仅说明 Ascend 950PR/Ascend 950DT 支持 A1->A2、B1->B2 | 补 `half`、A1->A2、32x32 调用片段 |
| L1->Bias Table Buffer `DataCopy` | 有 C1->C2 `DataCopyParams` 同原型样例 | 粘贴 `data_copy_gm2l1` 场景4关键代码 |
| L1->Fixpipe Buffer `DataCopy` | 有 C1->C2PIPE2GM `DataCopyParams` 同原型样例 | 粘贴 `data_copy_l0c2gm` Vector 量化关键代码 |
| `SetAippFunctions` | 未找到 examples 同原型样例；资料给出 Atlas 推理系列产品 AI Core 的 YUV420SP 场景 | 与 `LoadImageToLocal` 合并为 32x32 YUV420SP AIPP 配置 + GM->A1 搬运片段 |
| `LoadImageToLocal` | 未找到 examples 同原型样例；资料给出 Atlas 推理系列产品 AI Core 的 YUV420SP 场景 | 与 `SetAippFunctions` 合并为同一个调用片段 |
| L1->GM 连续数据搬运 `DataCopy` | 有 A2/A3 直接 `count` 重载同原型样例；950PR/950DT 资料不支持直接 L1->GM | 粘贴 `data_copy_l1togm` dav-2201 分支关键代码 |
| L1->GM 高维切分数据搬运 `DataCopy` | 未找到资料非连续场景完整样例；仓内只有 L1(C1)->GM 连续 `DataCopyParams` 相关片段 | 按资料补连续/非连续两个 L1->GM 调用片段 |
| L0C->UB 双目标模式 `Fixpipe` | 有基础 API 完整样例；另有 Tensor API 相关样例 | 粘贴 `fixpipe_l0c2ub` 双目标模式关键代码，并给出相关 Tensor API 样例路径 |
| L1->UB `DataCopyL1ToUB` | 未找到 examples 同原型样例；`data_copy_ub2l1` 为反向通路 | 按资料补 `count` 和 `DataCopyParams` 两个 L1->UB 调用片段 |

## 2.7.3.4.1 GM->L1 连续数据搬运 DataCopy

当前仓未找到该 `count` 重载的完整样例。下面按章节资料补充一个具体调用场景：从 GM 起始地址连续搬运 128 个 `half` 元素到 L1 的 `A1` Buffer。

章节对应的连续搬运原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const uint32_t count)
```

调用示例：

```cpp
constexpr uint32_t copyCount = 128;

// 源操作数：GM上连续存放的128个half。
AscendC::GlobalTensor<half> srcGm;
srcGm.SetGlobalBuffer((__gm__ half *)src, copyCount);

// 目的操作数：L1 Buffer的A1位置。
AscendC::LocalTensor<half> dstA1(AscendC::TPosition::A1, 0, copyCount);

// count = 128，表示搬运128个half元素，实际搬运字节数256B满足32B对齐约束。
AscendC::DataCopy(dstA1, srcGm, copyCount);
```

## 2.7.3.4.2 GM->L1 高维切分数据搬运 DataCopy

当前仓有 `DataCopyParams` 重载的 GM->L1 相关调用，但参数配置为连续搬运，未覆盖本章节资料中的非连续搬运场景。下面按资料章节分别补充连续搬运和非连续搬运两个调用示例。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                const DataCopyParams& repeatParams)
```

调用示例一：连续搬运

```cpp
constexpr uint32_t copyCount = 128;

// 源操作数：GM上连续存放128个half。
AscendC::GlobalTensor<half> srcGm;
srcGm.SetGlobalBuffer((__gm__ half *)src, copyCount);

// 目的操作数：L1 Buffer的A1位置。
AscendC::LocalTensor<half> dstA1(AscendC::TPosition::A1, 0, copyCount);

AscendC::DataCopyParams repeatParams;
// 搬运1个数据块，表示连续搬运。
repeatParams.blockCount = 1;
// 每个数据块长度为8个DataBlock，即256B，等于128个half。
repeatParams.blockLen = 8;
// blockCount = 1时，源端间隔无意义，设置为0。
repeatParams.srcGap = 0;
// blockCount = 1时，目的端间隔无意义，设置为0。
repeatParams.dstGap = 0;

AscendC::DataCopy(dstA1, srcGm, repeatParams);
```

调用示例二：非连续搬运

```cpp
constexpr uint32_t srcCount = 112;
constexpr uint32_t dstCount = 128;

// 源操作数：GM上有112个half，按“48个有效元素 + 16个跳过元素 + 48个有效元素”排布。
AscendC::GlobalTensor<half> srcGm;
srcGm.SetGlobalBuffer((__gm__ half *)src, srcCount);

// 目的操作数：L1 Buffer的A1位置，两个数据块之间预留32个half的间隔。
AscendC::LocalTensor<half> dstA1(AscendC::TPosition::A1, 0, dstCount);

AscendC::DataCopyParams repeatParams;
// 搬运2个数据块。
repeatParams.blockCount = 2;
// 每个数据块长度为3个DataBlock，即96B，等于48个half。
repeatParams.blockLen = 3;
// 源端两个数据块之间跳过1个DataBlock，即16个half。
repeatParams.srcGap = 1;
// 目的端两个数据块之间间隔2个DataBlock，即32个half。
repeatParams.dstGap = 2;

AscendC::DataCopy(dstA1, srcGm, repeatParams);
```

参数说明：

| 参数 | 示例取值 | 说明 |
|---|---|---|
| `T` | `half` | 源和目的 Tensor 的元素类型保持一致。 |
| `dst` | `dstA1` | 目的 `LocalTensor<half>`，逻辑位置为 L1 Buffer 的 `A1`，目的地址需要 32B 对齐。 |
| `src` | `srcGm` | 源 `GlobalTensor<half>`，指向 GM 上的数据，源地址需要 1B 对齐。 |
| `repeatParams.blockCount` | 连续：`1`；非连续：`2` | 搬运的数据块个数；为 1 时表示单块连续搬运。 |
| `repeatParams.blockLen` | 连续：`8`；非连续：`3` | 每个数据块长度，单位为 DataBlock；1 个 DataBlock 为 32B。 |
| `repeatParams.srcGap` | 连续：`0`；非连续：`1` | 源端相邻数据块间隔，单位为 DataBlock；非连续示例中跳过 16 个 `half` 元素。 |
| `repeatParams.dstGap` | 连续：`0`；非连续：`2` | 目的端相邻数据块间隔，单位为 DataBlock；非连续示例中预留 32 个 `half` 元素。 |

## 2.7.3.4.4 GM->L1 随路转换 DN2NZ 搬运 DataCopy

资料章节说明该接口仅支持 Ascend 950PR/Ascend 950DT 的 GM->A1/B1 通路；其他产品资料未说明支持。

章节对应的接口原型为：

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                const Dn2NzParams& intriParams)
```

当前仓已找到相同原型调用样例：

- [README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/README.md>)
- [代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/data_copy_gm2l1.asc>)

使用示例片段：

GM->L1，搬运 A 矩阵，DN2NZ
```cpp
// M = 40、K = 56、CUBE_BLOCK = 16，CeilAlign(40, 16) = 48。
AscendC::Dn2NzParams dn2nzA1Params;
// 传输1个DN矩阵。
dn2nzA1Params.dnNum = 1;
// DN矩阵的列数为40，对应M。
dn2nzA1Params.nValue = 40;
// DN矩阵的行数为56，对应K。
dn2nzA1Params.dValue = 56;
// 只搬1个DN矩阵，相邻DN矩阵stride不生效。
dn2nzA1Params.srcDnMatrixStride = 0;
// 源GM中同一DN矩阵相邻行起始地址间隔为40个half，表示行紧密排列。
dn2nzA1Params.srcDValue = 40;
// M方向按16元素对齐，CeilAlign(40, 16) = 48，单位为C0_SIZE（32B）。
dn2nzA1Params.dstNzC0Stride = 48;
// 目的NZ矩阵中Z型矩阵相邻行起始地址间隔为1个C0_SIZE。
dn2nzA1Params.dstNzNStride = 1;
// 只搬1个NZ矩阵，相邻NZ矩阵stride不生效。
dn2nzA1Params.dstNzMatrixStride = 0;

AscendC::DataCopy(a1Local, aGM, dn2nzA1Params);

```

完整示例请参考：[DataCopy_GM2L1样例](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/data_copy_gm2l1.asc>)

## 2.7.3.4.5 GM->L1 非对齐数据搬运 DataCopyPad

当前仓未找到 GM->A1/B1 `DataCopyPad` 同原型调用样例。已有 `DataCopyPad` 调用主要是 GM->VECIN/VECOUT 或 VECIN/VECOUT->GM，不是本章节的 GM->L1 通路；`data_copy_gm2l1` 样例中 Bias 场景使用 `ND2NZ` 替代搬运，也不是 `DataCopyPad` 调用。

资料章节说明：

- Ascend 950PR/Ascend 950DT：支持 GM->A1/B1。
- 其他产品：资料未说明支持 GM->A1/B1 `DataCopyPad`，按本章节不支持处理。

章节对应的接口原型为：

```cpp
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                   const DataCopyExtParams& dataCopyParams,
                                   const DataCopyPadExtParams<T>& padParams)
```

下面沿用资料“搬运模式的配置示例”中的参数场景：`blockCount = 3`、`blockLen = 48B`、`srcStride = 0`、`dstStride = 0`、右侧补 16B。本章节通路为 GM->A1，示例使用 `int8_t`，因此 `rightPadding = 16` 表示右侧补 16 个 `int8_t` 元素，即 16B。

调用示例一：Normal 模式

```cpp
constexpr uint32_t srcElemCount = 144;
constexpr uint32_t dstElemCount = 192;

// 源操作数：GM上连续存放3个数据块，每块48B，共144个int8_t。
AscendC::GlobalTensor<int8_t> srcGm;
srcGm.SetGlobalBuffer((__gm__ int8_t *)src, srcElemCount);

// 目的操作数：L1 Buffer的A1位置。Normal模式下每块48B有效数据右侧补16B，3块共192B。
AscendC::LocalTensor<int8_t> dstA1(AscendC::TPosition::A1, 0, dstElemCount);

AscendC::DataCopyExtParams copyParams;
// 资料场景搬运3个连续传输数据块。
copyParams.blockCount = 3;
// 每个连续传输数据块包含48B有效数据，故意保留非32B对齐长度。
copyParams.blockLen = 48;
// 源操作数在GM，单位为字节；相邻数据块紧密排列，间隔为0B。
copyParams.srcStride = 0;
// 目的端A1相邻数据块无额外间隔；Normal模式下每块数据包含右侧padding后顺序存放。
copyParams.dstStride = 0;
// 保留字段，固定填0。
copyParams.rsv = 0;

AscendC::DataCopyPadExtParams<int8_t> padParams;
// 资料场景设置为false，padding区域使用默认随机值，不使用paddingValue。
padParams.isPad = false;
// 左侧不填充，GM数据直接从每个目的数据块起始位置写入。
padParams.leftPadding = 0;
// 每块右侧填充16个int8_t，即16B；48B + 16B = 64B，满足32B对齐。
padParams.rightPadding = 16;
// isPad = false时该值不生效，填0保持字段完整。
padParams.paddingValue = static_cast<int8_t>(0);

AscendC::DataCopyPad<int8_t, AscendC::PaddingMode::Normal>(dstA1, srcGm, copyParams, padParams);
```

调用示例二：Compact 模式

```cpp
constexpr uint32_t srcElemCount = 144;
constexpr uint32_t dstElemCount = 160;

// 源操作数：GM上连续存放3个数据块，每块48B，共144个int8_t。
AscendC::GlobalTensor<int8_t> srcGm;
srcGm.SetGlobalBuffer((__gm__ int8_t *)src, srcElemCount);

// 目的操作数：L1 Buffer的A1位置。Compact模式下3块有效数据共144B，只在最后右侧补16B。
AscendC::LocalTensor<int8_t> dstA1(AscendC::TPosition::A1, 0, dstElemCount);

AscendC::DataCopyExtParams copyParams;
// 资料场景搬运3个连续传输数据块。
copyParams.blockCount = 3;
// 每个连续传输数据块包含48B有效数据。
copyParams.blockLen = 48;
// 源操作数在GM，单位为字节；相邻数据块紧密排列，间隔为0B。
copyParams.srcStride = 0;
// Compact模式下目的端有效数据紧密排列，不在每块之间插入padding间隔。
copyParams.dstStride = 0;
// 保留字段，固定填0。
copyParams.rsv = 0;

AscendC::DataCopyPadExtParams<int8_t> padParams;
// 资料场景设置为false，padding区域使用默认随机值，不使用paddingValue。
padParams.isPad = false;
// 左侧不填充，GM第一个元素直接搬到A1起始位置。
padParams.leftPadding = 0;
// 仅在最后一个数据块右侧填充16B，48B * 3 + 16B = 160B。
padParams.rightPadding = 16;
// isPad = false时该值不生效，填0保持字段完整。
padParams.paddingValue = static_cast<int8_t>(0);

AscendC::DataCopyPad<int8_t, AscendC::PaddingMode::Compact>(dstA1, srcGm, copyParams, padParams);
```

相关非 L1 样例：

- [DataCopyPad GM->UB->GM 样例](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm/README.md>)，但该样例不是 GM->L1。
- [data_copy_gm2l1 样例](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/data_copy_gm2l1.asc>) 的 Bias 场景使用 `ND2NZ` 替代搬运，不是本接口调用。

## 2.7.3.4.7 GM->L1 Load2DV2 指令搬运

当前仓未找到 `LoadData(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const LoadData2DParamsV2& loadDataParams)` 的 GM->A1/B1 同原型调用样例。`load_data_2dv2_l12l0` 是相关样例，但展示的是 L1->L0A/L0B，不是本章节的 GM->L1 通路。

资料章节说明 `Load2DV2` 支持 Ascend 950PR/Ascend 950DT 的 GM->A1/B1。本示例沿用资料中 `LoadData2DParamsV2` 结构体不启用转置时的图示参数场景，数据类型为 `half`，通路改为 GM->A1。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void LoadData(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                const LoadData2DParamsV2& loadDataParams)
```

调用示例：

```cpp
constexpr uint32_t fractalElemCount = 256;
constexpr uint32_t srcElemCount = 25 * fractalElemCount;
constexpr uint32_t dstElemCount = 9 * fractalElemCount;

// 源操作数：GM上按Nz分形排布存放half数据。一个512B分形包含256个half。
// 资料图示中的GM shape为80 * 80，M方向和K方向各有5个小分形，因此预留5 * 5 = 25个512B分形。
AscendC::GlobalTensor<half> srcGm;
srcGm.SetGlobalBuffer((__gm__ half *)src, srcElemCount);

// 目的操作数：L1 Buffer的A1位置。资料图示中的目的矩阵为3 * 3个小分形，因此预留9个512B分形。
AscendC::LocalTensor<half> dstA1(AscendC::TPosition::A1, 0, dstElemCount);

AscendC::LoadData2DParamsV2 loadDataParams;
// 源矩阵M轴起始位置为第32个元素，即2 * 16个元素。
loadDataParams.mStartPosition = 2;
// 源矩阵K轴起始位置为第32个half，即2 * 32B。
loadDataParams.kStartPosition = 2;
// M方向搬运32个元素，即2 * 16个元素。
loadDataParams.mStep = 2;
// K方向搬运48个half，即3 * 32B。
loadDataParams.kStep = 3;
// GM源矩阵M方向有5个小分形，K方向相邻源分形起始地址间隔为5 * 512B，等于1280个half。
loadDataParams.srcStride = 5;
// 目的A1矩阵M方向有3个小分形，K方向相邻目的分形起始地址间隔为3 * 512B，等于768个half。
loadDataParams.dstStride = 3;
// 资料场景不启用转置，GM->A1通路也不启用转置。
loadDataParams.ifTranspose = false;
// 预留参数，固定配置为0。
loadDataParams.sid = 0;

AscendC::LoadData(dstA1, srcGm, loadDataParams);
```

相关样例：

- [LoadData_2DV2_L12L0 README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0/README.md>)，但该样例不是 GM->L1。
- [LoadData_2DV2_L12L0 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0/load_data_2dv2_l12l0.asc>)，GM->L1 阶段使用 `DataCopy`，L1->L0 阶段使用 `Load2DV2`。

## 2.7.3.4.9 UB->L1 高维切分数据搬运 DataCopy

当前仓未找到 `DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)` 的 UB->A1/B1 高维切分同原型完整样例。`data_copy_ub2l1` 是相关样例，但使用的是 `count` 重载连续搬运，不是本章节的 `DataCopyParams` 重载。

资料章节说明该接口将 UB（`VECIN`/`VECCALC`/`VECOUT`）数据搬运到 L1 Buffer，支持连续和非连续搬运；支持 Ascend 950PR/Ascend 950DT、Atlas A3、Atlas A2、Atlas 推理系列产品 AI Core，其他列出的产品资料未说明支持。针对 Ascend 950PR/Ascend 950DT，资料说明 `ENABLE_CV_COMM_VIA_SSBUF=true` 时走 UB->L1 硬通道；未启用硬通道时会借助 Matmul 高阶 API 的 workspace GM 中转，需要先注册 Matmul 高阶 API。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                const DataCopyParams& repeatParams)
```

下面沿用资料图示中的参数场景：完成 2 个连续传输数据块的搬运，每个数据块包含 8 个 DataBlock，源操作数相邻数据块之间无间隔，目的操作数相邻数据块尾与头之间间隔 1 个 DataBlock。示例使用 `half`，因此 1 个 DataBlock 为 32B，即 16 个 `half`。

调用示例：

```cpp
constexpr uint32_t dataBlockElemCount = 16;
constexpr uint32_t srcElemCount = 2 * 8 * dataBlockElemCount;
constexpr uint32_t dstElemCount = (2 * 8 + 1) * dataBlockElemCount;

// 源操作数：UB的VECIN位置。2个数据块在源端紧密排列，每块8个DataBlock，共16个DataBlock，即256个half。
AscendC::LocalTensor<half> srcUb(AscendC::TPosition::VECIN, ubAddr, srcElemCount);

// 目的操作数：L1 Buffer的A1位置。两个目的数据块之间预留1个DataBlock间隔，总跨度为17个DataBlock，即272个half。
AscendC::LocalTensor<half> dstA1(AscendC::TPosition::A1, a1Addr, dstElemCount);

AscendC::DataCopyParams repeatParams;
// 资料场景搬运2个连续传输数据块。
repeatParams.blockCount = 2;
// 每个数据块长度为8个DataBlock，即256B，等于128个half。
repeatParams.blockLen = 8;
// 源UB相邻数据块尾与头之间无间隔，两个数据块紧密读取。
repeatParams.srcGap = 0;
// 目的A1相邻数据块尾与头之间间隔1个DataBlock，即32B，等于16个half。
repeatParams.dstGap = 1;

AscendC::DataCopy(dstA1, srcUb, repeatParams);
```

相关样例：

- [DataCopy_UB2L1 README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1/README.md>)，但该样例是连续 UB->L1 搬运。
- [DataCopy_UB2L1 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1/data_copy_ub2l1.asc>)，其中 UB->L1 阶段使用 `DataCopy(a1Local, ubA1Local, aSize)` 的 `count` 重载。

## 2.7.3.4.10 UB->L1 随路转换 ND2NZ 搬运 DataCopy

当前仓未找到 `DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Nd2NzParams& intriParams)` 的 UB->TSCM 同原型完整样例。已有 `Nd2NzParams` 样例主要是 GM->A1/B1；`matmul_tscm_src_vecout` 是相关 TSCM 样例，但使用的是 `count` 重载连续搬运，不是本章节的 ND2NZ 随路转换。

资料章节说明该接口将 UB（`VECIN`/`VECCALC`/`VECOUT`）数据搬运到 L1 Buffer 的 `TSCM`，搬运过程中完成 ND->NZ 格式转换；支持 Ascend 950PR/Ascend 950DT、Atlas A3、Atlas A2，Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品、Kirin X90、Kirin 9030 不支持。该接口为软件仿真实现，需要借助 Matmul 高阶 API 的 workspace GM 中转，使用前需要先注册 Matmul 高阶 API。若目标是普通 `A1`/`B1`，本章节资料未说明该 UB->L1 ND2NZ 通路支持。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                const Nd2NzParams& intriParams)
```

下面沿用资料图示中的 ND2NZ 转换参数场景：传输 2 个 ND 矩阵，每个 ND 矩阵高度为 2、宽度为 24 个 `half`；源端按 3 个 DataBlock 作为行跨度、9 个 DataBlock 作为矩阵跨度；目的端按 NZ 排布写入 TSCM。示例使用 `half`，因此 1 个 DataBlock 为 32B，即 16 个 `half`。

调用示例：

```cpp
AscendC::Nd2NzParams nd2nzParams;
// 传输2个ND矩阵，分别对应资料图示中的A/B矩阵和C/D矩阵。
nd2nzParams.ndNum = 2;
// 每个ND矩阵高度为2行。
nd2nzParams.nValue = 2;
// 每行宽度为24个half；不足32B对齐的部分在目的端补0。
nd2nzParams.dValue = 24;
// 源端相邻ND矩阵起始地址间隔为9个DataBlock，即9 * 16 = 144个half。
nd2nzParams.srcNdMatrixStride = 144;
// 源端同一ND矩阵相邻行起始地址间隔为3个DataBlock，即3 * 16 = 48个half。
nd2nzParams.srcDValue = 48;
// 目的NZ中同一源行拆出的相邻C0块间隔为11个DataBlock。
nd2nzParams.dstNzC0Stride = 11;
// 目的NZ中相邻源行转换后的起始地址间隔为2个DataBlock。
nd2nzParams.dstNzNStride = 2;
// 目的端相邻NZ矩阵起始地址间隔为6个DataBlock，即6 * 16 = 96个half。
nd2nzParams.dstNzMatrixStride = 96;

// 目的端最大访问跨度为 (6 + 2 + 11 + 1) 个DataBlock，即320个half，对应上面的dstElemCount。
AscendC::DataCopy(dstTscm, srcUb, nd2nzParams);
tscm.EnQue(dstTscm);
```

相关样例：

- [DataCopy_GM2L1 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/data_copy_gm2l1.asc>)，但该样例是 GM->A1/B1 的 ND2NZ。
- [matmul_tscm_src_vecout 代码](<../../examples/01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout/matmul_tscm_src_vecout.asc>)，但该样例是 VECOUT->TSCM 的连续搬运。

## 2.7.3.4.11 UB->L1 非对齐数据搬运 DataCopyPad

当前仓未找到 `DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams)` 的 UB->TSCM 同原型完整样例。已有 `DataCopyPad` 样例主要是 GM->UB 或 UB->GM；`matmul_tscm_src_vecout` 是相关 TSCM 样例，但使用的是 `count` 重载连续搬运，不是本章节的非对齐搬运。

资料章节说明该接口将 UB（`VECIN`/`VECOUT`）数据非对齐搬运到 L1 Buffer 的 `TSCM`，实际路径为 VECIN/VECOUT->GM->TSCM，并可在 GM->TSCM 阶段完成 ND->NZ 格式转换。该接口支持 Ascend 950PR/Ascend 950DT、Atlas A3、Atlas A2，Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品不支持。该接口为软件仿真实现，需要借助 Matmul 高阶 API 的 workspace GM 中转，使用前需要先注册 Matmul 高阶 API。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopyPad(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                   const DataCopyExtParams& dataCopyParams,
                                   const Nd2NzParams& nd2nzParams)
```

下面沿用资料图示中的 VECIN/VECOUT->TSCM 参数场景：源数据类型为 `half`，单个 DataBlock 为 32B，即 16 个 `half`；源 UB 中每行搬运 A/B/C 的 6 个 DataBlock，行间跳过 1 个 DataBlock；中转 GM 中相邻行之间保留 2 个空白 DataBlock；最终搬入 TSCM 时转换为 NZ 排布。

调用示例一：VECIN->TSCM 非对齐搬运并 ND->NZ

```cpp
constexpr uint32_t dataBlockElemCount = 16;
constexpr uint32_t srcElemCount = (3 * 6 + 2 * 1) * dataBlockElemCount;
constexpr uint32_t dstElemCount = (5 * 7 + 2 * 2 + 1) * dataBlockElemCount;
constexpr uint32_t tscmAddr = 0;

// 源操作数：UB的VECIN位置。3行有效数据各6个DataBlock，行间各跳过1个DataBlock，共20个DataBlock，即320个half。
AscendC::LocalTensor<half> srcUb(AscendC::TPosition::VECIN, ubAddr, srcElemCount);

// 目的操作数：L1 Buffer的TSCM位置。静态Tensor直接指定TPosition::TSCM，预留640个half。
AscendC::LocalTensor<half> dstTscm(AscendC::TPosition::TSCM, tscmAddr, dstElemCount);

AscendC::DataCopyExtParams copyParams;
// VECIN->GM阶段搬运3个连续传输数据块，对应资料图示中的A/B/C三行。
copyParams.blockCount = 3;
// 每个连续传输数据块包含6个DataBlock，即192B，等于96个half。
copyParams.blockLen = 6 * 32;
// 源操作数在UB，单位为DataBlock；相邻行之间跳过1个DataBlock。
copyParams.srcStride = 1;
// 中转目的端为GM，单位为字节；相邻行之间保留2个空白DataBlock，即64B。
copyParams.dstStride = 64;
// 保留字段，固定填0。
copyParams.rsv = 0;

AscendC::Nd2NzParams nd2nzParams;
// 本接口资料约束ndNum仅支持设置为1，即A/B/C三行视作一个ND矩阵。
nd2nzParams.ndNum = 1;
// ND矩阵高度为3行。
nd2nzParams.nValue = 3;
// 每行宽度为6个DataBlock，即96个half。
nd2nzParams.dValue = 6 * dataBlockElemCount;
// 仅1个ND矩阵，不涉及相邻ND矩阵间隔。
nd2nzParams.srcNdMatrixStride = 0;
// GM中相邻行起始地址间隔为8个DataBlock，即128个half。
nd2nzParams.srcDValue = 8 * dataBlockElemCount;
// 目的NZ中同一源行的相邻DataBlock间隔为7个DataBlock。
nd2nzParams.dstNzC0Stride = 7;
// 目的NZ中相邻源行转换后的起始地址间隔为2个DataBlock。
nd2nzParams.dstNzNStride = 2;
// 仅1个NZ矩阵，按资料场景配置为1。
nd2nzParams.dstNzMatrixStride = 1;

// 目的端最大访问跨度为5 * 7 + 2 * 2 + 1 = 40个DataBlock，即640个half，对应上面的dstElemCount。
AscendC::DataCopyPad(dstTscm, srcUb, copyParams, nd2nzParams);
```

资料同章还给出了 Normal/Compact 两种搬运模式配置。该模式配置对应带 `DataCopyPadExtParams` 的 `DataCopyPad` 原型，用于 GM->VECIN/VECOUT 这类带左右 padding 参数的搬运；不是上面 UB->TSCM 的 `DataCopyExtParams + Nd2NzParams` 原型。

调用示例二：Normal 模式

```cpp
constexpr uint32_t srcElemCount = 144;
constexpr uint32_t dstElemCount = 192;

// 源操作数：GM上连续存放3个数据块，每块48B，共144个int8_t。
AscendC::GlobalTensor<int8_t> srcGm;
srcGm.SetGlobalBuffer((__gm__ int8_t *)src, srcElemCount);

// 目的操作数：UB的VECIN位置。Normal模式下每块48B有效数据右侧补16B，3块共192B。
AscendC::LocalTensor<int8_t> dstUb(AscendC::TPosition::VECIN, ubAddr, dstElemCount);

AscendC::DataCopyExtParams copyParams;
// 资料场景搬运3个连续传输数据块。
copyParams.blockCount = 3;
// 每个连续传输数据块包含48B有效数据。
copyParams.blockLen = 48;
// 源操作数在GM，单位为字节；相邻数据块紧密排列，间隔为0B。
copyParams.srcStride = 0;
// 目的操作数在UB，单位为DataBlock；相邻数据块无额外间隔。
copyParams.dstStride = 0;
// 保留字段，固定填0。
copyParams.rsv = 0;

AscendC::DataCopyPadExtParams<int8_t> padParams;
// 资料场景设置为false，padding区域使用默认随机值。
padParams.isPad = false;
// 左侧不填充。
padParams.leftPadding = 0;
// 每块右侧填充16个int8_t，即16B；48B + 16B = 64B，满足32B对齐。
padParams.rightPadding = 16;
// isPad = false时该值不生效，填0保持字段完整。
padParams.paddingValue = static_cast<int8_t>(0);

AscendC::DataCopyPad<int8_t, AscendC::PaddingMode::Normal>(dstUb, srcGm, copyParams, padParams);
```

调用示例三：Compact 模式

```cpp
constexpr uint32_t srcElemCount = 144;
constexpr uint32_t dstElemCount = 160;

// 源操作数：GM上连续存放3个数据块，每块48B，共144个int8_t。
AscendC::GlobalTensor<int8_t> srcGm;
srcGm.SetGlobalBuffer((__gm__ int8_t *)src, srcElemCount);

// 目的操作数：UB的VECIN位置。Compact模式下3块有效数据共144B，只在最后右侧补16B。
AscendC::LocalTensor<int8_t> dstUb(AscendC::TPosition::VECIN, ubAddr, dstElemCount);

AscendC::DataCopyExtParams copyParams;
// 资料场景搬运3个连续传输数据块。
copyParams.blockCount = 3;
// 每个连续传输数据块包含48B有效数据。
copyParams.blockLen = 48;
// 源操作数在GM，单位为字节；相邻数据块紧密排列，间隔为0B。
copyParams.srcStride = 0;
// Compact模式下目的端有效数据紧密排列，不在每块之间插入padding间隔。
copyParams.dstStride = 0;
// 保留字段，固定填0。
copyParams.rsv = 0;

AscendC::DataCopyPadExtParams<int8_t> padParams;
// 资料场景设置为false，padding区域使用默认随机值。
padParams.isPad = false;
// 左侧不填充。
padParams.leftPadding = 0;
// 仅在最后一个数据块右侧填充16B，48B * 3 + 16B = 160B。
padParams.rightPadding = 16;
// isPad = false时该值不生效，填0保持字段完整。
padParams.paddingValue = static_cast<int8_t>(0);

AscendC::DataCopyPad<int8_t, AscendC::PaddingMode::Compact>(dstUb, srcGm, copyParams, padParams);
```

相关样例：

- [DataCopyPad GM->UB->GM 样例](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm/README.md>)，但该样例不是 UB->TSCM，也不带 `Nd2NzParams`。
- [matmul_tscm_src_vecout 代码](<../../examples/01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm_src_vecout/matmul_tscm_src_vecout.asc>)，但该样例是 VECOUT->TSCM 的连续搬运。

## 2.7.3.5.3 Load2DBitMode

当前仓未找到 `LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Load2DBitModeParam& loadDataParams)` 的同原型 examples 完整样例。`load_data_2dv2_l12l0` 是相关样例，但使用的是 `LoadData2DParamsV2` 直接调用，不是本章节的 `Load2DBitModeParam` bit mode 调用。

资料章节说明：

- `Load2DBitMode` 仅支持 Ascend 950PR/Ascend 950DT，资料表中其他产品均标记为不支持。
- 虽然资料“功能说明”列出 GM->A1、GM->B1、GM->A2、GM->B2、A1->A2、B1->B2，但本章节给出的函数原型为 `LocalTensor -> LocalTensor`，参数表说明 `Load2DBitMode` 仅支持 A1->A2、B1->B2。
- 对 GM->L1 或 GM->L0 的 bit mode Load2D，当前资料未给出 `GlobalTensor` 源操作数的对应原型，按本章节不写 GM 通路示例。

章节对应的接口原型为：

```cpp
template <TPosition Dst, TPosition Src, typename T>
__aicore__ inline void LoadData(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                const Load2DBitModeParam& loadDataParams)
```

资料没有给出具体搬运参数场景。下面补充一个对齐的 A1->A2 调用场景：源 A1 中存放一个 32x32 的 `half` 矩阵，不启用转置，搬入 L0A 的 A2 Buffer。`Load2DBitModeParam` 通过 `LoadData2DParamsV2` 构造，字段含义与 `Load2DV2` 保持一致。

示例一：使用 `LoadData2DParamsV2` 构造 `Load2DBitModeParam`

```cpp
constexpr uint32_t fractalElemCount = 256;
constexpr uint32_t tensorElemCount = 4 * fractalElemCount;

// 源操作数：L1 Buffer的A1位置，存放32x32的half矩阵。
// half类型下，一个512B分形包含16 * 16 = 256个元素，32x32对应2 * 2 = 4个分形。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, tensorElemCount);

// 目的操作数：L0A Buffer的A2位置，预留4个512B分形接收完整32x32 half矩阵。
AscendC::LocalTensor<half> dstA2(AscendC::TPosition::A2, a2Addr, tensorElemCount);

AscendC::LoadData2DParamsV2 loadDataParams;
// 从源矩阵M轴第0个16元素分组开始搬运。
loadDataParams.mStartPosition = 0;
// 从源矩阵K轴第0个32B分组开始搬运。
loadDataParams.kStartPosition = 0;
// M方向搬运32个元素，即2 * 16个元素。
loadDataParams.mStep = 2;
// K方向搬运32个half，即2 * 32B。
loadDataParams.kStep = 2;
// 源A1的M方向有2个分形，K方向相邻源分形起始地址间隔为2 * 512B。
loadDataParams.srcStride = 2;
// 目的A2按2个M方向分形连续排布，K方向相邻目的分形起始地址间隔为2 * 512B。
loadDataParams.dstStride = 2;
// 不对每个分形做转置，保持A1中的分形方向搬入A2。
loadDataParams.ifTranspose = false;
// 预留参数，固定配置为0。
loadDataParams.sid = 0;

// 使用LoadData2DParamsV2构造bit mode参数，底层以bit位形式传递上述搬运配置。
AscendC::Load2DBitModeParam bitModeParams(loadDataParams);
AscendC::LoadData<AscendC::TPosition::A2, AscendC::TPosition::A1, half>(
    dstA2, srcA1, bitModeParams);
```

示例二：使用 `Load2DBitModeParam` 的 Set 函数配置同一场景

`Load2DBitModeParam` 也可以默认构造后通过 Set 函数逐项修改参数值。当前 Set 接口没有单独的 `sid` 设置函数；默认构造会将 bit mode 配置清零，本例只设置与同一 32x32 `half` A1->A2 搬运场景相关的字段。

```cpp
constexpr uint32_t fractalElemCount = 256;
constexpr uint32_t tensorElemCount = 4 * fractalElemCount;

// 源操作数：L1 Buffer的A1位置，存放32x32的half矩阵。
// half类型下，一个512B分形包含16 * 16 = 256个元素，32x32对应2 * 2 = 4个分形。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, tensorElemCount);

// 目的操作数：L0A Buffer的A2位置，预留4个512B分形接收完整32x32 half矩阵。
AscendC::LocalTensor<half> dstA2(AscendC::TPosition::A2, a2Addr, tensorElemCount);

AscendC::Load2DBitModeParam bitModeParams;
// 从源矩阵M轴第0个16元素分组开始搬运。
bitModeParams.SetMStartPosition(0);
// 从源矩阵K轴第0个32B分组开始搬运。
bitModeParams.SetKStartPosition(0);
// M方向搬运32个元素，即2 * 16个元素。
bitModeParams.SetMStep(2);
// K方向搬运32个half，即2 * 32B。
bitModeParams.SetKStep(2);
// 源A1的M方向有2个分形，K方向相邻源分形起始地址间隔为2 * 512B。
bitModeParams.SetSrcStride(2);
// 目的A2按2个M方向分形连续排布，K方向相邻目的分形起始地址间隔为2 * 512B。
bitModeParams.SetDstStride(2);
// 不对每个分形做转置，保持A1中的分形方向搬入A2。
bitModeParams.SetIfTranspose(false);

AscendC::LoadData<AscendC::TPosition::A2, AscendC::TPosition::A1, half>(
    dstA2, srcA1, bitModeParams);
```

相关样例：

- [LoadData_2DV2_L12L0 README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0/README.md>)，但该样例直接使用 `LoadData2DParamsV2`，不是 `Load2DBitModeParam`。
- [LoadData_2DV2_L12L0 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_2dv2_l12l0/load_data_2dv2_l12l0.asc>)，可作为普通 L1->L0 `Load2DV2` 参数配置参考。

## 2.7.3.5.9 L1->Bias Table Buffer 数据搬运 DataCopy

资料章节说明 `DataCopy` 支持将矩阵计算使用的 Bias 参数从 L1 Buffer 搬运到 Bias Table Buffer；支持 Ascend 950PR/Ascend 950DT、Atlas A3、Atlas A2，Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品、Kirin X90、Kirin 9030 不支持。源操作数为 `C1`，目的操作数为 `C2`，源地址要求 32B 对齐，目的地址要求 64B 对齐。

章节对应的接口原型包括：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                const DataCopyParams& repeatParams)

template <typename T, typename U>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<U>& src,
                                const DataCopyParams& repeatParams)
```

当前仓已找到 C1->C2 `DataCopyParams` 重载的同通路调用样例。资料列出的 `count` 重载和源/目的异类型重载，当前 `examples` 中未找到完整调用；下面只粘贴仓内已有的 `DataCopyParams` 片段。

使用示例片段：

L1(C1)->Bias Table Buffer(C2)，搬运 Bias。`data_copy_gm2l1` 场景4中 `M = 128`、`K = 128`、`N/baseN = 256`、`outputType = float`、`biasSizeAlign = 256` 个 `float`。

```cpp
// c1Addr = 128 * 1024、c2Addr = 0；__NPU_ARCH__ == 3510时c12c2Params = {1, 32, 0, 0}，否则为 {1, 16, 0, 0}。
AscendC::LocalTensor<outputType> bias1Local(AscendC::TPosition::C1, c1Addr, biasSizeAlign);
AscendC::LocalTensor<outputType> bias2Local(AscendC::TPosition::C2, c2Addr, biasSizeAlign);

if constexpr (SCENARIO_NUM == 4) {
    SplitBias(bias1Local, bias2Local);
}

__aicore__ inline void SplitBias(AscendC::LocalTensor<outputType> bias1Local,
                                AscendC::LocalTensor<outputType> bias2Local)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    AscendC::DataCopyParams c12c2Params =
        {1, static_cast<uint16_t>(biasSizeAlign * sizeof(outputType) / 32), 0, 0};
#else
    AscendC::DataCopyParams c12c2Params =
        {1, static_cast<uint16_t>(biasSizeAlign * sizeof(outputType) / 64), 0, 0};
#endif
    AscendC::DataCopy(bias2Local, bias1Local, c12c2Params);
}
```

完整示例请参考：

- [DataCopy_GM2L1 README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/README.md>)
- [DataCopy_GM2L1 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1/data_copy_gm2l1.asc>)

相关样例：

- [mmad 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad/mmad.asc>)，也包含 C1->C2 `DataCopyParams` 搬运 Bias 的调用。
- [Tensor API CopyL12BT 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_in_tensor_api/copy_in_tensor_api.asc>)，但这是 Tensor API 的 `CopyL12BT`，不是本章节基础 API `DataCopy` 原型。

## 2.7.3.5.10 L1->Fixpipe Buffer 数据搬运 DataCopy

资料章节说明 `DataCopy` 支持将随路量化参数从 L1 Buffer 搬运到 Fixpipe Buffer；支持 Ascend 950PR/Ascend 950DT、Atlas A3、Atlas A2，Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品、Kirin X90、Kirin 9030 不支持。源操作数为 L1 Buffer，示例中使用 `C1`；目的操作数为 Fixpipe Buffer，逻辑位置为 `C2PIPE2GM`。资料说明该通路仅支持 `uint64_t`。

章节对应的接口原型包括：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)

template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                const DataCopyParams& repeatParams)
```

当前仓已找到 C1->C2PIPE2GM `DataCopyParams` 重载的同通路调用样例。资料列出的 `count` 重载，当前 `examples` 中未找到完整调用；下面只粘贴仓内已有的 `DataCopyParams` 片段。

使用示例片段：

L1(C1)->Fixpipe Buffer(C2PIPE2GM)，搬运 Vector 量化参数。`data_copy_l0c2gm` 中 `M = 128`、`K = 128`、`N = 256`、`K_ROUND = 2`、`n = N = 256`，量化参数类型为 `uint64_t`。

```cpp
// c1Addr = 0、fbAddr = 0、N = 256、n = 256；burstLen = CeilAlign(256 * sizeof(uint64_t), 128) / 128 = 16，dataCopyParams = {1, 16, 0, 0}。
AscendC::LocalTensor<uint64_t> quantAlphaTensor(AscendC::TPosition::C1, c1Addr, N);

if constexpr (scenarioNum == 2 || scenarioNum == 4 || scenarioNum == 6) {
    CopyQuantAlphaGmToL1(quantAlphaTensor);
}

// vector quant mode
if constexpr (scenarioNum == 2) {
    intriParams.quantPre = QuantMode_t::VDEQF16;
} else if constexpr (scenarioNum == 4) {
    intriParams.quantPre = QuantMode_t::VQF322B8_PRE;
} else {
    intriParams.quantPre = QuantMode_t::VREQ8;
}
AscendC::LocalTensor<uint64_t> fbTensor(AscendC::TPosition::C2PIPE2GM, fbAddr, N);
uint16_t burstLen = CeilAlign(n * sizeof(uint64_t), 128) / 128;
AscendC::DataCopyParams dataCopyParams(1, burstLen, 0, 0);
AscendC::DataCopy(fbTensor, quantAlphaTensor, dataCopyParams);
AscendC::SetFixPipeConfig(fbTensor);
```

完整示例请参考：

- [DataCopy_L0C2GM README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm/README.md>)
- [DataCopy_L0C2GM 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm/data_copy_l0c2gm.asc>)

相关样例：

- [Fixpipe_L0C2GM 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm/fixpipe_l0c2gm.asc>)，但该样例直接把 `quantAlphaTensor` 传给 `Fixpipe`，不是本章节 C1->C2PIPE2GM 的 `DataCopy` 调用。
- [SetFixPipeConfig 文档](<../api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/SetFixPipeConfig.md>)，包含相近的 C1->C2PIPE2GM 配置片段。

## 2.7.3.6.8 SetAippFunctions

当前仓未找到 `SetAippFunctions` 的 examples 同原型完整样例。资料章节说明该接口用于设置 AIPP 图片预处理参数，需要和 `LoadImageToLocal` 配合使用；Atlas 推理系列产品 Vector Core、Atlas 训练系列产品不支持，资料表中其他产品支持。

本节资料来源：

- [SetAippFunctions 文档](<../api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/SetAippFunctions.md>)
- [LoadImageToLocal 文档](<../api/SIMD-API/基础API/矩阵计算（ISASI）/数据搬运/LoadImageToLocal.md>)

章节对应的接口原型包括：

```cpp
template<typename T, typename U>
__aicore__ inline void SetAippFunctions(const GlobalTensor<T>& src0, AippInputFormat format,
                                        AippParams<U> config)

template<typename T, typename U>
__aicore__ inline void SetAippFunctions(const GlobalTensor<T>& src0, const GlobalTensor<T>& src1,
                                        AippInputFormat format, AippParams<U> config)
```

`SetAippFunctions` 需要先于 `LoadImageToLocal` 调用，二者共用同一张输入图片和同一组 AIPP 配置；合并调用示例见下一节。

## 2.7.3.6.9 LoadImageToLocal

当前仓未找到 `LoadImageToLocal` 的 examples 同原型完整样例。资料章节说明该接口把图像数据从 GM 搬运到 Local Memory，搬运过程中使用 `SetAippFunctions` 配置好的 AIPP 参数完成预处理；Atlas 推理系列产品 Vector Core、Atlas 训练系列产品不支持，资料表中其他产品支持。

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void LoadImageToLocal(const LocalTensor<T>& dst,
                                        const LoadImageToLocalParams& loadDataParams)
```

下面沿用资料中的 Atlas 推理系列产品 AI Core、32x32 YUV420SP 场景，合并展示 `SetAippFunctions` 与 `LoadImageToLocal` 的调用顺序：输入 `T` 为 `uint8_t`，输出 `U` 为 `int8_t`，不做 HW 方向 padding、通道交换、单行读取和色域转换，仅配置通道 padding 到 32B。目的 Tensor 使用 `A1`，`cPaddingMode = 0` 时通道 padding 到 32B，因此目的空间按 `32 * 32 * 32` 个 `int8_t` 预留。Ascend 950PR/Ascend 950DT 的资料支持 TPosition 为 `VECIN`、`VECCALC`、`VECOUT`，不使用本示例的 `A1`。

合并调用示例：

```cpp
constexpr uint16_t imageWidth = 32;
constexpr uint16_t imageHeight = 32;
constexpr uint32_t yPlaneSize = imageWidth * imageHeight;
constexpr uint32_t inputSize = yPlaneSize * 3 / 2;
constexpr uint32_t outputChannels = 32;
constexpr uint32_t dstElemCount = imageWidth * imageHeight * outputChannels;

// 源操作数：GM上一张32x32 YUV420SP图片，前1024B为Y平面，后512B为UV平面。
AscendC::GlobalTensor<uint8_t> fmGlobal;
fmGlobal.SetGlobalBuffer((__gm__ uint8_t *)src, inputSize);

// 目的操作数：Atlas推理系列产品AI Core场景使用L1 Buffer的A1位置。
AscendC::LocalTensor<int8_t> featureMapA1(AscendC::TPosition::A1, a1Addr, dstElemCount);

// format = YUV420SP_U8，表示输入为uint8_t类型的YUV420 Semi-Planar图片。
AscendC::AippInputFormat inputFormat = AscendC::AippInputFormat::YUV420SP_U8;

AscendC::AippParams<int8_t> aippConfig;
// 本示例不做HW padding、通道交换、单行读取、均值/缩放和色域转换，这些子参数使用结构体默认值。
// 输出U = int8_t时将通道padding到32通道，即每个像素输出32B。
aippConfig.cPaddingParams.cPaddingMode = 0;
// 通道padding补0，目的Tensor额外通道写入0。
aippConfig.cPaddingParams.cPaddingValue = static_cast<int8_t>(0);

// src0为Y平面起始地址；src1为UV平面起始地址，YUV420SP 32x32的UV偏移为32 * 32 = 1024个uint8_t。
// AippParams<int8_t> 的输出类型U需要与featureMapA1的LocalTensor<int8_t> 保持一致。
AscendC::SetAippFunctions(fmGlobal, fmGlobal[yPlaneSize], inputFormat, aippConfig);

AscendC::LoadImageToLocalParams loadParams;
// 从源图加载32像素宽，等于整张32x32示例图的宽度。
loadParams.horizSize = 32;
// 从源图加载32像素高，等于整张32x32示例图的高度。
loadParams.vertSize = 32;
// 水平起始位置为第0个像素；YUV420SP场景要求为偶数，0满足约束。
loadParams.horizStartPos = 0;
// 垂直起始位置为第0行；YUV420SP场景要求为偶数，0满足约束。
loadParams.vertStartPos = 0;
// 源图每行宽度为32像素；YUV420SP场景要求为偶数，32满足约束。
loadParams.srcHorizSize = 32;
// 顶部不做HW方向padding。
loadParams.topPadSize = 0;
// 底部不做HW方向padding。
loadParams.botPadSize = 0;
// 左侧不做HW方向padding。
loadParams.leftPadSize = 0;
// 右侧不做HW方向padding。
loadParams.rightPadSize = 0;
// 预留参数，固定配置为0。
loadParams.sid = 0;

AscendC::LoadImageToLocal(featureMapA1, loadParams);
```

## 2.7.5.6.1 L1->GM 连续数据搬运 DataCopy

资料章节说明该接口将矩阵从 L1 Buffer 连续搬运到 GM，格式和内容保持不变。支持 Atlas A3、Atlas A2、Kirin X90、Kirin 9030；Ascend 950PR/Ascend 950DT、Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品资料标记为不支持。

本节资料来源：

- [L1ToGM连续数据搬运（DataCopy）文档](<../../../asc-devkit_zl/docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM连续数据搬运（DataCopy）.md>)

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
                                const uint32_t count)
```

当前仓已找到 A2/A3 分支的同原型调用样例。样例同时包含 Ascend 950PR/Ascend 950DT 的兼容绕行分支，但该分支不是本章节 `DataCopy(GlobalTensor, LocalTensor, count)` 直接调用。

使用示例片段：

L1(A1)->GM，连续搬运 64x128 的 `float` 矩阵。`data_copy_l1togm` 样例中 `T = float`、`S = float`、`M = 64`、`K = 128`，所以 `count = M * K = 8192` 个 `float`，实际搬运 32768B，满足 `count * sizeof(T)` 32B 对齐约束。

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    // Atlas A2/A3训练/推理系列产品
    AscendC::LocalTensor<T> leftMatrix(AscendC::TPosition::A1, a1Addr, M * K);
    // GM -> L1(A1)
    AscendC::DataCopy(leftMatrix, aGlobal, M * K);
    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    // L1(A1) -> GM
    AscendC::DataCopy(cGlobal, leftMatrix, M * K);
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    // Ascend 950PR/950DT不支持直接L1 -> GM，样例通过Mmad + Fixpipe绕行。
#endif
```

完整示例请参考：

- [DataCopy_L1ToGM README](<../../examples/01_simd_cpp_api/05_compatibility_guide/data_copy_l1togm/README.md>)
- [DataCopy_L1ToGM 代码](<../../examples/01_simd_cpp_api/05_compatibility_guide/data_copy_l1togm/data_copy_l1togm.asc>)

注意：Ascend 950PR/Ascend 950DT 资料不支持直接 L1->GM，本兼容性样例在 dav-3510 分支通过 `GM -> A1/B1 -> L0A/L0B -> L0C -> GM` 绕行，不属于本章节直接 `DataCopy` 原型。

## 2.7.5.6.2 L1->GM 高维切分数据搬运 DataCopy

资料章节说明该接口将矩阵从 L1 Buffer 搬运到 GM，格式和内容保持不变；通过 `DataCopyParams` 配置数据块个数、单块长度和相邻块间隔，可同时表达连续搬运和非连续搬运。支持 Atlas A3、Atlas A2、Kirin X90、Kirin 9030；Ascend 950PR/Ascend 950DT、Atlas 200I/500 A2、Atlas 推理系列产品 AI Core/Vector Core、Atlas 训练系列产品资料标记为不支持。

本节资料来源：

- [L1ToGM高维切分数据搬运（DataCopy）文档](<../../../asc-devkit_zl/docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到GM数据搬运/L1ToGM高维切分数据搬运（DataCopy）.md>)

章节对应的接口原型为：

```cpp
template <typename T>
__aicore__ inline void DataCopy(const GlobalTensor<T>& dst, const LocalTensor<T>& src,
                                const DataCopyParams& repeatParams)
```

当前仓找到 `fixpipe_l0c2l1` 中 L1(C1)->GM 的 `DataCopyParams` 连续搬运相关片段，但未找到资料图示的非连续高维切分完整样例。下面按资料分别补充连续搬运和非连续搬运两个具体调用示例。

调用示例一：连续搬运

```cpp
constexpr uint32_t srcElemCount = 128;
constexpr uint32_t dstElemCount = 128;

// 源操作数：L1 Buffer的A1位置，连续存放128个half。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, srcElemCount);

// 目的操作数：GM上预留128个half，接收从L1连续搬出的数据。
AscendC::GlobalTensor<half> dstGm;
dstGm.SetGlobalBuffer((__gm__ half *)dst, dstElemCount);

AscendC::DataCopyParams repeatParams;
// 资料图示为1个连续传输数据块，表示连续搬运。
repeatParams.blockCount = 1;
// 单块长度为8个DataBlock，即8 * 32B = 256B，等于128个half。
repeatParams.blockLen = 8;
// blockCount = 1时源端间隔无意义，设置为0。
repeatParams.srcGap = 0;
// blockCount = 1时目的端间隔无意义，设置为0。
repeatParams.dstGap = 0;

AscendC::DataCopy(dstGm, srcA1, repeatParams);
```

调用示例二：非连续搬运

```cpp
constexpr uint32_t srcElemCount = 112;
constexpr uint32_t dstElemCount = 128;

// 源操作数：L1 Buffer的A1位置，按“48个有效half + 16个跳过half + 48个有效half”排布。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, srcElemCount);

// 目的操作数：GM上预留128个half，两个有效数据块之间保留32个half间隔。
AscendC::GlobalTensor<half> dstGm;
dstGm.SetGlobalBuffer((__gm__ half *)dst, dstElemCount);

AscendC::DataCopyParams repeatParams;
// 资料图示搬运2个连续传输数据块。
repeatParams.blockCount = 2;
// 每个数据块长度为3个DataBlock，即96B，等于48个half。
repeatParams.blockLen = 3;
// 源端相邻数据块之间跳过1个DataBlock，即32B，等于16个half。
repeatParams.srcGap = 1;
// 目的端相邻数据块之间间隔2个DataBlock，即64B，等于32个half。
repeatParams.dstGap = 2;

AscendC::DataCopy(dstGm, srcA1, repeatParams);
```

相关样例：

- [Fixpipe_L0C2L1 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1/fixpipe_l0c2l1.asc>)，其中 A2/A3 分支包含 L1(C1)->GM 的 `DataCopyParams` 连续搬运片段，但未覆盖本节资料的非连续高维切分场景。
- [DataCopy_L1ToGM README](<../../examples/01_simd_cpp_api/05_compatibility_guide/data_copy_l1togm/README.md>)，说明 Ascend 950PR/Ascend 950DT 不支持直接 L1->GM，需要通过 `GM -> A1/B1 -> L0A/L0B -> L0C -> GM` 绕行，不属于本章节直接 `DataCopy` 原型。

## 补充项：L0C->UB 双目标模式 Fixpipe

当前仓已找到 L0C->UB 双目标模式的完整样例。基础 API 样例为 `fixpipe_l0c2ub`，覆盖按 M 维度拆分和按 N 维度拆分两个场景；另有 Tensor API 的 `copy_out_tensor_api` 相关样例，使用 `CopyL0C2UB` 和 `DUAL_DST_SPLIT_M/N`。

使用示例片段：

示例一：按 M 维度拆分

`fixpipe_l0c2ub` 样例规格为 `baseM = 128`、`baseN = 256`、`CUBE_BLOCK = 16`，输出数据类型为 `float`，L0C 数据类型为 `float`，`CFG_ROW_MAJOR_UB` 表示输出到 UB 且布局为 `ROW_MAJOR`。

```cpp
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
// 按M维度拆分时，M必须为2的倍数；CeilAlign(128,2)=128。
fixpipeParams.mSize = CeilAlign(baseM, 2);
// 输出N维度为256。
fixpipeParams.nSize = baseN;
// L0C源端M方向stride为CeilAlign(128,16)=128。
fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
// UB目的端ND布局每行stride为256。
fixpipeParams.dstStride = baseN;
// dualDstCtl=0b01，表示双目标模式按M维度拆分，两个子块各接收64x256个float。
fixpipeParams.dualDstCtl = 0b01;
AscendC::Fixpipe<outputType, l0cType, CFG_ROW_MAJOR_UB>(cUB, c, fixpipeParams);
```

示例二：按 N 维度拆分

`fixpipe_l0c2ub` 样例规格同上，`baseM = 128`、`baseN = 256`、`CUBE_BLOCK = 16`，输出数据类型为 `float`，L0C 数据类型为 `float`。

```cpp
AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
// 输出M维度为128。
fixpipeParams.mSize = baseM;
// 按N维度拆分时，N必须为32的倍数；CeilAlign(256,32)=256。
fixpipeParams.nSize = CeilAlign(baseN, 32);
// L0C源端M方向stride为CeilAlign(128,16)=128。
fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
// dstStride=baseN/2=128，两个目标各接收128列后目的端连续排列。
fixpipeParams.dstStride = baseN / 2;
// dualDstCtl=0b10，表示双目标模式按N维度拆分，两个子块各接收128x128个float。
fixpipeParams.dualDstCtl = 0b10;
AscendC::Fixpipe<outputType, l0cType, CFG_ROW_MAJOR_UB>(cUB, c, fixpipeParams);
```

完整示例请参考：

- [Fixpipe_L0C2UB README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub/README.md>)
- [Fixpipe_L0C2UB 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2ub/fixpipe_l0c2ub.asc>)

相关样例：

- [CopyOut_Tensor_API README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api/README.md>)，场景 2/3 使用 Tensor API 的 `CopyL0C2UB` 实现 L0C->UB 后由 AIV 写回 GM。
- [CopyOut_Tensor_API 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_out_tensor_api/copy_out_tensor_api.asc>)，但这是 Tensor API `CopyL0C2UB`，不是本节基础 API `Fixpipe` 调用。

## 补充项：L1->UB 数据搬运 DataCopyL1ToUB

当前仓未找到 `examples` 下直接调用 `DataCopyL1ToUB` 的同原型完整样例。`data_copy_ub2l1` 是 UB->L1 反向通路，不能作为本节 L1->UB 样例；当前只找到 API 文档和实现。

资料章节说明 `DataCopyL1ToUB` 提供从 L1 Buffer 搬运数据到 Unified Buffer 的基础能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续搬运；仅 Ascend 950PR/Ascend 950DT 支持。`T` 支持 `b8`、`b16`、`b32`、`b64` 类型；`subBlockId` 表示 AI Core 上的 Vector 核 ID，用于指定搬运到哪个 Vector 核的 UB 地址。如果需要执行多条 `DataCopyL1ToUB` 指令且目的地址存在重叠，资料要求通过 `PipeBarrier(ISASI)` 串行化。

本节资料来源：

- [L1到UB数据搬运（DataCopyL1ToUB）文档](<../../../asc-devkit_zl/docs/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬出/L1到UB数据搬运（DataCopyL1ToUB）.md>)

章节对应的接口原型包括：

```cpp
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                      const uint32_t count)

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                      const DataCopyParams& repeatParams)
```

调用示例一：`count`重载连续搬运

```cpp
constexpr uint32_t copyCount = 512;

// 源操作数：L1 Buffer的A1位置，连续存放512个half。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, copyCount);

// 目的操作数：UB的VECIN位置，接收512个half。
AscendC::LocalTensor<half> dstVecIn(AscendC::TPosition::VECIN, ubAddr, copyCount);

// subBlockId=0，表示将数据搬运到Vector核ID为0的UB地址。
// count=512，表示搬运512个half元素；512*sizeof(half)=1024B，满足32B对齐约束。
AscendC::DataCopyL1ToUB<half, 0>(dstVecIn, srcA1, copyCount);
```

调用示例二：`DataCopyParams`重载连续搬运

```cpp
constexpr uint32_t copyCount = 512;

// 源操作数：L1 Buffer的A1位置，连续存放512个half。
AscendC::LocalTensor<half> srcA1(AscendC::TPosition::A1, a1Addr, copyCount);

// 目的操作数：UB的VECIN位置，接收512个half。
AscendC::LocalTensor<half> dstVecIn(AscendC::TPosition::VECIN, ubAddr, copyCount);

AscendC::DataCopyParams repeatParams;
// 资料示例使用1个连续传输数据块，表示连续搬运。
repeatParams.blockCount = 1;
// blockLen单位为DataBlock，512个half共1024B，1024B/32B=32个DataBlock。
repeatParams.blockLen = 512 * sizeof(half) / 32;
// 只有1个连续传输数据块，源端间隔设置为0。
repeatParams.srcGap = 0;
// 只有1个连续传输数据块，目的端间隔设置为0。
repeatParams.dstGap = 0;

// subBlockId=0，表示将数据搬运到Vector核ID为0的UB地址。
AscendC::DataCopyL1ToUB<half, 0>(dstVecIn, srcA1, repeatParams);
```

相关样例：

- [DataCopy_UB2L1 README](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1/README.md>)，这是 UB->L1 反向通路，不是 `DataCopyL1ToUB`。
- [DataCopy_UB2L1 代码](<../../examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_ub2l1/data_copy_ub2l1.asc>)，该样例调用的是 `DataCopy(a1Local, ubA1Local, aSize)`，不是 `DataCopyL1ToUB`。
