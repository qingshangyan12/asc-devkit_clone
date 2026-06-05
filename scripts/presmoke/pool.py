from __future__ import annotations

import threading
import time
from typing import Callable

from .model import Cell, Command


class NpuSlotPool:
    def __init__(self, slots: int) -> None:
        if slots <= 0:
            raise ValueError("npu slots must be positive")
        self._sem = threading.BoundedSemaphore(slots)

    def gate(self, cell: Cell, command: Command, fn: Callable[[], int]) -> tuple[int, float]:
        started = time.monotonic()
        with self._sem:
            wait_s = time.monotonic() - started
            return fn(), wait_s
