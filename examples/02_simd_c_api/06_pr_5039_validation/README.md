# PR 5039 C API验证样例

本目录验证[PR 5039](https://gitcode.com/cann/asc-devkit/pull/5039)在DAV 3510上新增的以下能力：

- `asc_duplicate_highest`：广播源矢量寄存器的最高位元素，未被mask选中的目标位置清零。
- `asc_duplicate_highest_merge`：广播源矢量寄存器的最高位元素，未被mask选中的目标位置保留原值。
- `asc_load_dev`：通过`ld_dev`绕过DataCache直接读取GM；样例同时使用`asc_store_dev`验证绕过DataCache的标量读写可见性。

## 编译前提

当前`alll`分支不包含PR 5039的接口实现。验证PR时，将环境变量指向PR 5039的checkout；PR合入后，在仓库根目录执行时可不设置该变量。

```bash
git fetch https://gitcode.com/cann/asc-devkit.git refs/merge-requests/5039/head
git worktree add --detach /tmp/asc-devkit-pr5039 FETCH_HEAD
export ASC_DEVKIT_PR5039_ROOT=/tmp/asc-devkit-pr5039
```

每个`.asc`文件第一行均给出了独立编译运行命令。验证时建议在命令外增加`timeout 30s`。

## 验证设计

| 样例 | 关键输入 | 预期输出 |
| --- | --- | --- |
| `reg/data_fill/asc_duplicate_highest.asc` | 源矢量为1到64，mask仅选择前4个元素，目标初始值为-7 | 前4个元素为64，其余元素为0 |
| `reg/data_fill/asc_duplicate_highest_merge.asc` | 源矢量为1到64，mask仅选择前4个元素，目标初始值为-7 | 前4个元素为64，其余元素保留-7 |
| `scalar_compute/asc_load_dev.asc` | 普通标量读取11使其进入DataCache，再用`asc_store_dev`将GM改为123456789 | `asc_load_dev`读取123456789，普通标量再次读取仍观察到DataCache中的11，Host最终从GM读取123456789 |
| `scalar_compute/asc_load_dev_signed_repro.asc` | 默认覆盖窄有符号返回值；定义`REPRO_INT64_COMPILE_FAILURE`后覆盖`int64_t*`重载 | 默认运行产生预期精度失败；宏模式产生预期device编译失败 |

`asc_load_dev`样例中的DataCache旧值是刻意构造的观察结果。样例只通过普通标量读取建立干净Cache Line，不通过普通标量写入制造脏Cache Line，因此不会在kernel结束时用旧值覆盖`asc_store_dev`直接写入GM的新值。PR head的完整实机结果及发现的问题见[VALIDATION.md](VALIDATION.md)。

## 问题复现

准备好上述PR checkout和`ASC_DEVKIT_PR5039_ROOT`后，在仓库根目录执行：

```bash
# 复现int8_t/int16_t/int32_t负数未符号扩展；程序打印实际值与golden并以1退出。
"$ASCEND_HOME_PATH/bin/bisheng" \
  -iquote "$ASC_DEVKIT_PR5039_ROOT/include" -iquote "$ASC_DEVKIT_PR5039_ROOT" \
  --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang \
  examples/02_simd_c_api/06_pr_5039_validation/scalar_compute/asc_load_dev_signed_repro.asc \
  -lm -o /tmp/pr5039_asc_load_dev_signed_repro
timeout 30s /tmp/pr5039_asc_load_dev_signed_repro

# 复现int64_t*重载无法device编译；bisheng应在asc_load_dev_impl.h:57报错。
"$ASCEND_HOME_PATH/bin/bisheng" -DREPRO_INT64_COMPILE_FAILURE \
  -iquote "$ASC_DEVKIT_PR5039_ROOT/include" -iquote "$ASC_DEVKIT_PR5039_ROOT" \
  --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang \
  examples/02_simd_c_api/06_pr_5039_validation/scalar_compute/asc_load_dev_signed_repro.asc \
  -lm -o /tmp/pr5039_asc_load_dev_int64_repro
```
