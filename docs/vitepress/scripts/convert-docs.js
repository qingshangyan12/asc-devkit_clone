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

  const args = [
    '-c',
    `import sys; sys.path.insert(0, '${scriptsDir}'); from mdparser.cli import main; main()`,
    input,
    '-o', output,
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
