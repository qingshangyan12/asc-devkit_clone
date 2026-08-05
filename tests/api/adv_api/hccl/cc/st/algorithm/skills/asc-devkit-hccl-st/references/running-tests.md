# 构建、运行和排障

## 1. 先解析当前 worktree

```bash
cd <用户指定 worktree>
git status --short
sed -n '1,220p' tests/api/adv_api/hccl/cc/st/algorithm/CMakeLists.txt
sed -n '1,220p' tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/CMakeLists.txt
```

不要把基线仓的构建目录、CANN 路径或旧二进制名套到功能 worktree。

## 2. 当前 MC2 CCU 目标

```text
ascendc_ut_adv_api_hccl_ccu_mc2_host
ascendc_ut_adv_api_hccl_ccu_mc2_device
```

使用仓库 README 给出的配置命令。找到实际二进制后先列用例：

```bash
<host-binary> --gtest_list_tests
<device-binary> --gtest_list_tests
```

然后运行：

```bash
<host-binary>
<device-binary>
```

单 case 使用：

```bash
<binary> --gtest_filter='<Suite>.<Case>'
```

不要预设 suite 名。以 `--gtest_list_tests` 输出为准。

## 3. 回归范围

- Host tiling/资源改动：Host 全量。
- collective kernel 改动：Kernel Device 全量，再跑 KFC 结果用例。
- KFC server/消息改动：KFC Device 全量。
- taskArg/peer 表：Device 目标全量。
- 公共 CCU proxy/sim/checker：所有 CCU Device case。
- AICPU 通用原语/DAG：所有受影响 AICPU suites。

当前 AllToAllV 最近历史基线为 Host 9、Kernel 8、KFC 10，共 27 case；只作为回归参考，不能代替本次实际统计。

## 4. 常见问题

### 构建成功但没有算法执行证据

检查是否只命中 success-only Stub。Register/Launch 成功必须伴随 Program、任务、状态变化或最终数据断言。

### 新 `.cc` 未生效

检查它属于 testcase target、`hccl-model-proxy` 还是 `hccl-verifier`，再查对应层 CMake。不要仅增加 include。

### undefined reference

先确认真实产品源是否应进入目标，再判断依赖应放到 Host 最小 Stub、模型库还是产品静态库。weak fallback 不能掩盖被测主路径。

### case 卡住

缩小到单 case，打印 Rank、Mission、队列槽和 Event/CKE 状态。区分 AICPU DAG 死锁、CCU wait 无生产者和 KFC 状态未推进。

### Checker 误报

先判断规则属于公共结构还是 collective ABI。固定 taskArg/peer 规则移到 `semantics_check`，不要放宽公共 Checker 来消除一个算法的误报。

## 5. 提交前

```bash
git diff --check
rg -n 'ccu_direct|ccu_kfc_direct|ccu_mc2_kfc_proxy|ST_ALL_TO_ALL_V_MC2_CCU_TEST' \
  tests/api/adv_api/hccl/cc/st/algorithm
```

汇报目标名、实际 case 数、通过数、未运行项和硬件层能力边界。
