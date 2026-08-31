# asc_get_arch_ver

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

获取当前AI处理器架构版本号。

读取ARCH_VER特殊寄存器，并通过输出参数`core_version`返回当前核的核心版本字段值。该寄存器由系统控制在芯片初始化阶段配置，反映当前核的核心版本编码，kernel运行期间不期望被修改。本接口将寄存器原始值中的核心版本字段提取为`uint32_t`写入调用方提供的引用参数，常用于在kernel内根据当前核的核心版本选择不同代码分支或配置不同参数。

## 函数原型

```c
__aicore__ inline void asc_get_arch_ver(uint32_t& core_version)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| core_version | 输出 | AI处理器架构版本，数据类型uint32_t。接收当前核的核心版本字段值，接口返回时该参数被覆写为ARCH_VER寄存器中的核心版本字段值，调用前无需初始化。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在调用asc_get_arch_ver接口前，需先定义core_version，调用asc_get_arch_ver接口后core_version会变成相对应架构版本号的值。
- 由于硬件约束，在查看转换后的AI处理器架构版本号时需要将其打印成十六进制的数或者自行转换成十六进制的数。
- 本接口为只读查询接口，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- ARCH_VER寄存器由系统控制在芯片初始化阶段配置完成，kernel运行期间不应依赖返回值发生变化。
- 本接口仅返回ARCH_VER中的核心版本字段，不返回核ID、子块ID、Block ID等其他层级标识，调用方不应将本接口返回值用于核路由或子块路由逻辑。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含对应头文件会触发编译错误。

</font>

## 调用示例

```cpp
uint32_t core_version = 0;// 定义AI处理器版本
asc_get_arch_ver(core_version);
printf("core version is %x", core_version);// 需用%x将其打印成十六进制的数
```