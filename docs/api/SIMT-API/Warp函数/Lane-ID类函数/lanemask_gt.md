# lanemask\_gt

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | x |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明

提供当前线程的一个32位掩码，在当前线程所属的线程束中，将 “laneid严格大于当前线程” 的线程的对应位设为1，其余位为0。

如laneid为0的线程，其lanemask_gt为 1111 1111 1111 1111 1111 1111 1111 1110。

如laneid为31的线程，其lanemask_gt为 0000 0000 0000 0000 0000 0000 0000 0000。

**图 1**  lanemask\_gt示意图

![](../../../figures/lanemask_gt图示.png "lanemask_gt图示")

## 函数原型

```
int32_t lanemask_gt()
```

## 参数说明

无

## 返回值说明

返回一个32位无符号整数的位掩码。

## 约束说明

无

## 需要包含的头文件

无

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void kernel_laneid(float* x, float* y, float* z)
    {
        int32_t lanemaskeq = lanemask_gt();
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) void kernel_laneid(float* x, float* y, float* z)
    {
        int32_t lanemaskeq = lanemask_gt();
    }
    ```
