# DataSyncBarrier样例

## 概述

本样例演示DataSyncBarrier接口的使用场景：确保核内数据写入GM全局可见后再执行后续读取操作。

DataSyncBarrier阻塞后续指令执行，直到所有之前的内存访问指令执行结束。通过参数MemDsbT可控制等待的内存位置：ALL（DDR+UB）、DDR（仅GM）、UB（仅UB）。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── data_sync_barrier
│   ├── scripts
│   │   ├── gen_data.py           // 输入数据和真值数据生成脚本
│   │   └── verify_result.py      // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt            // 编译工程文件
│   ├── data_utils.h              // 数据读入写出函数
│   └── data_sync_barrier.asc     // Ascend C样例实现 & 调用样例
```

## 样例规格

<table>
<thead>
<tr><th>参数</th><th>规格</th></tr>
</thead>
<tbody>
<tr><td>输入shape</td><td>[32]</td></tr>
<tr><td>输入数据类型</td><td>float</td></tr>
<tr><td>输出shape</td><td>[32]</td></tr>
<tr><td>输出数据类型</td><td>float</td></tr>
<tr><td>核数</td><td>1</td></tr>
<tr><td>数据排布格式</td><td>ND</td></tr>
</tbody>
</table>

## 核心逻辑

本样例演示单核内DataSyncBarrier确保数据写入全局可见的场景：

1. **写入数据**：通过DataCopy将输入数据写入临时GM区域（tmpGlobal）
2. **同步等待**：调用DataSyncBarrier阻塞后续指令，确保写入完成
3. **读取计算**：读取写入的数据与原始输入执行加法，验证数据已全局可见

关键代码片段：

```cpp
__aicore__ inline void Process()
{
    WriteData();
    AscendC::DataSyncBarrier<AscendC::MemDsbT::DDR>();
    ReadAndCompute();
}
```

MemDsbT::DDR表示仅等待GM（DDR）访问完成，可选值：ALL（DDR+UB）、DDR、UB。

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
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-2201| dav-2201 |

执行结果如下，说明样例运行成功。
```bash
test pass
```
