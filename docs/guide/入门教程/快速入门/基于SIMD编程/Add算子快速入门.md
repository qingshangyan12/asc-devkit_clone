# Add算子快速入门<a name="ZH-CN_TOPIC_0000002500781060"></a>

本示例基于 Ascend C SIMD实现经典Add算子，全面讲解设备侧核函数实现、主机侧调用逻辑，以及代码编译与运行的全流程。开始前请参考[环境准备](../../环境准备.md)安装所需的Toolkit包，完整样例请见[基于C语言API实现Add样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/02_simd_c_api/00_introduction/01_add/c_api_async_add/README.md)或[基于C++ Tensor实现的Add样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/00_introduction/01_vector/add/README.md)。


 - **Add 算子功能介绍**：
    Add算子的数学表达式为：

    ![](../../../figures/zh-cn_formulaimage_0000002501241896.png)

    计算逻辑：在NPU上逐元素完成 `z = x + y`，结果存储在 Device Memory 中。

 - **Device 端代码实现**：
    代码文件 `add.asc` 包含 Host 端与 Device 端代码。
    - **基于C语言API实现示例**
        ```cpp
        __vector__ __global__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
        {
            asc_init();

            uint32_t blockLength = NUM_BLOCKS * TILE_LENGTH / asc_get_block_num();

            __gm__ float* xGm = x + asc_get_block_idx() * blockLength;
            __gm__ float* yGm = y + asc_get_block_idx() * blockLength;
            __gm__ float* zGm = z + asc_get_block_idx() * blockLength;

            __ubuf__ float xLocal[TILE_LENGTH];
            __ubuf__ float yLocal[TILE_LENGTH];
            __ubuf__ float zLocal[TILE_LENGTH];

            asc_copy_gm2ub((__ubuf__ void*)xLocal, (__gm__ void*)xGm, blockLength * sizeof(float));
            asc_copy_gm2ub((__ubuf__ void*)yLocal, (__gm__ void*)yGm, blockLength * sizeof(float));
            asc_sync();

            asc_add(zLocal, xLocal, yLocal, blockLength);
            asc_sync();

            asc_copy_ub2gm((__gm__ void*)zGm, (__ubuf__ void*)zLocal, blockLength * sizeof(float));
            asc_sync();
        }
        ```

    - **基于C++ Tensor实现示例**
        ```cpp
        template <uint32_t blockLength>
        __vector__ __global__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
        {
            AscendC::InitSocState();

            AscendC::GlobalTensor<float> xGm, yGm, zGm;
            xGm.SetGlobalBuffer(x + block_idx * blockLength, blockLength);
            yGm.SetGlobalBuffer(y + block_idx * blockLength, blockLength);
            zGm.SetGlobalBuffer(z + block_idx * blockLength, blockLength);

            AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
            AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, blockLength>();
            AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, blockLength>();
            AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, blockLength>();

            AscendC::DataCopy(xLocal, xGm, blockLength);
            AscendC::DataCopy(yLocal, yGm, blockLength);
            AscendC::PipeBarrier<PIPE_ALL>();

            AscendC::Add(zLocal, xLocal, yLocal, blockLength);
            AscendC::PipeBarrier<PIPE_ALL>();

            AscendC::DataCopy(zGm, zLocal, blockLength);
            AscendC::PipeBarrier<PIPE_ALL>();
        }
        ```
 - **Host 端代码实现**：
    Host端通过<<<>>>语法糖调用Device端代码。
    ```cpp
    int32_t main(int argc, char const *argv[])
    {
        ...
        // Configure kernel launch parameters
        constexpr uint32_t numBlocks = 8;
        ...
        // Launch kernel <<<NumBlocks, Dynamic memory, Stream>>>

        // Example2: Launch add kernel which is implemented by SIMD C API
        add_custom<<<numBlocks, 0>>>(xDevice, yDevice, zDevice);

        // Example2: Launch add kernel which is implemented by C++ Basic API
        // add_custom<blockLength><<<numBlocks, 0, stream>>>(xDevice, yDevice, zDevice);
       
        aclrtSynchronizeDevice();
        ...
    }
    ```

 - **算子编译与运行**：
    CMake 配置文件示例：
    ```
    cmake_minimum_required(VERSION 3.16)

    find_package(ASC REQUIRED)

    project(kernel_samples LANGUAGES ASC CXX)

    add_executable(c_api_add_example
        c_api_add.asc
    )

    # ======================================================================================
    # NPU 编译选项配置
    #
    # 说明：
    #   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
    # ======================================================================================
    target_compile_options(c_api_add_example PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
    )
    ```
    编译执行示例：
    ```
    mkdir -p build && cd build;   # 创建并进入build目录
    cmake ..;make -j;             # 编译工程
    ./c_api_add_example           # 执行样例
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
