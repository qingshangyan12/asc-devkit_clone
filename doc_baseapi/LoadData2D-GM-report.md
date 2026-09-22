# 适配报告：LoadData 2DV2+Nd2Nz GM→L1 接口（L109）

## 适配后支持矩阵

| 版本 | Arch | 声明守卫 | 包装函数 | LoadDataImpl(Nd2NzParamsV2) | LoadData2DGM2L1Cal(Nd2NzParamsV2) | 适配前 → 适配后 |
|------|------|---------|---------|---------------------------|----------------------------------|----------------|
| dav_3510 | 3510 | ❌不包含 | ❌不编译 | 未定义 | **桩函数**（L451-458） | 未定义 → **桩函数** |
| dav_l300 | 3003 | ❌不包含 | ❌不编译 | 未定义 | **桩函数**（L312-317） | 未定义 → **桩函数** |
| dav_l310 | 3103 | ❌不包含 | ❌不编译 | 未定义 | **桩函数**（L315-320） | 未定义 → **桩函数** |
| dav_l311 | 3113 | ✅包含 | ✅编译 | ✅完整 | **完整实现**（L195-221） | 未定义 → **完整实现** |
| dav_5161 | 5101 | ✅包含 | ✅编译 | ✅完整 | **完整实现**（L229-255） | 未定义 → **完整实现** |
| dav_m510 | 5102 | ✅包含（维持） | ✅（维持） | ✅（维持） | ✅完整（维持） | 维持 |

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `include/basic_api/kernel_operator_mm_intf.h` | L108 arch 守卫扩展：`5102` → `5102\|\|3113\|\|5101\|\|5161\|\|5165\|\|5163` |
| `impl/basic_api/kernel_operator_mm_intf_impl.h` | L138 arch 守卫扩展：同上 |
| `impl/basic_api/kernel_operator_mm_base_impl.h` | L341 arch 守卫扩展：同上 |
| `impl/basic_api/dav_3510/kernel_operator_mm_impl.h` | 新增 `LoadData2DGM2L1Cal(Nd2NzParamsV2)` 桩函数（L451-458） |
| `impl/basic_api/dav_l300/kernel_operator_mm_impl.h` | 新增 `LoadData2DGM2L1Cal(Nd2NzParamsV2)` 桩函数（L312-317） |
| `impl/basic_api/dav_l310/kernel_operator_mm_impl.h` | 新增 `LoadData2DGM2L1Cal(Nd2NzParamsV2)` 桩函数（L315-320） |
| `impl/basic_api/dav_l311/kernel_operator_mm_impl.h` | 新增 `LoadData2DGM2L1Cal(Nd2NzParamsV2)` 完整实现（L195-221，从 dav_m510 移植） |
| `impl/basic_api/dav_5161/kernel_operator_mm_impl.h` | 新增 `LoadData2DGM2L1Cal(Nd2NzParamsV2)` 完整实现（L229-255，从 dav_m510 移植） |

## 适配后各版本原始代码

### dav_3510（参照版本，**已修改**）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 桩函数（L451-458，**新增**）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 with Nd2NzParamsV2 is not supported on current device!");
    });
}
```

---

### dav_l300（目标版本，3003，**已修改**）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 桩函数（L312-317，**新增**）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 with Nd2NzParamsV2 is not supported!"); });
}
```

---

### dav_l310（目标版本，3103，**已修改**）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 桩函数（L315-320，**新增**）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 with Nd2NzParamsV2 is not supported!"); });
}
```

---

### dav_l311（目标版本，3113，**已修改**）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 完整实现（L195-221，**新增**，从 dav_m510 移植）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cbuf_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}
```

---

### dav_5161（目标版本，5101，**已修改**）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 完整实现（L229-255，**新增**，从 dav_m510 移植）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cbuf_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}
```

---

### dav_m510（5102，维持不变）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 完整实现（L211-237，未修改）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cbuf_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}
```
