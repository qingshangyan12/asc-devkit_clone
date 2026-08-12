# truncate_reg_api 样例

## 概述

本样例使用 experimental Reg Tensor API 的 `asc::te::trunc`，将寄存器中的浮点数逐元素向零截断。
完整计算流水为：

```text
GM -> UB -> reg_tensor load -> asc::te::trunc -> reg_tensor store -> UB -> GM
```

样例覆盖接口支持的全部数据类型：

| `SCENARIO_NUM` | 输入/输出类型 | 示例 |
| --- | --- | --- |
| 1 | `half` | `-3.9 -> -3.0` |
| 2 | `bfloat16_t` | `-3.9 -> -3.0` |
| 3 | `float` | `3.9 -> 3.0` |

`asc::te::trunc` 只处理源 `reg_tensor` 中 mask 激活的元素，并将源 mask 原样传递给返回值。

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

依次将 `SCENARIO_NUM` 设置为 `1`、`2` 和 `3`，可以验证全部支持类型。CPU 调试或 NPU
仿真模式分别增加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim`。切换模式前需要清理
`build/CMakeCache.txt`。

执行成功时输出：

```text
test pass!
```

> `asc::te::trunc` 当前为 experimental 接口，样例包含公开头文件
> `tensor_api/arch/vector/experimental/type_conversion.h`。
