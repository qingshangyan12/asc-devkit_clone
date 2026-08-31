# asc_get_smmu_tag_version

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

获取SMMU（System Memory Management Unit）标签版本信息。本接口读取SMMU TAG VER特殊寄存器的64 bit整体值并按`int64_t`返回，该寄存器为只读特殊寄存器，由系统控制在kernel启动前配置完成，常用于在kernel内识别当前SMMU标签版本以适配不同SMMU行为或做版本相关分支处理。

## 函数原型

```c
__aicore__ inline int64_t asc_get_smmu_tag_version()
```

## 参数说明

本接口无参数。

## 返回值说明

返回`int64_t`类型的SMMU标签版本信息。寄存器位[63:28]为保留位，低28 bit承载SMMU标签版本字段。各bit含义如下：

**表1** SMMU标签版本比特位说明

| bit范围    | 含义 |
| ----------- |:----|
| 63:28 | 保留位。 |
| 27:0 | SMMU标签版本。 |
| 27:14 | 年。格式：YY-YYYY-YYYY。 |
| 13:8 | 月。格式：MM-MMMM。 |
| 7:2 | 日。格式：DD-DDDD。 |
| 1:0 | 同一日期的子版本。 |

<font color="red">

待合入版对位域划分的描述与上表不一致，如下：

- 返回值的低28 bit承载SMMU标签版本字段：[1:0] 子版本、[7:0] 日期、[13:8] 月份、[27:14] 年份，[63:28]为保留位。

</font>

## 流水类型

PIPE_S

## 约束说明

- 本接口为只读查询接口，仅读取SMMU TAG VER寄存器当前值，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- SMMU TAG VER寄存器为只读寄存器，由SC在kernel启动前配置完成，kernel运行期间不应依赖返回值发生变化。
- 寄存器位[63:28]为保留位，读取时硬件返回值在该段不一定为0，调用方解析返回值时应仅使用低28 bit的版本字段。
- 本接口仅返回SMMU TAG VER寄存器整体值，不返回核ID、block数量、子块数量、架构版本、控制位等其他系统变量。

<font color="red">

- 本接口不支持纯SIMT编译模式，在该模式下包含`sys_var.h`头文件会触发编译错误。

</font>

## 调用示例

```cpp
int64_t smmu_tag_version = asc_get_smmu_tag_version();
printf("smmu tag version is %x", smmu_tag_version);// 需用%x将其打印成十六进制的数
```
