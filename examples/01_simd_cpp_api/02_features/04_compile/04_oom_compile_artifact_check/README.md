# OOM编译产物检测样例

## 概述

本样例检查开启OOM编译时生成源码和单算子JSON的兼容性契约，不启动Kernel，
不依赖NPU设备或特定SoC。

样例覆盖以下场景：

- 动态普通Tensor同时生成storage-shape注册路径和原有地址范围回退路径；
- 动态TensorList同时生成storage-shape注册路径和原有二级指针回退路径；
- 静态shape继续使用原有固定长度OOM注册地址范围逻辑；
- 非OOM产物保持条件编译隔离，JSON不增加OOM能力字段；
- OOM JSON生成`version=1`、`tensor_version=1`，且`opParaSize`为2字节扩展头预留空间。

## 环境要求

- Python 3；
- AscendC devkit源码仓；
- CANN安装包，需包含`python/site-packages/asc_op_compile_base`及其本地库依赖。

不需要配置`SOC_ARCH`。脚本默认从所在目录解析devkit根目录；CANN路径可通过
`--cann-path`指定，也可使用`ASCEND_HOME_PATH`。

## 运行样例

在devkit仓根目录执行：

```bash
python3 examples/01_simd_cpp_api/02_features/04_compile/04_oom_compile_artifact_check/check_oom_artifacts.py \
  --cann-path /path/to/Ascend/cann
```

从其他目录执行时可显式指定devkit路径：

```bash
python3 check_oom_artifacts.py \
  --devkit-path /path/to/asc-devkit \
  --cann-path /path/to/Ascend/cann
```

全部检查通过时输出：

```text
PASS dynamic tensor keeps legacy fallback
PASS dynamic tensor emits storage-shape registration
PASS dynamic TensorList keeps legacy fallback
PASS dynamic TensorList emits storage-shape registration
PASS static shape keeps legacy OOM generation
PASS non-OOM artifacts remain unchanged
PASS OOM JSON capability and opParaSize
All OOM compile artifact checks passed.
```

在未安装驱动的环境中，CANN平台库退出时可能额外输出驱动日志级别探测提示，
该提示不影响检查结果和退出码。

## 退出码

- `0`：全部产物契约符合预期；
- `1`：至少一项产物契约不符合预期；
- `2`：参数、路径、模块导入或依赖环境错误。

若返回`2`，请优先检查`--devkit-path`、`--cann-path`以及CANN安装目录中的
Python包和动态库是否完整。
