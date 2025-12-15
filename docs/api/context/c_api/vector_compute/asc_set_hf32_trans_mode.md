# asc_set_hf32_trans_mode

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

设置HF32模式取整的具体方式，需要先使用asc_set_hf32_mode开启HF32取整模式。


### 函数原型



 ```cpp
   __aicore__ inline void asc_set_hf32_trans_mode_impl(bool hf32_transmode)
```


### 参数说明

### 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| hf32_transmode | 输入 | asc_mmad HF32取整模式控制入参，bool类型<br>• true：FP32将以向零靠近的方式四舍五入为HF32<br>• false：FP32将以最接近偶数的方式四舍五入为HF32 |


### 返回值说明

无

### 流水类型

PIPE_TYPE_S

### 约束说明

无

### 调用示例

```cpp
bool hf32_transmode=true;
asc_set_hf32_trans_mode_impl(hf32_transmode);
```