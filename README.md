#RK4Benchmark

This is a very simple microbenchmark of a naive implementation of the ["classic" 4th Order Runge-Kutta Method](http://en.wikipedia.org/wiki/Runge%E2%80%93Kutta) for solving differential equations.

I wanted to play with Haskell, and I wanted to play with Runge-Kutta, and I wanted to see how it stacked up compared to C.

This is an OS X benchmark.  The build script uses clang for the C algorithm, and the benchmark timing stuff uses some Mach headers.

## Requirements/Dependencies

Requires OS X, a modern GHC (7.6 or higher)/Haskell Platform, and Cabal/cabal-install >= 1.18.0 (project is built around the new cabal sandbox).

## Building

`cd` in to the `hsrc` directory, run `cabal sandbox init`.

Go back to the main project directory, make sure `buildAll.sh` is executable, and then run it.  It will use cabal to pull in stream-fusion and criterion in to the sandbox and then build the binary for the benchmark, which will be under `hsrc/.cabal-sandbox/bin`.

It will also build the C benchmark, which will be under `csrc/`.

## Running

Invoke them individually, or make sure the executable bit is flipped on `runAll.sh` and just run that.

## Motivation

This isn't meant to be a straight up comparison of Haskell vs. C.  Amortized over all algorithms and all time, I'm fairly certain that C will always be the king of speed.  I just wanted to try implementing a straight-forward, by-the-book example of RK4.  No hand-optimizing of the algorithm itself, just turning the equations in to code and then seeing how fast it could be under the stewardship of a high-level language vs. the grit of a low-level language. The only "optimizations" that were done to the Haskell code was in using [Stream Fusion](http://hackage.haskell.org/package/stream-fusion) to tune out allocation/GC overhead from [thunking](http://www.haskell.org/haskellwiki/Thunk) so that the only thing really being benchmarked was how fast the actual RK algorithm itself was running.

## The Benchmark

The benchmark itself simply applies the RK4 method to the relatively trivial Initial Value Problem

    dy/dt = -y; y(0) = 1.0

The closed form solution of this ODE IVP gives `y(t) = e^-t`, if you feel the need to verify that the results calculated by the RK method are correct.

The benchmark uses a timestep of `h = 1e-3`, or one millisecond.  It moves through 1e7 steps, or 10000 seconds. It does this 100 times, and measures the minimum, maximum, and average run times as well as the standard deviation (Criterion also puts an upper bound and lower bound on sigma; I'm not 100% sure what this means but I assume it's some sort of jitter measurement).

I use `-O2` optimizations with clang and GHC.

## Results

Currently, the Haskell implementation is actually slightly faster.  Only very slightly, though.

Anecdotally, I've come to this take-away:  The optimizations required to make the Haskell code run just as fast as the C code were trivial, not time consuming, easy to understand, and most importantly: If you already grok the Haskell syntax, then understanding what the optimizations are doing is relatively trivial.  Compared to the sort of bit fiddling, pointer slinging wizardry that goes in to making C code crazy fast, I'd pick tuning Haskell to be like naive C over tuning C with sacrifices to the Old Ones any day of the week.  I quite simply have better things to do with my time than become a Artificer of the Arcane C Arts.

## Thanks and Acknowledgements

The RK4 algorithm was adapted directly from the equations as presented on Wikipedia (linked above).  The stream fusion optimization and nod to criterion were adapted from answers to [this Stack Overflow question](http://stackoverflow.com/questions/18578370/haskell-performance-struggling-with-utilizing-profiling-results-and-basic-tunin).

The monotonic time function used in the C code comes from [this Stack Overflow](http://stackoverflow.com/questions/5167269/clock-gettime-alternative-in-mac-os-x) question; OS X apparently doesn't have an implementation of `clock_gettime()`.

## License

This is licensed under the WTFPL:

<pre>
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
</pre>
