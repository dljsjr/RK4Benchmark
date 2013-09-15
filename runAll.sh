#!/bin/sh

cd hsrc/.cabal-sandbox/bin
./RK4BenchmarkHS
cd ../../../csrc
./RK4BenchmarkC
