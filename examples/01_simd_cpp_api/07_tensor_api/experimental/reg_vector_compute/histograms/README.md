# histograms接口样例

## 概述

本样例介绍如何使用实验性 Tensor API `asc::te::experimental::histograms`，对256个 `uint8_t` 元素统计高半区 `[128, 255]` 的累计直方图，输出128个 `uint16_t` 元素。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── CMakeLists.txt                 // 编译工程
├── README.md                      // 样例说明
└── histograms.asc                 // histograms接口样例实现
```

## 样例描述

- 样例功能：

  样例将输入 Tensor 从 GM 搬运到 UB，通过 `load` 加载到寄存器，调用高半区累计模式的 `histograms`，再将结果写回 UB 和 GM。Host 侧直接生成输入并校验输出。

- 样例场景：

  | SCENARIO_NUM | 输入和输出 Tensor Layout | 说明 |
  | --- | --- | --- |
  | 1 | 一维，shape 分别为 `[256]` 和 `[128]` | 按一维坐标处理完整输入和输出。 |
  | 2 | 二维，shape 分别为 `[1, 256]` 和 `[1, 128]` | 使用二维坐标处理完整输入和输出。 |

- 样例实现：

  核心计算代码如下：

  ```cpp
  template <typename SrcTensor, typename DstTensor>
  __simd_vf__ inline void histograms_vf(
      const SrcTensor src, DstTensor dst, uint16_t repeat_times, uint32_t total, uint32_t one_repeat)
  {
      uint32_t remain = total;
      asc::te::experimental::reg_tensor<uint16_t> dst_reg;
      uint32_t dst_remain = bin_length;
      dst_reg.mask = asc::te::experimental::update_mask<uint16_t>(dst_remain).reg;
      asc_duplicate_scalar(dst_reg.reg, initial_value, dst_reg.mask);
      for (uint16_t i = 0; i < repeat_times; ++i) {
          const uint32_t offset = i * one_repeat;
          const auto coord = asc::te::make_coord(offset);
          auto mask = asc::te::experimental::update_mask<uint8_t>(remain);
          auto src_reg = asc::te::experimental::load(src, coord).with_mask(mask);
          dst_reg = asc::te::experimental::histograms<histogram_config>(dst_reg, src_reg);
      }
      asc::te::experimental::store(dst, asc::te::make_coord(0), dst_reg);
  }
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
