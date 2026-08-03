# Mutex Cross-Pipeline Synchronization Validation Sample

## Overview

This sample validates Mutex patterns between two consecutive Vector operations and a following MTE3 copy on Ascend 950PR/Ascend 950DT. `SCENARIO_NUM` selects a scenario. BiSheng compiler auto synchronization is explicitly disabled so it cannot mask the synchronization being validated.

## Scenarios

| `SCENARIO_NUM` | Scenario | Device result | Conclusion |
| --- | --- | --- | --- |
| 0 | One `PIPE_V` lock contains two dependent VFs separated by `PipeBarrier<PIPE_V>`, followed by an MTE3 lock and copy | Pass | Correct, but the barrier is unnecessary because hardware guarantees `PIPE_V` synchronization on architecture 3510 |
| 1 | Two `PIPE_V` lock pairs with the same MutexID each contain one VF, with `PipeBarrier<PIPE_V>` between them, followed by an MTE3 lock and copy | Pass | Correct; the second Lock does not replace an intra-pipeline barrier, but still acquires and releases the Mutex; this form is redundant |
| 2 | One `PIPE_V` lock contains two dependent VFs without `PipeBarrier<PIPE_V>`, followed by an MTE3 lock and copy | Pass | Correct; validates the architecture 3510 hardware guarantee for `PIPE_V` synchronization |
| 3 | Two independent VFs use separate `PIPE_V` lock pairs with the same MutexID, followed by an MTE3 lock and copy | Pass | Correct; MTE3 executes after the second `Unlock<PIPE_V>`, but the V lock pairs can be combined |
| 4 | Two independent VFs share one `PIPE_V` lock without a barrier, followed by an MTE3 lock and copy | Pass | Correct and recommended for this case |

In scenarios 3 and 4, `Vector2` runs 32 consecutive VFs and the final value 32 is copied out. `DataCopyPad` copies the `Vector2` result first to enlarge and observe any window in which MTE3 might start too early.

> [!NOTE]
> `PipeBarrier<PIPE_V>` handles dependencies within the PIPE_V pipeline, while Mutex synchronizes different pipelines. The architecture 3510 hardware guarantee described above must not be generalized to other architectures. Mutex itself is supported only on Ascend 950PR/Ascend 950DT.

## Recommended Pattern

When `Vector1` and `Vector2` have no data dependency and MTE3 depends on both completing, place both VFs in one V lock pair:

```cpp
AscendC::Mutex::Lock<PIPE_V>(mutexId);
Vector1();
Vector2();
AscendC::Mutex::Unlock<PIPE_V>(mutexId);

AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
AscendC::DataCopyPad(...);
AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
```

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Build and Run

After configuring the CANN environment, run all scenarios from this sample directory:

```bash
source ${install_path}/cann/set_env.sh
bash run_all.sh
```

Each scenario is expected to report:

```text
different elements: 0/262144
test pass!
```
