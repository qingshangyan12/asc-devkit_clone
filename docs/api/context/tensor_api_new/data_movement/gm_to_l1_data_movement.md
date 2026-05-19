# GM到L1数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

## 功能说明

该接口用于将 Global Memory 中的数据搬运到 L1 Buffer。Tensor API 通过 `Copy` 统一执行搬运，GM 到 L1 通路需要使用 `CopyGM2L1` 构造搬运原子对象，再将该对象传入 `Copy`。接口根据源张量和目的张量的存储位置、数据类型和 Layout 选择具体搬运实现。

GM 到 L1 搬运只在 AIC 侧执行。源张量位于 Global Memory，目的张量位于 L1 Buffer。搬运块数、搬运长度、源/目的侧步长以及 NZ 转换相关信息由 Tensor Layout 推导，用户不需要在 `Copy` 调用中额外传入搬运参数。

该通路用于矩阵计算输入搬入，支持以下场景：

- 格式保持搬入：源数据从 GM 搬入 L1 后数据格式不变。
- 高维切分搬入：源/目的侧存在步长时，根据 Layout 推导多段非连续搬运。
- 随路格式转换搬入：从 GM 搬入 L1 的同时完成 ND/DN 到 NZ/ZN 的矩阵排布转换。
- Scale 数据搬入：用于 MX 矩阵计算中 ScaleA/ScaleB 数据从 GM 搬入 L1。

## 函数原型

- 执行 GM 到 L1 搬运。

    ```cpp
    template <typename AtomType, typename DstTensor, typename SrcTensor>
    __aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy,
        const DstTensor& dst, const SrcTensor& src)
    ```

- 使用默认 trait 构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType>
    __aicore__ inline constexpr auto MakeCopy()
    ```

- 使用指定 trait 构造搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy()
    ```

GM 到 L1 通路使用 `CopyGM2L1` 作为 `CopyOperationType`，使用 `CopyGM2L1TraitDefault` 作为默认 trait 类型。

## 参数说明

**模板参数说明**

| 参数名 | 描述 |
| :--- | :--- |
| `AtomType` | `CopyAtom` 的模板参数，由 `MakeCopy<CopyGM2L1>()` 或 `MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>()` 推导得到。 |
| `DstTensor` | 目的张量类型。GM 到 L1 搬运时需为 Tensor API Tensor 类型，存储位置为 `Location::L1`。 |
| `SrcTensor` | 源张量类型。GM 到 L1 搬运时需为 Tensor API Tensor 类型，存储位置为 `Location::GM`。 |
| `CopyOperationType` | 搬运操作类型。GM 到 L1 搬运取 `CopyGM2L1`。 |
| `CopyTraitType` | 搬运 trait 类型。GM 到 L1 默认取 `CopyGM2L1TraitDefault`。 |

**函数参数说明**

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。GM 到 L1 搬运可通过 `MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>()` 构造。 |
| `dst` | 输出 | 目的张量，存储位置为 `Location::L1`。 |
| `src` | 输入 | 源张量，存储位置为 `Location::GM`。 |

## 返回值说明

`Copy` 无返回值。`MakeCopy` 返回 `CopyAtom` 对象。

## 搬运场景

### 格式保持搬入

格式保持搬入表示源张量和目的张量的数据格式保持一致，仅从 Global Memory 搬入 L1 Buffer。支持 `NDExtLayoutPtn/NDLayoutPtn` 到 `NDExtLayoutPtn/NDLayoutPtn`、`NZLayoutPtn` 到 `NZLayoutPtn`、`ZNLayoutPtn` 到 `ZNLayoutPtn`。

当 Layout 描述的源矩阵和目的矩阵均为连续空间时，`Copy` 会推导为单段连续搬运。

**图 1** 连续搬运示意图

![](../../figures/连续搬运示意图-53.png "连续搬运示意图")

当 Layout 描述的数据块之间存在步长时，`Copy` 会根据 Layout 推导为多段非连续搬运。

**图 2** 非连续搬运示意图

![](../../figures/非连续搬运示意图-55.png "非连续搬运示意图")

### 随路格式转换搬入

随路格式转换搬入表示从 Global Memory 搬入 L1 Buffer 时同步完成矩阵排布转换。接口根据源 Layout 和目的 Layout 自动选择转换方向，支持以下组合：

| 源 Layout | 目的 Layout | 说明 |
| :--- | :--- | :--- |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NZLayoutPtn` | ND 格式搬入并转换为 NZ 格式。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `ZNLayoutPtn` | ND 格式搬入并转换为 ZN 格式。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `NZLayoutPtn` | DN 格式搬入并转换为 NZ 格式。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `ZNLayoutPtn` | DN 格式搬入并转换为 ZN 格式。 |

ND 到 NZ 的格式转换等价于 DN 到 ZN 的格式转换，DN 到 NZ 的格式转换等价于 ND 到 ZN 的格式转换。

**图 3** ND2NZ 与 DN2ZN 示意图

![](../../figures/ND2NZ与DN2ZN示意图.png "ND2NZ与DN2ZN示意图")

**图 4** ND2NZ 转换示意图（连续搬运）

![](../../figures/ND2NZ转换示意图（连续搬运）.png "ND2NZ转换示意图（连续搬运）")

**图 5** DN2NZ 转换示意图（连续搬运）

![](../../figures/DN2NZ转换示意图（连续搬运）.png "DN2NZ转换示意图（连续搬运）")

随路格式转换支持连续源矩阵，也支持源侧或目的侧存在步长的高维切分场景。搬运过程中，当矩阵尾部不足硬件对齐粒度时，目的侧会按转换语义补 0。

### Scale 数据搬入

Scale 数据搬入用于矩阵计算中 scale 相关数据从 Global Memory 搬入 L1 Buffer。A 矩阵相关 scale 支持 `ScaleANDLayoutPtn` 到 `ZZLayoutPtn`、`ScaleADNLayoutPtn` 到 `ZZLayoutPtn`、`ZZLayoutPtn` 到 `ZZLayoutPtn`；B 矩阵相关 scale 支持 `ScaleBNDLayoutPtn` 到 `NNLayoutPtn`、`ScaleBDNLayoutPtn` 到 `NNLayoutPtn`、`NNLayoutPtn` 到 `NNLayoutPtn`。

## Layout 支持情况

| 源 Layout | 目的 Layout | 搬运场景 |
| :--- | :--- | :--- |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NDExtLayoutPtn`、`NDLayoutPtn` | 格式保持搬入。 |
| `NZLayoutPtn` | `NZLayoutPtn` | NZ 格式保持搬入。 |
| `ZNLayoutPtn` | `ZNLayoutPtn` | ZN 格式保持搬入。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `NZLayoutPtn` | ND 到 NZ 随路格式转换。 |
| `NDExtLayoutPtn`、`NDLayoutPtn` | `ZNLayoutPtn` | ND 到 ZN 随路格式转换。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `NZLayoutPtn` | DN 到 NZ 随路格式转换。 |
| `DNExtLayoutPtn`、`DNLayoutPtn` | `ZNLayoutPtn` | DN 到 ZN 随路格式转换。 |
| `ScaleANDLayoutPtn` | `ZZLayoutPtn` | ScaleA 数据随路转换搬入。 |
| `ScaleADNLayoutPtn` | `ZZLayoutPtn` | ScaleA 数据随路转换搬入。 |
| `ZZLayoutPtn` | `ZZLayoutPtn` | ScaleA 数据格式保持搬入。 |
| `ScaleBNDLayoutPtn` | `NNLayoutPtn` | ScaleB 数据随路转换搬入。 |
| `ScaleBDNLayoutPtn` | `NNLayoutPtn` | ScaleB 数据随路转换搬入。 |
| `NNLayoutPtn` | `NNLayoutPtn` | ScaleB 数据格式保持搬入。 |

## 数据类型

源张量和目的张量的数据类型需要保持一致。

- 常规 GM 到 L1 搬运支持 `half`、`bfloat16_t`、`float`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`。
- 格式保持的 align 搬运路径还支持 `int64_t`、`uint64_t`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`。
- 随路格式转换路径中，`NDExtLayoutPtn/NDLayoutPtn` 到 `NZLayoutPtn`、`DNExtLayoutPtn/DNLayoutPtn` 到 `ZNLayoutPtn` 支持 `fp4x2_e1m2_t`、`fp4x2_e2m1_t`。
- Scale 数据搬入支持 `fp8_e8m0_t`。

## API 映射关系

`MakeCopy<CopyGM2L1>()` 或 `MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>()` 返回 GM 到 L1 的搬运原子对象。调用 `Copy(atomCopy, dst, src)` 或 `atomCopy.Call(dst, src)` 时，`CopyGM2L1` 根据 `dst` 和 `src` 的 Layout Pattern 选择具体搬运实现。

NPU 架构版本 3510 下，底层 API 映射关系如下。该章节用于说明 Tensor API 的实现路径，不表示用户需要直接调用这些接口。

格式保持搬入映射到 `asc_copy_gm2l1_align`，对应 `NDExtLayoutPtn/NDLayoutPtn` 到 `NDExtLayoutPtn/NDLayoutPtn`、`NZLayoutPtn` 到 `NZLayoutPtn`、`ZNLayoutPtn` 到 `ZNLayoutPtn` 等 Layout 组合。

```cpp
void asc_copy_gm2l1_align(__cbuf__ T* dst, __gm__ T* src, uint32_t blockCount,
    uint32_t blockLen, uint8_t leftPaddingCnt, uint8_t rightPaddingCnt,
    bool dataSelectBit, uint8_t cacheMode, uint64_t srcStride, uint32_t dstStride);
```

ND2NZ 类随路转换搬入映射到 `asc_copy_gm2l1_nd2nz`，对应 `NDExtLayoutPtn/NDLayoutPtn` 到 `NZLayoutPtn` 等 Layout 组合；与 ND2NZ 等价的转换场景会由实现选择对应转换路径。

```cpp
void asc_copy_gm2l1_nd2nz(__cbuf__ T* dst, __gm__ T* src,
    uint64_t loop1SrcStride, uint8_t cacheMode, uint16_t nValue,
    uint32_t dValue, uint64_t loop4SrcStride, bool enableSmallC0);
```

DN2NZ 类随路转换搬入映射到 `asc_copy_gm2l1_dn2nz`，对应 `DNExtLayoutPtn/DNLayoutPtn` 到 `NZLayoutPtn` 等 Layout 组合；与 DN2NZ 等价的转换场景会由实现选择对应转换路径。

```cpp
void asc_copy_gm2l1_dn2nz(__cbuf__ T* dst, __gm__ T* src,
    uint64_t loop1SrcStride, uint8_t cacheMode, uint16_t nValue,
    uint32_t dValue, uint64_t loop4SrcStride, bool enableSmallC0);
```

ND2NZ/DN2NZ 类随路转换搬入会通过 `asc_set_gm2l1_nz_para` 配置目的侧 NZ 相关步长参数。

```cpp
void asc_set_gm2l1_nz_para(uint64_t para);
```

Scale 数据搬入根据 Layout 组合复用上述底层 API 映射：`ZZLayoutPtn` 到 `ZZLayoutPtn`、`NNLayoutPtn` 到 `NNLayoutPtn` 等格式保持场景映射到 `asc_copy_gm2l1_align`；`ScaleANDLayoutPtn`、`ScaleADNLayoutPtn`、`ScaleBNDLayoutPtn`、`ScaleBDNLayoutPtn` 相关转换场景映射到 `asc_copy_gm2l1_nd2nz` 或 `asc_copy_gm2l1_dn2nz`。

`Copy` 对外不暴露上述映射接口的参数，搬运块数、搬运长度、stride、`nValue`、`dValue` 等信息均由 `src/dst` Tensor 的 Layout 自动推导。

| 场景 | Tensor API 实现路径 | 底层 API 映射 |
| :--- | :--- | :--- |
| 格式保持搬入 | `CopyGmToCbufAlignV2ND`、`CopyGmToCbufAlignV2NZ`、`CopyGmToCbufAlignV2ZN`、`CopyGmToCbufScaleAZz2Zz`、`CopyGmToCbufScaleBNn2Nn` | `asc_copy_gm2l1_align` |
| ND/DN 到 NZ/ZN 随路转换 | `CopyGmToCbufMultiND2Nz`、`CopyGmToCbufMultiND2Zn`、`CopyGmToCbufMultiDN2Nz`、`CopyGmToCbufMultiDN2Zn` | `asc_set_gm2l1_nz_para` + `asc_copy_gm2l1_nd2nz` 或 `asc_copy_gm2l1_dn2nz` |
| Scale 数据搬入 | `CopyGmToCbufScaleAND2Zz`、`CopyGmToCbufScaleADN2Zz`、`CopyGmToCbufScaleBND2Nn`、`CopyGmToCbufScaleBDN2Nn` | `asc_set_gm2l1_nz_para` + `asc_copy_gm2l1_nd2nz` 或 `asc_copy_gm2l1_dn2nz` |

## 约束说明

**Tensor 与 Layout 约束**

- `dst` 和 `src` 必须为 Tensor API Tensor 类型。
- 目的张量必须位于 L1 Buffer，源张量必须位于 Global Memory。
- 支持的 Layout 组合需要满足 Layout 支持情况中的范围，不支持的组合会在编译期进入不支持路径。
- 源张量和目的张量的数据类型需要保持一致。
- GM 到 L1 `Copy` 不直接暴露搬运参数，相关参数由 `src/dst` Tensor 的 Layout 自动推导。Layout 推导出的参数仍需满足搬运指令约束。

**地址与对齐约束**

- 位于 Global Memory 的源地址必须 1B 对齐，位于 L1 Buffer 的目的地址必须 32B 对齐。
- 格式保持的连续搬运场景中，有效搬运字节数需要 32B 对齐。若未对齐，搬运量会按硬件语义向下取整到 32B 对齐。
- 格式保持的高维切分搬运场景中，搬运长度、源侧步长和目的侧步长均按 32B DataBlock 推导。使用时应保证 Layout 描述的行长和步长能推导出满足搬运约束的参数。
- ND2NZ/DN2NZ 随路转换场景中，当 `dValue * sizeof(T)` 不满足 32B 对齐时，目的矩阵中会按转换搬运语义补 0 对齐。
- 启用 Small C0 语义的随路转换场景中，当 `dValue` 小于或等于 4 时，目的侧 C0 方向按 4 个元素对应的存储粒度补 0。
- b4/fp4 数据随路转换时按 b8 搬运粒度处理，Layout 推导参数需要满足对应粒度约束；DN2NZ 类转换中 N 方向有效元素数需要满足偶数约束。

**格式保持搬运参数约束**

格式保持搬入映射到 `asc_copy_gm2l1_align`。Tensor API 不暴露以下参数，但 Layout 推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `blockCount` | 搬运数据块个数 | `[0, 4095]` |
| `blockLen` | 单个数据块长度，单位为 32B DataBlock | `[0, 65535]` |
| `srcGap`/`srcStride` | 源侧相邻数据块间隔，单位为 32B DataBlock | `[0, 65535]` |
| `dstGap`/`dstStride` | 目的侧相邻数据块间隔，单位为 32B DataBlock | `[0, 65535]` |

当 `blockCount` 或 `blockLen` 任一值为 0 时，该次搬运视为空操作。

**ND2NZ 随路转换参数约束**

ND2NZ 类随路转换映射到 `asc_set_gm2l1_nz_para` 和 `asc_copy_gm2l1_nd2nz`。Tensor API 不暴露以下参数，但 Layout 推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `ndNum` | 搬运的 ND 矩阵个数 | `[0, 4095]` |
| `nValue` | 单个 ND 矩阵的行数 | `[0, 16384]` |
| `dValue` | 单个 ND 矩阵的列数 | `[0, 65535]` |
| `srcNdMatrixStride` | 相邻 ND 矩阵起始地址偏移 | `[0, 65535]` |
| `srcDValue` | 源矩阵一行包含的元素个数 | `[1, 65535]` |
| `dstNzC0Stride` | NZ 矩阵中相邻 Z 矩阵起始地址偏移 | `[1, 16384]` |
| `dstNzNStride` | NZ 矩阵中相邻行起始地址偏移 | `[1, 16384]` |
| `dstNzMatrixStride` | 相邻 NZ 矩阵起始地址偏移 | `[0, 65535]` |

当 `ndNum`、`nValue` 或 `dValue` 任一值为 0 时，该次搬运视为空操作。

**DN2NZ 随路转换参数约束**

DN2NZ 类随路转换映射到 `asc_set_gm2l1_nz_para` 和 `asc_copy_gm2l1_dn2nz`。Tensor API 不暴露以下参数，但 Layout 推导出的内部参数需满足对应范围。

| 内部参数 | 含义 | 取值范围 |
| :--- | :--- | :--- |
| `dnNum` | 搬运的 DN 矩阵个数 | `[0, 4095]` |
| `nValue` | 单个 DN 矩阵的 N 方向元素个数 | `[0, 16384]` |
| `dValue` | 单个 DN 矩阵的 D 方向元素个数 | `[0, 2^21 - 1]` |
| `srcDnMatrixStride` | 相邻 DN 矩阵起始地址偏移 | `[0, 2^40 - 1]` |
| `srcDValue` | 源矩阵一行包含的元素个数 | `[1, 2^40 - 1]` |
| `dstNzC0Stride` | NZ 矩阵中相邻 Z 矩阵起始地址偏移 | `[1, 16384]` |
| `dstNzNStride` | NZ 矩阵中相邻行起始地址偏移 | `[1, 16384]` |
| `dstNzMatrixStride` | 相邻 NZ 矩阵起始地址偏移 | `[0, 65535]` |

当 `dnNum`、`nValue` 或 `dValue` 任一值为 0 时，该次搬运视为空操作。

**目的地址重叠约束**

- ND2NZ/DN2NZ 随路转换搬入到 L1 Buffer 的目的数据区域不能重叠。如果存在重叠写入，不保证重叠数据的写入顺序。
- 格式保持搬入未额外声明支持目的地址重叠，使用时应避免由 Layout 推导出的目的数据区域发生非预期重叠。

**Cache 控制约束**

- 源 GM 张量可以通过 `SetL2CacheHint` 设置 L2 Cache 控制模式，GM 到 L1 路径会读取该模式作为搬运的 cache 控制参数。
- L2 Cache 控制模式为 `CacheMode::CACHE_MODE_DISABLE` 和 `CacheMode::CACHE_MODE_NORMAL`。

## 调用示例

以下示例将 GM 中的 ND 矩阵搬入 L1 Buffer，并转换为 NZ 布局。

```cpp
#include "tensor_api/tensor.h"

using namespace AscendC::Te;

constexpr uint32_t M = 32;
constexpr uint32_t K = 64;

__aicore__ inline void CopyGmToL1(__gm__ half* gmAddr)
{
    __cbuf__ half l1Buf[M * K];

    auto gmA = MakeTensor(MakeMemPtr(gmAddr),
        MakeFrameLayout<NDExtLayoutPtn>(M, K));
    auto l1A = MakeTensor(MakeMemPtr(l1Buf),
        MakeFrameLayout<NZLayoutPtn>(M, K));

    gmA.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);

    auto copyGm2L1 = MakeCopy<CopyGM2L1, CopyGM2L1TraitDefault>();
    Copy(copyGm2L1, l1A, gmA);
}
```