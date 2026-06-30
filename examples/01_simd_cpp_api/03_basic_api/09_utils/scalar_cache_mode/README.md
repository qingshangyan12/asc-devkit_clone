# ScalarCacheMode样例

## 概述

本样例基于 `SetScalarCacheMode` 和 `GetScalarCacheMode` 接口验证 CTRL 寄存器的 L2Cache 读写模式设置和获取功能。

本样例演示了设置和验证 CTRL 寄存器的 L2Cache 模式控制位：
- **READ mode**: CTRL[19:16] 控制 L2Cache 读取模式
- **WRITE mode**: CTRL[23:20] 控制 L2Cache 写入模式

CacheMode 对应的 4-bit 值：
- `CACHE_MODE_NORMAL`: 0 (4'b0000) - 正常模式
- `CACHE_MODE_LAST`: 1 (4'b0001) - Last Victim 模式
- `CACHE_MODE_PERSISTENT`: 2 (4'b0010) - Persistent 模式
- `CACHE_MODE_DISABLE`: 4 (4'b0100) - 禁用缓存模式

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── scalar_cache_mode
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── scalar_cache_mode.asc   // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：  
  验证 CTRL 寄存器 L2Cache 模式控制位的设置和获取功能：
  1. SetScalarCacheMode：设置 READ/WRITE 模式的指定 CacheMode
  2. GetScalarCacheMode：获取 READ/WRITE 模式的 CacheMode
  3. 比较获取的 mode 是否与设置的 mode 一致，一致输出对应值，不一致输出 -1
  4. Duplicate：将结果存入 ctrlLocal（每次填充4个元素，保证32字节对齐）
  5. DataCopy：输出到 dstCtrlGlobal

- 样例规格：
  <table>
  <caption>表1：样例规格说明</caption>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">1</td></tr>
  <tr><td rowspan="1" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[1, 256]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output_ctrl</td><td align="center">[1, 32]</td><td align="center">int64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_scalar_cache_mode</td></tr>  
  </table>

- 输出说明：
  - output_ctrl.bin：寄存器值验证输出，32个 int64_t 值
    - [0-3]: READ CACHE_MODE_NORMAL = 0
    - [4-7]: READ CACHE_MODE_LAST = 1
    - [8-11]: READ CACHE_MODE_PERSISTENT = 2
    - [12-15]: READ CACHE_MODE_DISABLE = 4
    - [16-19]: WRITE CACHE_MODE_NORMAL = 0
    - [20-23]: WRITE CACHE_MODE_LAST = 1
    - [24-27]: WRITE CACHE_MODE_PERSISTENT = 2
    - [28-31]: WRITE CACHE_MODE_DISABLE = 4

- 验证逻辑：
  - SetScalarCacheMode 设置指定 CacheMode
  - GetScalarCacheMode 获取当前 CacheMode
  - 比较获取值与设置值是否一致（rwMode 2种 × cacheMode 4种 = 8种组合）

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                 # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py                                       # 生成测试输入数据和golden数据
  ./demo                                                               # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py                                  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;   # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  > **注意：** 本样例仅在 Ascend 950PR/Ascend 950DT 上支持。

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```