# tensor_scalar_tensor算子直调样例

## 概述
本样例支持以下类型API的调试：  
两个输入操作数，第一个为tensor类型，第二个为scalar类型。  
一个输出操作数，为tensor类型。  
样例中，API支持各类计算方式，如count连续计算模式，高位切分计算模式（mask连续模式、mask逐比特模式）。   
支持API特殊使用场景的调试，如复合指令指令分离、操作数重叠。  

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── tensor_scalar_tensor
│   ├── scripts
│   │   ├── gen_data.py                         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                          // 编译工程文件
│   ├── data_utils.h                            // 数据读入写出函数
│   └── tensor_scalar_tensor.asc                // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：
    - 支持不同API的切换
    - 支持API计算模式的切换（count模式、mask连续模式、mask逐比特计算模式）
    - 支持特殊使用场景（复合指令拆分、操作数重叠）

- 算子实现：  
    - kernel函数。核函数入口函数。
    - Kernel函数。使用静态Tensor编程方法，申请LocalTensor。
    - TestPerformance函数。性能测试主函数。
    - TestFunctionality函数。功能测试主函数。
    - ComputeCountMode函数。调用API，使能count计算模式。
    - ComputeContinuousMaskMode函数。调用API，使能mask连续计算模式。
    - ComputeBitwiseMaskMode函数。调用API，使能mask逐比特计算模式。
    - CustomCompute函数。用户可自定义实现。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```
    
  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  SIZE=1024
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake .. -DSIZE=$SIZE;make -j;                                            # 编译工程
  python3 ../scripts/gen_data.py -size=$SIZE                                # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 测试场景说明

### 测试单个API不同场景

以Axpy为例：  
```c++
// half+half场景
// ComputeMode::USE_COUNT_MODE表示使用count模式计算
// CaseType::NORMAL表示一般场景下使用API，不分离复合指令，不重叠操作数
// false表示不进行性能测试，在开启时会重复调用API多次
using srcType = half;                                      // 源操作数的数据类型
using dstType = half;                                      // 目的操作数的数据类型
using CurrentConfig = KernelConfig<ComputeMode::USE_COUNT_MODE, CaseType::NORMAL, false>;
```  
用户可根据自身需要自定义配置。  

### 测试其他API

当前默认为Axpy接口，如果需要改为其它两输入一输出接口，需要进行以下修改
1. 修改tensor_tensor_tensor.asc文件
    ```c++
    /************************ 用户可自定义配置 ********************** */
    constexpr uint32_t size = SIZE;                             // 计算量
    constexpr float scalar = 2.0;                               // 传给API的scalar值
    constexpr uint32_t REPEAT_TIMES_FOR_PERF_TEST = 1000;       // 性能测试中的循环次数
    using srcType = half;                                       // 源操作数的数据类型
    using dstType = float;                                      // 目的操作数的数据类型
    using CurrentConfig = KernelConfig<ComputeMode::USE_COUNT_MODE, TestType::NORMAL, false>;
    #define TEMPLATE_API Axpy
    /************************************************************** */
    ```
2. 修改gen_data.py
    ```py
    # 数据类型需要根据情况修改
    src_dtype = np.float16
    dst_dtype = np.float32

    def compute(input_x, value, golden):
        golden = (input_x * value + golden).astype(dst_dtype)
    ```
3. 修改verify_result.py
    ```py
    def verify_result(output, golden):
        # 数据类型需要用户根据实际情况修改
        output_type = np.float32
    ```
## 实测性能说明

### Axpy性能数据
| 输入数据类型 | 输出数据类型 | SIZE（单位元素） | 理论性能（cycles） | 源操作数与目的操作数不重叠（cycles） | 拆分为Muls和Add（cycles） | 源操作数与目的操作数重叠（cycles） |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| half | half | 32K | 256 | 282 | 568 | 282 |
| float | float | 16K | 256 | 282 | 568 | 282 |
| half | float | 16K | 256 | 280 | 603 | / |