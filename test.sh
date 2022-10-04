#!/bin/bash
assert(){
    expected="$1"
    input="$2"
    ./0cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}
#整数を返す
assert 0 0
assert 1 1
assert 32 32
#足し算引き算
assert 21 "5+20-4"
#スペースの除去
assert 41 " 12 + 34 - 5 "
#四則演算・括弧
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
#単項演算子
assert 10 '-10+20'
assert 75 '-(12+3)*-5'
echo OK