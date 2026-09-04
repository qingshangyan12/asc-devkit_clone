# fma接口样例

## 概述

本样例介绍如何使用实验性 Tensor API `asc::te::experimental::fma` 对三组 shape 为 `[64]` 的一维 `float` Tensor 执行逐元素融合乘加计算。

```text
dst[i] = src0[i] * src1[i] + src2[i]
```

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── CMakeLists.txt                 // 编译工程
├── README.md                      // 样例说明
└── fma.asc                        // fma接口样例实现
```

## 样例描述

- 样例功能：

  样例将三组输入 Tensor 从 GM 搬运到 UB，通过 `load` 加载到寄存器，调用 `fma` 完成逐元素融合乘加，再通过 `store` 将结果写回 UB 和 GM。Host 侧直接生成输入并校验输出。

- 样例实现：

  - Kernel关键步骤

    1. 使用 `asc::te::make_tensor`、`asc::te::make_mem_ptr` 和 `asc::te::make_layout` 构造 GM、UB 上的一维 Tensor。
    2. 使用 `asc::te::copy` 将三组输入从 GM 搬运到 UB。
    3. 使用 `all_mask` 和 `load` 将 UB 数据加载为 Reg Tensor。
    4. 调用 `fma` 完成逐元素融合乘加。
    5. 使用 `store` 和 `asc::te::copy` 将结果写回 UB 和 GM。

  核心计算代码如下：

  ```cpp
  const auto coord = asc::te::make_coord(0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src0 = asc::te::experimental::load(src0_tensor, coord).with_mask(mask);
  auto src1 = asc::te::experimental::load(src1_tensor, coord).with_mask(mask);
  auto src2 = asc::te::experimental::load(src2_tensor, coord).with_mask(mask);
  auto dst = asc::te::experimental::fma(src0, src1, src2);
  asc::te::experimental::store(dst_tensor, coord, dst);
  ```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  配置 CANN 软件包环境变量，详细操作请参考[环境变量配置](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md#prepare&install)。

  ```bash
  source ${install_path}/set_env.sh
  ```

  `${install_path}` 为 CANN 软件包安装目录，请根据实际安装路径替换。

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  使用 NPU 仿真模式时，在 CMake 命令中增加 `-DCMAKE_ASC_RUN_MODE=sim`。切换运行模式或芯片型号时，建议清除 `build` 目录中的 CMake 缓存后重新配置。

- 执行结果

  验证成功时输出：

  ```text
  test pass!
  ```
