# asc_set_l1_3d_size

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置[asc_copy_l12l0a_3d](./asc_copy_l12l0a_3d.md)/[asc_copy_l12l0b_3d](./asc_copy_l12l0b_3d.md)接口在L1 Buffer的边界值。

## 函数原型

```c++
__aicore__ inline void asc_set_l1_3d_size(uint64_t value)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| value | 输入 | 源操作数（标量），单位是字节。最小值是1024。<br>如果value设置为0，则表示无边界，asc_copy_l12l0a_3d/asc_copy_l12l0b_3d接口可使用整个L1 Buffer。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需配合asc_copy_l12l0a_3d/asc_copy_l12l0b_3d接口使用。
- 如果使用asc_set_l1_3d_size接口设置了边界值，asc_copy_l12l0a_3d/asc_copy_l12l0b_3d接口的L1 Buffer初始地址要在设置的边界内（value）。
- 如果asc_copy_l12l0a_3d/asc_copy_l12l0b_3d接口的源操作数在L1 Buffer上的地址超出设置的边界值（value），则会从L1 Buffer起始地址开始读取数据。
- 操作数地址对齐约束请参考[通用地址对齐约束](../general_instruction.md#通用地址对齐约束)。



## 调用示例

```c++
uint64_t value = 1024;
asc_set_l1_3d_size(value);    // 设置L1 Buffer的边界值为1024（单位：字节）
```
