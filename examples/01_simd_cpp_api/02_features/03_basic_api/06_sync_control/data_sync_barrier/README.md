# DataSyncBarrier样例

## 概述

本样例介绍DataSyncBarrier的调用，该接口功能：阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束。

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
<tr><td>输入</td><td>srcGm</td><td>[8]</td><td>int32_t</td><td>ND</td></tr>
<tr><td>输出</td><td>dstGm</td><td>[8]</td><td>int32_t</td><td>ND</td></tr>
<tr><td>核函数名</td><td>kernel_data_sync_barrier</td><td>-</td><td>-</td><td>-</td></tr>
</table>

## 样例说明

下面介绍本样例中DataSyncBarrier的使用场景：
系统中有两个 AIV 核（核 0 与核 1），变量 x、y 初始值均为 1。核 0 通过标量流水接口[WriteGmByPassDCache](../../16_scalar_computation/write_gm_by_pass_dcache/)向GM依次写入 x=7、y=6；核 1 持续轮询变量 y，直到检测到 y=6 后，再从 GM 中读取变量 x 并打印。

预期行为：当 y=6 写入 GM 时，x=7 必须已完成写入 GM，因此核 1 最终打印的 x 值应为 7。

但由于标量流水不能保证指令间的写序一致性，若不加同步，可能出现 y 已更新而 x 尚未写入 GM 的情况，导致核 1 读到错误的 x 值。
此时需要插入同步指令 DataSyncBarrier，以确保两次写操作顺序执行。

如下所示，核 0 通过同步指令完成对GM中张量srcGlobal第 1 个元素与第 0 个元素的顺序写入。调用DataSyncBarrier后，将阻塞后续WriteGmByPassDCache指令的执行，直至此前所有WriteGmByPassDCache类型的 GM(对应DDR) 访问指令全部完成。

        if (blockIdx == 0) {
            AscendC::WriteGmByPassDCache<T>(reinterpret_cast<__gm__ T *>(srcGm)+1, value+1);
            AscendC::DataSyncBarrier<AscendC::MemDsbT::DDR>(); // 三种取值，ALL(包含DDR+UB)、DDR、UB
            AscendC::WriteGmByPassDCache<T>(reinterpret_cast<__gm__ T *>(srcGm), value); // value = 6
        }

在核 1 中，一直循环读取GM中输入tensor srcGlobal的第0个元素的取值value，当value的取值与核 0 向srcGlobal写入的值相等时，此时可以认为GM中srcGlobal第 1 个元素也已经被核 0 写入。

        if (blockIdx == 1) {
            while(true){
                __gm__ T *addr = const_cast<__gm__ T *>(srcGlobal.GetPhyAddr());
                T value = AscendC::ReadGmByPassDCache<T>(addr);
                if(value == 6){
                    __gm__ T *addr = const_cast<__gm__ T *>(srcGlobal.GetPhyAddr());
                    T valueNew = AscendC::ReadGmByPassDCache<T>(addr+1);
                    AscendC::printf("GM上addr+1位置的值：%u\n",valueNew);
                    AscendC::WriteGmByPassDCache<T>(reinterpret_cast<__gm__ T *>(dstGm), 2 * valueNew);
                    return;
                }
            }
        }

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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201;make -j;    # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证输出结果是否正确
  ```

  使用CPU调试或NPU仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201;make -j; # CPU调试模式
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201;make -j; # NPU仿真模式
  ```
  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：dav-2201对应Atlas A2/A3，dav-3510对应Ascend 950 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
