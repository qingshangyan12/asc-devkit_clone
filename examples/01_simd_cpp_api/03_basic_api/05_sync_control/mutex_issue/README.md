# Mutex跨流水同步写法验证样例

## 概述

本样例在Ascend 950PR/Ascend 950DT上验证连续两个Vector操作与后续MTE3搬出之间的Mutex同步写法。样例通过`SCENARIO_NUM`选择场景，并显式关闭毕昇编译器自动同步，避免自动插入的同步掩盖待验证代码的行为。

## 验证场景

| `SCENARIO_NUM` | 场景 | 实机结果 | 结论 |
| --- | --- | --- | --- |
| 0 | 一组`PIPE_V`锁包含两个有依赖的VF，VF之间插入`PipeBarrier<PIPE_V>`，随后锁`PIPE_MTE3`搬出 | 通过 | 正确；但3510架构的`PIPE_V`同步由硬件保证，因此该barrier可省略 |
| 1 | 两组同MutexID的`PIPE_V`锁分别包含一个VF，两组之间插入`PipeBarrier<PIPE_V>`，随后锁`PIPE_MTE3`搬出 | 通过 | 正确；第二次Lock不能替代单流水barrier，但仍参与Mutex获取和释放，该写法存在冗余 |
| 2 | 一组`PIPE_V`锁包含两个有依赖的VF，不插入`PipeBarrier<PIPE_V>`，随后锁`PIPE_MTE3`搬出 | 通过 | 正确；验证3510架构的`PIPE_V`同步由硬件保证 |
| 3 | 两个无依赖VF分别使用两组同MutexID的`PIPE_V`锁，随后锁`PIPE_MTE3`搬出 | 通过 | 正确；MTE3在第二次`Unlock<PIPE_V>`后执行，但两组V锁可以合并 |
| 4 | 两个无依赖VF共用一组`PIPE_V`锁且不插barrier，随后锁`PIPE_MTE3`搬出 | 通过 | 正确，且是本场景的推荐写法 |

场景3和场景4中的`Vector2`连续执行32个VF，并将最终值32搬出。`DataCopyPad`优先搬出`Vector2`的结果，以扩大并观测MTE3提前执行的竞争窗口。

> [!NOTE]说明
> `PipeBarrier<PIPE_V>`用于PIPE_V单流水内的依赖同步，Mutex用于不同流水之间的同步，两者解决的问题不同。上述“PIPE_V同步由硬件保证”仅针对NPU架构3510，不应扩展到其他架构；Mutex本身也仅支持Ascend 950PR/Ascend 950DT。

## 推荐写法

当`Vector1`和`Vector2`之间无数据依赖，且MTE3依赖二者完成时，可将两个VF放入同一组V锁中：

```cpp
AscendC::Mutex::Lock<PIPE_V>(mutexId);
Vector1();
Vector2();
AscendC::Mutex::Unlock<PIPE_V>(mutexId);

AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
AscendC::DataCopyPad(...);
AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
```

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
mutex_issue
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── data_utils.h
├── mutex_issue.asc
├── README.md
├── README_en.md
└── run_all.sh
```

## 编译运行

配置CANN环境变量后，在本样例目录执行全部场景：

```bash
source ${install_path}/cann/set_env.sh
bash run_all.sh
```

也可单独验证一个场景：

```bash
SCENARIO_NUM=4
cmake -S . -B build_scenario_${SCENARIO_NUM} \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DSCENARIO_NUM=${SCENARIO_NUM}
cmake --build build_scenario_${SCENARIO_NUM} -j
cd build_scenario_${SCENARIO_NUM}
python3 ../scripts/gen_data.py -scenarioNum ${SCENARIO_NUM}
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

全部场景的预期校验结果均为：

```text
different elements: 0/262144
test pass!
```
