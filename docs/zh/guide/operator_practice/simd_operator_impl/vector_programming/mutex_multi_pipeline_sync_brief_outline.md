# 基于Mutex实现多流水同步汇报大纲

1. 背景与适用范围：说明Mutex面向Ascend 950PR/Ascend 950DT多流水算子的同步需求。
2. 双缓冲算子的多流水依赖：以MTE2、V、MTE3为例，说明数据搬入、计算、搬出的执行关系。
3. Buffer生命周期与MutexID管理：说明一个MutexID管理一个Buffer，双缓冲需要交替使用两个ID。
4. Lock/Unlock同步机制：说明Lock阻塞当前流水，等待同一MutexID此前所有流水完成Unlock。
5. 正向依赖与反向同步实现：说明同一MutexID既串联当前tile的处理阶段，也保证Buffer复用安全。
6. 与SetFlag/WaitFlag的机制差异：说明Mutex按资源状态同步，SetFlag/WaitFlag按源、目的流水和事件配对。
7. 开发复杂度与硬件架构影响：说明Mutex减少循环边界判断，并避免依赖HardEvent流水组合。
8. 使用约束与接口选择：说明Lock/Unlock配对、MutexID释放等约束，以及不同产品下的接口选择。


# 新大纲
1.引出A5新增支持mutex可以用来做核内多流水间同步。



# 基于Mutex实现多流水同步汇报大纲

1. 背景与适用范围：说明Mutex面向Ascend 950PR/Ascend 950DT多流水算子的同步需求。
2. 双缓冲算子的多流水依赖：以MTE2、V、MTE3为例，说明数据搬入、计算、搬出的执行关系。
3. Buffer生命周期与MutexID管理：说明一个MutexID管理一个Buffer，双缓冲需要交替使用两个ID。
4. Lock/Unlock同步机制：说明Lock阻塞当前流水，等待同一MutexID此前所有流水完成Unlock。
5. 正向依赖与反向同步实现：说明同一MutexID既串联当前tile的处理阶段，也保证Buffer复用安全。
6. 与SetFlag/WaitFlag的机制差异：说明Mutex按资源状态同步，SetFlag/WaitFlag按源、目的流水和事件配对。
7. 开发复杂度与硬件架构影响：说明Mutex减少循环边界判断，并避免依赖HardEvent流水组合。
8. 使用约束与接口选择：说明Lock/Unlock配对、MutexID释放等约束，以及不同产品下的接口选择。
