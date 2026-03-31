# `GetShape` / `GetStride`（`Layout` 特化）说明

本文档说明 `impl/experimental/tensor_api/tensor/layout_method.h` 中针对 `Layout<ShapeType, StrideType>` 的自由函数 `GetShape`、`GetStride`，及其与 `layout_definition.h` 中成员 `Layout::Shape` / `Layout::Stride`、`tuple_impl.h` 中 `GetValue` 的关系。

---

## 1. `GetShape`

### 1.1 功能介绍

`GetShape` 在 `Layout` 上按模板索引包 `Is...` 取出 **shape 子结构**（或某一维的标量），等价于在 `layout` 上调用成员函数 `Shape<Is...>()`。

- 内部通过 `Layout::Shape()` 访问 `Layout` 继承的 `Std::tuple<T, U>` 中 **下标 0** 的 shape 元组，再用 `GetValue<0, I...>` 做嵌套/切片选取（见 `tuple_impl.h`）。
- 提供 `const Layout&` 与 `Layout&` 两个重载，以便在常量与非常量场景下都能以统一接口取 shape，并保持与成员函数一致的引用类别与返回值类型（`decltype(auto)` 透传）。

### 1.2 参数说明

| 模板参数 | 含义 |
|---------|------|
| `Is...` | 可选。多级索引：先按 `Is` 在 **shape 元组** 上逐级 `get`，再对最内层元组取维；若 `sizeof...(Is)==0`，则行为由 `GetValue` 的特化决定（见下节与实现）。 |
| `ShapeType` | `Layout` 的 shape 类型，须为 `Std::tuple<...>`。 |
| `StrideType` | `Layout` 的 stride 类型，须为 `Std::tuple<...>`。 |

| 函数参数 | 含义 |
|---------|------|
| `layout` | 待查询的 `Layout` 对象；可为只读或非常量引用。 |

### 1.3 约束说明

- **编译期断言**：`Std::is_tuple_v<ShapeType> && Std::is_tuple_v<StrideType>`，否则静态断言失败（提示 *ShapeType or StrideType is not tuple*）。
- **与 `Layout` 构造一致**：`Layout` 本身也要求 `T`、`U` 均为 tuple（见 `layout_definition.h` 构造函数中的 `static_assert`）。

### 1.4 返回值

- 返回类型由 `layout.template Shape<Is...>()` 决定，为 **`decltype(auto)`** 语义下的引用或值（与 `GetValue<0, I...>` 一致）。
- **典型情况**：
  - **`sizeof...(Is)==0`**：`Shape<>` 对应 `GetValue<0>(tuple)`，返回 **完整 shape 元组** `T`。
  - **单索引 `I` 且内层无后续 `Is`**：返回 `Std::get<I>(tupleEle)`，即 **某一维或子元组**，取决于 `tupleEle` 的元素类型。
  - **多索引 `I, Is...` 且 `sizeof...(Is)>0`**：返回 `Std::make_tuple(Std::get<I>(tupleEle), ...)`，即从当前层元组中 **抽出多个分量组成新元组**。

### 1.5 示例（概念性）

```cpp
// const Layout<ShapeType, StrideType>& L = ...;

// 无模板实参时 Is... 为空，等价于 layout.Shape<>()，经 GetValue<0> 得到整条 shape 元组
auto shapeTuple = GetShape(L);

// 带索引时等价于 layout.Shape<0>() 等，按 GetValue<0, I, Is...> 规则取子结构或某一维
auto s0 = GetShape<0>(L);
```

嵌套 `ShapeType` 时，`Is...` 的写法需与元组层级一致；可对照 `MakeLayout` 与测试（如 `tests/api/tensor_api/struct/test_tensor_tile_layout.cpp`）。

---

## 2. `GetStride`

### 2.1 功能介绍

`GetStride` 与 `GetShape` 对称：在 `Layout` 上按 `Is...` 取出 **stride 子结构**（或某一维的 stride 标量），等价于 `layout.template Stride<Is...>()`。

- 成员 `Stride()` 通过 `GetValue<1, I...>` 访问 **tuple 中下标 1** 的 stride 元组，选取规则与 `GetShape` 相同，仅 **index 从 0 换为 1**。

### 2.2 参数说明

与 `GetShape` 相同，仅 **语义从 shape 换为 stride**：

| 模板参数 | 含义 |
|---------|------|
| `Is...` | 在 **stride 元组** 上的多级索引（与 shape 侧结构通常并行，维数需与 layout 定义一致）。 |
| `ShapeType` / `StrideType` | 同 `Layout` 模板参数。 |

| 函数参数 | 含义 |
|---------|------|
| `layout` | `const Layout&` 或 `Layout&`。 |

### 2.3 约束说明

- 与 `GetShape` **相同**的 `static_assert`：`ShapeType`、`StrideType` 均须为 tuple。

### 2.4 返回值

- 由 `layout.template Stride<Is...>()` 推导，规则同 **1.4**，只是把操作对象从 shape 元组换成 stride 元组。

### 2.5 示例（概念性）

```cpp
// const Layout<ShapeType, StrideType>& L = ...;
auto t0 = GetStride<0>(L);
```

---

## 3. 与 `Layout::Shape` / `Stride`、`GetValue` 的对应关系（实现要点）

- `Layout::Shape()` / `Stride()` 分别对 `Std::tuple<T, U>` 做 `GetValue<0, I...>`、`GetValue<1, I...>`（见 `layout_definition.h`）。
- `GetValue<index, I, Is...>`（`tuple_impl.h`）：
  - 取 `Std::get<index>(t)` 得到 **shape 或 stride 那一侧的元组** `tupleEle`；
  - 若 `sizeof...(Is)==0`，返回 `Std::get<I>(tupleEle)`；
  - 否则返回 `Std::make_tuple(Std::get<I>(tupleEle), Std::get<Is>(tupleEle)...)`。

因此 **`GetShape<Is...>(layout)` 与 `layout.Shape<Is...>()`、`GetStride<Is...>(layout)` 与 `layout.Stride<Is...>()` 在功能上完全一致**；自由函数主要用于泛型代码中统一写 `GetShape` / `GetStride`，并与同文件中针对原始 tuple 的 `GetShape` 重载形成一致风格。

---

## 4. 文件与行号参考

| 符号 | 文件 | 大致位置 |
|------|------|----------|
| `GetShape` / `GetStride`（`Layout`） | `layout_method.h` | 约 142–154、173–185 行 |
| `Layout::Shape` / `Stride` | `layout_definition.h` | 约 69–90 行 |
| `GetValue` | `tuple_impl.h` | 约 155–170 行 |

---

*文档基于仓库内头文件阅读整理，若后续 API 变更，请以源码为准。*
