# 案例分析：SetFmatrix 接口跨版本适配

## 接口概述

`SetFmatrix` 用于设置 LoadData3D 的 FMatrix 寄存器（左矩阵/右矩阵的 H/W 及 padding），有两个变体：

| 变体 | 参数类型 | 声明位置 | arch 守卫 |
|------|---------|---------|----------|
| V1 | `(uint16_t l1H, uint16_t l1W, const uint8_t padList[4], const FmatrixMode&)` | `intf.h:414` | 无 |
| V2 | `(const SetFMatrixBitModeParams&, const FmatrixMode&)` | `intf.h:416-418` | `3510 \|\| 5102` |

### 辅助类型

**`FmatrixMode`**（enum class，`kernel_struct_mm.h:463-466`）：
```cpp
enum class FmatrixMode : uint8_t {
    FMATRIX_LEFT = 0,   // 左矩阵 → Load3DSetFMatrixCal
    FMATRIX_RIGHT = 1,  // 右矩阵 → Load3DSetFMatrixBCal
};
```

**`SetFMatrixBitModeParams`**（class，`kernel_operator_mm_bitmode_intf.h:124-142`）：
- 类本身 arch 守卫：`3510 || 5102 || 3003 || 3103 || 3113 || 5101`（**全版本可用**）
- 内部 union 含 `uint64_t config0` 和 `struct SetFMatrixBitModeConfig0 config0BitMode`
- `GetConfig0()` 返回 `uint64_t`，V2 调用链通过此方法获取 64 位寄存器值

## 调用链

### V1 调用链（无 arch 守卫）

```
intf.h:414（声明，无 arch 守卫）
  → intf_impl.h:577-582（公共包装，无 arch 守卫）
    → base_impl.h:803-811（共享分发，无 arch 守卫）
      → fmatrixMode == FMATRIX_LEFT  → Load3DSetFMatrixCal(l1H, l1W, padList)
      → fmatrixMode == FMATRIX_RIGHT → Load3DSetFMatrixBCal(l1H, l1W, padList)
        → dav_XXXX/kernel_operator_mm_impl.h（版本特定实现）
```

### V2 调用链（arch 守卫 `3510 || 5102`，适配后扩展为 `3510 || 5102 || 3003 || 3103 || 3113 || 5101`）

```
intf.h:416-418（声明，arch 守卫，适配后已扩展）
  → intf_impl.h:584-589（公共包装，arch 守卫，适配后已扩展）
    → base_impl.h:813-822（共享分发，arch 守卫，适配后已扩展）
      → fmatrixMode == FMATRIX_LEFT  → Load3DSetFMatrixCal(param.GetConfig0())   // uint64_t 重载
      → fmatrixMode == FMATRIX_RIGHT → Load3DSetFMatrixBCal(param.GetConfig0())  // uint64_t 重载
        → dav_XXXX/kernel_operator_mm_impl.h（版本特定实现）
```

### 共享层分发逻辑（base_impl.h）

**V1 分发**（L803-811，无 arch 守卫）：
```cpp
__aicore__ inline void SetFmatrixImpl(
    uint16_t l1H, uint16_t l1W, const uint8_t padList[4], const FmatrixMode& fmatrixMode)
{
    if (fmatrixMode == FmatrixMode::FMATRIX_LEFT) {
        Load3DSetFMatrixCal(l1H, l1W, padList);
    } else if (fmatrixMode == FmatrixMode::FMATRIX_RIGHT) {
        Load3DSetFMatrixBCal(l1H, l1W, padList);
    }
}
```

**V2 分发**（L813-822，arch 守卫 `3510 || 5102`，适配后已扩展）：
```cpp
__aicore__ inline void SetFmatrixImpl(const SetFMatrixBitModeParams& param, const FmatrixMode& fmatrixMode)
{
    if (fmatrixMode == FmatrixMode::FMATRIX_LEFT) {
        Load3DSetFMatrixCal(param.GetConfig0());
    } else if (fmatrixMode == FmatrixMode::FMATRIX_RIGHT) {
        Load3DSetFMatrixBCal(param.GetConfig0());
    }
}
```

## 支持矩阵

### V1: `SetFmatrix(uint16_t, uint16_t, uint8_t[4], FmatrixMode&)`

| 版本 | Arch | intf.h 声明 | intf_impl.h 包装 | base_impl.h 分发 | `Load3DSetFMatrixCal` (uint16_t,uint16_t,uint8_t*) | `Load3DSetFMatrixBCal` (uint16_t,uint16_t,uint8_t*) | 状态 |
|------|------|------------|------------------|-----------------|---------------------------------------------------|----------------------------------------------------|------|
| dav_3510 | 3510 | ✓ 无守卫 | ✓ 无守卫 | ✓ 无守卫 | **完整** (L793-810) | **完整** (L819-836) | **可用** |
| dav_l300 | 3003 | ✓ 无守卫 | ✓ 无守卫 | ✓ 无守卫 | **完整** (L511-526) | **完整** (L528-543) | **可用** |
| dav_l310 | 3103 | ✓ 无守卫 | ✓ 无守卫 | ✓ 无守卫 | **完整** (L490-505) | **完整** (L507-522) | **可用** |
| dav_l311 | 3113 | ✓ 无守卫 | ✓ 无守卫 | ✓ 无守卫 | **完整** (L508-523) | **完整** (L525-540) | **可用** |
| dav_5161 | 5101 | ✓ 无守卫 | ✓ 无守卫 | ✓ 无守卫 | **桩** (L399-402) | **桩** (L409-412) | **桩函数** |

### V2: `SetFmatrix(SetFMatrixBitModeParams&, FmatrixMode&)`

| 版本 | Arch | intf.h 声明 | intf_impl.h 包装 | base_impl.h 分发 | `Load3DSetFMatrixCal` (uint64_t) | `Load3DSetFMatrixBCal` (uint64_t) | 状态 |
|------|------|------------|------------------|-----------------|----------------------------------|----------------------------------|------|
| dav_3510 | 3510 | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | **完整** (L812-817) | **完整** (L838-843) | **可用** |
| dav_l300 | 3003 | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | **完整** (L545-550，新增) | **完整** (L552-557，新增) | **可用** |
| dav_l310 | 3103 | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | **完整** (L524-529，新增) | **完整** (L531-536，新增) | **可用** |
| dav_l311 | 3113 | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | **完整** (L542-547，新增) | **完整** (L549-554，新增) | **可用** |
| dav_5161 | 5101 | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | ✓ 守卫(适配后扩展) | **桩** (L404-407) | **桩** (L414-417) | **桩函数** |

### 合并支持矩阵

| 版本 | Arch | V1 (uint16_t,uint16_t,uint8_t*,FmatrixMode) | V2 (SetFMatrixBitModeParams,FmatrixMode) |
|------|------|---------------------------------------------|------------------------------------------|
| dav_3510 | 3510 | **完整实现** | **完整实现** |
| dav_l300 | 3003 | **完整实现** | **完整实现**（适配后新增） |
| dav_l310 | 3103 | **完整实现** | **完整实现**（适配后新增） |
| dav_l311 | 3113 | **完整实现** | **完整实现**（适配后新增） |
| dav_5161 | 5101 | **桩函数** | **桩函数** |

## 各版本原始代码

### dav_3510（参照版本，3510）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L793-810）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    if ASCEND_IS_AIC {
        uint64_t regFMatrix = 0;
        regFMatrix |= uint64_t(l1W & 0xFFFF);

        constexpr uint32_t l1HShiftBit = 16;
        regFMatrix |= uint64_t(l1H & 0xFFFF) << l1HShiftBit;

        constexpr uint32_t padNumber = 4;
        constexpr uint32_t padListShiftBit = 8;
        constexpr uint32_t padListShiftBase = 32;
        for (uint32_t i = 0; i < padNumber; i++) {
            regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
        }
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixCal(uint64_t)` — 完整实现（L812-817）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L819-836）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    if ASCEND_IS_AIC {
        uint64_t regFMatrix = 0;
        regFMatrix |= static_cast<uint64_t>(l1W);

        constexpr uint32_t l1HShiftBit = 16;
        regFMatrix |= static_cast<uint64_t>(l1H) << l1HShiftBit;

        constexpr uint32_t padNumber = 4;
        constexpr uint32_t padListShiftBit = 8;
        constexpr uint32_t padListShiftBase = 32;
        for (uint32_t i = 0; i < padNumber; i++) {
            regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
        }
        set_fmatrix_b(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint64_t)` — 完整实现（L838-843）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix_b(regFMatrix);
    }
}
```

---

### dav_l300（目标版本，3003）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L511-526）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= uint64_t(l1W & 0xFFFF);

    constexpr uint32_t l1HShiftBit = 16;
    regFMatrix |= uint64_t(l1H & 0xFFFF) << l1HShiftBit;

    constexpr uint32_t padNumber = 4;
    constexpr uint32_t padListShiftBit = 8;
    constexpr uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix(regFMatrix);
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L510-525）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= (uint64_t)l1W;

    constexpr uint32_t l1HShiftBit = 16;
    regFMatrix |= (uint64_t)l1H << l1HShiftBit;

    constexpr uint32_t padNumber = 4;
    constexpr uint32_t padListShiftBit = 8;
    constexpr uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix_b(regFMatrix);
}
```

> **注意**：dav_l300 适配后已新增 `uint64_t` 重载（L545-550/L552-557），从 dav_3510 移植。

---

### dav_l310（目标版本，3103）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L490-505）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= uint64_t(l1W & 0xFFFF);

    uint32_t l1HShiftBit = 16;
    regFMatrix |= uint64_t(l1H & 0xFFFF) << l1HShiftBit;

    uint32_t padNumber = 4;
    uint32_t padListShiftBit = 8;
    uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix(regFMatrix);
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L507-522）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= (uint64_t)l1W;

    uint32_t l1HShiftBit = 16;
    regFMatrix |= (uint64_t)l1H << l1HShiftBit;

    uint32_t padNumber = 4;
    uint32_t padListShiftBit = 8;
    uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix_b(regFMatrix);
}
```

> **注意**：dav_l310 适配后已新增 `uint64_t` 重载（L524-529/L531-536），从 dav_3510 移植。常量使用普通局部变量（非 `constexpr`）。

---

### dav_l311（目标版本，3113）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L508-523）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= uint64_t(l1W & 0xFFFF);

    uint32_t l1HShiftBit = 16;
    regFMatrix |= uint64_t(l1H & 0xFFFF) << l1HShiftBit;

    uint32_t padNumber = 4;
    uint32_t padListShiftBit = 8;
    uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix(regFMatrix);
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L507-522）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    uint64_t regFMatrix = 0;
    regFMatrix |= (uint64_t)l1W;

    uint32_t l1HShiftBit = 16;
    regFMatrix |= (uint64_t)l1H << l1HShiftBit;

    uint32_t padNumber = 4;
    uint32_t padListShiftBit = 8;
    uint32_t padListShiftBase = 32;
    for (uint32_t i = 0; i < padNumber; i++) {
        regFMatrix |= uint64_t(padList[i] & 0xFF) << (padListShiftBase + i * padListShiftBit);
    }
    set_fmatrix_b(regFMatrix);
}
```

> **注意**：dav_l311 适配后已新增 `uint64_t` 重载（L542-547/L549-554），从 dav_3510 移植。代码与 dav_l310 完全一致（含非 `constexpr` 风格）。

---

### dav_5161（目标版本，5101）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 桩函数（L399-402）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixCal(uint64_t)` — 桩函数（L404-407）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 桩函数（L409-412）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixBCal(uint64_t)` — 桩函数（L414-417）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

> **注意**：dav_5161 的 `uint64_t` 重载为死代码——V2 调用链（`SetFmatrixImpl(SetFMatrixBitModeParams, FmatrixMode)`）被 arch 守卫 `3510 || 5102` 排除，不会编译。

## 代码级实现差异分析

### 风格对比表

| 特征 | dav_3510 (3510) | dav_l300 (3003) | dav_l310 (3103) | dav_l311 (3113) | dav_5161 (5101) |
|------|-----------------|----------------|----------------|----------------|----------------|
| `if ASCEND_IS_AIC` 守卫 | ✓ 有 | ✗ 无 | ✗ 无 | ✗ 无 | ✗ 无（桩函数） |
| `constexpr` 常量 | ✓ 使用 | ✓ 使用 | ✗ 普通局部变量 | ✗ 普通局部变量 | N/A（桩函数） |
| cast 风格 (Cal) | `uint64_t(x & 0xFFFF)` | `uint64_t(x & 0xFFFF)` | `uint64_t(x & 0xFFFF)` | `uint64_t(x & 0xFFFF)` | N/A |
| cast 风格 (BCal, l1W) | `static_cast<uint64_t>` | `(uint64_t)l1W` | `(uint64_t)l1W` | `(uint64_t)l1W` | N/A |
| cast 风格 (BCal, l1H) | `static_cast<uint64_t>` | `(uint64_t)l1H` | `(uint64_t)l1H` | `(uint64_t)l1H` | N/A |
| `l1W` 掩码 (Cal) | `& 0xFFFF` | `& 0xFFFF` | `& 0xFFFF` | `& 0xFFFF` | N/A |
| `l1W` 掩码 (BCal) | 无掩码 | 无掩码 | 无掩码 | 无掩码 | N/A |
| 硬件指令 | `set_fmatrix` / `set_fmatrix_b` | `set_fmatrix` / `set_fmatrix_b` | `set_fmatrix` / `set_fmatrix_b` | `set_fmatrix` / `set_fmatrix_b` | 无（桩函数） |
| `uint64_t` 重载 | ✓ 完整 | ✓ 完整（适配后新增） | ✓ 完整（适配后新增） | ✓ 完整（适配后新增） | ✓ 桩 |
| 整体状态 | **完整实现** | **完整实现** | **完整实现** | **完整实现** | **桩函数** |

### 实现模式总结

**模式 A — 3510 参照版（含 AIC 守卫 + constexpr + static_cast）**：
- 独有 `if ASCEND_IS_AIC` 守卫，确保仅在 AIC 核上执行寄存器写入
- 使用 `constexpr` 编译期常量
- BCal 中使用 `static_cast<uint64_t>`（C++ 风格）
- 同时提供 `(uint16_t,uint16_t,uint8_t*)` 和 `(uint64_t)` 两种重载

**模式 B — l300 版（无 AIC 守卫 + constexpr + C-style cast）**：
- 无 `if ASCEND_IS_AIC` 守卫（架构本身可能仅 AIC 或不需要区分）
- 使用 `constexpr` 编译期常量
- BCal 中使用 C 风格转换 `(uint64_t)`
- 仅提供 `(uint16_t,uint16_t,uint8_t*)` 重载

**模式 C — l310/l311 版（无 AIC 守卫 + 无 constexpr + C-style cast）**：
- 无 `if ASCEND_IS_AIC` 守卫
- 常量使用普通局部变量（非 `constexpr`，可能有微小性能差异或编译器优化差异）
- BCal 中使用 C 风格转换
- 代码完全一致（l310 和 l311 逐字符相同）

**模式 D — 5161 版（全桩函数）**：
- 所有 4 个重载均为桩函数，消息为 `"LoadData 3D is not supported!"`
- 含 `uint64_t` 重载但为死代码（V2 不编译）

### 关键差异点

1. **`if ASCEND_IS_AIC` 守卫**：仅 dav_3510 有此守卫。l300/l310/l311 不区分 AIC/AIV，意味着在 AIV 核上调用也会执行寄存器写入（可能因架构差异在 AIV 上是空操作或安全操作）。

2. **`constexpr` vs 普通变量**：l300 和 3510 使用 `constexpr`，l310/l311 使用普通局部变量。功能等价，但 `constexpr` 更规范（编译期常量）。

3. **cast 风格**：3510 在 BCal 中使用 `static_cast`，其余版本使用 C 风格转换。功能等价，`static_cast` 更安全。

4. **`uint64_t` 重载的存在性**：
   - 3510：有（V2 需要，且 V2 编译）
   - l300/l310/l311：无（V2 不编译，不需要）
   - 5161：有但为死代码（V2 不编译，但桩函数仍存在）

5. **5161 桩函数**：所有 FMatrix 函数均为桩，报错 "LoadData 3D is not supported!"，表明 5101 硬件不支持 LoadData 3D 相关 ISA。

## 待确认问题

### 1. dav_5161 的 V1 桩函数：硬件不支持还是待补全？

**已确认**：5101 不支持 `set_fmatrix` 和 `set_fmatrix_b`，V1 和 V2 均打桩。维持现状。

### 2. dav_5161 的 `uint64_t` 重载桩函数是否为死代码？

**已确认**：适配后 V2 守卫已扩展到 5101，`(uint64_t)` 重载不再是死代码，V2 API 调用时将触发桩函数。维持现状。

### 3. V2 守卫扩展问题：`SetFMatrixBitModeParams` 类 vs V2 API 守卫不一致

**已解决**：适配后 V2 API 守卫已从 `3510 || 5102` 扩展为 `3510 || 5102 || 3003 || 3103 || 3113 || 5101`，与 `SetFMatrixBitModeParams` 类的守卫一致。dav_l300/l310/l311 已补充 `uint64_t` 重载完整实现（从 dav_3510 移植），dav_5161 维持桩函数。

### 4. l310/l311 与 l300 的 `constexpr` 差异是否有意？

l300 使用 `constexpr`，l310/l311 使用普通局部变量。两者功能等价，但风格不一致。

- **若有意**：可能因 l310/l311 编译器对 `constexpr` 处理有差异。
- **若无意**：l310/l311 是从 l300 或 3510 派生时丢失了 `constexpr`。

**推测**：风格差异，无功能影响，不需要修改。**需用户确认是否统一**。

### 5. l300/l310/l311 缺少 `if ASCEND_IS_AIC` 守卫是否有风险？

3510 在所有 FMatrix 函数中使用 `if ASCEND_IS_AIC` 守卫，l300/l310/l311 没有。

- **若有风险**：在 AIV 核上调用 `set_fmatrix`/`set_fmatrix_b` 可能导致未定义行为。
- **若安全**：这些架构可能是纯 Cube 枸（无 AIV），或 `set_fmatrix` 在 AIV 上为空操作。

**推测**：l300/l310/l311 可能是纯 AIC 架构或 `if ASCEND_IS_AIC` 在这些架构上恒为 true。**需用户确认**。
