# 矢量计算边界值检测样例

## 概述

本样例采用静态Tensor编程方式，演示调用[NumericLimits](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/intro.md)工具类，结合[Compare](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compare.md)和[Compares](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares.md)接口，逐元素检测矢量加法结果中的NaN、正无穷大和负无穷大。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR&950DT系列产品 | CANN 9.3.0 weekly.20261009.01（已验证） |

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

  计算`z = x + y`，输出计算结果及三组比较结果：

  | 比较结果 | 判断条件 | 实现 |
  | --- | --- | --- |
  | NaN | `z[i] != z[i]` | Compare，NE模式 |
  | 正无穷大 | `z[i] == +Inf` | NumericLimits<float>::Infinity矢量接口生成比较值，Compare，EQ模式 |
  | 负无穷大 | `z[i] == -Inf` | NumericLimits<float>::NegativeInfinity标量接口生成比较值，Compares，EQ模式 |

  NaN不等于自身，不能通过与QuietNaN进行相等比较检测NaN。最大有限值和最低有限值不属于NaN或Inf，不会被标记。

- 样例规格

  | 类别 | name | shape | data type | format |
  | --- | --- | --- | --- | --- |
  | 输入 | x、y | 各[256] | float | ND |
  | 输出 | z | [256] | float | ND |
  | 输出 | masks | [3, 32] | uint8_t | ND |

  每个比较结果占1 bit，比较成立时为1，否则为0。masks的三行依次表示NaN、正无穷大、负无穷大；第`i`个元素对应第`i / 8`字节的第`i % 8`位（从最低位开始）。输入包含普通有限值、零、最大/最低有限值、正负无穷大和NaN；还覆盖有限值相加溢出以及正负无穷大相加产生NaN的情况。

- 样例实现

  **Kernel实现**

  1. 调用[InitSocState](../../../../../docs/zh/api/SIMD-API/basic_api/tool_interface/system_init/InitSocState.md)初始化全局状态寄存器。
  2. 使用[LocalMemAllocator](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/LocalMemAllocator/LocalMemAllocator_intro.md)静态分配UB内存，构造输入、计算结果、比较值和比较结果的LocalTensor。四个float缓冲区各占1024字节，比较结果缓冲区占96字节，共4192字节，各缓冲区起始地址满足32字节对齐要求。
  3. 调用DataCopy将x、y从GM（Global Memory）搬运到UB（Unified Buffer），通过[SetFlag/WaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)的MTE2_V事件保证搬运完成后执行矢量计算。
  4. 调用Add计算`z = x + y`，通过NumericLimits获取正负无穷大，调用Compare和Compares逐元素检测NaN、正无穷大和负无穷大。
  5. 通过V_MTE3事件保证矢量计算完成后，将z和masks从UB搬运到GM；核函数结束前调用[PipeBarrier](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)，等待流水线任务完成。

  **调用实现**

  Host侧读取输入数据，申请Device内存并将输入拷贝到Device，使用内核调用符`<<<>>>`启动一个Vector核。同步等待核函数执行完成后，将计算结果和比较结果拷贝回Host并写入文件。

  **数据生成与结果验证**

  gen_data.py使用固定随机种子生成输入，通过NumPy计算加法结果及三组比较结果。verify_result.py分别检查NaN位置、正负无穷大位置、有限值结果，并逐字节比较三组比特位，校验失败时返回非零退出码。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。运行数据生成和结果验证脚本需安装Python 3及NumPy。

- 配置环境变量

  请根据CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录。本样例使用上述weekly版本验证，使用独立安装目录时请填写对应路径。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  MODE=npu
  cmake -S . -B build_${MODE} -DCMAKE_ASC_RUN_MODE=${MODE} -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build_${MODE} -j
  (
      cd build_${MODE} || exit 1
      python3 ../scripts/gen_data.py &&
      ./demo &&
      python3 ../scripts/verify_result.py output
  )
  ```

  使用CPU调试或NPU仿真模式时，将`MODE`分别设置为`cpu`或`sim`，执行相同命令，CMake分别使用`-DCMAKE_ASC_RUN_MODE=cpu`或`-DCMAKE_ASC_RUN_MODE=sim`配置工程。

  > **说明：** 三种模式使用独立构建目录，切换模式时无需清理其他模式的CMake缓存。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU上板、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | 对应Ascend 950PR&950DT系列产品 |

- 执行结果

  执行结果如下，说明计算结果和比较结果验证通过。

  ```text
  NaN: 3, +Inf: 3, -Inf: 3
  test pass!
  ```

  > **说明：** NPU仿真时，测试数据中的NaN/Inf可能触发模型的`vec_err_idata_inf_nan_t0`诊断日志。本样例验证中仿真正常结束，逐元素校验通过；请同时检查程序退出码和校验结果。
