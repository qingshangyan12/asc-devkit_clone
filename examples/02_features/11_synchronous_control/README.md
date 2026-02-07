
# 同步控制类api样例介绍

## 概述

本路径下包含了与同步控制相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。

## 算子开发样例

| 目录名称                                                                   |  功能描述                                           |
|------------------------------------------------------------------------| ------------------------------------------------- |
| [cross_core_set_flag_mode0_aic](./cross_core_set_flag_mode0_aic)       | 本样例基于CrossCoreSetFlag和CrossCoreWaitFlag实现核间同步，适用于以下场景：当多个AI Core中的所有AIC需要进行核间同步，来保证所有AIC都做完某项流水任务后，AIC再往下继续执行指令。|
| [cross_core_set_flag_mode0_aiv](./cross_core_set_flag_mode0_aiv)       | 本样例基于CrossCoreSetFlag和CrossCoreWaitFlag实现核间同步，适用于以下场景：当多个AI Core中的所有AIV需要进行核间同步，来保证所有AIV都做完某项流水任务后，AIV再往下继续执行指令。|
| [cross_core_set_flag_mode1](./cross_core_set_flag_mode1)               | 本样例基于CrossCoreSetFlag和CrossCoreWaitFlag实现核间同步，适用于以下场景：当单个AI Core中的所有AIV需要进行核间同步，来保证该AI Core中的所有AIV都做完某项流水任务后，AIV再往下继续执行指令。|
| [cross_core_set_flag_mode2_aic](./cross_core_set_flag_mode2_aic)       | 本样例基于CrossCoreSetFlag和CrossCoreWaitFlag实现核间同步，适用于以下场景：当单个AI Core中的AIC与AIV(AIC Set, AIV Wait)需要进行核间同步，来保证AIC做完某项流水任务后，AIV再往下继续执行指令。|
| [cross_core_set_flag_mode2_aiv](./cross_core_set_flag_mode2_aiv)       | 本样例基于CrossCoreSetFlag和CrossCoreWaitFlag实现核间同步，适用于以下场景：当单个AI Core中的AIV与AIC(AIV Set, AIC Wait)需要进行核间同步，来保证单个核内所有AIV都做完某项流水任务后，AIC再往下继续执行指令。|
| [ib_set](./ib_set)                                                     | 本样例基于IBSet实现核间同步，适用于以下场景：当不同核之间操作同一块全局内存且可能存在读后写、写后读以及写后写等数据依赖问题时，通过调用该函数来插入同步语句来避免上述数据依赖时可能出现的数据读写错误问题。 |
| [init_determine_compute_workspace](./init_determine_compute_workspace) | 本样例模拟8个核进行数据处理，使用确定性计算接口保证核间运行顺序，进行原子累加。 |
| [mutex](./mutex)                                                       | 本样例基于Mutex::Lock和Mutex::Unlock实现核内异步流水之间的同步，通过锁定指定流水再释放流水来实现流水的同步依赖。|
| [set_next_task_start](./set_next_task_start)                           | 本样例基于SetNextTaskStart接口实现Superkernel的子kernel并行。 |
| [sync_all](./sync_all)                                                 | 本样例基于SyncAll实现核间同步，适用于以下场景：不同核之间操作同一块全局内存且可能存在读后写、写后读以及写后写等数据依赖问题，通过调用本接口来插入同步语句来避免上述数据依赖时可能出现的数据读写错误问题。|
| [wait_pre_task_end](./wait_pre_task_end)                                                 | 本样例基于SetPreTaskEnd接口实现Superkernel的子kernel并行。|