# asc_copy_gm2l1_align（废弃）

## 功能说明

**`asc_copy_gm2l1_align`的旧常规重载和`asc_copy_gm2l1_align_sync`接口已废弃。请使用带`asc_load_l2_cache_mode l2_cache_mode`参数的[asc_copy_gm2l1_align](../cube_datamove/asc_copy_gm2l1_align.md)接口；同步场景请额外调用`asc_sync`。**

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ T* dst, __gm__ T* src, uint32_t n_burst,
    uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit,
    uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ T* dst, __gm__ T* src, uint32_t n_burst,
    uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit,
    uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
```
