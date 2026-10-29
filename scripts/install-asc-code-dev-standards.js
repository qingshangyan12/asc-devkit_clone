#!/usr/bin/env node
"use strict";

const fs = require("fs");
const os = require("os");
const path = require("path");
const { spawnSync } = require("child_process");

const skillName = "asc-code-dev-standards";
const packageRoot = path.resolve(__dirname, "..");
const config = require(path.join(packageRoot, "package.json"));
const source = path.join(packageRoot, config.ascCodeStandardsSource);

function commandExists(command) {
  return spawnSync(command, ["--version"], { stdio: "ignore" }).status === 0;
}

function selectDestination() {
  const home = os.homedir();
  const projectHas = (name) => fs.existsSync(path.join(process.cwd(), name));
  const homeHas = (name) => fs.existsSync(path.join(home, name));
  if (process.env.CODEX_HOME || process.env.OPENAI_API_KEY || homeHas(".codex") || commandExists("codex")) {
    return path.join(process.env.CODEX_HOME || path.join(home, ".codex"), "skills");
  }
  if (process.env.CLAUDE_CODE || process.env.ANTHROPIC_API_KEY || projectHas(".claude") || homeHas(".claude") || commandExists("claude")) {
    return path.join(home, ".claude", "skills");
  }
  if (process.env.OPENCODE_DIR || projectHas(".opencode") || homeHas(".opencode") || commandExists("opencode")) {
    return path.join(home, ".opencode", "skills");
  }
  if (process.env.HERMES_HOME || homeHas(".hermes") || commandExists("hermes")) {
    return path.join(process.env.HERMES_HOME || path.join(home, ".hermes"), "skills");
  }
  return path.join(home, ".agent", "skills");
}

function exists(target) {
  try {
    fs.lstatSync(target);
    return true;
  } catch (error) {
    if (error.code === "ENOENT") return false;
    throw error;
  }
}

function install() {
  for (const file of ["SKILL.md", "references/development-standard.md", "references/cpp-development-standard.md"]) {
    if (!fs.existsSync(path.join(source, file))) throw new Error(`Missing skill file: ${file}`);
  }
  const root = path.resolve(selectDestination());
  const destination = path.join(root, skillName);
  if (exists(destination)) {
    try {
      if (fs.realpathSync(destination) === fs.realpathSync(source)) {
        console.log(`${skillName}: already available at ${destination}`);
        return;
      }
    } catch (error) {
      if (error.code !== "ENOENT") throw error;
    }
  }
  fs.mkdirSync(root, { recursive: true });
  const staging = fs.mkdtempSync(path.join(root, `.${skillName}-`));
  const incoming = path.join(staging, "incoming");
  const previous = path.join(staging, "previous");
  let previousMoved = false;
  let preserveStaging = false;
  try {
    fs.cpSync(source, incoming, { recursive: true });
    if (exists(destination)) {
      fs.renameSync(destination, previous);
      previousMoved = true;
    }
    try {
      fs.renameSync(incoming, destination);
    } catch (error) {
      if (previousMoved) {
        try {
          fs.renameSync(previous, destination);
        } catch (restoreError) {
          preserveStaging = true;
          throw new Error(`Install failed: ${error.message}. Previous installation retained at ${previous}: ${restoreError.message}`);
        }
      }
      throw error;
    }
  } finally {
    if (!preserveStaging) fs.rmSync(staging, { recursive: true, force: true });
  }
  console.log(`${skillName}: installed or updated at ${destination}`);
}

try {
  if (process.argv.length > 2) throw new Error("This installer takes no arguments.");
  install();
} catch (error) {
  console.error(`Installation failed: ${error.message}`);
  process.exitCode = 1;
}
