const { existsSync, unlinkSync, readFileSync } = require('node:fs')
const { resolve } = require('node:path')

const manifestFile = resolve(__dirname, '..', '.stubs-manifest.json')
if (!existsSync(manifestFile)) {
  console.log('No stubs manifest found, nothing to clean')
  process.exit(0)
}

const stubs = JSON.parse(readFileSync(manifestFile, 'utf-8'))
let removed = 0
for (const stub of stubs) {
  if (existsSync(stub)) {
    unlinkSync(stub)
    removed++
  }
}

unlinkSync(manifestFile)
console.log(`Cleaned up ${removed} .md stubs`)
