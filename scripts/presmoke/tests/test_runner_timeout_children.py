#!/usr/bin/python3
# coding=utf-8

# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

import io
import os
from pathlib import Path
import shlex
import signal
import sys
import tempfile
import time
import unittest
import uuid

from presmoke.model import Command
from presmoke.runner import _run_command


@unittest.skipUnless(sys.platform.startswith("linux") and hasattr(os, "killpg"), "requires Linux process groups")
class RunnerTimeoutChildrenTest(unittest.TestCase):
    def test_timeout_kills_child_after_leader_exits(self):
        with tempfile.TemporaryDirectory() as tmp:
            marker = Path(tmp) / "child.pid"
            token = "presmoke-timeout-" + uuid.uuid4().hex
            child = (
                "import os,pathlib,signal,time; "
                "signal.signal(signal.SIGTERM, signal.SIG_IGN); "
                f"pathlib.Path({str(marker)!r}).write_text(str(os.getpid())); "
                "print('child ready', flush=True); time.sleep(30)"
            )
            parent = f"import subprocess,sys,time; subprocess.Popen([sys.executable, '-c', {child!r}, {token!r}]); time.sleep(30)"
            command = Command(f"{shlex.quote(sys.executable)} -c {shlex.quote(parent)}", "run")
            log = io.StringIO()
            start = time.monotonic()
            try:
                rc = _run_command(command, Path(tmp), log, timeout=3)
                self.assertEqual(rc, 124)
                self.assertIn("[TIMEOUT]", log.getvalue())
                self.assertTrue(marker.exists(), "child did not start")
                self.assertLess(time.monotonic() - start, 9)
            finally:
                if marker.exists():
                    pid = int(marker.read_text())
                    try:
                        cmdline = (Path("/proc") / str(pid) / "cmdline").read_bytes()
                        if token.encode() in cmdline:
                            os.kill(pid, signal.SIGKILL)
                    except (FileNotFoundError, ProcessLookupError):
                        pass


if __name__ == "__main__":
    unittest.main()
