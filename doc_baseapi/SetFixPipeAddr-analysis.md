# 案例分析：SetFixPipeAddr 接口跨版本适配

## 接口概述

`SetFixPipeAddr` 用于配置 Fixpipe/Cube 硬件的 eltwise 源地址特殊功能寄存器（3002 上为 `ELT_SRC_PARA`，5161 家族上为 `M_ELT_SRC_PARA`），为 conv/mm fixpipe 的 eltwise 融合指定 L1 侧源数据地址与 stride。**共 3 个变体**，分属两条独立路径（Fixpipe 路径 / Macro 路径）：

| 变体 | 签名 | 声明位置 | arch 守卫 |
|------|------|---------|----------|
| V1 模板版（Fixpipe 路径） | `template <typename T> __aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride)` | `kernel_operator_fixpipe_intf.h` **L73-74** | **无**（L73 位于无守卫区；L63-67 守卫仅管 `SetFixpipeNz2ndFlag` 参数类型，L76-77 起的守卫仅管 `Fixpipe` 模板族） |
| V2 BitMode 版（Macro 路径） | `__aicore__ inline void SetFixPipeAddr(const FixPipeAddrBitModeParams& params)` | `kernel_operator_macro_intf.h` **L130** | **有**（文件级守卫 L28-29：`__NPU_ARCH__ == 5101 \|\| 5161 \|\| 5165 \|\| 5163`） |
| V3 Params 版（Macro 路径） | `__aicore__ inline void SetFixPipeAddr(const FixPipeAddrParams& params)` | `kernel_operator_macro_intf.h` **L131** | 同上 |

要点：

- **V1 是模板接口**：包装层为模板，未调用时不会实例化（桩代码也不会编译）；接受 `LocalTensor`（完整实现在 3002 上要求位于 L1/A1/B1/C1）+ `c0ChStride`（C0 channel stride，ELT_SRC_PARA[15:0]）。
- **V2/V3 是非模板接口**：仅在 5101/5161/5165/5163 四个 arch 下声明可见，其他版本**连声明都不存在**（整个 `kernel_operator_macro_intf.h` 被排除）。
- `FixPipeAddrParams` 定义于 `kernel_struct_macro.h` L168-180（同样有 5101/5161/5165/5163 文件级守卫 L27-28）：`{ bool c0Indicator; uint32_t eltSrcAddr; uint32_t srcStride1; uint32_t srcStride2; }`。
- `FixPipeAddrBitModeParams` 类定义于 `kernel_operator_macro_bitmode_intf.h` L226-248（文件级守卫 L27-28），内部为 `union { uint64_t config0; FixPipeAddrBitModeConfig0 config0BitMode; }`；位域结构 `FixPipeAddrBitModeConfig0` 定义于 `dav_5161/kernel_operator_macro_bitmode_impl.h` L253-276，类方法实现于 `kernel_operator_macro_bitmode_intf_impl.h` L297-323。
- 入口可见性：`kernel_operator_intf.h` 无条件 include 两个 intf.h；但 intf.h 尾部仅在 `#if defined(__NPU_ARCH__)` 时 include 对应 intf_impl.h（fixpipe_intf.h L182-184、macro_intf.h L264-266）→ host 编译（无 `__NPU_ARCH__`）时声明可见但无实现。
- **同名不同签名**：dav_5161 的 fixpipe_impl.h 中也有一个 `SetFixPipeAddrImpl(LocalTensor, uint16_t)`（V1 的桩），与 macro 链路的 `SetFixPipeAddrImpl(FixPipeAddrParams/BitModeParams)` 重载共存，互不冲突。

## 调用链

### V1 模板版（Fixpipe 路径）

```
kernel_operator_fixpipe_intf.h L73-74（声明，无 arch 守卫）
  → kernel_operator_fixpipe_intf_impl.h L104-108（公共包装，无 arch 守卫，模板）
      SetFixPipeAddr(eleWiseData, c0ChStride) → SetFixPipeAddrImpl(eleWiseData, c0ChStride)
  → [intf_impl.h L30-54：#elif __NPU_ARCH__ include 分派链（共享层分发逻辑）]
      __NPU_ARCH__ == 1001                → dav_c100/kernel_operator_fixpipe_impl.h           （L30-31）
      __NPU_ARCH__ == 2002                → dav_m200/kernel_operator_fixpipe_impl.h           （L32-33）
      __NPU_ARCH__ == 2201                → dav_c220/...impl.h + dav_c220/..._v2_impl.h       （L34-36）
      __NPU_ARCH__ == 3002                → dav_m300/...impl.h + dav_m300/..._v2_impl.h       （L37-39）
      __NPU_ARCH__ == 3102                → dav_m310/kernel_operator_fixpipe_impl.h           （L40-41）
      __NPU_ARCH__ == 3510                → dav_3510/kernel_operator_fixpipe_impl.h           （L42-43）
      __NPU_ARCH__ == 5102                → dav_m510/kernel_operator_fixpipe_impl.h           （L44-45）
      __NPU_ARCH__ == 3003                → dav_l300/kernel_operator_fixpipe_impl.h           （L46-47）
      __NPU_ARCH__ == 3113                → dav_l311/kernel_operator_fixpipe_impl.h           （L48-49）
      __NPU_ARCH__ == 3103                → dav_l310/kernel_operator_fixpipe_impl.h           （L50-51）
      __NPU_ARCH__ == 5101/5161/5165/5163 → dav_5161/kernel_operator_fixpipe_impl.h           （L52-53）
  → dav_XXXX/kernel_operator_fixpipe_impl.h（或 _v2_impl.h）中的 SetFixPipeAddrImpl
  → 编译器内置 SPR 写指令：set_fixp_addr（仅 3002 完整实现）
```

### V2/V3 Macro 路径（仅 5101/5161/5165/5163）

```
kernel_operator_macro_intf.h L130/L131（声明，文件级守卫 L28-29）
  → kernel_operator_macro_intf_impl.h L93-95（公共包装，文件级守卫 L24-25）
      SetFixPipeAddr(FixPipeAddrBitModeParams) → SetFixPipeAddrImpl(params)   （L93）
      SetFixPipeAddr(FixPipeAddrParams)        → SetFixPipeAddrImpl(params)   （L95）
  → kernel_operator_macro_base_impl.h L97-99（共享层，文件级守卫 L25-26）
      SetFixPipeAddrImpl(params) → SetFixPipeAddrCal(params)     （L97 BitMode / L99 Params）
      [共享层分发：L30-33 仅 include dav_5161/kernel_operator_macro_impl.h，无 #elif 分支——
       macro 路径当前只有一个版本目录，隔离完全靠文件级 arch 守卫]
  → dav_5161/kernel_operator_macro_impl.h L201-220（文件级守卫 L25-26）
      SetFixPipeAddrCal(FixPipeAddrBitModeParams) → set_m_elt_src_para(params.GetConfig0())   （L201-204）
      SetFixPipeAddrCal(FixPipeAddrParams)        → 手动位打包 → set_m_elt_src_para(eltSrcPara)（L206-220）
  → 编译器内置 SPR 写指令：set_m_elt_src_para
```

### 各层守卫汇总

| 层 | 文件:行号 | 守卫 | 条件 |
|----|-----------|------|------|
| V1 声明 | fixpipe_intf.h L73-74 | 无 | — |
| V1 实现 include | fixpipe_intf.h L182-184 | `#if defined(__NPU_ARCH__)` | 任何 NPU arch |
| V1 公共包装 | fixpipe_intf_impl.h L104-108 | 无（分发在 include 链 L30-54） | `#elif __NPU_ARCH__` |
| V1 版本实现 | dav_*/kernel_operator_fixpipe_impl.h（或 _v2） | 文件内无 `__NPU_ARCH__` 守卫（靠 include 选择） | — |
| V2/V3 声明 | macro_intf.h L130-131 | 文件级 | `5101 \|\| 5161 \|\| 5165 \|\| 5163`（L28-29） |
| V2/V3 实现 include | macro_intf.h L264-266 | `#if defined(__NPU_ARCH__)`（嵌套在文件级守卫内） | — |
| V2/V3 公共包装 | macro_intf_impl.h L93-95 | 文件级 | 同上（L24-25） |
| V2/V3 共享层 | macro_base_impl.h L97-99 | 文件级 | 同上（L25-26） |
| V2/V3 版本实现 | dav_5161/kernel_operator_macro_impl.h L201-220 | 文件级 | 同上（L25-26） |

### 最底层编译器 API 列表

| 版本 | Arch | V1 底层 API | V2/V3 底层 API |
|------|------|-------------|----------------|
| dav_l300 | 3003 | —（桩，无指令调用） | —（声明不存在） |
| dav_l310 | 3103 | —（桩） | —（声明不存在） |
| dav_l311 | 3113 | —（桩） | —（声明不存在） |
| dav_5161 | 5101/5161/5165/5163 | —（桩） | `set_m_elt_src_para(uint64_t)` |
| dav_3510 | 3510（参照） | —（桩） | —（声明不存在） |
| dav_m300 | 3002（对照） | `set_fixp_addr(uint64_t)` | —（声明不存在） |
| dav_c100/m200/c220/m310/m510 | 1001/2002/2201/3102/5102 | —（桩） | —（声明不存在） |

> `set_fixp_addr` / `set_m_elt_src_para` 均为 CCE 编译器内置 SPR 写指令，本仓库内无定义（dav_l300 的 `kernel_operator_set_spr_impl.h` 仅包装 `set_quant_pre`/`set_loop3_para`/`set_nd_para`/`set_fpc`；dav_5161 的 set_spr_impl.h 为空文件，macro 链路直接调用编译器内置）；CPU DEBUG（`ASCENDC_CPU_DEBUG==1`）下由仓库外的 `stub_def.h` 提供 stub（3002 的 V1 完整实现中另有 `SetEleSrcPara(eltBaseAddr)` CPU DEBUG 分支）。

## 支持矩阵

### 目标版本 × 参照版本

| 版本 | Arch | V1 模板版（Fixpipe 路径） | V2 BitMode（Macro 路径） | V3 Params（Macro 路径） |
|------|------|--------------------------|--------------------------|-------------------------|
| dav_l300 | 3003 | **桩**（NOT_SUPPORT，impl.h L35-39） | 声明不存在 | 声明不存在 |
| dav_l310 | 3103 | **桩**（NOT_SUPPORT，impl.h L35-39） | 声明不存在 | 声明不存在 |
| dav_l311 | 3113 | **桩**（NOT_SUPPORT，impl.h L35-39） | 声明不存在 | 声明不存在 |
| dav_5161 | 5101 | **桩**（NOT_SUPPORT，fixpipe_impl.h L31-37） | **完整**（macro_impl.h L201-204） | **完整**（macro_impl.h L206-220） |
| dav_3510 | 3510（参照） | **桩**（NOT_SUPPORT，impl.h L35-39） | 声明不存在 | 声明不存在 |

### 全版本补充视野

| 版本 | Arch | V1 | V2/V3 | 实现位置 |
|------|------|----|----|----------|
| dav_m300 | 3002 | **完整**（唯一） | 不存在 | `kernel_operator_fixpipe_v2_impl.h` L30-49 |
| dav_c100 | 1001 | 桩 | 不存在 | impl.h L59-63 |
| dav_m200 | 2002 | 桩 | 不存在 | impl.h L60-64 |
| dav_c220 | 2201 | 桩 | 不存在 | v2_impl.h L34-38 |
| dav_m310 | 3102 | 桩 | 不存在 | impl.h L72-76 |
| dav_m510 | 5102 | 桩 | 不存在 | impl.h L31-35 |

**结论**：V1 的完整实现全仓库仅 3002 一处；V2/V3 的完整实现全仓库仅 dav_5161（5101/5161/5165/5163）一处。两条路径互不共享实现：5161 家族上 V1 是桩、V2/V3 是完整实现；3002 上 V1 是完整实现、V2/V3 不存在。

## 各版本原始代码

### 一、V1 模板版（Fixpipe 路径）

#### dav_l300（目标版本，3003）— 桩函数（L35-39）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}
```

#### dav_l310（目标版本，3103）— 桩函数（L35-39）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}
```

#### dav_l311（目标版本，3113）— 桩函数（L35-39）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}
```

> dav_l300 / dav_l310 / dav_l311 三者的 `SetFixPipeAddrImpl` 逐字符相同。

#### dav_5161（目标版本，5101）— 桩函数（带参数消除，L31-37）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    (void)(eleWiseTensor);
    (void)(c0ChStride);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}
```

#### dav_3510（参照版本，3510）— 桩函数（L35-39）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}
```

#### dav_m300（对照版本，3002）— 完整实现，全仓库唯一（kernel_operator_fixpipe_v2_impl.h L30-49）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWise, uint16_t c0ChStride)
{
    static_assert(
        SupportType<T, half>(), "Failed to check dtype in SetFixPipeAddr, current api support dtype is : half");
    const Hardware eleWiseHWPos = GetPhyType((TPosition)eleWise.GetPosition());
    if (eleWiseHWPos != Hardware::L1)
        ASCENDC_CHECK_TPOSITION(
            false, "eleWise", "L1 Buffer(A1/B1/C1)", "SetFixPipeAddr",
            GetPositionDisplay(static_cast<TPosition>(eleWise.GetPosition())));
    uint64_t config = 0;
    config = config | c0ChStride; // ELT_SRC_PARA[15:0], C0 channel stride Size.
    config = config | ((static_cast<uint64_t>(eleWise.GetPhyAddr()) >> 5)
                       << 16); // L1 address, ELT_SRC_PARA[31:16], align with 32bit.
    set_fixp_addr(config);
#if ASCENDC_CPU_DEBUG
    uint64_t eltBaseAddr = static_cast<uint64_t>(eleWise.GetPhyAddr());
    SetEleSrcPara(eltBaseAddr);
#endif
}
```

#### 其余版本 V1 桩（补充视野，均为 NOT_SUPPORT 单条）

- dav_c100（1001）impl.h L59-63、dav_m200（2002）impl.h L60-64：参数名 `eleWise`，无 `(void)` 消除。
- dav_c220（2201）v2_impl.h L34-38：参数名 `eleWise`，无 `(void)` 消除。
- dav_m310（3102）impl.h L72-76、dav_m510（5102）impl.h L31-35：参数名 `eleWiseTensor`，无 `(void)` 消除。

### 二、V2/V3 Macro 路径（仅 dav_5161 有实现）

#### dav_5161 — SetFixPipeAddrCal(FixPipeAddrBitModeParams) 完整实现（macro_impl.h L201-204）

```cpp
__aicore__ inline void SetFixPipeAddrCal(const FixPipeAddrBitModeParams& params)
{
    set_m_elt_src_para(params.GetConfig0());
}
```

#### dav_5161 — SetFixPipeAddrCal(FixPipeAddrParams) 完整实现（macro_impl.h L206-220）

```cpp
__aicore__ inline void SetFixPipeAddrCal(const FixPipeAddrParams& params)
{
    constexpr uint8_t POS_C0_INDICATOR = 0;
    constexpr uint8_t POS_ELT_SRC_ADDR = 16;
    constexpr uint8_t POS_SRC_STRIDE1 = 32;
    constexpr uint8_t POS_SRC_STRIDE2 = 48;

    uint64_t eltSrcPara = 0;
    eltSrcPara |= (uint64_t)(params.c0Indicator ? 1 : 0) << POS_C0_INDICATOR;
    eltSrcPara |= (uint64_t)(params.eltSrcAddr) << POS_ELT_SRC_ADDR;
    eltSrcPara |= (uint64_t)(params.srcStride1) << POS_SRC_STRIDE1;
    eltSrcPara |= (uint64_t)(params.srcStride2) << POS_SRC_STRIDE2;

    set_m_elt_src_para(eltSrcPara);
}
```

#### 参数结构体定义（佐证材料）

`kernel_struct_macro.h` L168-180 — `FixPipeAddrParams`（V3 入参，POD）：

```cpp
struct FixPipeAddrParams {
    __aicore__ FixPipeAddrParams() {}

    __aicore__ FixPipeAddrParams(
        const bool c0IndicatorIn, const uint32_t eltSrcAddrIn, const uint32_t srcStride1In, const uint32_t srcStride2In)
        : c0Indicator(c0IndicatorIn), eltSrcAddr(eltSrcAddrIn), srcStride1(srcStride1In), srcStride2(srcStride2In)
    {}

    bool c0Indicator = false;
    uint32_t eltSrcAddr = 0;
    uint32_t srcStride1 = 0;
    uint32_t srcStride2 = 0;
};
```

`dav_5161/kernel_operator_macro_bitmode_impl.h` L253-276 — `FixPipeAddrBitModeConfig0` 位域结构（V2 的 union 内部位域）：

```cpp
struct FixPipeAddrBitModeConfig0 {
    uint64_t c0Indicator : 1;
    uint64_t eltSrcAddr : 16;
    uint64_t srcStride1 : 16;
    uint64_t srcStride2 : 16;

    __aicore__ inline void Init(const FixPipeAddrParams& params)
    {
        c0Indicator = params.c0Indicator ? 1 : 0;
        eltSrcAddr = params.eltSrcAddr;
        srcStride1 = params.srcStride1;
        srcStride2 = params.srcStride2;
    }
    // ... Set/Get 方法略（L267-275）
};
```

> **注意**：该位域为**顺序紧排**布局（c0Indicator 占 bit0，eltSrcAddr 占 bit[16:1]，srcStride1 占 bit[32:17]，srcStride2 占 bit[48:33]），与 V3 手动打包的布局（bit0 / [31:16] / [47:32] / [63:48]）**不一致**，详见"待确认问题"第 3 条。

#### 包装层与共享层（完整链路佐证）

`kernel_operator_macro_intf_impl.h` L93-95（公共包装）：

```cpp
__aicore__ inline void SetFixPipeAddr(const FixPipeAddrBitModeParams& params) { SetFixPipeAddrImpl(params); }

__aicore__ inline void SetFixPipeAddr(const FixPipeAddrParams& params) { SetFixPipeAddrImpl(params); }
```

`kernel_operator_macro_base_impl.h` L97-99（共享层）：

```cpp
__aicore__ inline void SetFixPipeAddrImpl(const FixPipeAddrBitModeParams& params) { SetFixPipeAddrCal(params); }

__aicore__ inline void SetFixPipeAddrImpl(const FixPipeAddrParams& params) { SetFixPipeAddrCal(params); }
```

`kernel_operator_fixpipe_intf_impl.h` L104-108（V1 公共包装，模板）：

```cpp
template <typename T>
__aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride)
{
    SetFixPipeAddrImpl(eleWiseData, c0ChStride);
}
```

## 代码级实现差异分析

### 实现风格对比（V1 模板版）

| 特征 | dav_m300（3002，唯一完整） | dav_l300/l310/l311/3510 | dav_5161 |
|------|---------------------------|--------------------------|----------|
| 类型检查方式 | `static_assert(SupportType<T, half>)` 编译期强制 half | 无 | 无 |
| tensor 位置检查 | 运行时 `GetPhyType` + `ASCENDC_CHECK_TPOSITION`（必须 L1/A1/B1/C1） | 无 | 无 |
| 地址对齐处理 | `addr >> 5`（32B 对齐）写 [31:16] | — | — |
| 硬件指令调用 | `set_fixp_addr(config)` | 无 | 无 |
| 参数 unused 处理 | 全部使用 | 无 `(void)` 消除（依赖编译器不告警或忽略告警） | `(void)(eleWiseTensor); (void)(c0ChStride);` 显式消除 |
| CPU DEBUG 支持 | 有（`SetEleSrcPara` 分支） | 无 | 无 |
| 支持的数据类型 | 仅 `half` | 无 | 无 |

### 实现风格对比（V2/V3 Macro 版，仅 dav_5161）

| 特征 | V2 BitMode | V3 Params |
|------|-----------|-----------|
| 打包方式 | 位域结构 `FixPipeAddrBitModeConfig0`（union + `GetConfig0()`），用户也可 `SetConfig0` 直接注入原始寄存器值 | `constexpr POS_*` 手动移位打包 |
| 位布局 | 顺序紧排：c0Indicator[0]，eltSrcAddr[16:1]，srcStride1[32:17]，srcStride2[48:33] | POS 常量：c0Indicator[0]，eltSrcAddr[31:16]，srcStride1[47:32]，srcStride2[63:48] |
| 类型检查 | 无（非模板，uint64 直通） | 无（无截断/范围 assert） |
| 硬件指令 | `set_m_elt_src_para` | `set_m_elt_src_para` |
| 面向用户 | 专家模式（可绕过结构体直接设寄存器值） | 常规模式 |

### 实现模式总结

- **模式1：3002 直接寄存器模式（V1）** —— 从 `LocalTensor` 提取物理地址，与 `c0ChStride` 拼成 `ELT_SRC_PARA`（[15:0]=stride，[31:16]=addr>>5），单条 `set_fixp_addr` 完成。类型与位置双重校验，仅支持 half + L1。
- **模式2：5161 家族 Macro 寄存器模式（V2/V3）** —— 功能从 fixpipe SPR 迁移到 cube 侧 `M_ELT_SRC_PARA`，参数模型升级（`c0Indicator` 替代隐式 stride 语义，双 stride 字段，`eltSrcAddr` 直接以 32B 单位传递而非 tensor 地址换算）。V3 负责位打包，V2 允许专家直接操作 64bit 寄存器值。
- **模式3：其余全部版本的桩模式** —— `ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr")`，运行期报不支持；因 V1 是模板，桩代码仅在用户实例化时编译。

### 关键差异点

1. **两条路径、两套 SPR、两代参数模型**：3002 `ELT_SRC_PARA`（tensor 地址 + C0 stride）→ 5161 家族 `M_ELT_SRC_PARA`（eltSrcAddr + srcStride1/2 + c0Indicator）。跨这两代硬件适配时**参数语义不可直接映射**。
2. **5161 家族上 V1 桩 / V2V3 完整并存**：说明新硬件把 eltwise 地址配置收编进 Macro（MacroCube）体系，旧的 fixpipe 入口被废弃为桩——但也可能是 V1 尚未桥接到 `set_m_elt_src_para`（见待确认问题 2）。
3. **V2/V3 位布局不一致**：BitMode 位域（紧排 49bit）与 Params 手动打包（带保留间隔 64bit）产生不同的寄存器值，二者其一必然与硬件真实布局不符。
4. **dav_5161 桩风格更严谨**：唯一显式 `(void)` 消除未用参数的版本。
5. **目标版本（3003/3103/3113/5101）与参照版本（3510）的 V1 实现完全一致（桩）**，无任何版本差异，适配时无需区分。

## 待确认问题

1. **l300/l310/l311/3510 的 V1 桩是硬件不支持还是待补全？** 3002 有完整实现（老 C220/M300 系有 eltwise 能力），5161 家族有 `M_ELT_SRC_PARA`（新硬件能力还在且已升级）。需确认 3003/3103/3113/3510 硬件是否保留 eltwise SPR（`ELT_SRC_PARA` 或等价物）：若保留，桩应补全（参照 dav_m300 移植）；若不保留，建议桩消息注明"current device"以与族内其他 API 一致。
2. **5161 家族 V1 桩是否应桥接到 macro 实现？** dav_5161 已有 `SetFixPipeAddrCal`（V2/V3 完整），V1 模板版仍为桩。若 V1 语义（half eltwise tensor + c0ChStride）在新硬件上仍有效，可考虑在 `SetFixPipeAddrImpl(LocalTensor, uint16_t)` 中换算为 `FixPipeAddrParams` 并复用 `set_m_elt_src_para`；若 V1 参数模型与新 SPR 不兼容，则维持桩。
3. **V2 BitMode 位域与 V3 Params 手动打包的位布局不一致（疑似 bug）**：`FixPipeAddrBitModeConfig0` 顺序紧排（eltSrcAddr 落在 [16:1]）而 `SetFixPipeAddrCal(FixPipeAddrParams)` 打包到 [31:16]（bit[15:1] 保留）。二者写入 `set_m_elt_src_para` 的值不同。需对照硬件手册确认 `M_ELT_SRC_PARA` 真实布局，修正其一。
4. **`FixPipeAddrParams.eltSrcAddr` 为 uint32_t 但位域/打包仅 16bit** —— 超过 0xFFFF 时静默截断，无 assert/报错。是否需要加范围检查？
5. **V1 与 V3 的 stride 语义差异**：V1 的 `c0ChStride`（ELT_SRC_PARA[15:0]，C0 channel stride）与 V3 的 `srcStride1/srcStride2`（M_ELT_SRC_PARA[47:32]/[63:48]）不是同一组参数；从 3002 迁移算子到 5161 家族时需按新模型重新推导，不能机械替换。
6. **3103 目标版本与 3113 的实现逐字符相同**（本 API 无差异），若 3103 硬件规格与 3113 不同（如 L1/FB 布局差异导致地址换算 `>>5` 或单位不同），需硬件侧确认。
7. **host 编译（无 `__NPU_ARCH__`）时 V1 声明可见但无实现**（intf_impl.h 未被 include）；CPU DEBUG 下仅 3002 完整实现有 `SetEleSrcPara` stub 分支，其余版本（含 5161 macro 路径）无 CPU DEBUG 专用处理，需确认 CPU 调试时的预期行为。
