# SIMT样例运行操作指导

本文档记录从 GitCode 下载 `cann/asc-devkit` 仓库 `exp/kadc` 分支，并编译运行 `demo/simt` 下两个样例的完整步骤。

## 1. 环境说明

- 仓库地址：`https://gitcode.com/cann/asc-devkit.git`
- 分支：`exp/kadc`
- 本次验证提交：`a894b8c8c75e71fe0076e8218f4912c7260eed0a`
- 本地目录：`/home/wly/asc-devkit-exp-kadc-run`
- CANN 环境脚本：`/usr/local/Ascend/cann/set_env.sh`

`demo/simt` 下包含两个样例：

| 样例目录 | 说明 |
|---|---|
| `demo/simt/basic_scatter` | SIMT Scatter 样例 |
| `demo/simt/matrix_transpose` | SIMT MatrixTranspose 访存优化样例 |

## 2. 下载代码

进入工作目录：

```bash
cd /home/wly
```

克隆 `exp/kadc` 分支：

```bash
git clone --branch exp/kadc --single-branch https://gitcode.com/cann/asc-devkit.git /home/wly/asc-devkit-exp-kadc-run
```

确认分支和提交：

```bash
cd /home/wly/asc-devkit-exp-kadc-run
git rev-parse --abbrev-ref HEAD
git rev-parse HEAD
```

本次验证输出：

```text
exp/kadc
a894b8c8c75e71fe0076e8218f4912c7260eed0a
```

查看 `demo/simt` 下的样例：

```bash
find demo/simt -mindepth 1 -maxdepth 1 -type d | sort
```

输出：

```text
demo/simt/basic_scatter
demo/simt/matrix_transpose
```

## 3. 配置 CANN 环境

每次打开新终端后，先执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
```

可检查 `cmake` 是否可用：

```bash
which cmake
```

本次验证输出：

```text
/usr/bin/cmake
```

## 4. 运行 basic_scatter 样例

进入样例目录：

```bash
cd /home/wly/asc-devkit-exp-kadc-run/demo/simt/basic_scatter
```

创建并进入构建目录：

```bash
mkdir -p build
cd build
```

配置工程：

```bash
cmake ..
```

编译工程：

```bash
make -j
```

运行默认样例：

```bash
./scatter
```

本次验证关键输出：

```text
[INFO] input_length=10, output_length=50
[INFO] Block number is 1.
[INFO] Thread number in a block is 10.
[Success] Case accuracy is verification passed.
```

运行指定 shape 样例：

```bash
./scatter 8 32
```

本次验证关键输出：

```text
[INFO] input_length=8, output_length=32
[INFO] Block number is 1.
[INFO] Thread number in a block is 8.
[Success] Case accuracy is verification passed.
```

## 5. 运行 matrix_transpose 样例

进入样例目录：

```bash
cd /home/wly/asc-devkit-exp-kadc-run/demo/simt/matrix_transpose
```

该样例通过编译参数 `SCENARIO_NUM` 选择不同场景：

| SCENARIO_NUM | 场景 |
|---|---|
| `0` | copy |
| `1` | transpose_naive |
| `2` | transpose_coalesced |
| `3` | transpose_avoid_bank_conflicts |

逐个编译并运行 4 个场景：

```bash
for s in 0 1 2 3; do
  echo "=== matrix_transpose SCENARIO_NUM=${s} ==="
  mkdir -p build_case_${s}
  cd build_case_${s}
  cmake -DSCENARIO_NUM=${s} ..
  make -j
  ./matrix_transpose
  cd ..
done
```

本次验证关键输出：

```text
=== matrix_transpose SCENARIO_NUM=0 ===
Running case 0: copy...
[Success] Case accuracy is verification passed.

=== matrix_transpose SCENARIO_NUM=1 ===
Running case 1: transpose_naive...
[Success] Case accuracy is verification passed.

=== matrix_transpose SCENARIO_NUM=2 ===
Running case 2: transpose_coalesced...
[Success] Case accuracy is verification passed.

=== matrix_transpose SCENARIO_NUM=3 ===
Running case 3: transpose_avoid_bank_conflicts...
[Success] Case accuracy is verification passed.
```

## 6. 一键复现实验命令

如果已经完成代码下载，可直接执行下面命令重新编译并运行两个样例：

```bash
source /usr/local/Ascend/cann/set_env.sh

cd /home/wly/asc-devkit-exp-kadc-run/demo/simt/basic_scatter
mkdir -p build
cd build
cmake ..
make -j
./scatter
./scatter 8 32

cd /home/wly/asc-devkit-exp-kadc-run/demo/simt/matrix_transpose
for s in 0 1 2 3; do
  echo "=== matrix_transpose SCENARIO_NUM=${s} ==="
  mkdir -p build_case_${s}
  cd build_case_${s}
  cmake -DSCENARIO_NUM=${s} ..
  make -j
  ./matrix_transpose
  cd ..
done
```

## 7. 注意事项

1. `SCENARIO_NUM` 必须显式设置为 `0`、`1`、`2` 或 `3`。不要执行 `cmake -DSCENARIO_NUM=$SCENARIO_NUM ..` 时让 shell 变量为空，否则会生成空宏并导致编译失败。
2. 如果运行时报 `context is a null pointer`，优先检查 CANN 环境是否已 source、设备是否可见、当前用户是否有设备访问权限。
3. 若复用旧构建目录遇到异常，可删除对应 `build` 或 `build_case_*` 目录后重新配置。
