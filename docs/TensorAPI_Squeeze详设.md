# TensorAPI Squeeze能力详细设计

## 1 需求及场景分析

### 1.1 需求背景

TensorAPI使用Layout的Shape和Stride描述Tensor的逻辑维度与存储布局。在batch切片、广播或分形Layout处理过程中，Shape中可能出现大小为1的冗余维度。例如，对batch Layout中的单个batch进行Slice后，Shape会从`(B, M, K)`变为`(1, M, K)`。后续接口如果期望二维Layout，用户需要重新手工构造Layout，使用不便且容易配错Stride或LayoutPattern。

因此，TensorAPI需要提供Squeeze能力，删除Shape中大小为1的指定维度，同时保留未删除维度的Stride、原LayoutPattern和LayoutTrait。Squeeze只修改元数据视图，不搬移数据，不修改Tensor的起始地址。

### 1.2 典型场景

1. **单batch视图**：将`(1, M, K)`压缩为`(M, K)`，便于复用非batch通路。
2. **Slice后维度归一**：对多batch Tensor执行Slice后，删除Slice Shape中已经变为1的batch维度。
3. **分形Layout解包**：将`(1, ((C0, M1), (C0, N1)))`压缩为`((C0, M1), (C0, N1))`，同时保留`NZLayoutPtn`、`NNLayoutPtn`等Pattern。
4. **嵌套Shape定点压缩**：对`(1, (1, (M, K)))`等嵌套Shape，按与Shape同构的Pattern递归指定需要删除的位置。
5. **Tensor视图转换**：直接对Tensor执行Squeeze，返回复用原Engine的新Tensor，避免用户手工拆分和重建Tensor。

### 1.3 设计目标

- 支持Layout和Tensor两类输入。
- 支持按顶层维度索引和按同构Pattern两种调用方式。
- 支持一次删除多个大小为1的维度。
- 支持嵌套tuple Shape，Pattern方式可递归处理嵌套维度。
- 全部结构转换通过模板在编译期完成，不增加数据搬运和运行时分支。
- 保留原Layout的Pattern和Trait；Tensor输入同时保留Engine和Memory Location。

### 1.4 非目标

- 不调整Tensor数据排布，不进行内存拷贝。
- 不自动删除所有大小为1的维度，只处理用户指定的维度。
- 不在运行时校验动态维度的实际值是否为1。
- 不改变原Layout或Tensor对象，接口返回新的视图对象。

## 2 方案概述

### 2.1 对外接口

接口声明位于`include/tensor_api/tensor/layout.h`，命名空间为`AscendC::Te`。

```cpp
template <size_t... SqueezeDims, typename T, typename Enable>
__aicore__ inline constexpr auto Squeeze(const T& x);

template <typename Pattern, typename T, typename Enable>
__aicore__ inline constexpr auto Squeeze(const T& x, const Pattern& pattern);
```

两种接口的输入和返回规则如下。

| 输入 | 返回值 | 处理规则 |
| :--- | :--- | :--- |
| Layout | 新Layout | 删除Shape和Stride中对应的维度，保留Pattern和Trait |
| Tensor | 新Tensor | 先生成新Layout，再复用原Tensor的Engine和Location构造新Tensor |

#### 2.1.1 按维度索引压缩

`Squeeze<SqueezeDims...>(x)`通过模板参数指定顶层Shape中待压缩维度的索引。索引顺序不影响结果，可一次指定多个索引。

```cpp
auto layout = MakeLayout(MakeShape(_1{}, _4{}, _1{}));
auto squeezed = Squeeze<0, 2>(layout); // Shape: (4)
```

该方式只匹配顶层维度。对于`(1, ((16, 2), (16, 4)))`，`Squeeze<0>`删除batch维，内层分形结构整体保留。

#### 2.1.2 按Pattern压缩

`Squeeze(x, pattern)`接收与Shape嵌套结构完全相同的Pattern，使用`_1{}`标记待压缩位置，使用`_`标记保留位置。

```cpp
auto layout = MakeLayout(MakeShape(_1{}, _4{}, _1{}));
auto squeezed = Squeeze(layout, MakeCoord(_1{}, _, _1{})); // Shape: (4)
```

Pattern方式支持递归处理嵌套Shape。

```cpp
auto layout = MakeLayout(
    MakeShape(_1{}, MakeShape(_1{}, MakeShape(_2{}, _3{}))),
    MakeStride(_6{}, MakeStride(_6{}, MakeStride(_3{}, _1{}))));

auto squeezed = Squeeze(
    layout, MakeCoord(_1{}, MakeCoord(_1{}, MakeCoord(_, _)))); // Shape: (2, 3)
```

### 2.2 维度可压缩判定

维度是否可压缩由Shape元素的类型决定。

| Shape元素类型 | 处理方式 |
| :--- | :--- |
| 编译期常量`Std::Int<1>` | 删除该维度 |
| 编译期常量`Std::Int<N>`，`N != 1` | 保留该维度，不报错 |
| 运行时整数类型 | 按用户指定删除，不生成运行时校验 |

对于运行时维度，接口信任调用者已保证实际值为1。如果实际值不为1仍执行Squeeze，新Layout将丢失该逻辑维度，可能导致后续访存范围与用户预期不一致。

### 2.3 总体处理流程

```text
Squeeze(Layout/Tensor)
        |
        +-- Tensor输入：取出Tensor.Layout()
        |
        +-- 索引方式：构造DimsMask -> 生成KeepIndexSeq
        |
        +-- Pattern方式：检查同构 -> 递归构造PatternMask -> 生成KeepIndexSeq
        |
        +-- 按KeepIndexSeq同步选择Shape和Stride
        |
        +-- 必要时解包单元素嵌套tuple
        |
        +-- 使用原Pattern/Trait构造新Layout
        |
        +-- Tensor输入：复用原Engine构造新Tensor
```

### 2.4 实现分层

| 层次 | 文件 | 职责 |
| :--- | :--- | :--- |
| 对外接口 | `include/tensor_api/tensor/layout.h` | 声明两种Squeeze接口 |
| Pattern标记 | `impl/tensor_api/utils/constant_impl.h` | 定义`KeepMark`和保留标记`_`，`_1{}`复用`Std::Int<1>` |
| tuple工具 | `impl/tensor_api/tensor/tuple_impl.h` | 生成保留索引序列、按序列选择tuple元素、检查Shape和Pattern是否同构 |
| Layout处理 | `impl/tensor_api/tensor/layout_impl.h` | 生成压缩掩码，处理索引模式和递归Pattern模式，构造新Layout |
| Tensor适配 | `impl/tensor_api/tensor/tensor_impl.h` | 识别Layout/Tensor输入，对Tensor复用Engine并构造新Tensor |

### 2.5 关键编译期元语

#### 2.5.1 索引模式

1. `IsInPack<I, Dims...>`判断顶层索引`I`是否被用户指定。
2. `SqueezableSize<T>`根据Shape元素类型判断该维是否可压缩。
3. `DimsMask`为每个顶层维度生成保留或删除的布尔掩码。当索引被指定且该维可压缩时，对应掩码为`false`。
4. `KeepIndexSeqFromSeq`将布尔掩码转换为需要保留的`index_sequence`。
5. `SelectBySeq`使用同一序列选择Shape和Stride，保证二者结构一致。

#### 2.5.2 Pattern模式

1. `IsSameStructureV<Pattern, Shape>`递归检查两者每一层是否同为tuple且元素数相同。
2. `IsSqueezeMarkV<T>`判断Pattern叶子是否为`_1{}`。非`_1{}`位置均保留。
3. `SqueezeRecNode`遇到tuple节点时继续递归，遇到叶子时返回Shape/Stride对。
4. `PatternMask`在当前层根据Pattern标记和Shape元素类型生成保留掩码。
5. 每一层都使用同一掩码同步选择Shape和Stride，从而保持两者同构。

### 2.6 结果Layout构造

新Layout通过`MakePatternLayout<OriginalPattern, OriginalTrait>`构造，不重新推导Pattern和Trait。这保证下游Copy、Mmad等依赖LayoutPattern分发的接口在Squeeze后仍能选择原通路。

压缩后如果只剩一个嵌套tuple，则自动解包一层。例如：

```text
(1, (M, K))                       -> (M, K)
(1, ((C0, M1), (C0, K1)))         -> ((C0, M1), (C0, K1))
(4, 1)                            -> (4)
```

如果Pattern递归处理后得到标量叶子，实现会将其重新包装为单元素tuple，以保持Layout对象的结构。

### 2.7 Tensor适配

对Tensor输入，接口仅对`x.Layout()`执行Squeeze，然后使用原Engine构造新Tensor：

```text
newLayout = SqueezeLayout(x.Layout())
newTensor = Tensor{x.Engine(), newLayout}
```

因此：

- `newTensor.Data() == x.Data()`；
- Engine类型和Memory Location不变；
- 未删除维度的Stride不变；
- 该过程不产生数据读写。

### 2.8 约束与异常处理

1. 输入类型必须是TensorAPI Layout或带属性的Tensor类型，其他类型不参与模板匹配。
2. 索引模式至少需要指定一个维度索引。
3. Pattern必须为tuple类型，且与Shape的嵌套结构完全相同；不同构时通过`static_assert`报编译错误。
4. Pattern中建议仅使用`_1{}`和`_`；只有`_1{}`具有压缩语义，其他标记会被当作保留。
5. 编译期明确不为1的维度不会被删除，也不报错。
6. 运行时维度由调用者保证实际值为1，接口不进行运行时保护。
7. 索引模式对超出顶层rank的索引不会命中任何维度，当前实现不额外报错；调用代码应保证索引在合法范围内。

### 2.9 性能与兼容性

- 掩码生成、tuple选择和返回类型均在编译期确定。
- 接口不调用底层指令，不增加额外数据搬运开销。
- 对运行时Shape，构造新Layout只保留所需Shape/Stride字段，不增加维度值判断。
- Squeeze不修改已有Layout/Tensor接口语义，为新增的独立接口。

## 3 测试用例场景说明

### 3.1 测试范围

测试从返回类型、Shape、Stride、Pattern/Trait、Tensor Engine和编译期约束六个方面验证Squeeze。现有正向UT位于`tests/api/tensor_api/basic_struct/layout/test_tensor_api_squeeze.cpp`。

### 3.2 正向功能用例

| 编号 | 场景 | 输入与操作 | 预期结果 | 对应现有UT |
| :--- | :--- | :--- | :--- | :--- |
| S01 | 索引模式删除单维 | `(4, 1, 5)`，`Squeeze<1>` | Shape为`(4, 5)`，Stride为`(5, 1)` | `TestSqueezeSingleDim` |
| S02 | 索引模式删除多维 | `(1, 4, 1)`或`(1, 4, 1, 5)`，`Squeeze<0, 2>` | 指定的大小为1的维度被删除，rank正确减少 | `TestSqueezeMultipleDims`、`TestSqueezeRankChange` |
| S03 | 索引顺序无关 | 分别调用`Squeeze<0, 2>`和`Squeeze<2, 0>` | 返回rank、Shape和Stride一致 | `TestSqueezeUnorderedDims` |
| S04 | 非1静态维保留 | `(4, 5)`，指定大小为4或5的维度 | rank不变，Shape和Stride不变 | `TestSqueezeNoOpWhenSizeNotOne`、`TestSqueezeStaticNonOneKept` |
| S05 | 平铺Pattern压缩 | `(1, 4, 1)`，Pattern为`(_1, _, _1)` | Shape为`(4)` | `TestSqueezePatternFlat` |
| S06 | Pattern全部保留 | `(4, 5)`，Pattern为`(_, _)` | Layout不变 | `TestSqueezePatternKeepAll` |
| S07 | Pattern标记非1静态维 | `(4, 5)`，Pattern为`(_1, _)` | 维度4保留，不报错 | `TestSqueezePatternMarkNonOne`、`TestSqueezePatternStaticNonOneKept` |
| S08 | 两种模式等价 | 对同一Layout分别按索引和Pattern压缩 | rank、Shape和Stride一致 | `TestSqueezeModeEquivalence` |
| S09 | 嵌套batch维压缩 | `(1, ((16, 2), (16, 3)))`，`Squeeze<0>` | 解包为`((16, 2), (16, 3))`，内层Stride不变 | `TestSqueezeNestedBatchDim` |
| S10 | 嵌套Pattern压缩 | 对batch分形Layout使用同构Pattern | 删除batch维，保留分形结构 | `TestSqueezeNestedPattern` |
| S11 | 多层递归Pattern | `(1, (1, (2, 3)))`，外层和内层的1均标记`_1` | 返回`(2, 3)` | `TestSqueezeDoubleNestedProbe` |
| S12 | 嵌套分支递归 | `((1, 4), (1, 5))`，两个内层维度1均标记`_1` | 返回`(4, 5)` | `TestSqueezeInnerOnesProbe` |
| S13 | 单元素tuple解包 | `(1, (8, 16))`，删除batch维 | 返回`(8, 16)` | `TestSqueezeUnwrapBatch` |
| S14 | 保留rank 1 Layout | `(4, 1)`，删除维度1 | 返回`(4)`，不解包成标量 | `TestSqueezeScalarNotUnwrapped` |
| S15 | 保留LayoutPattern | 对batch NZ Layout分别执行两种Squeeze | 返回Layout的Pattern仍为`NZLayoutPtn` | `TestSqueezePreservesPatternDims`、`TestSqueezePreservesPatternMode2` |
| S16 | 运行时维度 | Shape元素为运行时`int`，实际值为1 | 指定维度被删除，不产生运行时校验 | `TestSqueezeRuntimeDim`、`TestSqueezeRuntimeMultipleDims`、`TestSqueezeRuntimePattern`、`TestSqueezeRuntimeNoValidation` |
| S17 | 静态和动态维度混合 | Shape同时包含`_1{}`和运行时`1` | 两类指定维度均被删除 | `TestSqueezeMixedStaticAndRuntime` |
| S18 | `MakeFrameLayout`单batch | `MakeFrameLayout<NZLayoutPtn>(1, 32, 64)`后删除batch维 | Shape/Stride正确，Pattern为`NZLayoutPtn` | `TestSqueezeMakeFrameLayoutBatchOne` |
| S19 | `Slice + Squeeze`端到端 | 对多batch NN Tensor执行Slice，再删除两个大小为1的batch维 | Tensor偏移由Slice完成；Squeeze后Shape/Stride和`NNLayoutPtn`正确 | `TestSqueezeNnTwoBatchAfterSlice` |

### 3.3 Tensor输入专项用例

当前Layout路径覆盖较完整，还需对Tensor适配层补充以下用例。

| 编号 | 场景 | 验证点 |
| :--- | :--- | :--- |
| T01 | Tensor按索引压缩 | 返回类型为Tensor，Shape/Stride正确，`Data()`与原Tensor相同 |
| T02 | Tensor按Pattern压缩 | 递归Pattern生效，新Tensor复用原Engine |
| T03 | Global/Local Tensor适配 | 分别验证GM和Local Memory Location不变 |
| T04 | Tensor Pattern/Trait保留 | 通过`static_assert`验证新Layout的Pattern/Trait与原Layout一致 |
| T05 | Slice后Tensor直接Squeeze | 验证Slice负责指针偏移，Squeeze不再改变数据指针 |

### 3.4 编译期约束用例

这类用例建议放入编译失败测试集，通过预期编译器诊断验证约束。

| 编号 | 场景 | 预期结果 |
| :--- | :--- | :--- |
| C01 | 输入不是Layout或Tensor | 无匹配的Squeeze重载 |
| C02 | 索引模式未指定任何维度 | 无匹配的Squeeze重载 |
| C03 | Pattern不是tuple | 无匹配的Pattern Squeeze重载 |
| C04 | Pattern和Shape顶层元素数不同 | 触发同构`static_assert` |
| C05 | Pattern和Shape的嵌套深度不同 | 触发同构`static_assert` |

### 3.5 回归测试

1. 运行TensorAPI basic structure UT，确认Layout、Slice、MakeFrameLayout等现有能力无回归。
2. 对`NZLayoutPtn`和`NNLayoutPtn`至少各覆盖一个Squeeze后继续调用Layout查询接口的场景。
3. 对Tensor输入执行Squeeze后，验证`Data()`、Shape、Stride、Pattern和Trait。
4. 使用`static_assert`验证返回rank和类型信息，使用`EXPECT_EQ`验证运行时Shape/Stride值。

## 4 风险与后续改进

| 风险 | 影响 | 应对方案 |
| :--- | :--- | :--- |
| 调用者对实际值不为1的运行时维度执行Squeeze | 新Layout丢失逻辑维度，后续索引语义错误 | 在接口文档和调用示例中明确调用方责任 |
| 索引越界未显式报错 | 调用结果与用户预期不符 | 后续可增加所有`SqueezeDims < rank`的编译期校验 |
| 静态非1维度被静默保留 | 可能遮蔽调用方错误 | 保持当前容错语义，后续如要改为严格模式需单独评估兼容性 |
| 极深嵌套Pattern导致模板实例化增多 | 增加编译时间 | 嵌套深度受实际Layout结构限制，不生成运行时开销；持续关注编译性能 |
| Tensor路径缺少独立UT | Engine复用、Location和返回类型可能回归 | 补充T01至T05用例 |
