# 1.2 编程思想

TensorAPI 的设计围绕三个核心理念展开：**Layout 驱动**、**类型即契约**、**分形为先**。

## 1.2.1 Layout 驱动

TensorAPI 的核心抽象是 **Layout**——它描述了"一段连续内存上的数据排布方式"。在一个 Tensor 中：

- **Engine (引擎)** 持有原始内存指针，表示"数据在哪"
- **Layout (布局)** 持有形状 (Shape) 和步长 (Stride)，表示"数据怎么排列"

这两者的组合形成 LocalTensor，TensorAPI 中的所有操作都在这个抽象上展开：

```
         ┌──────────────────────────────────┐
         │          LocalTensor              │
         │                                   │
         │   ┌──────────┐  ┌──────────────┐ │
         │   │  Engine   │  │   Layout     │ │
         │   │  (指针)    │  │ (Shape+Stride)│ │
         │   └──────────┘  └──────────────┘ │
         │                          │        │
         │                    坐标→地址映射    │
         └──────────────────────────────────┘
```

这种设计的好处是：**同一份数据可以有不同的 Layout 视图**。例如同一块 L1 内存，可以用 NZ Layout 给 Cube 计算单元使用，也可以用 ND Layout 给 Vector 单元使用——无需实际搬移数据。

## 1.2.2 类型即契约

TensorAPI 大量使用 C++ 模板元编程，将关键信息**编码到类型系统中**：

- `Layout<Shape, Stride, Info>` — Shape、Stride 和 LayoutPattern 都是类型的一部分
- `LayoutTraitDefault<T, C0>` — 数据类型 T 和 C0 块大小嵌入类型
- `CopyTraits<Op, Traits, OpWith, TraitsWith>` — 操作类型与配置参数全部模板化

这使得：
- **编译期检查**：不兼容的数据搬运路径在编译时即可发现
- **零开销抽象**：所有布局计算在编译期完成，运行时无额外开销
- **可组合性**：通过 `operator()` / `Slice()` / `Compose()` 等方法，可以对 Tensor 进行各种变换，产生新的、类型正确的 Tensor 对象

## 1.2.3 分形为先

与传统 CPU/GPU 编程中"行优先"或"列优先"的线性布局不同，昇腾 NPU 的数据在片内存储时采用 **分形 (Fractal) 布局**——数据以固定大小的块（典型的如 16×16）为单位进行分块存储。

TensorAPI 原生支持这种分形数据组织方式，通过 13 种 Layout Pattern 覆盖所有 NPU 数据格式：

| 类别 | Pattern | 说明 |
|------|---------|------|
| 通用分形 | `NZLayoutPtn` | NZ 格式（16×16 块，行优先） |
| 通用分形 | `ZNLayoutPtn` | ZN 格式（16×16 块，列优先） |
| 通用分形 | `ZZLayoutPtn` | ZZ 格式 |
| 平坦布局 | `NDLayoutPtn` | 行优先连续存储 |
| 平坦布局 | `DNLayoutPtn` | 列优先连续存储 |
| 扩展布局 | `NDExtLayoutPtn` | 扩展 ND（2×1 内分形） |
| 扩展布局 | `DNExtLayoutPtn` | 扩展 DN（2×1 内分形） |
| Scale 格式 | `NNLayoutPtn` | fp8 Scale NN 格式 |
| Scale 格式 | `ScaleANDLayoutPtn` | ScaleA ND 格式 |
| Scale 格式 | `ScaleADNLayoutPtn` | ScaleA DN 格式 |
| Scale 格式 | `ScaleBNDLayoutPtn` | ScaleB ND 格式 |
| Scale 格式 | `ScaleBDNLayoutPtn` | ScaleB DN 格式 |

理解并善用这些 Layout Pattern 是写出高性能 TensorAPI 算子的关键。

## 1.2.4 三段式编程模型

典型的 TensorAPI 程序遵循三段式流程：

```
┌──────────────┐      ┌──────────────┐      ┌──────────────┐
│  构造阶段     │ ───→ │  配置阶段     │ ───→ │  执行阶段     │
│              │      │              │      │              │
│ MakeMemPtr() │      │ .with(...)   │      │ Copy()       │
│ MakeLayout() │      │ .SetCache... │      │ Mmad()       │
│ MakeTensor() │      │              │      │ atom.Call()  │
└──────────────┘      └──────────────┘      └──────────────┘
```

1. **构造阶段** — 使用工厂函数创建 Tensor、Layout、Atom 等对象
2. **配置阶段** — 通过 `with()` / `SetL2CacheHint()` 等方法配置运行时参数
3. **执行阶段** — 调用 `Copy()` / `Mmad()` 等算法函数执行实际操作

## 1.2.5 Atom + Algorithm 分层

数据搬运和矩阵计算在 TensorAPI 中分为两层：

- **Atom (原子层)** — 封装具体的硬件操作类型和配置参数，如 `CopyAtom<CopyTraits<CopyGM2L1>>`
- **Algorithm (算法层)** — 调用 Atom 的 `.Call()` 方法执行，如 `Copy(atom, dst, src)`

这种分层使操作配置可以独立于执行逻辑。通过 `atom.with(...)` 可以创建新配置的 Atom，而不改变原始配置。

```cpp
// 创建基础 Atom
auto baseAtom = MakeCopy(CopyGM2L1{});

// 派生新配置的 Atom（不改变 baseAtom）
auto burstAtom = baseAtom.with(LoadDataTrait{true});
Copy(burstAtom, l1Tensor, gmTensor);
```
