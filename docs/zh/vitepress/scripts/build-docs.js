/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * Licensed under the CANN Open Software License Agreement Version 2.0.
 */

const { spawnSync } = require('node:child_process')
const { resolve } = require('node:path')

const root = resolve(__dirname, '..')
const scriptsDir = resolve(root, 'scripts')
const vitepressCli = resolve(root, 'node_modules', 'vitepress', 'bin', 'vitepress.js')
const prebuildScripts = [
  'install-deps.js',
  'convert-docs.js',
  'gen-md-stubs.js',
  'copy-api-sources.js',
]

function run(command, args, extraEnv = {}) {
  const result = spawnSync(command, args, {
    cwd: root,
    env: { ...process.env, ...extraEnv },
    stdio: 'inherit',
  })
  if (result.error) throw result.error
  return result.status ?? 1
}

let exitCode = 1
try {
  exitCode = 0
  for (const script of prebuildScripts) {
    exitCode = run(process.execPath, [resolve(scriptsDir, script)])
    if (exitCode !== 0) break
  }
  if (exitCode === 0) {
    const nodeOptions = [
      process.env.NODE_OPTIONS,
      '--max-old-space-size=14336',
    ].filter(Boolean).join(' ')
    exitCode = run(
      process.execPath,
      [vitepressCli, 'build', 'docs'],
      { NODE_OPTIONS: nodeOptions }
    )
  }
} catch (error) {
  console.error('[docs:build] Build process failed:', error)
  exitCode = 1
} finally {
  try {
    const cleanupCode = run(process.execPath, [resolve(scriptsDir, 'clean-md-stubs.js')])
    if (exitCode === 0 && cleanupCode !== 0) exitCode = cleanupCode
  } catch (error) {
    console.error('[docs:build] Cleanup failed:', error)
    if (exitCode === 0) exitCode = 1
  }
}

process.exitCode = exitCode
