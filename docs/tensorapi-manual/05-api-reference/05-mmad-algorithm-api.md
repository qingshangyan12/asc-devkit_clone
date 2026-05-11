# 5.5 MMAD 算法 API

MMAD (Matrix Multiply-Add) 是 NPU Cube 计算单元的核心操作，用于执行矩阵乘加运算：**C = A × B + Bias**。TensorAPI 通过 **MmadAtom + Mmad()** 的模型来组织和执行矩阵计算。

## 5.5.1 核心类型

### MmadOperation

Mmad 操作的基本标签类型，声明一个标准的矩阵乘加操作。

### MmadTraits

```cpp
template <typename MmadOperation, typename... MmadOpArgs>
struct MmadTraits {};
```

MMAD 操作的配置容器。4 参数全特化形式为：

```cpp
template <typename MmadOp, typename MmadTraitsType, typename MmadOpWith, typename MmadTraitsWith>
struct MmadTraits<MmadOp, MmadTraitsType, MmadOpWith, MmadTraitsWith> {
    using TraitType = typename MmadTraitsType::TraitType;
    static constexpr const TraitType defaultTrait = MmadTraitsType::value;
};
```

### MmadAtom

```cpp
template <typename... Args>
struct MmadAtom;

template <typename MmadOperation>
struct MmadAtom<MmadOperation> : MmadAtom<MmadTraits<MmadOperation>> {};

template <typename MmadOperation, typename... Args>
struct MmadAtom<MmadTraits<MmadOperation, Args...>> : MmadTraits<MmadOperation, Args...> {
    template <const TraitType& traits = defaultTrait, typename... Params>
    void Call(const Params& ...params) const;

    template <typename... TraitsArgs>
    auto with(TraitsArgs&&... args) const;
};
```

### MmadOpWith

```cpp
struct MmadOpWith : public MmadOperation {};
```

`MmadOpWith` 继承自 `MmadOperation`，用于携带运行时参数（如 `MmadParams`）。它与 `MmadParams` 配合使用时，可以将 M/N/K 等维度参数在运行时传递给 MMAD 操作。

## 5.5.2 Mmad 函数

### Mmad()

```cpp
// 带显式 traits
template <typename Tp, const Tp& traits, typename T, typename... Params>
void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);

// 使用默认 traits
template <typename T, typename... Params>
void Mmad(const MmadAtom<T>& atomMmad, const Params& ...params);
```

执行矩阵乘加运算。参数一般为 `(aTensor, bTensor, cTensor)` 或 `(aTensor, bTensor, cTensor, biasTensor)`。

### MakeMmad()

```cpp
template <typename... Args>
auto MakeMmad(const Args& ...traits);
```

创建 MmadAtom 的工厂函数。

## 5.5.3 MmadTrait 配置结构

### MmadTrait

```cpp
struct MmadTrait {
    // 字段（示例）：
    int fmOffset;            // FM 偏移
    bool kDirectionAlign;    // K 方向对齐
    int cmatrixSource;       // C 矩阵源
    bool disableGemv;        // 禁用 GEMV
    int mmadType;            // MMAD 类型
};
```

| 字段 | 说明 |
|------|------|
| `fmOffset` | 频率调制偏移 |
| `kDirectionAlign` | 是否启用 K 方向对齐优化 |
| `cmatrixSource` | C 矩阵初始化来源 |
| `disableGemv` | 禁用 GEMV（矩阵-向量乘）特殊路径 |
| `mmadType` | MMAD 类型选择 |

### MmadTraitDefault

默认的 Mmad Traits，提供合理的缺省配置：

```cpp
struct MmadTraitDefault {
    using TraitType = MmadTrait;
    static constexpr MmadTrait value = {/* 默认值 */};
};
```

### MmadType 枚举

| 值 | 说明 |
|----|------|
| `MmadType::NORMAL` | 标准 MMAD |
| `MmadType::MX` | MX 格式 MMAD（用于 fp8 等精度格式） |

### MmadParams

携带运行时参数的配置结构，配合 `MmadOpWith` 使用：

```cpp
struct MmadParams {
    int m;                 // M 维度大小
    int n;                 // N 维度大小
    int k;                 // K 维度大小
    int unitFlag;          // 单元标志
    int cmatrixInitVal;    // C 矩阵初始值
};
```

| 字段 | 说明 |
|------|------|
| `m` | 矩阵 A 的行数 |
| `n` | 矩阵 B 的列数 |
| `k` | 矩阵 A 的列数 / 矩阵 B 的行数 |
| `unitFlag` | 硬件单元选择标志 |
| `cmatrixInitVal` | C 矩阵累加器的初始值 |

## 5.5.4 Buffer 布局要求

Mmad 操作对各 buffer 的 Layout Pattern 有明确要求：

| Buffer | 推荐 Layout Pattern | 说明 |
|--------|---------------------|------|
| L0A (A 矩阵) | `NZLayoutPtn` | 16×C0 分形块 |
| L0B (B 矩阵) | `ZNLayoutPtn` | C0×16 分形块 |
| L0C (C 矩阵) | `ZZLayoutPtn` | 分形累加器格式 |
| BIAS | `NDLayoutPtn` | Bias 数据，平坦格式 |
| FIXBUF | `NDLayoutPtn` | Fixpipe 输入 |

## 5.5.5 使用模式

### 模式 1：基本 MMAD

```cpp
auto atom = MakeMmad(MmadOperation{});
Mmad(atom, aTensor, bTensor, cTensor);
// C = A × B
```

### 模式 2：带 Bias 的 MMAD

```cpp
auto atom = MakeMmad(MmadOperation{});
Mmad(atom, aTensor, bTensor, cTensor, biasTensor);
// C = A × B + Bias
```

### 模式 3：带运行时参数 (MmadParams)

```cpp
auto atom = MakeMmad(MmadOperation{}, MmadParams{m, n, k, unitFlag, initVal});
Mmad(atom, aTensor, bTensor, cTensor);
```

### 模式 4：with() 链式配置

```cpp
auto baseAtom = MakeMmad(MmadOperation{});
auto configured = baseAtom.with(MmadTrait{/* 自定义 */});
Mmad(configured, aTensor, bTensor, cTensor);
```

### 模式 5：带显式 traits 模板参数

```cpp
static constexpr MmadTrait myTraits = {
    .kDirectionAlign = true,
    .disableGemv = false,
};
Mmad<myTraits>(atom, aTensor, bTensor, cTensor);
```

## 5.5.6 完整 MMAD 流程示例

```cpp
// 前置条件：A 和 B 矩阵已从 L1 搬入 L0A 和 L0B
// L0A: NZ 格式, half 类型, 16×32 矩阵
// L0B: ZN 格式, half 类型, 32×64 矩阵

auto l0aTensor = MakeTensor(l0aPtr, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(16, 32));
auto l0bTensor = MakeTensor(l0bPtr, MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(32, 64));
auto l0cTensor = MakeTensor(l0cPtr, MakeFrameLayout<ZZLayoutPtn, LayoutTraitDefault<half>>(16, 64));

auto mmad = MakeMmad(MmadOperation{});
Mmad(mmad, l0aTensor, l0bTensor, l0cTensor);
// 结果: C(16×64) = A(16×32) × B(32×64)

// 可选：将 L0C 结果通过 Fixpipe 写回 GM
auto fixAtom = MakeCopy(CopyL0C2GM{});
Copy(fixAtom, gmOutTensor, l0cTensor);
```

## 5.5.7 注意事项

1. **M/K/N 必须对齐到块大小**：M 通常对齐到 16，K 和 N 对齐到 C0（如 half 为 16）。使用 `Std::ceil_align()` 可实现正确的对齐。
2. **Layout Pattern 必须匹配**：A 矩阵用 NZ，B 矩阵用 ZN，C 矩阵用 ZZ——这是 MMAD 硬件的要求。不匹配的组合将在编译期或运行期报错。
3. **数据类型需一致**：A、B、C 三者的元素类型应兼容。混用不同精度类型时需注意隐式转换行为。
4. **Bias 是可选参数**：当不需要 Bias 时，直接省略即可。
