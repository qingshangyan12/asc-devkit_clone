# AI Core SIMD 编译<a name="ZH-CN_TOPIC_0000002429141954"></a>

毕昇编译器是一款专为AI处理器设计的编译器，支持异构编程扩展，可以将用户编写的昇腾算子代码编译成二进制可执行文件和动态库等形式。毕昇编译器的可执行程序命名为bisheng，支持x86、aarch64等主机系统，并且原生支持设备侧AI Core架构指令集编译。通过使用毕昇编译器，用户可以更加高效地进行针对昇腾AI处理器的编程和开发工作。

## 入门示例<a name="section17595637204517"></a>

以下是一个使用毕昇编译器编译静态Shape的add\_custom算子入门示例。该示例展示了如何编写源文件add\_custom.asc以及具体的编译命令。通过这个示例，您可以了解如何使用毕昇编译器进行算子编译。完整样例请参考[LINK](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/00_introduction/01_add/basic_api_tque_add)。

1.  包含头文件。

    在编写算子源文件时，需要包含必要的头文件。

    ```
    // 头文件
    #include "acl/acl.h"
    #include "kernel_operator.h"
    ```

2.  核函数实现。

    -   核函数支持模板。
    -   核函数入参支持传入用户自定义的结构体，比如示例中用户自定义的AddCustomTilingData结构体。

    ```
    // 用户自定义的TilingData结构体
    struct AddCustomTilingData { 
        uint32_t totalLength; 
        uint32_t tileNum;
    };
    
    // Kernel核心实现逻辑，包括搬运，计算等
    class KernelAdd {
    public:
        __aicore__ inline KernelAdd() {}
        // ...
    
    };
    
    __global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, AddCustomTilingData tiling) // 该算子执行时仅启动AI Core上的Vector核
    {
        
        KernelAdd op;
        op.Init(x, y, z, tiling.totalLength, tiling.tileNum); 
        op.Process();
    }
    ```

3.  Host侧调用函数逻辑，包括内存申请和释放，初始化和去初始化，内核调用符调用核函数等。

    ```
    // Host侧应用程序需要包含的头文件
    #include "acl/acl.h"
    // Kernel侧需要包含的头文件
    #include "kernel_operator.h"
    // 核函数开发部分
    ...
    
    __global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, AddCustomTilingData tiling)
    {
           
        KernelAdd op;
        op.Init(x, y, z, tiling.totalLength, tiling.tileNum);
        op.Process();
    }
    
    // 通过<<<...>>>内核调用符调用算子
    std::vector<float> kernel_add(std::vector<float> &x, std::vector<float> &y)
    {
    ...
    }
    
    
    // 计算结果比对
    uint32_t VerifyResult(std::vector<float> &output, std::vector<float> &golden)
    {
    ...
    }
    
    // 算子验证主程序
    int32_t main(int32_t argc, char *argv[])
    {
        constexpr uint32_t totalLength = 8 * 2048;
        constexpr float valueX = 1.2f;
        constexpr float valueY = 2.3f;
        std::vector<float> x(totalLength, valueX);
        std::vector<float> y(totalLength, valueY);
    
        std::vector<float> output = kernel_add(x, y);
    
        std::vector<float> golden(totalLength, valueX + valueY);
        return VerifyResult(output, golden);
    }
    ```

4.  采用如下的编译命令进行编译。

    -   -o demo：指定输出文件名为demo。
    -   --npu-arch=dav-2201：指定NPU的架构版本为dav-2201。dav-后为NPU架构版本号，各产品型号对应的架构版本号请通过[对应关系表](../../语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114)进行查询。

    ```
    bisheng add_custom.asc -o demo --npu-arch=dav-2201
    ```

5.  执行可执行文件。

    ```
    ./demo
    ```

