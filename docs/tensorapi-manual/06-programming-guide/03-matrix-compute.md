# 6.3 矩阵计算实战

本章展示基于 TensorAPI 完成一个带 Bias 的矩阵乘加运算端到端实现。

## 6.3.1 矩阵乘加的标准流程

```
     GM 输入
       │
  CopyGM2L1                   ← 数据搬入
       │
       ▼
      L1 (ND / NZ)
       │
  CopyL12L0A / CopyL12L0B     ← A、B 搬入计算单元
       │
       ▼
  L0A (NZ) + L0B (ZN)
       │
     MMAD                      ← 矩阵乘加
       │
       ▼
      L0C (ZZ)
       │
  CopyL0C2GM (Fixpipe)        ← 结果写回
       │
       ▼
     GM 输出
```

## 6.3.2 完整示例：C(m×n) = A(m×k) × B(k×n) + Bias(1×n)

```cpp
__global__ void matmul_kernel(
    __gm__ half* gmA,      // A 矩阵 (m × k)
    __gm__ half* gmB,      // B 矩阵 (k × n)
    __gm__ half* gmBias,   // Bias (1 × n)
    __gm__ half* gmC,      // C 矩阵 (m × n)
    int m, int n, int k
) {
    // ==========================================
    // 阶段 1: 构造 GM Tensors
    // ==========================================

    // GM 上的数据使用 ND (行优先) 格式
    auto gmATensor = MakeTensor(
        MakeMemPtr<Location::GM>(gmA),
        MakeFrameLayout<NDLayoutPtn>(m, k));

    auto gmBTensor = MakeTensor(
        MakeMemPtr<Location::GM>(gmB),
        MakeFrameLayout<NDLayoutPtn>(k, n));

    auto gmBiasTensor = MakeTensor(
        MakeMemPtr<Location::GM>(gmBias),
        MakeFrameLayout<NDLayoutPtn>(1, n));

    auto gmCTensor = MakeTensor(
        MakeMemPtr<Location::GM>(gmC),
        MakeFrameLayout<NDLayoutPtn>(m, n));

    // 缓存策略
    gmATensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
    gmBTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
    gmBiasTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
    gmCTensor.SetL2CacheHint(CacheMode::CACHE_MODE_LAST);

    // ==========================================
    // 阶段 2: 计算对齐后的尺寸
    // ==========================================
    // 分形块要求 M 对齐到 16, K 和 N 对齐到 C0 (half: C0=16)
    constexpr int BLOCK_SIZE = 16;
    constexpr int C0 = 16;  // half 类型

    int mAlign  = Std::ceil_align(m, BLOCK_SIZE);
    int kAlign  = Std::ceil_align(k, C0);
    int nAlign  = Std::ceil_align(n, C0);

    // ==========================================
    // 阶段 3: 构造 L1 Tensors (分形格式)
    // ==========================================

    // L1 上使用 NZ 格式 (为后续 L0A/L0B 做准备)
    __cbuf__ half* l1BufA = /* 分配 */;
    __cbuf__ half* l1BufB = /* 分配 */;
    __cbuf__ half* l1BufBias = /* 分配 */;

    auto l1ATensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1BufA),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(mAlign, kAlign));

    auto l1BTensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1BufB),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(kAlign, nAlign));

    auto l1BiasTensor = MakeTensor(
        MakeMemPtr<Location::L1>(l1BufBias),
        MakeFrameLayout<NDLayoutPtn>(1, nAlign));  // Bias 用 ND

    // ==========================================
    // 阶段 4: 数据搬入 GM → L1
    // ==========================================
    auto gm2l1Atom = MakeCopy(CopyGM2L1{});
    Copy(gm2l1Atom, l1ATensor, gmATensor);
    Copy(gm2l1Atom, l1BTensor, gmBTensor);
    Copy(gm2l1Atom, l1BiasTensor, gmBiasTensor);

    // ==========================================
    // 阶段 5: L1 → L0A / L0B / BIAS
    // ==========================================

    __ca__ half* l0aBuf = /* 分配 */;
    __cb__ half* l0bBuf = /* 分配 */;
    __biasbuf__ half* biasBuf = /* 分配 */;

    auto l0aTensor = MakeTensor(
        MakeMemPtr<Location::L0A>(l0aBuf),
        MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(mAlign, kAlign));

    auto l0bTensor = MakeTensor(
        MakeMemPtr<Location::L0B>(l0bBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(kAlign, nAlign));

    auto biasTensor = MakeTensor(
        MakeMemPtr<Location::BIAS>(biasBuf),
        MakeFrameLayout<NDLayoutPtn>(1, nAlign));

    auto l12l0a = MakeCopy(CopyL12L0A{});
    auto l12l0b = MakeCopy(CopyL12L0B{});
    auto l12bt  = MakeCopy(CopyL12BT{});

    Copy(l12l0a, l0aTensor,  l1ATensor);
    Copy(l12l0b, l0bTensor,  l1BTensor);
    Copy(l12bt,  biasTensor, l1BiasTensor);

    // ==========================================
    // 阶段 6: MMAD 计算
    // ==========================================

    __cc__ half* l0cBuf = /* 分配 */;

    auto l0cTensor = MakeTensor(
        MakeMemPtr<Location::L0C>(l0cBuf),
        MakeFrameLayout<ZZLayoutPtn, LayoutTraitDefault<half>>(mAlign, nAlign));

    auto mmad = MakeMmad(MmadOperation{});
    Mmad(mmad, l0aTensor, l0bTensor, l0cTensor, biasTensor);
    // C = A × B + Bias

    // ==========================================
    // 阶段 7: Fixpipe 写回 GM
    // ==========================================
    auto fixAtom = MakeCopy(CopyL0C2GM{});
    Copy(fixAtom, gmCTensor, l0cTensor);
    // 注意：gmCTensor 的 Layout 为 ND (m×n)，
    // 与 L0C 的 ZZ 格式通过 Fixpipe 自动转换
}
```

## 6.3.3 分块 (Tiling) 处理

当矩阵维度超过单次可处理范围时，需要进行分块（Tiling）。TensorAPI 的 `operator()` 切片操作天然支持分块编程：

```cpp
// 假设单次 MMAD 最多处理 16×16 的子块
constexpr int TILE_M = 16;
constexpr int TILE_N = 16;
constexpr int TILE_K = 16;

for (int i = 0; i < m; i += TILE_M) {
    for (int j = 0; j < n; j += TILE_N) {
        // 累加器归零
        // ...

        for (int kk = 0; kk < k; kk += TILE_K) {
            // 切片：取得当前子块的 Tensor 视图
            auto subA = l1ATensor(
                MakeCoord(i, kk),
                MakeShape(TILE_M, TILE_K));   // 或使用 Slice()

            auto subB = l1BTensor(
                MakeCoord(kk, j),
                MakeShape(TILE_K, TILE_N));

            // 子块 MMAD（累加到 L0C）
            Mmad(mmad, subA, subB, l0cTensor);
        }

        // 子块结果写回
        // ...
    }
}
```

## 6.3.4 仅矩阵乘（无 Bias）

如果不需要 Bias，直接省略 Bias 参数即可：

```cpp
Mmad(mmad, l0aTensor, l0bTensor, l0cTensor);
// C = A × B （不用 Bias）
```

## 6.3.5 注意事项

1. **对齐到块边界**：M 对齐到 16，K 和 N 对齐到 C0（half=16, float=8）。不对齐的数据需要 Padding 或特殊处理。

2. **Layout Pattern 必须一致**：
   - A 矩阵：L1 到 L0A 两者的 Layout Pattern 通常一致（均为 NZ）
   - B 矩阵：L1 → L0B 使用 ZN
   - C 矩阵（L0C）：ZZ

3. **数据类型兼容**：MMAD 的输入类型（A, B）和输出类型（C）应兼容。若需要混合精度（如 fp16 输入、fp32 累加），TraitType 需做相应调整。

4. **L0C 写回后的数据布局**：Fixpipe (CopyL0C2GM) 会自动将 ZZ 格式的 L0C 数据转换为 ND 格式写入 GM。
