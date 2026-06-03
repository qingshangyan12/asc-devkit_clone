from __future__ import annotations

import unittest
from pathlib import Path

from presmoke.generate_case_runners import is_atc_prerequisite_command, quote_env_value, render_runner
from presmoke.model import Command


class GenerateCaseRunnersTest(unittest.TestCase):
    def test_atc_prerequisite_python_stays_in_build_before_atc(self) -> None:
        commands = [
            Command("python3 ../leaky_relu.py", "run"),
            Command("atc --model=./leaky_relu.onnx --soc_version=${soc_version}", "shell"),
            Command("cmake ..", "cmake"),
            Command("make -j", "make"),
            Command("./execute_leaky_relu_op", "run"),
        ]
        build_cmds = [
            command
            for command in commands
            if command.kind in {"cmake", "make", "gen_data", "shell"} or is_atc_prerequisite_command(command, commands)
        ]
        run_cmds = [command for command in commands if command.kind in {"run", "package_run"} and command not in build_cmds]
        script = render_runner("x", build_cmds, run_cmds, [])
        self.assertLess(script.index("python3 ../leaky_relu.py"), script.index("atc --model=./leaky_relu.onnx"))
        self.assertLess(script.index("atc --model=./leaky_relu.onnx"), script.index("cmake .."))
        self.assertLess(script.index("cmake .."), script.index("make -j"))
        self.assertIn("./execute_leaky_relu_op", script)

    def test_cmake_build_command_is_not_treated_as_configure(self) -> None:
        script = render_runner(
            "x",
            [Command("cmake -S . -B build", "cmake"), Command("cmake --build build -j", "cmake")],
            [],
            [],
        )
        self.assertIn("cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES", script)
        self.assertIn("cmake --build build -j'", script)
        self.assertNotIn("cmake --build build -j -DCMAKE_ASC_ARCHITECTURES", script)

    def test_env_value_references_are_not_single_quoted(self) -> None:
        self.assertEqual(quote_env_value("$ARCH"), "$ARCH")
        self.assertEqual(quote_env_value("$SOC_VERSION"), "$SOC_VERSION")
        self.assertEqual(quote_env_value("$SCENARIO"), "$SCENARIO")
        self.assertEqual(quote_env_value("${SCENARIO}"), "${SCENARIO}")
        self.assertEqual(quote_env_value("plain value"), "'plain value'")


if __name__ == "__main__":
    unittest.main()
