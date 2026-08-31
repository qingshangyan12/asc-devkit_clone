# asc_get_phy_stack_base

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

获取物理栈基地址（Physical Stack Base Address）。该值为只读特殊寄存器，宽度48 bit，由系统在kernel启动前配置完成，运行期不可由kernel修改，常用于栈地址推导、Data Cache锁定区起始地址计算等需要栈物理基地址的场景。本接口无参数，固定读取当前核上下文中的物理栈基地址寄存器值并按`int64_t`返回，在AIC与AIV上均可调用。

## 函数原型

```c
__aicore__ inline int64_t asc_get_phy_stack_base()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的当前核的物理栈基地址。底层寄存器宽度为48 bit，返回值取值范围[0, 2^48−1]，按`int64_t`返回时高16位为0，调用方无需额外掩码处理。

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- 返回值由系统控制在kernel启动前配置完成，kernel运行期间不应依赖返回值发生变化。
- 返回值落在48 bit范围内，按`int64_t`返回时高16位为0，调用方无需额外掩码处理。
- 本接口读取的是物理栈基地址寄存器，与系统虚拟基地址、核ID、block数量等其他系统变量语义不同，不可混用。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

</font>

## 调用示例

```cpp
int64_t phy_stack_base = asc_get_phy_stack_base();
printf("phy stack base is %x", phy_stack_base);// 需用%x将其打印成十六进制的数
```