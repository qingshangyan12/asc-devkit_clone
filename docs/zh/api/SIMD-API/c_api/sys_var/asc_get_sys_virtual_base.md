# asc_get_sys_virtual_base

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

获取系统虚拟基地址。

## 函数原型

```cpp
 __aicore__ inline int64_t asc_get_sys_virtual_base()
```

## 参数说明

无

## 返回值说明

系统虚拟基地址。

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口为只读查询接口，仅读取系统虚拟基地址寄存器当前值，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- 系统虚拟基地址寄存器为只读寄存器，由系统在kernel启动前按核配置完成，kernel运行期间不应依赖返回值发生变化；若运行期间系统侧修改了该寄存器，本接口读到的新值与kernel启动时的地址布局假设可能不一致，后续基于系统虚拟基地址的访问需自行保证一致性。
- 本接口仅返回系统虚拟基地址寄存器整体值，不返回核ID、block数量、子块数量、架构版本、控制位、物理栈基地址、SMMU标签版本等其他系统变量，调用方不应将本接口返回值用于核路由、子块路由或控制流分支。
- 返回值落在64 bit范围内，按`int64_t`返回时高1位为符号位，调用方若需按无符号64 bit整数解析应自行做位运算处理。
- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

## 调用示例

```cpp
int64_t sys_virtual_base = asc_get_sys_virtual_base();
printf("sys virtual base is %x", sys_virtual_base);// 需用%x将其打印成十六进制的数
```