# asc_set_l0c2gm_config
## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的矢量量化参数。

## 函数原型

```c++
__aicore__ inline void asc_set_l0c2gm_config(uint64_t reluPre, uint64_t quantPre, bool enableUnitFlag)
```

## 参数说明

|参数名|输入/输出|描述|
|------------|------------|-----------|
| reluPre | 输入     | ReLU操作前张量的矢量起始地址。|
| quantPre | 输入     | 量化操作前张量的矢量起始地址。|
| enableUnitFlag | 输入     | 是否启用UnitFlag。UnitFlag是一种矩阵计算指令和矩阵搬运指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。|
## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

支持以下三种传参形式：
- 同时设置reluPre和quantPre。
- 仅传入reluPrequantPre传入0。
- 仅传入quantPre，reluPre传入0。

## 调用示例

```c++
constexpr uint64_t reluPre = 0;
constexpr uint64_t quantPre = 0x1000;// 假设量化操作有效地址为 0x1000
asc_set_l0c2gm_config(reluPre, quantPre, true);
```