# asc_copy_l12l0b

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

用于搬运存放在L1 Buffer里的512B大小的矩阵到l0b Buffer里。包含2D格式搬运、3D格式搬运。

## 函数原型

- 高维切分搬运 2D格式
     ```c++
    __aicore__ inline void asc_copy_l12l0b(__ca__ uint8_t * dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ int8_t * dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ uint16_t * dst, __cbuf__ uint16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ int16_t * dst, __cbuf__ int16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ half * dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ bfloat16_t * dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ uint32_t * dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ int32_t * dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ float * dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b(__ca__ void * dst, __cbuf__ void* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    ```

- 同步高维切分搬运 2D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ uint8_t * dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ int8_t * dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ uint16_t * dst, __cbuf__ uint16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ int16_t * dst, __cbuf__ int16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ half * dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ bfloat16_t * dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ uint32_t * dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ int32_t * dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ float * dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ void * dst, __cbuf__ void* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);
    ```

- 高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0b(__ca__ half * dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b(__ca__ bfloat16_t * dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b(__ca__ uint32_t * dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b(__ca__ int32_t * dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b(__ca__ float * dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    ```

- 同步高维切分搬运 3D格式

    ```c++
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ half * dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ bfloat16_t * dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ uint32_t * dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ int32_t * dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    __aicore__ inline void asc_copy_l12l0b_sync(__ca__ float * dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size);
    ```

## 参数说明

表1 2D格式参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0B Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| start_index | 输入 | 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。<br>单位为256元素。 |
| repeat | 输入 | 迭代次数，每个迭代可以处理512B数据。 |
| src_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长。<br>单位为256元素。|
| dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为256元素。 |
| sid | 输入 | 保留，未使用。填0即可。 |
| transpose | 输入 | 是否转置。仅当数据类型为16字节时，可以设置为true。 |
| addr_mode | 输入 | 保留，未使用。填0即可。 |

表2 3D格式参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0B Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| k_extension | 输入 | |
| m_extension | 输入 | |
| k_start_pt | 输入 | |
| m_start_pt | 输入 | |
| stride_w | 输入 | |
| stride_h | 输入 | |
| filter_w | 输入 | |
| filter_h | 输入 | |
| dilation_filter_w | 输入 | |
| dilation_filter_h | 输入 | |
| filter_size_w | 输入 | |
| filter_size_h | 输入 | |
| transpose | 输入 | |
| f_matrix_ctrl | 输入 | |
| channel_size | 输入 | |

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- dst的起始地址需要512字节对齐，src的起始地址需要32字节对齐。
- repeat为0表示不执行。
- 2D格式搬运中，转置场景(transpose=true)仅支持数据类型是16字节，其他数据类型的转置场景请参考asc_copy_l12l0b_trans[./asc_copy_l12l0b_trans.md].

## 调用示例

```cpp
__cbuf__ half src[256];
__ca__ half dst[256];
uint16_t start_index = 1
uint8_t repeat = 1
uint16_t src_stride = 1 
uint16_t dst_gap = 0
uint8_t sid = 0;
bool transpose = false;
uint8_t addr_mode = 0;
asc_copy_l12l0b(dst, src, start_index, repeat, src_stride, dst_gap, sid, transpose, addr_mode);
```
