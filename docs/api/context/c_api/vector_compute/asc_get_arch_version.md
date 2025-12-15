
# asc_get_arch_version

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

获取当前AI处理器架构版本号。


### 函数原型

```cpp
__aicore__ inline int64_t asc_get_arch_ver()
```


### 返回值说明

int64数据类型，表示AI处理器架构版本。

### 流水类型

PIPE_TYPE_S

### 约束说明

无

### 调用示例

```cpp
asc_get_arch_version();
```