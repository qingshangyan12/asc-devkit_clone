# asc_set_hf32_trans2even

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

设置HF32模式取整方式, FP32以最接近偶数的方式四舍五入为HF32，需要先使用asc_set_hf32_mode开启HF32取整模式。


### 函数原型



 ```cpp
   __aicore__ inline void asc_set_hf32_trans2even()
```


### 参数说明

### 参数说明

无


### 返回值说明

无

### 流水类型

PIPE_TYPE_S

### 约束说明

无

### 调用示例

```cpp
asc_set_hf32_trans2even();
```