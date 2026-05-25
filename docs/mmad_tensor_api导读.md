# `mmad_tensor_api.asc` 样例导读：用 Tensor API 组织矩阵计算和随路量化

本文以
`examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_tensor_api/mmad_tensor_api.asc`
为例，解释这个样例如何基于 Tensor API 实现 `int8 x int8 -> int32 -> half` 的矩阵乘和随路量化写回，并穿插对比基础 API 的实现方式。

## 1. 样例要做什么

这个样例实现的是：

```text
A[M, K] int8, ND
B[K, N] int8, 逻辑上按转置输入保存，样例用 DN layout 描述
acc = A * B, L0C 中 int32 累加
C[M, N] half, ND
quant[1, N] uint64_t, Fixpipe/Copy L0C->GM 时做 per-channel 反量化
```

默认 host 侧参数在 `main` 里写死：

```text
M = 1024, N = 1024, K = 256
singleCoreM = 256, singleCoreN = 128, singleCoreK = 256
BASE_M = 256, BASE_N = 256, BASE_K = 64
STEP_K = 4
NUM_BLOCKS = 32
```

`NUM_BLOCKS = 32` 正好覆盖 `(M / singleCoreM) * (N / singleCoreN) = 4 * 8 = 32` 个核块。每个 block 负责一个 `singleCoreM x singleCoreN` 的输出子矩阵。

从官方 Ascend C API 列表看，Ascend C Kernel API 包括基础数据结构、基础 API 和高阶 API；其中 `DataCopy` 负责数据搬运，`Mmad` 完成矩阵乘加，`Fixpipe` 在矩阵计算后处理结果，例如量化并从 CO1/L0C 搬到 GM。这个样例使用的 Tensor API 没有改变底层硬件流程，仍然是 GM -> L1 -> L0A/L0B -> Mmad -> L0C -> GM，只是把“指针、shape、layout、位置”封成 Tensor，再由 `Copy`/`Mmad` 做编译期路由。

## 2. 和基础 API 样例的核心差异

基础 API 写法更接近硬件指令参数。例如
`fixpipe_co12gm_quantization_s322f16.asc` 中，GM->L1 需要手写 `Nd2NzParams`：

```cpp
AscendC::Nd2NzParams nd2nzA1Params;
nd2nzA1Params.ndNum = 1;
nd2nzA1Params.nValue = m;
nd2nzA1Params.dValue = k;
nd2nzA1Params.srcDValue = k;
nd2nzA1Params.dstNzC0Stride = CeilAlign(m, CUBE_BLOCK);
nd2nzA1Params.dstNzNStride = 1;
AscendC::DataCopy(a1Local, aGM[kIndex * aSingleSize], nd2nzA1Params);
```

Tensor API 写法把这些信息拆到 Tensor 的 layout 中：

```cpp
auto gmATensor = MakeTensor(MakeMemPtr(x), MakeFrameLayout<NDExtLayoutPtn>(m, k));
auto l1ATensor = MakeTensor(
    MakeMemPtr(l1ABuf),
    MakeFrameLayout<NZLayoutPtn, A_TYPE>(baseM * stepM, baseK * stepK)
);

Copy(CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{},
    l1ATensor,
    gmASingleTensor.Slice(MakeCoord(i * baseM, kIter * baseK * stepK),
                          MakeShape(baseM, baseK * stepK)));
```

也就是说，基础 API 用户直接描述 `ndNum/nValue/dValue/stride`；Tensor API 用户描述“源是 GM 上的 ND 视图，目的地是 L1 上的 NZ 视图”。`impl/tensor_api/arch/cube/gm_to_l1/routing.h` 会根据 `Location::L1 <- Location::GM` 和 `NZ <- ND` 选择 `CopyGmToCbufMultiND2Nz`，再从 layout 反推出底层 `DataCopy` 参数。

## 3. 单核切分：先确定当前 block 负责哪块 C

核函数开头：

```cpp
uint32_t mIter = (m + singleCoreM - 1) / singleCoreM;
uint32_t mIterIdx = AscendC::GetBlockIdx() % mIter;
uint32_t nIterIdx = AscendC::GetBlockIdx() / mIter;
```

含义是按 M 优先铺 block：

```text
blockIdx -> (mIterIdx, nIterIdx)
当前 block 负责：
A: [mIterIdx * singleCoreM : ..., 0 : singleCoreK]
B: [0 : singleCoreK, nIterIdx * singleCoreN : ...]
C: [mIterIdx * singleCoreM : ..., nIterIdx * singleCoreN : ...]
quant: [0, nIterIdx * singleCoreN : ...]
```

随后 `actualSingleCoreM/N` 处理尾块。默认 shape 下没有 M 尾块，N 方向 `singleCoreN = 128`，但 `BASE_N = 256`，所以每个 block 的 `nLoop = 1`、`curN = 128`，这是一个小于 `BASE_N` 的尾 tile。

## 4. Tensor 构造：把物理位置和 layout 放入类型系统

样例先构造 GM Tensor：

```cpp
auto gmATensor = MakeTensor(MakeMemPtr(x), MakeFrameLayout<NDExtLayoutPtn>(m, k));
auto gmBTensor = MakeTensor(MakeMemPtr(y), MakeFrameLayout<DNExtLayoutPtn>(k, n));
auto gmCTensor = MakeTensor(MakeMemPtr(z), MakeFrameLayout<NDExtLayoutPtn>(m, n));
auto gmQuantTensor = MakeTensor(MakeMemPtr(quant), MakeFrameLayout<NDExtLayoutPtn>(1, n));
```

这里有三个关键信息：

- `MakeMemPtr(x)` 从 `__gm__ A_TYPE*` 推出 `Location::GM`。
- `MakeFrameLayout<NDExtLayoutPtn>(m, k)` 表示 GM 中 A 是普通二维 ND 视图。
- `MakeFrameLayout<DNExtLayoutPtn>(k, n)` 表示 B 采用列优先/转置视图，配合后续目标 `ZNLayoutPtn`，让 GM->L1 走 DN->ZN 的搬运路径。

再用 `Slice` 裁出当前 block 的 GM 子视图：

```cpp
auto gmASingleTensor = gmATensor.Slice(
    MakeCoord(mIterIdx * singleCoreM, 0),
    MakeShape(actualSingleCoreM, singleCoreK)
);
```

`Slice(coord, shape)` 不搬数据，只是把起始地址和 layout 变成新的 Tensor 视图。基础 API 通常用 `aGM[offset]` 这种一维 offset 表达相同含义；Tensor API 用坐标表达，代码更接近矩阵语义。

## 5. 本地 Buffer：显式声明 L1/L0，但用 Tensor 包装

样例仍然手动声明硬件 buffer：

```cpp
__cbuf__ A_TYPE l1ABuf[baseM * stepM * baseK * stepK];
__cbuf__ B_TYPE l1BBuf[baseK * stepK * baseN * stepN];
__cbuf__ uint64_t l1QuantBuf[baseN * stepN];
__ca__ A_TYPE l0ABuf[baseM * baseK];
__cb__ B_TYPE l0BBuf[baseK * baseN];
__cc__ L0C_TYPE l0CBuf[baseM * baseN];
```

这说明 Tensor API 不是 `TPipe/TQue` 式的自动流水管理。本样例走的是静态 buffer + 手动事件同步：

```text
L1:   __cbuf__
L0A:  __ca__
L0B:  __cb__
L0C:  __cc__
```

随后把 L1 buffer 做成矩阵 layout：

```cpp
auto l1ATensor = MakeTensor(
    MakeMemPtr(l1ABuf),
    MakeFrameLayout<NZLayoutPtn, A_TYPE>(baseM * stepM, baseK * stepK)
);
auto l1BTensor = MakeTensor(
    MakeMemPtr(l1BBuf),
    MakeFrameLayout<ZNLayoutPtn, B_TYPE>(baseK * stepK, baseN * stepN)
);
```

这一步非常关键：A 到 L1 后已经是 `NZ`，B 到 L1 后是 `ZN`，后续从 L1 裁出当前 K tile 再进 L0A/L0B。

## 6. 主循环：M、N、K 三层 tile

主循环结构是：

```text
for i in mLoop:
  curM = 当前 M tile 大小
  for j in nLoop:
    curN = 当前 N tile 大小
    l0CTensor = L0C(curM, curN)
    for kIter in kLoop:
      curK = 当前 K tile 大小
      GM -> L1，每 stepK 个 K tile 搬一次
      L1 -> L0A/L0B，每个 K tile 搬一次
      Mmad 累加到 L0C
    quant GM -> L1
    L0C -> GM，带 vector quant
```

默认 `singleCoreK = 256`、`BASE_K = 64`，所以 `kLoop = 4`。`STEP_K = 4` 意味着一次 GM->L1 搬完整个单核 K 范围，即 A 搬 `256 x 256`，B 搬 `256 x curN`，然后内部 4 次 L1->L0 + Mmad。

## 7. GM -> L1：用 layout 触发 ND/DN 到 NZ/ZN 的格式搬运

A 的搬运：

```cpp
Copy(CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{},
    l1ATensor,
    gmASingleTensor.Slice(MakeCoord(i * baseM, kIter * baseK * stepK),
                          MakeShape(baseM, baseK * stepK)));
```

此时目的 Tensor 是：

```text
Location::L1 + NZLayoutPtn
```

源 Tensor 是：

```text
Location::GM + NDExtLayoutPtn
```

`impl/tensor_api/arch/cube/gm_to_l1/routing.h` 中有对应特化：

```cpp
CopyGM2L1Tensor2Tensor<Location::L1, Location::GM,
    ArchVersion::V3510, NZLayoutPtn, NDExtLayoutPtn>
    -> CopyGmToCbufMultiND2Nz
```

B 的搬运同理，只是目的 layout 是 `ZN`，源 layout 是 `DNExt`：

```cpp
Copy(CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{},
    l1BTensor,
    gmBSingleTensor.Slice(MakeCoord(kIter * baseK * stepK, j * baseN),
                          MakeShape(baseK * stepK, baseN)));
```

会路由到：

```cpp
CopyGM2L1Tensor2Tensor<Location::L1, Location::GM,
    ArchVersion::V3510, ZNLayoutPtn, DNExtLayoutPtn>
    -> CopyGmToCbufMultiDN2Zn
```

基础 API 对应写法需要手工构造 `Nd2NzParams` 或相关 transpose 参数；Tensor API 的收益是把这些参数从 layout 里推导出来。

## 8. L1 -> L0A/L0B：用 L1 的分形 layout 裁 K tile

每个 `kIter` 都会从 L1 裁一个 `curK` tile：

```cpp
Copy(CopyAtom<CopyTraits<CopyL12L0A, CopyL12L0ATraitDefault>>{},
    l0ATensor,
    l1ATensor.Slice(MakeCoord(0, kIter % stepK * baseK), MakeShape(curM, curK))
);

Copy(CopyAtom<CopyTraits<CopyL12L0B, CopyL12L0BTraitDefault>>{},
    l0BTensor,
    l1BTensor.Slice(MakeCoord(kIter % stepK * baseK, 0), MakeShape(curK, curN))
);
```

这里 `kIter % stepK` 的原因是：L1 buffer 一次缓存 `stepK` 个 K tile，当前 K tile 在 L1 内部的位置是 `(kIter % stepK) * baseK`。

Tensor API 的 `CopyL12L0A` 和 `CopyL12L0B` 会读取目的/源 layout pattern：

```text
L0A 目标: NZLayoutPtn
L1A 源:   NZLayoutPtn
L0B 目标: ZNLayoutPtn
L1B 源:   ZNLayoutPtn
```

在 `impl/tensor_api/arch/cube/l1_to_l0a/copy.h` 和 `l1_to_l0b/copy.h` 中，代码根据：

```cpp
constexpr auto noTrans = Std::is_same_v<DstPattern, SrcPattern>;
constexpr auto isB8B4Type = sizeof(typename T::elementType) == 1;
```

选择 normal/transpose/B8B4 等 LoadData 模式。基础 API 样例里这部分通常要显式写：

```cpp
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = ...;
loadDataParams.srcStride = ...;
loadDataParams.dstGap = ...;
AscendC::LoadData(...);
```

或者对 B 使用 `LoadDataWithTranspose`。Tensor API 把“要不要 transpose、以什么分形方式搬”变成 layout pattern 的编译期推导。

## 9. Mmad：每个 K tile 累加一次

样例中的参数：

```cpp
MmadParams para;
para.m = curM;
para.n = curN;
para.k = curK;
para.cmatrixInitVal = (kIter == 0) ? true : false;
if constexpr (ENABLE_UNITFLAG) {
    para.unitFlag = (kIter == kLoop - 1) ? 3 : 2;
}
Mmad(l0CTensor, l0ATensor, l0BTensor, para);
```

语义是：

- `kIter == 0` 时 `cmatrixInitVal = true`，表示 L0C 先清零或初始化。
- 后续 K tile 设置为 `false`，表示继续累加到同一块 L0C。
- `unitFlag = 2/3` 用于和后续 Fixpipe/Copy L0C->GM 的细粒度并行配合；最后一个 K tile 设置为 3，其余设置为 2。

实现层的 Tensor API 路径是：

```text
MmadAtom / MmadTraits
  -> MmadOperation::Mmad
  -> MmadImpl 根据 dst/fm/filter 的 Location 选择路由
  -> MmadTensor2Tensor<Location::L0C, Location::L0A, Location::L0B, ...>
  -> Mmad3510
  -> MmadInstr::Mmad(...)
```

在 `impl/tensor_api/arch/cube/mmad/npu_arch_3510/mmad.h` 里，最终会调用：

```cpp
MmadInstr::Mmad(dst, fm, filter,
    params.m, params.k, params.n,
    params.unitFlag,
    trait.disableGemv,
    trait.cmatrixSource,
    params.cmatrixInitVal);
```

### 接口注意点

同仓其他 Tensor API 样例，例如 `matmul_tensor_api/matmul.asc` 和 `copy_in_tensor_api.asc`，使用的是显式 `MmadAtom` 写法：

```cpp
auto mmadAtom = MakeMmad(MmadOperation{}, MmadTraitDefault{});
Mmad(mmadAtom.with(para), l0CTensor, l0ATensor, l0BTensor);
```

当前 `mmad_tensor_api.asc` 第 152 行写成：

```cpp
Mmad(l0CTensor, l0ATensor, l0BTensor, para);
```

从本仓 `include/tensor_api/algorithm/mmad.h` 暴露的 Tensor API 原型看，推荐按显式 atom 形式理解和书写：

```cpp
auto mmadAtom = MakeMmad(MmadOperation{}, MmadTraitDefault{});
Mmad(mmadAtom.with(para), l0CTensor, l0ATensor, l0BTensor);
```

如果当前代码依赖外部版本里额外提供的便捷重载，则语义仍等价于上面的 atom 路径；如果只按本仓头文件编译，建议统一改成显式 `MakeMmad` 形式。

## 10. L0C -> GM：Copy 语义下的 Fixpipe 随路量化

Mmad 完成后，样例先把量化系数从 GM 搬到 L1：

```cpp
auto l1QuantTensor = MakeTensor(l1QuantPtr, MakeFrameLayout<NDExtLayoutPtn>(1, curN));
Copy(CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{},
    l1QuantTensor,
    gmQuantSingleTensor.Slice(MakeCoord(0, j * baseN), MakeShape(1, curN))
);
```

然后等待 Mmad 的 L0C 结果可被 Fixpipe 使用：

```cpp
AscendC::WaitFlag<AscendC::HardEvent::M_FIX>(L0C_EVENT_ID);
```

最后通过 `CopyL0C2GM` 把 L0C 写到 GM：

```cpp
FixpipeParams params;
params.unitFlag = 3;
Copy(CopyAtom<CopyTraits<CopyL0C2GM, CopyL0C2GMTraitDefault>>{}.with(params),
    gmCSilce,
    l0CTensor,
    l1QuantTensor
);
```

这里虽然接口名仍是 `Copy`，但底层走的是 L0C->GM 的 Fixpipe/CO1 搬运语义。`impl/tensor_api/arch/cube/l0c_to_gm/copy.h` 对第三个参数分两类：

- `uint64_t` 标量：scalar quant。
- Tensor API Tensor：vector quant。

本样例传入 `l1QuantTensor`，所以走 vector quant 路径：

```text
CopyL0C2GMVectorQuantTensor2Tensor<Location::GM, Location::L0C, V3510>
  -> DataCopyL0C2GMVectorQuant3510
```

在 `l0c2out_utils.h` 中，`GetQuantMode` 会根据源/目的类型和量化参数类型推导量化模式：

```text
src = int32_t(L0C), dst = half(GM), quant = Tensor
  -> QuantMode_t::VDEQF16
```

基础 API 对应写法通常是直接构造 `FixpipeParamsV220`：

```cpp
AscendC::FixpipeParamsV220 fixpipeParams;
fixpipeParams.nSize = n;
fixpipeParams.mSize = m;
fixpipeParams.srcStride = ...;
fixpipeParams.dstStride = ...;
fixpipeParams.quantPre = QuantMode_t::VDEQF16;
AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>(
    cGM, c, quantAlphaTensor, fixpipeParams);
```

Tensor API 则从 `gmCSlice` 的 ND layout、`l0CTensor` 的 NZ layout、`C_TYPE=half`、`L0C_TYPE=int32_t` 和 `l1QuantTensor` 推导出 `nSize/mSize/srcStride/dstStride/nz2nd` 以及 `VDEQF16`。

## 11. 事件同步：Tensor API 没有替你隐藏流水

样例仍然显式管理硬件事件：

```text
L1_EVENT_ID:    MTE2(GM->L1) 与 MTE1(L1->L0) 同步
L0_EVENT_ID:    MTE1(L1->L0) 与 M(Mmad) 同步
L0C_EVENT_ID:   M(Mmad) 与 FIX(L0C->GM) 同步
QUANT_EVENT_ID: MTE2(quant GM->L1) 与 FIX 同步
```

典型节奏是：

```text
Wait MTE1_MTE2
GM -> L1
Set MTE2_MTE1 / Wait MTE2_MTE1

Wait M_MTE1
L1 -> L0A/L0B
Set MTE1_M / Wait MTE1_M

Mmad
最后一个 K tile Set M_FIX

Wait FIX_MTE2
quant GM -> L1
Set MTE2_FIX / Wait MTE2_FIX

Wait M_FIX
L0C -> GM with quant
Set FIX_M / Set FIX_MTE2
```

所以这个样例不是“自动流水 Tensor API”，而是“静态 Tensor 描述 + 手动流水同步”。Tensor API 主要简化数据视图和搬运/计算参数，事件依赖仍由样例显式控制。

## 12. 调用链总览

完整数据路径可以按下面理解：

```text
Host:
  读 input_x.bin/input_y.bin/quant.bin
  aclrtMalloc/aclrtMemcpy
  启动 32 个 cube block

Kernel 每个 block:
  1. 根据 blockIdx 得到当前 (M tile, N tile)
  2. MakeTensor 构造 GM/L1/L0 视图
  3. 对 i/j/k 做 tile 循环
  4. CopyGM2L1:
       A: GM ND -> L1 NZ
       B: GM DN -> L1 ZN
  5. CopyL12L0A / CopyL12L0B:
       L1 分形 tile -> L0A/L0B
  6. Mmad:
       L0C += L0A * L0B
  7. quant GM -> L1
  8. CopyL0C2GM:
       L0C int32 -> GM half, vector dequant, NZ -> ND
```

## 13. 对照基础 API 后的实现思路总结

| 阶段 | 基础 API 写法 | Tensor API 写法 |
| --- | --- | --- |
| GM Tensor | `GlobalTensor.SetGlobalBuffer` 或裸 GM 指针 + offset | `MakeMemPtr` + `MakeFrameLayout` + `MakeTensor` |
| 子块寻址 | 手写 `aGM[offset]`、`bGM[offset]` | `Slice(MakeCoord, MakeShape)` |
| GM->L1 | 手写 `DataCopyParams` / `Nd2NzParams` / stride | `CopyGM2L1` + 源/目的 layout 自动路由 |
| L1->L0 | 手写 `LoadData2DParams` / `LoadDataWithTranspose` | `CopyL12L0A/B` + layout pattern 判断 normal/transpose |
| Mmad | `AscendC::Mmad(c, a, b, MmadParams)` | `MmadAtom.with(MmadParams)` + Tensor location/type 检查 |
| L0C->GM | `FixpipeParamsV220` + `Fixpipe`，手写 `nSize/mSize/stride/quantPre` | `CopyL0C2GM` + `FixpipeParams(unitFlag)` + 类型/layout 推导 quant 和 stride |
| 同步 | `SetFlag/WaitFlag` 或 TPipe/TQue | 本样例仍手写 `SetFlag/WaitFlag` |

一句话总结：基础 API 是“用户把硬件指令参数填好”；Tensor API 是“用户把数据视图描述清楚，库从类型和 layout 里推导硬件指令参数”。这个样例仍然需要开发者理解 Cube 的 GM/L1/L0A/L0B/L0C/Fixpipe 流程，但矩阵分块、格式转换和量化写回的代码表达更接近 Tensor 视图。

## 参考资料

- Ascend C API 列表，说明 Kernel API 分层，以及 `DataCopy`、`Mmad`、`Fixpipe` 的基础职责：<https://www.hiascend.com/document/detail/zh/canncommercial/800/apiref/ascendcopapi>
- Fixpipe 官方接口说明，描述矩阵计算后对 CO1/L0C 结果进行量化处理并搬到 GM：<https://www.hiascend.com/document/detail/zh/canncommercial/83RC1/API/ascendcopapi/atlasascendc_api_07_0251.html>
- 本仓 Tensor API 总览文档：`docs/api/context/tensor_api_new/readme.md`
- 本仓普通矩阵计算 Tensor API 文档：`docs/api/context/tensor_api_new/matrix_computation/normal_matrix_computation.md`
- 本仓数据搬运 Tensor API 文档：`docs/api/context/tensor_api_new/data_movement/data_movement_overview.md`
