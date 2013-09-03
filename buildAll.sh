#!/bin/sh

cd hsrc
cabal-dev install
cd ..
cd csrc
clang RK.c -o RK4BenchmarkC
