# 案例分析：SetFixPipeConfig 接口跨版本适配

## 接口概述

`SetFixPipeConfig` 用于配置 Fixpipe 硬件的 FPC（Fixpipe Config）特殊功能寄存器，控制 L0C→GM/L1 搬出时的 ReLU/Quant 预处理地址与 UnitFlag。共有 **4 个声明**，分布在 2 个接口文件中，构成**两条完全独立的调用路径**：

| 路径 | 变体 | 签名 | 声明位置 | arch 守卫 |
|------|------|------|---------|----------|
| Fixpipe | V1 双 tensor | `template<typename T> SetFixPipeConfig(const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)` | `kernel_operator_fixpipe_intf.h` L56-58 | **无** |
| Fixpipe | V2 单 tensor | `template<typename T, bool setRelu = false> SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag = false)` | `kernel_operator_fixpipe_intf.h` L60-61 | **无** |
| Macro | V1 BitMode | `SetFixPipeConfig(const FixPipeConfigBitModeParams& params)` | `kernel_operator_macro_intf.h` L100 | 文件级 L28-29（`5101\|\|5161\|\|5165\|\|5163`） |
| Macro | V2 Params | `SetFixPipeConfig(const FixPipeConfigParams& params)` | `kernel_operator_macro_intf.h` L101 | 同上 |

> Fixpipe 变体声明上方的注释块（L41-54）描述的是 `Fixpipe` 的参数，与 `SetFixPipeConfig` 不匹配（文档瑕疵）。

### 参数结构体（仅 Macro 路径使用）

| 结构体 | 定义位置 | 字段 | arch 守卫 |
|--------|---------|------|----------|
| `FixPipeConfigParams` | `include/basic_api/kernel_struct_macro.h` L133-153 | `preReluAddr` / `preQuantAddr` / `postQuantAddr` / `antiquantAddr` / `shiftValue` / `restorationCtrl`（均 `uint8_t`） | 文件级 L27-28 |
| `FixPipeConfigBitModeParams` | `include/basic_api/kernel_operator_macro_bitmode_intf.h` L178-204 | `union { uint64_t config0; FixPipeConfigBitModeConfig0 config0BitMode; }`，经 `GetConfig0()` 输出预打包值 | 文件级 L27-28 |
| `FixPipeConfigBitModeConfig0`（位域布局） | `impl/basic_api/dav_5161/kernel_operator_macro_bitmode_impl.h` L203-234 | 位域结构（见下文） | 文件级 L25-26 |

### 入口可见性

- `kernel_operator_intf.h` L35 无条件 include `kernel_operator_fixpipe_intf.h` → **Fixpipe 变体全版本可见**（但实现仅在 `defined(__NPU_ARCH__)` 时 include，见 L182-184；host 侧无包装实现）。
- `kernel_cube_intf.h` L38 include fixpipe_intf.h；L51-52 include `kernel_operator_macro_intf.h` / `kernel_operator_macro_bitmode_intf.h` → **Macro 变体仅 5101/5161/5165/5163 可见**（文件内部自守卫）。

## 调用链

### Fixpipe 路径（LocalTensor 模板变体，全版本）

```
kernel_operator_fixpipe_intf.h L56-58 / L60-61（声明，无 arch 守卫）
  → kernel_operator_fixpipe_intf_impl.h L75-79 / L81-85（公共包装 SetFixPipeConfig，无 arch 守卫）
      调用 SetFixPipeConfigImpl<T>(reluPre, quantPre, isUnitFlag)
         / SetFixPipeConfigImpl<T, setRelu>(preData, isUnitFlag)
  → [intf_impl.h L30-54 的 #elif __NPU_ARCH__ include 分派链（共享层分发逻辑）]
      __NPU_ARCH__ == 3003              → dav_l300/kernel_operator_fixpipe_impl.h   （L46-47）
      __NPU_ARCH__ == 3113              → dav_l311/kernel_operator_fixpipe_impl.h   （L48-49）
      __NPU_ARCH__ == 3103              → dav_l310/kernel_operator_fixpipe_impl.h   （L50-51）
      __NPU_ARCH__ == 5101/5161/5165/5163 → dav_5161/kernel_operator_fixpipe_impl.h （L52-53）
      __NPU_ARCH__ == 3510              → dav_3510/kernel_operator_fixpipe_impl.h   （L42-43）
      （其余：1001→c100, 2002→m200, 2201→c220, 3002→m300, 3102→m310, 5102→m510）
  → dav_XXXX/kernel_operator_fixpipe_impl.h 中的 SetFixPipeConfigImpl（版本特定实现，文件内无 arch 守卫）
  → 编译器内置 SPR 写指令：set_fpc / set_m_fpc
```

### Macro 路径（仅 5101/5161/5165/5163）

```
kernel_operator_macro_intf.h L100 / L101（声明，文件级守卫 L28-29，#endif 于 L268）
  → kernel_operator_macro_intf_impl.h L78 / L80（公共包装，文件级守卫 L24-25）
      调用 SetFixPipeConfigImpl(params)
  → kernel_operator_macro_base_impl.h L82 / L84（共享实现 Impl，文件级守卫 L25-26）
      调用 SetFixPipeConfigCal(params)
  → [base_impl.h L30-33：无 #elif 分支，直接 #include "dav_5161/kernel_operator_macro_impl.h"
     （共享层分发逻辑：4 个 arch 共享 dav_5161 同一套实现，dav_l300/l310/l311/dav_3510 无 macro_impl.h）]
  → dav_5161/kernel_operator_macro_impl.h L158 / L160-178（SetFixPipeConfigCal，文件级守卫 L25-26）
  → 编译器内置 SPR 写指令：set_m_fpc
```

BitMode 参数构造子链：

```
FixPipeConfigBitModeParams（bitmode_intf.h L178-204 声明）
  → kernel_operator_macro_bitmode_intf_impl.h L231-235（构造函数 → config0BitMode.Init(params)）
  → dav_5161/kernel_operator_macro_bitmode_impl.h L203-234（FixPipeConfigBitModeConfig0 位域打包）
```

### 各层守卫汇总

| 层 | 文件 | 守卫 | 条件 |
|----|------|------|------|
| 声明（fixpipe） | fixpipe_intf.h L56/L60 | 无 | — |
| 包装（fixpipe） | fixpipe_intf_impl.h L75/L81 | 无（分发在 include 链 L30-54） | `#elif __NPU_ARCH__` |
| 实现（fixpipe） | dav_*/fixpipe_impl.h | 无内部守卫（靠 include 选择） | — |
| 声明（macro） | macro_intf.h L100/L101 | 文件级 L28-29 | `5101\|\|5161\|\|5165\|\|5163` |
| 包装（macro） | macro_intf_impl.h L78/L80 | 文件级 L24-25 | 同上 |
| 共享（macro） | macro_base_impl.h L82/L84 | 文件级 L25-26 | 同上 |
| 实现（macro） | dav_5161/macro_impl.h L158/L160 | 文件级 L25-26 | 同上 |

### 最底层编译器 API 列表

| 版本 | Fixpipe 路径 | Macro 路径 |
|------|-------------|-----------|
| 3003（dav_l300） | `set_fpc` | — |
| 3103（dav_l310） | `set_fpc` | — |
| 3113（dav_l311） | `set_fpc` | — |
| 5101（dav_5161） | `set_m_fpc` | `set_m_fpc` |
| 3510（dav_3510，参照） | `set_fpc` | — |

> `set_fpc` / `set_m_fpc` 均为 CCE 编译器内置 SPR 写指令，本仓库内无定义（`dav_l300/kernel_operator_set_spr_impl.h` 等直接调用）；CPU DEBUG（`ASCENDC_CPU_DEBUG==1`）下由仓库外的 `stub_def.h` 提供 stub。

## 支持矩阵

| 变体 | 3003（dav_l300） | 3103（dav_l310） | 3113（dav_l311） | 5101（dav_5161） | 3510（dav_3510）参照 |
|------|------|------|------|------|------|
| Fixpipe V1（reluPre+quantPre） | **完整**（set_fpc） | **完整**（set_fpc） | **完整**（set_fpc） | **受限**（set_m_fpc，仅 isUnitFlag=true，不写 FPC[63]） | **完整**（set_fpc，AIC 守卫内） |
| Fixpipe V2（preData+setRelu） | **完整** | **完整** | **完整** | **受限**（同上） | **完整**（AIC 守卫内） |
| Macro V1（BitMode） | N/A | N/A | N/A | **完整**（set_m_fpc） | N/A |
| Macro V2（Params） | N/A | N/A | N/A | **完整**（set_m_fpc） | N/A |

说明：

- **受限**：dav_5161 实现含 `ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, "SetFixPipeConfig isUnitFlag")`。该宏仅在 CPU DEBUG 下检查并 `raise(SIGABRT)`（kernel_log.h L88-94）；device 侧展开为**空宏**（kernel_log.h L295）→ isUnitFlag 实际被忽略，且实现不打包 FPC[63] UnitFlag 位。
- **N/A**：Macro 路径声明/包装/共享/实现整条链均受 `5101||5161||5165||5163` 文件级守卫保护，其他架构下接口完全不存在（无桩、无声明，调用直接编译报错）。
- 5101 与 5161/5165/5163 共享 dav_5161 目录，无 per-arch 差异。

## 各版本原始代码

### dav_3510（参照版本，3510）

> 文件：`impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h`。整个文件无内部 `__NPU_ARCH__` 守卫。

#### Fixpipe V1 — 完整实现（L44-55，`if ASCEND_IS_AIC` 运行时守卫内）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
        config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
        set_fpc(config);
    }
}
```

#### Fixpipe V2 — 完整实现（L57-71，`if ASCEND_IS_AIC` 运行时守卫内）

```cpp
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    if ASCEND_IS_AIC {
        uint64_t config = 0;
        if constexpr (setRelu) {
            config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
        } else {
            config =
                config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
        }
        config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
        set_fpc(config);
    }
}
```

#### Macro 变体 — 不存在

> dav_3510 目录无 `kernel_operator_macro_impl.h`，Macro 路径对其不可见。

---

### dav_l300（目标版本，3003）

> 文件：`impl/basic_api/dav_l300/kernel_operator_fixpipe_impl.h`。文件内无 `__NPU_ARCH__` 守卫。

#### Fixpipe V1 — 完整实现（L44-53）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}
```

#### Fixpipe V2 — 完整实现（L55-66）

```cpp
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}
```

#### Macro 变体 — 不存在

> dav_l300 目录无 `kernel_operator_macro_impl.h`。

---

### dav_l310（目标版本，3103）

> 文件：`impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h`。文件内无 `__NPU_ARCH__` 守卫。

#### Fixpipe V1 — 完整实现（L44-53）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}
```

#### Fixpipe V2 — 完整实现（L55-66）

```cpp
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}
```

> **与 dav_l300 的 SetFixPipeConfigImpl 逐字符相同。** 注意：该文件中的 Fixpipe 参数结构体命名为 `TransformParamsL311` / `FixpipeParamsL311`（L84/L104），构造参数带下划线后缀（`nSizeIn_`，L109），与 dav_l311 完全一致而与 dav_l300（`TransformParamsL300`、`nSizeIn`）不同——dav_l310 的整个文件是从 dav_l311 复制的。

#### Macro 变体 — 不存在

> dav_l310 目录无 `kernel_operator_macro_impl.h`。

---

### dav_l311（目标版本，3113）

> 文件：`impl/basic_api/dav_l311/kernel_operator_fixpipe_impl.h`。文件内无 `__NPU_ARCH__` 守卫。

#### Fixpipe V1 — 完整实现（L44-53）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}
```

#### Fixpipe V2 — 完整实现（L55-66）

```cpp
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}
```

> 与 dav_l300 对应函数逐字符相同（结构体命名为 L311 系）。

#### Macro 变体 — 不存在

> dav_l311 目录无 `kernel_operator_macro_impl.h`。

---

### dav_5161（目标版本，5101；5101/5161/5165/5163 共享）

#### Fixpipe V1 — 受限实现（`dav_5161/kernel_operator_fixpipe_impl.h` L39-48）

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, "SetFixPipeConfig isUnitFlag");
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    set_m_fpc(config);
}
```

#### Fixpipe V2 — 受限实现（`dav_5161/kernel_operator_fixpipe_impl.h` L50-61）

```cpp
template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, "SetFixPipeConfig isUnitFlag");
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    set_m_fpc(config);
}
```

> 与其他版本相比：① SPR 指令为 `set_m_fpc`（非 `set_fpc`）；② 不打包 FPC[63] UnitFlag；③ 以 `ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, ...)` 限制 isUnitFlag 必须 true（仅 CPU DEBUG 生效，device 侧为空宏）。

#### Macro V1（BitMode）— 完整实现（`dav_5161/kernel_operator_macro_impl.h` L158）

```cpp
__aicore__ inline void SetFixPipeConfigCal(const FixPipeConfigBitModeParams& params) { set_m_fpc(params.GetConfig0()); }
```

#### Macro V2（Params）— 完整实现（`dav_5161/kernel_operator_macro_impl.h` L160-178）

```cpp
__aicore__ inline void SetFixPipeConfigCal(const FixPipeConfigParams& params)
{
    constexpr uint8_t POS_RELU_PRE_ADDR = 0;
    constexpr uint8_t POS_QUANT_PRE_ADDR = 8;
    constexpr uint8_t POS_QUANT_POST_ADDR = 24;
    constexpr uint8_t POS_ANTIQ_PARA_ADDR = 32;
    constexpr uint8_t POS_FIXED_SHIFT_VAL = 40;
    constexpr uint8_t POS_RESTORATION_CTRL = 63;

    uint64_t fpc = 0;
    fpc |= (uint64_t)(params.preReluAddr) << POS_RELU_PRE_ADDR;
    fpc |= (uint64_t)(params.preQuantAddr) << POS_QUANT_PRE_ADDR;
    fpc |= (uint64_t)(params.postQuantAddr) << POS_QUANT_POST_ADDR;
    fpc |= (uint64_t)(params.antiquantAddr) << POS_ANTIQ_PARA_ADDR;
    fpc |= (uint64_t)(params.shiftValue) << POS_FIXED_SHIFT_VAL;
    fpc |= (uint64_t)(params.restorationCtrl) << POS_RESTORATION_CTRL;

    set_m_fpc(fpc);
}
```

#### FixPipeConfigBitModeConfig0 位域布局（`dav_5161/kernel_operator_macro_bitmode_impl.h` L203-234）

```cpp
struct FixPipeConfigBitModeConfig0 {
    uint64_t preReluAddr : 8;
    uint64_t preQuantAddr : 8;
    uint64_t postQuantAddr : 8;
    uint64_t antiquantAddr : 8;
    uint64_t shiftValue : 7;
    uint64_t restorationCtrl : 1;

    __aicore__ inline void Init(const FixPipeConfigParams& params)
    {
        preReluAddr = params.preReluAddr;
        preQuantAddr = params.preQuantAddr;
        postQuantAddr = params.postQuantAddr;
        antiquantAddr = params.antiquantAddr;
        shiftValue = params.shiftValue;
        restorationCtrl = params.restorationCtrl;
    }

    __aicore__ inline void SetPreReluAddr(uint8_t preReluAddr_) { preReluAddr = preReluAddr_; };
    __aicore__ inline void SetPreQuantAddr(uint8_t preQuantAddr_) { preQuantAddr = preQuantAddr_; };
    __aicore__ inline void SetPostQuantAddr(uint8_t postQuantAddr_) { postQuantAddr = postQuantAddr_; };
    __aicore__ inline void SetAntiquantAddr(uint8_t antiquantAddr_) { antiquantAddr = antiquantAddr_; };
    __aicore__ inline void SetShiftValue(uint8_t shiftValue_) { shiftValue = shiftValue_; };
    __aicore__ inline void SetRestorationCtrl(uint8_t restorationCtrl_) { restorationCtrl = restorationCtrl_; };

    __aicore__ inline uint8_t GetPreReluAddr() const { return preReluAddr; };
    __aicore__ inline uint8_t GetPreQuantAddr() const { return preQuantAddr; };
    __aicore__ inline uint8_t GetPostQuantAddr() const { return postQuantAddr; };
    __aicore__ inline uint8_t GetAntiquantAddr() const { return antiquantAddr; };
    __aicore__ inline uint8_t GetShiftValue() const { return shiftValue; };
    __aicore__ inline uint8_t GetRestorationCtrl() const { return restorationCtrl; };
};
```

#### FixPipeConfigParams 结构体（`include/basic_api/kernel_struct_macro.h` L133-153）

```cpp
struct FixPipeConfigParams {
    __aicore__ FixPipeConfigParams() {}

    __aicore__ FixPipeConfigParams(
        const uint8_t preReluAddrIn, const uint8_t preQuantAddrIn, const uint8_t postQuantAddrIn,
        const uint8_t antiquantAddrIn, const uint8_t shiftValueIn, const uint8_t restorationCtrlIn)
        : preReluAddr(preReluAddrIn),
          preQuantAddr(preQuantAddrIn),
          postQuantAddr(postQuantAddrIn),
          antiquantAddr(antiquantAddrIn),
          shiftValue(shiftValueIn),
          restorationCtrl(restorationCtrlIn)
    {}

    uint8_t preReluAddr = 0;
    uint8_t preQuantAddr = 0;
    uint8_t postQuantAddr = 0;
    uint8_t antiquantAddr = 0;
    uint8_t shiftValue = 0;
    uint8_t restorationCtrl = 0;
};
```

## 代码级实现差异分析

### FPC 寄存器位布局对比（三套布局）

**① Fixpipe 路径（3003/3103/3113/3510 写入）：**

| 位域 | 含义 | 打包方式 |
|------|------|---------|
| FPC[7:0] | ReluPreAddr | `reluPre.GetPhyAddr() >> 6`（64B 对齐） |
| FPC[15:8] | QuantPreAddr | `quantPre.GetPhyAddr() >> 7`（128B 对齐） |
| FPC[63] | UnitFlag | `isUnitFlag` |

**② dav_5161 Fixpipe 路径：** 仅 FPC[7:0] 与 FPC[15:8]（同上移位），**无 FPC[63]**。

**③ dav_5161 Macro Params 路径（手工位拼接）vs ④ Macro BitMode 路径（位域）：**

| 字段 | ③ Params Cal（POS_* 常量） | ④ BitMode 位域 |
|------|---------------------------|---------------|
| preReluAddr | bit 0（8bit） | [7:0] |
| preQuantAddr | bit 8（8bit） | [15:8] |
| postQuantAddr | **bit 24** | **[23:16]** |
| antiquantAddr | **bit 32** | **[31:24]** |
| shiftValue | **bit 40**（uint8 直移，未限宽） | **[38:32]**（7bit） |
| restorationCtrl | **bit 63** | **bit 39** |

> **③ 与 ④ 不一致**：同一份 `FixPipeConfigParams` 分别经 `SetFixPipeConfig(FixPipeConfigParams)` 与 `SetFixPipeConfig(FixPipeConfigBitModeParams(params))` 会产生**不同的寄存器值**（postQuant/antiquant/shift/restoration 位置均不同）。

### 实现风格对比表

| 特征 | dav_3510（参照） | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|------|------|------|------|------|
| Fixpipe V1/V2 状态 | 完整 | 完整 | 完整 | 完整 | 受限 |
| 底层指令 | `set_fpc` | `set_fpc` | `set_fpc` | `set_fpc` | `set_m_fpc` |
| AIC 运行时守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |
| UnitFlag → FPC[63] | 打包 | 打包 | 打包 | 打包 | 不打包 |
| isUnitFlag=false 行为 | 正常打包 0 | 正常打包 0 | 正常打包 0 | 正常打包 0 | CPU debug: SIGABRT；device: 静默忽略 |
| 类型检查 | 无（地址打包） | 无 | 无 | 无 | 无 |
| Macro 变体 | N/A | N/A | N/A | N/A | BitMode 透传 + Params 位拼接 |
| 文件来源痕迹 | 原创命名 | `TransformParamsL300`/`nSizeIn` | **复制自 l311**（`TransformParamsL311`/`nSizeIn_`） | `TransformParamsL311`/`nSizeIn_` | 独立（m_ 前缀指令族） |

### 实现模式总结

**模式1：物理地址打包模式（dav_3510 / dav_l300 / dav_l310 / dav_l311，Fixpipe 路径）**
- 从 `LocalTensor::GetPhyAddr()` 取物理地址，按对齐要求移位（`>>6` 64B / `>>7` 128B）拼入 FPC；
- V2 通过模板参数 `setRelu` 以 `if constexpr` 在编译期选择 ReluPreAddr/QuantPreAddr 字段；
- 3510 额外用运行时 `if ASCEND_IS_AIC` 包裹（Fixpipe 为 Cube 指令，防 AIV 误写）。

**模式2：简化受限模式（dav_5161，Fixpipe 路径）**
- 同样的地址移位打包，但 SPR 指令换成 `set_m_fpc`（5101 族 Cube 寄存器带 `m_` 前缀，与 `set_m_fmatrix`/`set_m_padding`/`set_m_relu_alpha` 同族）；
- 丢弃 UnitFlag 位，并以 `ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, ...)` 声明仅支持 `isUnitFlag=true`（约束仅在 CPU debug 生效）。

**模式3：寄存器字段直传模式（dav_5161，Macro 路径）**
- BitMode：用户（或 `Init`）预打包 `uint64_t config0` → `GetConfig0()` 单行透传；
- Params：`constexpr POS_*` + `|=` 位拼接（与 dav_5161 macro 家族其他接口风格完全统一）。

### 关键差异点

1. **SPR 指令族不同**：非 5101 族用 `set_fpc`，5101 族用 `set_m_fpc`——跨版本移植时指令名必须切换，不能直接拷贝实现。
2. **dav_5161 是唯一不写 FPC[63] 的版本**，也是唯一对 `isUnitFlag` 附加限制的版本；其限制在 device 侧实际不生效（空宏）。
3. **dav_3510 是唯一带 `if ASCEND_IS_AIC` 守卫的版本**，l300/l310/l311/5161 均无。
4. **l300/l310/l311 三版本 SetFixPipeConfigImpl 逐字符相同**；dav_l310 整个 fixpipe 实现文件复制自 dav_l311（结构体命名残留 `L311`）。
5. **dav_5161 上两条路径能力不对等**：Fixpipe 变体只能配置 2 个地址字段（reluPre/quantPre），Macro 变体可配置 6 个字段（含 postQuantAddr/antiquantAddr/shiftValue/restorationCtrl）——Macro 路径是 5101 族上功能更全的 FPC 配置入口。
6. **dav_5161 Macro 两种变体的位布局互不一致**（见位布局对比表 ③ vs ④），同参数不同变体产生不同寄存器值。

## 待确认问题

1. **dav_5161 Macro BitMode 与 Params 位布局不一致**（postQuant@16 vs @24、antiquant@24 vs @32、shift@32 vs @40、restoration@39 vs @63）——哪套是 5101 族 FPC 寄存器的真实硬件布局？是其中一处 bug，还是两变体有意面向不同语义？若为 bug 需确认修正方（`FixPipeConfigBitModeConfig0` 位域 or `SetFixPipeConfigCal` 的 POS_* 常量）。
2. **dav_5161 Fixpipe 变体 device 侧 isUnitFlag 被静默忽略且不写 bit63**——5101 族硬件是否固定 UnitFlag=1（fixpipe 单元强制 unit 模式）？若不是，是否需要补打包 bit63？bit63 与 Macro 变体的 `restorationCtrl@63` 是否同一控制位？
3. **`ASCENDC_REPORT_NOT_SUPPORT` 在 device 侧为空宏**（kernel_log.h L295）——dav_5161 对 `isUnitFlag=false` 的限制在真机上完全不生效。是否应改用 `ASCENDC_ASSERT`（device 侧 printf+abort）或编译期 `static_assert`？
4. **dav_3510 的 `if ASCEND_IS_AIC` 守卫**——l300/l310/l311/5161 均无此守卫。是其他版本遗漏（存在 AIV 误调用写 SPR 的风险），还是 3510 特有的多核形态（AIC/AIV 混布）要求？跨版本移植时应保留还是去除？
5. **3003/3103/3113/3510 无 Macro 变体**——这些架构的 FPC 是否只需 relu/quant 两字段（Fixpipe 变体已足够），还是同样存在 postQuant/antiquant/shift/restoration 配置需求但接口未适配（需确认硬件是否支持对应寄存器位）？
6. **Fixpipe 路径与 Macro 路径的字段语义换算**——Fixpipe 打包的是移位后的物理地址（`addr>>6`/`addr>>7`），Macro 打包的是 `uint8_t` 原值直传。用户通过 Macro 变体设置 `preReluAddr` 时是否需自行传入 `addr>>6` 的结果？两路径的 [7:0]/[15:8] 字段语义是否一致？文档未说明。
7. **dav_l310 文件复制残留**——`dav_l310/kernel_operator_fixpipe_impl.h` 内结构体仍命名为 `TransformParamsL311`/`FixpipeParamsL311`，是否需要重命名为 L310？（不影响功能，仅可维护性）
