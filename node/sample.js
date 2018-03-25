// 文字列
{
    var s = "test";
    for (var i = 0; i < s.length; i++) {
        console.log(s[i]);
    }
}

// 配列
{
    var array = [0, 1, 2];
    // 添字, length
    for (var i = 0; i < array.length; i++) {
        console.log(array[i]);
    }
    // 表示
    console.log(array);

    // push_front
    array.unshift(3);
    // pop_front
    array.shift();

    // push_back
    array.push(4);
    // pop_back
    array.pop();

    array.forEach(v=>{
        console.log(v + "#value");
    });
    array.forEach((v, i, a)=>{
        console.log(v + "#index" + i + " " + a);
    });
}

// 辞書
{
    var array = Object();

    // いろんなアクセス方法がある
    array.hoge = 'huga';
    array['xx'] = 'yy';
    array[1] = 'one';
    array[2] = 'yeh';
    console.log(array);
    console.log(array['hoge']);
    console.log(array.hoge); // 重要！！！キーは.オペレータでもアクセスできる
    console.log(array[1]);
//    console.log(array.1); // error. 数字は.オペレータではアクセス不能

    // 長さのとり方がめんどい
    console.log(Object.keys(array).length + "#Dict length");

    // 無いものにアクセスすると、undefinedがかえる 
    console.log(array["donotexist"]); 

    // キーの削除はdelete
    console.log(array);
    delete array.hoge;
    delete array[1];
    console.log(array);

    // for in構文は、キーを回す！
    for (var i in array) {
        console.log(i + "# dict for");
    }
}

/*
// ダメな例。awaitはasync関数の中でしか使えない。
async function waitms(time) { setTimeout(() => { return time; }, time); }
async function fn(n) { console.log(n + "#numver"); return n; }
console.log(fn(2));
// var a = await fn(10); console.log(await a); // Error, await is only in an async func
*/

async function echoNumber(n) { console.log(n + "#number"); return n; }
// console.log(echoNumber(2)); // 何か出力されるけどこれはバグ！この時点でプロミスが解消されているとは限らない。
echoNumber(3).then(v=>{console.log(v);}); // asyncはプロミスを返すので、thenで簡易async関数内awaitができる。


// awaitが書かれた時点で、プロミスの解消(resolve)を待つ。
// async関数内部での「待つ」なので、メインポインタの性能には影響しない！うれしい！
//
// しかし、async関数の中ではやはり「待つ」ので、この関数の性能には影響する。
// 以下のように書くと、
// plus(0, 1)の結果が得られない限りplus(0, 2)に進まず、非効率になり得る。
//
// 0 -> +1  -> *2 -+-> ret
//   -> +2  -> *2
//   -> +3  -> *2 
{
    async function plus(x, y) { return x + y; }
    async function multiplyTwo(n) { return n * 2; }
    async function exec() {
        var one = await plus(0, 1);
        var two = await plus(0, 2);
        var three = await plus(0, 3);
        one = await multiplyTwo(one);
        two = await multiplyTwo(two);
        three = await multiplyTwo(three);
        return one + two + three;
    }

    // 非同期関数とメインを繋ぐところだけthenをスマート
    exec().then(v=>{console.log(v + "#test 1"); }); 
}

// 以下のように書くと、
// plus(0, 1), plus(0, 2), plus(0, 4)は平行に走るので効率的。
//
// 0 -> +1  -> *2 -+-> ret
//   -> +2  -> *2
//   -> +3  -> *2 
{
    async function plus(x, y) { return x + y; }
    async function multiplyTwo(n) { return n * 2; }
    async function exec() {
        var one = plus(0, 1);
        var two = plus(0, 2);
        var three = plus(0, 3);
        one = await multiplyTwo(await one);
        two = await multiplyTwo(await two);
        three = await multiplyTwo(await three);
        return one + two + three;
        console.log(one + two + three + "#test 2");
    }
    exec().then(v=>{console.log(v + "#test 2"); });
}

// 非効率なforを使ったawait
// 直列forのawait
//
// 0 -> +1  -> *2 -+-> ret
//   -> +2  -> *2
//   -> +3  -> *2 
{
    async function plus(x, y) { return x + y; }
    async function multiplyTwo(n) { return n * 2; }
    async function exec(n) {
        // plusを直列に処理する
        var ret = 0;
        for (var i = 0; i < n; i++) {
            var tmp = await plus(0, i);
            ret += await multiplyTwo(tmp);
        }
        ret += await(0); // 別にpromiseでないものにawaitしてもよい。意味ないけど。
        return ret;
    }
    exec(10).then(v=>{console.log(v + "#test for")});
}

// プロミスで全部集めはじめてからconcatする
// 並列forのawait
//
// 0 -> +1  -> *2 -+-> ret
//   -> +2  -> *2
//   -> +3  -> *2 
{
    async function plus(x, y) { return x + y; }
    async function multiplyTwo(n) { return n * 2; }
    async function exec(n) {
        // plusを並列に処理する
        plusPromise = [];
        for (var i = 0; i < n; i++) {
            plusPromise[i] = plus(0, i);
        }
        var ret = 0;
        for (var i = 0; i < n; i++) {
            ret += await multiplyTwo(await plusPromise[i]);
        }
        return ret;
    }
    exec(10).then(v=>{console.log(v + "#test promise for")});
}


// JSON
{
    // 読み込み
    var v = require('./test.json');
    v['hamko'] = 'hamster';
    console.log(v);
    const fs = require('fs');

    //    非同期関数なので、書き終わったらちゃんとコールバックをしないといけない！ 
    //    さもなくば(node:7812) [DEP0013] DeprecationWarning: Calling an asynchronous function without callback is deprecated.とかでる
    // fs.writeFile('./test_out.json', JSON.stringify(v, null, '    ')); 

    // ちゃんとerrがあったら投げ返す！
    fs.writeFile('./test_out.json', JSON.stringify(v, null, '    '), err=>{if (err) throw err; }); 

    // 望ましいこととしては、プロミス化してthen, catch
    const util = require('util');
    util.promisify(fs.writeFile)('./test_out2.json', JSON.stringify(v, null, '    ')).catch(err=>{if (err) throw err;});

    // promisifyした時のthenの無名関数の引数は、err以外の全部が入ったオブジェクト1つになる
    const exec = require('child_process').exec;
    util.promisify(exec)('ls').then((std)=>{console.log(std.stdout+"#stdout\n"+std.stderr+"#stderr");}).catch(err=>{if(err)console.log("no such file");});
    util.promisify(exec)('grep hamster . -r').then((std)=>{console.log(std.stdout+"#stdout\n"+std.stderr+"#stderr");}).catch(err=>{if(err)console.log("no such file");});
    util.promisify(exec)('cat ./nosuchfile.json').then((std)=>{console.log(std.stdout+"#stdout\n"+std.stderr+"#stderr");}).catch(err=>{if(err)console.log("no such file");});
}


// 変な型
// [Object object]はJSON.stringfy(obj);しておけばよい。
// <BufferはtoString();しておけば良い。
{
    const execSync = require('child_process').execSync;
    const result =  execSync('ls -la ./');
    console.log(result); // <Buffer
    console.log(result.toString());

    const util = require('util');
    const exec = require('child_process').exec;
    util.promisify(exec)('grep hamster . -r').then((std)=>{console.log(std + "Strange!!!!!!!!!!!!");}).catch(err=>{if (err) console.log("no such file");}); // [Object object]
    util.promisify(exec)('grep hamster . -r').then((std)=>{console.log(JSON.stringfy(std));}).catch(err=>{if (err) console.log("no such file");});
}
