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
import locale
from pathlib import Path
import tempfile
import unittest

from presmoke.model import Command
from presmoke.runner import _run_command


class RunnerOutputDecodingTest(unittest.TestCase):
    @unittest.skipUnless(locale.getpreferredencoding(False).lower().replace("-", "") == "utf8", "requires UTF-8 locale")
    def test_command_output_with_invalid_bytes_is_recorded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            log = io.StringIO()
            command = Command(r"printf 'start\377end\n'", "build")
            rc = _run_command(command, Path(tmp), log, timeout=5)
            self.assertEqual(rc, 0)
            self.assertEqual(log.getvalue(), "start\ufffdend\n")

    @unittest.skipUnless(locale.getpreferredencoding(False).lower().replace("-", "") == "utf8", "requires UTF-8 locale")
    def test_invalid_stderr_preserves_failure_status(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            log = io.StringIO()
            command = Command(r"printf 'error\377\n' >&2; exit 23", "build")
            rc = _run_command(command, Path(tmp), log, timeout=5)
            self.assertEqual(rc, 23)
            self.assertEqual(log.getvalue(), "error\ufffd\n")
