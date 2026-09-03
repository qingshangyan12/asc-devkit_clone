# asc_copy_gm2l1（2D矩阵搬运旧形式，废弃）

## 功能说明

**`asc_copy_gm2l1`使用`uint8_t decomp_mode`和`uint8_t l2_cache_ctl`的旧常规重载及其`asc_copy_gm2l1_sync`接口已废弃。请使用带`asc_load_l2_cache_mode l2_cache_mode`参数的[asc_copy_gm2l1_arch_3510（2D矩阵搬运模式）](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_2d_arch_3510.md)接口。**

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ T* dst, __gm__ T* src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step,
    uint8_t decomp_mode, uint8_t l2_cache_ctl)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ T* dst, __gm__ T* src, uint32_t m_start_position,
    uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step,
    uint8_t decomp_mode, uint8_t l2_cache_ctl)
```
