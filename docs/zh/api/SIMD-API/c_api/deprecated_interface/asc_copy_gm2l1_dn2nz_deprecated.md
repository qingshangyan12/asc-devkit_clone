# asc_copy_gm2l1_dn2nz（废弃）

## 功能说明

**`asc_copy_gm2l1_dn2nz`的旧常规重载和`asc_copy_gm2l1_dn2nz_sync`接口已废弃。请使用带`asc_load_l2_cache_mode l2_cache_mode`参数的[asc_copy_gm2l1_dn2nz](../cube_datamove/asc_copy_gm2l1_dn2nz.md)接口；同步场景请额外调用`asc_sync`。**

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ T* dst, __gm__ T* src, uint64_t loop1_src_stride,
    uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ T* dst, __gm__ T* src, uint64_t loop1_src_stride,
    uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
```
