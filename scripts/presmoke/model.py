from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional


@dataclass(frozen=True)
class Command:
    raw: str
    kind: str
    env: Dict[str, str] = field(default_factory=dict)


@dataclass
class Suggestion:
    example: str
    category: str
    severity: str
    message: str
    hint: str = ""


@dataclass
class ExampleSpec:
    path: Path
    rel_path: str
    commands: List[Command]
    archs: List[str]
    modes: List[str]
    source: str
    suggestions: List[Suggestion] = field(default_factory=list)


@dataclass
class Cell:
    example: ExampleSpec
    arch: str
    mode: str
    commands: List[Command]
    build_dir: Path

    @property
    def key(self) -> str:
        return f"{self.example.rel_path}|{self.arch}|{self.mode}"


@dataclass
class StepResult:
    command: str
    kind: str
    rc: Optional[int]
    duration_s: float
    wait_s: float = 0.0
    npu_slot: bool = False
    timeout: bool = False


@dataclass
class RunResult:
    example: str
    arch: str
    mode: str
    status: str
    reason: str = ""
    failing_step: Optional[str] = None
    rc: Optional[int] = None
    duration_s: float = 0.0
    log_file: str = ""
    source: str = ""
    steps: List[StepResult] = field(default_factory=list)


@dataclass
class RunReport:
    host_arch: str
    modes: List[str]
    started_at: str
    finished_at: str
    results: List[RunResult]
    suggestions: List[Suggestion]
