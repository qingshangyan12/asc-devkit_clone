# cast_reg_api 样例

## 概述

本样例使用 experimental Reg Tensor API 的 `asc::te::cast` 完成寄存器级数据类型转换，不使用
`Transform` 接口。完整计算流水为：

```text
GM -> UB -> load/unpack -> reg_tensor cast -> store/pack -> UB -> GM
```

样例覆盖以下场景：

| SCENARIO_NUM | 转换 | `cast_round_mode` | `cast_sat_mode` | `cast_layout` |
| --- | --- | --- | --- | --- |
| 1 | `half -> int32_t` | `floor` | `not_sat` | `zero` |
| 2 | `float -> int16_t` | `rint` | `sat` | `zero` |
| 3 | `int8_t -> int32_t` | `rint` | `not_sat` | `zero` |
| 4 | `int32_t -> uint8_t` | `rint` | `sat` | `zero` |
| 5 | `bfloat16_t -> float` | `rint` | `not_sat` | `zero` |
| 6 | `float -> bfloat16_t` | `rint` | `not_sat` | `zero` |

位宽扩展场景通过 unpack load 将连续输入展开到寄存器 lane，位宽收缩场景通过 packed store
把转换结果连续写回 UB。`cast` 只完成寄存器转换并原样传播 mask，不负责数据搬运或 lane 拼接。

## 编译运行

在本样例目录执行：

```bash
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

CPU 调试或 NPU 仿真模式分别增加 `-DCMAKE_ASC_RUN_MODE=cpu` 或
`-DCMAKE_ASC_RUN_MODE=sim`。切换模式前需要清理 `build/CMakeCache.txt`。

执行成功时输出：

```text
test pass!
```

> `asc::te::cast` 当前为 experimental 接口，样例包含 `tensor_api/arch/vector/experimental/cast/reg_cast.h`。
