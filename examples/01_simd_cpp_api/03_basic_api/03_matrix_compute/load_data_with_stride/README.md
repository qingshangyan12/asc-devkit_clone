# 矩阵乘法中LoadDataWithStride数据搬运示例

## 概述

本样例介绍LoadDataWithStride指令在矩阵乘法中的使用场景和方法。配合SetLoadDataRepeatWithStride可以将二维的A、B矩阵从L1搬运到L0A/L0B，其中 A 和 B 分别表示矩阵乘法的左右输入矩阵。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── load_data_with_stride
│   ├── figures                     // 图示
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── load_data_with_stride.asc   // Ascend C样例实现 & 调用样例
│   └── README.md                   // 样例说明文档
```

## 算子描述

LoadDataWithStride指令对于二维矩阵的转置能力和支持的数据类型，具体来说：

- 支持数据类型为：int8_t/uint8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t/half/bfloat16_t/int32_t/uint32_t/float。  
- 仅在目的地址位于L0A上，并且类型为b8/b16/b32时，enTranspose参数有效，能够决定是否启用转置功能。 
  目的地址位于L0B上时，会自动进行转置，enTranspose参数无效。

由于本样例暂不支持输入数据类型为int4b_t，因此本样例展示了以下六种LoadDataWithStride在矩阵乘法的使用：

### LoadDataWithStride接口场景对照表

| scenarioNum | 输入数据类型 | A矩阵转置 | B矩阵转置 |
| --- | --- | --- | --- |
| 1 | int8_t | 不转置 | 不转置 |
| 2 | int8_t | 转置 | 不转置 |
| 3 | half | 不转置 | 不转置 |
| 4 | half | 转置 | 不转置 |
| 5 | float | 不转置 | 不转置 |
| 6 | float | 转置 | 不转置 |

### 使用LoadDataWithStride完成矩阵计算所需2D格式数据的搬运

LoadDataWithStride本质上是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。

对于NC1HWC0格式的数据，当N=1，卷积核width和height为1，padding为0，卷积核在源操作数width维度和height维度滑动的步长为1，卷积核width和height膨胀系数为1，此时经过image to column展开后的数据排布可以看成矩阵计算过程中L1 Buffer上的NZ分形排布，因此该场景下可以使用LoadDataWithStride接口进行矩阵计算所需2D格式数据的搬运，示意图如下所示：

<div align="center">
  <img src="figures/load_data_with_stride_2d.png" width="900"><br>
  图1：使用LoadDataWithStride进行2D格式数据搬运示意图
</div>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # CPU调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim`、`cpu` | 运行模式：NPU运行、NPU仿真、CPU调试 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（必须） | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2`、`3`、`4`、`5`、`6` | 场景编号 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```