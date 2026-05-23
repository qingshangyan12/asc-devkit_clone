# SetLoadDataRepeat<a name="ZH-CN_TOPIC_0000001788021082"></a>
<cann-filter npu-type="910b,A3,950">

接口头文件定义路径为：basic_api/kernel_operator_mm_intf.h
</cann-filter>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
|------|:------:|
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |
| Atlas 200I/500 A2 推理产品 | √ |
| Atlas 推理系列产品AI Core | x |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明<a name="section618mcpsimp"></a>

用于设置[Load3Dv2接口](LoadData/Load3D.md#li83241850104315)的repeat参数。设置repeat参数后，可以通过调用一次Load3Dv2接口完成多个迭代的数据搬运。

## 函数原型<a name="section620mcpsimp"></a>

```
__aicore__ inline void SetLoadDataRepeat(const LoadDataRepeatParam& repeatParams)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

| 参数名称 | 输入/输出 | 含义 |
|------|------|------|
| repeatParams | 输入 | 设置Load3Dv2接口的repeat参数，类型为LoadDataRepeatParam。<br>具体定义请参考：<br>\${INSTALL_DIR}/include/ascendc/basic_api/interface/kernel_struct_mm.h，\${INSTALL_DIR}请替换为CANN软件安装后文件存储路径。<br>参数说明请参考[表2](#table15780447181917)。 |

**表 2**  LoadDataRepeatParam结构体参数说明

| 参数名称 | 含义 |
| --- | --- |
| repeatStride | height/width方向上的前一个迭代与后一个迭代起始地址的距离，取值范围：n∈[0, 65535]，默认值为0。<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&bull; repeatMode为0，repeatStride的单位为16个元素。<br>&nbsp;&nbsp;&nbsp;&nbsp;&bull; repeatMode为1，repeatStride的单位和具体型号有关。下文中的data_type指Load3Dv2中源操作数的数据类型。<br>Ascend 950PR/Ascend 950DT，repeatStride的单位为32/sizeof(data_type)个元素。<br>Atlas A2训练系列产品/Atlas A2推理系列产品，repeatStride的单位为32/sizeof(data_type)个元素。<br>Atlas A3训练系列产品/Atlas A3推理系列产品，repeatStride的单位为32/sizeof(data_type)个元素。<br>Atlas 200I/500 A2推理产品，repeatStride的单位为64/sizeof(data_type)个元素。 |
| repeatTime | height/width方向上的迭代次数，取值范围：repeatTime ∈[0, 255]。默认值为1。 |
| repeatMode | 控制repeat迭代的方向，取值范围：k∈[0, 1]。默认值为0。<br><br>0：迭代沿height方向；<br>1：迭代沿width方向。 |

## 返回值说明

无

## 约束说明

迭代沿height方向时，repeatStride的单位为16个元素，迭代沿width方向时，repeatStride的单位为32/sizeof\(data\_type\)个元素。

repeatTimes = 0表示Load3D不执行搬运，Load3D接口将被视为 NOP（空操作）。

针对Ascend 950PR/Ascend 950DT，调用[Load3D](Load3D.md#ZH-CN_TOPIC_0000002538071224)指令时，必须配置本接口中dstStride参数，若不使能repeat模式，repeatTime、repeatStride、repeatMode使用默认构造值。

## 调用示例<a name="section642mcpsimp"></a>

参考[调用示例](SetFmatrix.md#section642mcpsimp)

展示代码示例片段：

```
// Load3Dv2指令完成img2col的过程，可知 img2col后A矩阵高度为ho * wo,根据ho和wo的计算公式，代入卷积核宽度、卷积核滑动步长、卷积核膨胀系数等参数可知：A矩阵的高度为 CeilAlign(k, fractalShape[0])；img2col后A矩阵宽度为ci * kh * kw,代入kh=1,kw=1，可知A矩阵的宽度为CeilAlign(m, fractalShape[1])。最后，配置loadDataParams.enTranspose = true，将整个A 矩阵转置并且将其中每一个分形转置
// 使用load3d接口，实现NZ2ZZ
AscendC::LoadData3DParamsV2<T> loadDataParams;

// 设置loadDataParams相关参数
...

// 使用SetLoadDataRepeat接口，设置Load3Dv2接口的repeat参数
AscendC::LoadDataRepeatParam repeatParams;
repeatParams.repeatTime = 1;  // height/width方向上的迭代次数
repeatParams.repeatStride = 1;  // height/width方向上前后迭代起始位置的距离
repeatParams.repeatMode = 0;  // 迭代方向 0: 迭代沿height; 1: 迭代沿width
repeatParams.dstStride = CeilDivision(m, fractalShape[0]);  // 输出矩阵K轴方向偏移
AscendC::SetLoadDataRepeat(repeatParams);

AscendC::LoadData(a2Local, a1Local, loadDataParams);
```