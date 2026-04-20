# AscendC VF函数与RegBase编程详解

## 1. 什么是VF函数

VF（Vector Function）函数是AscendC中基于RegBase编程模型定义的向量函数，使用 `__simd_vf__` 关键字标记。VF函数被发送到硬件中的向量运算单元执行，函数内部通过Reg矢量计算API直接操作寄存器完成计算。

```cpp
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr,
                               uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0, srcReg1, dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

**VF函数的核心特征：**

| 特征 | 说明 |
|------|------|
| 标记关键字 | `__simd_vf__` |
| 数据地址类型 | `__ubuf__`（指向Unified Buffer） |
| 调用方式 | `AscendC::VF_CALL<VF函数名>(...)` |
| 内部操作 | 直接操作Reg矢量计算寄存器 |
| 每次处理数据量 | VL（Vector Length），通过 `GetVecLen()` 获取 |

## 2. 什么是RegBase

RegBase是AscendC中的一种**基于寄存器**的编程模型，与传统的基于内存（MemBase）的编程模型相对应。

### 2.1 MemBase vs RegBase

```
MemBase编程模型（传统方式）：
┌──────────┐    DataCopy    ┌──────────┐   基础API   ┌──────────┐
│Global Mem │ ──────────→  │Local Mem │ ──────────→ │Local Mem │
│  (GM)     │              │  (UB)    │   计算结果   │  (UB)    │
└──────────┘               └──────────┘              └──────────┘
                            ↑ 每步操作都涉及UB读写 ↓

RegBase编程模型（VF方式）：
┌──────────┐    DataCopy    ┌──────────┐  LoadAlign  ┌─────────┐
│Global Mem │ ──────────→  │Local Mem │ ──────────→ │VF Reg   │
│  (GM)     │              │  (UB)    │              │(寄存器)  │
└──────────┘               └──────────┘  ←────────── └─────────┘
                                           StoreAlign   计算在
                                                        寄存器中
                                                        完成
```

| 对比维度 | MemBase | RegBase |
|---------|---------|---------|
| 编程抽象 | 基于LocalTensor | 基于寄存器（RegTensor） |
| 数据搬运 | 框架自动管理 | 用户显式Load/Store |
| 中间结果 | 暂存到UB（需读写UB） | 暂存到寄存器（无UB开销） |
| 数据切分 | 框架处理 | 用户按VL手动切分 |
| 灵活性 | 较低 | 更高，用户自主控制 |
| 性能潜力 | 一般 | 更高，减少UB读写开销 |

### 2.2 RegBase的内存层级

```
             ┌─────────────────────────────┐
             │     Global Memory (GM)       │  ← 大容量，高延迟
             └─────────────┬───────────────┘
                           │ DMA搬运 (DataCopy)
             ┌─────────────▼───────────────┐
             │    Unified Buffer (UB)       │  ← Local Memory，__ubuf__
             └─────────────┬───────────────┘
                           │ LoadAlign / StoreAlign
             ┌─────────────▼───────────────┐
             │     VF Reg (寄存器)          │  ← 最靠近计算单元，VL宽度
             │  RegTensor / MaskReg / ...   │
             └─────────────────────────────┘
```

### 2.3 RegBase的核心寄存器类型

| 寄存器类型 | 说明 | 位宽 |
|-----------|------|------|
| `RegTensor<T>` | 矢量数据寄存器，基本存储单元 | VL |
| `MaskReg` | 掩码寄存器，控制参与计算的元素 | VL/8 |
| `UnalignRegForLoad` | 非对齐搬入缓冲寄存器 | - |
| `UnalignRegForStore` | 非对齐搬出缓冲寄存器 | - |
| `AddrReg` | 地址寄存器，存储地址偏移量 | - |

### 2.4 函数调用层次

```
核函数 (__global__ __aicore__)
  └── __aicore__ 函数
        └── VF函数 (__simd_vf__)       ← 通过 VF_CALL 调用
              └── __simd_callee__ 子函数  ← VF函数内部的子函数
```

**合法调用链**：`__aicore__` → `VF_CALL<VF函数>` → `__simd_vf__` → `__simd_callee__`

## 3. 如何基于RegBase实现一个VF函数

以GELU样例为例，完整的实现流程如下：

### 3.1 Step 1: 定义VF函数

VF函数内部直接操作寄存器完成计算：

```cpp
// gelu.asc 中的 GeluVf 函数
__simd_vf__ inline void GeluVf(__ubuf__ float* xAddr, __ubuf__ float* yAddr,
                                uint32_t n, uint32_t loopNum)
{
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(float);
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<float> xReg, yReg, tmpReg, coeffReg;

    for (uint16_t i = 0; i < loopNum; ++i) {
        mask = AscendC::Reg::UpdateMask<float>(n);        // 更新掩码
        AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);  // 从UB加载到寄存器

        // 所有计算在寄存器中完成，无需中间结果写回UB
        AscendC::Reg::Mul(tmpReg, xReg, xReg, mask);       // x²
        AscendC::Reg::Mul(tmpReg, tmpReg, xReg, mask);     // x³
        AscendC::Reg::Muls(coeffReg, tmpReg, COEFF_A, mask); // 0.044715 * x³
        AscendC::Reg::Add(tmpReg, xReg, coeffReg, mask);   // x + 0.044715 * x³
        AscendC::Reg::Muls(tmpReg, tmpReg, COEFF_B, mask); // -1.595769 * (...)
        AscendC::Reg::Exp(tmpReg, tmpReg, mask);           // e^(...)
        AscendC::Reg::Adds(tmpReg, tmpReg, 1.0f, mask);    // 1 + e^(...)
        AscendC::Reg::Div(yReg, xReg, tmpReg, mask);       // x / (1 + e^(...))

        AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);  // 从寄存器写回UB
    }
}
```

### 3.2 Step 2: 在 `__aicore__` 函数中获取UB地址并调用VF函数

```cpp
__aicore__ inline void GeluRegBaseCompute(
    const AscendC::LocalTensor<float>& xLocal, const AscendC::LocalTensor<float>& yLocal,
    const AscendC::LocalTensor<float>& tmpLocal, const AscendC::LocalTensor<float>& coeffLocal, uint32_t n)
{
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(float);
    uint32_t loopNum = AscendC::CeilDivision(n, oneRepeatSize);
    __ubuf__ float* xAddr = (__ubuf__ float*)xLocal.GetPhyAddr();  // 获取UB物理地址
    __ubuf__ float* yAddr = (__ubuf__ float*)yLocal.GetPhyAddr();
    GeluVf(xAddr, yAddr, n, loopNum);  // 调用VF函数
}
```

### 3.3 Step 3: 在Process流程中调用

```cpp
__aicore__ inline void Process()
{
    // ... 数据搬运到UB ...
    if constexpr (scenarioNum == 0) {
        GeluCompute(xLocal, yLocal, tmpLocal, coeffLocal, tileLen);   // MemBase方式
    } else if (scenarioNum == 1) {
        GeluRegBaseCompute(xLocal, yLocal, tmpLocal, coeffLocal, tileLen);  // RegBase方式
    }
    // ... 数据从UB搬出 ...
}
```

### 3.4 MemBase vs RegBase实现GELU的对比

**MemBase方式**（`GeluCompute`）：每次向量操作后需要通过 `PipeBarrier<PIPE_V>()` 同步，中间结果暂存在UB中，每次操作都涉及UB读写。

```cpp
// MemBase: 7次UB读写 + 7次PipeBarrier
AscendC::Mul(tmpLocal, xLocal, xLocal, n);           // 读UB(x)×2, 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Mul(tmpLocal, tmpLocal, xLocal, n);          // 读UB(tmp,x), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
// ... 共7步，每步都需要读写UB并同步
```

**RegBase方式**（`GeluVf`）：所有计算在寄存器中完成，中间结果暂存在寄存器中，无需PipeBarrier同步，仅需在开始Load、结束Store时访问UB。

```cpp
// RegBase: 仅1次Load + 7次寄存器计算 + 1次Store
AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);  // 从UB加载1次
AscendC::Reg::Mul(tmpReg, xReg, xReg, mask);               // 寄存器计算，无UB开销
AscendC::Reg::Mul(tmpReg, tmpReg, xReg, mask);              // 寄存器计算，无UB开销
// ... 共7步寄存器计算，无同步开销
AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);  // 写回UB 1次
```

**性能提升来源**：
- 消除了中间结果的UB读写开销
- 消除了PipeBarrier同步开销
- 寄存器访问延迟远低于UB

---

## 4. VF函数优化手段详解

### 4.1 优化一：VF循环优化（optimize_vf_loop）

#### 4.1.1 优化背景

VF函数内部的核心结构是一个循环，每次迭代处理一个VL长度的数据。循环内部的指令排列、地址管理方式、成员变量访问模式等都会影响硬件的指令发射效率。

#### 4.1.2 优化手段

本样例通过以下手段优化VF循环：

**1) 循环内地址管理优化**

使用多维嵌套循环+stride参数替代单层循环+偏移计算，让硬件更好地进行地址计算：

```cpp
// optimize_vf_loop 样例
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* srcAddr, __ubuf__ T* dstAddr,
                               uint16_t stride1, uint16_t stride2,
                               uint16_t stride3, uint16_t stride4, uint16_t loop)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::VL8>();
    for (uint16_t i = 0; i < loop; i++) {
        for (uint16_t j = 0; j < loop; j++) {
            for (uint16_t k = 0; k < loop; k++) {
                for (uint16_t m = 0; m < loop; m++) {
                    // 使用stride层级管理地址，替代手动计算偏移
                    AscendC::Reg::LoadAlign(srcReg,
                        srcAddr + i * stride1 + j * stride2 + k * stride3 + m * stride4);
                    AscendC::Reg::Adds(dstReg, srcReg, 1.0f, mask);
                    AscendC::Reg::StoreAlign(
                        dstAddr + i * stride1 + j * stride2 + k * stride3 + m * stride4,
                        dstReg, mask);
                }
            }
        }
    }
}
```

调用端配置stride层级关系（stride1 > stride2 > stride3 > stride4）：

```cpp
constexpr uint16_t loop = 4;
constexpr uint16_t stride1 = 512;        // 最外层stride
constexpr uint16_t stride2 = stride1 / loop;  // 128
constexpr uint16_t stride3 = stride2 / loop;  // 32
constexpr uint16_t stride4 = stride3 / loop;  // 8
AscendC::VF_CALL<AddVF<T>>(srcAddr, dstAddr, stride1, stride2, stride3, stride4, loop);
```

**2) 循环内指令分布优化**

通过使用固定Mask（`CreateMask<T, MaskPattern::VL8>()`）替代动态Mask（`UpdateMask`），减少了循环内的Mask更新开销。当数据量恰好是VL的整数倍时，可以使用固定Mask。

**3) 为什么能优化**

- 多层嵌套循环让编译器更容易识别数据访问模式，进行指令调度优化
- 层级化的stride计算减少运行时地址计算复杂度
- 固定Mask避免了每次循环的Mask更新指令

---

### 4.2 优化二：VF指令双发优化（optimize_vf_dual_instr）

#### 4.2.1 优化背景

昇腾AI处理器的Vector计算单元支持**指令双发**（Dual Issue），即同时发射两条独立的指令到不同的执行端口。但在单一VF循环中，如果前后指令存在数据依赖（写后读），则无法双发。

#### 4.2.2 问题场景

计算 `z = x + y + 60`，其中 `x` 和 `y` 是长度为2048的向量：

```cpp
// 单个VF循环中：x + y 的结果立即被后续 Adds 使用
// dstReg0 依赖前一条 Add 的输出 → 无法双发
AscendC::Reg::Add(dstReg0, srcReg0, srcReg1, mask);    // Add
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);        // 依赖dstReg0 → 串行
```

#### 4.2.3 优化方案

通过**拆分VF循环**，将一个长循环拆成两个独立循环，使两个循环之间的指令可以双发：

```cpp
// optimize_vf_dual_instr 样例
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* src0Addr, __ubuf__ T* src1Addr, __ubuf__ T* dstAddr,
                               uint16_t oneRepeatSize, uint16_t repeatTimes, uint32_t count)
{
    AscendC::Reg::RegTensor<T> srcReg0, srcReg1, dstReg0, dstReg1;
    AscendC::Reg::MaskReg mask;

    // 循环1: z = x + y + 30（Add + 3次Adds）
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Add(dstReg0, srcReg0, srcReg1, mask);   // x + y
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg0, mask);
    }
    // 循环2: z = z + 30（3次Adds，与循环1独立）
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(dstReg0, dstAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::Adds(dstReg0, dstReg0, 10, mask);       // + 10
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg0, mask);
    }
}
```

#### 4.2.4 为什么能优化

```
优化前：单个循环（串行执行）
  Add → Adds → Adds → Adds → Store | Add → Adds → Adds → Adds → Store | ...
  每条指令等待前一条完成

优化后：拆分为两个循环（可能双发）
  循环1: Add → Adds → Adds → Adds → Store | Add → Adds → ...
  循环2: (与循环1交替执行时) Load → Adds → Adds → Adds → Store | ...
  当循环1的Store完成与循环2的Load之间没有数据依赖时，
  硬件可以交替发射两个循环中的独立指令
```

关键思想：**适当搬出中间结果到UB**，将一个长依赖链拆成两个短依赖链，减少单条链上的串行依赖，使硬件有机会双发执行。

---

### 4.3 优化三：VF融合优化（optimize_vf_fusion）

#### 4.3.1 优化背景

当算子中需要依次调用多个VF函数时，每个VF函数独立执行，函数之间需要将中间结果写回UB再由下一个VF函数读取。这带来了额外的UB读写开销。

#### 4.3.2 问题场景

计算 `y = 1/x + 1`（先取倒数再加一），拆成两个VF函数：

```cpp
// 两个独立VF函数
AscendC::VF_CALL<DivVF<T>>(dstAddr, srcAddr, ...);   // 第1个VF: 取倒数
AscendC::VF_CALL<AddVF<T>>(dstAddr, srcAddr, ...);   // 第2个VF: 加一
```

每个VF函数内部的循环结构为 `Load → 计算 → Store`，两个VF函数之间必须通过UB传递数据。

#### 4.3.3 优化方案

编译器可以将连续调用的多个VF函数**融合**为一个VF函数：

```cpp
// optimize_vf_fusion 样例
template <typename T>
__simd_vf__ inline void DivVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                               uint16_t oneRepeatSize, uint32_t repeatTimes, uint32_t count)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<T> srcReg0, srcReg1, dstReg;
    constexpr float num = 1.0f;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Duplicate(srcReg1, num, mask);
        AscendC::Reg::Div(dstReg, srcReg1, srcReg0, mask);  // 1/x
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr,
                               uint16_t oneRepeatSize, uint32_t repeatTimes, uint32_t count)
{
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::RegTensor<T> srcReg, dstReg;
    constexpr float num = 1.0f;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(dstReg, srcReg, num, mask);      // + 1
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

// 调用端：两个VF_CALL
AscendC::VF_CALL<DivVF<T>>(dstAddr, srcAddr, oneRepeatSize, repeatTimes, count);
AscendC::VF_CALL<AddVF<T>>(dstAddr, srcAddr, oneRepeatSize, repeatTimes, count);
```

编译器会将这两个VF函数融合，效果等价于：

```cpp
// 融合后的等效VF函数
for (uint16_t i = 0; i < repeatTimes; ++i) {
    LoadAlign(srcReg0, srcAddr + i * oneRepeatSize);    // Load 1次
    Duplicate(srcReg1, 1.0f, mask);
    Div(dstReg, srcReg1, srcReg0, mask);                // 1/x（寄存器计算）
    Adds(dstReg, dstReg, 1.0f, mask);                   // +1（寄存器计算）
    StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask); // Store 1次
}
```

#### 4.3.4 为什么能优化

```
融合前：两个独立VF函数
  DivVF: [Load → Div → Store] [Load → Div → Store] ...
  AddVF: [Load → Adds → Store] [Load → Adds → Store] ...
         ↑ 需要从UB重新Load DivVF的Store结果

融合后：一个VF函数
  [Load → Div → Adds → Store] [Load → Div → Adds → Store] ...
         ↑ Div结果直接在寄存器中传给Adds，无需Store再Load
```

- 消除了DivVF和AddVF之间的UB Store/Load开销
- 减少了总体的Load/Store次数（从4次减为2次/迭代）
- 寄存器中直接传递中间结果，延迟更低

---

### 4.4 优化四：连续非对齐搬运优化（optimize_vf_continious_align）

#### 4.4.1 优化背景

当数据地址不是32字节对齐时，需要使用非对齐搬运接口 `LoadUnAlign/StoreUnAlign`。这些接口内部使用非对齐寄存器（UnalignRegForLoad/UnalignRegForStore）作为缓冲区来处理跨对齐边界的数据。

#### 4.4.2 优化方案

关键优化点：**将初始化和后处理操作移到循环外部**。

```cpp
// optimize_vf_continious_align 样例
template <typename T>
__simd_vf__ inline void LoadVF(__ubuf__ T* srcAddr, __ubuf__ T* dstAddr,
                                uint16_t postUpdateStride, uint32_t repeatTimes, uint16_t count)
{
    AscendC::Reg::RegTensor<T> srcReg, dstReg;
    AscendC::Reg::UnalignRegForLoad ureg0;
    AscendC::Reg::UnalignRegForStore ureg1;

    // 优化1：初始化移到循环外（仅执行一次）
    AscendC::Reg::LoadUnAlignPre(ureg0, srcAddr);

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        AscendC::Reg::LoadUnAlign(srcReg, ureg0, srcAddr + i * postUpdateStride);
        AscendC::Reg::StoreUnAlign(dstAddr, srcReg, ureg1, postUpdateStride);
    }

    // 优化2：后处理移到循环外（仅执行一次）
    AscendC::Reg::StoreUnAlignPost(dstAddr, ureg1, 0);
}
```

#### 4.4.3 为什么能优化

**非对齐寄存器的工作原理**：

```
LoadUnAlignPre: 缓存UB地址前32B的数据到ureg（仅首次需要）
LoadUnAlign:    从UB搬入数据 + 与ureg拼接 → 写入RegTensor
                同时缓存后续未对齐数据到ureg（供下次使用）
                → 因此连续搬入时不需要再次调用Pre

StoreUnAlign:   将RegTensor数据写入UB + 缓存尾部数据到ureg
StoreUnAlignPost: 将ureg中缓存的尾部数据写入UB（仅最后需要）
                  → 因此连续搬出时不需要每次调用Post
```

| 操作 | 循环内执行（反例） | 循环外执行（正例） |
|------|-----------------|-----------------|
| `LoadUnAlignPre` | 每次循环1次 → N次 | 仅1次 |
| `StoreUnAlignPost` | 每次循环1次 → N次 | 仅1次 |

当 `repeatTimes` 较大时，节省的指令数非常可观。

---

## 5. 两个文件夹核心本质的对比

### 5.1 gelu_high_performance 文件夹

**核心思想**：用RegBase编程范式替代MemBase编程范式，通过VF函数实现GELU计算。

| 维度 | MemBase方式 | RegBase方式 |
|------|-----------|-----------|
| 函数 | `GeluCompute` | `GeluVf` + `GeluRegBaseCompute` |
| 数据容器 | LocalTensor | RegTensor + `__ubuf__`指针 |
| 中间结果存储 | UB（需PipeBarrier同步） | 寄存器（无需同步） |
| 计算模式 | 7次UB读+7次UB写+7次PipeBarrier | 1次Load+7次寄存器计算+1次Store |
| 同步开销 | 每步需 `PipeBarrier<PIPE_V>()` | 无需PipeBarrier |

**本质**：展示RegBase相比MemBase的性能优势——中间计算结果在寄存器中流转，避免反复读写UB和同步等待。

### 5.2 optimize_vf_loop 等文件夹

**核心思想**：在已经使用RegBase/VF函数的基础上，进一步优化VF函数内部的执行效率。

| 样例 | 优化维度 | 具体手段 |
|------|---------|---------|
| optimize_vf_loop | 循环结构 | 多维嵌套循环+层级stride、固定Mask |
| optimize_vf_dual_instr | 指令调度 | 拆分VF循环减少数据依赖，使能双发 |
| optimize_vf_fusion | 函数融合 | 多个VF函数编译器融合为一个，消除UB中间读写 |
| optimize_vf_continious_align | 搬运效率 | LoadUnAlignPre/StoreUnAlignPost提到循环外 |

**本质**：展示VF函数内部的微架构级优化——指令调度、循环结构、数据搬运的细粒度优化。

### 5.3 核心本质是否一样？

**两者共享相同的技术基础**：都是基于RegBase编程范式，使用 `__simd_vf__` VF函数 + Reg矢量计算API。

**但优化层次不同**：

```
优化层次（从宏观到微观）：

1. 编程范式选择
   ┌──────────────────────────────────────────┐
   │  MemBase → RegBase                        │ ← gelu_high_performance 的重点
   │  （从LocalTensor编程切换到寄存器编程）        │
   └──────────────────────────────────────────┘

2. VF函数内部优化
   ┌──────────────────────────────────────────┐
   │  循环结构 / 指令调度 / 函数融合 / 搬运优化   │ ← 02_reg_vector_compute_practices 的重点
   │  （在VF函数内部进一步压榨硬件性能）          │
   └──────────────────────────────────────────┘
```

| 对比 | gelu_high_performance | 02_reg_vector_compute_practices |
|------|----------------------|-------------------------------|
| **回答的问题** | 为什么用RegBase？ | RegBase怎么用得更好？ |
| **优化层次** | 编程范式级 | 微架构指令级 |
| **性能收益来源** | 消除UB中间读写+同步 | 指令双发/融合/搬运效率 |
| **核心本质** | **一样的**：都是利用寄存器直 |

**结论**：两者的核心本质相同——**都是利用寄存器直接计算来消除UB访存开销**。gelu_high_performance 展示的是"是否使用RegBase"的宏观收益，而 02_reg_vector_compute_practices 展示的是"如何更高效地使用RegBase"的微观优化。前者是后者的基础，后者是前者的深化。

---

## 6. 参考文档

| 文档名称 | 链接 |
|---------|------|
| Reg矢量计算编程 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0059.html) |
| 连续非对齐场景优化 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0022.html) |
| 通过UB融合实现连续vector计算 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0020.html) |
| 优化建议总览表 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlas_ascendc_best_practices_10_0010.html) |
| SIMD与SIMT混合编程基础知识 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0060.html) |
| CANN社区版9.0.0-beta.2 文档首页 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/index/index.html) |
