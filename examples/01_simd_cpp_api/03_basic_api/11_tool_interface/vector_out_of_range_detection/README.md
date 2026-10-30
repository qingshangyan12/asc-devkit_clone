# 矢量计算边界值检测样例

## 概述

本样例演示调用[NumericLimits](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/intro.md)工具类，结合[Compare](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare.md)和[Compares](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares.md)接口，逐元素检测矢量加法结果中的正无穷大和负无穷大。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR&950DT系列产品 | >= CANN 9.3.0 |

## 目录结构介绍

```text
vector_out_of_range_detection
├── scripts
│   ├── gen_data.py                    # 生成输入数据和真值数据
│   └── verify_result.py               # 逐元素验证计算结果和比较结果
├── CMakeLists.txt                     # 编译工程文件
├── data_utils.h                       # 数据读入写出函数
├── vector_out_of_range_detection.asc  # 核函数及调用实现
├── README.md
└── README_en.md
```

## 样例描述

- 样例功能

  计算`z = x + y`，输出计算结果及两组比较结果：

  | 比较结果 | 判断条件 | 实现 |
  | --- | --- | --- |
  | 正无穷大 | `z[i] == +Inf` | 使用`NumericLimits<float>::Infinity`矢量接口生成用于比较的正无穷矢量，使用Compare接口与矢量z进行逐元素比较 |
  | 负无穷大 | `z[i] == -Inf` | 使用`NumericLimits<float>::NegativeInfinity`标量接口生成用于比较的负无穷标量值，使用Compares接口与矢量z进行逐元素比较 |

- 样例规格

  | 类别 | name | shape | data type | format |
  | --- | --- | --- | --- | --- |
  | 输入 | x、y | [256] | float | ND |
  | 输出 | z | [256] | float | ND |
  | 输出 | masks | [2, 32] | uint8_t | ND |

  每个比较结果占1 bit，判断条件成立时为1，否则为0。masks的两行依次表示正无穷大、负无穷大的比较结果；第`i`个元素对应第`i/8`字节的第`i%8`位（从最低位开始）。输入包含普通有限值、零、最大/最低有限值、正负无穷大，还覆盖有限值相加溢出的情况。

- 样例实现

  **Kernel实现**

  1. 调用[InitSocState](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/system_init/InitSocState.md)初始化全局状态寄存器。
  2. 使用[LocalMemAllocator](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/LocalMemAllocator/LocalMemAllocator_intro.md)静态分配UB内存，构造输入、计算结果、比较值和比较结果的LocalTensor。四个float矢量各占1024字节，比较结果矢量占64字节，共4160字节，各内存块起始地址满足32字节对齐要求。
  3. 调用DataCopy将x、y从GM（Global Memory）搬运到UB（Unified Buffer），通过[SetFlag/WaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)的MTE2_V事件保证搬运完成后执行矢量计算。
  4. 调用Add计算`z = x + y`，通过NumericLimits获取正负无穷大，调用Compare和Compares逐元素检测正无穷大和负无穷大。
  5. 通过V_MTE3事件保证矢量计算完成后，将z和masks从UB搬运到GM；核函数结束前调用[PipeBarrier](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)，等待流水线任务完成。

  **结果文件与验证**

  `output/output.bin`和`output/golden.bin`采用相同布局：前1024字节为256个float类型的加法结果z，后64字节为两组uint8_t类型的比较结果masks（正无穷大在前，负无穷大在后）。验证脚本接收输出文件和真值文件两个路径参数，检查文件大小、正负无穷大位置、有限值结果和全部比较比特位。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。数据生成和验证脚本依赖Python 3及NumPy。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项　　　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真　　　　　　　　　　　　　　　　　　　　　　　　 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR&950DT系列产品 |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
