# asc_set_l13d_rpt_b

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持

## 功能说明

配置右矩阵3D搬运的repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_rpt_b(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction,
    uint16_t dst_stride_k, uint16_t dst_start_pos_m)
```

## 参数说明

|参数名|输入/输出|描述|
|---|---|---|
|repeat_stride|输入|重复步长。|
|repeat_times|输入|重复次数。|
|repeat_direction|输入|重复方向。|
|dst_stride_k|输入|输出矩阵K方向步长。|
|dst_start_pos_m|输入|输出矩阵M方向起始位置。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 配置右矩阵的3D搬运时，需在对应搬运接口前调用本接口。
- `repeat_direction`使用`asc_l13d_repeat_direction::M_DIRECTION`或`asc_l13d_repeat_direction::K_DIRECTION`。

## 调用示例

```cpp
asc_set_l13d_rpt_b(0, 1, asc_l13d_repeat_direction::M_DIRECTION, 0, 0);
```
