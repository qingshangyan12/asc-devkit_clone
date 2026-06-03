from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from presmoke.readme_spec import parse_readme, split_commands


README = """# sample

## 编译运行

- 配置环境变量
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```
- 样例执行
  ```bash
  SCENARIO=0
  mkdir -p build && cd build; # comment
  cmake -DSCENARIO_NUM=${SCENARIO} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  python3 ../scripts/gen_data.py ${SCENARIO}
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

- 编译选项说明
| 参数 | 说明 | 可选值 | 默认值 |
| :--- | :--- | :--- | :--- |
| CMAKE_ASC_RUN_MODE | 运行模式 | npu, sim | npu |
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
"""


class ReadmeSpecTest(unittest.TestCase):
    def test_split_commands_keeps_env_and_classifies(self) -> None:
        commands = split_commands("SCENARIO=0\ncmake ..; make -j && ./demo")
        self.assertEqual([c.kind for c in commands], ["cmake", "make", "run"])
        self.assertEqual(commands[0].env["SCENARIO"], "0")
        self.assertEqual(commands[-1].env["SCENARIO"], "0")

    def test_parse_readme_extracts_sample_block_arch_and_modes(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "examples"
            ex = root / "x"
            (ex / "scripts").mkdir(parents=True)
            (ex / "README.md").write_text(README, encoding="utf-8")
            (ex / "scripts" / "gen_data.py").write_text("", encoding="utf-8")
            (ex / "scripts" / "verify_result.py").write_text("", encoding="utf-8")
            spec = parse_readme(ex, root)
        self.assertEqual(spec.source, "readme")
        self.assertIn("dav-3510", spec.archs)
        self.assertEqual(spec.modes, ["npu", "sim"])
        self.assertTrue(any(c.kind == "verify" for c in spec.commands))


if __name__ == "__main__":
    unittest.main()

