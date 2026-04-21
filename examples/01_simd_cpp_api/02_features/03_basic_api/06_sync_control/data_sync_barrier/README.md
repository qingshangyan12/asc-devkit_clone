# DataSyncBarrier样例

## 概述

本样例在双核同步场景下，基于DataSyncBarrier实现核内标量流水（PIPE_S）同步。样例展示了两个AIV核通过GM共享变量进行同步的场景：核0通过WriteGmByPassDCache接口依次向GM写入两个变量，使用DataSyncBarrier确保写操作按顺序执行；核1通过轮询机制检测核0的写入完成信号后读取数据并计算输出结果。

DataSyncBarrier接口功能：阻塞后续指令执行，直到之前所有内存访问指令（需要等待的内存位置可通过参数控制）执行结束，适用于核内标量单元流水（PIPE_S）的同步。与PipeBarrier不同，调用PipeBarrier<PIPE_S>()会引发硬件错误，因此标量流水同步必须使用DataSyncBarrier。

## 支持的产品

- Ascend 950PR/Ascend 950DT
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
<tr><td>输入</td><td>srcGm</td><td>[8]</td><td>int32_t</td><td>ND</td></tr>
<tr><td>输出</td><td>dstGm</td><td>[8]</td><td>int32_t</td><td>ND</td></tr>
<tr><td>核函数名</td><td>kernel_data_sync_barrier</td><td>-</td><td>-</td><td>-</td></tr>
</table>

## DataSyncBarrier使用场景说明

核内单流水同步指令包含PipeBarrier和DataSyncBarrier，各自适用场景不同：

- **PipeBarrier**：用于同一条流水内部前后指令的先后依赖，但调用`PipeBarrier<PIPE_S>()`会引发硬件错误。
- **DataSyncBarrier**：用于标量流水（PIPE_S）的同步，阻塞后续指令执行直到之前所有内存访问指令完成。

本样例展示了DataSyncBarrier在双核同步场景中的使用：

**场景描述**：系统中有两个AIV核（核0与核1），变量x、y初始值均为1。核0通过WriteGmByPassDCache接口向GM依次写入x=7、y=6；核1持续轮询变量y，直到检测到y=6后，从GM读取变量x并打印。

**预期行为**：当y=6写入GM时，x=7必须已完成写入GM，因此核1最终打印的x值应为7。

**问题分析**：由于标量流水不能保证指令间的写序一致性，若不加同步，可能出现y已更新而x尚未写入GM的情况，导致核1读到错误的x值。此时需要插入同步指令DataSyncBarrier，以确保两次写操作顺序执行。

**核0代码流程**：
```cpp
if (blockIdx == 0) {
    AscendC::WriteGmByPassDCache<T>(srcGm+1, value+1);    // 先写入 GM[1] = 7
    AscendC::DataSyncBarrier<AscendC::MemDsbT::DDR>();    // 同步，确保写入完成
    AscendC::WriteGmByPassDCache<T>(srcGm, value);        // 后写入 GM[0] = 6（信号）
}
```

**核1代码流程**：
```cpp
if (blockIdx == 1) {
    while(true) {
        T value = AscendC::ReadGmByPassDCache<T>(srcGm);  // 轮询 GM[0]
        if(value == 6) {                                  // 检测到信号
            T valueNew = AscendC::ReadGmByPassDCache<T>(srcGm+1);  // 读取 GM[1] = 7
            AscendC::WriteGmByPassDCache<T>(dstGm, 2 * valueNew);  // 输出 14
            return;
        }
    }
}
```

**DataSyncBarrier参数说明**：
- **MemDsbT::ALL**：等待所有存储单元（DDR+UB）的内存访问完成
- **MemDsbT::DDR**：等待GM（DDR）存储单元的内存访问完成
- **MemDsbT::UB**：等待UB存储单元的内存访问完成

本样例使用MemDsbT::DDR，因为WriteGmByPassDCache是GM（DDR）存储单元的写操作。

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