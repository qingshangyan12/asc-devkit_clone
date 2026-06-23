# C API vector_datamove_impl 中涉及 postupdate 的 `__ubuf__` 地址传参方式统计

> 统计范围: `impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl/`
> 统计内容: 涉及 postupdate 的接口及其 `__ubuf__` 地址参数是传递 **指针** (`T*`) 还是 **指针引用** (`T*&`)

---

## 一、LOAD 类接口 (reg_load/)

### 1.1 asc_loadalign — vlds + POST_UPDATE

所有 postupdate 变体均通过 `__ubuf__ T*&` (指针引用) 传入 `src` 地址，因为 `vlds` 的 POST_UPDATE 模式会硬件修改 `src` 指针值。

| 接口 | `__ubuf__` 地址传参方式 | 覆盖类型 |
|------|----------------------|---------|
| `asc_loadalign_postupdate_impl` (NORM) | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float, int64_t, uint64_t, bool |
| `asc_loadalign_brc_postupdate_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float |
| `asc_loadalign_upsample_postupdate_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, bool |
| `asc_loadalign_downsample_postupdate_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, bool |
| `asc_loadalign_unpack_postupdate_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float |
| `asc_loadalign_unpack_postupdate_v2_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t |
| `asc_loadalign_brc_postupdate_v2_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float |
| `asc_loadalign_brc_postupdate_v3_impl` | `__ubuf__ T*& src` | int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float |
| `asc_loadalign_deintlv_postupdate_impl` | `__ubuf__ T*& src` | int8_t, uint8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, hifloat8_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float |
| `asc_loadalign_postupdate_impl` (vsldb) | `__ubuf__ T*& src` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, fp8_e5m2_t, fp8_e8m0_t, fp4x2_e1m2_t, int4b_t, fp4x2_e2m1_t, half, float |

### 1.2 asc_loadunalign — vldus/vldu + POST_UPDATE

与 loadalign 不同，loadunalign 的 postupdate 接口中 `__ubuf__` 地址**传指针** (`T*`)，而非指针引用。

| 接口 | `__ubuf__` 地址传参方式 | 底层指令 | 覆盖类型 |
|------|----------------------|---------|---------|
| `asc_loadunalign_postupdate_impl` (vldus) | `__ubuf__ T* src1` | `vldus(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, int64_t, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, fp8_e5m2_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t |
| `asc_loadunalign_postupdate_impl` (vldu) | `__ubuf__ T* src1` | `vldu(..., count)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, int64_t, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, fp8_e5m2_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t, fp8_e8m0_t |

---

## 二、STORE 类接口 (reg_store/)

### 2.1 asc_storealign — vsts/vst + POST_UPDATE

storealign 的 postupdate 接口**传指针** (`T*`)，而非指针引用。postupdate 通过 offset 参数 + POST_UPDATE mode 自动更新 offset 或用 iter_reg。

| 接口 | `__ubuf__` 地址传参方式 | 底层指令 | 覆盖类型 |
|------|----------------------|---------|---------|
| `asc_storealign_postupdate_impl` (int32_t offset) | `__ubuf__ T* dst` | `vsts(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, bfloat16_t |
| `asc_storealign_1st_postupdate_impl` (int32_t offset) | `__ubuf__ T* dst` | `vsts(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, bfloat16_t |
| `asc_storealign_pack_postupdate_impl` (int32_t offset) | `__ubuf__ T* dst` | `vsts(..., POST_UPDATE)` | int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, half, float, bfloat16_t |
| `asc_storealign_pack_postupdate_v2_impl` (int32_t offset) | `__ubuf__ T* dst` | `vsts(..., POST_UPDATE)` | int32_t, uint32_t, float |

### 2.2 asc_storeunalign — vstus/vstur/vstu + POST_UPDATE

storeunalign 的 postupdate 有多套重载，**部分传指针引用** (`T*&`)，**部分传指针** (`T*`)。

| 接口 | `__ubuf__` 地址传参方式 | 底层指令 | 覆盖类型 |
|------|----------------------|---------|---------|
| `asc_storeunalign_postupdate_impl` (vstus + count) | `__ubuf__ T*& dst` | `vstus(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, int64_t, uint32_t, half, float, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, fp8_e5m2_t, fp8_e8m0_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t |
| `asc_storeunalign_postupdate_impl` (vstur) | `__ubuf__ T* dst` | `vstur(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, int64_t, uint32_t, half, float, bfloat16_t, fp8_e4m3fn_t, fp8_e5m2_t, fp8_e8m0_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t |
| `asc_storeunalign_postupdate_impl` (vstu + iter_reg) | `__ubuf__ T* dst` | `vstu(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, half, float, bfloat16_t, hifloat8_t, fp8_e4m3fn_t, fp8_e5m2_t, fp8_e8m0_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t |
| `asc_storeunalign_post_postupdate_impl` | `__ubuf__ T*& dst` | `vstas(..., POST_UPDATE)` | int8_t, uint8_t, int16_t, uint16_t, int32_t, int64_t, uint32_t, half, float, bfloat16_t, hifloat8_t, fp8_e4m3fn_t, fp8_e5m2_t, fp8_e8m0_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t |

---

## 三、汇总对比

### 3.1 涉及 postupdate 的接口分类

| 类别 | 接口族 | 底层指令族 | `__ubuf__` 传参 | 覆盖类型数 |
|------|-------|-----------|----------------|----------|
| Load (align) | `asc_loadalign_*_postupdate_*` | `vlds + POST_UPDATE` | **指针引用** `T*&` | 14~18 |
| Load (unalign vldus) | `asc_loadunalign_postupdate_impl` (vldus) | `vldus + POST_UPDATE` | **指针** `T*` | 17 |
| Load (unalign vldu) | `asc_loadunalign_postupdate_impl` (vldu) | `vldu` (隐含 postupdate) | **指针** `T*` | 17 |
| Store (align) | `asc_storealign_*_postupdate_*` | `vsts + POST_UPDATE` | **指针** `T*` | 7~8 |
| Store (unalign vstus) | `asc_storeunalign_postupdate_impl` (vstus) | `vstus + POST_UPDATE` | **指针引用** `T*&` | 17 |
| Store (unalign vstur) | `asc_storeunalign_postupdate_impl` (vstur) | `vstur + POST_UPDATE` | **指针** `T*` | 15 |
| Store (unalign vstu) | `asc_storeunalign_postupdate_impl` (vstu) | `vstu + POST_UPDATE` | **指针** `T*` | 17 |
| Store (unalign post) | `asc_storeunalign_post_postupdate_impl` | `vstas + POST_UPDATE` | **指针引用** `T*&` | 16 |

### 3.2 对比：非 postupdate 版本中的 `__ubuf__` 传参

| 接口族 | 传参方式 |
|-------|---------|
| `asc_loadalign_*_impl` (无 postupdate) | **指针** `T*` |
| `asc_load_impl` (无 postupdate) | **指针** `T*` |
| `asc_loadunalign_impl` (无 postupdate) | **指针** `T*` |
| `asc_loadunalign_pre_impl` (无 postupdate) | **指针** `T*` |
| `asc_storealign_*_impl` (无 postupdate) | **指针** `T*` |
| `asc_store_impl` (无 postupdate) | **指针** `T*` |
| `asc_storeunalign_impl` (无 postupdate) | **指针** `T*` (除 `int4b_t`: `T*&`) |
| `asc_storeunalign_post_impl` (无 postupdate) | **指针** `T*` |
| `asc_scatter_impl` | **指针** `T*` |
| `asc_gather_impl` | **指针** `T*` |
| `asc_gather_datablock_impl` | **指针** `T*` |
| `asc_copy_gm2ub_align_impl` | **指针** `T*` |
| `asc_copy_ub2gm_align_impl` | **指针** `T*` |
| `asc_ndim_copy_gm2ub_impl` | **指针** `T*` |
| `asc_copy_ub2ub_impl` | **指针** `T*` |
| `asc_copy_ub2l1_impl` | **指针** `void*` |

### 3.3 关键结论

1. **Load align postupdate 系列** (vlds + POST_UPDATE): `src` 统一使用 **指针引用** `__ubuf__ T*&`。因为 vlds 的 POST_UPDATE 机制会直接修改指针本身（而非仅改变 offset），传引用才能将修改反映到调用方。

2. **Load unalign postupdate 系列** (vldus/vldu): `src` 统一使用 **指针** `__ubuf__ T*`。vldus 的 POST_UPDATE 通过 `vector_load_unalign` 中间结构管理地址更新，不需要直接修改指针。

3. **Store align postupdate 系列** (vsts + POST_UPDATE): `dst` 统一使用 **指针** `__ubuf__ T*`，因为 vsts 的 postupdate 通过 offset (int32_t 或 iter_reg) 机制工作，不需要修改指针值。

4. **Store unalign postupdate 系列**: 存在两种传参方式混用:
   - `vstus + POST_UPDATE` 版本: `dst` 使用 **指针引用** `T*&`（硬件直接修改指针）
   - `vstur + POST_UPDATE` 版本: `dst` 使用 **指针** `T*`（全量 store，不需要更新）
   - `vstu + POST_UPDATE` 版本: `dst` 使用 **指针** `T*`（通过 iter_reg& 管理循环）
   - `vstas + POST_UPDATE` 版本: `dst` 使用 **指针引用** `T*&`

5. **何时需要 `T*&` (指针引用)**: 当底层硬件指令的 POST_UPDATE 模式会修改 `__ubuf__` 指针本身的值时（如 vlds, vstus, vstas），必须传引用，否则调用方的指针不会更新。
