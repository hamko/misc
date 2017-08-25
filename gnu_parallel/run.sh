ls ./* | parallel -j 10 "echo {}.org {/}.dir_cut"

g++ -std=c++11 -o a.out template.cpp 
time seq 1000 | parallel -j 1 ./a.out > tmp
time seq 1000 | parallel -j 10 ./a.out > tmp

# N行ずつ分解してwcに食わせる
seq 1000000 |  parallel --pipe -N100000 wc

# 1000行のtmpファイルを、100行ずつに分けて総和をとる
cat tmp | parallel --pipe -N100 "awk '{n+=\$1}END{print n}'" 

#グリッドサーチ
lr=(0 0.05 0.10)
units=(100 200 300)
parallel "./a.out {1} {2} > {1}_{2}.log" \
     ::: ${lr[@]} ::: ${units[@]}
