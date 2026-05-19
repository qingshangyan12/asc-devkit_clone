# Hello World <a name="ZH-CN_TOPIC_0000002523206878"></a>

本示例基于Ascend C SIMD搭建经典Hello World入门程序，完整讲解设备侧入口核函数实现、主机侧调用逻辑，以及代码编译与程序运行全流程。开始前请参考[环境准备](../../环境准备.md)安装所需的Toolkit包，完整样例请参考[hello\_world](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/00_introduction/00_quickstart/hello_world/README.md)。

 - **Hello World功能介绍**：
    在NPU上打印`Hello World!!!`。

 - **Device 端代码实现**：
    代码文件hello\_world.asc包括Host端代码和Device端代码：
    ```cpp
    __global__ __vector__ void hello_world()
    {
        printf("Hello World!!!\n");
    }
    ```
 - **Host 端代码实现**：
    Host端通过<<<>>>语法糖调用Device端代码。
    ```cpp
    int main(int argc, char const* argv[])
    {
        aclrtSetDevice(0); // 运行管理资源申请。
        // 1. 使用内核调用符<<<numBlock, dynUBufSize, stream>>>调用核函数。
        // 2. numBlock：8表示参与计算的核数为8（核数可根据实际需求设置）。
        // 3. dynUBufSize: 0表示不使用Unified Buffer的动态内存。
        // 4. stream: nullptr表示使用默认stream。
        hello_world<<<8, 0, nullptr>>>();
        aclrtSynchronizeDevice(); // 等待核函数执行完成
        return 0;
    }
    ```

 - **算子编译与运行**：
    ```
    bisheng hello_world.asc --npu-arch=dav-2201 -o demo
    ./demo
    ```
    运行结果如下。本样例共调度8个核，打印了核号和"Hello World!!!"等信息

    ```
    [AIV Block 0/8] Hello World!!!
    [AIV Block 1/8] Hello World!!!
    [AIV Block 2/8] Hello World!!!
    [AIV Block 3/8] Hello World!!!
    [AIV Block 4/8] Hello World!!!
    [AIV Block 5/8] Hello World!!!
    [AIV Block 6/8] Hello World!!!
    [AIV Block 7/8] Hello World!!!
    ```

## 进一步学习

可继续阅读[AI Core SIMD编程](../../../编程指南/编程模型/AI-Core-SIMD编程/概述.md) 深入了解相关内容。

> [!NOTE] 说明
>- 该样例仅支持如下型号：
>    - Ascend 950PR/Ascend 950DT
>    - Atlas A3 训练系列产品/Atlas A3 推理系列产品
>    - Atlas A2 训练系列产品/Atlas A2 推理系列产品
> - 编译选项 `--npu-arch` 用于指定NPU架构版本，`dav-` 后为架构版本号，请替换为您实际使用的版本。各 AI 处理器型号对应的架构版本号请通过[AI 处理器型号和 \_\_NPU\_ARCH\_\_ 的对应关系](../../../编程指南/语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114) 查询。
> - SIMD算子的Kernel函数需要额外的修饰符，如[`__vector__`](../../../编程指南/语言扩展层/SIMD-BuiltIn关键字.md)修饰符说明该算子仅在向量计算单元上执行。
