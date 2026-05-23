# SetLoadDataBoundary<a name="ZH-CN_TOPIC_0000001787861430"></a>
<cann-filter npu-type="910b,A3,950">

接口头文件定义路径为：basic\_api/kernel\_operator\_mm\_intf.h
</cann-filter>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
|------|:------:|
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | √ |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明<a name="section618mcpsimp"></a>

设置[Load3D](LoadData/LoadData.md)接口所需的L1 Buffer(TPosition: A1/B1)时边界值。

如果Load3D指令在处理源操作数时，源操作数在A1/B1上的地址超出设置的边界，则会从A1/B1起始地址开始读取数据。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline void SetLoadDataBoundary(uint32_t boundaryValue)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

| 参数名称 | 输入/输出 | 含义 |
|------|------|------|
| boundaryValue | 输入 | 边界值。<br>Load3Dv1指令：单位是32字节。<br>Load3Dv2指令：单位是字节。 |

## 返回值说明

无

## 约束说明<a name="section633mcpsimp"></a>

-   用于Load3Dv1时，boundaryValue的最小值是16（单位：32字节）；用于Load3Dv2时，boundaryValue的最小值是1024（单位：字节）。
-   如果使用SetLoadDataBoundary接口设置了边界值，配合Load3D指令使用时，Load3D指令的A1/B1初始地址要在设置的边界内。
-   如果boundaryValue设置为0，则表示无边界，可使用整个A1/B1。
-   操作数地址对齐要求请参见[通用地址对齐约束](../../../../通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

参考[调用示例](SetFmatrix.md#section642mcpsimp)。

