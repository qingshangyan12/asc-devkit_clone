# Data Reorder Tensor API 样例

## 概述

本样例展示 `data_reorder.h` 中寄存器数据重排接口的用法，使用 `SCENARIO_NUM` 选择场景：

| SCENARIO_NUM | 接口 | 功能 |
| --- | --- | --- |
| 1 | `interleave` | 将两个寄存器按元素交织，返回两个结果寄存器 |
| 2 | `deinterleave` | 将两个连续寄存器按奇偶位置拆分，返回两个结果寄存器 |

输入 `x`、`y` 均为 64 个 `float`，对应 dav-3510 的一个 256 字节向量寄存器。输出为两个结果寄存器连续存放，共 128 个 `float`。

## 目录结构

```text
data_reorder_api
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_reorder_api.asc
├── data_utils.h
└── README.md
```

## 编译运行

在本目录执行：

```bash
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

场景 2 将 `SCENARIO_NUM` 改为 `2`。CPU 调试或 NPU 仿真时，在 CMake 命令中分别增加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim`。切换编译场景或运行模式前，应清理 CMake 缓存并重新配置。

运行成功时输出：

```text
test pass!
```
