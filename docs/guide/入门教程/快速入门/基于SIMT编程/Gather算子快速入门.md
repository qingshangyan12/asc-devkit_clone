# Gather算子快速入门<a name="ZH-CN_TOPIC_0000002509977650"></a>

本入门示例基于Ascend C SIMT实现一维Gather算子，带你快速上手实践，涵盖Device端核函数实现、Host端调用以及编译运行的完整流程，帮助开发者建立整体认知。

开始前请参考[环境准备](../../环境准备.md)安装所需的CANN软件包，完整样例请见[Gather](https://gitcode.com/cann/asc-devkit/tree/master/examples/03_simt_api/00_introduction/01_gather/basic_gather/gather_1d)。

 - **Gather 算子功能介绍**：

    Gather算子的数学表达式为：

    ```
    output[i] = input[index[i]]
    ```
    
    计算逻辑：根据索引张量`index`中的每个元素，从一维输入向量`input`中采集对应位置的数据，并写入输出张量`output`。
    
 - **Device 端代码实现**：

    后缀名为`*.asc`的代码文件包含Host端与Device端代码，其Device端部分示例如下：
    ```cpp
    __global__ void gather_1d_custom(float* input, int32_t* index, float* output, uint64_t index_total_length)
    {
        // Calculate global thread ID
        int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
        // Maps to the index of output tensor
        if (idx >= index_total_length) {
            return;
        }
        output[idx] = input[index[idx]];
    }
    ```

 - **Host 端代码实现**：

    Host端通过<<<>>>语法糖调用Device端代码。
    ```cpp
    std::vector<float> gather_1d(std::vector<float>& input, std::vector<int32_t>& index)
    {
        ...
        // Configure kernel launch parameters
        uint32_t block_num = 48; // Number of thread blocks (Grid size)
        uint32_t thread_num_per_block = 256; // Number of threads per block (Block size)
        uint32_t dyn_ubuf_size = 0;  // No dynamic memory required in this sample

        // Launch kernel <<<Grid, Block, Dynamic memory, Stream>>>
        gather_1d_custom<<<block_num, thread_num_per_block, dyn_ubuf_size, stream>>>(
            input_device, index_device, output_device, index.size());
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
    
    add_executable(gather_1d
        gather_1d.asc
    )
    
    # 通过编译选项设置NPU架构
    target_compile_options(gather_1d PRIVATE   
       $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt>
    )
    ```
    编译并执行：
    ```
    mkdir -p build && cd build; 
    cmake ..;make -j;
    ./gather_1d
    ```
    > [!NOTE] 说明
    > - 该样例支持以下型号：
    >     - Ascend 950PR / Ascend 950DT
    > - 编译选项 `--npu-arch` 用于指定NPU架构版本，`dav-` 后为架构版本号，请替换为您实际使用的版本。各 AI 处理器型号对应的架构版本号请通过[AI 处理器型号和 \_\_NPU\_ARCH\_\_ 的对应关系](../../../编程指南/语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114) 查询。
    > - 编译选项`--enable-simt` 用于启用SIMT编程场景。

如需进一步了解Ascend C的SIMD与SIMT编程模型，请参阅[Ascend C 编程模型概述](../../../编程指南/编程模型/编程模型概述.md)。
