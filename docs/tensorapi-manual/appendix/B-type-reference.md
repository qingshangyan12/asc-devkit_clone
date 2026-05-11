# B. 类型速查表

## B.1 核心类型

| 类型 | 定义 | 说明 |
|------|------|------|
| `Shape<Ts...>` | `Std::tuple<Ts...>` | 形状元组 |
| `Stride<Ts...>` | `Std::tuple<Ts...>` | 步长元组 |
| `Coord<Ts...>` | `Std::tuple<Ts...>` | 坐标元组 |
| `Tile<Ts...>` | `Std::tuple<Ts...>` | 分块元组 |
| `Layout<T, U, Info>` | — | 布局描述 (Shape + Stride + Pattern) |
| `LocalTensor<TensorAttribute<E,L>>` | — | 带 Layout 的 Tensor |
| `ViewEngine<Iterator>` | — | 内存视图引擎 |
| `ConstViewEngine<Iterator>` | — | 只读视图引擎 |
| `HardwareMemPtr<P, Ptr>` | `IterAdaptor<Ptr, ...>` | 硬件内存指针 |
| `CopyAtom<CopyTraits<...>>` | — | Copy 原子操作 |
| `MmadAtom<MmadTraits<...>>` | — | MMAD 原子操作 |
| `CopyTraits<Op, Traits, OpWith, TraitsWith>` | — | Copy 配置容器 |
| `MmadTraits<Op, Traits, OpWith, TraitsWith>` | — | MMAD 配置容器 |

## B.2 Layout Pattern 类型

| 类型 | C0 元素数 | 用途 |
|------|-----------|------|
| `NZLayoutPtn` | 32/sizeof(T) | A 矩阵分形格式 |
| `ZNLayoutPtn` | 32/sizeof(T) | B 矩阵分形格式 |
| `ZZLayoutPtn` | 32/sizeof(T) | C 矩阵 / 通用分形 |
| `NDLayoutPtn` | — | 行优先平坦 |
| `DNLayoutPtn` | — | 列优先平坦 |
| `NDExtLayoutPtn` | — | 扩展 ND |
| `DNExtLayoutPtn` | — | 扩展 DN |
| `NNLayoutPtn` | 2 | fp8 Scale NN |
| `ScaleANDLayoutPtn` | 2 | fp8 ScaleA ND |
| `ScaleADNLayoutPtn` | 2 | fp8 ScaleA DN |
| `ScaleBNDLayoutPtn` | 2 | fp8 ScaleB ND |
| `ScaleBDNLayoutPtn` | — | fp8 ScaleB DN |
| `DefaultPtn` | — | 默认 |

## B.3 LayoutTrait 类型

| 类型 | 默认 T | 默认 C0 | 用途 |
|------|--------|---------|------|
| `LayoutTraitDefault<T, C0>` | `uint16_t` | `32/sizeof(T)` | 标准数据类型 |
| `LayoutTraitScale<T, C0>` | `fp8_e8m0_t` | `2/sizeof(T)` | fp8 Scale |
| `LayoutTraitFP4<T, C0>` | `fp4x2_e2m1_t` | `64/sizeof(T)` | fp4 类型 |

## B.4 Location 类型 (内存层次)

| Location | 指针类型 | 存储层次 |
|----------|----------|----------|
| `Location::GM` | `__gm__ T*` | Global Memory |
| `Location::L1` | `__cbuf__ T*` | L1 Buffer |
| `Location::UB` | `__ubuf__ T*` | Unified Buffer |
| `Location::L0A` | `__ca__ T*` | L0A Buffer |
| `Location::L0B` | `__cb__ T*` | L0B Buffer |
| `Location::L0C` | `__cc__ T*` | L0C Buffer |
| `Location::BIAS` | `__biasbuf__ T*` | BIAS Buffer |
| `Location::FIXBUF` | `__fbuf__ T*` | Fix Buffer |
| `Location::SSBUF` | `__ssbuf__ T*` | Shared Scratch |
| `Location::INVALID` | — | 无效位置 |

## B.5 Copy 操作标签

| 类型 | 数据路径 |
|------|----------|
| `CopyGM2L1` | GM → L1 |
| `CopyL12UB` | L1 → UB |
| `CopyL12L0A` | L1 → L0A |
| `CopyL12L0B` | L1 → L0B |
| `CopyL12BT` | L1 → BIAS |
| `CopyL12FB` | L1 → FIXBUF |
| `CopyL0C2GM` | L0C → GM |
| `CopyL0C2GMWith` | L0C → GM (带参数) |
| `CopyL0C2UB` | L0C → UB |
| `CopyL0C2UBWith` | L0C → UB (带参数) |
| `CopyGM2UB` | GM → UB |
| `CopyUB2GM` | UB → GM |
| `CopyUB2L1` | UB → L1 |

## B.6 CacheMode 枚举

| 值 | 含义 |
|----|------|
| `CACHE_MODE_NORMAL` | 正常缓存 |
| `CACHE_MODE_DISABLE` | 禁用缓存 |
| `CACHE_MODE_LAST` | 仅最后使用缓存 |
| `CACHE_MODE_PERSISTENT` | 持久缓存 |

## B.7 关键 Trait / Type Check

| 类型/Trait | 用途 |
|------------|------|
| `IsLayout<T>` / `IsLayoutV<T>` | 判断是否为 Layout |
| `IsAttrTensor<T>` / `IsAttrTensorV<T>` | 判断是否为 LocalTensor |
| `GetLayoutPattern<T>` | 提取 Layout 的 Pattern |
| `GetLayoutTrait<T>` | 提取 Layout 的 TraitType |
| `NestingDepthV<T>` | 元组嵌套深度 |
| `IsIntegralConstant<T>` / `IsIntegralConstantV<T>` | 判断是否为 `Std::Int<N>` |
| `IsFrameLayoutTrait<T>` / `IsFrameLayoutTraitV<T>` | 判断是否定义 C0_ELEMENT |

## B.8 支持的数据类型

| 类型 | 说明 |
|------|------|
| `half` | 半精度浮点 (fp16) |
| `float` | 单精度浮点 (fp32) |
| `bfloat16_t` | Brain Float 16 |
| `hifloat8_t` | 高精度浮点 8 |
| `fp8_e5m2_t` | fp8 E5M2 格式 |
| `fp8_e4m3fn_t` | fp8 E4M3 格式 |
| `fp8_e8m0_t` | fp8 Scale 格式 (E8M0) |
| `fp4x2_e1m2_t` | fp4x2 E1M2 格式 |
| `fp4x2_e2m1_t` | fp4x2 E2M1 格式 |
| `int8_t` / `uint8_t` | 8 位整数 |
| `int16_t` / `uint16_t` | 16 位整数 |
| `int32_t` / `uint32_t` | 32 位整数 |
| `int64_t` / `uint64_t` | 64 位整数 |

## B.9 常量速查

| 常量 | 值 | 说明 |
|------|-----|------|
| `FRACTAL_FIXED` | 16 | 标准分形块边长 |
| `BLOCK_CUBE` | 16 | Cube 块大小 |
| `C0_SIZE` | 32 (fp4: 64) | C0 字节宽度 |
| `TWO_DIM_DATA` | 2 | 平坦 2D 标记 |
| `FOUR_DIM_DATA` | 4 | 分形 4D 标记 |
| `MX_SCALE_K0` | 2 | MX 格式 Scale K0 |
