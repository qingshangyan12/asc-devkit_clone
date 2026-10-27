# 案例分析：SetFixpipeNz2ndFlag 接口跨版本适配

## 接口概述

`SetFixpipeNz2ndFlag` 用于直接设置 Fixpipe 硬件的 **LOOP3_PARA** 特殊功能寄存器，控制 L0C→GM/L1 搬出时的 NZ→ND（Nz2nd）格式转换参数。共有 **2 个互斥声明**（同一函数名的两种签名，按 arch 二选一）：

| 变体 | 签名 | 声明位置 | arch 守卫 |
|------|------|---------|----------|
| V1（uint32 dstNdStride） | `SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)` | `kernel_operator_fixpipe_intf.h` L64 | L63：`#if (__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` |
| V2（uint16 dstNdStride） | `SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)` | `kernel_operator_fixpipe_intf.h` L66 | L63 守卫的 `#else` 分支（**其余全部 arch，含 `__NPU_ARCH__` 未定义的 host 编译**） |

要点：

- **声明层守卫（L63）没有 `defined(__NPU_ARCH__)` 前缀**。`__NPU_ARCH__` 未定义时按 0 处理 → host 编译走 `#else` 分支，声明 V2（uint16 变体）。
- 声明处无独立注释块；其上方 L41-54 的注释描述的是 `Fixpipe` 的 `intriParams.nz2ndParams` 参数（文档错位）。
- LOOP3_PARA 寄存器位布局（各完整实现版本一致）：
  - `[63:32]`：dst nd stride（单位：element）
  - `[31:16]`：src nd stride（单位：C0_SIZE）
  - `[15:0]`：nd number
- 关联结构体 `Nz2NdParams`（`impl/basic_api/utils/kernel_utils_struct_dma_params.h` L37-78）同样按 arch 分形态：3510/5102 为 `{ndNum, srcNdStride, dstNdStride(uint32)}`；**其余 arch（含目标/5101 族）为老形态 `{nz2ndEn, ndNum, srcNdStride, dstNdStride(uint16), originalNSize}`**。

## 调用链

### 完整链路

```
kernel_operator_fixpipe_intf.h L63-67（声明，arch 守卫 L63：3510||5102 → V1；#else → V2）
  → kernel_operator_fixpipe_intf_impl.h L87-98（公共包装，arch 守卫 L87-88：
      #if defined(__NPU_ARCH__) && (3510||5102||5101||5161||5165||5163) → uint32 包装（L89-92）
      #else                                                              → uint16 包装（L94-97）
      均调用 SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride)）
  → [intf_impl.h L30-54 的 #elif __NPU_ARCH__ include 分派链]
      3003 → dav_l300/kernel_operator_fixpipe_impl.h（L46-47）
      3113 → dav_l311/kernel_operator_fixpipe_impl.h（L48-49）
      3103 → dav_l310/kernel_operator_fixpipe_impl.h（L50-51）
      5101/5161/5165/5163 → dav_5161/kernel_operator_fixpipe_impl.h（L52-53）
      3510 → dav_3510/kernel_operator_fixpipe_impl.h（L42-43）
  → dav_XXXX/kernel_operator_fixpipe_impl.h 中的 SetFixpipeNz2ndFlagImpl（版本特定实现，文件内无 arch 守卫）
  → 编译器内置 SPR 写指令：set_loop3_para
```

### 各层守卫汇总

| 层 | 文件:行号 | 守卫条件 | 备注 |
|----|----------|---------|------|
| 声明 V1 | fixpipe_intf.h L63-64 | `(__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` | 无 `defined()` 前缀 |
| 声明 V2 | fixpipe_intf.h L65-66（#else） | 上述条件的补集 | 含 host（`__NPU_ARCH__` 未定义） |
| 包装 V1(uint32) | fixpipe_intf_impl.h L87-92 | `defined(__NPU_ARCH__) && (3510\|\|5102\|\|5101\|\|5161\|\|5165\|\|5163)` | **与声明层守卫不一致！** |
| 包装 V2(uint16) | fixpipe_intf_impl.h L93-97（#else） | 上述条件的补集 | |
| 版本实现 | dav_*/fixpipe_impl.h | 无内部守卫（靠 intf_impl.h include 链选择） | |

### 关键发现：声明/定义守卫不一致（5101 族）

**声明层（L63）**：仅 3510/5102 走 uint32 变体 → 5101 族声明的是 **V2（uint16）**。
**包装层（L87-88）**：3510/5102/**5101/5161/5165/5163** 均走 uint32 包装 → 5101 族定义的是 **V1（uint32）**。

5101 族在完整包含头文件后，可见的重载集合为：

1. `SetFixpipeNz2ndFlag(uint16_t, uint16_t, uint16_t)` — 仅声明（L66），**无任何定义**
2. `SetFixpipeNz2ndFlag(uint16_t, uint16_t, uint32_t)` — 有定义（intf_impl.h L89-92，内部为 dav_5161 桩），但 intf.h 中**未声明**（定义本身使其可见）

由此产生的调用后果：

| 调用方式（5101 族） | 重载决议结果 | 后果 |
|--------------------|-------------|------|
| 第三实参为 `uint16_t`（按声明签名使用） | V2 精确匹配胜出（V1 需 u16→u32 转换） | 选中**未定义**的 V2 → **链接错误**（inline 函数 used-but-not-defined） |
| 第三实参为 `int` 字面量（如 `SetFixpipeNz2ndFlag(2, 16, 320)`） | 两个候选第三参均为整型转换、同级 | **歧义 → 编译错误** |
| 第三实参为 `uint32_t` | V1 精确匹配胜出 | 调用 V1 → dav_5161 **桩函数**（device 侧空操作；CPU debug 侧 `raise(SIGABRT)`） |

### 最底层编译器 API 列表

| 版本 | 编译器 API |
|------|-----------|
| 3003（dav_l300） | `set_loop3_para` |
| 3103（dav_l310） | `set_loop3_para` |
| 3113（dav_l311） | `set_loop3_para` |
| 5101（dav_5161） | **无**（桩函数，不调用任何指令） |
| 3510（dav_3510，参照） | `set_loop3_para`（`if ASCEND_IS_AIC` 守卫内） |

> `set_loop3_para` 为 CCE 编译器内置 SPR 写指令，本仓库内无定义；CPU DEBUG 下由仓库外 `stub_def.h` 提供 stub。

### 关联内部路径（同一寄存器的其他写入点）

- `Fixpipe`（ROW_MAJOR/COLUMN_MAJOR 格式）内部经 `SetLoop3Para<config>(intriParams)` 写同一 `set_loop3_para` 寄存器（dav_l300 L197-219、dav_l310/l311 同构、dav_3510 同构）——即 `SetFixpipeNz2ndFlag` 是**绕过 Fixpipe 参数结构体、直接配置 LOOP3_PARA 的独立入口**。
- `dav_l300/kernel_operator_set_spr_impl.h` L28 另有 `SetNdParaImpl` → `set_nd_para`（不同寄存器，勿混淆）。

## 支持矩阵

| 版本 | V1（uint32，L64） | V2（uint16，L66） | 包装层实际编译 | SetFixpipeNz2ndFlagImpl 实现 |
|------|------------------|------------------|---------------|------------------------------|
| 3003（dav_l300） | 未声明（守卫不含 3003） | **完整**（声明✓ 包装✓(uint16) 实现✓） | uint16 包装（#else） | **完整**（L68-76，`set_loop3_para`） |
| 3103（dav_l310） | 未声明 | **完整** | uint16 包装（#else） | **完整**（L68-76，`set_loop3_para`） |
| 3113（dav_l311） | 未声明 | **完整** | uint16 包装（#else） | **完整**（L68-76，`set_loop3_para`） |
| 5101（dav_5161） | intf.h 未声明，但 intf_impl.h 提供 uint32 定义 | **声明未定义**（调用 → 链接错误） | **uint32 包装**（守卫含 5101 族） | **桩函数**（L63-69，`ASCENDC_REPORT_NOT_SUPPORT`） |
| 3510（dav_3510）参照 | **完整**（声明✓ 包装✓(uint32) 实现✓） | 未声明（守卫为真，#else 不可见） | uint32 包装 | **完整**（L73-83，AIC 守卫内 `set_loop3_para`） |

> 5161/5165/5163 与 5101 共享 dav_5161 实现，状态相同。其余非目标 arch（c100/m200/c220/m300/m310/m510）不在本次分析范围。

## 各版本原始代码

### 声明（kernel_operator_fixpipe_intf.h L63-67）

```cpp
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride);
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride);
#endif
```

### 公共包装（kernel_operator_fixpipe_intf_impl.h L87-98）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 5101 || \
                              __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride)
{
    SetFixpipeNz2ndFlagImpl(ndNum, srcNdStride, dstNdStride);
}
#endif
```

### dav_3510（参照版本，3510）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L73-83，`if ASCEND_IS_AIC` 运行时守卫内）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    if ASCEND_IS_AIC {
        // ND_PARA[63:32], dst nd stride in unit of element
        // ND_PARA[31:16], src nd stride in uint of C0_SIZE
        // ND_PARA[15:0], nd number.
        uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                          (static_cast<uint64_t>(ndNum));
        set_loop3_para(config);
    }
}
```

> 公共 API 为 V1（uint32）变体，Impl 形参同为 uint32_t，签名链路一致。

---

### dav_l300（目标版本，3003）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

> 公共 API 为 V2（uint16）变体（3003 不在两层守卫的 uint32 分支中）；Impl 形参为 uint32_t，经包装层 u16→u32 隐式转换，**dstNdStride 实际取值范围被公共 API 限制在 0~65535**。

---

### dav_l310（目标版本，3103）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

> 与 dav_l300 逐字符相同（该文件整体复制自 dav_l311，Fixpipe 参数结构体仍命名 `FixpipeParamsL311`）。

---

### dav_l311（目标版本，3113）

#### SetFixpipeNz2ndFlagImpl — 完整实现（L68-76）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}
```

> 与 dav_l300 逐字符相同。

---

### dav_5161（目标版本，5101；5101/5161/5165/5163 共享）

#### SetFixpipeNz2ndFlagImpl — 桩函数（`dav_5161/kernel_operator_fixpipe_impl.h` L63-69）

```cpp
__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    (void)(ndNum);
    (void)(srcNdStride);
    (void)(dstNdStride);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixpipeNz2ndFlag");
}
```

> - `ASCENDC_REPORT_NOT_SUPPORT` 仅在 CPU DEBUG 下检查并 `raise(SIGABRT)`（kernel_log.h L88-94）；device 侧为**空宏**（kernel_log.h L295）→ 桩函数在真机上为**静默空操作**。
> - Impl 形参为 uint32_t（拷贝自 3510 形态），与 intf.h 对 5101 族声明的 uint16 变体不匹配（见调用链"关键发现"）。
> - 同目录无 `kernel_operator_macro_impl.h` 路径的 Nz2nd 接口；5101 族 nz2nd 能力由 ConvFix/MmadFix 宏参数路径承载（Fixpipe V220 声明守卫 L76 不含 5101 族）。

## 代码级实现差异分析

### 实现风格对比表

| 特征 | dav_3510（参照） | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|------|------|------|------|------|
| 实现状态 | 完整 | 完整 | 完整 | 完整 | **桩** |
| 底层指令 | `set_loop3_para` | `set_loop3_para` | `set_loop3_para` | `set_loop3_para` | 无 |
| AIC 运行时守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |
| Impl dstNdStride 形参 | uint32_t | uint32_t | uint32_t | uint32_t | uint32_t |
| 公共 API 变体 | V1（uint32） | V2（uint16） | V2（uint16） | V2（uint16） | 声明 V2 / 定义 V1（**不一致**） |
| 位打包方式 | `(dst<<32)\|(src<<16)\|nd` | 同左 | 同左 | 同左 | — |
| 类型检查 | 无 | 无 | 无 | 无 | 无 |
| 参数合法性校验 | 无（如 ndNum>0 未查） | 无 | 无 | 无 | 无 |
| 声明/包装/实现签名一致性 | 一致 | 一致（Impl 形参更宽） | 一致（同左） | 一致（同左） | **不一致** |

### 实现模式总结

**模式1：直接位打包模式（dav_3510 / dav_l300 / dav_l310 / dav_l311）**

```cpp
uint64_t config = (dstNdStride << 32) | (srcNdStride << 16) | ndNum;
set_loop3_para(config);
```

- 三个参数按 LOOP3_PARA 位域直接拼接，单次 SPR 写入；
- 无类型检查、无参数范围校验（对比：`Fixpipe` 内部路径 `SetLoop3Para<config>` 对 `ndNum > 0` 有 `ASCENDC_ASSERT`，本公共入口没有）；
- 3510 额外套 `if ASCEND_IS_AIC` 运行时守卫（与 `SetFixPipeConfigImpl` 等同文件其他 SPR 接口风格一致）。

**模式2：桩函数模式（dav_5161）**

- `(void)` 消参 + `ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixpipeNz2ndFlag")`；
- device 侧为空操作，CPU debug 侧报错终止；
- 签名（uint32）保留 3510 形态，为未来补全实现预留接口形状。

### 关键差异点

1. **dav_5161 是唯一桩实现**，且其声明/包装守卫错位导致 API 在 5101 族上"按声明签名调用必然链接失败"——即便桩函数本身也无法通过 uint16 实参触达。
2. **dav_3510 是唯一带 AIC 守卫的版本**（与 SetFixPipeConfig 等同家族接口的跨版本差异模式一致）。
3. **l300/l310/l311 三版本实现逐字符相同**；dav_l310 文件整体复制自 dav_l311（`FixpipeParamsL311` 命名残留）。
4. **Impl 形参（uint32）宽于公共 API（uint16）**：l300/l310/l311 上 LOOP3_PARA[63:32] 域虽为 32bit，公共入口却只能传 0~65535；仅 3510/5102（及 intf_impl 层的 5101 族）暴露完整 32bit 入参。
5. **Nz2NdParams 结构体形态与公共 API 不配套**：l300/l310/l311/5101 族的 `Nz2NdParams::dstNdStride` 为 uint16（老形态，含 `nz2ndEn`/`originalNSize`），与 Impl 的 uint32 形参、3510 的 uint32 API 均不同步。
6. **nz2nd 能力入口因版本而异**：3510（Fixpipe V7/V8 + SetFixpipeNz2ndFlag）、l300/l311（Fixpipe V220 + SetFixpipeNz2ndFlag）、l310（Fixpipe 完整但走 V220 声明不可见路径，仅 SetFixpipeNz2ndFlag 可用）、5101 族（Fixpipe V220 不可见，SetFixpipeNz2ndFlag 为桩，nz2nd 由 ConvFix/MmadFix 宏参数承载）。

## 待确认问题

1. **intf.h L63 与 intf_impl.h L87-88 守卫不一致（最高优先级）**——5101 族声明 uint16 变体（无定义）而定义 uint32 变体（无声明）。是 intf.h L63 漏加 `|| 5101 || 5161 || 5165 || 5163`（应同步为 uint32 声明），还是 intf_impl.h 守卫误含 5101 族？**需修复**：当前状态下 5101 族用户按声明签名（uint16）调用必然链接错误，int 字面量调用产生歧义编译错误。
2. **dav_5161 桩函数是硬件不支持还是待补全？**——5101 族硬件是否有 LOOP3_PARA 寄存器/nz2nd 转换能力？若 nz2nd 完全由 ConvFix/MmadFix 宏参数路径承载且无手动设置需求，桩 + 修复守卫即为合理终态；若硬件支持直接配置，应从 dav_3510 移植位打包实现（注意指令名是否需 `set_loop3_para` → 5101 族等价物）。
3. **桩函数在 device 侧为空宏**——`ASCENDC_REPORT_NOT_SUPPORT` 真机不生效，若确认 5101 族不支持，是否应改为 device 侧也可见的 `ASCENDC_ASSERT`，避免静默无效果？
4. **l300/l310/l311 的公共 uint16 变体是否够用？**——Impl 形参与 LOOP3_PARA[63:32] 域均支持 uint32，但公共 API 限死 16bit。若用户需要 >65535 element 的 dst nd stride，是否应将三版本公共变体升级为 uint32（对齐 3510）？
5. **dav_3510 的 `if ASCEND_IS_AIC` 守卫**——l300/l310/l311/5161 均无。是其他版本遗漏（AIV 误调用风险），还是 3510 特有多核形态要求？（与 SetFixPipeConfig 分析中的同一问题同源，建议一并裁决。）
6. **`Nz2NdParams` 结构体形态分裂**（3510/5102 新形态 vs 其余老形态）与 SetFixpipeNz2ndFlag 的 uint32/uint16 分裂不同步——5101 族的 Impl 签名取 uint32 而其 Nz2NdParams 仍为 uint16 老形态，佐证 5101 的这套代码处于"半适配"状态。结构体是否需要随 API 一并统一？
7. **公共入口缺少参数校验**——`Fixpipe` 内部路径 `SetLoop3Para` 有 `ndNum > 0` 断言，公共 `SetFixpipeNz2ndFlagImpl` 无任何校验（各版本一致）。是否需要补齐（如 ndNum=0、stride 越界检查）？
