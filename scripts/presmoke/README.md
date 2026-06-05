# Presmoke Watch Entry

`scripts/presmoke_orchestrate.sh` is the CI and manual watch entry. It runs one full 910B or 950 presmoke round, writes archiveable reports, and can retry timeout cases on other cards.

`scripts/run_presmoke2.sh` is the lower-level debug entry. Use it for dry-run, single-case checks, direct Python CLI experiments, or regenerating schedules.

Before a full watch run, the orchestrator removes all entries under the active CANN `opp/vendors` directory. This keeps custom-op package installation deterministic across rounds. Runs with `--filter`, `--dry-run`, or `--suggestions-only` do not clean `opp/vendors`.

## CI Commands

910B:

```bash
PROJECT_ROOT=/path/to/asc-devkit \
ARCH=dav-2201 \
PRIMARY_CARD=7 \
RETRY_CARDS="0 1 2 3 4 5 6" \
bash scripts/presmoke_orchestrate.sh
```

950:

```bash
PROJECT_ROOT=/path/to/asc-devkit \
ARCH=dav-3510 \
PRIMARY_CARD=0 \
bash scripts/presmoke_orchestrate.sh
```

Dry-run:

```bash
ARCH=dav-2201 PRIMARY_CARD=7 bash scripts/presmoke_orchestrate.sh --dry-run
ARCH=dav-3510 PRIMARY_CARD=0 bash scripts/presmoke_orchestrate.sh --dry-run
```

## Parameters

| Variable | Default | Meaning |
|---|---|---|
| `PROJECT_ROOT` | script parent repo | asc-devkit checkout to run. |
| `ASCEND_HOME_DIR` | `/usr/local/Ascend/ascend-toolkit/latest` | CANN toolkit root containing `set_env.sh`. |
| `ARCH` | `dav-2201` | Target arch. Use `dav-2201` for 910B and `dav-3510` for 950. |
| `PRIMARY_CARD` | `0` | Main NPU card id used for the full run. |
| `RETRY_CARDS` | empty | Space-separated card ids for retrying timeout cases. |
| `JOBS` | `auto` | Parallel build workers. NPU mode uses a conservative CPU-count ladder; CPU-only mode uses `ceil(CPU(s) / 4)`, capped at 64. |
| `NPU_SLOTS` | `1` | NPU queue concurrency. Keep `1` for watch runs. |
| `CPU_RUN_SLOTS` | `auto` | CPU-only run workers. `auto` uses host `CPU(s)` from `lscpu`. Ignored by NPU mode. |
| `MAKE_JOBS` | `auto` | Per-case `make -j` limit. `auto` uses `CPU(s) / resolved JOBS`; explicit `make -j` commands are capped by the presmoke wrapper. |
| `TIMEOUT` | `120` | Timeout in seconds for NPU run commands. Build and verify commands do not use this timeout. |
| `CPU_RUN_TIMEOUT` | `300` | Timeout in seconds for CPU run commands. Increase this when CPU-only validation is slower than expected. |
| `SCHEDULE` | `fixed` | Case submit order. Watch runs should use `fixed`. |
| `SCHEDULE_FILE` | built-in | Optional fixed order override. |
| `OUT_ROOT` | `presmoke_reports/orchestrate_<arch>_<timestamp>` | Output directory for CI archive. |
| `RUNNER_MODE` | `case-runner` | Default runner implementation. |
| `MODES` | `npu` | Presmoke modes. |
| `MODE` | unset | Alias used only when `MODES` is unset. For example, `MODE=cpu`. |

CPU debug mode is opt-in. Use `MODES=cpu` or `MODE=cpu`; the default watch path remains `npu`. CPU mode does not use the NPU fixed schedule unless `SCHEDULE_FILE` is explicitly provided.

For CPU mode, the default parallelism is computed from the host CPU count instead of hard-coded for specific machines. For example, a 192-CPU host resolves to `JOBS=48`, `MAKE_JOBS=4`, and `CPU_RUN_SLOTS=192`; an 88-CPU host resolves to `JOBS=22`, `MAKE_JOBS=4`, and `CPU_RUN_SLOTS=88`.

## Reports

The orchestrator writes:

- `status.txt`: high-level progress and final location.
- `full_card<N>/meta.txt`: env, card, elapsed time, and `npu-smi` snapshots.
- `full_card<N>/stdout.log` and `stderr.log`: raw wrapper logs.
- `full_card<N>/results/report.json`: machine-readable presmoke report.
- `full_card<N>/results/report.md`: detailed Markdown report.
- `full_card<N>/results/logs/<case>__<mode>.log`: full per-case command log.
- `full_card<N>/results/logs/stages/<case>__<mode>__<stage>.log`: per-stage command logs for build, run, and verify.
- `FINAL_REPORT.md`: concise summary with failures and retry results.
- `ALL_CASE_TIMINGS.tsv`: per-case timing, NPU wait, step details, and executed commands.
- `FAILURES.tsv`: failed or skipped primary results plus retry outcome.

## Fixed Schedule

The default fixed schedules are:

- `scripts/presmoke/schedules/dav-2201_npu.txt`
- `scripts/presmoke/schedules/dav-3510_npu.txt`

They preserve the required custom-op ordering:

- `custom_op_static_lib` runs before `custom_op`.
- `parallel_ops_package` runs after `custom_op_static_lib`.
- `aclnn`, `aclop`, `onnx`, `tensorflow`, and `tiling_sink` run after `custom_op`.

New cases not listed in a schedule file are appended to the end, then the custom-op dependency order is enforced again.

To regenerate a fixed schedule from a previous run:

```bash
PYTHONPATH=scripts PRESMOKE_PROJECT_ROOT=$PWD python3 -m presmoke \
  --runner-mode case-runner \
  --arch dav-2201 \
  --modes npu \
  --jobs auto \
  --schedule npu-idle-min \
  --schedule-report presmoke_reports/latest/full_card7/results/report.json \
  --export-schedule scripts/presmoke/schedules/dav-2201_npu.txt \
  --dry-run \
  --report-format json \
  --results /tmp/presmoke_schedule_export
```

## Notes

- Do not run multiple orchestrators in the same checkout at the same time. A lock under `.presmoke_locks` protects against accidental overlap.
- The full watch entry cleans the active CANN `opp/vendors` directory. Make sure the CI user has permission to remove and reinstall custom-op vendor packages.
- `run_presmoke.sh` is the old presmoke script and is not the watch entry for this flow.
- TensorFlow cases that require TensorFlow 2.6.5 are intentionally skipped by the case manifest.
- Timeout retry is for timeout cases only. Non-timeout failures are reported and left for diagnosis.
