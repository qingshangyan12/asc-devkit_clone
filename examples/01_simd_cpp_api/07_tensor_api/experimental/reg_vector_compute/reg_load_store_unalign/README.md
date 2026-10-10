# 非对齐寄存器加载与存储接口样例

## 概述

本样例使用实验性 Tensor API `asc::te::experimental::unalign_load` 和 `unalign_store`，演示 UB 与寄存器之间的非对齐数据搬运，不直接调用非对齐 C API 或基础 API 的 `AscendC::Reg::LoadUnAlign/StoreUnAlign`。GM 与 UB 之间通过 Tensor API `copy` 搬运，寄存器计算使用实验性 Tensor API，流水同步仍使用 C API `asc_lock/asc_unlock`。

样例包含两个场景，通过 `SCENARIO_NUM` 选择：

| 场景 | 输入 | 输出 | 功能 |
| --- | --- | --- | --- |
| 1 | `float [1024]` | `float [1024]` | 从第 1 个元素开始非对齐复制两个寄存器，即 128 个元素；其余输出元素为 0。 |
| 2 | `uint16_t [14, 255]` | `uint16_t [14, 16]` | 4 个核按行分工，每核最多 4 行，尾核处理 2 行；每 16 个输入元素取最大值，最后一个数据块只包含 15 个有效元素。 |

## 支持的产品

- Ascend 950PR/Ascend 950DT

CANN 软件包须包含 PR 6049 新增的 `unalign_load` / `unalign_store` 接口。本次验证使用同步了这些接口并修正连续加载状态推进的 CANN 9.3.0；未同步接口的安装包不能直接编译本样例。`unalign_load` 内部使用 postupdate 操作更新加载状态，外部 Tensor 和坐标保持不变。CMake 从 CANN 包中加载接口，不引用本仓头文件。

## 目录结构介绍

```text
├── CMakeLists.txt                 // 编译工程及 CTest 校验入口
├── data_utils.h                   // 样例本地工具头文件
├── README.md                      // 中文说明
├── README_en.md                   // 英文说明
└── reg_load_store_unalign.asc     // 核函数、输入生成和结果校验
```

## 样例描述

### 场景 1：地址非对齐复制

1. 将输入从 GM 搬入 UB，并使用 `fill/store` 将输出 UB 清零。
2. 在核函数中通过 `slice` 创建源、目的 Tensor 视图，均从第 1 个元素开始，即偏移 4 字节，再将视图传入 VF。
3. 创建 `unalign_load` 和 `unalign_store` 会话，调用 `loader.prepare(src_stream)` 初始化连续加载状态。
4. 通过 `loader.load` 和 `storer.store`，按相对视图的坐标连续加载、存储两个完整寄存器，每次加载和存储显式传入 `count`。
5. 调用 `storer.flush(dst_stream, 2 * elements_per_register)` 提交尚未写出的数据，再将输出搬回 GM。

核心代码如下：

```cpp
asc::te::experimental::unalign_load loader;
asc::te::experimental::unalign_store storer;
loader.prepare(src_stream);
for (uint32_t repeat = 0; repeat < 2; ++repeat) {
    auto coord = asc::te::make_coord(repeat * elements_per_register);
    auto src = loader.load(src_stream, coord, elements_per_register);
    storer.store(dst_stream, coord, src, elements_per_register);
}
storer.flush(dst_stream, static_cast<int32_t>(2 * elements_per_register));
```

### 场景 2：多核地址和数据量非对齐归约

1. 使用 `block_idx` 计算当前核的起始行和实际行数，将各核输入按连续的一维数据搬入 UB。
2. 每行包含 255 个 `uint16_t` 元素，行间不插入填充；后续行的 UB 起始地址非 32 字节对齐，后续核的 GM 输入地址也非 32 字节对齐。
3. 在核函数中按行创建源、目的 Tensor 视图，每行调用一次 VF 并建立独立的 `unalign_load` / `unalign_store` 会话，连续加载两个 256 字节寄存器。源 UB 区域额外预留一个寄存器的空间，避免尾部完整寄存器加载访问输出区域。
4. 使用 `update_mask<uint16_t>` 屏蔽最后一个无效元素，调用 `reduce_max<reduce_scope::datablock>` 完成每 32 字节数据块的最大值归约，数据类型由输入推导。
5. 使用 `storer.store` 在目的行的坐标 0、8 各非对齐存储 8 个结果，最后调用 `storer.flush(dst_row, output_columns)`，将输出搬回 GM。

**接口约束：** 每段连续加载前先调用 `prepare`，第一次加载的地址须与准备地址一致，后续加载地址须连续。加载和存储坐标相对传入的 Tensor 视图，显式由样例计算，不会更新外部 Tensor 或坐标。`load` 读取完整寄存器，其 `count` 指定加载流推进的元素数量，本样例传入每个寄存器的元素数；`store` 按 `count` 个元素写出，不使用 Reg Tensor 的 mask。有效元素屏蔽在归约计算阶段完成。每段存储结束时调用一次 `flush`，传入首次存储使用的目的 Tensor 视图和流末尾相对该视图的元素偏移。CANN 9.3.0 的 `slice` 不可在 VF 中直接调用，因此视图在核函数中构造。

### 结果校验

主机端生成确定性输入，并直接计算 golden，不依赖 Python 或外部数据文件。场景 1 校验全部 1024 个输出，包括复制范围之外的零值；场景 2 校验全部 224 个归约结果，包括每行的尾数据块和尾核。ACL 调用失败或结果不一致时程序返回非零值。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  根据 CANN 软件包安装目录加载环境变量：
  ```bash
  source ${install_path}/set_env.sh
  ```
  `${install_path}` 为 CANN 软件包安装目录。样例通过 `find_package(ASC REQUIRED)` 使用 CANN 软件包中的 Tensor/C API 头文件、编译器、ACL 运行库及仿真库。

- 样例执行

  下面以场景 1 为例，默认使用 NPU 模式：

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  运行场景 2 时，将 `SCENARIO_NUM` 修改为 `2` 后重新配置、编译并执行。输入和 golden 由主程序生成，无需执行数据生成脚本。

  使用 NPU 仿真时，增加 `-DCMAKE_ASC_RUN_MODE=sim` 编译选项：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  ./demo
  ```

  切换运行模式、芯片型号或场景时，建议清除 build 目录中的 CMake 缓存后重新配置。可在 build 目录下执行 `rm CMakeCache.txt`，然后使用所需的 `SCENARIO_NUM` 重新运行上述编译命令。

  > **注意：** 更新 CANN 包中的头文件后，应执行 `make clean` 再 `make -j`，或使用新的构建目录，避免复用更新前的编译产物。

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | NPU 运行或 NPU 仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | 对应 Ascend 950PR/Ascend 950DT |
| `CANN_ASC_USE_EXPERIMENTAL` | `ON`（本样例必选）、`OFF`（默认） | 开启实验性 ASC 接口 |
| `SCENARIO_NUM` | `1`（默认）、`2` | 非对齐复制或 datablock 最大值归约 |

- 执行结果

验证成功时，两个场景分别输出：

```text
scenario 1: test pass!
scenario 2: test pass!
```
