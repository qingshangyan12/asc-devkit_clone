# Reg Load Store Mask样例

## 概述

本样例演示Ascend C API中MaskReg的加载、存储和使用方式。场景1从UB加载MaskReg后执行掩码复制和MaskReg存储；场景2按MaskReg位选择两个`float`输入中的一个元素写出。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```text
reg_load_store_mask/
├── scripts/
│   ├── gen_data.py                // 生成输入和golden数据
│   └── verify_result.py           // 校验输出数据
├── CMakeLists.txt                 // 编译配置
├── data_utils.h                   // 输入输出文件工具
├── reg_load_store_mask.asc        // Ascend C API实现和调用入口
├── README.md                      // 中文说明
├── README_en.md                   // English documentation
└── figures/
    └── reg_load_store_mask.png    // 场景示意图
```

![MaskReg操作示意图](./figures/reg_load_store_mask.png)

## 场景详细说明

两个场景均在一个AIV核上运行。输入数据先由MTE2从GM搬运至UB，入口函数通过`asc_vf_call`调用SIMD VF函数加载或生成MaskReg并完成寄存器操作，最后由MTE3将输出从UB搬运至GM；内核末尾通过`asc_sync_pipe(PIPE_ALL)`完成所有流水的收尾同步。

| 场景 | 数据类型和长度 | 说明 |
| --- | --- | --- |
| 1 | `uint8_t[1024]` | 从输入的前32B加载MaskReg；掩码位置写入值2，并将MaskReg写入输出的第256B开始位置。 |
| 2 | `float[256]`和`uint8_t[128]`掩码 | 每次处理64个`float`元素，按MaskReg位从两个输入中选择元素并写出。 |

## 编译与运行

### 配置环境变量

请根据当前环境中CANN开发套件包的安装方式配置环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

> **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

### 运行样例

在样例根目录执行以下命令。`SCENARIO_NUM`可取1或2，默认值为1。

```bash
SCENARIO_NUM=1                                                                    # 选择场景1
mkdir -p build && cd build;                                                        # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # 编译工程，默认NPU模式
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM                         # 生成测试输入数据和golden数据
./demo                                                                             # 执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin           # 校验输出数据与golden数据
```

默认使用NPU运行模式。使用NPU仿真模式时，增加`-DCMAKE_ASC_RUN_MODE=sim`：

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
```

切换场景或运行模式前，请在`build`目录删除`CMakeCache.txt`后重新执行CMake，避免复用缓存配置。

### 编译选项

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `SCENARIO_NUM` | `1`、`2` | 选择MaskReg加载、存储和选择场景。 |
| `CMAKE_ASC_RUN_MODE` | `npu`、`sim` | 选择NPU运行或NPU仿真模式。 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | 指定NPU架构，对应Ascend 950PR/Ascend 950DT。 |

## 运行结果

输出结果与golden数据一致时，校验脚本输出如下信息：

```bash
test pass!
```
