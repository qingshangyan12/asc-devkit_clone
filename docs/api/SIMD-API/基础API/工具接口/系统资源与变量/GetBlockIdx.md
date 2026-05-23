# GetBlockIdx

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | √ </cann-filter>|
| <cann-filter npu-type="x90">Kirin X90 | √ </cann-filter>|
| <cann-filter npu-type="9030">Kirin 9030 | √ </cann-filter>|

## 功能说明

获取当前核的index，用于代码内部的多核逻辑控制及多核偏移量计算等。

## 函数原型

```cpp
__aicore__ inline int64_t GetBlockIdx()
```

## 参数说明

无

## 返回值说明

- GetBlockIdx返回当前核的索引，index的范围为\[0, numBlocks\)。numBlocks由用户使用<<<\>\>\>调用时配置，numBlocks数量可以通过[GetBlockNum](GetBlockNum.md)获取。
- 同时启动AIC和AIV的场景，在AIV、AIC上调用GetBlockIdx的取值范围不同：
    - 当AIC和AIV比例为1:2时，AIC上取值范围为\[0, numBlocks\)，AIV上取值范围为\[0, 2 \* numBlocks\)；
    - 当AIC和AIV比例为1:1时，AIC上取值范围为\[0, numBlocks\)，AIV上取值范围为\[0, numBlocks\)。
    - 分离模式下，可以通过[GetTaskRatio](./GetTaskRatio.md)获取AI Core上AIC与AIV的比例。

## 约束说明

GetBlockIdx为一个系统内置函数，返回当前核的index。

## 调用示例

```cpp
// srcGm、dstGm为外部输入的gm空间
AscendC::GlobalTensor<float> srcGlobal;
AscendC::GlobalTensor<float> dstGlobal;
blockNum = AscendC::GetBlockNum(); // 获取核总数
perBlockSize = srcDataSize / blockNum; // 每个核平分处理相同个数
blockIdx = AscendC::GetBlockIdx(); // 获取当前工作的核ID
srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(srcGm + blockIdx * perBlockSize * sizeof(float)), perBlockSize);    // 分配每个核上的srcGlobal的内存地址
dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(dstGm + blockIdx * perBlockSize * sizeof(float)), perBlockSize);    // 分配每个核上的dstGlobal的内存地址
```
