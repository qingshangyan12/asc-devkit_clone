# conv2d_forward_tensor_api

## 概述

使用 Tensor API 实现的极简 Conv2D Forward 样例，演示以下 Tensor API 卷积通路串联成完整计算：

- GM(NC1HWC0) -> Local Memory L1(NC1HWC0)：`CopyGM2L1`（flat-5D 卷积搬运路径）
- Local Memory L1(NC1HWC0) -> Local Memory L0A(NZ)：`CopyL12L0AImg2Col`（img2col 展开）
- GM(DN) -> Local Memory L1(NZ) -> Local Memory L0B(ZN)：`CopyGM2L1` + `CopyL12L0B`（权重走普通通路）
- Local Memory L0A x Local Memory L0B -> Local Memory L0C(NZ)：`Mmad`
- Local Memory L0C(NZ) -> GM(NC1HWC0)：`CopyL0C2GM`（flat-5D 卷积搬运路径）

## 规格

| 项目 | 取值 |
|---|---|
| Batch | 1 |
| 输入通道 Cin (C1 x C0) | 16 (1 x 16) |
| 输出通道 Cout | 16 |
| 输入 H x W | 8 x 8 |
| 卷积核 KH x KW | 3 x 3 |
| Stride | 1 |
| Dilation | 1 |
| Pad（上下左右） | 1 |
| 输出 Ho x Wo | 8 x 8 |
| 数据类型 | fmap/weight/输出：half；L0C 累加：float |

## 编译运行

```bash
source <cann_path>/set_env.sh
rm -rf build
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

预期输出 `test pass!`。
