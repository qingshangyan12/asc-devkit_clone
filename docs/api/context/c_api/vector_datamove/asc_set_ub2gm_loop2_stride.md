# asc_set_ub2gm_loop2_stride

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

用于设置外层循环中，源操作数在相邻迭代间的数据块间隔，以及目的操作数在相邻迭代间的数据块间隔。需和[asc_copy_ub2gm_align](asc_copy_ub2gm_align.md)操作配合使用。
## 函数原型

```cpp
__aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| loop2_src_stride | 输入 | 用于设置外层循环中相邻迭代源操作数的数据块间的间隔，单位为Byte，数据类型为uint64_t，取值范围为[0, 2^21)，并且loop2_src_stride必须32B对齐。 |
| loop2_dst_stride | 输入 | 用于设置外层循环中相邻迭代目的操作数的数据块间的间隔，单位为Byte，数据类型为uint64_t，取值范围为[0, 2^40)。 |

## 返回值说明

无

## 约束说明

无

## 流水类型

PIPE_V

## 调用示例

```cpp
uint32_t loop1_size = 2;
uint32_t loop2_size =2;
uint64_t loop1_src_stride = 32;
uint64_t loop1_dst_stride = 64;
uint32_t loop2_src_stride = 32;
uint32_t loop2_dst_stride = 64;
asc_set_ub2gm_loop_size(loop1_size, loop2_size);
asc_set_ub2gm_loop1_stride(loop1_src_stride, loop1_dst_stride);
asc_set_ub2gm_loop2_stride(loop2_src_stride, loop2_dst_stride);
//total_length指参与搬运的数据总长度。dst是外部输入的uint8_t类型的GM内存。
constexpr uint64_t total_length = 128;
__ubuf__ uint8_t src[total_length];
asc_copy_ub2gm_align(dst, src, total_length * sizeof(uint8_t));
```