# Reg矢量计算编程<a name="ZH-CN_TOPIC_0000002509863891"></a>

> 📌 **提示**：Reg矢量计算编程仅在Ascend 950PR/Ascend 950DT支持

本文介绍如何在AscendC中使用Reg矢量计算API编写寄存器级向量计算代码。该能力面向已经熟悉基础AscendC编程模型的开发者，适用于需要进一步减少UB读写、压缩关键路径访存开销、并愿意显式管理寄存器Load/Compute/Store流程的高性能场景。 

·如果熟悉CUDA kernel、CUTLASS/CuTe中的register fragment，或者CPU侧AVX-512、Arm SVE等SIMD intrinsics，可以将Reg矢量计算理解为AscendC中更贴近硬件的向量寄存器编程方式：数据从UB加载到寄存器，在寄存器内完成计算，再写回UB。

## 适用场景<a name="section1825793122916"></a>

在前置的资料中可以学习到AscendC向量编程为Membase矢量接口，即操作数据对象为LocalTensor，实际物理位置为Unified Buffer（UB）。新增Regbase矢量接口并不是替代Membase，而是面向不同开发层次需求。

| 维度 | 基础API（Membase） | Reg矢量计算API（Regbase） |
| --- | --- | --- |
| 编程对象 | `LocalTensor` | `RegTensor`等寄存器对象 |
| 数据位置 | UB中的本地张量 | 向量寄存器 |
| 中间结果 | 必须回写UB | 可保留在寄存器中连续消费 |
| 使用成本 | 较低 | 较高，需要理解寄存器、掩码等 |
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
        ^
        |
        | 显式DataCopy
        v
Unified Buffer (UB)            类比CUDA Shared Memory或Local shared buffer
        ^
        |
        | Reg Load / Store
        v
VF Reg（RegTensor等）           类比CUDA Register File或SIMD vector register
        ^
        |
        | Vector instruction
        v
Vector Unit
```

SIMD架构不支持从全局内存GM直接加载数据到Reg矢量计算寄存器。数据必须先从GM搬运至Unified Buffer，再通过显式的Load/Store类搬运接口在UB和寄存器之间实现数据移动。该路径与CUDA Tensor Core编程中常见的`GMEM -> SMEM -> Register Fragment -> MMA`思路相似，只是对应的硬件层级和编程接口不同。

**图 1**  SIMD Reg矢量计算内存层级<a name="fig597318712265"></a>  
![](../../../../figures/aiv_simd_regbase_memory_level.png "SIMD-Reg矢量计算内存层级")

### 执行关系

SIMD Reg矢量计算编程架构中，向量计算执行单元从寄存器读取数据，完成计算后将结果写回寄存器；向量搬运单元负责在Registers和LocalTensor之间搬运数据。


**图 2**  SIMD Reg矢量计算编程架构<a name="fig116711840123520"></a>  
![](../../../../figures/SIMD-Reg矢量计算编程架构.png "SIMD-Reg矢量计算编程架构")

### 调用层级<a name="section61258392540"></a>

在Regbase架构中，Reg矢量计算API既可以被算子实现直接调用，也可以作为基础API或高阶API的内部实现手段；Reg矢量计算API本身直接调用编译器BuiltIn API。

![](../../../../figures/API-调用-regbase.png)

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

Reg矢量计算API操作的基础对象是寄存器。它们在C++代码中表现为变量，编译后映射到硬件寄存器或相关控制资源。具体API请参考[Reg矢量计算](https://gitcode.com/horming/asc-devkit/tree/master/docs/api/SIMD-API/%E5%9F%BA%E7%A1%80API/Reg%E7%9F%A2%E9%87%8F%E8%AE%A1%E7%AE%97。

| 类型 | 作用 |
| --- | --- |
| `RegTensor<T>` | 保存VL长度的`T`数据类型向量元素，是Reg矢量计算API的输入或输出 |
| `MaskReg` | 描述哪些元素参与本轮计算，常用于处理尾块 |
| `AddrReg` | 保存本轮访问UB的地址偏移，适合循环中按stride递增 |
| `UnalignRegForLoad` | 非对齐读取的辅助寄存器 |
| `UnalignRegForStore` | 非对齐写入的辅助寄存器 |

## Reg矢量计算API<a name="section1227863453815"></a>

一个Regbase实现通常分为外层`__aicore__`逻辑和内层`__simd_vf__`逻辑。

外层`__aicore__`代码负责：

1. 从队列中获取输入`LocalTensor`。
2. 为输出分配`LocalTensor`。
3. 单轮循环可以并行计算的元素个数`oneRepeatSize`和总共需要的循环次数`repeatTimes`。
4. 使用`asc_vf_call`调用VF函数。
5. 完成输出入队和输入释放。

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
    uint32_t offset = i * oneRepeatSize; 
    Load(srcReg, srcAddr + offset);       // UB -> Reg
    Compute(dstReg, srcReg, ..., mask);   // Reg -> Reg
    Store(dstAddr + offset, dstReg);         // Reg -> UB
}
```

基于寄存器的编程模型每次循环将一个`VL`长度的数据从LocalTensor加载到寄存器中，在寄存器中完成计算后再搬出到LocalTensor。与在LocalTensor之间反复搬运中间结果相比，该方式可以减少UB访问并提升热点片段性能。

![](../../../../figures/微指令编程流程图.png)

## 编程示例<a name="section_add_example"></a>

下面的VF函数实现`dst[i] = src0[i] + src1[i]`。它展示了Regbase中最常见的模式：循环外声明寄存器，循环内按`VL`加载、计算和写回。

```cpp
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr,
                              __ubuf__ T* src0Addr,
                              __ubuf__ T* src1Addr,
                              uint32_t oneRepeatSize,
                              uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        uint32 offset = i * oneRepeatSize;
        AscendC::Reg::Load(srcReg0, src0Addr + offset);
        AscendC::Reg::Load(srcReg1, src1Addr + offset);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::Store(dstAddr + offset, dstReg);
    }
}
```

理解这段代码时，重点关注以下几点：

- `srcReg0`、`srcReg1`和`dstReg`在循环外声明，便于编译器将其作为持续复用的寄存器资源处理。
- `CreateMask` 创建数据类型为T的MaskReg。
- `Load`、`Add`和`Store`明确表达了`UB -> Reg -> Reg -> UB`的数据路径。

`Add`接口原型如下：

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Add(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
```

其中，模板参数`T`表示操作数数据类型，`MaskMergeMode`表示未被mask选中的元素在`dstReg`中置零或保留原值。这个行为类似AVX-512中的zeroing/merging masking。

如果计算链路更长，例如`dst = (a + b) * c + d`，Regbase的价值会更明显：中间结果可以留在寄存器中继续被`Mul`、`Add`等指令消费，而不是每一步都回写UB。

下面示例串起从`__aicore__`外层逻辑到`__simd_vf__`寄存器计算的完整路径。

```cpp
// VF函数：在寄存器域完成向量加法。
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr,
                              __ubuf__ T* src0Addr,
                              __ubuf__ T* src1Addr,
                              uint32_t oneRepeatSize,
                              uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> src0Reg;
    AscendC::Reg::RegTensor<T> src1Reg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();

    for (uint16_t i = 0; i < repeatTimes; ++i) {
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
    uint16_t repeatTimes = count / oneRepeatSize;

    __ubuf__ T* dstAddr = (__ubuf__ T*)dst.GetPhyAddr();
    __ubuf__ T* src0Addr = (__ubuf__ T*)src0.GetPhyAddr();
    __ubuf__ T* src1Addr = (__ubuf__ T*)src1.GetPhyAddr();

    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr,
                          oneRepeatSize, repeatTimes);

    outQueueZ.EnQue(dst);
    inQueueX.FreeTensor(src0);
    inQueueY.FreeTensor(src1);
}
```

参数含义如下：

| 参数 | 含义 |
| --- | --- |
| `dstAddr`、`src0Addr`、`src1Addr` | UB中的物理地址，由`LocalTensor::GetPhyAddr()`获取 |
| `count` | 业务需要计算的有效元素数 |
| `oneRepeatSize` | 单轮向量寄存器可处理的元素数，即`VL / sizeof(T)` |
| `repeatTimes` | 循环轮数，即`count / oneRepeatSize` |

## 特性描述<a name="features_list"></a>

### 对齐搬运

对齐搬运用于UB地址满足对齐要求(UB地址可被32整除)的场景，是RegTensor和UB之间数据移动的首选方式。常用接口包括`LoadAlign`和`StoreAlign`，分别完成`UB -> RegTensor`和`RegTensor -> UB`的数据搬运。与通用搬运`Load`和`Store`相比，性能开销更低。

在连续数据场景中，通常按`oneRepeatSize`递增地址，每轮处理一个VL长度的数据：

```cpp
for (uint16_t i = 0; i < repeatTimes; ++i) {
    uint32_t offset = i * oneRepeatSize;
    AscendC::Reg::LoadAlign(srcReg, srcAddr + offset);
    ... // RegTensor计算
    AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
}
```

如果访问模式包含固定步长或多维索引，也可以使用带`dataBlockStride`、`repeatStride`的接口形态。

### 非对齐搬运

非对齐搬运用于UB访问地址不能被32整除，但仍需要在UB和RegTensor之间连续读写数据的场景。Regbase通过`UnalignRegForLoad`和`UnalignRegForStore`作为临时缓存，保存跨对齐边界的数据，从而降低连续非对齐访问的额外开销。

非对齐搬入一般由`LoadUnAlignPre`和`LoadUnAlign`组合完成：先初始化`UnalignRegForLoad`，再连续搬入数据。非对齐搬出一般由`StoreUnAlign`和`StoreUnAlignPost`组合完成：循环中连续写出，循环结束后调用一次后处理接口，将缓存中尚未写出的尾部数据刷回UB。

推荐写法是把初始化和后处理放在循环外，只在循环体内保留连续搬运和计算：

```cpp
AscendC::Reg::UnalignRegForLoad loadUreg;
AscendC::Reg::UnalignRegForStore storeUreg;

AscendC::Reg::LoadUnAlignPre(loadUreg, srcAddr);
for (uint16_t i = 0; i < repeatTimes; ++i) {
    AscendC::Reg::LoadUnAlign(srcReg, loadUreg, srcAddr, postUpdateStride);
    // RegTensor计算
    AscendC::Reg::StoreUnAlign(dstAddr, dstReg, storeUreg, postUpdateStride);
}
AscendC::Reg::StoreUnAlignPost(dstAddr, storeUreg, 0);
```

连续非对齐场景中优先使用`uint32_t`类型管理步长并配合post update模式，让接口在搬运后自动更新UB地址。

### PostUpdate

PostUpdate表示搬运接口在完成本次访问后自动更新UB地址指针，适合连续访问场景。它可以减少循环内显式计算`addr + i * stride`的标量指令数量，也能让地址更新语义和搬运动作绑定在一起，便于编译器识别连续访问模式。

Reg数据搬运接口中常见的post update写法包括：

```cpp
AscendC::Reg::LoadAlign<T, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(
    srcReg, srcAddr, postUpdateStride);

AscendC::Reg::StoreAlign<T, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(
    dstAddr, dstReg, postUpdateStride, mask);
```

对于`LoadUnAlign`，post update模式下实际搬运起始地址为`srcAddr`，搬运后执行`srcAddr += postUpdateStride`。对于`StoreUnAlign`，`POST_MODE_NORMAL`与`POST_MODE_UPDATE`下`postUpdateStride`含义不同：前者表示本次搬运数量且不更新地址，后者表示本次搬运数量并在搬运后执行地址更新。连续非对齐搬运推荐使用post update模式。

### AddrReg

`AddrReg`是Regbase中用于保存UB访问地址偏移的地址寄存器，适合循环中存在固定stride、多维索引或重复寻址表达式的场景。它的作用是把地址偏移相关的标量计算转化为寄存器化的地址管理，减少VF循环内反复计算地址带来的标量指令开销。

在对齐搬运场景中，编译器可以从规则的地址表达式中识别寻址模式，并有机会生成`AddrReg`相关优化。例如多层循环中按固定常量步长访问UB时，代码可以先写成普通地址表达式：

```cpp
for (uint16_t i = 0; i < extent1; ++i) {
    for (uint16_t j = 0; j < extent2; ++j) {
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * stride0 + j * stride1);
    }
}
```

当代码结构满足地址寄存器生成模式时，编译器可将其优化为使用`AddrReg`管理偏移：

```cpp
AscendC::Reg::AddrReg aReg;
for (uint16_t i = 0; i < extent1; ++i) {
    for (uint16_t j = 0; j < extent2; ++j) {
        aReg = AscendC::Reg::CreateAddrReg(i, stride0, j, stride1);
        AscendC::Reg::LoadAlign(srcReg, srcAddr, aReg);
    }
}
```

使用建议如下：

- 规则连续访问优先使用普通指针递增或post update模式，避免为了使用`AddrReg`引入额外复杂度。
- 多维固定stride访问可以保留清晰的地址表达式，让编译器优先做模式匹配和自动优化。
- 连续非对齐搬运不建议用`AddrReg`存储偏移量，推荐使用`uint32_t`步长配合post update模式。
- 只有在接口形态明确需要地址寄存器，或地址表达式复用能显著减少循环内标量计算时，再显式声明和传递`AddrReg`。

### MaskReg

`MaskReg`用于描述本轮Reg矢量计算中哪些元素参与计算，位宽为`VL / 8`。在计算接口和部分搬运接口中，`MaskReg`通常作为最后一个参数传入，mask为1的位置参与本轮操作，mask为0的位置是否清零或保留旧值由接口的`MaskMergeMode`决定。

`MaskReg`的常见生成方式有两种：

- 固定模式：`CreateMask<T, MaskPattern::...>()`，适合整VL或规律性选择。
- 动态尾块：`UpdateMask<T>(remainCount)`，适合循环末尾不足一个VL的场景。

`MaskPattern`中常用的模式包括`ALL`、`VL1`、`VL2`、`VL3`、`VL4`、`VL8`、`VL16`、`VL32`、`VL64`、`VL128`、`M3`、`M4`、`H`、`Q`和`ALLF`。其中`ALL`表示所有元素有效，`ALLF`表示所有元素无效。`UpdateMask`会把剩余元素数作为输入/输出参数，并在生成当前轮mask后自动减去一个VL对应的元素个数，因此如果后续还需要使用原始总数，建议单独保留一份计数变量。

对于`b64`或`complex32`等特殊场景，还需要结合`RegTrait`选择`RegTraitNumOne`或`RegTraitNumTwo`，确保`MaskReg`覆盖范围与`RegTensor`一致。

固定VL场景的写法如下：

```cpp
template <typename T>
__simd_vf__ inline void add_full_vf(__ubuf__ T* dstAddr,
                                    __ubuf__ T* src0Addr,
                                    __ubuf__ T* src1Addr,
                                    uint32_t oneRepeatSize,
                                    uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        uint32_t offset = i * oneRepeatSize;
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + offset);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + offset);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
    }
}
```

尾块场景的写法如下：

```cpp
template <typename T>
__simd_vf__ inline void add_tail_vf(__ubuf__ T* dstAddr,
                                    __ubuf__ T* src0Addr,
                                    __ubuf__ T* src1Addr,
                                    uint32_t count,
                                    uint32_t oneRepeatSize,
                                    uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    uint32_t remainCount = count;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        AscendC::Reg::MaskReg mask = AscendC::Reg::UpdateMask<T>(remainCount);
        uint32_t offset = i * oneRepeatSize;
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + offset);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + offset);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
    }
}
```

如果mask未选中全部元素，`MaskMergeMode::ZEROING`会让未选中的元素在结果中清零；如果需要保留`dstReg`原值，则应选用对应的`MERGING`语义接口。

### VF融合

VF融合是编译器将多个控制流等价的VF函数合并为一个VF函数的优化能力。融合后可以减少VF调用和Hardware Loop启动开销，并有机会消除多个VF之间冗余的`Load`、`Store`和同步指令，使中间结果尽量保留在寄存器中继续消费。

编译器会在融合前检查控制流是否等价、Main侧中间代码是否能够放入VF内执行、融合后是否有正收益，以及是否会导致传参寄存器溢出或VF代码过大。满足条件时，编译器会自动执行融合，并在需要的位置保守插入同步指令保证语义一致。

为了增加VF融合机会，建议遵循以下写法：

- 多个VF保持相同或等价的循环结构，尽量使循环满足Hardware Loop规范。
- 优先使用基础API的连续计算模式，减少复杂高维切分参数对编译器分析的影响。
- 避免在VF内直接访问类成员变量，必要值先在`__aicore__`侧取出为局部变量再传入VF。
- 控制寄存器使用量，避免融合后超过寄存器资源限制。

### 指令并行

指令并行依赖硬件同时发射和执行多条互不依赖指令的能力。两条指令能够并行执行通常需要满足两个条件：指令之间没有数据依赖关系，并且硬件执行资源充足。Regbase代码中，如果连续多条计算指令都依赖上一条指令的结果，执行队列会形成较长的依赖链，难以充分利用硬件并行能力。

优化思路是在保持语义不变的前提下，为执行队列提供更多互不依赖的指令。例如，先加载多个独立数据块，再分别计算；或者对循环做适度展开，让不同迭代之间的`LoadAlign`、计算和`StoreAlign`交错出现。对于依赖链很长的计算，也可以合理拆分VF循环，必要时将阶段性结果写回UB，以换取更好的指令调度空间。

同时需要控制VF内寄存器数量。一个VF中硬件可同时处理的`RegTensor`数量有限，`MaskReg`、`UnalignRegForLoad`和`UnalignRegForStore`也有数量限制；超过限制后编译器会插入换入换出和同步指令，反而降低性能。因此，指令并行优化应同时关注依赖关系和寄存器压力。

### 硬件循环

硬件循环（Hardware Loop）是VF循环优化的重要基础。VF中的循环如果满足硬件识别规范，编译器可以将其转换为硬件级循环，减少软件循环中的计数、比较和跳转开销。无法识别为Hardware Loop的循环会退化为Software Loop，性能通常较差，也会影响VF融合等后续优化。

常见编码要求如下：

- 循环迭代变量使用`uint16_t`类型。
- 循环起始值从0开始，步长为1。
- 循环边界在执行过程中不被修改。
- 循环体内避免`if/else`、三目运算符等运行时跳转控制流；可用`if constexpr`处理编译期分支，尾块场景可用`for (uint16_t i = 0; i < hasTail; ++i)`表达可选执行。
- VF内最多支持有限层级的嵌套循环，复杂控制流应拆分或前移到`__aicore__`侧。

典型Hardware Loop写法如下：

```cpp
for (uint16_t i = 0; i < repeatTimes; ++i) {
    AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
    AscendC::Reg::Add(dstReg, srcReg, biasReg, mask);
    AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
}
```

### 流水线同步控制<a name="section1633824511379"></a>

在SIMD VF函数中，执行单元和搬运流水可以并行工作。因此，某些UB访问顺序需要显式同步。判断重点不是代码书写顺序，而是硬件流水之间是否已经建立必要的数据依赖。

| 场景                                   | 是否需要同步      | 原因                                   |
| -------------------------------------- | ----------------- | -------------------------------------- |
| 同一寄存器内的写后读                   | 通常不需要        | 寄存器依赖由硬件按指令顺序保证         |
| 不同寄存器访问同一UB地址，且存在写后读 | 需要`LocalMemBar` | 搬运和计算执行单元之间没有自动顺序约束 |
| 不同寄存器访问同一UB地址，且存在写后写 | 需要`LocalMemBar` | 需要保证写入顺序                       |
| 地址无重叠、寄存器无依赖               | 通常不需要        | 不存在数据竞争                         |

![](../../../../figures/同步流水.png)

函数原型：

```cpp
template <MemType src, MemType dst>
__simd_callee__ inline void LocalMemBar()
```

经验上，只要出现以下情况，就应主动检查是否需要同步：

- 源地址和目标地址可能重叠。
- 同一段UB数据在循环内先写后读。
- 不同寄存器对象围绕同一UB区域交替Load/Store。
- 为了优化流水而重排了Load、Compute、Store顺序。

可以将`LocalMemBar`类比为CUDA异步拷贝路径中的`cp.async.wait`或某些场景下的同步栅栏。它不是为了“让代码更保险”而随意添加，而是为了给硬件流水补上必要的数据依赖。

### MergeMode

`MaskMergeMode`用于描述带mask的Reg计算中，未被mask选中的元素如何写入目标寄存器。它影响的是`dstReg`中无效lane的结果，而不是mask本身。常见取值如下：

| 取值 | 含义 |
| --- | --- |
| `MaskMergeMode::ZEROING` | mask未选中的元素在`dstReg`中置零，是多数接口的默认模式。 |
| `MaskMergeMode::MERGING` | mask未选中的元素保留`dstReg`原值，适合需要保留旧结果的尾块或条件更新场景。 |
| `MaskMergeMode::UNKNOWN` | 当转换类型不支持MaskMergeMode模式时使用。 |

以`Add`为例，接口模板参数中可以指定merge模式：

```cpp
AscendC::Reg::Add<T, AscendC::Reg::MaskMergeMode::ZEROING>(
    dstReg, srcReg0, srcReg1, mask);

AscendC::Reg::Add<T, AscendC::Reg::MaskMergeMode::MERGING>(
    dstReg, srcReg0, srcReg1, mask);
```

使用`MERGING`时，`dstReg`在调用前应包含需要保留的旧值；否则未选中lane会保留未定义或不符合预期的数据。对于只关心有效元素、尾块之外的无效元素不再被消费的场景，使用默认的`ZEROING`更简单。
