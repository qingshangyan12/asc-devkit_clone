import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

import yaml


RUN = Path(__file__).resolve().parent
WORKSPACE = RUN.parents[3]
ROOT = Path("/home/hugh/code/asc-devkit")
DRAFT_ROOT = WORKSPACE / "docs/zh/api/SIMD-API/c_api/sync"
DESCRIPTION_CLI = WORKSPACE / "okf_skills/okf-description-generate/scripts/description.py"
TASK = json.loads((RUN / "batches/batch-01/task.json").read_text())
SCAFFOLD = json.loads((RUN / "batches/batch-01/scaffold.json").read_text())
TAXONOMY = json.loads(Path(TASK["taxonomy"]).read_text())
NAMESPACES = list(TAXONOMY["namespaces"])


def source_excerpt(body, kind):
    match = re.search(r"(?ms)^## \u529f\u80fd\u8bf4\u660e\s*\n(.*?)(?=^## |\Z)", body)
    if match:
        section = match.group(1).strip()
        section = re.sub(r'^\u5934\u6587\u4ef6\u8def\u5f84\u4e3a\uff1a[^\n]*\n\s*', '', section)
        return section[:3000].rstrip()
    if kind == "reference.concept":
        return body[:3000].rstrip()
    raise ValueError("missing feature section")


def line_for_dependency(body, stem):
    for line in body.splitlines():
        if stem in line and line.strip() and not line.lstrip().startswith("#"):
            return line.strip()
    raise ValueError(f"no source evidence for dependency {stem}")


annotations = []
for item in SCAFFOLD["documents"]:
    if item["plan"]["okftype"] == "reference.index":
        continue
    path = item["path"]
    source = ROOT / path
    body = source.read_text(encoding="utf-8")
    digest = hashlib.sha256(source.read_bytes()).hexdigest()
    draft_text = (DRAFT_ROOT / Path(path).relative_to("docs/zh/api/SIMD-API/c_api/sync")).read_text(encoding="utf-8")
    draft = yaml.safe_load(draft_text.split("---\n", 2)[1])
    description = draft["description"]
    excerpt = source_excerpt(body, item["plan"]["okftype"])
    if not description or not excerpt:
        raise ValueError(f"missing description or evidence: {path}")

    run_id = Path(path).stem
    description_dir = RUN / "descriptions" / run_id
    subprocess.run([
        sys.executable, str(DESCRIPTION_CLI), "prepare", "--devkit-root", str(ROOT),
        "--document", path, "--run-id", run_id, "--run-dir", str(description_dir),
    ], check=True, stdout=subprocess.DEVNULL)
    result = {
        "schema_version": 1, "purpose": "okf_api_description_result", "run_id": run_id,
        "document_path": path, "document_sha256": digest, "status": "ready",
        "description": description, "evidence": [{"excerpt": excerpt}], "confidence": 0.9,
    }
    result_path = description_dir / "description-result.json"
    result_path.write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    subprocess.run([
        sys.executable, str(DESCRIPTION_CLI), "validate", "--run-dir", str(description_dir),
        "--result", str(result_path),
    ], check=True, stdout=subprocess.DEVNULL)
    validation = json.loads((description_dir / "validation.json").read_text())
    if not validation["valid"]:
        raise ValueError(f"description validation failed: {path}: {validation['errors']}")

    selected = {key: [] for key in NAMESPACES}
    for tag in draft.get("tag", []):
        selected.update(tag)
    if not selected["roles"]:
        selected["roles"] = ["control"]
    evidence = {}
    for namespace, values in selected.items():
        for value in values:
            key = f"{namespace}:{value}"
            if namespace == "api_surfaces":
                text = body.splitlines()[0]
            elif namespace == "execution_models" and "PIPE_S" in body:
                text = "PIPE_S"
            elif namespace == "traits" and value == "deprecated":
                text = next(line.strip() for line in body.splitlines() if "\u5e9f\u5f03" in line)
            else:
                text = excerpt
            evidence[key] = [{"source_path": path, "excerpt": text}]

    dependencies = []
    for old in draft.get("dependencies", []):
        stem = Path(old["path"]).stem
        dep = {key: old[key] for key in ("path", "kind", "condition") if key in old}
        dep["evidence"] = {"source_path": path, "excerpt": line_for_dependency(body, stem)}
        dependencies.append(dep)
    annotations.append({
        "document_path": path, "document_sha256": digest,
        "description": {"value": description, "evidence": [{"source_path": path, "excerpt": excerpt}]},
        "tag": selected, "tag_evidence": evidence, "dependencies": dependencies,
    })

payload = {
    "schema_version": 1, "purpose": "okf_yaml_semantic_result",
    "run_id": TASK["run_id"], "batch_id": TASK["batch_id"], "annotations": annotations,
}
output = RUN / "batches/batch-01/semantic-input.json"
output.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
print(json.dumps({"annotations": len(annotations), "output": str(output)}, ensure_ascii=False))
