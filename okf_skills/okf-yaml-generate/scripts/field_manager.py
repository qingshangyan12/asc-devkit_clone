#!/usr/bin/env python3
"""Validate the OKF field registry and code capability boundary."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys
import importlib.util

import yaml


SKILL_ROOT = Path(__file__).resolve().parents[1]
REGISTRY = SKILL_ROOT / "config" / "document-profile-registry.yaml"
CONTROLLER = Path(__file__).resolve().parent
sys.path.insert(0, str(CONTROLLER.resolve()))


def load_registry() -> dict:
    return yaml.safe_load(REGISTRY.resolve().read_text(encoding="utf-8"))


def controller_capabilities() -> tuple[set[str], set[str]]:
    source = (CONTROLLER / "controller.py").resolve()
    spec = importlib.util.spec_from_file_location("okf_yaml_controller", source)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load controller: {source}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return set(module.GENERATED_FIELDS), set(module.VALIDATED_FIELDS)


def check() -> dict:
    model = load_registry()
    errors: list[str] = []
    common = set(model.get("common_fields", []))
    required = set(model.get("common_required_fields", []))
    order = model.get("field_order", [])
    catalog = set(model.get("field_catalog", {}))
    if not required <= common:
        errors.append("common_required_fields contains fields outside common_fields")
    if set(order) != catalog or len(order) != len(set(order)):
        errors.append("field_order must contain every field_catalog field exactly once")
    registered = common | catalog
    for name, profile in model.get("profiles", {}).items():
        unknown = set(profile.get("fields", [])) - registered
        if unknown:
            errors.append(f"profile {name} references unknown fields: {sorted(unknown)}")
    generated, validated = controller_capabilities()
    if missing := sorted(registered - generated):
        errors.append(f"missing generators: {missing}")
    if missing := sorted(registered - validated):
        errors.append(f"missing validators: {missing}")
    return {"ok": not errors, "registry": str(REGISTRY.resolve()), "errors": errors}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("command", choices=["check"])
    parser.parse_args(argv)
    result = check()
    print(json.dumps(result, ensure_ascii=False, indent=2))
    return 0 if result["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
