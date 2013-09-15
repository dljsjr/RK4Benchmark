#!/bin/sh

cd hsrc
cabal install
cd ..
cd csrc
clang -O2 RK.c -o RK4BenchmarkC
