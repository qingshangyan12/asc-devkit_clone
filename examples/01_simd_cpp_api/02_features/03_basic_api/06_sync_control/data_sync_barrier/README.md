# DataSyncBarrier样例

## 概述

本样例在矢量计算场景下，基于DataSyncBarrier实现核内内存访问完成屏障同步，用于确保数据搬运完成后才开始计算。样例实现了两个输入Tensor的逐元素加法运算，数据量为8192，数据类型为float。

DataSyncBarrier接口功能：阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束，适用于核内内存访问完成的同步场景。与SetFlag/WaitFlag事件同步不同，DataSyncBarrier是一种内存访问完成屏障，确保内存操作真正完成后再执行后续指令。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── data_sync_barrier
│   ├── scripts
│   │   ├── gen_data.py            // 输入数据和真值数据生成脚本
│   │   └── verify_result.py       // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt             // 编译工程文件
│   ├── data_utils.h               // 数据读入写出函数
│   └── data_sync_barrier.asc      // Ascend C样例实现 & 调用样例
```

## 样例规格

<table border="2">
<caption>表1：样例规格对照表</caption>
<tr><th>类型</th><th>名称</th><th>Shape</th><th>数据类型</th><th>格式</th></tr>
<tr><td>输入</td><td>x</td><td>[8192]</td><td>float</td><td>ND</td></tr>
<tr><td>输入</td><td>y</td><td>[8192]</td><td>float</td><td>ND</td></tr>
<tr><td>输出</td><td>z</td><td>[8192]</td><td>float</td><td>ND</td></tr>
<tr><td>核函数名</td><td>kernel_data_sync_barrier</td><td>-</td><td>-</td><td>-</td></tr>
</table>

## DataSyncBarrier使用场景说明

核内同步接口包含SetFlag/WaitFlag、PipeBarrier和DataSyncBarrier，各自适用场景不同：

- **SetFlag/WaitFlag**：用于不同pipe之间的生产者/消费者依赖同步，如MTE2搬运完成后通知V开始计算。
- **PipeBarrier**：用于同一条流水内部前后指令的先后依赖。
- **DataSyncBarrier**：用于等待内存访问真正完成，确保数据已写入存储单元后再执行后续操作。注意：DataSyncBarrier并非所有芯片都支持，使用前需确认目标芯片的API支持表。

本样例展示了DataSyncBarrier在矢量计算场景中的使用：在数据搬运（DataCopy）完成后，调用DataSyncBarrier确保数据已写入UB存储单元，然后再执行加法计算（Add）。

DataSyncBarrier的参数MemDsbT有三种取值：
- **MemDsbT::ALL**：等待所有存储单元（DDR+UB）的内存访问完成
- **MemDsbT::DDR**：等待GM（DDR）存储单元的内存访问完成
- **MemDsbT::UB**：等待UB存储单元的内存访问完成

本样例使用MemDsbT::UB，因为DataCopy搬运的目标是UB存储单元。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |

### 配置环境变量

请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root用户安装CANN软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非root用户安装CANN软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径install_path，安装CANN软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

```bash
mkdir -p build && cd build;
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

使用CPU调试或NPU仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

示例如：
```bash
cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

执行结果如下，说明精度对比成功。
```bash
test pass!
```