# Reg矢量计算编程<a name="ZH-CN_TOPIC_0000002509863891"></a>

本文介绍如何在AscendC中使用Reg矢量计算API编写寄存器级向量计算代码。该能力面向已经熟悉基础AscendC编程模型的开发者，适用于需要进一步减少UB读写、压缩关键路径访存开销、并愿意显式管理寄存器Load/Compute/Store流程的高性能场景。 

如果熟悉CUDA kernel、CUTLASS/CuTe中的register fragment，或者CPU侧AVX-512、Arm SVE等SIMD intrinsics，可以将Reg矢量计算理解为AscendC中更贴近硬件的向量寄存器编程方式：数据从UB加载到寄存器，在寄存器内完成计算，再写回UB。

## 适用场景<a name="section1825793122916"></a>

在前置的资料中可以学习到AscendC向量编程为Membase矢量接口，即操作数据对象为LocalTensor，实际物理位置为Unified Buffer（UB）。新增Regbase矢量接口并不是替代Membase，而是面向不同开发层次需求。

| 维度 | 基础API（Membase） | Reg矢量计算API（Regbase） |
| --- | --- | --- |
| 编程对象 | `LocalTensor` | `RegTensor`等寄存器对象 |
| 数据位置 | UB中的本地张量 | 向量寄存器 |
| 中间结果 | 必须回写UB | 可保留在寄存器中连续消费 |
| 使用成本 | 较低 | 较高，需要理解寄存器、掩码、同步等 |
| 适用目标 | 快速实现、不追求极致性能 | 愿意用高复杂度编程追求性能 |

通常在以下条件同时满足时，建议考虑使用Regbase：

1. 核心耗时集中在向量计算。
2. 中间结果在UB中存在明显的反复读写。
3. 追求极致性能收益，可接受更高的代码复杂度。

Regbase更适合作为热点片段的精细化优化手段。

## Regbase编程模型<a name="section1977234218287"></a>

基于寄存器（Regbase）的编程模型支持开发者编写Vector Function（向量函数）。向量函数使用`__simd_vf__`标记，并由硬件中的向量运算单元执行。在`__simd_vf__`函数内部，开发者通过Reg矢量计算API显式完成数据搬运和寄存器计算。

### 内存层级

Reg矢量计算的数据路径如下：

```text
Global Memory (GM)             类比CUDA Global Memory
        |
        | 显式DMA
        v
Unified Buffer (UB)            类比CUDA Shared Memory或Local shared buffer
        |
        | Reg Load / Store
        v
VF Reg（RegTensor等）           类比CUDA Register File或SIMD vector register
        |
        | Vector instruction
        v
Vector Unit
```

SIMD架构不支持从全局内存GM直接加载数据到Reg矢量计算寄存器。数据必须先从GM搬运至Unified Buffer，再通过显式的Load/Store类搬运接口在UB和寄存器之间实现数据移动。该路径与CUDA Tensor Core编程中常见的`GMEM -> SMEM -> Register Fragment -> MMA`思路相似，只是对应的硬件层级和编程接口不同。

**图 1**  SIMD Reg矢量计算内存层级<a name="fig597318712265"></a>  
![](../../../../figures/aiv_simd_regbase_memory_level.png "SIMD-Reg矢量计算内存层级")

### 执行关系

SIMD Reg矢量计算编程架构中，向量计算执行单元从寄存器读取数据，完成计算后将结果写回寄存器；向量搬运单元负责在Registers和Local Memory之间搬运数据。

Reg矢量计算和Reg矢量搬运执行单元都属于Vector流水，但属于不同硬件执行单元，可以并行推进。并行带来性能空间，也带来顺序约束：当同一段UB数据被不同寄存器读写复用时，需要显式同步，避免写后读或写后写竞争。同步规则见[流水线同步控制](#section1633824511379)。

**图 2**  SIMD Reg矢量计算编程架构<a name="fig116711840123520"></a>  
![](../../../../figures/SIMD-Reg矢量计算编程架构.png "SIMD-Reg矢量计算编程架构")

## Regbase和Membase编程调用层级<a name="section61258392540"></a>

在Regbase架构中，Reg矢量计算API既可以被算子实现直接调用，也可以作为基础API或高阶API的内部实现手段；Reg矢量计算API本身直接调用编译器BuiltIn API。

![](../../../../figures/API-调用-regbase.png)


## Reg矢量计算调用层次<a name="section1048755114224"></a>

Reg矢量计算通过函数标签区分不同执行域。这些标签类似CUDA中的`__host__`、`__device__`、`__global__`，用于在编译期限制调用关系，避免将只能在某个执行域使用的BuiltIn API放到错误位置。

| 标签 | 角色 | 调用方 | 可调用对象 |
| --- | --- | --- | --- |
| `__global__ __aicore__` | 核函数入口 | Host侧通过`<<<...>>>`启动 | `__aicore__`函数 |
| `__aicore__` | Device侧普通函数 | 核函数或其他`__aicore__`函数 | `__aicore__`函数，或通过`asc_vf_call`进入VF函数 |
| `__simd_vf__` | VF函数，承载寄存器级向量计算 | `asc_vf_call<F>(...)` | `__simd_callee__`、`constexpr aicore` |
| `__simd_callee__` | VF内部子函数 | `__simd_vf__`或其他`__simd_callee__` | `__simd_callee__`、`constexpr aicore` |

具体的调用关系图如下：

![](../../../../figures/reg_calculation_call_hierarchy.png)

以下为唯一合法函数调用链：

![](../../../../figures/simd_vf.png)

这种隔离有三个目的：

- **编译期校验**：错误的跨域调用可以在编译阶段暴露，例如在`__simd_vf__`中误调用`__aicore__`函数。
- **职责清晰**：`__aicore__`负责LocalTensor、队列、地址获取等外层逻辑；`__simd_vf__`专注寄存器级计算。
- **优化边界明确**：编译器可以针对VF函数做独立优化，例如多个VF函数融合，且不影响外层算子结构。

典型调用形式如下：

```cpp
template <typename T>
__aicore__ inline void Compute()
{
    // 1. 准备输入/输出LocalTensor，并取得UB物理地址。
    // ...

    // 2. 通过asc_vf_call进入寄存器级向量计算域。
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr,
                          count, oneRepeatSize, repeatTimes);

    // 3. 完成入队、释放等收尾工作。
    // ...
}
```

## Reg矢量计算寄存器<a name="section1910719438134"></a>

Reg矢量计算API操作的基础对象是寄存器。它们在C++代码中表现为变量，编译后映射到硬件寄存器或相关控制资源。具体API请参考[Reg矢量计算](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Reg矢量计算.md)。

| 类型 | 作用 |
| --- | --- |
| `RegTensor<T>` | 保存VL长度的`T`数据类型向量元素，是Reg矢量计算API的输入或输出 |
| `MaskReg` | 描述哪些元素参与本轮计算，常用于处理尾块 |
| `AddrReg` | 保存本轮访问UB的地址偏移，适合循环中按stride递增 |
| `UnalignRegForLoad` | 非对齐读取的辅助寄存器 |
| `UnalignRegForStore` | 非对齐写入的辅助寄存器 |

### `VL`与单轮处理元素数

`VL`表示向量寄存器位宽。对类型`T`而言，单个寄存器一次可以容纳的元素数通常写作：

```cpp
constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
```

循环次数通常写作：

```cpp
uint16_t repeatTimes = AscendC::CeilDivision(count, oneRepeatSize);
```

其中，`count`是业务有效元素数，`oneRepeatSize`是单轮可处理的元素数，`repeatTimes`是完整处理`count`个元素所需的循环轮数。

## 如何使用Reg矢量计算API<a name="section1227863453815"></a>

一个Regbase实现通常分为外层`__aicore__`逻辑和内层`__simd_vf__`逻辑。

外层`__aicore__`代码负责：

1. 从队列中获取输入`LocalTensor`。
2. 为输出分配`LocalTensor`。
3. 计算`count`、`oneRepeatSize`和`repeatTimes`。
4. 通过`GetPhyAddr()`获取UB物理地址。
5. 使用`asc_vf_call`调用VF函数。
6. 完成输出入队和输入释放。

内层`__simd_vf__`函数通常遵循以下骨架：

```text
for (i = 0; i < repeatTimes; ++i) {
    1. 计算本轮UB地址偏移。
    2 从UB Load到寄存器。
    3. 在寄存器上执行计算。
    4. 将结果Store回UB。
}
```

对应伪代码如下：

```text
for (i = 0; i < repeatTimes; ++i) {
    MaskReg mask = UpdateMask(count);
    uint32_t offset = i * oneRepeatSize; 
    Load(srcReg, srcAddr + offset);       // UB -> Reg
    Compute(dstReg, srcReg, ..., mask);   // Reg -> Reg
    Store(dstAddr + offset, dstReg, mask);         // Reg -> UB
}
```

基于寄存器的编程模型每次循环将一个`VL`长度的数据从LocalTensor加载到寄存器中，在寄存器中完成计算后再搬出到LocalTensor。与在LocalTensor之间反复搬运中间结果相比，该方式可以减少UB访问并提升热点片段性能。

![](../../../../figures/微指令编程流程图.png)

## Add VF 示例<a name="section_add_example"></a>

下面的VF函数实现`dst[i] = src0[i] + src1[i]`。它展示了Regbase中最常见的模式：循环外声明寄存器，循环内按`VL`加载、计算和写回。

```cpp
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr,
                              __ubuf__ T* src0Addr,
                              __ubuf__ T* src1Addr,
                              uint32_t count,
                              uint32_t oneRepeatSize,
                              uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        uint32 offset = i * oneRepeatSize;
        AscendC::Reg::Load(srcReg0, src0Addr + offset);
        AscendC::Reg::Load(srcReg1, src1Addr + offset);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::Store(dstAddr + offset, dstReg, mask);
    }
}
```

理解这段代码时，重点关注以下几点：

- `srcReg0`、`srcReg1`和`dstReg`在循环外声明，便于编译器将其作为持续复用的寄存器资源处理。
- `UpdateMask<T>(count)`负责尾块有效元素控制，开发者不需要手写最后一轮的掩码位图。
- `Load`、`Add`和`Store`明确表达了`UB -> Reg -> Reg -> UB`的数据路径。

`Add`接口原型如下：

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Add(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
```

其中，模板参数`T`表示操作数数据类型，`MaskMergeMode`表示未被mask选中的元素在`dstReg`中置零或保留原值。这个行为类似AVX-512中的zeroing/merging masking。

如果计算链路更长，例如`dst = (a + b) * c + d`，Regbase的价值会更明显：中间结果可以留在寄存器中继续被`Mul`、`Add`等指令消费，而不是每一步都回写UB。

## Reg矢量计算编程示例<a name="section13119114173116"></a>

下面示例串起从`__aicore__`外层逻辑到`__simd_vf__`寄存器计算的完整路径。

```cpp
// VF函数：在寄存器域完成向量加法。
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr,
                              __ubuf__ T* src0Addr,
                              __ubuf__ T* src1Addr,
                              uint32_t count,
                              uint32_t oneRepeatSize,
                              uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> src0Reg;
    AscendC::Reg::RegTensor<T> src1Reg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        uint32 offset = i * oneRepeatSize;
        AscendC::Reg::Load(src0Reg, src0Addr + offset);
        AscendC::Reg::Load(src1Reg, src1Addr + offset);
        AscendC::Reg::Add(dstReg, src0Reg, src1Reg, mask);
        AscendC::Reg::Store(dstAddr + offset, dstReg, mask);
    }
}

// 调用方：在__aicore__域准备LocalTensor和UB地址。
template <typename T>
__aicore__ inline void Compute()
{
    AscendC::LocalTensor<T> dst = outQueueZ.AllocTensor<T>();
    AscendC::LocalTensor<T> src0 = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> src1 = inQueueY.DeQue<T>();

    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint32_t count = 512;
    uint16_t repeatTimes = AscendC::CeilDivision(count, oneRepeatSize);

    __ubuf__ T* dstAddr = (__ubuf__ T*)dst.GetPhyAddr();
    __ubuf__ T* src0Addr = (__ubuf__ T*)src0.GetPhyAddr();
    __ubuf__ T* src1Addr = (__ubuf__ T*)src1.GetPhyAddr();

    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr,
                          count, oneRepeatSize, repeatTimes);

    outQueueZ.EnQue(dst);
    inQueueX.FreeTensor(src0);
    inQueueY.FreeTensor(src1);
}
```

参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `dstAddr`、`src0Addr`、`src1Addr` | UB中的物理地址，由`LocalTensor::GetPhyAddr()`获取 |
| `count` | 业务有效元素数 |
| `oneRepeatSize` | 单轮向量寄存器可处理的元素数，即`VL / sizeof(T)` |
| `repeatTimes` | 循环轮数，即`ceil(count / oneRepeatSize)` |

## 开发检查清单<a name="section_checklist"></a>

将计算片段改写为Regbase前，建议逐项确认：

1. 该计算片段确实位于性能关键路径。
2. 中间结果存在明显的UB反复读写，改用寄存器驻留有收益。
3. 数据可以按`VL`切分，尾块可以用`MaskReg`表达。
4. `__aicore__`与`__simd_vf__`的职责边界清晰，没有跨域误调用。

## 常见误区<a name="section_common_pitfalls"></a>

| 误区 | 影响 | 建议 |
| --- | --- | --- |
| 只因为“更底层”就改用Regbase | 代码复杂度上升，但性能未必提升 | 先定位瓶颈，再决定是否改写 |
| 忽略尾块掩码 | 可能写越界或污染无效元素 | 使用`UpdateMask`管理每轮有效元素 |
| 把`__aicore__`逻辑写进VF函数 | 编译失败或职责混乱 | VF只承载寄存器级计算 |

## 快速回顾<a name="section_summary"></a>

Reg矢量计算API的核心思想是：**把性能关键路径中的短生命周期中间结果留在寄存器中，让开发者显式控制UB与寄存器之间的数据流动。**

实际开发时可以记住以下路径：

```text
LocalTensor -> GetPhyAddr -> asc_vf_call -> __simd_vf__
             -> Load -> Reg Compute -> Store -> LocalTensor
```

从使用者角度看，Regbase不是Membase的替代品，而是面向热点片段的精细化工具。先用Membase建立正确、清晰的算子结构，再将最值得优化的内层循环下沉到Regbase，通常是更稳妥的工程路径。
