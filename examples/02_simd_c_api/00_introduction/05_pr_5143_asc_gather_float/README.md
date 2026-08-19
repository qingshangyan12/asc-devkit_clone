# PR 5143 asc_gather vector_float验证

## 验证目标

本样例验证PR 5143新增的寄存器源接口：

```cpp
asc_gather(vector_float& dst, vector_float src, vector_uint32_t index);
```

该接口将`src`和`dst`按位重解释为`vector_uint32_t`，再调用`uint32_t`类型的`vselr`。样例在同一个kernel中分别调用新增的`vector_float`重载和既有的`vector_uint32_t`重载，并对两组结果进行逐位比较。

## 覆盖场景

- 正数、负数和普通小数。
- 正零和负零。
- 正负无穷。
- 带payload的正负NaN。
- 正负非规格化数。
- 最大有限数和最小正规数。
- 任意顺序、重复索引，以及超过寄存器元素数后按`index % 64`取值的索引。

浮点相等无法区分正零和负零，NaN也无法用`==`比较，因此Host侧以`uint32_t`原始编码生成golden并进行位级校验。

## 编译运行

PR 5143尚未合入当前分支时，先将PR源码导出到临时目录：

```bash
git fetch https://gitcode.com/cann/asc-devkit.git \
  refs/merge-requests/5143/head:refs/remotes/cann/mr-5143
mkdir -p /tmp/asc-devkit-pr5143-no-lfs-9b6962b7e
git -c filter.lfs.process= -c filter.lfs.smudge= -c filter.lfs.required=false \
  archive refs/remotes/cann/mr-5143 | \
  tar -x -C /tmp/asc-devkit-pr5143-no-lfs-9b6962b7e
export ASC_DEVKIT_PR5143_ROOT=/tmp/asc-devkit-pr5143-no-lfs-9b6962b7e
```

在仓库根目录执行：

```bash
"$ASCEND_HOME_PATH/bin/bisheng" \
  -iquote "$ASC_DEVKIT_PR5143_ROOT/include" \
  -iquote "$ASC_DEVKIT_PR5143_ROOT" \
  --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations \
  --asc-aicore-lang \
  examples/02_simd_c_api/00_introduction/05_pr_5143_asc_gather_float/asc_gather_vector_float.asc \
  -lm -o /tmp/pr5143_asc_gather_vector_float
timeout --signal=TERM --kill-after=5s 30s /tmp/pr5143_asc_gather_vector_float
```

验证所用PR提交为`9b6962b7eb431045f74735c3ea219232681f360d`。实际编译运行结果见[VALIDATION.md](./VALIDATION.md)。
