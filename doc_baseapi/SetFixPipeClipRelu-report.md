# 适配报告：SetFixPipeClipRelu 接口跨版本适配

> 接口：`__aicore__ inline void SetFixPipeClipRelu(uint64_t config)`
> 声明：`asc-devkit/include/basic_api/kernel_operator_fixpipe_intf.h` L71（无 arch 守卫，本次未修改）
>
> **确认方案**：5101 不支持 M_CLIP_RELU 寄存器，新增打桩排除此版本；5161 是支持的。

## 适配后支持矩阵

| 版本 | Arch | 路径（Fixpipe，唯一变体） | 适配前 | 适配后 |
|------|------|--------------------------|--------|--------|
| dav_l300 | 3003 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_l310 | 3103 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_l311 | 3113 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_5161 | 5101 | `SetFixPipeClipReluImpl` | 完整（`set_m_clip_relu`） | **桩（新增，本次修改）** |
| dav_5161 | 5161/5165/5163 | `SetFixPipeClipReluImpl` | 完整（`set_m_clip_relu`） | **维持**（完整，`set_m_clip_relu`） |
| dav_3510 | 3510（参照） | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_m300 | 3002 | `SetFixPipeClipReluImpl`（v2 文件） | 完整（`set_fix_clip_relu`） | **维持**（完整） |
| dav_m510 | 5102 | `SetFixPipeClipReluImpl` | 完整（`set_fix_clip_relu`） | **维持**（完整） |
| dav_c100 | 1001 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_m200 | 2002 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_c220 | 2201 | `SetFixPipeClipReluImpl`（v2 文件） | 桩（NOT_SUPPORT） | **维持**（桩） |
| dav_m310 | 3102 | `SetFixPipeClipReluImpl` | 桩（NOT_SUPPORT） | **维持**（桩） |

说明：

- 本接口为单一变体、单一路径（声明 L71 → 包装 `fixpipe_intf_impl.h` L102 → `#elif` include 链分派 → 各版本 `SetFixPipeClipReluImpl`），无 L0A/L0B 分路径区分。
- 调用链各层守卫状态：声明（L71，无守卫）、包装（L102，无守卫）、分发（`fixpipe_intf_impl.h` L30-54 `#elif __NPU_ARCH__` include 链）——本次均未修改；**版本区分在实现文件内部以 `#if`/`#else` 完成**（dav_5161 为 5101/5161/5165/5163 四 arch 共享目录）。
- 5165/5163 方案未提及，按"维持不变的版本不修改"原则保留完整实现（与适配前行为一致）。
- 桩函数语义：CPU DEBUG 下 `KERNEL_LOG(KERNEL_ERROR)` + `raise(SIGABRT)`（`kernel_log.h` L88-94）；device 侧为空宏（`kernel_log.h` L295），静默无操作。

## 修改的文件清单

| # | 文件 | 修改内容 |
|---|------|---------|
| 1 | `asc-devkit/impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h` | `SetFixPipeClipReluImpl`（原 L29 单行完整实现）改为 arch 守卫双分支：新增 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)` 桩函数分支，原 `set_m_clip_relu` 完整实现**逐字符保留**于 `#else` 分支。文件由 78 行变为 86 行（+8 行） |

未修改的文件（核实确认无需改动）：

| 文件 | 原因 |
|------|------|
| `include/basic_api/kernel_operator_fixpipe_intf.h` | L71 声明无 arch 守卫，全版本可见 —— 5101 打桩后接口仍可编译（桩即"存在但报不支持"），符合方案"新增打桩排除"而非"删除声明" |
| `impl/basic_api/kernel_operator_fixpipe_intf_impl.h` | L102 公共包装为 arch 无关透传，无需修改 |
| 其余 10 个 arch 的 `kernel_operator_fixpipe(_v2)_impl.h` | 方案未涉及，全部维持 |
| `dav_5161/kernel_operator_macro_impl.h` 等同目录其他文件 | 核实 `set_m_clip_relu` 全仓库仅 fixpipe_impl.h 一处调用，无其他引用点需同步修改 |

修改后代码（dav_5161/kernel_operator_fixpipe_impl.h L28-37）：

```cpp
namespace AscendC {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
#else
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config) { set_m_clip_relu(config); }
#endif
```

## 适配后各版本原始代码

### dav_5161（5101/5161/5165/5163 共享）— **已修改** ✏️

> 文件：`impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h` L29-37。原 L29 单行实现拆分为 arch 守卫双分支。

#### SetFixPipeClipReluImpl — 5101：新增桩函数（L30-34）；5161/5165/5163：完整实现维持（L36）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
#else
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config) { set_m_clip_relu(config); }
#endif
```

- **5101 分支（新增）**：`ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu")` 统一桩格式，与 dav_l300/l310/l311/3510 等现有桩逐字符一致；`(void)(config)` 消除未用参数告警。
- **`#else` 分支（未修改）**：原实现逐字符保留，仅移入分支内。
- 守卫风格 `#if defined(__NPU_ARCH__) && (...)` 与同目录先例一致（`kernel_operator_macro_impl.h` L573 以同风格排除 5101 的 `MmadMxFixCal`）。

---

### dav_l300（目标版本，3003）— 未修改 🔒

> 文件：`impl/basic_api/dav_l300/kernel_operator_fixpipe_impl.h` L29-33。

#### SetFixPipeClipReluImpl — 桩函数（维持）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

---

### dav_l310（目标版本，3103）— 未修改 🔒

> 文件：`impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h` L29-33。

#### SetFixPipeClipReluImpl — 桩函数（维持）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

---

### dav_l311（目标版本，3113）— 未修改 🔒

> 文件：`impl/basic_api/dav_l311/kernel_operator_fixpipe_impl.h` L29-33。

#### SetFixPipeClipReluImpl — 桩函数（维持）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

---

### dav_3510（参照版本，3510）— 未修改 🔒

> 文件：`impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h` L29-33。

#### SetFixPipeClipReluImpl — 桩函数（维持）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

---

### 其他版本（均未修改 🔒）

#### dav_m300（3002）— 完整实现维持（`kernel_operator_fixpipe_v2_impl.h` L28）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config) { set_fix_clip_relu(config); }
```

#### dav_m510（5102）— 完整实现维持（`kernel_operator_fixpipe_impl.h` L29）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config) { set_fix_clip_relu(config); }
```

#### dav_c100（1001）— 桩函数维持（`kernel_operator_fixpipe_impl.h` L53-57）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

#### dav_m200（2002）— 桩函数维持（`kernel_operator_fixpipe_impl.h` L54-58）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

#### dav_c220（2201）— 桩函数维持（`kernel_operator_fixpipe_v2_impl.h` L28-32）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

#### dav_m310（3102）— 桩函数维持（`kernel_operator_fixpipe_impl.h` L66-70）

```cpp
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
```

---

## 验证记录

1. **改动正确性**：修改后全文重读核对 —— `#if`（L29）/`#else`（L35）/`#endif`（L37）配对平衡，位于 `namespace AscendC`（L28）内；新增桩格式与全仓库现有桩一致；`#else` 分支原实现逐字符未变；文件其余部分（L39-86 的 `SetFixPipeAddrImpl`/`SetFixPipeConfigImpl`/`SetFixpipeNz2ndFlagImpl`/`SetFixpipePreQuantFlagImpl`）未受影响。
2. **编译可用性**：`ASCENDC_REPORT_NOT_SUPPORT` 在本文件已有 4 处使用（L44/L51/L61/L76），宏可用性无问题；`SetFixPipeClipReluImpl` 在 5101/5161/5165/5163 四个 arch 下均有定义（桩或完整），`fixpipe_intf_impl.h` L102 包装的调用不会出现未定义符号。
3. **引用点核查**：`set_m_clip_relu` 全仓库仅 `dav_5161/kernel_operator_fixpipe_impl.h` 一处调用（同目录 `kernel_operator_macro_impl.h`、`kernel_operator_set_spr_impl.h` 等已逐一核查，无其他引用），无需同步修改其他文件。
4. **方案符合性**：仅 5101 行为改变（完整 → 桩）；5161/5165/5163 及其余全部 arch 行为不变；声明与包装层未动，符合"严格按照确认方案、不自行假设、维持不变不修改"的要求。
