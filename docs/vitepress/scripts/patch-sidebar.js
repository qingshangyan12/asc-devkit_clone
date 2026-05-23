const { readFileSync, writeFileSync } = require('fs')
const { resolve } = require('path')

const file = resolve(__dirname, '..', 'node_modules', 'vitepress', 'dist', 'client', 'theme-default', 'composables', 'sidebar.js')

let content = readFileSync(file, 'utf-8')

content = content.replace(
  /\(isActiveLink\.value \|\| hasActiveLink\.value\) && \(collapsed\.value = false\);/g,
  '// auto-expand disabled by postinstall patch'
)

writeFileSync(file, content)
console.log('sidebar.js patched: auto-expand disabled')
