# Issue1761 Sanitizer工具使用指导

## 1. 适用范围

本文用于在DAV 2201 NPU环境中人工复核`group_barrier`样例的多核GM竞争问题，重点确认：

1. 样例功能输出是否正确；
2. sanitizer插桩是否生效；
3. Issue1761涉及的28条初始化WAW是否消失；
4. 剩余RAW是否来自GroupBarrier的软件同步路径。

目标样例：[group_barrier.asc:L39](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:39)

## 2. 环境信息

本机已验证环境：

```text
CANN_PATH=/home/developer/Ascend/cann-9.2.0
NpuArch=dav-2201
NPU=Ascend 910B3
ASCEND_VISIBLE_DEVICES=7
mssanitizer=/home/developer/Ascend/cann-9.2.0/bin/mssanitizer
```

当设置`ASCEND_VISIBLE_DEVICES=7`时，程序中的逻辑设备`deviceId=0`对应物理NPU7。

执行前配置环境：

```bash
source /home/developer/Ascend/cann-9.2.0/set_env.sh
export ASCEND_VISIBLE_DEVICES=7

npu-smi info
mssanitizer --version
```

人工检查点：

- `npu-smi info`中NPU7的Health应为`OK`；
- `ASCEND_HOME_PATH`应指向预期CANN安装目录；
- `mssanitizer`应来自同一套CANN环境。

## 3. 普通模式功能验证

建议使用临时构建目录，避免污染仓库：

```bash
cd /mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6

BUILD_DIR=$(mktemp -d /tmp/issue1761-group-barrier.XXXXXX)
cmake \
  -S examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier \
  -B "${BUILD_DIR}" \
  -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build "${BUILD_DIR}" -j8

cd "${BUILD_DIR}"
./demo
```

预期打印6行，顺序可能因核调度不同而变化，但每行结果均应为`24`：

```text
[AIV Block 2/8] [Block (0/6)]: OUTPUT = 24
[AIV Block 3/8] [Block (1/6)]: OUTPUT = 24
...
[AIV Block 7/8] [Block (5/6)]: OUTPUT = 24
```

如果结果不是`24`，应先按功能问题处理，不继续将racecheck报告解释为工具误报。

## 4. Sanitizer插桩编译

racecheck要求kernel在编译时启用sanitizer。仅对普通二进制执行`mssanitizer`不会得到有效竞争检测结果。

```bash
cd /mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6

SAN_BUILD_DIR=$(mktemp -d /tmp/issue1761-group-barrier-san.XXXXXX)
cmake \
  -S examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier \
  -B "${SAN_BUILD_DIR}" \
  -DCMAKE_ASC_ARCHITECTURES=dav-2201 \
  '-DCMAKE_ASC_FLAGS=-g --sanitizer'
cmake --build "${SAN_BUILD_DIR}" -j8
```

参数说明：

| 参数 | 作用 |
|---|---|
| `-DCMAKE_ASC_ARCHITECTURES=dav-2201` | 按本机2201架构编译kernel |
| `-g` | 保留源码行号和调用栈信息 |
| `--sanitizer` | 启用sanitizer设备侧插桩 |

人工检查点：编译必须完整结束并出现`Built target demo`，不能只确认CMake配置成功。

## 5. 执行racecheck

```bash
cd "${SAN_BUILD_DIR}"

mssanitizer \
  --tool=racecheck \
  --log-file=./racecheck.log \
  ./demo
```

需要进一步核对DCache使用时，可增加：

```bash
mssanitizer \
  --tool=racecheck \
  --check-dcci=yes \
  --log-file=./racecheck-dcci.log \
  ./demo
```

不要把以下提示视为检测通过：

```text
racecheck is not supported on the kernel that without
`--cce-enable-sanitizer' compiler option
```

出现该提示说明二进制没有正确插桩，应返回第4节重新编译。

## 6. 日志快速统计

```bash
cd "${SAN_BUILD_DIR}"

rg -o 'Potential (WAW|RAW|WAR) hazard' racecheck.log \
  | sort \
  | uniq -c
```

分别查看三类竞争：

```bash
rg -n 'Potential WAW hazard' racecheck.log
rg -n 'Potential RAW hazard' racecheck.log
rg -n 'Potential WAR hazard' racecheck.log
```

查看报告对应的源码位置：

```bash
rg -n '#[01] .*group_barrier\.asc:' racecheck.log
```

## 7. Issue1761验收标准

### 必须满足

- 普通运行时6个Waiter均输出`24`；
- sanitizer运行时功能输出仍为`24`；
- 日志中没有`Potential WAW hazard`；
- 不再出现8个核在输出基址执行`InitOutput`导致的28条WAW。

### 当前环境的已知结果

修复后的实际检测结果为：

```text
WAW: 0
RAW: 36
WAR: 0
```

36条RAW分为：

- 24条：Arriver原子更新`barWorkspace`，Waiter通过`dcci`和标量轮询读取；
- 12条：2个Arriver原子写输出，6个Waiter在`GroupBarrier::Wait()`返回后读取输出。

相关代码：

- [group_barrier.asc:L53](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:53)
- [group_barrier.asc:L60](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:60)
- [kernel_operator_group_barrier_impl.h:L84](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/impl/basic_api/dav_c220/core_mng/roc/kernel_operator_group_barrier_impl.h:84)
- [kernel_operator_group_barrier_impl.h:L101](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/impl/basic_api/dav_c220/core_mng/roc/kernel_operator_group_barrier_impl.h:101)

这些RAW位于GroupBarrier保护的软件同步路径。人工复核时应分别确认：

1. Arriver使用原子方式更新计数或输出；
2. Waiter在读取输出前已从`Wait()`返回；
3. `Wait()`通过`dcci`刷新缓存并轮询到期望计数；
4. RAW地址属于`barWorkspace`或受GroupBarrier保护的输出区，而不是新的无保护业务地址。

只有同时满足以上条件，才能将对应RAW归类为工具未建模的软件同步报告。

## 8. 修复前后对比方法

如需严格复核28条WAW的消除情况，可分别对修复前提交和当前工作树进行插桩构建。不要在当前工作区直接切换或覆盖文件，建议使用两个独立`git worktree`或两份仓库。

对两份日志执行：

```bash
rg -o 'Potential (WAW|RAW|WAR) hazard' racecheck-before.log \
  | sort | uniq -c

rg -o 'Potential (WAW|RAW|WAR) hazard' racecheck-after.log \
  | sort | uniq -c
```

预期核心差异：

| 版本 | 初始化WAW |
|---|---:|
| 修复前 | 28 |
| 修复后 | 0 |

## 9. 其他Sanitizer检查

racecheck通过专项验收后，可补充执行：

```bash
mssanitizer --tool=memcheck --log-file=./memcheck.log ./demo
mssanitizer --tool=initcheck --log-file=./initcheck.log ./demo
mssanitizer --tool=synccheck --log-file=./synccheck.log ./demo
```

用途：

| 工具 | 关注点 |
|---|---|
| `memcheck` | GM/Local Memory越界、非法地址和内存泄漏等问题 |
| `initcheck` | 未初始化内存读取 |
| `synccheck` | 流水同步及同步原语使用问题 |
| `racecheck` | 多核或多流水间RAW、WAR、WAW竞争 |

## 10. 常见问题处理

### 10.1 未启用sanitizer

现象：日志提示kernel没有`--cce-enable-sanitizer`。

处理：删除旧临时构建目录，按第4节重新配置并编译，确认使用了`-g --sanitizer`。

### 10.2 NPU不可见或设备号错误

```bash
echo "${ASCEND_VISIBLE_DEVICES}"
npu-smi info
```

本样例Host代码使用逻辑设备0。当只暴露物理NPU7时，应设置`ASCEND_VISIBLE_DEVICES=7`。

### 10.3 日志没有源码行号

处理：确认编译参数包含`-g`，并确认执行的是刚完成插桩构建的`demo`。

### 10.4 输出正确但仍有RAW

不要仅凭输出正确判定RAW为误报。先核对报告地址、写侧操作、读侧操作以及两者之间是否存在明确的核间同步原语，再按第7节分类。

### 10.5 程序长时间不退出

可能原因包括输入文件缺失、Waiter未收到同步消息或kernel阻塞。先查看终端错误和日志末尾；确认需要终止时使用`Ctrl+C`，随后检查输入文件、启动核数和同步参数。

## 11. 人工复核记录模板

```text
复核日期：
复核人：
代码提交：
CANN版本：
NPU型号：
物理设备号：
ASCEND_VISIBLE_DEVICES：

普通编译：通过/失败
普通运行：通过/失败
6个Waiter是否均输出24：是/否
Sanitizer插桩编译：通过/失败

WAW数量：
RAW数量：
WAR数量：
是否存在InitOutput初始化WAW：是/否
剩余RAW地址及分类：

最终结论：通过/不通过/需工具侧确认
备注：
```
