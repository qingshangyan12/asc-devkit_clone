# Exp样例
## 概述
本样例基于Exp的tensor前n个数据计算接口，实现将输入的tensor按元素取自然对数。样例侧重Exp接口的性能验证，提供了两种性能验证方式：代码打点和msProf工具性能采集，详见[API性能分析](#api性能分析)。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── exp
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── exp.asc                 // Ascend C算子实现 & 调用样例
```

## 算子描述
- 算子功能：  
  ExpCustom算子，将输入的tensor按元素取自然对数。
- 算子规格：  
  算子输入tensor的data type类型和shape大小，支持用户进行配置。在程序中分别以INPUT_TYPE和INPUT_SIZE表示。  
  - INPUT_TYPE与算子输入tensor的data type类型对应关系，如下表所示
    <table>
    <td rowspan="1" align="center">配置参数名</td>
    <td colspan="1" align="center">取值</td><td colspan="1" align="center">对应的数据类型</td></tr></tr>
    <tr><td rowspan="3" align="center">INPUT_TYPE</td></tr>
    <tr><td align="center">1</td><td align="center">half</td></tr>
    <tr><td align="center">2</td><td align="center">float</td></tr>
    </table>
  - INPUT_SIZE的大小即为算子的shape大小，单位是元素个数。
  - 算子整体规格如下表所示
    <table>
    <td rowspan="1" align="center">算子类型(OpType)</td>
    <td colspan="4" align="center">Exp</td></tr>
    <tr><td rowspan="3" align="center">算子输入</td></tr>
    <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
    <tr><td align="center">x</td><td align="center">INPUT_SIZE</td><td align="center">half或float</td><td align="center">ND</td></tr>
    <tr><td rowspan="2" align="center">算子输出</td></tr>
    <tr><td align="center">y</td><td align="center">INPUT_SIZE</td><td align="center">half或float</td><td align="center">ND</td></tr>
    <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">exp_custom</td></tr>
    </table>

- 算子实现：  
  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬入片上存储，然后使用Exp基础API接口完成计算，得到最终结果，再搬出到外部存储上。

    ExpCustom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm搬入至srcLocal中。Compute任务负责将srcLocal按元素取自然对数，并将结果存储在dstLocal中。CopyOut任务负责将计算结果数据从dstLocal搬运至Global Memory上的输出Tensor dstGm。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。


## 编译运行  
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
    
- 样例执行
  ```bash
  INPUT_TYPE=1 INPUT_SIZE=32768 # 输入Tensor的数据类型是half，数据大小是32K(32*1024=32768)
  mkdir -p build && cd build; # 创建并进入build目录
  cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j; # 编译工程
  python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE # 生成测试输入数据
  ./demo # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## API性能分析
### 理论性能分析  
Exp接口时在built-in接口（vexp）的基础上进行抽象封装实现的。vexp指令的理论并行度参照下表：
  <table>
      <thead>
          <tr>
              <th>数据类型</th>
              <th>理论并行度（element / cycle）</th>
          </tr>
      </thead>
      <tbody>
          <tr>
              <td>half (fp16)</td>
              <td>32</td>
          </tr>
          <tr>
              <td>float (fp32)</td>
              <td>32</td>
          </tr>
      </tbody>
  </table>
  假设输入tensor的长度为32 * 1024，数据类型为half。对照上表，vexp指令在该场景下的理论并行度为32。则单次Exp接口所需执行时间（cycles）= 数据长度 / 理论并行度 = (32 * 1024) / 32 = 1024。该数值为理想并行度条件下，Exp接口的理论性能。

### 实测性能验证 
具体性能测试请参考下述提供的两种测试方法：[代码打点](#测试方式一代码打点)和[msProf工具性能采集](#测试方式二msprof工具性能采集)。  
下表是基于样例的实测性能数据与理论性能数据的对比：

<table>
    <thead>
        <tr>
            <th>数据类型</th>
            <th>数据长度（元素个数）</th>
            <th>理论并行度</th>
            <th>实测并行度</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>half (fp16)</td>
            <td>32 * 1024</td>
            <td>32</td>
            <td>31.03</td>
        </tr>
        <tr>
            <td>float (fp32)</td>
            <td>16 * 1024</td>
            <td>32</td>
            <td>30.12</td>
        </tr>
    </tbody>
</table>
由上述结果可见，实测性能达到理论性能的94%以上。跟理论性能的差异，主要在于接口调用还存在指令下发的耗时。

#### 测试方式一：代码打点
- 测试输入配置  
  - 输入数据长度：32 * 1024
  - 数据类型：half
  - 指令重复执行次数：1000
  - 实测硬件平台：AI处理器910B3（频率：1800MHz）

- 代码实现
  ```cpp
  constexpr uint32_t INPUT_TENSOR_SIZE = INPUT_SIZE; // 输入数据长度
  constexpr uint32_t INPUT_TENSOR_TYPE = INPUT_TYPE; // 输入数据类型
  constexpr uint32_t REPEAT_TIMES_FOR_INSTR_EXCUTE = 1000; // 指令重复执行次数（用于性能测试）

  __aicore__ inline void Compute()
  {
      ...

      AscendC::PipeBarrier<PIPE_ALL>();
      uint64_t systemCycleBefore = AscendC::GetSystemCycle();
      for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXCUTE; ++i) {
          AscendC::Exp(dstLocal, srcLocal, INPUT_SIZE_PARAM);
      }
      AscendC::PipeBarrier<PIPE_ALL>();
      uint64_t systemCycleAfter = AscendC::GetSystemCycle();
      AscendC::PRINTF("[LOG] systemCycleBefore: %d\n", systemCycleBefore);
      AscendC::PRINTF("[LOG] systemCycleAfter: %d\n", systemCycleAfter);
      AscendC::PRINTF("[LOG] Exp instruction has excuted %d times, cost total cycles: %d\n",
          REPEAT_TIMES_FOR_INSTR_EXCUTE, (systemCycleAfter - systemCycleBefore));

      ...
  }
  ```
  通过在Exp接口调用前后进行打点，获取Exp接口执行耗时。通过重复执行1000次，减少单次执行的统计偏差。在调用`GetSystemCycle`接口之前，插入了`PIPE_ALL`同步，可以保证相关指令执行完后再获取准确的cycle数。  
  注：`GetSystemCycle`获取当前系统cycle数，若换算成时间需要按照50MHz进行换算，时间单位为us，换算公式为：time = (cycle数 / 50) us。

- 执行命令
  ```bash
  # 验证half类型输入，数据长度为32K
  INPUT_TYPE=1 INPUT_SIZE=32768
  mkdir -p build && cd build;
  cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j;
  python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE
  ./demo
  ```

- 日志结果
  ```
  [LOG] systemCycleBefore: 14106388455589
  [LOG] systemCycleAfter: 14106388484922
  [LOG] Exp instruction has excuted 1000 times, cost total cycles: 29333
  ```

- 实测性能计算说明
  - Exp接口执行1000次耗时（50MHz）：T1 = systemCycleAfter - systemCycleBefore = 29333(cycles)。
  - 接口平均单次执行耗时（50MHz）：T2 = T1 / 1000 = 29.33(cycles)。
  - 换算到AI处理器频率的等效单次执行耗时（1800MHz）：T3 = T2 / 50 * 1800 = 1055.88(cycles)。
  - 换算到AI处理器频率的等效并行度：T4 = 数据长度(element) / 执行耗时(cycle) = (32 * 1024) / T3 = 32768 / 1055.88 = 31.03，与理论并行度32基本接近。

#### 测试方式二：msProf工具性能采集  
msProf工具支持上板调优，可以基于编译生成的可执行文件，直接在NPU硬件上采集算子性能数据，数据为算子预热后的真实运行指标。

- 测试输入配置
  - 输入数据长度：32 * 1024
  - 数据类型：half
  - 指令重复执行次数：1000
  - 实测硬件平台：AI处理器910B3（频率：1800MHz） 

- 代码实现  
  同测试方法一

- 执行命令
  ```bash
  # 验证half类型输入，数据长度为32K
  INPUT_TYPE=1 INPUT_SIZE=32768
  mkdir -p build && cd build;
  cmake .. -DINPUT_TYPE=$INPUT_TYPE -DINPUT_SIZE=$INPUT_SIZE;make -j;
  python3 ../scripts/gen_data.py -inputType=$INPUT_TYPE -inputSize=$INPUT_SIZE
  # msprof op采集性能数据
  msprof op ./demo
  ```

- 查看性能数据结果  
  执行上述的命令后，在当前目录下会生成OPPPROF_为前缀的文件夹，目录结构及文件说明如下：
  ```bash 
  OPPROF_202xxxxx_XXXXXX
  ├── dump                             # 原始性能数据（无需关注）
  ├── OpBasicInfo.csv                  # 算子基础数据
  ├── PipeUtilization.csv              # 采集计算单元和搬运单元耗时和占比
  ├── ArithmeticUtilization.csv        # cube及vector类型指令的cycle占比数据
  ├── ResourceConflictRatio.csv        # 资源冲突占比数据
  ├── ... (开启的aic-metrics)
  └── visualize_data.bin              # 算子可视化文件（可通过MindStudio Insight加载，直观查看算子性能）
  ```
  可直接查看PipeUtilization.csv文件，获取vec类型指令（向量类运算指令）耗时性能数据，对应字段aiv_vec_time。
  <table>
  <td rowspan="1" align="center">aiv_vec_time</td>
  <tr><td rowspan="1" align="center">584.460022</td></tr>
  </table>

- 实测性能计算说明
  - 单次指令平均耗时（单位：us）：T1 = aiv_vec_time / 1000 = 584.460022 / 1000 = 0.5845。
  - 单次指令平均耗时（单位：cycles）：T2 = T1 * 1800 = 1052.1。
  - 换算到AI处理器频率的等效并行度：T3 = 数据长度(element) / 执行耗时(cycle) = (32 * 1024) / T2 = 32768 / 1052.1 = 31.14，与理论并行度32基本接近。

- 补充说明  
  更多性能指标的详细说明及调优方案，请参考《算子开发工具手册》。