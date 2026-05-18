# 	Reg矢量计算编程<a name="ZH-CN_TOPIC_0000002600000001"></a>

> 📌 **提示**：Reg矢量编程仅在Ascend 950PR/Ascend 950DT进行SIMD编程矢量运算时支持。

本文介绍如何在Ascend C中使用基础API编写寄存器级**向量计算代码**，该能力面向已经熟悉基础Ascend C编程模型的开发者。这种基于硬件的寄存器编程方式称为Reg矢量计算编程。

## 1 为什么需要Reg矢量编程<a name="section_part1"></a>

### 1.1 Memory矢量计算的瓶颈

在以往的SIMD矢量编程中，矢量计算单元的源操作数和目的操作数都直接落在LocalMemory（Unified Buffer，以下简称UB）上。每一条矢量计算的执行过程都包含"从UB读 → 在执行单元算 → 写回UB"。当一个算子由多个矢量计算串联组成时（例如`dst = (a + b) * c + d`），每一步的中间结果都必须先写回UB，再被下一条指令重新读出。

这种模式下存在三个典型瓶颈：

- **UB读写带宽被反复占用**：中间结果在UB上往返，访存压力随着矢量计算增加而线性放大。
- **UB Bank 冲突严重**：对UB的反复读写，极大增加了UB Bank冲突的概率。
- **关键路径被访存拉长**：单此矢量计算，哪怕计算指令本身很短，整体耗时仍然被UB读写时间主导。

这种"每步落地UB"的开销会成为优化的天花板。

### 1.2 Reg矢量计算的定位

为了突破上述Memory矢量编程瓶颈限制，Ascend 950PR/Ascend 950DT在硬件中新增了**SIMD Register File**（以下简称寄存器）这一级存储；Reg矢量编程正是面向这一级寄存器的编程方式。其核心思想是：将一段连续的向量计算保留在寄存器中完成，仅在进入和退出这段计算时与UB交互一次，中间结果可不再回写UB。

Reg矢量计算与Memory矢量计算的对比如下：

| 维度 | Memory矢量计算 | Reg矢量计算 |
| --- | --- | --- |
| 数据位置 | UB | SIMD Registers |
| 中间结果 | 必须回写UB | 可在寄存器中连续消费 |
| 使用成本 | 较低 | 较高（需要理解寄存器等相关操作） |
| 适用目标 | 快速实现、不追求极致性能 | 追求极致性能、可接受更高代码复杂度 |

**在Ascend 950PR/Ascend 950DT此类开放寄存器编程的硬件情况下，为了与前序代际硬件保持兼容，同时提供Memory矢量计算方式，其实现为软件封装的Reg矢量计算**

### 1.3 适用场景

当以下条件同时满足时，建议考虑使用Reg矢量编程：

1. 目标设备支持Reg矢量编程。
2. 核心耗时集中在连续矢量计算上。
3. 中间结果在UB中存在明显的反复读写。
4. 追求极致性能收益，可接受更高的代码复杂度。

## 2 硬件原理<a name="section_part2"></a>

### 2.1 SIMD Register File与执行单元

在Ascend 950PR/Ascend 950DT中，AI Core被拆分为AIC（矩阵计算）和AIV（向量计算）两个独立的核，配比为1:2。AIV内部参与Reg矢量编程的硬件单元包括：

- **Reg矢量计算执行单元**：从寄存器读取数据，完成计算后将结果写回寄存器。在Ascend 950PR/Ascend 950DT中，单指令每拍可处理256Bytes的数据。
- **Reg矢量搬运单元**：负责在寄存器和UB之间搬运数据。
- **Aux Scalar单元**：处理Reg矢量计算执行单元和Reg搬运单元所需的标量计算（例如地址计算）。

Reg矢量计算单元、Reg矢量搬运单元和Aux Scalar单元属于不同的硬件执行单元，但在实际执行时都归属PIPE.V流水。这一组织方式带来两个直接结果：

1. 有寄存器依赖时，由硬件按指令顺序保证数据依赖正确性，用户无需显式插入同步；但跨寄存器对同一UB区域的读写需要显式插入同步：搬运单元与计算单元之间没有自动顺序约束，详见[3.4 流水线同步](#section_sync)。
2. Reg矢量计算单元和Reg矢量搬运单元个单元在没有数据依赖时可以**同时发射、并行执行**。

**图 1** SIMD Reg矢量执行关系  
![](../../../../figures/Reg执行单元.png "Reg执行单元")

### 2.2 内存层级

AIV的内存层级如下：

**图 2** 内存层级关系  
![](../../../../figures/Reg内存层级.png "Reg内存层级")

在如上的SIMD架构中，UB和寄存器均为每个AIV核内独享的存储空间。其中寄存器不支持从GM直接加载数据或者直接将数据写出到GM。当需要读取GM输入数据到寄存器时，必须先由通过DataCopy从GM搬运到UB，再通过Reg矢量搬运API将UB上的数据加载到寄存器。当需要写出寄存器输出数据到GM时，必须先由Reg矢量搬运API将寄存器上的数据存储到UB上，再通过DataCopy从UB搬运到GM。

**寄存器作为整体使用**：寄存器无法按索引或偏移访问其中的某个元素或者bit。根据不同的功能，主要分为五类：

1. **矢量计算寄存器**：参与矢量计算的主要寄存器，单个寄存器宽度定义为Vector Length（一下简称VL），可以通过Reg矢量搬运单元与UB进行数据交换。
2. **掩码寄存器**：用于控制参与矢量计算的有效元素或者代表单bit元素参与计算等，单个寄存器宽度定义为 VL/8, 可以通过Reg矢量搬运单元与UB进行数据交换。
3. **地址寄存器**：辅助Reg矢量搬运单元完成矢量计算寄存器和UB数据交换，存储UB地址的偏移，单个寄存器宽度为32bis，不能直接与UB进行数据交换。
4. **搬入非对齐寄存器**：辅助Reg矢量搬运单元完成UB数据加载到矢量计算寄存器，临时存储包含源非对齐UB地址数据的，起始对齐UB地址的1个DataBlock长度的数据，单个寄存器宽度定义为DataBlock，可以通过Reg搬运单元将UB进行数据载入到搬入非对齐寄存器。
5. **搬出非对齐寄存器**：辅助Reg矢量搬运单元矢量计算寄存器数据存储到UB，临时存储包含需写出非对齐UB地址的对应的矢量寄存器的数据，单个寄存器宽度定义为DataBlock，可以通过Reg搬运单元将搬出非对齐寄存器数据存储到UB。



## 3 编程模型<a name="section_part3"></a>

### 3.1 总体结构

基于寄存器（Regbase）的编程模型是在Memory矢量编程的`CopyIn` -> `Compute` -> `CopyOut` 编程模型基础上扩展为`CopyIn` -> `Load` -> `Compute`  ->`Store` -> `CopyOut` 的Reg矢量计算编程模型。

**图 3** Regbase编程总体结构  
![](../../../../figures/微指令编程流程图.png "Regbase编程总体结构")

### 3.2 VF函数与执行域

Reg矢量计算编程模型中`Load` -> `Compute`  ->`Store` 的VF-Compute的过程，在实现中定义为Vector Function（一下简称VF函数）。为了区分不同的执行域，将在不同执行域的函数使用不同的函数标签，编译期就限制了API的调用关系：

| 标签 | 角色 | 调用方 | 可调用对象 |
| --- | --- | --- | --- |
| `__aicore__` | Device侧普通函数 | 核函数或其他`__aicore__`函数 | `__aicore__`函数，或通过`asc_vf_call`调用VF函数 |
| `__simd_vf__` | VF函数 | `__aicore__`函数通过`asc_vf_call`调用 | `__simd_callee__`修饰的Reg矢量API或者VF内部子函数 |
| `__simd_callee__` | VF内部子函数 | `__simd_vf__`或其他`__simd_callee__` | `__simd_callee__` |

> 📌 **提示**：VF函数内只能调用Reg矢量接口和标量计算；Reg矢量接口只能在VF函数内被调用。

这种执行域隔离带来三方面好处：

- **编译期校验**：跨域错误调用（例如在`__aicore__`中直接调用Reg矢量API）会在编译阶段被发现。
- **职责清晰**：`__aicore__`负责UB、队列、地址等外层逻辑；`__simd_vf__`专注寄存器级计算。
- **优化边界明确**：编译器可以针对VF函数做独立优化（如VF融合），且不影响外层算子结构。

**图 4** 函数标签调用关系  
<img src="../../../../figures/reg_calculation_call_hierarchy.png" title="调用层级" style="zoom: 33%;" />

一个完整的实现由"外层`__aicore__`逻辑"和"内层`__simd_vf__`逻辑"两部分组成：

- **外层`__aicore__`代码**：完成核外GM数据交互等准备工作，并通过`asc_vf_call`调用VF函数。
- **内层`__simd_vf__`代码**：完成UB与寄存器之间的搬运以及寄存器上的矢量计算。



调用VF函数的典型骨架如下：

```cpp
template <typename T>
__aicore__ inline void Compute()
{
    // 1. 准备输入/输出数据。
    // ...

    // 2. 通过asc_vf_call进入寄存器级VF函数域。
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr,
                          oneRepeatSize, repeatTimes);

    // 3. 收尾工作。
    // ...
}
```

VF函数内部通常遵循如下骨架：

```text
for (i = 0; i < repeatTimes; ++i) {
    1. 计算本轮UB地址偏移。
    2. 从UB Load到寄存器。
    3. 在寄存器上执行计算。
    4. 将结果Store回UB。
}
```

### 3.3 寄存器类型与申请

寄存器在代码中表现为变量，编译后由编译器自动分配到硬件寄存器。

| 寄存器类型 | 变量定义 | Ascend 950PR/Ascend 950DT |
| --- | --- | --- |
| 矢量数据寄存器 | `RegTensor` | VL = 256B |
| 掩码寄存器 | `MaskReg` | VL/8 = 32B |
| 地址寄存器 | `AddrReg` | 32bits = 8B |
| 搬入非对齐寄存器 | `UnalignRegForLoad` | DataBlock = 32B |
| 搬出非对齐寄存器 | `UnalignRegForStore` | DataBlock = 32B |

#### 3.3.1 矢量数据寄存器（RegTensor）

`RegTensor`是Reg矢量计算的基本操作数，按数据类型包含不同元素个数：

- `b8`（int8/uint8/fp8等）：256个元素
- `b16`（half/bfloat16/int16等）：128个元素
- `b32`（float/int32等）：64个元素
- `b64`（int64/complex64等）：32个元素

对于`b64`或`complex32`等特殊场景，可以通过`RegTrait`选择`RegTraitNumTwo`，使一个`RegTensor`包含两个VL长度的矢量寄存器。

```c++
// 申请矢量数据寄存器，定义数据类型为float，为1个VL长度
Reg::RegTensor<float> reg0;

// 申请矢量数据寄存器，定义数据类型为complex64，且为两个VL长度
Reg::RegTensor<complex64, Reg::RegTraitNumTwo>> reg0;
```



#### 3.3.2 掩码寄存器（MaskReg）

`MaskReg`宽度为`VL/8`。最小对应关系为1bit掩码对应1Byte数据；当数据位宽大于1Byte时，由每个N bytes元素对应Nbits位置中的LSB（最低位）bit生效。如下表格中对应RegTensor不同元素宽度时，第几个元素生效对应MaskReg中的第几bit。

| RegTensor单元素宽度 | 0th bit | 1st bit | 2nd bit | 3rd bit | 4th bit | 5th bit | 6th bit | 7th bit | 8th bit | 9th bit | 10th bit | 11th bit | 12th bit | ...  |
| ------------------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | ------- | -------- | -------- | -------- | ---- |
| b8                  | 0       | 1       | 2       | 3       | 4       | 5       | 6       | 7       | 8       | 9       | 10       | 11       | 12       | ...  |
| b16                 | 0       | -       | 1       | -       | 2       | -       | 3       | -       | 4       | -       | 5        | -        | 6        | ...  |
| b32                 | 0       | -       | -       | -       | 1       | -       | -       | -       | 2       | -       | -        | -        | 3        | ...  |
| b64                 | 0       | -       | -       | -       | -       | -       | -       | -       | -       | 1       | -        | -        | -        | ...  |

> 📌 **提示**：b64数据部分接口为软件封装接口，对应MaskReg生效索引与b32相同，具体需要参考支持b64对应API描述。

`MaskReg`既可作为计算接口的mask输入，也可承载比较类运算的结果，还可作为移位、逻辑、交织等操作的源/目的参数，以及搬入/搬出接口的目的/源参数。

`MaskReg`常见的生成方式有两种：

```cpp
// 固定模式：适合整VL或规律性选择
AscendC::Reg::MaskReg mask =
    AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();

// 动态尾块：适合循环末尾不足一个VL的场景
AscendC::Reg::MaskReg mask = AscendC::Reg::UpdateMask<T>(remainCount);
```

`MaskPattern`包含`ALL`、`VL1`、`VL2`、`VL4`、`VL8`、`VL16`、`VL32`、`VL64`、`VL128`、`M3`、`M4`、`H`、`Q`、`ALLF`等模式。`UpdateMask`将剩余元素数作为输入/输出参数，每次调用后自动减去一个VL对应的元素数；如后续仍需原始总数，应另行保留。

当用作1bit元素参与运算时，支持RegTensor和MaskReg的混合计算以及MaskReg和MaskReg的计算

```
// RegTensor和MaskReg的混合计算作为比较接口输出
AscendC::Reg::Compare<float, CMPMODE::NE>(maskreg0, srcReg0, srcReg1, maskReg);

// MaskReg和MaskReg计算
AscendC::Reg::And(maskreg0, maskreg1, maskreg2, maskReg);
```

#### 3.3.3 地址寄存器（AddrReg）

`AddrReg`用于保存UB访问的地址偏移量。它把原本需要在循环体内反复执行的标量地址计算转化为寄存器化的地址管理，最多支持四维偏移叠加。`AddrReg`通过`CreateAddrReg`初始化，偏移量计算公式为：`offset = index0*stride0 + index1*stride1 + ...`。

```cpp
// 申请地址寄存器
AscendC::Reg::AddrReg aReg;

for(uint16_t i = 0;i < extent1; i++){
    for(uint16_t j = 0;j < extent2; j++){
        aReg = AscendC::Reg::CreateAddrReg<float>(i, stride0, j, stride1);
        AscendC::Reg::LoadAlign(srcReg, srcAddr, aReg);
    }
}
```

#### 3.3.4 搬入非对齐寄存器（UnalignRegForLoad）

`UnalignRegForLoad`用于实现从UB非对齐地址连续搬入RegTensor的临时缓存，首先通过`LoadUnAlignPre`初始化，将非对齐的头部数据载入`UnalignRegForLoad`，然后调用`LoadUnAlign`搬运数据。具体的使用流程可以参考后续`非对齐搬运`章节。

```
// 申请搬入非对齐寄存器
AscendC::Reg::UnalignRegForLoad ureg0;

// 初始化UnalignRegForLoad
AscendC::Reg::LoadUnAlignPre(ureg0, srcAddr);

AscendC::Reg::LoadUnAlign(srcReg, ureg0, srcAddr);
```

#### 3.3.5 搬出非对齐寄存器（UnalignRegForStore）

`UnalignRegForStore`用于实现从RegTensor存储到UB非对齐地址的临时缓存，首先通过`StoreunAlign`搬运数据，非对齐的尾部数据会保留在`UnalignRegForStore`，最后调用一次`StoreUnAlignPost`，将`UnalignRegForStore`内残余的尾部数据搬出到UB，具体的使用流程可以参考后续`非对齐搬运`章节。

```cpp
// 搬出vreg0 到非对齐UB地址dstAddr
StoreUnAlign((__ubuf__ T*&)dstAddr, vreg0, ureg0, postUpdateStride);

// 将搬出非对齐寄存器残留数据搬出到UB
Reg::StoreUnAlignPost((__ubuf__ T*&)dstAddr, ureg0, 0);
```

### 3.4 流水线同步<a name="section_sync"></a>

矢量计算单元与矢量搬运单元可以并行工作。判断是否需要同步的关键不在于代码书写顺序，而在于硬件流水之间是否已经建立必要的数据依赖：

| 场景 | 是否需要同步 | 原因 |
| --- | --- | --- |
| 同一寄存器内的写后读 | 不需要 | 寄存器依赖由硬件按指令顺序保证 |
| 不同寄存器访问同一UB地址，存在写后读 | 需要 | 搬运和计算执行单元之间没有自动顺序约束 |
| 不同寄存器访问同一UB地址，存在写后写 | 需要 | 需要保证写入顺序 |
| 地址无重叠、寄存器无依赖 | 不需要 | 不存在数据竞争 |

显式同步通过`LocalMemBar`实现：

```cpp
template <MemType src, MemType dst>
__simd_callee__ inline void LocalMemBar();
```

`MemType`包括`VEC_STORE`、`VEC_LOAD`、`SCALAR_STORE`、`SCALAR_LOAD`、`VEC_ALL`、`SCALAR_ALL`等枚举值，用于指定需要同步的两条流水方向。

**图 5** 流水线同步示意  
![](../../../../figures/同步流水.png "流水线同步")

> 📌 **提示**：在`asc_vf_call`调用VF函数后，外层无需关注PIPE.V的Barrier；VF函数退出时，所有内部Reg矢量指令均已执行完成。

### 3.5 编程示例

下面是一个Compute调用的VF函数示例，实现`dst[i] = src0[i] + src1[i]`：

```cpp
// VF函数：在寄存器域完成向量加法
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
    AscendC::Reg::MaskReg mask =
        AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        uint32_t offset = i * oneRepeatSize;
        AscendC::Reg::LoadAlign(src0Reg, src0Addr + offset);
        AscendC::Reg::LoadAlign(src1Reg, src1Addr + offset);
        AscendC::Reg::Add(dstReg, src0Reg, src1Reg, mask);
        AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
    }
}

// 调用方：在__aicore__域准备LocalTensor和UB地址
template <typename T>
__aicore__ inline void Compute()
{
    AscendC::LocalTensor<T> dst = outQueueZ.AllocTensor<T>();
    AscendC::LocalTensor<T> src0 = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> src1 = inQueueY.DeQue<T>();

    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint32_t count = 512;
    uint16_t repeatTimes = count / oneRepeatSize;

    __ubuf__ T* dstAddr  = (__ubuf__ T*)dst.GetPhyAddr();
    __ubuf__ T* src0Addr = (__ubuf__ T*)src0.GetPhyAddr();
    __ubuf__ T* src1Addr = (__ubuf__ T*)src1.GetPhyAddr();

    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr,
                          oneRepeatSize, repeatTimes);

    outQueueZ.EnQue(dst);
    inQueueX.FreeTensor(src0);
    inQueueY.FreeTensor(src1);
}
```

## 4 关键特性<a name="section_part4"></a>

Reg矢量编程的关键特性按用途分为三大类：

| 分类 | 特性 |
| --- | --- |
| 数据搬运 | 通用搬运、对齐搬运、非对齐搬运、PostUpdate、使用地址寄存器搬运 |
| 矢量计算 | 尾块处理、MaskMergeMode |
| VF函数优化 | VF融合、Hardware Loop、指令双发 |

### 4.1 数据搬运

#### 4.1.1 通用搬运

`Load` / `Store`是不区分UB地址是否对齐的通用搬运接口，用于将连续VL数据在UB与矢量数据寄存器之间搬运，或将不超过一个VL的元素数据从矢量数据寄存器搬出到UB。

```cpp
// 搬运1个VL长度的数据，从UB地址srcAddr到矢量数据寄存器srcReg
AscendC::Reg::Load(srcReg, srcAddr);

// 搬运1个VL长度的数据，从矢量数据寄存器dstReg到UB地址dstAddr
AscendC::Reg::Store(dstAddr, dstReg);

// 搬运少于1个VL长度的数据10个元素，从矢量数据寄存器dstReg到UB地址dstAddr
AscendC::Reg::Store(dstAddr, dstReg, 10);
```

通用搬运适合需要统一处理对齐与非对齐场景的代码。如果已知地址满足32B对齐要求，并追求更高性能，应优先选择对齐搬运接口。

> 📌 **提示**：通用搬运是软件封装接口，内部实现依赖非对齐搬运的相关接口和寄存器。

#### 4.1.2 对齐搬运

`LoadAlign` / `StoreAlign`要求UB地址满足32B对齐。相比通用搬运，对齐搬运的性能开销更低，是连续对齐场景下的首选接口。

```cpp
for (uint16_t i = 0; i < repeatTimes; ++i) {
    uint32_t offset = i * oneRepeatSize;
    AscendC::Reg::LoadAlign(srcReg, srcAddr + offset);
    // RegTensor上的计算
    ...
    AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
}
```

如果访问模式存在固定步长或多维索引，也可以使用带`dataBlockStride`、`repeatStride`参数的接口形态。

#### 4.1.3 非对齐搬运

非对齐搬运用于UB访问地址不能被32整除，但仍需要在UB和RegTensor之间连续读写数据的场景。其核心思想是通过`UnalignRegForLoad` / `UnalignRegForStore`作为临时缓存，保存跨对齐边界的数据，从而摊薄连续非对齐访问的额外开销。

- **非对齐搬入**由`LoadUnAlignPre`和`LoadUnAlign`组合完成：
例如在Ascend 950PR/Ascend 950DT，当想要搬入如下UB地址为48开始的256Bytes数据时

**图 6** 非对齐搬入预期  
![](../../../../figures/Reg非对齐读1.png "Reg非对齐读1")

1. 先使用非对齐的UB地址初始化`UnalignRegForLoad`，会将UB地址截断到32B对齐地址32，再将一个DataBlock 32B数据搬入到UnalignRegForLoad
**图 7**  初始化`UnalignRegForLoad`
![](../../../../figures/Reg非对齐读2.png "Reg非对齐读2")


2. 调用`LoadUnAlign`搬运数据，实际第一步从原非对齐UB地址向上对齐到32B后，搬入240B数据，与`UnalignRegForLoad`中的部分数据拼接成预期读入256B数据后，写入RegTensor。同时将`UnalignRegForLoad`内的数据更新读入为下一次读取UB地址非对齐对应的DataBlock 32B
**图 8**  读入预期数据
![](../../../../figures/Reg非对齐读4.png "Reg非对齐读4")



- **非对齐搬出**由`StoreUnAlign`和`StoreUnAlignPost`组合完成：，循环结束后由`StoreUnAlignPost`将缓存中尚未写出的尾部数据刷回UB。
例如在Ascend 950PR/Ascend 950DT，当想要搬出如下UB地址为48开始的512Bytes数据时

**图 9** 非对齐搬出预期  
![](../../../../figures/Reg非对齐写0.png "Reg非对齐写0")

1. 先使用`StoreUnAlign`，首次先写出时，先将尾部对齐到32B的长度为240B的RegTensor数据写入UB，同时将RegTensor剩余的16B写入`UnalignRegForStore`; 

**图 10** 第一次写出预期数据
![](../../../../figures/Reg非对齐写1.png "Reg非对齐写1")

当第二次循环时，将暂存在`UnalignRegForStore`中的16B数据，与本次循环需要写出的RegTensor中的240B拼接成256B数据后，写出到UB。同时将RegTensor中剩余的16B更新写入`UnalignRegForStore`; 

**图 11** 第二次写出预期数据
![](../../../../figures/Reg非对齐写2.png "Reg非对齐写2")

2. 调用`StoreUnAlignPost`将`UnalignRegForStore`缓存中尚未写出的尾部数据刷回UB

**图 11** 剩余数据写出
![](../../../../figures/Reg非对齐写3.png "Reg非对齐写3")

推荐写法是把初始化和后处理放在循环外，循环体内只保留连续搬运和计算：

```cpp
AscendC::Reg::UnalignRegForLoad  loadUreg;
AscendC::Reg::UnalignRegForStore storeUreg;

AscendC::Reg::LoadUnAlignPre(loadUreg, srcAddr);
for (uint16_t i = 0; i < repeatTimes; ++i) {
    AscendC::Reg::LoadUnAlign(srcReg, loadUreg, srcAddr, postUpdateStride);
    // RegTensor计算
    AscendC::Reg::StoreUnAlign(dstAddr, dstReg, storeUreg, postUpdateStride);
}
AscendC::Reg::StoreUnAlignPost(dstAddr, storeUreg, 0);
```

> 📌 **提示**：连续非对齐场景下，优先使用`uint32_t`类型管理步长并配合PostUpdate模式。


#### 4.1.4 PostUpdate
PostUpdate表示搬运接口在完成本次访问后自动更新UB地址指针，适合连续访问场景。通过`PostLiteral`模板参数控制：

```cpp
enum class PostLiteral {
    POST_MODE_NORMAL,  // UB地址不自动更新
    POST_MODE_UPDATE   // UB地址作为输入输出，每次调用自动更新
};
```

使用示例：

```cpp
AscendC::Reg::LoadAlign<T, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(
    srcReg, srcAddr, postUpdateStride);

AscendC::Reg::StoreAlign<T, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(
    dstAddr, dstReg, postUpdateStride, mask);
```

PostUpdate的优势在于：

- 减少循环内显式计算`addr + i * stride`的标量指令数。
- 让地址更新语义与搬运动作绑定，便于编译器识别连续访问模式。

对于`StoreUnAlign`，`POST_MODE_NORMAL`与`POST_MODE_UPDATE`下`postUpdateStride`含义不同：前者表示本次搬运数量且不更新地址，后者表示本次搬运数量并在搬运后执行地址更新。

#### 4.1.5 使用地址寄存器搬运

当循环中存在固定stride或多维索引时，可以使用`AddrReg`管理地址偏移，让编译器把地址表达式收敛到地址寄存器：

```cpp
AscendC::Reg::AddrReg aReg;
for (uint16_t i = 0; i < extent1; ++i) {
    for (uint16_t j = 0; j < extent2; ++j) {
        aReg = AscendC::Reg::CreateAddrReg<T>(i, stride0, j, stride1);
        AscendC::Reg::LoadAlign(srcReg, srcAddr, aReg);
    }
}
```

`AddrReg`最多支持四维寻址，偏移量公式为`offset = index0*stride0 + index1*stride1 + index2*stride2 + index3*stride3`。

使用建议：

- 规则连续访问优先使用普通指针递增或PostUpdate模式，不必为了使用`AddrReg`引入额外复杂度。
- 多维固定stride访问可保留清晰的地址表达式，让编译器优先做模式匹配并自动生成`AddrReg`相关优化。
- 连续非对齐搬运不建议用`AddrReg`存储偏移量，推荐使用`uint32_t`步长配合PostUpdate模式。

### 4.2 矢量计算

#### 4.2.1 尾块处理

当待计算数据总量不是`VL`的整数倍时，最后一轮迭代需要处理不足一个VL的数据。Reg矢量编程通过`UpdateMask`实现尾块处理：每次调用根据剩余元素数生成当前轮的mask，并在生成后自动减去一个VL对应的元素数。

```cpp
template <typename T>
__simd_vf__ inline void AddTailVF(__ubuf__ T* dstAddr,
                                  __ubuf__ T* src0Addr,
                                  __ubuf__ T* src1Addr,
                                  uint32_t count,
                                  uint32_t oneRepeatSize,
                                  uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> src0Reg;
    AscendC::Reg::RegTensor<T> src1Reg;
    AscendC::Reg::RegTensor<T> dstReg;
    uint32_t remainCount = count;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        AscendC::Reg::MaskReg mask = AscendC::Reg::UpdateMask<T>(remainCount);
        uint32_t offset = i * oneRepeatSize;
        AscendC::Reg::LoadAlign(src0Reg, src0Addr + offset);
        AscendC::Reg::LoadAlign(src1Reg, src1Addr + offset);
        AscendC::Reg::Add(dstReg, src0Reg, src1Reg, mask);
        AscendC::Reg::StoreAlign(dstAddr + offset, dstReg, mask);
    }
}
```

#### 4.2.2 MaskMergeMode

`MaskMergeMode`用于描述带mask的Reg计算中，**未被mask选中的元素**如何写入目标寄存器：

| 取值 | 含义 |
| --- | --- |
| `MaskMergeMode::ZEROING` | 未选中的元素在`dstReg`中置零，是多数接口的默认模式 |
| `MaskMergeMode::MERGING` | 未选中的元素保留`dstReg`原值，适合保留旧结果的尾块或条件更新场景 |
| `MaskMergeMode::UNKNOWN` | 当转换类型不支持MaskMergeMode时使用 |

接口模板参数中指定merge模式：

```cpp
AscendC::Reg::Add<T, AscendC::Reg::MaskMergeMode::ZEROING>(
    dstReg, src0Reg, src1Reg, mask);

AscendC::Reg::Add<T, AscendC::Reg::MaskMergeMode::MERGING>(
    dstReg, src0Reg, src1Reg, mask);
```

使用`MERGING`时，`dstReg`在调用前应包含需要保留的旧值；否则未选中lane会保留未定义或不符合预期的数据。对于只关心有效元素的场景，使用默认的`ZEROING`更简单。

📌 **提示**：`MaskMergeMode::MERGING`仅在部分Reg矢量计算接口支持，具体情况需要参考单个Reg矢量计算API介绍。

### 4.3 VF函数优化

#### 4.3.1 Hardware Loop

Hardware Loop是VF循环性能优化的基础。VF中的循环如果满足硬件识别规范，编译器可以将其转换为硬件级循环，减少软件循环中的计数、比较和跳转开销。无法识别为Hardware Loop的循环会退化为Software Loop，性能显著降低，并影响后续VF融合等优化。

Hardware Loop编码规范如下：

- 循环迭代变量必须是`uint16_t`类型。
- 循环起始值从0开始，步长为1。
- 循环边界在执行过程中不被修改。
- 循环体内避免`if/else`等运行时跳转控制流。
- VF内最多支持四层级的嵌套循环。

对编译时分支和尾块场景，可使用以下方式替代`if`：

```cpp
// 编译时分支：if constexpr 无运行时开销
if constexpr (kHasBias) {
    AscendC::Reg::Add(dstReg, dstReg, biasReg, mask);
}

// 尾块场景：用for(1)替代if
uint16_t tailK = srcK % floatRepSize;
uint16_t hasTail = !!tailK;
for (uint16_t i = 0; i < hasTail; ++i) {
    // 尾块处理内容
}
```

典型Hardware Loop写法：

```cpp
for (uint16_t i = 0; i < repeatTimes; ++i) {
    AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
    AscendC::Reg::Add(dstReg, srcReg, biasReg, mask);
    AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
}
```

#### 4.3.2 VF融合

VF融合是Reg矢量计算编程最重要的优化手段。其是将多个计算以及其循环进行合并在1个VF函数内，融合后可以减少VF调用和Hardware Loop启动开销，并消除多个VF之间冗余的Load/Store和同步指令，使中间结果尽量保留在寄存器中继续消费。

VF融合可以分成两个层级：

- **VF浅融合**：将多个VF函数实现合并为一个VF函数，可以消除VF的调用开销，消除冗余的Load/Store，充分复用寄存器。
- **VF深融合**：在VF浅融合基础上，将多个计算循环进行融合，减少Hardware Loop启动开销。

为增加VF融合机会，建议遵循以下写法：

- 多个VF保持相同或等价的循环结构增加VF深度融合的机会，循环满足Hardware Loop规范。
- 控制寄存器使用量，避免融合后超过寄存器资源限制。

> 📌 **提示**：VF融合并非范围越大越好。融合粒度过大可能导致寄存器溢出，反而使性能下降。

#### 4.3.3 指令双发

指令双发依赖硬件**同时发射和执行多条互不依赖指令**的能力。Reg矢量搬运单元与Reg矢量计算单元在没有数据依赖时可以并行工作，Reg矢量计算单元的部分指令在无依赖时可以并行执行，Reg搬运单元的对齐搬入、对齐搬入和对齐搬出在无地址依赖时可以并行执行；Reg矢量编程的优化目标之一就是为执行队列提供足够多的独立指令，避免长依赖链阻塞硬件并行。

如果计算链路过长，也可以适度拆分VF循环、必要时将阶段性结果写回UB，以换取更好的指令调度空间。同时需要控制VF内寄存器数量：一个VF中可同时使用的`RegTensor`、`MaskReg`、`UnalignRegForLoad`、`UnalignRegForStore`数量都受硬件规格限制，超出后编译器会插入换入换出和同步指令，反而降低性能。
