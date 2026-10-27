# 适配报告：SetFmatrix 接口

## 适配后支持矩阵

| 版本 | Arch | V1 `SetFmatrix(uint16_t,uint16_t,uint8_t*,FmatrixMode)` | V2 `SetFmatrix(SetFMatrixBitModeParams,FmatrixMode)` |
|------|------|----------------------------------------------------------|------------------------------------------------------|
| dav_3510 | 3510 | 维持（完整实现） | 维持（完整实现） |
| dav_l300 | 3003 | 维持（完整实现） | arch 守卫排除 → **完整实现**（新增 `uint64_t` 重载） |
| dav_l310 | 3103 | 维持（完整实现） | arch 守卫排除 → **完整实现**（新增 `uint64_t` 重载） |
| dav_l311 | 3113 | 维持（完整实现） | arch 守卫排除 → **完整实现**（新增 `uint64_t` 重载） |
| dav_5161 | 5101 | 维持（桩函数） | arch 守卫排除 → **桩函数**（V2 API 新增可用，调用已有 `uint64_t` 桩重载） |

### 适配前 V2 arch 守卫

`3510 || 5102`

### 适配后 V2 arch 守卫

`3510 || 5102 || 3003 || 3103 || 3113 || 5101`

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `include/basic_api/kernel_operator_mm_intf.h` | L416: V2 声明 arch 守卫扩展 `+3003/3103/3113/5101` |
| `impl/basic_api/kernel_operator_mm_intf_impl.h` | L584: V2 包装 arch 守卫扩展 `+3003/3103/3113/5101` |
| `impl/basic_api/kernel_operator_mm_base_impl.h` | L813: V2 分发 arch 守卫扩展 `+3003/3103/3113/5101` |
| `impl/basic_api/dav_l300/kernel_operator_mm_impl.h` | L545-557: 新增 `Load3DSetFMatrixCal(uint64_t)` 和 `Load3DSetFMatrixBCal(uint64_t)` 完整实现（从 dav_3510 移植） |
| `impl/basic_api/dav_l310/kernel_operator_mm_impl.h` | L524-536: 新增 `Load3DSetFMatrixCal(uint64_t)` 和 `Load3DSetFMatrixBCal(uint64_t)` 完整实现（从 dav_3510 移植） |
| `impl/basic_api/dav_l311/kernel_operator_mm_impl.h` | L542-554: 新增 `Load3DSetFMatrixCal(uint64_t)` 和 `Load3DSetFMatrixBCal(uint64_t)` 完整实现（从 dav_3510 移植） |

> dav_3510 和 dav_5161 的 impl 文件未修改。

## 适配后各版本原始代码

### dav_3510（参照版本，3510，未修改）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L793-810，未修改）

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

#### `Load3DSetFMatrixCal(uint64_t)` — 完整实现（L812-817，未修改）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L819-836，未修改）

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

#### `Load3DSetFMatrixBCal(uint64_t)` — 完整实现（L838-843，未修改）

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

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L511-526，未修改）

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

#### `Load3DSetFMatrixCal(uint64_t)` — 完整实现（L545-550，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L528-543，未修改）

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

#### `Load3DSetFMatrixBCal(uint64_t)` — 完整实现（L552-557，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix_b(regFMatrix);
    }
}
```

---

### dav_l310（目标版本，3103）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L490-505，未修改）

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

#### `Load3DSetFMatrixCal(uint64_t)` — 完整实现（L524-529，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L507-522，未修改）

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

#### `Load3DSetFMatrixBCal(uint64_t)` — 完整实现（L531-536，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix_b(regFMatrix);
    }
}
```

---

### dav_l311（目标版本，3113）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L508-523，未修改）

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

#### `Load3DSetFMatrixCal(uint64_t)` — 完整实现（L542-547，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix(regFMatrix);
    }
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 完整实现（L525-540，未修改）

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

#### `Load3DSetFMatrixBCal(uint64_t)` — 完整实现（L549-554，**新增**，从 dav_3510 移植）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    if ASCEND_IS_AIC {
        set_fmatrix_b(regFMatrix);
    }
}
```

---

### dav_5161（目标版本，5101，未修改）

#### `Load3DSetFMatrixCal(uint16_t, uint16_t, uint8_t*)` — 桩函数（L399-402，未修改）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixCal(uint64_t)` — 桩函数（L404-407，未修改）

```cpp
__aicore__ inline void Load3DSetFMatrixCal(uint64_t regFMatrix)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixBCal(uint16_t, uint16_t, uint8_t*)` — 桩函数（L409-412，未修改）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint16_t l1H, uint16_t l1W, const uint8_t padList[4])
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```

#### `Load3DSetFMatrixBCal(uint64_t)` — 桩函数（L414-417，未修改）

```cpp
__aicore__ inline void Load3DSetFMatrixBCal(uint64_t regFMatrix)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 3D is not supported!"); });
}
```
