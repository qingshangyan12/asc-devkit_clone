from __future__ import annotations

import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path


class OrchestrateShellTest(unittest.TestCase):
    def copy_orchestrate_fixture(self, root: Path) -> Path:
        project_root = Path(__file__).resolve().parents[3]
        script = root / "scripts/presmoke_orchestrate.sh"
        script.parent.mkdir(parents=True)
        shutil.copy(project_root / "scripts/presmoke_orchestrate.sh", script)
        script.chmod(0o755)
        run_presmoke = root / "scripts/run_presmoke2.sh"
        run_presmoke.write_text(
            """#!/usr/bin/env bash
set -euo pipefail
results=""
while [[ $# -gt 0 ]]; do
    if [[ "$1" == "--results" ]]; then
        results="$2"
        shift 2
        continue
    fi
    shift
done
mkdir -p "$results"
cat > "$results/report.json" <<'JSON'
{"summary":{"PASS":0,"FAIL":0,"SKIP":0},"results":[],"npu_stats":{"busy_s":0,"idle_s":0,"utilization":0}}
JSON
cat > "$results/report.md" <<'MD'
# stub
MD
""",
            encoding="utf-8",
        )
        run_presmoke.chmod(0o755)
        return script

    def test_full_run_cleans_cann_vendors_before_presmoke(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            script = self.copy_orchestrate_fixture(root)
            vendors = root / "opp/vendors"
            stale = vendors / "stale_vendor"
            stale.mkdir(parents=True)
            (stale / "old.txt").write_text("old", encoding="utf-8")
            env = {
                **os.environ,
                "PROJECT_ROOT": str(root),
                "OUT_ROOT": str(root / "out"),
                "ASCEND_OPP_PATH": str(root / "opp"),
            }

            result = subprocess.run(["bash", str(script)], text=True, capture_output=True, env=env, check=False)

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertFalse(stale.exists())
            self.assertIn("vendors_clean_done", result.stdout)

    def test_filtered_run_does_not_clean_cann_vendors(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            script = self.copy_orchestrate_fixture(root)
            vendors = root / "opp/vendors"
            stale = vendors / "stale_vendor"
            stale.mkdir(parents=True)
            (stale / "old.txt").write_text("old", encoding="utf-8")
            env = {
                **os.environ,
                "PROJECT_ROOT": str(root),
                "OUT_ROOT": str(root / "out"),
                "ASCEND_OPP_PATH": str(root / "opp"),
            }

            result = subprocess.run(
                ["bash", str(script), "--filter", "x"],
                text=True,
                capture_output=True,
                env=env,
                check=False,
            )

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertTrue(stale.exists())
            self.assertIn("vendors_clean_skip reason=non_full_run", result.stdout)

    def test_dry_run_does_not_clean_cann_vendors(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            script = self.copy_orchestrate_fixture(root)
            vendors = root / "opp/vendors"
            stale = vendors / "stale_vendor"
            stale.mkdir(parents=True)
            (stale / "old.txt").write_text("old", encoding="utf-8")
            env = {
                **os.environ,
                "PROJECT_ROOT": str(root),
                "OUT_ROOT": str(root / "out"),
                "ASCEND_OPP_PATH": str(root / "opp"),
            }

            result = subprocess.run(
                ["bash", str(script), "--dry-run"],
                text=True,
                capture_output=True,
                env=env,
                check=False,
            )

            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertTrue(stale.exists())
            self.assertIn("vendors_clean_skip reason=non_full_run", result.stdout)


if __name__ == "__main__":
    unittest.main()
