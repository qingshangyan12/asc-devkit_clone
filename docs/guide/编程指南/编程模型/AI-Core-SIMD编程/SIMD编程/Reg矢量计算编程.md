# Reg 矢量计算编程

## 概述

Reg 矢量计算 API 是面向 RegBase 架构开发的底层编程接口，允许用户直接操作芯片中 Vector 计算单元的寄存器，提供更高的灵活性和更优的性能。

### 与基础 API 的对比

| 特性 | Reg 矢量计算 API | 基础 API |
|------|------------------|----------|
| 输入/输出数据类型 | Reg 矢量计算寄存器 | LocalTensor |
| 数据搬运控制 | 用户自主控制 | API 自动管理 |
| 计算灵活性 | 高（寄存器级操作） | 中（内存级操作） |
| 适用场景 | 性能敏感、精细控制需求 | 通用计算场景 |

---

## RegBase 编程模型

### 内存层级架构

RegBase 编程模型支持用户编写 Vector Function（向量函数），使用 `__simd_vf__` 标记，在向量运算单元中执行。

**SIMD Reg 矢量计算内存层级**如图所示：

![SIMD Reg 矢量计算内存层级](../../../../figures/SIMD-Reg矢量计算内存层级.png)

内存层级自上而下依次为：

- **VF Reg**：最靠近 Vector 计算单元的私有内存，包含多种类型的寄存器
- **Unified Buffer (UB)**：本地内存，单核内所有 VF Reg 共享
- **Global Memory (GM)**：全局内存

> **数据搬运路径**：GM → UB → Reg 矢量计算寄存器。SIMD 架构不支持 GM 直接加载到寄存器。

### 编程架构

**SIMD Reg 矢量计算编程架构**如图所示：

![SIMD Reg 矢量计算编程架构](../../../../figures/SIMD-Reg矢量计算编程架构.png)

核心组件：

- **执行单元**：从 Registers 读取数据，执行计算，结果写回 Registers
- **DMA 搬运单元**：负责 Registers 与 Local Memory 之间的数据搬运

---

## RegBase 与 MemBase 对比

### API 调用层级

![API 调用层级](../../../../figures/API-调用.png)

| 架构 | 调用链 | 数据暂存位置 |
|------|--------|--------------|
| **RegBase** | 用户代码 → Reg API → BuiltIn API | 寄存器（无搬运开销） |
| **MemBase** | 用户代码 → 高阶 API → 基础 API → BuiltIn API | Local Memory（需搬运） |

### 数据处理差异

| 架构 | 数据切分 | 处理单元 | 特点 |
|------|----------|----------|------|
| **RegBase** | 需按 VL 切分，每次处理 VL 长度数据 | 寄存器 | 中间结果暂存寄存器，减少搬运 |
| **Membase** | 无需切分，直接处理完整 LocalTensor | 内存 | 流程简化，但需内存搬运 |

> **VL (Vector Length)**：寄存器容纳的最大数据长度，受寄存器容量限制。

---

## 函数调用层次

### 函数类型定义

| 函数类型 | 标识符 | 说明 | 可调用函数 |
|----------|--------|------|------------|
| 核函数 | `__global__ __aicore__` | Device 侧入口，Host 通过 `<<<>>>` 调用 | `__aicore__` 函数 |
| AI Core 函数 | `__aicore__` | Device 侧执行 | `__simd_vf__` 函数 |
| SIMD VF 函数 | `__simd_vf__` | 向量函数，Reg API 调用入口 | `__simd_callee__` 函数、constexpr aicore |
| SIMD callee 函数 | `__simd_callee__` | VF 内子函数，可返回值或引用传参 | `__simd_callee__` 函数、constexpr aicore |

### 调用关系图

![AI Core 调用关系](../../../../figures/aicore.png)

**唯一合法调用链**：

![SIMD VF 调用链](../../../../figures/simd_vf.png)

### 设计优势

`__simd_vf__` 标记的设计优势：

- **代码隔离**：`__aicore__` 与 `__simd_vf__` 边界清晰，编译器可检测 BuiltIn API 使用范围合法性
- **调用检查**：完善报错机制，如禁止 `__simd_vf__` 内调用 `__aicore__` 或 simt 函数
- **优化控制**：用户可针对特定函数启用/禁用优化（如多 VF 函数融合）

### 示例代码

```cpp
template <typename T>
__aicore__ inline void Compute()
{
    // 申请输出队列并读取输入结果
    // ...

    // 调用 SIMD VF 函数
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);

    // 写入结果到输出队列并释放输入队列内存
    // ...
}
```

---

## Reg 矢量计算寄存器

### 寄存器类型

| 寄存器类型 | 说明 | 位宽 |
|------------|------|------|
| **RegTensor** | 矢量数据寄存器，计算基本存储单元 | VL（可存储 VL/sizeof(T) 个元素） |
| **MaskReg** | 掩码寄存器，控制参与计算的元素选择 | VL/8 |
| **UnalignRegForLoad** | 非对齐加载寄存器，优化 UB→Reg 非对齐访问 | 缓冲区 |
| **UnalignRegForStore** | 非对齐存储寄存器，优化 Reg→UB 非对齐访问 | 缓冲区 |
| **AddrReg** | 地址寄存器，存储地址偏移量 | 循环中根据 stride 自增 |

### 使用示例

#### 连续对齐场景

使用 RegTensor、MaskReg、AddrReg 实现向量加法：

```cpp
template<typename T>
__simd_vf__ inline void AddVF(
    __ubuf__ T* dstAddr,
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
    AscendC::Reg::AddrReg aReg;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        // 创建地址偏移寄存器
        aReg = AscendC::Reg::CreateAddrReg<T>(i, oneRepeatSize);
        // 更新掩码（控制参与运算的元素数量）
        mask = AscendC::Reg::UpdateMask<T>(count);
        // 加载数据到寄存器
        AscendC::Reg::LoadAlign(srcReg0, src0Addr, aReg);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr, aReg);
        // 执行向量加法
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        // 存储结果到 UB
        AscendC::Reg::StoreAlign(dstAddr, dstReg, aReg, mask);
    }
}
```

#### 连续非对齐场景

使用 UnalignRegForLoad/UnalignRegForStore 处理非对齐地址：

```cpp
template <typename T>
__simd_vf__ inline void LoadUnAlignVF(
    __ubuf__ T* dstAddr,
    __ubuf__ T* srcAddr,
    uint32_t oneRepeatSize,
    uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::UnalignRegForLoad ureg0;
    AscendC::Reg::UnalignRegForStore ureg1;
    AscendC::Reg::AddrReg aReg;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        aReg = AscendC::Reg::CreateAddrReg<T>(i, oneRepeatSize);
        // 预处理非对齐加载
        AscendC::Reg::LoadUnAlignPre(ureg0, srcAddr, aReg);
        // 执行非对齐加载
        AscendC::Reg::LoadUnAlign(srcReg, ureg0, srcAddr, aReg, 0);
        // 执行非对齐存储
        AscendC::Reg::StoreUnAlign(dstAddr, srcReg, ureg1, aReg);
    }
    // 非对齐存储后处理
    AscendC::Reg::StoreUnAlignPost(dstAddr, ureg1, aReg);
}
```

---

## 流水线同步控制

### 同步机制

当不同循环迭代写入同一地址，或源/目标地址相同时，需要流水线同步。SIMD VF 函数使用 `LocalMemBar` 进行同步：

- **目的流水线**：等待源流水线所有指令完成后执行
- **触发条件**：写读/写写场景下，寄存器不同时需插入同步指令

![同步流水线](../../../../figures/同步流水.png)

### 函数原型

```cpp
template <MemType src, MemType dst>
__simd_callee__ inline void LocalMemBar()
```

| 参数 | 说明 |
|------|------|
| `src` | 源流水线类型 |
| `dst` | 目的流水线类型 |

---

## 编程流程

### 标准流程

![Reg 矢量计算编程流程](../../../../figures/微指令编程流程图.png)

流程步骤：

1. **Load**：从 LocalTensor 加载 VL 长度数据到寄存器
2. **Compute**：在寄存器中执行数学计算
3. **Store**：将结果从寄存器搬出到 LocalTensor

> **优势**：计算逻辑在寄存器完成，减少 LocalTensor 数据搬运，提升性能。

### Add API 示例

#### API 原型

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Add(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
```

| 参数 | 说明 |
|------|------|
| `T` | 操作数数据类型 |
| `mode` | mask 未筛选元素的处理方式（ZEROING 置零 / KEEP 保留原值） |
| `dstReg` | 目标寄存器 |
| `srcReg0` | 源寄存器 0 |
| `srcReg1` | 源寄存器 1 |
| `mask` | 控制参与计算的元素个数 |

#### 数据搬运 API

| API | 说明 |
|-----|------|
| `LoadAlign(srcReg, srcAddr + offset)` | 从 LocalTensor 加载到寄存器 |
| `StoreAlign(dstAddr + offset, dstReg, mask)` | 从寄存器存储到 LocalTensor |

---

## 完整编程示例

### AddVF 函数实现

```cpp
// SIMD VF 函数：向量加法
template <typename T>
__simd_vf__ inline void AddVF(
    __ubuf__ T* dstAddr,
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
        // 更新掩码，每次消耗 VL 长度元素
        mask = AscendC::Reg::UpdateMask<T>(count);
        // 加载输入数据
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        // 执行加法
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        // 存储结果
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

// AI Core 函数：调用 SIMD VF
template <typename T>
__aicore__ inline void Compute()
{
    // 获取 LocalTensor
    AscendC::LocalTensor<T> dst = outQueueZ.AllocTensor<T>();
    AscendC::LocalTensor<T> src0 = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> src1 = inQueueY.DeQue<T>();

    // 计算参数
    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint32_t count = 512;
    uint16_t repeatTimes = AscendC::CeilDivision(count, oneRepeatSize);

    // 获取 UB 物理地址
    __ubuf__ T* dstAddr = (__ubuf__ T*)dst.GetPhyAddr();
    __ubuf__ T* src0Addr = (__ubuf__ T*)src0.GetPhyAddr();
    __ubuf__ T* src1Addr = (__ubuf__ T*)src1.GetPhyAddr();

    // 调用 SIMD VF 函数
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);

    // 队列操作
    outQueueZ.EnQue(dst);
    inQueueX.FreeTensor(src0);
    inQueueY.FreeTensor(src1);
}
```

### 参数说明

| 参数 | 类型 | 说明 |
|------|------|------|
| `dstAddr` | `__ubuf__ T*` | 输出数据 UB 地址 |
| `src0Addr` | `__ubuf__ T*` | 输入数据 0 UB 地址 |
| `src1Addr` | `__ubuf__ T*` | 输入数据 1 UB 地址 |
| `count` | `uint32_t` | 参与计算的元素个数 |
| `oneRepeatSize` | `uint32_t` | 每次循环处理的数据量（VL/sizeof(T)） |
| `repeatTimes` | `uint16_t` | 循环次数（向上取整） |

> **`__ubuf__`**：表示 Unified Buffer 物理地址，是 LocalTensor 的实际存储位置。