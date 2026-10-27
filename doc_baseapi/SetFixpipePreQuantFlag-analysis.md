# SetFixpipePreQuantFlag 接口跨版本支持分析

接口：`SetFixpipePreQuantFlag`（`asc-devkit\include\basic_api\kernel_operator_fixpipe_intf.h` L69）

目标版本：3003(dav_l300)、3103(dav_l310)、3113(dav_l311)、5101(dav_5161)
参照版本：3510(dav_3510)

---

## 一、接口概述

| 项 | 内容 |
|----|------|
| 声明 | `__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config);` |
| 声明位置 | `kernel_operator_fixpipe_intf.h` L69 |
| arch 守卫 | **无**（对比：同文件 L63-67 的 `SetFixpipeNz2ndFlag` 有 arch 分支守卫；Fixpipe 搬运重载 L76-179 有严格 arch 守卫） |
| 变体 | 单一变体，非模板，参数 `uint64_t config`（QUANT_PRE 寄存器位域由调用方打包） |
| 功能 | 设置 FixPipe 流水线 QUANT_PRE SPR 寄存器（预量化/反量化配置），与 Fixpipe L0C→L1/GM 搬运配合使用 |
| 文件级限制 | intf.h L16-18：`--enable-simt` 编译模式下整个文件禁用（`#error`）；L182-184：仅 `#if defined(__NPU_ARCH__)` 时引入 intf_impl.h |

用途佐证（l300 系 Fixpipe 主流程）：`dav_l300\kernel_operator_fixpipe_impl.h` L371-374 / L432-434、`dav_l310` L378-381 —— scalar quant 模式下 `FixpipeL0C2L1Impl/FixpipeL0C2GMImpl` 内部同样调用 `set_quant_pre(intriParams.deqScalar)`，即本接口是同一 SPR 的用户侧手动设置入口。

## 二、调用链

```
SetFixpipePreQuantFlag(uint64_t config)                     [intf.h L69，无 arch 守卫]
 └─ SetFixpipePreQuantFlagImpl(config)                      [intf_impl.h L100，无 arch 守卫，纯转发]
     ├─ dav_l300\kernel_operator_fixpipe_impl.h L78         [3003]  set_quant_pre(config)
     ├─ dav_l310\kernel_operator_fixpipe_impl.h L78         [3103]  set_quant_pre(config)
     ├─ dav_l311\kernel_operator_fixpipe_impl.h L78         [3113]  set_quant_pre(config)
     ├─ dav_5161\kernel_operator_fixpipe_impl.h L71         [5101/5161/5165/5163] set_m_quant_pre(config)
     └─ dav_3510\kernel_operator_fixpipe_impl.h L85-90      [3510]  if ASCEND_IS_AIC { set_quant_pre(config); }
```

各层守卫情况：

| 层 | 文件:行号 | __NPU_ARCH__ 守卫 | 说明 |
|----|-----------|------------------|------|
| 声明 | kernel_operator_fixpipe_intf.h L69 | 无 | 全架构声明 |
| include 分发 | kernel_operator_fixpipe_intf_impl.h L30-54 | 有 | `#if __NPU_ARCH__ == 3003` → dav_l300（L46-47）；`3103` → dav_l310（L50-51）；`3113` → dav_l311（L48-49）；`5101/5161/5165/5163` → dav_5161（L52-53）；`3510` → dav_3510（L42-43） |
| 公共包装 | kernel_operator_fixpipe_intf_impl.h L100 | 无 | `SetFixpipePreQuantFlag(config) { SetFixpipePreQuantFlagImpl(config); }` |
| 版本实现 | 各 dav\kernel_operator_fixpipe_impl.h | **文件内无 arch 守卫** | 分发完全靠上层 include 宏控（dav_5161 为 4 个 arch 共享文件） |
| 底层 intrinsic | set_quant_pre / set_m_quant_pre | — | CANN 编译器内置 SPR 写指令，仓库内无定义（kernel_reg.h 为向量 mask/barrier、utils/ 为 check 类、reg_compute/ 为向量寄存器计算，均不含） |

## 三、支持矩阵

| 版本 | intf.h 声明 | intf_impl.h 包装 | Impl 实现 | 底层编译器 API | 可用性 |
|------|------------|-----------------|-----------|----------------|--------|
| 3003 (dav_l300) | ✓ | ✓ | L78 完整实现 | `set_quant_pre` | **完整可用**（配合 Fixpipe 搬运接口，V220 参数版本已在 L76-77 放行 3003） |
| 3103 (dav_l310) | ✓ | ✓ | L78 完整实现 | `set_quant_pre` | **实现完整但"孤立"**：intf.h L76-77 的 Fixpipe 搬运重载守卫不含 3103，无搬运接口可消费该寄存器（见待确认问题 1） |
| 3113 (dav_l311) | ✓ | ✓ | L78 完整实现 | `set_quant_pre` | **完整可用**（Fixpipe V220 版本 L76-77 已放行 3113） |
| 5101 (dav_5161) | ✓ | ✓ | L71 完整实现（共享 5161） | `set_m_quant_pre` | **编译可用**；但 5101 无 Cube 核/L0C（TOTAL_L0C_SIZE=0），fixpipe 硬件流水线不存在，实际语义存疑（见待确认问题 2） |
| 3510 (dav_3510，参照) | ✓ | ✓ | L85-90 完整实现 | `set_quant_pre`（`if ASCEND_IS_AIC` 守卫） | **完整可用**（仅 Cube 核生效） |

## 四、各版本原始代码

### 4.1 声明（kernel_operator_fixpipe_intf.h L56-74）——【全架构共用】

```cpp
template <typename T>
__aicore__ inline void SetFixPipeConfig(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false);

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfig(const LocalTensor<T>& preData, bool isUnitFlag = false);

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride);
#else
__aicore__ inline void SetFixpipeNz2ndFlag(uint16_t ndNum, uint16_t srcNdStride, uint16_t dstNdStride);
#endif

__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config);    // ← L69，分析目标，无守卫

__aicore__ inline void SetFixPipeClipRelu(uint64_t config);

template <typename T>
__aicore__ inline void SetFixPipeAddr(const LocalTensor<T>& eleWiseData, uint16_t c0ChStride);
```

### 4.2 公共包装（kernel_operator_fixpipe_intf_impl.h L100）——【全架构共用】

```cpp
__aicore__ inline void SetFixpipePreQuantFlag(uint64_t config) { SetFixpipePreQuantFlagImpl(config); }
```

### 4.3 dav_l300（3003）L78——【完整实现】

```cpp
__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }
```

### 4.4 dav_l310（3103）L78——【完整实现，与 dav_l300 逐字一致】

```cpp
__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }
```

### 4.5 dav_l311（3113）L78——【完整实现，与 dav_l300 逐字一致】

```cpp
__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }
```

### 4.6 dav_5161（5101/5161/5165/5163 共享）L71——【完整实现，指令名不同】

```cpp
__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_m_quant_pre(config); }
```

### 4.7 dav_3510（3510 参照）L85-90——【完整实现，带 AIC 守卫】

```cpp
__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config)
{
    if ASCEND_IS_AIC {
        set_quant_pre(config);
    }
}
```

### 4.8 用途佐证：l300 系 Fixpipe 主流程内部调用（dav_l310 L360-386，dav_l300/dav_l311 同构）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    ...
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);      // ← 同一 SPR 的框架内部写入
    }
    PipeBarrier<PIPE_FIX>();
    ...
}
```

## 五、代码级实现差异分析

### 5.1 实现风格对比表（含同文件邻居 SPR 接口）

| 维度 | dav_l300/l310/l311 | dav_5161 | dav_3510 |
|------|--------------------|----------|----------|
| PreQuantFlag 指令 | `set_quant_pre` | `set_m_quant_pre`（m_ 前缀指令族） | `set_quant_pre` |
| AIC 守卫 | 无 | 无 | `if ASCEND_IS_AIC` |
| 函数体形态 | 一行透传 | 一行透传 | 守卫块包裹 |
| SetFixPipeConfigImpl | 完整：`set_fpc`，写 FPC[63] UnitFlag 位 | 完整：`set_m_fpc`，但 `ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag)`（强制 isUnitFlag=true）且**不写** FPC[63] | 完整：`set_fpc`（AIC 守卫），写 FPC[63] |
| SetFixPipeClipReluImpl | 桩（`ASCENDC_REPORT_NOT_SUPPORT`） | **完整**：`set_m_clip_relu` | 桩 |
| SetFixpipeNz2ndFlagImpl | 完整：`set_loop3_para` | 桩（`ASCENDC_REPORT_NOT_SUPPORT`） | 完整：`set_loop3_para`（AIC 守卫） |
| SetFixPipeAddrImpl | 桩 | 桩 | 桩 |
| Fixpipe 搬运实现 | 完整（FixpipeParamsL300/L311，`copy_matrix_cc_to_cbuf/gm`） | **无任何搬运实现**（文件仅 78 行，只有 SPR 设置） | 完整（FixpipeParamsArch3510） |

### 5.2 实现模式总结

1. **三段式透传**：intf.h 声明（无守卫）→ intf_impl.h 纯转发（无守卫）→ dav 层一行透传 intrinsic。本接口无参数检查、无 CPU debug 分支（intf_impl.h L100 不含 `#if ASCENDC_CPU_DEBUG`）。
2. **版本分发完全依赖 include 宏控**（intf_impl.h L30-54），dav 文件内部零 arch 守卫；dav_5161 一个文件服务 5101/5161/5165/5163 四个 arch。
3. **指令名按架构族分化**：l300/l310/l311/3510 用 `set_quant_pre`；5161 系统一律用 `m_` 前缀（`set_m_quant_pre`/`set_m_fpc`/`set_m_clip_relu`），与此前 MacroSet 分析中 `set_m_relu_alpha`/`set_m_quant_post` 等结论一致——5161 系 SPR 指令走独立命名空间。
4. **l300/l310/l311 三版本文件逐字同构**（仅内部结构体名 L300/L311 不同、557 行 vs 550 行的差异在注释排版），说明是同一套代码模板复制分发。

### 5.3 关键差异点

| # | 差异 | 影响 |
|---|------|------|
| 1 | 5161 用 `set_m_quant_pre`，其余用 `set_quant_pre` | 5101 适配时无需改代码（共享 dav_5161），但语义见待确认问题 2 |
| 2 | 3510 有 `if ASCEND_IS_AIC` 守卫，l300 系没有 | l300 系若为 AIC+AIV 混合核，AIV 上执行 `set_quant_pre` 行为未定义（待确认问题 3） |
| 3 | intf.h L76-77 Fixpipe 搬运重载守卫含 3003/3113 却**不含 3103** | 3103 的 SPR 设置可用但搬运接口编译期不存在（待确认问题 1） |
| 4 | 5161 系无 Fixpipe 搬运实现，SetFixpipePreQuantFlag 却为全架构无守卫声明 | 5101/5161 上调用本接口后无 Fixpipe 流水线消费该配置 |

## 六、底层编译器 API 列表

| API | 签名 | 使用版本 | 定义位置 |
|-----|------|---------|---------|
| `set_quant_pre` | `set_quant_pre(uint64_t config)` | 3003、3103、3113、3510 | CANN 编译器内置 SPR 写 intrinsic（仓库内无定义） |
| `set_m_quant_pre` | `set_m_quant_pre(uint64_t config)` | 5101、5161、5165、5163 | CANN 编译器内置 SPR 写 intrinsic（仓库内无定义，5161 系 m_ 前缀指令族） |

（佐证：`dav_l300\kernel_operator_set_spr_impl.h` L24-30 中 `SetQuantPreImpl/SetFpcImpl/SetLoop3ParaImpl` 等同样直接调用这些 intrinsic；`kernel_reg.h`、`utils/`、`reg_compute/` 目录均无定义。）

## 七、待确认问题

1. **3103 缺 Fixpipe 搬运接口声明——守卫遗漏还是硬件不支持？**
   intf.h L76-77 守卫为 `2201 || 3003 || 3113 || 3510 || 5102`，不含 3103；但 `dav_l310\kernel_operator_fixpipe_impl.h` 完整实现了 `FixpipeL0C2L1Impl/FixpipeL0C2GMImpl`（L360+，`copy_matrix_cc_to_cbuf/gm`，与已放行的 3113 同构）。实现层齐备而声明层不放行，**强烈暗示 intf.h 守卫遗漏**（待与硬件确认 3103 fixpipe 搬运能力后补守卫）。
2. **5101 上 set_m_quant_pre 的实际语义——硬件不支持还是寄存器仍存在？**
   5101 无 Cube 核与 L0C（TOTAL_L0C_SIZE=0，此前分析确认），fixpipe 流水线不存在；dav_5161（共享文件）仍提供完整 `set_m_quant_pre` 透传。5101 适配时需确认：该 SPR 写在 5101 硬件上是否为合法操作，若非法应加 `#if __NPU_ARCH__ == 5101` 桩隔离（保持 5161/5165/5163 不变）。
3. **l300/l310/l311 无 ASCEND_IS_AIC 守卫 vs 3510 有——风格不一致还是架构差异？**
   若 l300 系为 AIC+AIV 混合核，AIV 上执行 `set_quant_pre` 的行为（静默无效/异常/写共享 SPR）需确认；若为纯 Cube 核则无需守卫。
4. **5161 的 SetFixPipeConfigImpl 强制 isUnitFlag=true**（`ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag)` 且不写 FPC[63]），与其他版本（写 FPC[63] UnitFlag 位）行为不一致——是 5161 硬件不支持 UnitFlag 还是代码未同步？（顺带发现的邻居接口差异）
5. **CPU debug 下 intrinsic stub 覆盖**：intf_impl.h L100 无 CPU debug 分支，直接依赖 CPU debug 框架对 `set_quant_pre`/`set_m_quant_pre` 的 stub 模拟；需确认模拟层是否覆盖 `set_m_quant_pre`（5161 系新指令），否则 5101 CPU debug 编译可能失败。
