// 全体的な注意
// 基本的に逐次処理にしたほうがバグらないのでそっちを使う。

"use strict"

const fs = require('fs');


// コマンド操作
const exec = require('child_process').execSync;
// exec("command").toString()で、標準出力を返す
console.log(exec('cd ..; pwd; cd - > /dev/null').toString());
// 標準出力を出す。2>&1; trueをつけると標準出力が得られる。
console.log(exec('ls donotexist.txt      2>&1; true').toString());

const spawn = require('child_process').spawnSync;
obj = spawn();
console.log(');
