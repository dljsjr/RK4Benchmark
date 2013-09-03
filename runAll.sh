#!/bin/sh

cd hsrc/cabal-dev/bin
./RK4BenchmarkHS
cd ../../../csrc
./RK4BenchmarkC
