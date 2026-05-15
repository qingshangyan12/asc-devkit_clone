# Add算子快让入门<a name="ZH-CN_TOPIC_0000002509977650"></a>

本示例基于 Ascend C SIMT实现经典Add算子，全面讲解设备侧核函数实现、主机侧调用逻辑，以及代码编译与运行的全流程。开始前请参考[环境准备](../../环境准备.md)安装所需的Toolkit包，完整样例请见[Add](https://gitcode.com/cann/asc-devkit/tree/master/examples/03_simt_api/00_introduction/01_add)。

 - **Add 算子功能介绍**：
    Add算子的数学表达式为：

    ![](../../../figures/zh-cn_formulaimage_0000002541636777.png)

    计算逻辑：在NPU上逐元素完成 `z = x + y`，结果存储在 Device Memory 中。
    
 - **Device 端代码实现**：
    代码文件 `add.asc` 包含 Host 端与 Device 端代码。
    ```cpp
    __global__ void add_custom(float* x, float* y, float* z, uint64_t total_length)
    {
        // Calculate global thread ID
        int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Maps to the row index of output tensor
        if (idx >= total_length) {
            return;
        }
        z[idx] = x[idx] + y[idx];
    }
    ```

 - **Host 端代码实现**：
    Host端通过<<<>>>语法糖调用Device端代码。
    ```cpp
    int32_t main(int argc, char const *argv[])
    {
        ...
        // Calculate split parameters
        uint32_t block_num            = 48;
        uint32_t thread_num_per_block = 256;
        uint32_t dyn_ubuf_size        = 0;  // No dynamic memory required

        // Call kernel function with <<<...>>>
        add_custom<<<block_num, thread_num_per_block, dyn_ubuf_size, stream>>>(x_device, y_device, z_device, x.size());
        ...
    }
    ```

 - **算子编译与运行**：
    CMake 配置文件示例：
    ```
    cmake_minimum_required(VERSION 3.16)
    # find_package(ASC)是CMake中用于查找和配置Ascend C编译工具链的命令
    find_package(ASC REQUIRED)
    # 指定项目支持的语言包括ASC和CXX，ASC表示支持使用毕昇编译器对Ascend C编程语言进行编译
    project(kernel_samples LANGUAGES ASC CXX)
    
    add_executable(demo
        add.asc
    )
    
    # 通过编译选项设置NPU架构
    target_compile_options(demo PRIVATE   
       $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt>
    )
    ```
    编译并执行：
    ```
    mkdir -p build && cd build; 
    cmake ..;make -j;
    ./demo
    ```

## 进一步学习

可继续阅读[AI Core SIMT编程](../../../编程指南/编程模型/AI-Core-SIMT编程/AI-Core-SIMT编程.md) 深入了解相关内容。

> [!NOTE] 说明
> - 该样例仅支持以下型号：
>     - Ascend 950PR / Ascend 950DT
> - 编译选项 `--npu-arch` 用于指定NPU架构版本，`dav-` 后为架构版本号，请替换为您实际使用的版本。各 AI 处理器型号对应的架构版本号请通过[AI 处理器型号和 \_\_NPU\_ARCH\_\_ 的对应关系](../../../编程指南/语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114) 查询。
> - 编译选项`--enable-simt` 用于启用SIMT编程场景。
