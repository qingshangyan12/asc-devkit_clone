# cast样例

## 概述

本样例基于 Memory 矢量计算 C API [asc_half2int4](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md) 和 [asc_half2int32](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md)，演示 half 数据向上取整转换为 int4 或 int32。

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
cast/
├── scripts/gen_data.py       # 输入和golden数据生成脚本
├── CMakeLists.txt            # 编译工程文件
├── data_utils.h              # 二进制数据读取工具
├── cast.asc                  # C API内核和主机调用代码
├── README.md
└── README_en.md
```

## 场景说明

| `SCENARIO_NUM` | 功能 | 输入 | 输出 |
| --- | --- | --- | --- |
| 0 | half 转打包 int4，CEIL 舍入并饱和到 [-8, 7] | 512 个 half | 256 字节 |
| 1 | half 转 int32，CEIL 舍入 | 512 个 half | 512 个 int32 |

脚本生成固定的边界输入和对应 golden 数据。程序读取 `input/input_x.bin`、执行内核并逐字节比对 `output/golden.bin`。

## 编译运行

在本样例根目录执行以下步骤。

- 配置环境变量

  请根据 CANN 开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量。

```bash
source ${install_path}/cann/set_env.sh
```

  > **说明：** `${install_path}` 为 CANN 包安装目录；root 用户默认为 `/usr/local/Ascend`，非 root 用户默认为 `${HOME}/Ascend`。

- NPU运行

```bash
SCENARIO_NUM=0                                                                                  # 执行场景0
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- NPU仿真

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
```

切换运行模式或场景前，需清理 CMake 缓存，例如在 `build` 目录执行 `rm CMakeCache.txt` 后重新配置。

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |
| `SCENARIO_NUM` | `0`、`1` | 样例场景：half转打包int4、half转int32 |

- 执行结果

输出与 golden 数据比较通过时，打印如下信息：

```text
test pass!
```
