from __future__ import annotations

import concurrent.futures
import threading
import time
from typing import Callable, Iterable, List, TypeVar

from .model import Cell, Command

T = TypeVar("T")


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


def run_all(cells: Iterable[Cell], jobs: int, npu_slots: int, fn: Callable[[Cell], T]) -> List[T]:
    cells_list = list(cells)
    if jobs <= 1:
        return [fn(cell) for cell in cells_list]
    results: List[T] = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as executor:
        futures = [executor.submit(fn, cell) for cell in cells_list]
        for future in concurrent.futures.as_completed(futures):
            results.append(future.result())
    return results
