# asc_set_ub2gm_loop_size

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

用于设置内层循环和外层循环的次数。需和[asc_copy_ub2gm_align](../../vector_datamove/asc_copy_ub2gm_align.md)操作配合使用。
## 函数原型

```cpp
__aicore__ inline void asc_set_ub2gm_loop_size(uint32_t loop1_size, uint32_t loop2_size)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| loop1_size | 输入 | 用于设置内层循环的循环次数，数据类型为uint32_t,取值范围为[0, 2^21)。 |
| loop2_size | 输入 | 用于设置外层循环的循环次数，数据类型为uint32_t,取值范围为[0, 2^21)。 |

## 返回值说明

无

## 约束说明

无

## 调用示例

```cpp
uint32_t loop1_size = 2;
uint32_t loop2_size =2;
asc_set_ub2gm_loop_size(loop1_size, loop2_size);
//total_length指参与搬运的数据总长度。dst是外部输入的uint8_t类型的GM内存。
constexpr uint64_t total_length = 128;
__ubuf__ uint8_t src[total_length];
asc_copy_ub2gm_align(dst, src, total_length * sizeof(uint8_t));
```