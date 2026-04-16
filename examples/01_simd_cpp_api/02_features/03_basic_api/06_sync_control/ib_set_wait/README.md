# IBSet与IBWait核间同步样例
## 概述

本样例基于IBSet和IBWait实现核间同步，适用于以下场景：当两个核协同操作同一块全局内存且存在数据依赖时，通过IBSet和IBWait实现核间同步，避免数据读写错误。

具体场景说明：
- **核间数据依赖**：核0需要将计算结果写入全局内存，核1需要读取核0的输出数据作为输入
- **写后读依赖**：核0完成写操作后，核1才能执行读操作
- **写后写依赖**：核0和核1都需要写入同一块全局内存的不同位置，需要保证写操作顺序



## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── ib_set_wait
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── ib_set_wait.asc         // Ascend C算子实现 & 调用样例
```

## 样例功能描述

本样例通过两个核协同工作，每个核处理256个half数据，共512个元素。核0负责读取输入x和y，计算结果写入z；核1负责读取输入z和y，计算结果写入x。通过IBSet和IBWait保证核间同步，避免数据读写错误。

### 数据依赖场景

当两个核协同操作同一块全局内存时，可能出现以下数据依赖问题：

1. **写后读依赖**：核0完成写操作后，核1才能执行读操作
2. **写后写依赖**：核0和核1都需要写入同一块全局内存的不同位置，需要保证写操作顺序
3. **读后写依赖**：核0完成读操作后，核1才能执行写操作

### 同步机制

通过IBSet和IBWait实现核间同步：

- **IBSet**：设置当前核的同步标志位，通知其他核当前核已完成某个操作
- **IBWait**：等待指定核的同步标志位，确保该核已完成某个操作后再继续执行

本样例中：
- 核1在读取z之前，通过IBWait等待核0完成z的写操作
- 核0在完成z的写操作后，通过IBSet设置同步标志位

### 样例规格

<table>
<caption>表1：样例输入输出规格</caption>
<tr><td rowspan="1" align="center">样例类型</td><td colspan="4" align="center">核间同步</td></tr>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center"></td></tr>
<tr><td align="center">x</td><td align="center">[512]</td><td align="center">half</td><td align="center">ND</td><td align="center"></td></tr>
<tr><td align="center">y</td><td align="center">[512]</td><td align="center">half</td><td align="center">ND</td><td align="center"></td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[512]</td><td align="center">half</td><td align="center">ND</td><td align="center"></td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">kernel_ib_set_wait</td></tr>
</table>


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
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
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
test pass!
```