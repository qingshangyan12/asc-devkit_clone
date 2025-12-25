# asc_popc

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

获取一个uint64_t类型数字的二进制中1的个数。

## 函数原型

```c++
__aicore__ inline int64_t asc_popc(uint64_t value)
```

## 参数说明

|参数名|输入/输出|描述|
| :------ | :--- | :------------ |
|value   |输入   |被统计的二进制数字。

## 返回值说明

value的二进制中1的个数。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```c++
uint64_t scalar = 33;
// 输出数据count_one为2
int64_t count_one = asc_popc(scalar);
```