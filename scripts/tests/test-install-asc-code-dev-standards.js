"use strict";

const assert = require("assert");
const fs = require("fs");
const os = require("os");
const path = require("path");
const { spawnSync } = require("child_process");

const packageRoot = path.resolve(__dirname, "../..");
const config = require(path.join(packageRoot, "package.json"));
const source = path.join(packageRoot, config.ascCodeStandardsSource);
const installer = path.join(packageRoot, "scripts/install-asc-code-dev-standards.js");
const scratch = fs.mkdtempSync(path.join(os.tmpdir(), "asc-code-standards-test-"));
const cleanEnv = { ...process.env, HOME: scratch, PATH: "" };
for (const key of ["CODEX_HOME", "OPENAI_API_KEY", "CLAUDE_CODE", "ANTHROPIC_API_KEY", "OPENCODE_DIR", "HERMES_HOME"]) {
  delete cleanEnv[key];
}

function run(env = {}, script = installer) {
  const result = spawnSync(process.execPath, [script], {
    cwd: scratch,
    env: { ...cleanEnv, ...env },
    encoding: "utf8",
  });
  assert.strictEqual(result.status, 0, result.stderr);
  return result.stdout;
}

function checkSkill(destination) {
  for (const file of ["SKILL.md", "references/development-standard.md", "references/cpp-development-standard.md"]) {
    assert.deepStrictEqual(fs.readFileSync(path.join(destination, file)), fs.readFileSync(path.join(source, file)));
  }
}

try {
  const codexHome = path.join(scratch, "codex-custom");
  const codexSkill = path.join(codexHome, "skills", "asc-code-dev-standards");
  const sibling = path.join(codexHome, "skills", "other-skill");
  fs.mkdirSync(codexSkill, { recursive: true });
  fs.mkdirSync(sibling, { recursive: true });
  fs.writeFileSync(path.join(codexSkill, "stale"), "old");
  fs.writeFileSync(path.join(sibling, "keep"), "other skill");
  const both = { CODEX_HOME: codexHome, CLAUDE_CODE: "1", OPENCODE_DIR: "1", HERMES_HOME: path.join(scratch, "hermes") };
  assert(run(both).includes(codexSkill));
  checkSkill(codexSkill);
  assert(!fs.existsSync(path.join(codexSkill, "stale")));
  assert.strictEqual(fs.readFileSync(path.join(sibling, "keep"), "utf8"), "other skill");
  fs.writeFileSync(path.join(codexSkill, "SKILL.md"), "outdated");
  run(both);
  checkSkill(codexSkill);

  const fallback = path.join(scratch, ".agent", "skills", "asc-code-dev-standards");
  assert(run().includes(fallback));
  checkSkill(fallback);

  const claude = path.join(scratch, ".claude", "skills", "asc-code-dev-standards");
  assert(run({ CLAUDE_CODE: "1" }).includes(claude));
  checkSkill(claude);

  const brokenHome = path.join(scratch, "broken-codex");
  const broken = path.join(brokenHome, "skills", "asc-code-dev-standards");
  fs.mkdirSync(path.dirname(broken), { recursive: true });
  fs.symlinkSync(path.join(scratch, "missing"), broken);
  run({ CODEX_HOME: brokenHome });
  assert(!fs.lstatSync(broken).isSymbolicLink());
  checkSkill(broken);

  const invalidPackage = path.join(scratch, "invalid-package");
  fs.mkdirSync(path.join(invalidPackage, "scripts"), { recursive: true });
  fs.copyFileSync(installer, path.join(invalidPackage, "scripts", "install-asc-code-dev-standards.js"));
  fs.copyFileSync(path.join(packageRoot, "package.json"), path.join(invalidPackage, "package.json"));
  const invalid = spawnSync(process.execPath, [path.join(invalidPackage, "scripts", "install-asc-code-dev-standards.js")], {
    cwd: scratch,
    env: { ...cleanEnv, CODEX_HOME: codexHome },
    encoding: "utf8",
  });
  assert.notStrictEqual(invalid.status, 0);
  assert(invalid.stderr.includes("Missing skill file"));
  checkSkill(codexSkill);
  console.log("Installer checks passed: priority, overwrite, fallback, Claude, broken symlink, and missing source.");
} finally {
  fs.rmSync(scratch, { recursive: true, force: true });
}
