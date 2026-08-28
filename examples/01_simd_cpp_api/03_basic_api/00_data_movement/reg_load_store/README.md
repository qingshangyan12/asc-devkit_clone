# Load和Store样例

## 概述

本样例演示`Load`和`Store`易用性接口在`RegTraitNumOne`和`RegTraitNumTwo`场景下的使用方法。通过CMake编译参数`SCENARIO_NUM`选择场景，分别覆盖`Store`不带`count`和带`count`的两个重载。

| SCENARIO_NUM | RegTensor类型 | Store接口 | 搬运量 |
| --- | --- | --- | --- |
| 1 | `RegTensor<uint64_t, RegTraitNumOne>` | `Store(dstAddr, srcReg)` | 32个`uint64_t`元素（256B） |
| 2 | `RegTensor<uint64_t, RegTraitNumOne>` | `Store(dstAddr, srcReg, count)`，`count`为32 | 32个`uint64_t`元素（256B） |
| 3 | `RegTensor<uint64_t, RegTraitNumTwo>` | `Store(dstAddr, srcReg)` | 64个`uint64_t`元素（512B） |
| 4 | `RegTensor<uint64_t, RegTraitNumTwo>` | `Store(dstAddr, srcReg, count)`，`count`为64 | 64个`uint64_t`元素（512B） |

四个场景均将源地址和目的地址偏移一个`uint64_t`元素，使UB起始地址不满足32B对齐要求。`Load`和`Store`内部会完成非对齐搬运所需的预处理及后处理。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | 支持本特性的版本 |

## 目录结构介绍

```text
reg_load_store/
├── scripts/
│   └── gen_data.py        // 输入数据和真值数据生成脚本
├── CMakeLists.txt         // 编译工程文件
├── data_utils.h           // 数据读入写出函数
├── load_store.asc         // Ascend C样例实现和调用入口
├── README.md              // 中文说明
└── README_en.md           // 英文说明
```

## 样例描述

输入和输出均为68个`uint64_t`元素。Kernel先将数据从GM搬入UB，并将输入、输出UB地址各偏移8B，再调用`Load`将数据搬入RegTensor，最后调用当前场景对应的`Store`重载搬回输出区域。

- 场景1使用`RegTraitNumOne`和不带`count`的`Store`，搬运一个VL，即32个`uint64_t`元素。
- 场景2使用`RegTraitNumOne`和带`count`的`Store`，`count`设置为32，搬运一个VL。
- 场景3使用`RegTraitNumTwo`和不带`count`的`Store`，搬运两个VL，即64个`uint64_t`元素。
- 场景4使用`RegTraitNumTwo`和带`count`的`Store`，`count`设置为64，搬运两个VL。

## 编译运行

配置CANN环境后，在本样例目录执行：

```bash
source ${install_path}/cann/set_env.sh
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

将`SCENARIO_NUM`设置为`1`、`2`、`3`或`4`，可运行对应场景。切换场景前需要清理CMake缓存并重新编译。

执行成功时输出：

```text
test pass!
```
