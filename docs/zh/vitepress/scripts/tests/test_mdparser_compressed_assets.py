# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Run the actual batch CLI and read copied gzip/tar attachment contents."""

import io
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile
import unittest
import zipfile


SCRIPTS = Path(__file__).resolve().parents[1]


class CompressedStaticAssetTests(unittest.TestCase):
    def check_archive(self, name):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            source = root / "docs"
            target = root / "rendered"
            source.mkdir()
            (source / "index.md").write_text("# Attachments\n\n[Download](" + name + ")\n")
            (source / "guide.md").write_text("# Another page\n")
            payload = b"actual documentation attachment\n"
            archive = source / name
            if name.endswith(".zip"):
                with zipfile.ZipFile(archive, "w") as package:
                    package.writestr("payload.txt", payload)
            else:
                info = tarfile.TarInfo("payload.txt")
                info.size = len(payload)
                with tarfile.open(archive, "w:gz") as package:
                    package.addfile(info, io.BytesIO(payload))
            command = [sys.executable, "-c", "from mdparser.cli import main; main()", str(source), "-o", str(target)]
            result = subprocess.run(command, capture_output=True, text=True, check=False, timeout=10)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('href="' + name + '"', (target / "index.html").read_text())
            copied = target / name
            self.assertTrue(copied.is_file(), "batch conversion omitted a whitelisted attachment")
            self.assertEqual(copied.read_bytes(), archive.read_bytes())
            if name.endswith(".zip"):
                with zipfile.ZipFile(copied) as package:
                    self.assertEqual(package.read("payload.txt"), payload)
            else:
                with tarfile.open(copied, "r:gz") as package:
                    member = package.extractfile("payload.txt")
                    self.assertIsNotNone(member)
                    self.assertEqual(member.read(), payload)

    def test_tar_gzip_attachment_is_copied(self):
        self.check_archive("sample.tar.gz")

    def test_uppercase_tar_gzip_attachment_is_copied(self):
        self.check_archive("sample.TAR.GZ")

    def test_tar_gzip_with_additional_filename_suffix_is_copied(self):
        self.check_archive("sample.v1.tar.gz")

    def test_single_suffix_zip_attachment_remains_supported(self):
        self.check_archive("sample.zip")


if __name__ == "__main__":
    unittest.main()
