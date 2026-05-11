# Reg矢量计算<a name="ZH-CN_TOPIC_0000002509863891"></a>

## 简介<a name="section1825793122916"></a>

Reg矢量计算API是基于RegBase架构开放的矢量计算编程接口，用户可通过该API直接操作芯片中涉及Vector计算的寄存器，从而获得更大的开发灵活性与更优的计算性能。

Reg矢量计算API与基础API在功能上相似，区别在于：基础API的输入和输出数据必须为LocalTensor，而Reg矢量计算API的输入和输出数据均为寄存器。其中，计算类API负责从指定寄存器读取数据、执行计算，并将结果写回指定寄存器；搬运类API负责完成UB（Unified Buffer）与寄存器之间的数据搬运。相较于基础API，Reg矢量计算API将数据搬运与寄存器计算的控制权交由用户自主管理，从而提供更高的开发自由度。

## Regbase编程模型<a name="section1977234218287"></a>

**基于寄存器（Regbase）**的编程模型支持用户编写和调用向量函数（Vector Function，VF）。这些函数以 `__simd_vf__` 标记，由硬件向量运算单元负责执行。在VF函数内部，所有计算操作均通过Reg矢量计算API实现，其内存层级与编程架构如[图1](#fig597318712265)所示。

在SIMD Vector的内存架构中，最靠近Vector计算单元的是VF Reg，它是SIMD的私有内存，包含多种类型的Reg矢量计算寄存器，用于存放并行处理的多个数据元素。单核内所有VF Reg寄存器共享同一本地内存资源UB。由于SIMD架构不支持直接从全局内存（Global Memory）将数据加载到寄存器，数据须先从全局内存GM搬运至Unified Buffer，再通过显式的Load/Store指令由Unified Buffer加载到Reg矢量计算寄存器中。

**图 1**  SIMD Reg矢量计算内存层级<a name="fig597318712265"></a>  
![](../../../../../figures/SIMD-Reg矢量计算内存层级.png "SIMD-Reg矢量计算内存层级")

在SIMD Reg矢量计算编程架构中，指令被发送至Reg矢量计算执行单元，执行单元从寄存器读取数据并执行计算，计算结果写回寄存器。DMA搬运单元负责在寄存器与本地内存之间搬运数据。

**图 2**  SIMD Reg矢量计算编程架构<a name="fig116711840123520"></a>  
![](../../../../../figures/SIMD-Reg矢量计算编程架构.png "SIMD-Reg矢量计算编程架构")

## Regbase与Membase编程调用层级<a name="section61258392540"></a>

在Membase架构中，基础API通过调用框架API或直接调用编译器BuiltIn API实现功能，高阶API则通过调用基础API实现功能。在Regbase架构中新增了Reg矢量计算API：用户在算子实现中可直接调用该API，高阶API和基础API也可调用该API实现功能，而Reg矢量计算API本身则直接调用编译器BuiltIn API。

两种架构在中间结果处理上存在本质差异：
- **Regbase架构**：中间计算结果可暂存于寄存器中，无需将数据搬出到本地内存，从而避免额外的搬运开销。
- **Membase架构**：所有操作均基于内存进行，每次计算都需从本地内存（Local Memory）加载数据，计算完成后须将结果写回本地内存，中间结果亦需暂存于本地内存上。

此外，两种架构在数据处理粒度上也有所不同：Regbase架构中寄存器可容纳的最大数据长度为VL（Vector Length），受寄存器容量限制，每次只能处理VL长度的数据，因此需将数据切分后分批搬入寄存器计算，计算完成后再将结果搬回本地内存；Membase架构则能直接处理完整长度的LocalTensor，无需数据切分，数据处理流程相对简单。

![](../../../../../figures/API-调用.png)

## Reg矢量计算调用层次<a name="section1048755114224"></a>

Reg矢量计算涉及以下四类函数，各类函数的调用关系和使用限制如下：

- **核函数**：以 `__global__ __aicore__` 标识，是Device侧的入口函数，Host侧通过 `<<<...>>>` 语法调用。
- **`__aicore__` 函数**：以 `__aicore__` 标识，在Device侧执行；核函数内部可调用 `__aicore__` 函数。
- **VF函数（simd vf函数）**：以 `__simd_vf__` 标记，由核函数通过 `asc_vf_call` 调用。VF函数内部只能调用 `__simd_callee__` 函数和 `constexpr aicore` 函数。
- **`__simd_callee__` 子函数**：在VF函数内部可调用的子函数，支持返回值或引用传参，以 `__simd_callee__` 标识。`__simd_callee__` 函数内部只能调用 `__simd_callee__` 函数和 `constexpr aicore` 函数。

具体调用关系如下图所示：

![](../../../../../figures/reg_calculation_call_hierarchy.png)

唯一合法的函数调用链如下：

![](../../../../../figures/simd_vf.png)

Regbase编程模型中通过 `__simd_vf__` 标记定义VF函数，这一设计方案具有以下优点：

- `__aicore__` 与 `__simd_vf__` 代码隔离清晰，编译器可对编译器BuiltIn API的使用范围进行合法性检测。
- 对函数调用进行完善的检查与报错，例如可检测在 `__simd_vf__` 内调用 `__aicore__` 函数或simt函数等非法用法。
- 用户可控制某些优化选项（如多个VF函数融合）仅针对特定函数生效，或对特定函数关闭某些优化。

以下示例展示了在 `__aicore__` 函数 `Compute` 中调用VF函数 `AddVF` 执行向量加法操作的用法：

```cpp
template <typename T> 
__aicore__ inline void Compute()      
{  
     // 申请输出队列并读取输入结果
     ...
     // 调用simd vf函数
     asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);
     // 写入结果到输出队列并释放输入队列的内存
     ...
}
```

## Reg矢量计算寄存器<a name="section1910719438134"></a>

Reg矢量计算API操作的基础数据类型介绍如下，具体API请参考[Reg矢量计算](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/context/Reg矢量计算.md)。

- **RegTensor**

    矢量数据寄存器，是Reg矢量计算的基本存储单元，用于矢量计算。RegTensor的位宽为VL（Vector Length），可存储 VL/sizeof(T) 个数据元素（T表示数据类型）。

- **MaskReg**

    掩码寄存器，用于在矢量计算中选择参与计算的元素。MaskReg的位宽为VL/8。

- **UnalignRegForLoad & UnalignRegForStore**

    非对齐寄存器，作为缓冲区，用于优化UB与RegTensor之间连续非对齐地址访问的开销。读取非对齐地址前，须先通过 `LoadUnAlignPre` 对 `UnalignRegForLoad` 进行初始化，再调用 `LoadUnAlign` 执行读取；写入非对齐地址时，须先调用 `StoreUnAlign`，再调用 `StoreUnAlignPost` 进行后处理。

- **AddrReg**

    地址寄存器，用于存储地址偏移量。通过 `CreateAddrReg` 初始化后，在循环中使用AddrReg管理地址偏移量；AddrReg在每层循环中依据所设置的stride自动递增。

以下示例展示了上述寄存器类型的典型用法。示例中的 `AddVF` 函数通过 `Add` 接口实现两组数据的相加操作，通过 `MaskReg` 根据实际有效数据长度 `count` 生成掩码，控制参与运算的元素数量，并通过 `LoadAlign`/`StoreAlign` 完成UB与寄存器之间的数据搬运。

**示例一：连续对齐搬入搬出场景**（使用RegTensor、MaskReg和AddrReg）

```cpp
template<typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr,
                               uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::AddrReg aReg;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        aReg = AscendC::Reg::CreateAddrReg<T>(i, oneRepeatSize);
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr, aReg);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr, aReg);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr, dstReg, aReg, mask);
    }
}
```

**示例二：连续非对齐搬入搬出场景**（使用RegTensor、MaskReg、AddrReg、UnalignRegForLoad和UnalignRegForStore）

```cpp
template <typename T>
__simd_vf__ inline void LoadUnAlignVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                                       uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::UnalignRegForLoad ureg0;
    AscendC::Reg::UnalignRegForStore ureg1;
    AscendC::Reg::AddrReg aReg;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        aReg = AscendC::Reg::CreateAddrReg<T>(i, oneRepeatSize);
        AscendC::Reg::LoadUnAlignPre(ureg0, srcAddr, aReg);
        AscendC::Reg::LoadUnAlign(srcReg, ureg0, srcAddr, aReg, 0);
        AscendC::Reg::StoreUnAlign(dstAddr, srcReg, ureg1, aReg);
    }
    AscendC::Reg::StoreUnAlignPost(dstAddr, ureg1, aReg);
}
```

## 流水线同步控制<a name="section1633824511379"></a>

在VF函数的编写过程中，有时需要在循环中将不同的值写入同一地址，或目标地址dst与源地址src相同，此时涉及不同流水线之间的同步问题。VF函数内不同流水线之间的同步通过 `LocalMemBar` 指令实现。该指令需指定源流水线（src）和目的流水线（dst），目的流水线将等待源流水线上的所有指令执行完毕后才继续执行，如下图所示。

在写读场景下，若写指令与读指令使用相同的寄存器，硬件可触发寄存器保序，指令将按代码顺序执行，无需插入同步指令；若写指令与读指令使用不同的寄存器，则须插入同步指令以确保两条指令按序执行。写写场景的处理规则与此相同。

![](../../../../../figures/同步流水.png)

函数原型：

```cpp
template <MemType src, MemType dst> 
__simd_callee__ inline void LocalMemBar()
```

## 如何使用Reg矢量计算API<a name="section1227863453815"></a>

基于寄存器的编程模型的核心流程是：每次循环从LocalTensor中取出VL长度的数据，通过数据搬运指令加载到寄存器，在寄存器中完成所有计算逻辑后，再将结果搬出到LocalTensor。由于所有计算均在寄存器中完成，可显著减少LocalTensor之间的数据搬运次数，从而大幅提升整体性能。具体流程如下图所示：

![](../../../../../figures/微指令编程流程图.png)

以 `AddVF` 函数为例说明使用方法：首先定义三个矢量数据寄存器 `srcReg0`、`srcReg1` 和 `dstReg` 以及掩码寄存器 `mask`；每次循环使用数据搬运函数将VL长度的数据分别从 `src0`（起始地址 `src0Addr + i * oneRepeatSize`）和 `src1`（起始地址 `src1Addr + i * oneRepeatSize`）搬入寄存器 `srcReg0` 和 `srcReg1`；随后调用 `Add` 函数将计算结果存入 `dstReg`（即 `dstReg = srcReg0 + srcReg1`），其中 `mask` 控制参与计算的元素个数；最后调用数据搬运函数将 `dstReg` 中的结果搬出到 `dst`。

`Add` 的原型定义如下：

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Add(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
```

其中，模板参数 `T` 表示操作数的数据类型；`MaskMergeMode` 控制 `mask` 未筛选元素在 `dst` 中的处理方式，可置零或保留原值；`UpdateMask` 函数用于更新参与计算的掩码元素，每次循环消耗VL长度的元素。`LoadAlign(srcReg0, src0Addr + i * oneRepeatSize)` 表示将数据从LocalTensor搬入寄存器 `srcReg0`，起始地址为 `src0Addr + i * oneRepeatSize`；`StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask)` 表示将 `dstReg` 的数据搬出到LocalTensor，目标地址为 `dstAddr + i * oneRepeatSize`，`mask` 控制参与搬出的元素数量。

## Reg矢量计算编程示例<a name="section13119114173116"></a>

以 `Add` 函数为例，`AddVF` 是以 `__simd_vf__` 标记的VF函数，包含6个参数：`dstAddr` 为输出数据地址，`src0Addr` 和 `src1Addr` 为输入数据地址，`__ubuf__` 类型表示用于矢量计算的本地内存（Unified Buffer），是LocalTensor实际存储的物理位置；`count` 为参与计算的输入元素个数，`repeatTimes` 为循环次数，`oneRepeatSize` 为每次循环处理的数据量。

调用方 `Compute` 函数首先计算每次可搬入寄存器的数据量 `oneRepeatSize` 和循环次数 `repeatTimes`，再通过 `GetPhyAddr` 获取输入和输出数据的UB物理地址，最终通过 `asc_vf_call<AddVF<T>>` 调用VF函数执行计算。

```cpp
// VF函数（SIMD VF）
template <typename T>
__simd_vf__ inline void AddVF(
    __ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr,
    uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> src0Reg;
    AscendC::Reg::RegTensor<T> src1Reg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(src0Reg, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(src1Reg, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Add(dstReg, src0Reg, src1Reg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

// 调用方：__aicore__ 函数
template <typename T>
__aicore__ inline void Compute()
{
    AscendC::LocalTensor<T> dst = outQueueZ.AllocTensor<T>();
    AscendC::LocalTensor<T> src0 = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> src1 = inQueueY.DeQue<T>();
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint32_t count = 512;
    // 向上取整，计算循环次数
    uint16_t repeatTimes = AscendC::CeilDivision(count, oneRepeatSize);
    __ubuf__ T* dstAddr = (__ubuf__ T*)dst.GetPhyAddr();
    __ubuf__ T* src0Addr = (__ubuf__ T*)src0.GetPhyAddr();
    __ubuf__ T* src1Addr = (__ubuf__ T*)src1.GetPhyAddr();
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);
    outQueueZ.EnQue(dst);
    inQueueX.FreeTensor(src0);
    inQueueY.FreeTensor(src1);
}
```
