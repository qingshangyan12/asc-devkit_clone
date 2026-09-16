# reduce_sum/reduce_max/reduce_min接口样例

## 概述

本样例介绍如何使用实验性 Tensor API `asc::te::experimental::reduce_sum`、`reduce_max` 和 `reduce_min` 对一个 `float` Reg Tensor 进行 element scope 归约。输入 `src` 是 shape 为 `[64]` 的一维 Tensor，三个归约结果分别写入输出 Tensor 中三个长度为 64 的结果区。

计算关系如下：

```text
dst[0]   = sum(src[0:64])
dst[64]  = max(src[0:64])
dst[128] = min(src[0:64])
```

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── CMakeLists.txt                 // 编译工程
├── README.md                      // 中文样例说明
├── README_en.md                   // 英文样例说明
└── reduction_compute.asc          // 归约接口样例实现
```

## 样例描述

- 样例功能：

  样例将输入 Tensor 从 GM 搬运到 UB，通过 `load` 加载到寄存器，依次调用实验性 Reg Tensor 归约接口完成求和、求最大值和求最小值，再通过 `store` 将三个归约结果写回 UB 和 GM。默认模板参数使用 `reduce_scope::element`，每个归约结果位于对应输出区的首元素。

- 样例实现：

  - Kernel关键步骤

    1. 使用 `asc::te::make_tensor`、`asc::te::make_mem_ptr` 和 `asc::te::make_frame_layout` 构造 GM、UB 上的一维 Tensor。
    2. 使用 `asc_lock/asc_unlock` 包围 MTE2 流水，通过 `asc::te::copy` 将输入从 GM 搬运到 UB。
    3. 使用 `all_mask` 和 `load` 将 64 个 `float` 数据加载为 Reg Tensor。
    4. 调用 `reduce_sum<float>`、`reduce_max<float>` 和 `reduce_min<float>` 完成 element scope 归约。
    5. 使用 `store` 和 `asc::te::copy` 将结果写回 UB 和 GM，并在 Host 侧校验三个有效结果。

  核心计算代码如下：

  ```cpp
  const auto coord = asc::te::make_coord(0);
  auto mask = asc::te::experimental::all_mask<float>();
  auto src = asc::te::experimental::load(src_tensor, coord).with_mask(mask);

  auto sum = asc::te::experimental::reduce_sum<float>(src);
  auto max = asc::te::experimental::reduce_max<float>(src);
  auto min = asc::te::experimental::reduce_min<float>(src);

  asc::te::experimental::store(sum_tensor, coord, sum);
  asc::te::experimental::store(max_tensor, coord, max);
  asc::te::experimental::store(min_tensor, coord, min);
  ```

  `reduce_sum` 还支持 `reduce_scope::datablock` 和 `reduce_scope::pair`，`reduce_max`、`reduce_min` 还支持 `reduce_scope::datablock`。可通过第二个模板参数显式指定归约范围。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  配置 CANN 软件包环境变量，详细操作请参考[环境变量配置](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md)。

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

  使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 执行结果

  验证成功时输出：

  ```text
  test pass!
  ```
