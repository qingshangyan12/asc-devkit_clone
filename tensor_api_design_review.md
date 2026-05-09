# TensorAPI 设计评审：对标 CuTe

> 范围：`include/tensor_api/` + `impl/tensor_api/`
> 参照：NVIDIA CuTe (CUTLASS 3.x)
> 结论按"影响越靠后越严重"排序。每条问题都给出**位置**、**问题**、**修改建议**。

---

## 总览

| 维度 | 当前状态 | CuTe 状态 | Gap |
|---|---|---|---|
| Layout 代数（composition / divide / product / complement / coalesce） | 几乎缺失 | 完整 | **致命** |
| Shape/Stride 维度通用性 | 硬编码 2D/4D | 任意嵌套 | **严重** |
| 静态整型 | `Int<size_t>`（无符号） | `C<int>` 或 `Int<int>` | **严重** |
| Tensor 切片语义 | `op()` 与 `Slice` 重叠 | `_` (Underscore) 统一 | 中等 |
| Atom 注入参数（`with`） | `XxxWith` 影子子类 | 模板组合 | 中等 |
| Routing 维护性 | 手写 N 维偏特化表 | tag dispatch | 中等 |
| Engine / 指针抽象 | 缓存 hint 内嵌、依赖全局 | 无状态 iterator | 中等 |
| 编译期开销 | 顶层一次 include 全部 atom | 用什么 include 什么 | 轻 |

---

## 一、Layout / Tuple / Shape / Stride（核心代数）

### P1-1【致命】缺失 CuTe 最核心的代数运算

**位置**
- `impl/tensor_api/tensor/layout_definition.h:50-135`
- `impl/tensor_api/tensor/local_tensor_impl.h:144-161`

**问题**
`Layout` 仅暴露 `Shape/Stride/Get/Rank/Size/Capacity/operator()`。完全没有 `composition / complement / logical_divide / logical_product / zipped_divide / tiled_divide / coalesce / flatten / inverse / right_inverse`。
更严重的是，`LocalTensor::Compose` / `LocalTensor::Tile` 直接调用 `Layout().Compose(...)` 与 `Layout().Tile(...)`，**而 `Layout` 根本没有这两个成员函数**——一旦实例化即编译失败。

**修改建议**
1. 立刻删掉或 `static_assert(false, "not implemented")` 现在的 `LocalTensor::Compose/Tile`，避免假装可用。
2. 引入下列自由函数（建议放到新文件 `impl/tensor_api/tensor/layout_algebra.h`）：
   ```cpp
   // 形如 R = A ∘ B：R(c) = A(B(c))
   template <class LayoutA, class LayoutB> constexpr auto Composition(A, B);
   // 把连续可合并的维度合并成一维
   template <class L> constexpr auto Coalesce(L);
   // 把多层 tuple 拍平
   template <class L> constexpr auto Flatten(L);
   // 用 tiler 切分 layout，得到 ((tile), (rest)) 的两层结构
   template <class L, class Tiler> constexpr auto LogicalDivide(L, Tiler);
   template <class L, class Tiler> constexpr auto ZippedDivide(L, Tiler);
   // 与 LogicalDivide 对偶：把 layout 复制 tiler 次
   template <class L, class Tiler> constexpr auto LogicalProduct(L, Tiler);
   // 求 right-inverse / complement，用于线程映射
   template <class L> constexpr auto RightInverse(L);
   template <class L, class Cosize> constexpr auto Complement(L, Cosize);
   ```
3. 这一组算子是后续所有 "用 Layout 描述线程/tile 映射" 的基石。建议优先级排在最前。

---

### P1-2【严重】`StaticLayoutSize` 硬编码 2/4 维

**位置** `impl/tensor_api/tensor/layout_size.h:73-113`

**问题** `StaticLayoutSize` 只在 `IsStaticLayout<2>` 或 `IsStaticLayout<4>` 时给出尺寸，否则返回 `Std::Int<0>{}`。这会让任何 3 维或非对称嵌套（如 `((m0,m1,m2),(n0,n1))`）静默拿到 size = 0，进而污染 `Layout::size`（`layout_definition.h:54`）。
注意：同文件 `layout_method.h:184-213` 已经有正确的递归 `Coshape/Cosize`，两套实现各算各的，是设计裂痕。

**修改建议**
- 用统一的 cosize 公式重写：
  ```
  cosize(L) = 1 + Σ_i (shape_i - 1) * |stride_i|   (递归到叶子)
  ```
- 删掉 `GetTwoDimStaticLayoutSize` / `GetFourDimStaticLayoutSize`，让 `StaticLayoutSize::size` 直接用 `Cosize<>(layout)` 在编译期求值。
- 同时删掉 `TWO_DIM_DATA / FOUR_DIM_DATA` 宏依赖（`constant_impl.h:30-31`）。

---

### P1-3【严重】`Std::Int<v>` 是 `size_t`，无法表示负 stride

**位置**
- `impl/utils/std/type_traits/integral_constant.h:48` — `using Int = integral_constant<size_t, v>`
- `impl/tensor_api/tensor/layout_method.h:191-194` — `auto absStride = stride < 0 ? -stride : stride;`

**问题** Int 模板参数是无符号 `size_t`，对 `Std::Int<size_t,v>` 取负是 underflow，所谓 "absStride" 在编译期常量场景下是错误的。CuTe 的 `Int<v>` 用 `int`（或更通用的 `C<v>`），明确支持负值。后续做 reverse / negate / stride flip 会立刻踩坑。

**修改建议**
- 把 `using Int = integral_constant<size_t, v>` 改为 `using Int = integral_constant<int, v>`，或新增 `template <int v> using C = integral_constant<int, v>` 与现有 `Int` 共存（更兼容）。
- `STD_INT_BINARY_OP` 重新审视溢出：`/` 与 `%` 在负值上需谨慎。
- 在 `CoshapeCompute` 里删掉 `absStride` 的判断（CuTe 是 `(shape-1)*stride` 直接累加，符号统一处理）。

---

### P1-4【中等】`Tile` 一词被三种语义占用

**位置**
- `include/tensor_api/tensor/layout.h:37` — `MakeTile<...>` 工厂
- `impl/tensor_api/tensor/layout_definition.h:43` — `using Tile = Std::tuple<...>`
- `impl/tensor_api/tensor/local_tensor_impl.h:154-161` — `LocalTensor::Tile(...)` 动作

**问题** 同名 `Tile` 三义：数据类型别名、构造函数、张量动作。用户代码里很容易混淆。

**修改建议**
- 类型别名改名 `using Tiler = Std::tuple<...>;`；构造函数 `MakeTiler(...)`。
- `LocalTensor::Tile(...)` 改名 `TiledDivide(...)` 或 `LocalTile(...)`，与 CuTe 习惯一致。

---

### P1-5【严重】`MakeLayout(shape)` 默认 stride 仅支持 depth 2

**位置** `impl/tensor_api/tensor/layout_method.h:94-109` `ComputeStride`

**问题** 强制 `tuple_size_v<ShapeType> == 2`，且子元素长度必须相等。对 `(M,(K0,K1))` 这种合法层级 shape 直接 `static_assert` 失败。

**修改建议**
- 改造成通用递归版：对每个叶子计算 column-major（或 row-major，二选一作为 API 默认）stride，其余从底向上累乘。
- 暴露 `MakeLayoutLeft(shape)` / `MakeLayoutRight(shape)`，与 CuTe 命名一致。
- 同时把 `ComputeFlatStride`（row-major）和 `ComputeStride`（col-major）的语义在文档里写死。

---

### P1-6【严重】`Crd2Idx` 与 `MakeLayout` 默认布局方向相反

**位置** `impl/tensor_api/tensor/coord_index.h:40-77` 与 `layout_method.h:111-147`

**问题**
- `MakeLayout(shape)` 默认走 `ComputeFlatStride` → row-major（`stride[N-1] = 1`）。
- `Crd2IdxITT(int_coord, shape, stride)` 用 `Product(get<I0>(shape))` 做 mod/div，按 `I0` 优先展开，等价于 column-major 解码。

→ 用 `Layout()(int_coord)` 与 `Crd2Idx(MakeCoord(...), ...)` 算出的 idx 不一致，**语义不一致**而非"文档缺失"。

**修改建议**
- 选定一个方向（CuTe 用 column-major，对 GPU 内存更友好；NPU 上推荐 column-major 与 NZ/ZN 自然格式契合）。
- 让 `MakeLayout(shape)` 默认与 `Crd2Idx(int, shape)` 一致，或拆出 `MakeColMajorLayout` / `MakeRowMajorLayout` 两个工厂，避免隐式默认。

---

## 二、Tensor / Pointer / Engine

### P2-1【中等】`LocalTensor::operator()` 与 `Slice` 语义重叠

**位置** `impl/tensor_api/tensor/local_tensor_impl.h:96-141`

**问题**
- `op[](coord)` → 取标量 `Data()[Layout()(coord)]`
- `op()(coord)` → 调 `MakeCoordLayout`，新 shape = `shape - coord`（"从 coord 起的剩余区域"）
- `Slice(coord, info)` → `MakeSliceLayout`，再 `min(剩余, info)` 截断

这把"完整坐标查值"与"切尾"用 `[]` 与 `()` 强行分裂；用户无法用 CuTe 的 `_` (Underscore) 标记任意维度切片。

**修改建议**
1. 引入 `struct Underscore {};` 与全局常量 `inline constexpr Underscore _;`
2. `operator()` 实现 CuTe 风格：
   - 全部为整数 → 返回 reference（标量）
   - 含 `_` → 返回子张量，被 `_` 占位的维度保留
3. 把 `Slice(coord, sliceShape)` 改名 `LocalSlice(tensor, coord, shape)` 作为算法层 free function。
4. `MakeCoordLayout` 这种 "shape - coord" 的语义只在内部 helper 中保留，不暴露成 `operator()`。

---

### P2-2【中等】`Engine` 把 `CacheMode` 内嵌在指针里

**位置** `impl/tensor_api/tensor/engine_impl.h:31-65`

**问题** 每个 `LocalTensor` 都额外携带 1 byte `CacheMode`，对 UB/L0A 无意义；slice 时还要透传。CuTe 把 cache hint 编进 atom（如 `SM80_CP_ASYNC_CACHEALWAYS`）。

**修改建议**
- 删除 `ViewEngine::mode` 与 `LocalTensor::SetL2CacheHint`。
- 在 `CopyAtom` 上增加 `with(CacheMode)`，让 hint 仅在能用到的拷贝原子上出现。
- 用法：`auto atom2 = atom.with(CacheMode::CACHE_MODE_NORMAL); Copy(atom2, dst, src);`

---

### P2-3【中等】`MakeMemPtr` 依赖全局 `asc_get_phy_buf_addr(0)`

**位置** `impl/tensor_api/tensor/pointer_pattern_impl.h:43-48`

**问题**
- 隐式单例化的物理地址源，host 端 mock / 单测困难。
- `MakeMemPtr<H, TraitOrType>(args...)` 与 `MakeMemPtr<H>(args...)` 重载分发靠 `IsMemPtrIterator` —— 当 byteOffset 类型也实现了 `operator*`（如某 wrapper），二义性触发。

**修改建议**
- 拆成清晰的命名工厂：
  ```cpp
  template <class T> auto MakeGmemPtr(T* p);
  template <class T> auto MakeCbufPtr(T* p);
  template <class T> auto MakeUbPtr(T* p);
  // ...
  template <class T> auto MakeGmemPtrFromOffset(uint64_t byteOffset);
  ```
  对标 CuTe 的 `make_gmem_ptr / make_smem_ptr`。
- `asc_get_phy_buf_addr` 通过模板上下文（如 `HardwareCtx` 参数）传入，而非全局调用。

---

### P2-4【轻】`IterAdaptor::operator&|` 直接 reinterpret 指针

**位置** `impl/tensor_api/tensor/pointer_adaptor_impl.h:56-62`

**问题** 通用基类里出现 `reinterpret_cast<uint64_t>(ptr) & mask`，不同 location 的指针段未必能直接 reinterpret。

**修改建议**
- 把这类 alignment trick 下沉到具体 hardware 层（`arch/...`），并在文件头注释适用场景与硬件假设。
- 通用 `IterAdaptor` 仅保留 `+`、`*`、`[]`、比较运算。

---

## 三、Atom / Algorithm / Routing 三层

### P3-1【中等】`XxxWith` 影子子类 + 4 参偏特化

**位置**
- `impl/tensor_api/atom/cube/mmad.h:30` — `struct MmadOpWith : public MmadOperation {};`
- `impl/tensor_api/atom/copy_traits_impl.h:34` — 4 参 `CopyTraits<CopyOp, Traits, CopyOpWith, TraitsWith>`
- `impl/tensor_api/atom/cube/copy_l0c2gm.h:33-41`

**问题** 每加一个需要 `with(...)` 的 op，都要新建一个 `XxxWith` 空子类、再手写一份 4 参 `CopyTraits` 偏特化，模式重复且不直观。

**修改建议**
- 让 `with(args...)` 不切换 op 类型，只构造一个新的 `CopyTraits`：
  ```cpp
  template <class Op, class Trait, class... ExtraArgs>
  struct CopyTraits {
      Trait trait;
      Std::tuple<ExtraArgs...> extra;
      template <class... Args>
      constexpr auto with(Args&&... args) const {
          return CopyTraits<Op, Trait, Args...>{trait, {std::forward<Args>(args)...}};
      }
      template <class... Tensors>
      __aicore__ void Call(const Tensors&... ts) const {
          // 把 extra 解包到 Op::Copy 末尾
          Std::apply([&](auto&&... e){ Op::template Copy<Trait>(ts..., e...); }, extra);
      }
  };
  ```
- 删除 `CopyL0C2GMWith` / `MmadOpWith` 等空标签子类。

---

### P3-2【中等】Routing 表全是手写偏特化（组合爆炸）

**位置**
- `impl/tensor_api/arch/cube/gm_to_l1/routing.h:48-117`（14 条）
- `impl/tensor_api/arch/cube/l1_to_l0a/routing.h`（多 1 维 `CopyMode`）

**问题**
- 每加一种 layout pattern 要改所有 routing；
- Fallback `CopyGM2L1Ignore` 默默发空 Run，传错 layout 不会 fail（`l1_to_ub/routing.h:33` 加了 static_assert，其它 routing 都没有）；
- `CopyMode` 用 `(noTrans, isB8B4)` 两 bool 编 4 个枚举，扩展性差。

**修改建议**
1. **Fallback 一律 `static_assert`**：所有 `XxxIgnore::Run` 改成 `static_assert(sizeof...(Args) == 0, "Unsupported layout combination for XxxAtom");`，让错配在编译期立刻可见。
2. **用 trait selector 替换偏特化表**：
   ```cpp
   template <class Dst, class Src, class Arch>
   struct SelectGm2L1 {
       using DstPtn = GetLayoutPattern<typename Dst::layoutType>;
       using SrcPtn = GetLayoutPattern<typename Src::layoutType>;
       using type =
           Std::conditional_t<Std::is_same_v<DstPtn, NZLayoutPtn> && Std::is_same_v<SrcPtn, NDExtLayoutPtn>, CopyGmToCbufMultiND2Nz,
           Std::conditional_t<Std::is_same_v<DstPtn, ZNLayoutPtn> && Std::is_same_v<SrcPtn, NDExtLayoutPtn>, CopyGmToCbufMultiND2Zn,
           ... >>;
   };
   ```
   （或用 `TupleMap` 但带 `static_assert` 防 fallback。）
3. **`CopyMode` 改成 strong tag**：把 `(noTrans, isB8B4)` 拆成两个独立 tag `TransposeTag = NoTrans|Trans` 与 `WidthTag = B8B4|Normal`，避免再爆出第 3 第 4 维。

---

### P3-3【中等】`Copy(atom, ...)` 模板转发链 4 层

**位置**
- `impl/tensor_api/algorithm/copy_impl.h:33` → `atomCopy.Call<traits>(...)`
- `impl/tensor_api/atom/copy_atom_impl.h:59` → `CopyTraits::CopyUnpack`
- `impl/tensor_api/atom/copy_traits_impl.h:47` → `CopyOp::Copy`
- `impl/tensor_api/arch/cube/gm_to_l1/copy.h:42` → `DataCopyImpl` → `Tensor2Tensor::Run`

**问题** 中间层只是模板转发，调试 trace 长、出错信息深。

**修改建议**
- 合并 `CopyTraits::CopyUnpack` 与 `CopyAtom::Call`：`CopyAtom` 直接持有 `Op + trait + extras`，外部 `Copy()` 一次调用直达 `Op::Copy`。
- 保留 `arch::Tensor2Tensor::Run` 这一硬件分发层，避免它把"if AIV/AIC"逻辑也塞进算法层。

---

### P3-4【中等】Mmad/Copy 的 `with(...)` 调用约定不一致

**位置** `atom/cube/mmad.h:40` vs `arch/cube/gm_to_l1/copy.h:42` 等

**问题** 有 `with` 的 atom 走 `XxxWith::Copy(args..., params)`，无 `with` 的 atom 走 `CopyOp::Copy<Trait, trait, Args...>(args...)`。两套调用约定。

**修改建议** 配合 P3-1 改造：永远走 `Op::Copy<Trait>(tensors..., extras...)`，extras 由 `with(...)` 注入。

---

### P3-5【轻】`atom/copy_atom_impl.h` 一次 include 全部 atom

**位置** `impl/tensor_api/atom/copy_atom_impl.h:25-39`

**问题** 9 个 atom 头一次性带入，编译时间和耦合都高。

**修改建议**
- 拆成 `atom/cube/all.h`、`atom/vector/all.h`，并提供细粒度的 `atom/cube/copy_gm2l1.h`、`atom/cube/mmad.h` 等显式 include。
- `tensor.h` 顶层 include 改为按用户场景的子集（如 `tensor_cube.h` / `tensor_vector.h`）。

---

## 四、横切问题

### P4-1【中等】`if ASCEND_IS_AIC/AIV` 散落在每个 atom

**位置** `gm_to_l1/copy.h:42`、`l1_to_ub/copy.h:44`、`l0c_to_gm/copy.h:77`、`mmad/mmad.h:45` …

**问题** 运行时短路 + `__aicore__ inline`，难以做 host-side mock，未来扩到 mix 模式或测试需要每个 atom 改。

**修改建议**
- 引入 `template <class ExecPolicy>` 在 atom 层，由 `ExecPolicy::is_cube`/`is_vector` 编译期判断；
- host 测试时用 `MockExecPolicy` 替换，让 atom 走可观测的桩函数。

---

### P4-2【轻】`Crd2IdxITT` 多个分支揉在一处

**位置** `impl/tensor_api/tensor/coord_index.h:40-56`

**修改建议** 拆成 `Crd2IdxFlat` / `Crd2IdxZeroFast` / `Crd2IdxGeneral` 三个具名 helper，可读性大幅提升，且后续支持负 stride / stride normalization 时不会爆 if-else。

---

### P4-3【严重】`AttrInfo::Row/Column` 把 2D 假设硬编进 arch 工具

**位置** `impl/tensor_api/arch/utils/arch_utils.h:73-103`

**问题** `GetElement<info1, info2, dim>(layout)` 强假设 layout 是 (Shape|Stride, Row|Col)，并对 depth==2/depth!=2 各写一份。一旦支持 3D/batch，整个 arch utils 要重写。

**修改建议**
- 抛弃 `Row/Column` tag，改用 `Get<I, J...>(layout.shape())`：调用方明确给出维度索引。
- 如果业务上确实需要 "row/col" 这种语义，让上层 helper 显式转成 `Get<0, ...>` / `Get<1, ...>`，不下沉到通用工具。

---

### P4-4【轻】`AscendC::Te` 自封装一层 `Get/GetTuple/GetValue`

**位置** `impl/tensor_api/tensor/tuple_impl.h:111-206`

**问题** 这些只是 `Std::get` 的薄封装，但所有 layout 已经用 `Std::tuple`，再封一层只增加心智负担。

**修改建议**
- 直接暴露 `Std::get<I>` 与 `Std::tuple_size_v` 给 `Layout` 内部使用；
- 仅保留语义有差的辅助（如 `SelectTuple` / `GetTuple` 在标量 vs tuple 上有 normalization）；
- `Get<I0, I1, Is...>` 多级取值改成 fold expression 一行。

---

## 五、推荐实施顺序

按"边际收益 / 风险"排序：

| 阶段 | 项目 | 估时 | 依赖 |
|---|---|---|---|
| 1 | 修复 `LocalTensor::Compose/Tile` 的死代码（删除或加 static_assert） | 0.5d | — |
| 1 | `Std::Int` 改 signed；`MakeLayout(shape)` 默认布局方向统一 | 1d | — |
| 1 | `StaticLayoutSize` 改递归通用 cosize | 1d | P1-3 |
| 2 | 实现 `Coalesce / Flatten / Composition / LogicalDivide` | 3-5d | 阶段 1 |
| 2 | 引入 `Underscore _`，重写 `LocalTensor::operator()` | 2-3d | 阶段 1 |
| 2 | 所有 routing fallback 加 `static_assert` | 0.5d | — |
| 3 | 统一 `with(...)` 注入路径，删除 `XxxWith` 影子子类 | 2-3d | 阶段 2 |
| 3 | 拆 `AttrInfo::Row/Col`，arch utils 改通用 `Get<I>` | 2d | 阶段 2 |
| 3 | `MakeMemPtr` 拆分为命名工厂，去全局依赖 | 1d | — |
| 4 | 实现 `LogicalProduct / Complement / RightInverse`（CuTe 高级算子） | 3-5d | 阶段 2 |
| 4 | 引入 `ExecPolicy`，atom 去运行时分支 | 2-3d | 阶段 3 |

阶段 1 是"立刻能修的硬伤"；阶段 2 是"对标 CuTe 的能力补全"；阶段 3、4 是"形态收敛 + 长期可维护"。

---

## 附：与 CuTe 的等价对照表（建议作为后续设计的 north star）

| 概念 | CuTe | 本项目（建议） |
|---|---|---|
| 静态整型 | `Int<v>` / `C<v>`（int） | `Std::C<v>`（int），`Std::Int` 兼容别名 |
| Shape/Stride | `IntTuple` | `Std::tuple`（保持） |
| Layout | `Layout<Shape, Stride>` | `Layout<Shape, Stride, Info>` |
| 取值 | `layout(coord)` | `layout(coord)`（保持） |
| 切片 | `tensor(_, i, _)` | `tensor(_, i, _)`（新增 `_`） |
| 组合 | `composition(A, B)` | `Composition(A, B)`（新增） |
| 切分 | `logical_divide / zipped_divide` | `LogicalDivide / ZippedDivide`（新增） |
| 复制原子 | `Copy_Atom<Op, T>` + `with(args)` | `CopyAtom<Op, Trait>` + `with(args)`（去 `OpWith`） |
| 矩阵乘原子 | `MMA_Atom<Op>` + `with` | `MmadAtom<Op, Trait>` + `with`（去 `MmadOpWith`） |
| 指针 | `make_gmem_ptr<T>(p)` 等 | `MakeGmemPtr<T>(p)` 等（拆分工厂） |
| 缓存 hint | atom 模板参数 | `atom.with(CacheMode::...)` |
