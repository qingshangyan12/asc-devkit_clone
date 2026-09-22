# Issue1676 mssanitizer使用指导

## 1. Sanitizer与mssanitizer的关系

`sanitizer`和`mssanitizer`不是两套竞争检测工具：

| 名称 | 所在阶段 | 作用 |
|---|---|---|
| `--sanitizer`或`--cce-enable-sanitizer` | 编译/链接阶段 | 对Ascend C kernel插桩并链接检测运行库，使设备侧访存事件可被检测 |
| `mssanitizer` | 程序运行阶段 | 启动被插桩的程序，按`racecheck/memcheck/initcheck/synccheck`模式收集并报告问题 |

因此，完整流程必须同时包含插桩编译和`mssanitizer`运行。对未插桩二进制直接执行`mssanitizer`，工具会忽略对应kernel或无法完成有效检测。

仓内Sanitizer样例使用`-g --sanitizer`。Issue1676原始环境使用：

```bash
ASCFLAGS="-g --cce-enable-sanitizer"
cmake -DCMAKE_ASC_LINK_FLAGS=--cce-enable-sanitizer ...
```

两种写法的目的相同。本文脚本使用当前CANN 9.2.0已验证可编译的`-g --sanitizer`，并保留链接参数`--cce-enable-sanitizer`。

## 2. Issue1761与Issue1676的检测事项

两个Issue都使用`mssanitizer --tool=racecheck`，但检测对象和验收条件不同：

| 对比项 | Issue1761 | Issue1676 |
|---|---|---|
| 样例 | `group_barrier` | 兼容性指南`fill` |
| 原始目标架构 | DAV 2201 | DAV 3510 |
| 并发范围 | 多个AIV核之间 | 单个AIC内不同流水之间 |
| 存储区域 | GM | L1中的A1/B1 |
| 主要风险 | 8个核在同步前同址初始化 | A1/B1地址重叠；MTE2写与MTE1读缺少同步 |
| 原始报告 | 28条WAW及GroupBarrier相关RAW | 4条RAW和1条WAW |
| 修复要点 | 仅`block0`初始化，再执行`SyncAll()` | 错开A1/B1地址；增加`MTE2_MTE1`同步 |
| 验收标准 | 初始化WAW从28降为0 | L1 hazard从5降为0 |

结论：两个Issue使用同一工具和同一`racecheck`模式，但检测事项不一致，不能用Issue1761脚本代替Issue1676验收。

Issue原文：[GitCode Issue1676](https://gitcode.com/cann/asc-devkit/issues/1676)

## 3. 架构限制

Issue1676的问题只存在于DAV 3510分支：

- DAV 2201在[fill.asc:L120](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/06_compatibility_guide/fill/fill.asc:120)直接初始化L0A/L0B；
- DAV 3510在[fill.asc:L124](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/06_compatibility_guide/fill/fill.asc:124)先初始化A1/B1，再通过MTE1搬运到L0A/L0B。

当前机器是Ascend910B3/DAV 2201，可以完成DAV 3510交叉编译，但不能在本机NPU上执行DAV 3510 kernel。因此：

- 当前机器可执行脚本的`build-only`模式；
- 真正的racecheck验收必须在Ascend950/DAV 3510环境执行；
- 在2201上运行该样例通过，不能证明Issue1676已修复，因为不会进入问题代码分支。

## 4. 当前代码状态

当前分支已经包含Issue1676的两项修复：

1. [fill.asc:L130](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/06_compatibility_guide/fill/fill.asc:130)使用`SetFlag/WaitFlag<MTE2_MTE1>`，保证`Fill`完成后才执行`LoadData`；
2. [fill.asc:L145](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/06_compatibility_guide/fill/fill.asc:145)将`b1Addr`设置到A1区域之后，避免A1/B1重叠。

对应提交可通过以下命令查看：

```bash
git log --oneline -- \
  examples/01_simd_cpp_api/06_compatibility_guide/fill/fill.asc
```

## 5. 使用脚本

脚本位置：[run-issue-1676-mssanitizer.sh](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/run-issue-1676-mssanitizer.sh)

### 5.1 当前2201机器：仅交叉编译

```bash
./run-issue-1676-mssanitizer.sh build-only
```

该模式按`dav-3510`完成普通编译和sanitizer插桩编译，但不会运行kernel。

### 5.2 Ascend950机器：功能和racecheck验收

```bash
export CANN_PATH=/path/to/cann
export ISSUE1676_NPU_ID=3
./run-issue-1676-mssanitizer.sh racecheck
```

执行全部Sanitizer检查：

```bash
./run-issue-1676-mssanitizer.sh all
```

可将第二个参数指定为固定结果目录：

```bash
./run-issue-1676-mssanitizer.sh racecheck /tmp/issue1676-review
```

## 6. 手工执行步骤

以下步骤只能在Ascend950/DAV 3510设备上完成动态验证。

```bash
cd /mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6
source "${CANN_PATH}/set_env.sh"

export ASCEND_VISIBLE_DEVICES=3
export ASCEND_RT_VISIBLE_DEVICES=3

SAMPLE_DIR="${PWD}/examples/01_simd_cpp_api/06_compatibility_guide/fill"
BUILD_DIR=$(mktemp -d /tmp/issue1676-fill-san.XXXXXX)

cmake \
  -S "${SAMPLE_DIR}" \
  -B "${BUILD_DIR}" \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  '-DCMAKE_ASC_FLAGS=-g --sanitizer' \
  '-DCMAKE_ASC_LINK_FLAGS=--cce-enable-sanitizer'
cmake --build "${BUILD_DIR}" -j8

cd "${BUILD_DIR}"
python3 "${SAMPLE_DIR}/scripts/gen_data.py"
mssanitizer \
  --tool=racecheck \
  --log-level=error \
  --log-file=./racecheck.log \
  ./demo
python3 "${SAMPLE_DIR}/scripts/verify_result.py" \
  output/output.bin output/golden.bin
```

## 7. 日志判读

统计L1 hazard：

```bash
rg -o 'Potential (WAW|RAW|WAR) hazard' racecheck.log \
  | sort | uniq -c

rg -n 'L1|PIPE_MTE2|PIPE_MTE1|Potential .* hazard' racecheck.log
```

修复后的验收标准：

```text
功能精度校验：通过
WAW: 0
RAW: 0
WAR: 0
```

修复前典型结果：

```text
RAW: 4
WAW: 1
```

人工复核时重点确认：

1. WAW是否指向A1/B1相同L1偏移；
2. RAW写侧是否为`PIPE_MTE2 Fill`；
3. RAW读侧是否为`PIPE_MTE1 LoadData`；
4. A1区间为`[a1Addr, a1Addr + M*K*sizeof(T))`；
5. B1区间从`b1Addr`开始且不与A1区间重叠；
6. `Fill`与`LoadData`之间存在`MTE2_MTE1`事件同步。

## 8. 差异化数据验证

原问题被“两个L1区域都填1”掩盖。除racecheck外，建议人工构造不同填充值：

- A矩阵填2；
- B矩阵填1；
- 期望输出为256，而非地址重叠时可能出现的192。

该验证需要临时修改`InitConstA1`的填充值并重新编译。不要只修改输入文件，因为Issue1676涉及的是kernel内部`Fill`写入L1的数据。

## 9. 人工复核记录模板

```text
复核日期：
复核人：
代码提交：
CANN版本：
mssanitizer版本：
NPU型号：
NpuArch：dav-3510
设备号：

普通编译：通过/失败
插桩编译：通过/失败
普通功能验证：通过/失败
racecheck功能验证：通过/失败

L1 WAW数量：
L1 RAW数量：
L1 WAR数量：
A1地址区间：
B1地址区间：
MTE2_MTE1同步：存在/缺失

最终结论：通过/不通过
备注：
```
