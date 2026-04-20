# AscendC VF函数与RegBase编程指南

## 1. VF函数基础知识

### 1.1 什么是VF函数

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
| 调用方式 | `AscendC::VF_CALL<VF函数名>(...)` 或直接调用 |
| 内部操作 | 直接操作Reg矢量计算寄存器 |
| 每次处理数据量 | VL（Vector Length），通过 `GetVecLen()` 获取 |

### 1.2 VF函数在芯片架构中的数据流

在SIMD Vector的内存架构中，数据流经三个层级：

```
             ┌─────────────────────────────────┐
             │       Global Memory (GM)         │  ← 大容量，高延迟（DDR/HBM）
             │   Host侧传入的输入/输出数据         │
             └───────────────┬─────────────────┘
                             │ DMA搬运 (DataCopy/DataCopyPad)
                             │ 硬件自动完成，无需VF参与
             ┌───────────────▼─────────────────┐
             │     Unified Buffer (UB)          │  ← Local Memory，__ubuf__
             │   LocalTensor实际存储的物理位置     │     容量有限（如1.5MB/核）
             └───────────────┬─────────────────┘
                             │ LoadAlign / LoadUnAlign （UB → 寄存器）
             ┌───────────────▼─────────────────┐
             │     VF Reg (矢量计算寄存器)        │  ← 最靠近Vector计算单元
             │   RegTensor / MaskReg / AddrReg   │     位宽 = VL（如256B）
             │   UnalignRegForLoad/Store         │
             └───────────────┬─────────────────┘
                             │ StoreAlign / StoreUnAlign （寄存器 → UB）
             ┌───────────────▼─────────────────┐
             │     Vector Compute Unit           │  ← 执行Add/Mul/Exp等计算
             │   从寄存器读操作数，结果写回寄存器     │
             └─────────────────────────────────┘
```


**关键约束**：SIMD架构**不支持**从GM直接加载数据到VF寄存器。必须先将数据从GM搬运至UB，再通过显式的Load/Store指令由UB加载到VF寄存器中。

### 1.3 VF函数的编程架构

SIMD Reg矢量计算编程架构中，通过发出指令到Reg矢量计算执行单元，执行单元从寄存器读取数据，进行计算，计算结果写回寄存器。DMA搬运单元负责在寄存器和Local Memory之间搬运数据。

```
┌─────────────────────────────────────────────────┐
│                 编程架构                           │
│                                                   │
│  ┌──────────┐  指令下发  ┌───────────────────┐   │
│  │  用户代码  │ ────────→ │ Reg矢量计算执行单元  │   │
│  │ (VF函数)  │           │                   │   │
│  └──────────┘           │  读Registers ──→ 计算│   │
│                          │  计算 ──→ 写Registers│   │
│                          └────────┬──────────┘   │
│                                   │               │
│  ┌──────────┐  Load/Store  ┌──────▼──────┐      │
│  │   UB     │ ←──────────→ │  Registers   │      │
│  │(LocalMem)│   DMA搬运单元  │ (RegTensor等) │      │
│  └──────────┘              └─────────────┘      │
└─────────────────────────────────────────────────┘
```

### 1.4 VF函数的常见概念

#### 1.4.1 `__ubuf__` 地址类型

`__ubuf__` 是VF函数参数的地址修饰符，表示该指针指向Unified Buffer（本地内存）。VF函数的所有数据地址参数必须使用 `__ubuf__` 修饰：

```cpp
__simd_vf__ inline void MyVF(__ubuf__ float* dst, __ubuf__ float* src, ...)
//                           ^^^^^^                 ^^^^^^
//                           表示这些地址指向UB，不是GM
```

在 `__aicore__` 函数中，通过 `LocalTensor::GetPhyAddr()` 获取UB物理地址并转换为 `__ubuf__` 指针：

```cpp
__ubuf__ float* srcAddr = (__ubuf__ float*)xLocal.GetPhyAddr();
```

#### 1.4.2 VL（Vector Length）

RegTensor位宽为VL（Vector Length），即VF函数每次循环能处理的数据量，具体值可能因不同AI处理器型号而异。Atlas 350 加速卡（VL=256B。 VL与数据类型相关：

| 数据类型 | VL字节数 | 每次处理元素数 |
|---------|---------|-------------|
| float (4B) | 256B | 64 |
| half (2B) | 256B | 128 |
| int32_t (4B) | 256B | 64 |

获取方式：`AscendC::GetVecLen()` 返回VL的字节数，`oneRepeatSize = GetVecLen() / sizeof(T)` 为每次处理的元素数。

#### 1.4.3 Mask（掩码）

Mask控制VF计算中参与运算的元素数量。当数据量不是VL的整数倍时，最后一次迭代需要通过Mask限制有效元素。MaskReg用于指示在计算过程中哪些元素参与计算，宽度为RegTensor的八分之一（VL/8）。

```cpp
// 动态Mask：根据实际元素数count更新，适用于尾块处理
AscendC::Reg::MaskReg mask = AscendC::Reg::UpdateMask<T>(count);
    //count矢量计算需要操作的元素的具体数量，生成对应的MaskReg，元素有效范围从0到VL_T（位宽为VL的T类型元素个数）。执行完该函数后，count会减去VL_T.count = (count < VL_T) ? 0 : (count - VL_T)
// 固定Mask：当数据量恰好是VL的整数倍时使用，减少循环内指令数
AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::VL8>();
```

#### 1.4.4 循环切分

由于寄存器容量限制（VL），VF函数需要将数据按VL切分，每次处理一个VL长度的数据：

```cpp
constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);  // 每次处理元素数
uint32_t repeatTimes = AscendC::CeilDivision(totalCount, oneRepeatSize); // 循环次数（向上取整）

for (uint16_t i = 0; i < repeatTimes; ++i) {
    // 每次处理 srcAddr + i * oneRepeatSize 开始的一个VL数据块
}
```

#### 1.4.5 VF函数调用层次

```
核函数 (__global__ __aicore__)
  └── __aicore__ 函数
        └── VF函数 (__simd_vf__)       ← 通过 VF_CALL 或直接调用
              └── __simd_callee__ 子函数  ← VF函数内部的子函数
```

**合法调用链**：`__aicore__` → `VF_CALL<VF函数>` / 直接调用 → `__simd_vf__` → `__simd_callee__`

**关键约束**：
- `__simd_vf__` 函数内只能调用 `__simd_callee__` 函数和 `constexpr __aicore__` 函数
- `__simd_vf__` 函数内不能调用 `__aicore__` 函数或SIMT函数
- 这种隔离设计使编译器能对VF函数进行独立的优化（如VF融合）

---

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
                            每步操作后需PipeBarrier同步

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
| 同步方式 | `PipeBarrier<PIPE_V>()` | 寄存器天然保序，无需PipeBarrier |
| 灵活性 | 较低 | 更高，用户自主控制 |
| 性能潜力 | 一般 | 更高，减少UB读写开销 |

### 2.2 API调用层级差异

```
MemBase架构：
  高阶API → 基础API → 编译器BuiltIn API
                      ↗ 所有操作基于LocalTensor，中间结果暂存UB

RegBase架构：
  高阶API → 基础API → Reg矢量计算API → 编译器BuiltIn API
                      ↗ 操作基于寄存器，中间结果暂存寄存器
```

### 2.3 RegBase的核心寄存器类型

| 寄存器类型 | 说明 | 位宽 | 数量限制 |
|-----------|------|------|---------|
| `RegTensor<T>` | 矢量数据寄存器，基本存储单元 | VL | 单VF内≤32个 |
| `MaskReg` | 掩码寄存器，控制参与计算的元素 | VL/8 | 单VF内≤8个 |
| `UnalignRegForLoad` | 非对齐搬入缓冲寄存器，缓存跨对齐边界数据 | - | 单VF内≤4个 |
| `UnalignRegForStore` | 非对齐搬出缓冲寄存器，缓存跨对齐边界数据 | - | 单VF内≤4个 |
| `AddrReg` | 地址寄存器，存储地址偏移量 | - | - |

**注意**：如果VF内RegTensor超过32个，编译器会进行数据换入换出并加入同步指令，严重拖慢执行效率。同理MaskReg不超过8个，UnalignRegForLoad/UnalignRegForStore各不超过4个。

---

## 3. 如何基于RegBase实现一个VF函数

以GELU样例为例，完整的实现流程分为三步。

### 3.1 GELU算子背景

GELU（Gaussian Error Linear Unit）的tanh近似公式为：

```
GELU(x) ≈ x / (1 + e^(-1.595769 · (x + 0.044715 · x³)))
```

展开计算步骤共7步：
1. `tmp = x * x` — 计算x²
2. `tmp = tmp * x` — 计算x³
3. `coeff = tmp * 0.044715` — 计算0.044715·x³
4. `tmp = x + coeff` — 计算x + 0.044715·x³
5. `tmp = tmp * (-1.595769)` — 计算-1.595769·(x + 0.044715·x³)
6. `tmp = exp(tmp)` — 计算e^(-1.595769·(...))
7. `y = x / (1 + tmp)` — 计算最终结果

这是一个典型的多步骤向量计算，非常适合展示RegBase相比MemBase的优势。

### 3.2 Step 1: 定义VF函数

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

### 3.3 Step 2: 在 `__aicore__` 函数中获取UB地址并调用VF函数

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

注意：虽然 `GeluRegBaseCompute` 仍接收 `tmpLocal` 和 `coeffLocal` 参数，但在RegBase方式下这些空间不再使用——因为中间结果暂存在寄存器中，不需要额外的UB空间。

### 3.4 Step 3: 在Process流程中调用

```cpp
__aicore__ inline void Process()
{
    // ... 数据通过DataCopy从GM搬运到UB ...
    if constexpr (scenarioNum == 0) {
        GeluCompute(xLocal, yLocal, tmpLocal, coeffLocal, tileLen);   // MemBase方式
    } else if (scenarioNum == 1) {
        GeluRegBaseCompute(xLocal, yLocal, tmpLocal, coeffLocal, tileLen);  // RegBase方式
    }
    // ... 数据通过DataCopy从UB搬出到GM ...
}
```

### 3.5 MemBase vs RegBase实现GELU的详细对比

#### MemBase方式（`GeluCompute`）

每次向量操作后需要通过 `PipeBarrier<PIPE_V>()` 同步，中间结果暂存在UB中，每次操作都涉及UB读写。

```cpp
// MemBase: 7次UB读写 + 7次PipeBarrier
AscendC::Mul(tmpLocal, xLocal, xLocal, n);           // 1. 读UB(x)×2, 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();                       //   等待向量计算完成
AscendC::Mul(tmpLocal, tmpLocal, xLocal, n);          // 2. 读UB(tmp,x), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Muls(coeffLocal, tmpLocal, COEFF_A, n);     // 3. 读UB(tmp), 写UB(coeff)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Add(tmpLocal, xLocal, coeffLocal, n);        // 4. 读UB(x,coeff), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Muls(tmpLocal, tmpLocal, COEFF_B, n);       // 5. 读UB(tmp), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Exp(tmpLocal, tmpLocal, n);                  // 6. 读UB(tmp), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Adds(tmpLocal, tmpLocal, (float)1.0, n);    // 7. 读UB(tmp), 写UB(tmp)
AscendC::PipeBarrier<PIPE_V>();
AscendC::Div(yLocal, xLocal, tmpLocal, n);            // 读UB(x,tmp), 写UB(y)
```

#### RegBase方式（`GeluVf`）

所有计算在寄存器中完成，中间结果暂存在寄存器中，无需PipeBarrier同步，仅需在开始Load、结束Store时访问UB。

```cpp
// RegBase: 仅1次Load + 7次寄存器计算 + 1次Store
AscendC::Reg::LoadAlign(xReg, xAddr + i * oneRepeatSize);  // 从UB加载1次
AscendC::Reg::Mul(tmpReg, xReg, xReg, mask);               // 寄存器计算，无UB开销
AscendC::Reg::Mul(tmpReg, tmpReg, xReg, mask);              // 寄存器计算，无UB开销
AscendC::Reg::Muls(coeffReg, tmpReg, COEFF_A, mask);        // 寄存器计算，无UB开销
AscendC::Reg::Add(tmpReg, xReg, coeffReg, mask);            // 寄存器计算，无UB开销
AscendC::Reg::Muls(tmpReg, tmpReg, COEFF_B, mask);          // 寄存器计算，无UB开销
AscendC::Reg::Exp(tmpReg, tmpReg, mask);                    // 寄存器计算，无UB开销
AscendC::Reg::Adds(tmpReg, tmpReg, 1.0f, mask);             // 寄存器计算，无UB开销
AscendC::Reg::Div(yReg, xReg, tmpReg, mask);                // 寄存器计算，无UB开销
AscendC::Reg::StoreAlign(yAddr + i * oneRepeatSize, yReg, mask);  // 写回UB 1次
```

#### 性能差异量化

| 指标 | MemBase | RegBase | 提升 |
|------|---------|---------|------|
| UB读次数（每VL） | 10次 | 1次 | 10× |
| UB写次数（每VL） | 8次 | 1次 | 8× |
| PipeBarrier次数 | 7次 | 0次 | ∞ |
| UB空间占用 | xLocal + yLocal + tmpLocal + coeffLocal | xLocal + yLocal | 减少~50% |

**性能提升来源**：
1. **消除中间结果的UB读写开销**：7步计算只需1次Load + 1次Store，而非14次UB读写
2. **消除PipeBarrier同步开销**：寄存器内操作天然保序，无需同步等待
3. **减少UB空间占用**：中间结果不暂存UB，tmpLocal和coeffLocal不再需要
4. **寄存器访问延迟远低于UB**：寄存器是最靠近计算单元的存储

### 3.6 VF函数流水线同步控制

在VF函数内部，当需要将不同值写入同一地址，或目标dst和源src是同一地址时，需要使用 `LocalMemBar` 进行流水线同步。

```cpp
// 函数原型
template <MemType src, MemType dst>
__simd_callee__ inline void LocalMemBar();
```

**规则**：
- 当写指令使用的寄存器和读指令使用的寄存器**相同**时，硬件自动保序，不需要插入同步指令
- 当写指令使用的寄存器和读指令使用的寄存器**不同**时，如果要确保两条指令顺序执行，则需要插入同步指令

---

## 4. 参考文档

| 文档名称 | 链接 |
|---------|------|
| Reg矢量计算编程 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0059.html) |
| SIMD与SIMT混合编程基础知识 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0060.html) |
| Reg矢量计算API参考 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/opdevg/Ascendcopdevg/atlasascendc_1e_0060.html) |
| CANN社区版9.0.0-beta.2 文档首页 | [链接](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/index/index.html) |
