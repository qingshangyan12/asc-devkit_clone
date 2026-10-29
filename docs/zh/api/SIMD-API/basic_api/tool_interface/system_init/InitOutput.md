# InitOutput（废弃）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：支持
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/tool_interface/system_init/InitOutput_res.md#id1 -->

## 功能说明

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

**该接口已废弃，将在后续版本移除，请使用[Fill](../../cube_compute_ISASI/cube_load_aux_config/Fill.md)接口替代。**

本接口用于将GM（Global Memory）上从`gmWorkspaceAddr`起始的`size`个元素初始化为`value`。

本接口提供Tensor形和裸指针形两种函数原型，两者语义一致：裸指针形仅将Tensor形的`GlobalTensor<T>`参数替换为GM裸指针`__gm__ T*`，其余参数及行为相同。

## 函数原型

- Tensor形：

    ```cpp
    template <typename T> __aicore__ inline __in_pipe__(V) __out_pipe__(MTE3) void InitOutput(GlobalTensor<T> gmWorkspaceAddr, uint32_t size, T value = 0);
    ```

- 裸指针形：

    ```cpp
    template <typename T> __aicore__ inline __in_pipe__(V) __out_pipe__(MTE3) void InitOutput(__gm__ T* gmWorkspaceAddr, uint32_t size, T value = 0);
    ```

## 参数说明

**表1** 模板参数说明

| 参数名称 | 描述 |
| ---- | ---- |
| T | 待初始化元素的数据类型。 |

**表2** 参数说明

| 参数名称 | 输入/输出 | 含义 |
| ---- | ---- | ---- |
| gmWorkspaceAddr | 输出 | 目的操作数，待初始化的GM地址。Tensor形为`GlobalTensor<T>`类型，裸指针形为`__gm__ T*`类型。 |
| size | 输入 | 待初始化的元素个数。 |
| value | 输入 | 初始化值，默认值为0。数据类型与`gmWorkspaceAddr`的元素类型保持一致。 |

## 返回值说明

无

## 约束说明

- 该接口已废弃，将在后续版本移除。头文件注释原文为：`NOTICE: InitOutput has been deprecated and will be removed in the next version. Please use Fill instead!`。请使用[Fill](../../cube_compute_ISASI/cube_load_aux_config/Fill.md)接口替代。
- 对GM上的数据进行初始化时，使用[Fill](../../../adv_api/tensor_transform/Fill.md)接口（`Fill(GlobalTensor<T>& gmWorkspaceAddr, const uint64_t size, const T value)`）；上一条中的链接指向L1 Buffer/L0A Buffer/L0B Buffer上的`Fill`接口。

## 调用示例

```cpp
// Tensor形
__global__ __aicore__ void init_output_tensor(GM_ADDR workspace)
{
    AscendC::GlobalTensor<float> gmWorkspace;
    gmWorkspace.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(workspace));
    // 将GM上的256个float元素初始化为1.0f
    AscendC::InitOutput(gmWorkspace, 256, 1.0f);
}

// 裸指针形
__global__ __aicore__ void init_output_ptr(GM_ADDR workspace)
{
    // 将GM上的256个float元素初始化为1.0f
    AscendC::InitOutput(reinterpret_cast<__gm__ float*>(workspace), 256, 1.0f);
}
```
