# conv2d_forward_tensor_api

## 概述

使用 Tensor API 实现的 Conv2D Forward 样例，支持多种输入/输出格式（NC1HWC0、NCHW、NHWC），通过编译时参数 `SCENARIO_NUM` 切换。演示以下 Tensor API 卷积通路串联成完整计算：

- GM -> Local Memory L1(NC1HWC0)：`CopyGM2L1`（按 SCENARIO_NUM 路由：NC1HWC0 直搬、NCHW 走 dn2nz、NHWC 走 nd2nz）
- Local Memory L1(NC1HWC0) -> Local Memory L0A(NZ)：`CopyL12L0A`（img2col 展开）
- GM(ND) -> Local Memory L1(ZN) -> Local Memory L0B(ZN)：`CopyGM2L1` + `CopyL12L0B`（权重走普通通路）
- Local Memory L0A x Local Memory L0B -> Local Memory L0C(NZ)：`Mmad`
- Local Memory L0C(NZ) -> GM：`CopyL0C2GM`（按 SCENARIO_NUM 路由：NC1HWC0 直搬、NCHW 走 nz2dn、NHWC 走 nz2nd）

## 规格

| 项目 | 取值 |
|---|---|
| Batch (N) | 4 |
| BlockDim | 4（每核处理一个 batch） |
| 输入通道 Cin (C1 x C0) | 32 (2 x 16) |
| 输出通道 Cout | 16 |
| 输入 H x W | 9 x 9 |
| 卷积核 KH x KW | 3 x 3 |
| Stride | 2 |
| Dilation | 2 |
| Pad（上下左右） | 2 |
| Pad Value | 1 |
| 输出 Ho x Wo | 5 x 5 |
| 数据类型 | fmap/weight/输出：half；L0C 累加：float |
| GM 格式 | 由 SCENARIO_NUM 控制：0=NC1HWC0，1=NCHW，2=NHWC |

## 编译运行

```bash
source <cann_path>/set_env.sh
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

切换输入/输出格式：将 `-DSCENARIO_NUM=0` 改为 `1`（NCHW）或 `2`（NHWC），重新编译运行即可。

预期输出 `test pass!`。
