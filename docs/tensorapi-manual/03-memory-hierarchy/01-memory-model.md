# 3.1 NPU 内存模型

昇腾 NPU 采用多级存储层次架构，每级存储有不同的容量、带宽、延迟和访问特性。TensorAPI 统一封装了这些存储层，使开发者无需关心底层地址空间差异。

## 3.1.1 内存层次全景

```
┌─────────────────────────────────────────────────────────────────┐
│                        Global Memory (GM)                        │
│                   容量: GB 级 | 带宽: 最低 | 延迟: 最高           │
│                  指针类型: __gm__ T*                              │
└────────────────────────────┬────────────────────────────────────┘
                             │ CopyGM2L1 (Cube) / CopyGM2UB (Vector)
                             ▼
┌─────────────────────────────────────────────────────────────────┐
│                         L1 Buffer (Cbuffer)                      │
│                   容量: MB 级 | 主要数据中转站                    │
│                  指针类型: __cbuf__ T*                            │
└───┬─────────┬─────────┬─────────┬─────────┬─────────────────────┘
    │         │         │         │         │
    │    CopyL12L0A  CopyL12L0B  CopyL12UB  CopyL12BT / CopyL12FB
    ▼         ▼         ▼         ▼         ▼
┌────────┐┌────────┐┌────────┐┌────────┐┌────────┐
│  L0A   ││  L0B   ││  UB    ││ BIAS   ││FIXBUF  │
│(Matrix ││(Matrix ││(Unified││ (Bias  ││ (Fix   │
│   A)   ││   B)   ││ Buffer)││ Buffer)││ Buffer)│
│__ca__  ││__cb__  ││__ubuf__││__bias__││__fbuf__│
│  T*    ││  T*    ││  T*    ││  T*    ││  T*    │
└───┬────┘└───┬────┘└────┬───┘└────────┘└───┬────┘
    │         │           │                   │
    └────┬────┘           │        CopyL0C2GM│CopyL0C2UB
         ▼                │                   │
    ┌─────────┐           ▼                   ▼
    │   L0C   │◄────── 计算完成回写
    │(Accumu- │    (通过 Fixpipe 写回)
    │ lator)  │
    │ __cc__  │
    │   T*    │
    └─────────┘
```

## 3.1.2 各级存储详述

### Global Memory (GM)

| 属性 | 说明 |
|------|------|
| 定位 | 片外主存，HBM/DDR |
| 容量 | GB 级 |
| 指针类型 | `__gm__ T*` |
| Location 标记 | `Location::GM` |
| 访问方式 | 通过 Copy 搬入片内后计算；不可直接用于计算 |

### L1 Buffer (Cbuffer)

| 属性 | 说明 |
|------|------|
| 定位 | 片内核心数据中转站 |
| 容量 | MB 级 |
| 指针类型 | `__cbuf__ T*` |
| Location 标记 | `Location::L1` |

L1 是 Cube 数据通路的**唯一入口**。所有 Cube 计算通路（GM→L1→L0A/L0B→L0C）都必须经过 L1。

### Unified Buffer (UB)

| 属性 | 说明 |
|------|------|
| 定位 | 片内通用缓冲区，Vector 指令的操作数来源 |
| 容量 | 数百 KB |
| 指针类型 | `__ubuf__ T*` |
| Location 标记 | `Location::UB` |

UB 是 Vector 数据通路的入口。Vector 的 GM→UB→GM 通路不经过 L1及 Cube 单元。

### L0A / L0B（Cube Matrix Buffers）

| 属性 | 说明 |
|------|------|
| 定位 | Cube 矩阵计算单元 (MAC Array) 的输入缓冲区 |
| 指针类型 | `__ca__ T*` / `__cb__ T*` |
| Location 标记 | `Location::L0A` / `Location::L0B` |

L0A 和 L0B 是 Cube 矩阵乘加 (MMAD) 的输入。数据从 L1 搬入 L0A/L0B 时，必须遵循对应的分形格式（如 NZ 格式给 L0A）。

### L0C（Accumulator Buffer）

| 属性 | 说明 |
|------|------|
| 定位 | Cube 矩阵计算单元的输出缓冲区 / 累加器 |
| 指针类型 | `__cc__ T*` |
| Location 标记 | `Location::L0C` |

L0C 保存 MMAD 的计算结果。结果可以通过两条路径搬出：
- **CopyL0C2UB** — 搬入 UB，用于后续 Vector 处理
- **CopyL0C2GM (Fixpipe)** — 直接写回 GM，支持量化/激活等后处理

### BIAS Buffer

| 属性 | 说明 |
|------|------|
| 定位 | Cube 矩阵计算的 Bias 输入缓冲区 |
| 指针类型 | `__biasbuf__ T*` |
| Location 标记 | `Location::BIAS` |

### FIXBUF (Fix Buffer)

| 属性 | 说明 |
|------|------|
| 定位 | Fixpipe 通路专用缓冲区 |
| 指针类型 | `__fbuf__ T*` |
| Location 标记 | `Location::FIXBUF` |

## 3.1.3 两条数据通路

TensorAPI 封装了两条独立的 NPU 数据通路：

### Cube 通路（矩阵计算链路）

```
GM ──CopyGM2L1──▶ L1 ──CopyL12L0A──▶ L0A ─┐
                     ──CopyL12L0B──▶ L0B ─┤ MMAD ──▶ L0C ──CopyL0C2GM──▶ GM
                     ──CopyL12BT───▶ BIAS ┤               ──CopyL0C2UB──▶ UB
                     ──CopyL12FB───▶ FIXBUF              (Fixpipe)
                     ──CopyL12UB───▶ UB
```

- **核心操作**：矩阵乘加 (MMAD)、Fixpipe 写回
- **数据格式**：分形格式（NZ / ZN / ZZ 等）
- **典型场景**：矩阵乘、卷积（矩阵计算部分）

### Vector 通路（向量计算链路）

```
GM ──CopyGM2UB──▶ UB ──Vector 操作──▶ UB ──CopyUB2GM──▶ GM
                   └──CopyUB2L1──▶ L1   (转入 Cube 通路)
```

- **核心操作**：逐元素向量计算
- **数据格式**：通常为平坦格式（ND / DN），也可为分形格式
- **典型场景**：激活函数、逐元素运算、Reduce、数据预处理

## 3.1.4 TensorAPI 中的数据通路建模

TensorAPI 通过 Copy 操作的**类型系统**来建模上述数据通路。每种搬移操作对应不同的 `CopyOperation` 标签：

| Copy 操作 | 通路 | 数据路径 |
|-----------|------|----------|
| `CopyGM2L1` | Cube | Global Memory → L1 |
| `CopyL12L0A` | Cube | L1 → L0A |
| `CopyL12L0B` | Cube | L1 → L0B |
| `CopyL12BT` | Cube | L1 → BIAS Buffer |
| `CopyL12FB` | Cube | L1 → FIXBUF |
| `CopyL0C2GM` | Cube | L0C → Global Memory (Fixpipe) |
| `CopyL0C2UB` | Cube | L0C → Unified Buffer |
| `CopyL12UB` | Cube | L1 → Unified Buffer |
| `CopyGM2UB` | Vector | Global Memory → Unified Buffer |
| `CopyUB2GM` | Vector | Unified Buffer → Global Memory |
| `CopyUB2L1` | Vector | Unified Buffer → L1 |

## 3.1.5 位置类型匹配

TensorAPI 的 Copy 操作在执行时会**编译期校验**源和目标 Tensor 的 Location 是否匹配。例如：

```cpp
auto l1Tensor = MakeTensor(MakeMemPtr<Location::L1>(l1Ptr), layout);
auto gmTensor = MakeTensor(MakeMemPtr<Location::GM>(gmPtr), layout);

auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1Tensor, gmTensor);  // OK: dest=L1, src=GM 匹配 CopyGM2L1

// Copy(atom, gmTensor, l1Tensor);  // 编译错误：方向不对
```

位置信息通过指针类型嵌入类型系统（`__gm__ T*` → `Location::GM` 等），实现了零开销的编译期校验。
