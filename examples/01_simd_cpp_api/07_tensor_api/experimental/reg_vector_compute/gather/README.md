# gather接口样例

## 概述

本样例组合实验性 Tensor API 的寄存器加载、索引收集和存储能力，演示 `asc::te::experimental::gather` 的用法。用例设计参考基础 API 的 `03_basic_api/02_reg_vector_compute/gather` 样例，使用单个 vector 核处理 shape 为 `[128]` 的一维 `half` Tensor，索引 Tensor 的元素类型为 `uint16_t`。

计算关系如下：

```text
dst[i] = src[index[i]], 0 <= i < 128
```

索引范围为 `[0, 127]`，表示源寄存器内的元素位置，不是字节偏移或 GM 全局索引。128个 `half` 元素恰好占满一个256字节寄存器。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR&950DT系列产品 | >= CANN 9.3.0 |

## 目录结构介绍

```text
├── CMakeLists.txt          // 编译工程
├── README.md               // 中文样例说明
├── README_en.md            // 英文样例说明
├── gather.asc              // 算子Kernel、Host调用与结果校验
└── scripts
    ├── gen_data.py         // 输入数据和真值数据生成脚本
    └── verify_result.py    // 输出结果逐位校验脚本
```

## 样例描述

- 样例功能：

  样例从 `input/input_x.bin` 和 `input/input_y.bin` 读取源数据与索引，将输入 Tensor 从 GM 搬运到 UB，通过 `load` 加载到寄存器，调用实验性 Reg Tensor `gather` 接口按索引收集元素，再通过 `store` 将结果写回 UB 和 GM。Host 将输出保存到 `output/output.bin`，并使用 `output/golden.bin` 中的真值逐位校验全部128个元素。

- 样例规格：

  | 文件 | 内容 | 大小 |
  | --- | --- | --- |
  | `input/input_x.bin` | 128个 `half` 源数据元素 | 256 B |
  | `input/input_y.bin` | 128个 `uint16_t` 索引元素 | 256 B |
  | `output/output.bin` | 128个 `half` 输出元素 | 256 B |
  | `output/golden.bin` | 128个 `half` 真值元素 | 256 B |

- 样例实现：

  - Kernel关键步骤

    1. 使用 `asc::te::make_tensor`、`asc::te::make_mem_ptr` 和 layout 构造 GM、UB Tensor。
    2. 使用 `asc::te::mutex` 包围 MTE2 流水，通过 `make_copy(copy_gm_to_ub{})` 和 `copy` 将源数据与索引从 GM 搬运到 UB。
    3. 使用 `update_mask<half>` 创建128个有效元素的 mask，通过 `load` 加载源数据与索引，并使用 `with_mask` 设置有效元素。
    4. 调用 `gather(source_reg, index_reg)` 完成索引收集，结果继承索引寄存器的 mask。
    5. 使用 `store` 写入 UB 输出 Tensor，再通过 `make_copy(copy_ub_to_gm{})` 和 `copy` 写回 GM。

  核心计算代码如下：

  ```cpp
  const auto coord = asc::te::make_coord(0);
  uint32_t remaining = total_length;
  auto mask = asc::te::experimental::update_mask<half>(remaining);
  auto source_reg = asc::te::experimental::load(source, coord).with_mask(mask);
  auto index_reg = asc::te::experimental::load(indexes, coord).with_mask(mask);
  auto destination_reg = asc::te::experimental::gather(source_reg, index_reg);
  asc::te::experimental::store(destination, coord, destination_reg);
  ```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。数据生成与校验脚本依赖 Python 3 和 NumPy。

- 配置环境变量

  ```bash
  source ${install_path}/set_env.sh
  ```

  `${install_path}` 为 CANN 软件包安装目录，例如 `/usr/local/Ascend/cann`。

- 编译并执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py \
    --source input/input_x.bin --indexes input/input_y.bin --golden output/golden.bin
  ./demo
  ```

  数据生成脚本通过必填参数指定源数据、索引和真值文件路径，不内置文件名。`--case` 可选 `random`、`identity`、`reverse`、`repeated`，默认 `random`。Host 程序使用固定的样例目录布局，因此运行 `./demo` 时请使用上述路径。

  在支持的 NPU 环境运行时，可省略 `-DCMAKE_ASC_RUN_MODE=sim`，默认运行模式为 `npu`。使用仿真模式时，在上述 CMake 命令中添加 `-DCMAKE_ASC_RUN_MODE=sim`。切换运行模式或芯片型号时，建议先清除 `build` 目录中的 CMake 缓存。

- 编译选项说明

  | 选项 | 说明 |
  | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | 算子执行模式，可选 `npu`、`sim`，默认值为 `npu`。 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU芯片型号，本样例仅支持 `dav-3510`。 |

- 执行结果

  验证成功时输出：

  ```text
  PASS: 128/128 elements match golden bit-for-bit
  ```

  Host 程序已包含精度校验，也可以在 `build` 目录下执行以下命令，使用 Python 脚本独立校验输出文件：

  ```bash
  python3 ../scripts/verify_result.py --output output/output.bin --golden output/golden.bin
  ```
