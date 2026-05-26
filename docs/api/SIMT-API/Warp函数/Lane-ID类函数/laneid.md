# laneid

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

 获取线程在其线程束内的索引。

## 函数原型

```
int32_t laneid()
```

## 参数说明

无

## 返回值说明

线程在其线程束内的索引，当前一个线程束中的线程数量为固定值32，故laneid的取值范围为[0, 31]。

## 约束说明

无

## 需要包含的头文件

无

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void kernel_laneid(float* x, float* y, float* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        int32_t lane_id = laneid();
        if(lane_id % 2) {
            z[idx] = x[idx] + y[idx];
        } else {
            z[idx] = x[idx] - y[idx];
        }
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) void kernel_laneid(float* x, float* y, float* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        int32_t lane_id = laneid();
        if(lane_id % 2) {
            z[idx] = x[idx] + y[idx];
        } else {
            z[idx] = x[idx] - y[idx];
        }
    }
    ```
