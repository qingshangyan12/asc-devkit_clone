/**
 * Copyright (c) 2026 Example Corp. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


const { spawnSync } = require('node:child_process')
const { resolve, join } = require('node:path')
const { cpSync, existsSync } = require('node:fs')

const scriptsDir = resolve(__dirname)
const root = resolve(scriptsDir, '..')
const repoRoot = resolve(root, '..', '..')

function runMdparser(inputDir, outputDir, label) {
  console.log(`Converting ${label}...`)
  const input = resolve(repoRoot, 'docs', inputDir)
  const output = resolve(root, 'docs', outputDir)
  const scriptsDirUnix = scriptsDir.replace(/\\/g, '/')
  const inputUnix = input.replace(/\\/g, '/')
  const outputUnix = output.replace(/\\/g, '/')

  const args = [
    '-c',
    `import sys; sys.path.insert(0, '${scriptsDirUnix}'); from mdparser.cli import main; main()`,
    inputUnix,
    '-o', outputUnix,
    '-r',
  ]

  const result = spawnSync('python3', args, {
    cwd: root,
    stdio: 'inherit',
    encoding: 'utf-8',
  })

  if (result.status !== 0) {
    console.error(`Failed to convert ${label}`)
    process.exit(result.status || 1)
  }
}

runMdparser('api', 'api', 'API docs')
runMdparser('guide', 'guide', 'Guide docs')

const srcFigures = resolve(repoRoot, 'docs', 'figures')
const dstFigures = resolve(root, 'docs', 'figures')
if (existsSync(srcFigures)) {
  console.log(`Copying figures/...`)
  cpSync(srcFigures, dstFigures, { recursive: true })
  console.log(`  ${srcFigures} -> ${dstFigures}`)
}

console.log('Conversion complete')
