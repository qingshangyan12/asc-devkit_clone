# asc_set_nthbit

## AI处理器支持情况

| AI处理器类型 | 是否支持 |
| :-----------| :------: |
| Ascend 910C |    √    |
| Ascend 910B |    √    |

## 功能说明

计算一个uint64_t类型数字的指定二进制为置为1，其余位保持不变。

## 函数原型

```cpp
__aicore__ inline uint64_t set_nthbit_impl(uint64_t bits, int64_t idx)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| x | 输入 | 输入数据。 |
| idx | 输入 | 位索引，表示需要设置为1的位的位置。 |
## 返回值说明

位操作函数，用于将一个uint64_t整数bits的第idx位设置为1。

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

```cpp
uint64_t bits = 0x0;
int64_t  idx = 0x2;
uint64_t res = set_nthbit_impl(bits, idx);
res = 0x4;
```