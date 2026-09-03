# operator+接口样例

## 概述

本样例介绍如何使用实验性 Tensor API `asc::te::experimental::operator+` 实现两个 `float` Tensor 的逐元素加法。输入 `src0` 和 `src1` 均为 shape 为 `[2, 64]` 的二维 Tensor，输出 `dst` 的 shape 也为 `[2, 64]`，数据类型均为 `float`，输出元素为两个输入对应位置元素之和。

计算关系如下：

```text
dst[i] = src0[i] + src1[i]
```

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── CMakeLists.txt                 // 编译工程
├── README.md                      // 样例说明
└── add.asc                        // operator+接口样例实现
```

## 样例描述

- 样例场景：

  | SCENARIO_NUM | Reg Tensor Layout | 说明 |
  | --- | --- | --- |
  | 1 | 一维，shape 为 `[128]` | 按一维偏移加载和存储每个寄存器块。 |
  | 2 | 二维，shape 为 `[2, 64]` | 按二维坐标加载和存储每一行。 |

- 样例功能：

  两个场景均将输入 Tensor 按行从 GM 搬运到 UB，通过 `load` 加载到寄存器，调用实验性 Tensor API Reg Tensor `operator+` 完成逐元素加法，再通过 `store` 将结果写回 UB 和 GM。

- 样例实现：

  - Kernel关键步骤

    1. 使用 `asc::te::make_tensor`、`asc::te::make_mem_ptr` 和 `asc::te::make_frame_layout` 构造 GM、UB 上 shape 均为 `[2, 64]` 的二维搬运 Tensor。
    2. 使用 `asc_lock/asc_unlock` 包围 MTE2 流水，并通过 `asc::te::copy` 的坐标和 `copy_shape(1, 64)` 参数，按行将 GM 数据搬运到 UB。
    3. 基于同一块 UB 内存创建一维和二维计算 Tensor，根据 `SCENARIO_NUM` 选择 `compute_add_1d` 或 `compute_add_2d` 处理。
    4. 在计算函数中循环使用 `all_mask`、`load`、实验性 `operator+` 和 `store` 完成逐元素加法。
    5. 使用 `asc::te::copy`，按行将结果从 UB 写回 GM 中对应的二维区域。

  核心计算代码如下：

  ```cpp
  const auto coord = asc::te::make_coord(row, 0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src0 = asc::te::experimental::load(src0_tensor, coord).with_mask(mask);
  auto src1 = asc::te::experimental::load(src1_tensor, coord).with_mask(mask);
  auto dst = src0 + src1;
  asc::te::experimental::store(dst_tensor, coord, dst);
  ```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  配置 CANN 软件包环境变量，详细操作请参考[环境变量配置](../../../../../../docs/zh/quick_start.md#prepare&install)。

  ```bash
  source ${install_path}/set_env.sh
  ```

  `${install_path}` 为 CANN 软件包安装目录，请根据实际安装路径替换。

- 样例执行

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  使用 NPU 仿真模式时，在 CMake 命令中增加 `-DCMAKE_ASC_RUN_MODE=sim`。切换运行模式、芯片型号或场景时，建议清除 `build` 目录中的 CMake 缓存后重新配置。

- 执行结果

  验证成功时输出：

  ```text
  test pass!
  ```
