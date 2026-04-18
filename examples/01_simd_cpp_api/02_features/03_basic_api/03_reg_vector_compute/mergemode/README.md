# mergemode样例

## 概述
本样例基于RegBase编程范式，综合演示以下特性：
- **MaskMergeMode::MERGING**：Max接口的merging模式，未激活mask位保留dst原值
- **LoadAlign/StoreAlign postUpdate模式**：地址自动递增
- **UpdateMask**：数据非VL整数倍时动态生成mask
- **ReduceMax**：使用Reduce<MAX>归约求全局最大值

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── mergemode
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── mergemode.asc                  // AscendC样例实现 & 调用样例
│   └── README.md                      // 样例介绍
```

## 样例描述
- 样例功能：  
  对一个向量先做逐元素Max(x, x)（演示MaskMergeMode::MERGING），再做ReduceMax归约，得到全局最大值。向量shape为[1, 200]，数据类型为float。单核场景。

  **单核模式**
  - 一个核处理全部200个数据（200非VL整数倍，VL=64 for float）
  - 样例规格：
    <table>
    <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
    <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
    <tr><td align="center">x</td><td align="center">[1, 200]</td><td align="center">float</td></tr>
    <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[1, 8]</td><td align="center">float</td></tr>
    <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mergemode</td></tr>
    </table>
  - 样例实现：
    1. **MaxMergeModeVF**：使用`Max<T, MaskMergeMode::MERGING>`进行逐元素最大值计算，未激活mask位保留dst原值；使用`LoadAlign/StoreAlign`的`PostLiteral::POST_MODE_UPDATE`模式自动递增地址；使用`UpdateMask`处理非VL整数倍的数据尾部。
    2. **ReduceMaxVF**：使用`Reduce<ReduceType::MAX>`对每个VL块做块内归约，再用`Max`累加得到全局最大值。
    3. 输出为32B对齐的8个float，全局最大值在第一个元素。
    - 调用实现
      使用内核调用符`<<<>>>`调用核函数，启动1个核。

## 关键API说明

| API | 说明 |
|-----|------|
| `AscendC::Reg::Max<T, MaskMergeMode::MERGING>` | 逐元素最大值，MERGING模式：未激活mask位保留dst原值 |
| `AscendC::Reg::MaskMergeMode::ZEROING` | ZEROING模式：未激活mask位置零 |
| `AscendC::Reg::LoadAlign<T, PostLiteral::POST_MODE_UPDATE>` | postUpdate模式：加载后地址自动递增 |
| `AscendC::Reg::StoreAlign<T, PostLiteral::POST_MODE_UPDATE>` | postUpdate模式：存储后地址自动递增 |
| `AscendC::Reg::UpdateMask<T>(count)` | 根据剩余元素数动态生成mask（非VL整数倍时使用） |
| `AscendC::Reg::Reduce<ReduceType::MAX>` | 块内归约取最大值，结果放在dstReg最低位 |
| `AscendC::VF_CALL<Func>(args...)` | VF函数调用入口 |

## 编译运行
在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
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

- 样例执行
  ```bash
  mkdir -p build && cd build;                                                    # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                           # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                                 # 生成测试输入数据
  ./demo                                                                         # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
