# Ascend C API性能验证SKILL

## 核心理念

本 SKILL 的核心是基于用户指定的API信息，完成自动化代码适配、结果计算统计和展示。

## 工作流程

### 1. 自动化代码适配

#### 1.1. 解析输入（`user_input.md`）

- 阅读用户输入的API功能信息（包含API名称、API类型如单目/双目/双目标量等、功能说明、函数原型）理解这个API的功能和用法，用于指导后续的Kernel算子代码实现。

- 阅读用户输入的性能验证场景，包含要验证的数据类型（half/float/int16/int32）、数据长度（4/8/16等），用于构造真值、测试组合输入、Kernel算子代码适配。

    注：用户提供的输入要求是字典类型，比如{数据类型1: [数据长度1, 数据长度2, ..], ..}；具体示例  
    ```
    {half: [4, 16], float: [2, 8]}
    ```
    代表要验证四类场景下的性能：  
    - 场景一、half数据类型输入场景下，4K数据长度（即4*1024个elements）；  
    - 场景二、half数据类型输入场景下，16K数据长度（即16*1024个elements）；  
    - 场景三、float数据类型输入场景下，2K数据长度（即2*1024个elements）；  
    - 场景四、float数据类型输入场景下，8K数据长度（即8*1024个elements）；  

- 如果用户没提供完整上述信息，告警提示用户。

#### 1.2. 代码适配

- Kernel算子实现代码 `performance_verify.asc`
    - 新增API ID定义
    - 修改CopyIn函数，增加新增API的数据搬入处理
    - 修改Compute函数，增加新增API的计算处理

- 真值构造脚本
    - 创建`scripts/gen_data_{api_name}.py`，参考现有的代码实现
        ```python
        ...

        def compute_{api_name}_golden():
            ...

        def gen_{api_name}_data():
            ...
        ```
    - 完成`scripts/gen_data.py`的适配
        - 添加导入
        - 添加API ID和名称映射
        - 修改argparse的choices
        - 添加分支处理逻辑
    - 完成`scripts/verify_result.py`的适配
        - 添加API ID和名称映射
        - 添加分支处理逻辑
        - 修改argparse的choices

- 执行脚本`test.sh`  
    - 按照不同API、输入数据类型、输入数据长度的组合，依次执行，得到对应测试场景下的性能结果即total cycles的数值，并将结果保存到total_results。

### 2. 结果计算与统计
- 完成每个API的每个场景下性能测试结果换算
    - 计算公式：
        接口执行N次总耗时（50MHz）：T_total = systemCycleAfter - systemCycleBefore
        接口平均单次执行耗时（50MHz）：T_avg = T_total / N
        换算到AI处理器频率的等效单次执行耗时（1800MHz）：T_ai = T_avg / 50 * 1800
        换算到AI处理器频率的等效并行度：Throughput = 数据长度 / T_ai
        
    - 以Exp接口、输入数据类型是half，输入数据长度是16(K)为例，假设执行日志结果total cycles: 15112。
        - 接口执行1000次总耗时（50MHz）：T_total = 15112(cycles)
        - 接口平均单次执行耗时（50MHz）：T_avg = 15112 / 1000 = 15.112(cycles)
        - 换算到AI处理器频率的等效单次执行耗时（1800MHz）：T_ai = 15.112 / 50 * 1800 = 544.032(cycles)
        - 换算到AI处理器频率的等效并行度：Throughput = (16 * 1024) / 544.032 = 30.12(element/cycle)
- 将所有API的所有组合场景性能测试结果进行汇总
    上述`test.sh`脚本获取的total_results结果，最终拆解成各个API在对应场景下的性能
    | API接口名称 | 数据类型 | 数据长度（元素个数，单位K）| 接口执行1000次耗时(50MHz) | 接口单次执行耗时(50MHz) | 换算到AI处理器频率(1800MHz)的等效并行度 |
    | --- | --- | --- | --- | --- | --- |
    | Exp | half | 16 | 15112 | 15.112 | 30.12 |

### 3. 结果展示

- 将第2步的结果统计汇总，进行打印显示。

    结果展示模板参考如下
    | API接口名称 | 数据类型 | 数据长度（元素个数，单位K）| 理论并行度 | 实测并行度 | 达标率 | 接口执行1000次耗时(50MHz) | 接口单次执行耗时(50MHz) | 换算到AI处理器频率(1800MHz)的等效并行度 |
    | --- | --- | --- | --- | --- | --- | --- | --- | --- |
    | Exp | half | 16 | 32 | 30.12 | 94.12% | 15111 | 15.111 | 30.12 |

- 每个API的理论并行度，从`theoretical_performance.md`中获取，如果没有找到，告警提示用户提供。
