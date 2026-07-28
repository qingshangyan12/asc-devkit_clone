# vf_add样例

## 概述

本样例将基础 API `04_memory_management/tensor_allocation` 的场景4迁移为 Ascend C C API 实现。原场景在 `dav-3510` 上定义 `__simd_vf__` 的 `AddVF`，通过 `asc_vf_call` 调用，并在 VF 函数中完成 RegTensor 的加载、加法和搬出。

迁移后仍保持相同的计算、数据类型和输入公式：对两个长度为1024的 float32 ND 向量执行逐元素加法 `z = x + y`。C API 使用带地址空间限定符的 UB 数组代替 LocalTensor/LocalMemAllocator，使用 `asc_load`、`asc_add`、`asc_store` 代替 `Reg::LoadAlign`、`Reg::Add`、`Reg::StoreAlign`。

本样例仅支持 Ascend 950PR/Ascend 950DT（`dav-3510`），不提供 CPU 域调试模式。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── vf_add
│   ├── scripts
│   │   ├── gen_data.py              // 输入和真值生成脚本
│   │   └── verify_result.py         // 输出精度及样本打印脚本
│   ├── CMakeLists.txt               // CMake编译文件
│   ├── vf_add.asc                   // C API kernel与主机侧调用代码
│   ├── README.md                    // 中文说明文档
│   └── README_en.md                 // 英文说明文档
```

## 样例规格

| 项目 | 说明 |
| --- | --- |
| 算子 | Add，`z = x + y` |
| 输入x | `[1024]`，float32，ND |
| 输入y | `[1024]`，float32，ND |
| 输出z | `[1024]`，float32，ND |
| Kernel函数 | `vf_add` |
| 核数 | 1 |

数据生成沿用基础 API 样例的确定性公式：`x[i] = (i % 100) * 0.1`，`y[i] = ((i + 50) % 100) * 0.2`；真值为 float32 的 `x + y`。

## 实现说明

### 基础 API 与 C API 映射

| 基础 API 场景4 | C API 实现 | 说明 |
| --- | --- | --- |
| `LocalMemAllocator<Hardware::UB>` + `LocalTensor` | `__ubuf__ float x_ub/y_ub/z_ub[1024]` | C API通过数组申请UB内存。 |
| `DataCopy(GM→UB)` | `asc_copy_gm2ub_sync` | 同步接口在读取UB前完成等待。 |
| `asc_vf_call<AddVF>` | 直接调用`__simd_vf__`函数`AddVfCapi` | C API不使用基础 API 的 VF 调用封装。 |
| `Reg::LoadAlign` | `asc_load` | 每次读取一个 float VL到`vector_float`。 |
| `Reg::Add` | `asc_add` | 使用`asc_create_mask_b32(PAT_ALL)`执行逐元素加法。 |
| `Reg::StoreAlign` | `asc_store` | 每次将一个 float VL写回UB。 |
| `DataCopy(UB→GM)` | `asc_copy_ub2gm_sync` | 同步接口在核函数结束前完成写回。 |

在 `dav-3510` 上，`asc_get_vf_len()`返回256字节，因此 float32 的一个VL为64个元素。1024能被64整除，VF函数循环16次，所有循环均使用全掩码；这与原样例按 `GetVecLen() / sizeof(float)` 计算单次处理元素数的逻辑一致。

设备侧调用顺序如下：

```text
GM x/y --asc_copy_gm2ub_sync--> UB x/y
UB x/y --asc_load--> vector_float --asc_add--> vector_float --asc_store--> UB z
UB z --asc_copy_ub2gm_sync--> GM z
```

主机侧读取 `input/x.bin`、`input/y.bin`，申请Device/Host内存和Stream，启动一个 `vf_add<<<1, 0, stream>>>` 核函数，随后将输出写入 `output/output.bin`。所有 ACL 返回值和文件读写均会检查；失败路径使用统一清理标签释放已申请资源。

## 数据生成与精度对比

`scripts/gen_data.py`生成两个 float32 输入文件及真值文件。`scripts/verify_result.py`以原基础 API 相同的 `1e-3` 绝对误差阈值逐元素比较输出和真值。

无论对比通过还是失败，校验脚本都会打印 x、y、output、golden 的前8个 float32 元素；失败时还会额外打印最多101个超出阈值的元素索引、预期值、实际值和绝对误差。

## 编译运行

在样例根目录执行以下命令：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

> **注意：** 生成脚本、程序和校验脚本都应在`build`目录中执行，使其共享`input/`和`output/`目录。当前迁移仅进行了静态核对，未执行上述命令。

使用NPU仿真模式时，在CMake命令中增加 `-DCMAKE_ASC_RUN_MODE=sim`。切换运行模式前，应清理构建目录中的CMake缓存。

验证通过时，输出包含：

```text
test pass!
```
