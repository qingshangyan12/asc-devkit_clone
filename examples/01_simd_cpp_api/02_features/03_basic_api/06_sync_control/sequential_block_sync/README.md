# 核间顺序同步样例

## 概述

本样例在确定性计算场景下，基于InitDetermineComputeWorkspace、WaitPreBlock和NotifyNextBlock三个接口实现核间顺序同步。**这三个接口必须组合使用**，能够确保多个AIV核严格按照blockIdx的升序顺序执行，适用于要求确定性计算的场景。本样例模拟8个核进行数据处理，使用确定性计算接口保证核间运行顺序，进行原子累加，确保计算结果的确定性。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── sequential_block_sync
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── sequential_block_sync.asc      // Ascend C样例实现 & 调用样例
```

## 样例功能描述

本样例通过8个核协同工作，每个核有独立的256个float输入数据段（共2048个元素）。使用InitDetermineComputeWorkspace初始化GM共享内存的同步状态，然后通过WaitPreBlock和NotifyNextBlock确保核间按照blockIdx升序顺序执行。每个核将自己的输入数据分两次tile（每次128个元素）通过原子累加写入输出缓冲区，保证计算结果的确定性。

### 样例规格

<table>
<caption>表1：样例输入输出规格</caption>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[2048]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td align="center">workspace</td><td align="center">[64]</td><td align="center">int32_t</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[256]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">kernel_sequential_block_sync</td></tr>
<tr><td rowspan="1" align="center">核数量</td><td colspan="5" align="center">8</td></tr>
</table>

> workspace大小计算：blockNum * 32Bytes = 8 * 32 = 256Bytes，转换为int32_t元素个数为 256/4 = 64

### 计算流程

1. **初始化阶段**：调用InitDetermineComputeWorkspace初始化GM共享内存的同步状态
2. **数据搬入**：每个核从GM读取自己对应的256个元素（核0读取src[0:256]，核1读取src[256:512]，...），分两次tile（每次128个元素）搬入256个元素到UB
3. **核间同步**：通过WaitPreBlock等待前序核完成，确保核0→核1→核2→...顺序执行
4. **原子累加**：开启SetAtomicAdd，将本核数据原子累加写入GM的dst区域
5. **通知后序核**：通过NotifyNextBlock通知后序核可以开始执行


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

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
mkdir -p build && cd build;      # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;             # 编译工程，默认npu模式
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo                           # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确
```

使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

示例如：
```bash
cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

### 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |

执行结果如下，说明精度对比成功。
```bash
test pass
```