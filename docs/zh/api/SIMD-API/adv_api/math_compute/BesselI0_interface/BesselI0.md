# BesselI0

## 产品支持情况

<!-- npu="950" id3 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id3 -->
<!-- npu="A3" id4 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="910b" id5 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id5 -->
<!-- npu="310b" id6 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->
<!-- npu="x90" id1 -->
- Kirin X90：不支持
<!-- end id1 -->
<!-- npu="9030" id2 -->
- Kirin 9030：不支持
<!-- end id2 -->

## 功能说明

按元素计算第一类零阶修正贝塞尔函数 I₀(x)（modified Bessel function of the first kind, order 0），语义对齐 PyTorch 的 `prims.bessel_i0` / `torch.i0`，逐元素计算、无广播。数学定义如下：

```text
I0(x) = Σ_{k=0}^{+∞} (1 / (k!)^2) * (x/2)^{2k}
```

实现采用分段 Chebyshev 多项式近似（系数与 PyTorch ATen `calc_i0` / Cephes `i0`/`i0e` 一致），大值段使用 exponentially-scaled 求值以避免中间量提前溢出：

- |x| ≤ 8：`I0(x) = Exp(|x|) * chbevl(|x|/2 - 2, A[30])`
- |x| > 8：`I0(x) = Exp(|x| - 0.5*Ln(|x|) + Ln(chbevl(32/|x| - 2, B[25])))`

数值行为说明：

- 输出满足数学不变量 I₀(x) ≥ 1（实现含统一的 `Max(result, 1.0)` 下界钳位），x = ±0 输出精确 1.0；
- 输入 NaN 输出 NaN；输入 ±Inf 输出 +Inf；
- |x| 超过 float32 可表示边界（约 91.9）时结果自然上溢为 +Inf；
- float32 下精度满足"双万分之一"要求（1 万个数据中误差超过万分之一的不超过 1 个）。

## 函数原型

- 通过sharedTmpBuffer入参传入临时空间
    - 源操作数Tensor全部/部分参与计算

        ```cpp
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
        ```

    - 源操作数Tensor全部参与计算

        ```cpp
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
        ```

- 接口框架申请临时空间
    - 源操作数Tensor全部/部分参与计算

        ```cpp
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
        ```

    - 源操作数Tensor全部参与计算

        ```cpp
        template <typename T, bool isReuseSource = false>
        __aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
        ```

本接口（RegBase 实现）不需要任何临时空间，[GetBesselI0MaxMinTmpSize](GetBesselI0MaxMinTmpSize.md) 恒返回 0。为保持高阶API家族 4 重载签名惯例，仍保留 sharedTmpBuffer 参数，但接口接受而不使用该参数（可传入任意合法 `LocalTensor<uint8_t>`，包括零长度）。

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数的数据类型。支持的数据类型为：float。 |
| isReuseSource | 是否允许修改源操作数。该参数预留，传入默认值false即可。本实现全程只读 srcTensor，不向其内存写回，该参数取 true/false 对行为无影响。 |

**表2**  接口参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstTensor | 输出 | 目的操作数，`LocalTensor<T>`，shape/dtype 与 srcTensor 完全一致，元素个数 >= calCount。支持的TPosition为VECIN/VECCALC/VECOUT。 |
| srcTensor | 输入 | 源操作数，`LocalTensor<T>`，元素个数 >= calCount，与 dstTensor 地址不重叠。支持的TPosition为VECIN/VECCALC/VECOUT。 |
| sharedTmpBuffer | 输入 | 临时缓存。本实现不使用该参数（可传入任意合法 `LocalTensor<uint8_t>`，包括零长度），仅为与高阶API家族签名惯例保持一致。 |
| calCount | 输入 | 参与计算的元素个数。 |

## 返回值说明

无

## 约束说明

- 仅支持 Ascend 950PR/Ascend 950DT（DAV_3510），仅支持 float32；非 3510 架构下接口编译为空实现。
- src/dst Tensor 必须 32B 对齐，且不允许 src/dst Tensor 地址重叠。
- 调用方 buffer 容量约束：内部实现使用 `Reg::LoadAlign` 搬入数据，该接口无 mask 参数，每次 VF 迭代恒定整块搬入 oneRepeatSize（float32 下为 `AscendC::GetVecLen() / sizeof(float)` = 64）个元素，不受 calCount/尾块 mask 影响。因此 srcTensor/dstTensor 的**实际分配容量**（非仅 calCount）必须 >= `ceil(calCount / oneRepeatSize) * oneRepeatSize`，否则末块会读到 calCount 之外、调用方未分配的 UB 内存。调用方若按 calCount 精确分配，需自行 pad 到 64 的整数倍；BesselI0 自身不做该项校验。

## 调用示例

完整的调用样例请参考[更多样例](../more_examples.md)。

```cpp
#include "kernel_operator.h"

constexpr int32_t BUFFER_NUM = 1;

template <typename T>
class KernelBesselI0 {
public:
    __aicore__ inline KernelBesselI0() {}
    __aicore__ inline void Init(GM_ADDR srcGm, GM_ADDR dstGm, uint32_t dataSize)
    {
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);
        pipe.InitBuffer(inQueue, BUFFER_NUM, dataSize * sizeof(T));
        pipe.InitBuffer(outQueue, BUFFER_NUM, dataSize * sizeof(T));
        this->dataSize = dataSize;
    }
    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn()
    {
        LocalTensor<T> srcLocal = inQueue.AllocTensor<T>();
        DataCopy(srcLocal, srcGlobal, dataSize);
        inQueue.EnQue(srcLocal);
    }
    __aicore__ inline void Compute()
    {
        LocalTensor<T> dstLocal = outQueue.AllocTensor<T>();
        LocalTensor<T> srcLocal = inQueue.DeQue<T>();
        // BesselI0 不需要临时空间，直接调用无 sharedTmpBuffer 的重载
        BesselI0<T>(dstLocal, srcLocal, dataSize);
        outQueue.EnQue<T>(dstLocal);
        inQueue.FreeTensor(srcLocal);
    }
    __aicore__ inline void CopyOut()
    {
        LocalTensor<T> dstLocal = outQueue.DeQue<T>();
        DataCopy(dstGlobal, dstLocal, dataSize);
        outQueue.FreeTensor(dstLocal);
    }

private:
    TPipe pipe;
    TQue<TPosition::VECIN, BUFFER_NUM> inQueue;
    TQue<TPosition::VECOUT, BUFFER_NUM> outQueue;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    uint32_t dataSize;
};
```
