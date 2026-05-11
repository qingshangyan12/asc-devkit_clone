# 5.4 Copy 算法 API

Copy 算法 API 提供 NPU 各存储层次之间的数据搬运能力。TensorAPI 通过 **CopyAtom + Copy()** 的二元模型来组织和执行搬运操作。

## 5.4.1 核心类型

### CopyTraits

```cpp
template <typename CopyOperation, typename... CopyOpArgs>
struct CopyTraits {};
```

Copy 操作的配置容器。4 参数全特化形式为：

```cpp
template <typename CopyOp, typename Traits, typename CopyOpWith, typename TraitsWith>
struct CopyTraits<CopyOp, Traits, CopyOpWith, TraitsWith> {
    using TraitType = typename Traits::TraitType;
    static constexpr const TraitType defaultTrait = Traits::value;

    template <typename... Args>
    constexpr auto with(const Args& ...args) const;
};
```

### CopyAtom

```cpp
// 主模板
template <typename... Args>
struct CopyAtom;

// 单操作标签（自动包装为 CopyTraits）
template <typename CopyOperation>
struct CopyAtom<CopyOperation> : CopyAtom<CopyTraits<CopyOperation>> {};

// CopyTraits 版本（实际实现）
template <typename... Args>
struct CopyAtom<CopyTraits<Args...>> : CopyTraits<Args...> {
    // .Call() — 执行搬运
    template <const TraitType& traits = defaultTrait, typename... Params>
    void Call(const Params& ...params) const;

    // .with() — 创建新配置的 CopyAtom
    template <typename... TraitsArgs>
    auto with(TraitsArgs&&... args) const;
};
```

## 5.4.2 Copy 操作标签

### Cube 通路

| 操作标签 | 数据路径 | 说明 |
|----------|----------|------|
| `CopyGM2L1` | GM → L1 | Cube 通路入口 |
| `CopyL12UB` | L1 → UB | L1 数据转入 Unified Buffer |
| `CopyL12L0A` | L1 → L0A | 搬运 A 矩阵到 Cube 计算单元 |
| `CopyL12L0B` | L1 → L0B | 搬运 B 矩阵到 Cube 计算单元 |
| `CopyL12BT` | L1 → BIAS Buffer | 搬运 Bias 数据 |
| `CopyL12FB` | L1 → FIXBUF | 搬运到 Fix Buffer |
| `CopyL0C2GM` | L0C → GM | Fixpipe 写回 GM |
| `CopyL0C2GMWith` | L0C → GM (带参数) | Fixpipe 写回（带运行时参数） |
| `CopyL0C2UB` | L0C → UB | L0C 结果搬入 UB 供后续处理 |
| `CopyL0C2UBWith` | L0C → UB (带参数) | L0C → UB（带运行时参数） |

### Vector 通路

| 操作标签 | 数据路径 | 说明 |
|----------|----------|------|
| `CopyGM2UB` | GM → UB | Vector 通路入口 |
| `CopyUB2GM` | UB → GM | Vector 通路出口 |
| `CopyUB2L1` | UB → L1 | 转入 Cube 通路 |

## 5.4.3 Copy 函数

### Copy()

```cpp
// 带显式 traits 参数
template <typename Tp, const Tp& traits, typename T, typename... Params>
void Copy(const CopyAtom<T>& atomCopy, const Params& ...params);

// 使用 atom 内部的默认 traits
template <typename T, typename... Params>
void Copy(const CopyAtom<T>& atomCopy, const Params& ...params);
```

`Copy()` 是高层算法调用，接收一个 `CopyAtom` 和源/目标 Tensor 等参数，将调用委托给 `atomCopy.Call(params...)`。

**参数：**
| 参数 | 说明 |
|------|------|
| `atomCopy` | CopyAtom 实例，决定搬运路径和配置 |
| `params...` | 通常为 `(dstTensor, srcTensor)` 或 `(dstTensor, srcTensor, extraParams)` |

**示例：**
```cpp
auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1Tensor, gmTensor);                        // GM → L1
Copy(atom, l1Tensor, gmTensor, CopyGM2L1Trait{...});   // 带 Trait
```

### MakeCopy()

```cpp
template <typename... Args>
auto MakeCopy(const Args& ...traits);
```

创建 CopyAtom 的工厂函数。

**示例：**
```cpp
// 默认配置
auto a1 = MakeCopy(CopyGM2L1{});

// 带 LoadDataTrait
auto a2 = MakeCopy(CopyL12L0A{}, LoadDataTrait{true});

// 带 Fixpipe 参数
auto a3 = MakeCopy(CopyL0C2GMWith{}, FixpipeParams{}, FixpipeTrait{});
```

## 5.4.4 Traits 类型详解

### LoadDataTrait

```cpp
struct LoadDataTrait {
    bool transposed;
    // transposed=true: 转置读取
};
```

用于 L1 → L0A/L0B 的数据搬运，控制是否对数据进行转置。

### CopyGM2L1Trait / CopyGM2L1TraitDefault

GM → L1 搬运的配置。默认使用 `CopyGM2L1TraitDefault`。

### CopyL12UBTraitDefault

L1 → UB 搬运的默认配置。

### CopyL0C2GMTraitDefault / FixpipeTrait / FixpipeParams

Fixpipe (L0C → GM) 的配置：

| FixpipeTrait 字段 | 说明 |
|-------------------|------|
| `roundMode` | 舍入模式 (`RoundMode::DEFAULT` / `RoundMode::HYBRID`) |
| `enableRelu` | 是否使能 ReLU 后处理 |
| `enableChannelSplit` | 是否使能通道拆分 |
| `dualDstCtl` | 双目标控制 (`DualDstMode::DUAL_DST_DISABLE` 等) |

| FixpipeParams 字段 | 说明 |
|--------------------|------|
| `unitFlag` | 单元标志 |

| DualDstMode 枚举 | 说明 |
|-------------------|------|
| `DUAL_DST_DISABLE` | 禁用双目标 |
| `DUAL_DST_SPLIT_M` | 按 M 方向拆分 |
| `DUAL_DST_SPLIT_N` | 按 N 方向拆分 |

### CopyMode 枚举

```cpp
namespace CopyMode {
    struct NORMAL {};
    struct NORMAL_COORD {};
    struct TRANS {};
    struct TRANS_COORD {};
    struct TRANS_B8B4 {};
    struct TRANS_B8B4_COORD {};
}
```

控制搬运过程中的格式转换和坐标处理。

## 5.4.5 使用模式

### 模式 1：基本搬运

```cpp
auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1Tensor, gmTensor);
```

### 模式 2：带 Traits 的搬运

```cpp
auto atom = MakeCopy(CopyL12L0A{}, LoadDataTrait{true});  // 转置读
Copy(atom, l0aTensor, l1Tensor);
```

### 模式 3：with() 链式配置

```cpp
auto baseAtom = MakeCopy(CopyGM2L1{});
auto configuredAtom = baseAtom.with(LoadDataTrait{false});
Copy(configuredAtom, l1Tensor, gmTensor);
// baseAtom 保持不变，可以继续派生其他配置
```

### 模式 4：带运行时参数 (With 系列)

```cpp
auto atom = MakeCopy(CopyL0C2GMWith{}, FixpipeParams{.unitFlag = 1});
Copy(atom, gmTensor, l0cTensor);
```

### 模式 5：带显式 traits 模板参数的 Copy

```cpp
using TraitsType = CopyGM2L1TraitDefault::TraitType;
static constexpr TraitsType myTraits = ...;
Copy<myTraits>(atom, l1Tensor, gmTensor);
```
