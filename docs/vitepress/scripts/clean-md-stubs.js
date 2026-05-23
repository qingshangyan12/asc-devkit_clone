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
