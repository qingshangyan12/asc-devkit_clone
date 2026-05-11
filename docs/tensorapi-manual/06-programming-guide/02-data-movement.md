# 6.2 数据搬运实战

本章深入讲解 TensorAPI 中各种数据搬运模式的选择与使用。

## 6.2.1 搬运路径决策树

```
需要搬运数据？
│
├─ 目标：L1 (用于后续 Cube 计算)
│   └─ CopyGM2L1             （从 GM 搬入 L1）
│
├─ 目标：UB (用于后续 Vector 计算)
│   ├─ 源在 GM  → CopyGM2UB   (Vector 通路直通)
│   ├─ 源在 L1  → CopyL12UB   (Cube 通路转入)
│   └─ 源在 L0C → CopyL0C2UB  (计算结果转入)
│
├─ 目标：L0A / L0B (用于 MMAD)
│   └─ 源必须在 L1
│       ├─ A 矩阵 → CopyL12L0A
│       └─ B 矩阵 → CopyL12L0B
│
├─ 目标：GM (写回结果)
│   ├─ 源在 UB  → CopyUB2GM   (Vector 通路)
│   ├─ 源在 L0C → CopyL0C2GM  (Fixpipe 通路)
│   └─ 源在 L1  → 需先转到 UB (CopyL12UB → CopyUB2GM)
│
├─ 目标：L1 (从 UB)
│   └─ CopyUB2L1              (Vector 通路转入 Cube 通路)
│
├─ 特殊：Bias / Fixbuf
│   ├─ L1 → BIAS  → CopyL12BT
│   └─ L1 → FIXBUF → CopyL12FB
```

## 6.2.2 GM ↔ L1 (Cube 通路主入口)

### GM → L1

```cpp
auto gmTensor = MakeTensor(
    MakeMemPtr<Location::GM>(gmData),
    MakeFrameLayout<NDLayoutPtn>(rows, cols));

auto l1Tensor = MakeTensor(
    MakeMemPtr<Location::L1>(l1Data),
    MakeFrameLayout<NDLayoutPtn>(rows, cols));  // 也可以是 NZ

auto atom = MakeCopy(CopyGM2L1{});
Copy(atom, l1Tensor, gmTensor);
```

**注意事项：**
- GM 上的 Layout 通常是 ND，L1 上的 Layout 根据后续用途选择 ND 或 NZ
- GM → L1 的搬运会自动处理 Layout Pattern 的转换（从 ND 到 NZ）

### L1 → GM (经 UB 中转)

L1 本身不能直接写回 GM，必须经过 UB 中转：

```cpp
// L1 → UB
auto l12ubAtom = MakeCopy(CopyL12UB{});
Copy(l12ubAtom, ubTensor, l1Tensor);

// UB → GM
auto ub2gmAtom = MakeCopy(CopyUB2GM{});
Copy(ub2gmAtom, gmTensor, ubTensor);
```

## 6.2.3 L1 → L0A / L0B (Cube 计算准备)

在 MMAD 之前，A 和 B 矩阵需要从 L1 搬入 L0A 和 L0B：

```cpp
// L1 上的 A 矩阵：NZ 格式
auto l1ATensor = MakeTensor(
    MakeMemPtr<Location::L1>(l1AData),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));

// L0A 上的 A 矩阵：NZ 格式
auto l0aTensor = MakeTensor(
    MakeMemPtr<Location::L0A>(l0aData),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(m, k));

// 搬运 A：L1 → L0A
auto l12l0a = MakeCopy(CopyL12L0A{});
Copy(l12l0a, l0aTensor, l1ATensor);

// B 矩阵同理，使用 ZN 格式和 CopyL12L0B
auto l1BTensor = MakeTensor(
    MakeMemPtr<Location::L1>(l1BData),
    MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));

auto l0bTensor = MakeTensor(
    MakeMemPtr<Location::L0B>(l0bData),
    MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<half>>(k, n));

auto l12l0b = MakeCopy(CopyL12L0B{});
Copy(l12l0b, l0bTensor, l1BTensor);
```

### 带转置的搬运

通过 `LoadDataTrait{true}` 可以在搬运过程中转置数据：

```cpp
auto atom = MakeCopy(CopyL12L0A{}, LoadDataTrait{true});
Copy(atom, l0aTensor, l1ATensor);  // A 矩阵在搬运时转置
```

## 6.2.4 L0C → GM (Fixpipe 写回)

MMAD 计算结果在 L0C 中，通过 Fixpipe（CopyL0C2GM）写回 GM。这是 Cube 通路的"出口"。

```cpp
auto l0cTensor = MakeTensor(
    MakeMemPtr<Location::L0C>(l0cData),
    MakeFrameLayout<ZZLayoutPtn, LayoutTraitDefault<half>>(m, n));

auto gmTensor = MakeTensor(
    MakeMemPtr<Location::GM>(gmData),
    MakeFrameLayout<NDLayoutPtn>(m, n));

// 标准 Fixpipe 写回
auto fixAtom = MakeCopy(CopyL0C2GM{});
Copy(fixAtom, gmTensor, l0cTensor);

// 带后处理的 Fixpipe (如 ReLU)
auto fixWith = MakeCopy(CopyL0C2GMWith{},
    FixpipeParams{},
    FixpipeTrait{/* .enableRelu = true */});
Copy(fixWith, gmTensor, l0cTensor);
```

## 6.2.5 Vector 通路 (GM ↔ UB)

Vector 通路的搬运不经过 L1，而是直接在 GM 和 UB 之间进行：

```cpp
// GM → UB
auto gmTensor = MakeTensor(
    MakeMemPtr<Location::GM>(gmData),
    MakeFrameLayout<NDLayoutPtn>(rows, cols));

auto ubTensor = MakeTensor(
    MakeMemPtr<Location::UB>(ubData),
    MakeFrameLayout<NDLayoutPtn>(rows, cols));

auto gm2ub = MakeCopy(CopyGM2UB{});
Copy(gm2ub, ubTensor, gmTensor);

// Vector 计算 ...

// UB → GM
auto ub2gm = MakeCopy(CopyUB2GM{});
Copy(ub2gm, gmTensor, ubTensor);
```

## 6.2.6 Cube ↔ Vector 互通 (UB ↔ L1)

两条通路之间可以通过 UB 和 L1 进行数据交换：

```cpp
// Cube 通路 → Vector 通路：L1 → UB
auto l12ub = MakeCopy(CopyL12UB{});
Copy(l12ub, ubTensor, l1Tensor);

// Vector 通路 → Cube 通路：UB → L1
auto ub2l1 = MakeCopy(CopyUB2L1{});
Copy(ub2l1, l1Tensor, ubTensor);

// Cube 结果 → Vector 后处理：L0C → UB
auto l0c2ub = MakeCopy(CopyL0C2UB{});
Copy(l0c2ub, ubTensor, l0cTensor);
```

## 6.2.7 完整 Cube 通路搬运流程图

```
     ┌─────────┐
     │   GM    │ ← 输入/输出数据 (ND 格式)
     └────┬────┘
    CopyGM2L1     CopyL0C2GM (Fixpipe)
          │              ▲
          ▼              │
     ┌─────────┐    ┌─────────┐
     │   L1    │    │   L0C   │ ← MMAD 结果 (ZZ 格式)
     └───┬─┬─┬─┘    └────▲────┘
         │ │ │            │
  CopyL12L0A │ CopyL12UB  │ MMAD
         │ │ │            │
     ┌───┘ │ └──────┐     │
     ▼     ▼        ▼     │
  ┌────┐┌────┐  ┌──────┐  │
  │L0A ││L0B │  │  UB  │──┘ (CopyL0C2UB)
  │ NZ ││ ZN │  │(通用) │
  └────┘└────┘  └──┬───┘
     │     │        │
     └──┬──┘   CopyUB2GM
        │         ▲
        ▼         │
     MMAD    ┌───────┐
        │    │  GM   │
        └────┤       │
             └───────┘
```
