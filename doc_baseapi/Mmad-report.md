# Mmad 接口适配报告

## 适配后支持矩阵

| 版本 | Arch | V1 (MmadParams) | V2 (MmadParams+bias) | V3 (BitModeParams) | V4 (BitModeParams+bias) | 变更说明 |
|------|------|-----------------|----------------------|--------------------|-----------------------|---------|
| dav_l300 | 3003 | 维持 | 维持 | **新增** 完整 | **新增** 完整 | 从 3510 移植，移除 mad_mx |
| dav_l310 | 3103 | 维持 | 维持 | **新增** 完整 | **新增** 完整 | 从 3510 移植，移除 mad_mx |
| dav_l311 | 3113 | 维持 | 维持 | **新增** 完整 | **新增** 完整 | 从 3510 移植，移除 mad_mx |
| dav_5161 | 5101 | 维持 桩 | 维持 桩 | **新增** 桩 | **新增** 桩 | 硬件不支持 mad，V3/V4 打桩 |
| dav_3510 | 3510 | 维持 | 维持 | 维持 完整 | 维持 完整 | 参照版本，未修改 |

## 修改的文件清单

### 新建文件（4 个）

| 文件 | 说明 |
|------|------|
| `impl/basic_api/dav_l300/kernel_operator_mm_bitmode_impl.h` | l300 的 MmadBitModeConfig0 位域结构体（含 fmOffset） |
| `impl/basic_api/dav_l310/kernel_operator_mm_bitmode_impl.h` | l310 的 MmadBitModeConfig0 位域结构体（含 fmOffset） |
| `impl/basic_api/dav_l311/kernel_operator_mm_bitmode_impl.h` | l311 的 MmadBitModeConfig0 位域结构体（含 fmOffset） |
| `impl/basic_api/dav_5161/kernel_operator_mm_bitmode_impl.h` | 5161 的 MmadBitModeConfig0 位域结构体（无 fmOffset，与 3510 布局一致） |

### 修改文件（8 个）

| 文件 | 修改内容 |
|------|---------|
| `include/basic_api/kernel_operator_mm_bitmode_intf.h:145` | MmadBitModeParams 类声明 arch 守卫扩展：3510 → 3510/3003/3103/3113/5101/5161/5165/5163 |
| `impl/basic_api/kernel_operator_mm_bitmode_intf_impl.h:149` | MmadBitModeParams 方法实现 arch 守卫扩展：同上 |
| `impl/basic_api/kernel_operator_mm_bitmode_struct.h:27-37` | 新增 3003/3103/3113 和 5101/5161/5165/5163 的 #elif 分支 |
| `include/basic_api/kernel_operator_mm_intf.h:296` | V3/V4 Mmad 声明 arch 守卫扩展：同上 |
| `impl/basic_api/kernel_operator_mm_intf_impl.h:411` | V3/V4 Mmad 包装 arch 守卫扩展 + 拆分 MmadMx 到独立 3510 守卫 |
| `impl/basic_api/kernel_operator_mm_base_impl.h:511` | V3/V4 MmadImpl arch 守卫扩展 + 拆分 MmadMxImpl 到独立 3510 守卫 |
| `impl/basic_api/dav_l300/kernel_operator_mm_impl.h:490-506` | 新增 V3/V4 MmadCal 实现（仅 mad，无 mad_mx） |
| `impl/basic_api/dav_l310/kernel_operator_mm_impl.h:469-485` | 新增 V3/V4 MmadCal 实现（仅 mad，无 mad_mx） |
| `impl/basic_api/dav_l311/kernel_operator_mm_impl.h:487-503` | 新增 V3/V4 MmadCal 实现（仅 mad，无 mad_mx） |
| `impl/basic_api/dav_5161/kernel_operator_mm_impl.h:359-370` | 新增 V3/V4 MmadCal 桩函数 |

## 适配后各版本原始代码

### dav_l300（3003）— V3/V4 新增

#### MmadBitModeConfig0 位域结构体（新建文件 dav_l300/kernel_operator_mm_bitmode_impl.h）

```cpp
struct MmadBitModeConfig0 {
    uint64_t m : 12;
    uint64_t k : 12;
    uint64_t n : 12;
    uint64_t fmOffset : 8;
    uint64_t : 11;
    uint64_t unitFlag : 2;
    uint64_t : 4;
    uint64_t disableGemv : 1;
    uint64_t cmatrixSource : 1;
    uint64_t cmatrixInitVal : 1;

    __aicore__ inline void Init(const MmadParams& mmadParams_)
    {
        m = mmadParams_.m;
        k = mmadParams_.k;
        n = mmadParams_.n;
        fmOffset = mmadParams_.fmOffset;
        unitFlag = mmadParams_.unitFlag;
        disableGemv = false;
        cmatrixSource = mmadParams_.cmatrixSource;
        cmatrixInitVal = mmadParams_.cmatrixInitVal;
    }
    __aicore__ inline void SetM(uint16_t m_) { m = m_; };
    __aicore__ inline void SetK(uint16_t k_) { k = k_; };
    __aicore__ inline void SetN(uint16_t n_) { n = n_; };
    __aicore__ inline void SetUnitFlag(uint8_t unitFlag_) { unitFlag = unitFlag_; };
    __aicore__ inline void SetDisableGemv(bool disableGemv_) { disableGemv = disableGemv_; };
    __aicore__ inline void SetCmatrixSource(bool cmatrixSource_) { cmatrixSource = cmatrixSource_; };
    __aicore__ inline void SetCmatrixInitVal(bool cmatrixInitVal_) { cmatrixInitVal = cmatrixInitVal_; };
    __aicore__ inline uint16_t GetM() const { return m; };
    __aicore__ inline uint16_t GetK() const { return k; };
    __aicore__ inline uint16_t GetN() const { return n; };
    __aicore__ inline uint8_t GetUnitFlag() const { return unitFlag; };
    __aicore__ inline bool GetDisableGemv() const { return disableGemv; };
    __aicore__ inline bool GetCmatrixSource() const { return cmatrixSource; };
    __aicore__ inline bool GetCmatrixInitVal() const { return cmatrixInitVal; };
};
```

> 与 3510 的差异：增加 `fmOffset : 8` 位域（[36:43]），padding 从 19 位改为 11+4 位；Init() 中 `disableGemv = false`（因 MmadParams::disableGemv 在 3003 不存在）。

#### V3 MmadCal — 新增完整实现（dav_l300/kernel_operator_mm_impl.h:490-496）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mad(c, a, b, mmadParams.GetConfig0());
    }
}
```

#### V4 MmadCal — 新增完整实现（dav_l300/kernel_operator_mm_impl.h:498-506）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mmadParams.SetCmatrixInitVal(false);
        mad(c, a, b, bias, mmadParams.GetConfig0());
    }
}
```

> 与 3510 的差异：移除 isMx/mad_mx 分支和 CPU debug 双路径；V4 使用 `mad(c, a, b, bias, config)` 5 参数形式（匹配 V2 风格），不使用 xd 寄存器打包。

#### V1/V2 MmadCal — 维持不变

---

### dav_l310（3103）— V3/V4 新增

#### MmadBitModeConfig0 位域结构体（新建文件 dav_l310/kernel_operator_mm_bitmode_impl.h）

```cpp
struct MmadBitModeConfig0 {
    uint64_t m : 12;
    uint64_t k : 12;
    uint64_t n : 12;
    uint64_t fmOffset : 8;
    uint64_t : 11;
    uint64_t unitFlag : 2;
    uint64_t : 4;
    uint64_t disableGemv : 1;
    uint64_t cmatrixSource : 1;
    uint64_t cmatrixInitVal : 1;

    __aicore__ inline void Init(const MmadParams& mmadParams_)
    {
        m = mmadParams_.m;
        k = mmadParams_.k;
        n = mmadParams_.n;
        fmOffset = mmadParams_.fmOffset;
        unitFlag = mmadParams_.unitFlag;
        disableGemv = false;
        cmatrixSource = mmadParams_.cmatrixSource;
        cmatrixInitVal = mmadParams_.cmatrixInitVal;
    }
    // ... Set/Get 方法与 l300 完全相同
};
```

> 与 l300 完全相同。

#### V3 MmadCal — 新增完整实现（dav_l310/kernel_operator_mm_impl.h:469-475）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mad(c, a, b, mmadParams.GetConfig0());
    }
}
```

#### V4 MmadCal — 新增完整实现（dav_l310/kernel_operator_mm_impl.h:477-485）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mmadParams.SetCmatrixInitVal(false);
        mad(c, a, b, bias, mmadParams.GetConfig0());
    }
}
```

> 与 l300 完全相同。

#### V1/V2 MmadCal — 维持不变

---

### dav_l311（3113）— V3/V4 新增

#### MmadBitModeConfig0 位域结构体（新建文件 dav_l311/kernel_operator_mm_bitmode_impl.h）

> 与 l300/l310 完全相同。

#### V3 MmadCal — 新增完整实现（dav_l311/kernel_operator_mm_impl.h:487-493）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mad(c, a, b, mmadParams.GetConfig0());
    }
}
```

#### V4 MmadCal — 新增完整实现（dav_l311/kernel_operator_mm_impl.h:495-503）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mmadParams.SetCmatrixInitVal(false);
        mad(c, a, b, bias, mmadParams.GetConfig0());
    }
}
```

> 与 l300/l310 完全相同。

#### V1/V2 MmadCal — 维持不变

---

### dav_5161（5101）— V3/V4 新增桩函数

#### MmadBitModeConfig0 位域结构体（新建文件 dav_5161/kernel_operator_mm_bitmode_impl.h）

```cpp
struct MmadBitModeConfig0 {
    uint64_t m : 12;
    uint64_t k : 12;
    uint64_t n : 12;
    uint64_t : 20;
    uint64_t unitFlag : 2;
    uint64_t : 4;
    uint64_t disableGemv : 1;
    uint64_t cmatrixSource : 1;
    uint64_t cmatrixInitVal : 1;

    __aicore__ inline void Init(const MmadParams& mmadParams_)
    {
        m = mmadParams_.m;
        k = mmadParams_.k;
        n = mmadParams_.n;
        unitFlag = mmadParams_.unitFlag;
        disableGemv = false;
        cmatrixSource = mmadParams_.cmatrixSource;
        cmatrixInitVal = mmadParams_.cmatrixInitVal;
    }
    // ... Set/Get 方法与 3510 相同
};
```

> 与 3510 布局一致（无 fmOffset），Init() 中 `disableGemv = false`（因 MmadParams::disableGemv 在 5101 不存在）。

#### V1 MmadCal — 桩函数（维持）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

#### V2 MmadCal — 桩函数（维持）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

#### V3 MmadCal — 新增桩函数（dav_5161/kernel_operator_mm_impl.h:359-363）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

#### V4 MmadCal — 新增桩函数（dav_5161/kernel_operator_mm_impl.h:365-370）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

---

### dav_3510（3510）— 维持不变

#### V3 MmadCal — 完整实现（维持，L540-572）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        constexpr bool isMx = SupportType<...>();
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        if constexpr (isMx) {
            mad_mx(c, ...);
        } else {
            mad(c, a, b, mmadParams.GetM(), ...);
        }
#else
        if constexpr (isMx) {
            mad_mx(c, ...);
        } else {
            mad(c, a, b, mmadParams.GetConfig0());
        }
#endif
    }
}
```

#### V4 MmadCal — 完整实现（维持，L574-610）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mmadParams.SetCmatrixInitVal(false);
        constexpr bool isMx = SupportType<...>();
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        if constexpr (isMx) { mad_mx(...); } else { mad(c, a, b, bias, ...); }
#else
        uint64_t xd = ((uint64_t)c) & 0xffffffffULL | ((bias & 0xffffffffULL) << 32);
        if constexpr (isMx) { mad_mx(xd, ...); } else { mad(xd, a, b, mmadParams.GetConfig0()); }
#endif
    }
}
```

> 3510 的所有 MmadCal（V1-V4）和 MmadMxCal 均维持不变。

---

## 适配关键设计决策

### 1. MmadBitModeConfig0 位域布局差异

| 位域 | 3510 / 5161 | l300/l310/l311 | 说明 |
|------|-------------|----------------|------|
| [0:11] | m | m | 相同 |
| [12:23] | k | k | 相同 |
| [24:35] | n | n | 相同 |
| [36:43] | **padding** | **fmOffset** | l300/l310/l311 的 V1/V2 config 包含 fmOffset，必须保留 |
| [44:54] | padding | padding | 相同 |
| [55:56] | unitFlag | unitFlag | 相同 |
| [57:60] | padding | padding | 相同 |
| [61] | disableGemv | disableGemv | 相同（Init 中设为 false） |
| [62] | cmatrixSource | cmatrixSource | 相同 |
| [63] | cmatrixInitVal | cmatrixInitVal | 相同 |

### 2. V3/V4 MmadCal 与 3510 的差异

| 特征 | 3510 V3/V4 | l300/l310/l311 V3/V4 | 5161 V3/V4 |
|------|-----------|---------------------|------------|
| 实现类型 | 完整 | 完整 | 桩 |
| isMx/mad_mx 分支 | 有 | **移除** | 无 |
| CPU debug 双路径 | 有 | **移除** | 无 |
| V4 bias 传递 | xd 寄存器打包 | 独立参数 | 无 |
| if ASCEND_IS_AIC | 有 | **保留** | 无 |
| SetCmatrixInitVal(false) | V4 有 | **保留** | 无 |

### 3. MmadMx 隔离

原始代码中 V3/V4 MmadImpl 与 MmadMxImpl 在同一个 `#if __NPU_ARCH__ == 3510` 块内。适配后拆分为两个独立守卫：
- V3/V4 MmadImpl：`#if 3510 || 3003 || 3103 || 3113 || 5101 || 5161 || 5165 || 5163`
- MmadMxImpl：`#if 3510`（仅 3510，因 mad_mx 仅 3510 支持）

intf_impl.h 中的 MmadMx 包装也做了同样的拆分。
