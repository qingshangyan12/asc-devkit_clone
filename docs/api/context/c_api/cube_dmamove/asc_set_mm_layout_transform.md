# asc_set_mm_layout_transform

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置mmad计算时优先通过M/N中的哪个方向，M为矩阵的行，N为矩阵的列。

## 函数原型

```c++
__aicore__ inline void asc_set_mm_layout_transform(bool layout_mode)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| value | 输入 | 控制mmad优先通过M/N的哪个方向，bool型，支持如下两种取值：<br> true：代表CUBE将首先通过N方向，然后通过M方向产生结果。<br> true：代表CUBE将首先通过M方向，然后通过N方向产生结果。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

```c++
bool layout_mode = true;
asc_set_mm_layout_transform(layout_mode);
```