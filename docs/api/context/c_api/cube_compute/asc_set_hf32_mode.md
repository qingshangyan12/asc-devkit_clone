
# asc_set_hf32_mode

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

用于设置Mmad计算是否开启HF32模式，开启该模式后L0A/L0B中的FP32数据将在参与Mmad计算之前被舍入为HF32。


### 函数原型



 ```cpp
   __aicore__ inline void asc_set_hf32_mode(bool hf32Mode)
```


### 参数说明

### 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| hf32Mode | 输入 | Mmad HF32模式控制入参，bool类型，取值说明如下：<br>&bull;true：L0A/L0B中的FP32数据将在矩阵乘法之前被舍入为HF32；<br>&bull;false：将执行常规的FP32矩阵乘法。 |


### 返回值说明

无

### 流水类型

PIPE_TYPE_S

### 约束说明

无

### 调用示例

```cpp
bool hf32Mode=true;
asc_set_hf32_mode_impl(hf32Mode);
```