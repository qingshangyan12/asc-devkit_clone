# asc_copy_gm2l1_align_impl 3510 与 9201 对比

本文基于2026年9月8日仓库中的以下文件整理：

- `impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl/asc_copy_gm2l1_align_impl.h`
- `impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl/asc_copy_gm2l1_align_impl.h`

## 共同部分

### 文件结构

- 均使用相同的版权声明和编译器内部头文件开关。
- 均通过头文件保护宏避免重复包含，宏名中的架构号不同。
- 均包含对应架构的`utils_impl/utils_impl.h`。
- 所有搬运实现均在`if ASC_IS_AIC`条件内调用底层指令，非AIC场景不执行搬运。

### 核心功能和参数

两个架构都使用`copy_gm_to_cbuf_align_v2`完成GM到L1 Buffer的带对齐和填充搬运，公共参数语义一致：

| 参数 | 类型 | 说明 |
|---|---|---|
| `dst` | `__cbuf__ T*` | L1 Buffer目的地址。 |
| `src` | `__gm__ T*` | GM源地址。 |
| `burst_count`/`n_burst` | `uint32_t` | 搬运数据块数量。 |
| `burst_len`/`len_burst` | `uint32_t` | 单个数据块长度。 |
| `left_padding_count` | `uint8_t` | 左侧填充数量。 |
| `right_padding_count` | `uint8_t` | 右侧填充数量。 |
| `enable_data_select`/`data_select_bit` | `bool` | 数据选择开关。 |
| `l2_cache_mode`/`l2_cache_ctl` | `asc_load_l2_cache_mode`或`uint8_t` | L2 Cache策略。 |
| `src_stride`/`burst_src_stride` | `uint64_t` | 源端相邻数据块步长。 |
| `dst_stride`/`burst_dst_stride` | `uint32_t` | 目的端相邻数据块步长。 |

- 底层指令的`sid`参数在两个架构中均固定传入`0`。
- 使用`asc_load_l2_cache_mode`的重载均通过`static_cast<uint8_t>`转换后传给底层指令。
- 两个架构共同支持的类型为：`uint8_t`、`bfloat16_t`、`half`、`float`、`hifloat8_t`、`int16_t`、`int32_t`、`int8_t`、`uint16_t`、`uint32_t`。

## 不同部分

### 底层指令签名

9201的`copy_gm_to_cbuf_align_v2`在`enable_data_select`和L2 Cache策略之间新增了`enable_pre_load_ignore_sync`参数：

```cpp
// 3510
copy_gm_to_cbuf_align_v2(dst, src, 0, burst_count, burst_len,
    left_padding_count, right_padding_count, enable_data_select,
    static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);

// 9201
copy_gm_to_cbuf_align_v2(dst, src, 0, burst_count, burst_len,
    left_padding_count, right_padding_count, enable_data_select,
    enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode),
    src_stride, dst_stride);
```

9201不带该参数的重载会显式传入`false`，因此行为等价于关闭预加载忽略同步功能。

### 同步接口设计

3510为每种类型提供`asc_copy_gm2l1_align_sync_impl`：

- 先调用对应的`asc_copy_gm2l1_align_impl`。
- 搬运调用后执行`asc_sync_post_process()`。
- 同步行为由独立的`_sync_impl`接口提供。

9201没有`asc_copy_gm2l1_align_sync_impl`，而是为每种类型提供两种同名重载：

- 不带`enable_pre_load_ignore_sync`的重载，向底层指令传入`false`。
- 带`enable_pre_load_ignore_sync`的重载，将调用者参数直接传给底层指令。
- 9201文件中不调用`asc_sync_post_process()`。

### 数据类型

9201新增支持`hif4_scale`，并提供普通版和带`enable_pre_load_ignore_sync`参数的两个重载。

3510不支持`hif4_scale`。

### 3510额外兼容重载

3510除`asc_load_l2_cache_mode`版本外，还为以下类型提供一组以`uint8_t l2_cache_ctl`为参数的兼容重载：

- `bfloat16_t`
- `half`
- `float`
- `hifloat8_t`
- `int16_t`
- `int32_t`
- `int8_t`
- `uint16_t`
- `uint32_t`
- `uint8_t`

每种类型均包含普通版和`asc_copy_gm2l1_align_sync_impl`版。9201没有这组`uint8_t l2_cache_ctl`兼容重载，只保留强类型枚举`asc_load_l2_cache_mode`。

### 重载数量

| 架构 | 类型数量 | 每种类型主要重载 | 额外重载 | 总计 |
|---|---:|---:|---:|---:|
| 3510 | 10 | 1个普通版 + 1个同步版 | 10种类型各有1个`uint8_t`普通版和1个同步版 | 40 |
| 9201 | 11 | 1个普通版 + 1个预加载控制版 | 无 | 22 |

## 总结

- 共同核心是通过`copy_gm_to_cbuf_align_v2`执行GM到L1 Buffer的带对齐搬运，主要搬运参数和公共数据类型保持一致。
- 3510通过独立`_sync_impl`和`asc_sync_post_process()`提供同步语义，并保留`uint8_t l2_cache_ctl`兼容接口。
- 9201移除了本文件中的同步包装及`uint8_t`兼容接口，底层指令增加`enable_pre_load_ignore_sync`参数，并新增`hif4_scale`数据类型。
- 将3510代码迁移到9201时，不能直接复制底层指令调用；必须在L2 Cache参数前补充`enable_pre_load_ignore_sync`，普通接口应传`false`。
