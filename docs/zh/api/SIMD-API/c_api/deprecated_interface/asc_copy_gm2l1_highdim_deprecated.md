# asc_copy_gm2l1（高维切分旧形式，废弃）

## 功能说明

**`asc_copy_gm2l1`使用`uint8_t pad_func_mode`的旧常规重载及其`asc_copy_gm2l1_sync`接口已废弃。请使用带`asc_channel_pad_mode pad_mode`参数的[asc_copy_gm2l1](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md)接口。**

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
    uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst,
    uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride, uint32_t dst_stride)
```
