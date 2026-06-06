from __future__ import annotations

import unittest
from pathlib import Path


class JenkinsfileTest(unittest.TestCase):
    def test_jenkinsfile_publishes_junit_and_marks_presmoke_failures_unstable(self) -> None:
        root = Path(__file__).resolve().parents[3]
        text = (root / "scripts/presmoke/Jenkinsfile").read_text(encoding="utf-8")

        self.assertIn("choice(name: 'ARCH', choices: ['dav-2201', 'dav-3510']", text)
        self.assertIn("choice(name: 'MODES', choices: ['npu', 'cpu']", text)
        self.assertIn("string(name: 'REPO_DIR', defaultValue: 'asc-devkit'", text)
        self.assertIn("env.ASC_DEVKIT_DIR = resolveRepoDir(params.REPO_DIR)", text)
        self.assertIn('dir("${env.ASC_DEVKIT_DIR}")', text)
        self.assertIn('bash scripts/run_presmoke.sh ${EXTRA_ARGS}', text)
        self.assertIn("PRESMOKE_JUNIT_PATTERN", text)
        self.assertIn("asc-devkit/presmoke_reports/**/junit.xml", text)
        self.assertIn("archiveArtifacts allowEmptyArchive: true", text)
        self.assertIn("asc-devkit/presmoke_reports/**/*.xml", text)
        self.assertIn("catchError(buildResult: 'UNSTABLE', stageResult: 'UNSTABLE')", text)
        self.assertIn("PRESMOKE_OUT_ROOT = \"presmoke_reports/jenkins_", text)
        self.assertIn("def resolveRepoDir(String preferred)", text)


if __name__ == "__main__":
    unittest.main()
