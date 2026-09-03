# asc_copy_l12ub_sync（废弃）

## 功能说明

**`asc_copy_l12ub_sync`使用`bool sub_blockid`的旧接口已废弃。请使用[asc_copy_l12ub](../cube_datamove/asc_copy_l12ub.md)接口的`int8_t sub_blockid`重载，并按需调用`asc_sync`。**

## 函数原型

```cpp
__aicore__ inline void asc_copy_l12ub_sync(__ubuf__ void* dst_addr, __cbuf__ void* src_addr,
    bool sub_blockid, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
```
