#  AIC核内全量同步样例

## 概述

Atlas A2/A3系列产品的硬件架构如下图1所示。由于AIC核内部的执行单元采用异步并行的运行方式，在读写同一块存储资源时可能产生数据依赖关系（读后写、写后读、写后写）。为确保数据一致性与计算正确性，需通过核内同步指令来协调不同执行单元的时序。

本样例将介绍所有AIC核内同步使用的场景，学习本样例之前建议开发者先学习核内同步指令[SetFlag/WaitFlag](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/API/ascendcopapi/atlasascendc_api_07_0270.html)和[PipeBarrier](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/API/ascendcopapi/atlasascendc_api_07_0271.html)的使用。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── fixpipe_co12c1_quantization_f322s8
│   ├── scripts
│   │   ├── gen_data.py                                   // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                              // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                                    // 编译工程文件
│   ├── data_utils.h                                      // 数据读入写出函数
│   └── fixpipe_co12c1_quantization_f322s8.asc            // Ascend C算子实现 & 调用样例
```

## 算子描述

开启自动同步日志：https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/opdevg/BishengCompiler/atlas_bisheng_10_0008.html

leakrelu软仿：
    __aicore__ inline void Compute(int32_t progress)
    {
        // deque input tensors from VECIN queue
        AscendC::LocalTensor<float> xLocal = inQueueX.DeQue<float>();
        AscendC::LocalTensor<float> yLocal = outQueueY.AllocTensor<float>();
        AscendC::LocalTensor<float> tmpTensor1 = tmpBuffer1.Get<float>();
        AscendC::LocalTensor<float> tmpTensor2 = tmpBuffer2.Get<float>();
        float inputVal = 0.0f;
        AscendC::Maxs(tmpTensor1, xLocal, inputVal, this->tileLength);
        AscendC::Mins(tmpTensor2, xLocal, inputVal, this->tileLength);
        AscendC::Muls(tmpTensor2, tmpTensor2, this->negativeSlope, this->tileLength);
        AscendC::Add(yLocal, tmpTensor1, tmpTensor2, this->tileLength);
        // enque the output tensor to VECOUT queue
        outQueueY.EnQue<float>(yLocal);
        // free input tensors for reuse
        inQueueX.FreeTensor(xLocal);
    }

### AIV内硬件流水类型
<table border="1" cellpadding="8" cellspacing="0" style="width: 100%; border-collapse: collapse; font-family: monospace, sans-serif;">
  <thead>
    <tr style="background-color: #f2f2f2;">
      <th style="width: 20%; text-align: center;">流水类型</th>
      <th style="width: 80%; text-align: left;">含义</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_S</td>
      <td>标量流水线，使用Tensor GetValue函数时为此流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_V</td>
      <td>矢量计算流水及部分硬件架构下的L0C Buffer->UB数据搬运流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_M</td>
      <td>矩阵计算流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_MTE1</td>
      <td>L1 Buffer ->L0A Buffer、L1 Buffer->L0B Buffer数据搬运流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_MTE2</td>
      <td>GM->L1 Buffer、GM->UB等数据搬运流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_MTE3</td>
      <td>UB->GM等数据搬运流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_FIX</td>
      <td>L0C Buffer->GM、L0C Buffer ->L1等数据搬运流水</td>
    </tr>
  </tbody>
</table>
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
  enloaddatagm2l0=0
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake .. -DEN_LOADDATA_GM2L0=$enloaddatagm2l0;make -j;             # 编译工程
  python3 ../scripts/gen_data.py -en_loaddata_gm2l0=$enloaddatagm2l0   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```