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
assert 0 "0;"
assert 1 "1;"
assert 32 "32;"
#足し算引き算
assert 21 "5+20-4;"
#スペースの除去
assert 41 " 12 + 34 - 5 ;"
#四則演算・括弧
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
#単項演算子
assert 10 '-10+20;'
assert 75 '-(12+3)*-5;'
#比較演算子
assert 1 '1==1;'
assert 0 '3+2!=5;'
assert 1 '2*2 > 3;'
assert 0 '5*5<=5+5;'
#1文字変数
assert 5 'a=2;b=3;a+b;'
assert 144 'x=5;y=x+2;z=x+y;(x+y)*z;'
#変数
assert 5 'hoge=2;fuga=3;hoge+fuga;'
assert 18 'aaa=10;bbb=aaa*2;ccc=bbb/aaa;bbb-ccc;'
#return文
assert 4 'return 4;return 1;return 2;'
assert 14 'a=3;b=5*6-8;return a+b/2;'
#if文
assert 76 'if(11>3) return 76;return 38;'
assert 4 'a=3;b=4;if(a*a==b)return a;return b;'
assert 3 'a=3;b=9;if(a*a==b)return a;return b;'
echo OK