from __future__ import annotations

import tempfile
import threading
import time
import unittest
from pathlib import Path

from presmoke.case_runners import build_case_runner_cells_with_skips
from presmoke.model import Cell, Command, ExampleSpec
from presmoke.pool import NpuSlotPool
from presmoke.runner import run_cell


def cell(tmp: Path, name: str, mode: str = "npu", commands=None) -> Cell:
    spec = ExampleSpec(tmp / name, name, commands or [], ["dav-2201"], [mode], "test")
    spec.path.mkdir(parents=True, exist_ok=True)
    return Cell(spec, "dav-2201", mode, commands or [], spec.path / f"build_{mode}")


class RunnerPoolTest(unittest.TestCase):
    def test_runner_timeout(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            c = cell(Path(tmp), "x", commands=[Command("sleep 2", "run")])
            result = run_cell(c, Path(tmp) / "logs", timeout=1, keep_artifacts=True)
        self.assertEqual(result.status, "FAIL")
        self.assertEqual(result.rc, 124)

    def test_npu_gate_limits_concurrency(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            active = 0
            max_active = 0
            lock = threading.Lock()
            pool = NpuSlotPool(2)

            def work(c):
                nonlocal active, max_active
                command = Command("./demo", "run")

                def gated() -> int:
                    nonlocal active, max_active
                    with lock:
                        active += 1
                        max_active = max(max_active, active)
                    time.sleep(0.05)
                    with lock:
                        active -= 1
                    return 0

                pool.gate(c, command, gated)
                return c.key

            cells = [cell(Path(tmp), f"x{i}") for i in range(5)]
            threads = [threading.Thread(target=work, args=(c,)) for c in cells]
            for thread in threads:
                thread.start()
            for thread in threads:
                thread.join()
        self.assertLessEqual(max_active, 2)

    def test_manifest_skip_does_not_create_cell(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            runner = root / "scripts/presmoke/cases/x/run.sh"
            runner.parent.mkdir(parents=True)
            runner.write_text("#!/usr/bin/env bash\n", encoding="utf-8")
            manifest = root / "scripts/presmoke/reports/case_runner_manifest.json"
            manifest.parent.mkdir(parents=True)
            manifest.write_text(
                '[{"case":"x","skip":true,"skip_reason":"needs TensorFlow 2.6.5","supported_archs":["dav-2201"],"supported_modes":["npu"]}]',
                encoding="utf-8",
            )
            cells, suggestions, skipped = build_case_runner_cells_with_skips(root, "dav-2201", ["npu"], [], [])
        self.assertEqual(cells, [])
        self.assertEqual(len(skipped), 1)
        self.assertEqual(skipped[0].status, "SKIP")
        self.assertIn("TensorFlow 2.6.5", skipped[0].reason)
        self.assertEqual(len(suggestions), 1)

    def test_unsupported_arch_is_excluded_not_counted_as_skip(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            runner = root / "scripts/presmoke/cases/x/run.sh"
            runner.parent.mkdir(parents=True)
            runner.write_text("#!/usr/bin/env bash\n", encoding="utf-8")
            manifest = root / "scripts/presmoke/reports/case_runner_manifest.json"
            manifest.parent.mkdir(parents=True)
            manifest.write_text(
                '[{"case":"x","skip":false,"supported_archs":["dav-3510"],"supported_modes":["npu"]}]',
                encoding="utf-8",
            )
            cells, suggestions, skipped = build_case_runner_cells_with_skips(root, "dav-2201", ["npu"], [], [])
        self.assertEqual(cells, [])
        self.assertEqual(skipped, [])
        self.assertEqual(len(suggestions), 1)
        self.assertIn("Excluded on unsupported arch dav-2201", suggestions[0].message)


if __name__ == "__main__":
    unittest.main()
