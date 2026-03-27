# CrossCoreSetFlag和CrossCoreWaitFlag，Cube和Vector融合计算场景样例
## 概述

本样例详细说明：在包含Cube和Vector融合计算应用场景下，如何使用 [CrossCoreSetFlag和CrossCoreWaitFlag](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/API/ascendcopapi/atlasascendc_api_07_0273.html)的模式 0、模式 2 实现多个AI Core 中所有AIC间的同步和单个 AI Core 中AIC与AIV间的同步。
<table border="1" align="center">
  <tr bgcolor="lightgray">
    <td>同步控制模式</td>
    <td align="center">说明</td>
  </tr>
  <tr>
    <td rowspan="2">mode 0</td>
    <td>对于AIC场景，同步所有的AIC核，直到所有的AIC核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td>对于AIV场景，同步所有的AIV核，直到所有的AIV核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td>mode 1</td>
    <td>单个AI Core内部，AIV核之间的同步控制。如果两个AIV核都运行了CrossCoreSetFlag，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td rowspan="2">mode 2</td>
    <td>在AIC核执行CrossCoreSetFlag之后， 两个AIV上CrossCoreWaitFlag后续的指令才会继续执行。</td>
  </tr>
  <tr>
    <td>两个AIV都执行CrossCoreSetFlag后，AIC上CrossCoreWaitFlag后续的指令才能执行。</td>
  </tr>
</table>

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── cross_core_set_flag_mix
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── cross_core_set_flag_mix.asc      // Ascend C算子实现 & 调用样例
```

## 算子描述

### 整体计算逻辑
<p align="center">
  <img src="img/融合场景_示意图.png" width="100%">
</p>

<p align="center">
图1：整体计算逻辑示意图
</p>

本样例针对融合算子（配置__mix__(1,2)）场景展开，该场景下算子计算配置为numBlocks = 8，对应 8 个 AIC 与 16 个 AIV。如图1所示，整体计算逻辑分为四个核心阶段：

（1）精度转换阶段（模式 2，AIC等AIV）：每个AI Core内部，AIC 需等待该AI Core内部其它2 个 AIV 完成数据精度转换后，才能开始分块矩阵乘计算。具体为：将 GM 中左矩阵数据沿 K 轴切分 8 份，分配至 BlockIdx 为偶数的 AIV 中完成 U8 到 F16 的精度转换；将 GM 中右矩阵数据沿 K 轴切分 8 份，分配至 BlockIdx 为奇数的 AIV 中完成 U8 到 F16 的精度转换（转换后数据满足 mmad 指令对输入数据类型的要求）。

        if (blockIdx % 2 == 0) {
            AscendC::DataCopy(aLocal, aGM, A_BLOCKS_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::Cast(castALocal, aLocal, AscendC::RoundMode::CAST_NONE, A_BLOCKS_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::DataCopy(AVectorGM, castALocal, A_BLOCKS_LENGTH);
        } else {
            AscendC::DataCopy(bLocal, bGM, B_BLOCKS_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::Cast(castBLocal, bLocal, AscendC::RoundMode::CAST_NONE, B_BLOCKS_LENGTH);
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::DataCopy(BVectorGM, castBLocal, B_BLOCKS_LENGTH);
        }

        // 模式2,每一个AICore内部，AIC等2个AIV
        AscendC::CrossCoreSetFlag<2, PIPE_MTE3>(SYNC_AIV_AIC_FLAG);

（2）分块矩阵乘与原子累加阶段（模式 0，AIC全核同步）：每个 AIC 执行分块矩阵乘计算后，将计算结果搬运至同一块 GM 区域，并开启原子累加求和机制，对基于分块矩阵乘的计算结果进行累加，最终得到完整的 MxN 维度矩阵相乘结果。

        // 模式2,每一个AICore内部，AIC等2个AIV
        AscendC::CrossCoreWaitFlag(SYNC_AIV_AIC_FLAG);

        CopyIn(a1Local, b1Local);
        SplitA(a1Local, a2Local);
        SplitBTranspose(b1Local, b2Local);
        Compute(a2Local, b2Local, c1Local);
        CopyOut(c1Local);
        // 模式0,8个AICore 包含的8个AIC同步
        AscendC::CrossCoreSetFlag<0, PIPE_FIX>(SYNC_AIC_FLAG);  
        AscendC::CrossCoreWaitFlag(SYNC_AIC_FLAG);  

        // 模式2,每一个AICore内部，2个AIV等AIC
        AscendC::CrossCoreSetFlag<2, PIPE_FIX>(SYNC_AIC_AIV_FLAG);  
（3）LeakRelu 运算阶段（模式 2，AIC等AIV）：每个 AI Core 内部，2 个 AIV 需等待 AIC 完成分块矩阵乘及累加操作后，再对完整矩阵相乘结果沿 M 轴切分 16 份，分配至 16 个 AIV 中分别执行 LeakRelu 运算。

        // 模式2,每一个AICore内部，2个AIV等AIC
        AscendC::CrossCoreWaitFlag(SYNC_AIC_AIV_FLAG);

        // 进行leakrelu运算
        float alpha = 0.001;
        AscendC::DataCopy(cLocal, CVectorGM, C_AIV_BLOCKS_LENGTH); 
        AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
        AscendC::LeakyRelu(reluCLocal, cLocal, alpha, C_AIV_BLOCKS_LENGTH);
（4）结果回写阶段：16 个 AIV 将各自完成 LeakRelu 运算的结果，依次写回 GM 中对应的存储位置，完成整个融合算子的计算流程。

        AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
        AscendC::DataCopy(CVectorGM, reluCLocal, C_AIV_BLOCKS_LENGTH);
### 注意事项
（1）融合算子中需要通过ASCEND_IS_AIV/ ASCEND_IS_AIC进行AIV和AIC核代码的隔离。

    KernelMmad op;
    if ASCEND_IS_AIC {
        op.InitAIC(A, B, c);
        op.ProcessAIC();
    } 
    if ASCEND_IS_AIV {
        op.InitAIV(a, b, A, B, c);
        op.ProcessAIV();
    }
    
    
（2）GetBlockIdx在AIC和AIV的取值范围不同，其取值与算子设置的逻辑核数和一个AI Core中的AIC与AIV的比例有关。本样例中设置NUM_BLOCKS=8、AIC与AIV的比例为1:2（配置__mix__(1,2)），因此GetBlockIdx在AIC和AIV的取值范围分别为0-7和0-15。

 (3) 样例采用[静态tensor编程范式](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/opdevg/Ascendcopdevg/atlas_ascendc_10_00019.html)需要手动插入核内同步。

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
  M=32 K=32 N=64
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -m=$M -k=$K -n=$N   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```