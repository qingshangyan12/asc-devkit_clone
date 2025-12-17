# asc_ffz

## 昇腾AI处理器支持情况

|昇腾AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

获取一个uint64_t类型数字的二进制表示中从最低有效位开始的第一个0出现的位置，如果没找到则返回-1。

## 函数原型

```c++
__aicore__ inline int64_t asc_ffz(uint64_t in)
```

## 参数说明
表1 参数说明

|参数名|输入/输出|描述|
| :------ | :--- | :------------ |
|in   |输入   |输入数据，数据类型是uint64_t。

## 返回值说明

int64_t类型的数，valueIn中第一个0出现的位置。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```c++
uint64_t scalar = 33;
// 输出数据count_first_zero为1
int64_t count_first_zero = asc_ffz(uint64_t scalar);
```
