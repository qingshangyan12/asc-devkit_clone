const mj3 = require('markdown-it-mathjax3');
const mdi = require('markdown-it');
const md = mdi();
md.use(mj3);
const result = md.render('\x24\x24dstReg_i = srcReg0_i + srcReg1_i\x24\x24');
console.log(result.substring(0, 2000));
