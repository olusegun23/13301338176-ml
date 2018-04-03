# Performance Analysis Survey

This page will list summaries of existing techniques for performance analysis and optimization.  The purpose of this survey is to make an informed decision on how to design Paddle's performance benchmarking tools.

## Profiling tools

### Intel VTune

https://software.intel.com/en-us/intel-vtune-amplifier-xe

### NVidia Visual Profiler

https://developer.nvidia.com/nvidia-visual-profiler

## Survey of self-tuning or easily auto-tunable frameworks

### ATLAS BLAS - Automatically Tuned Linear Algebra Software

http://math-atlas.sourceforge.net/

There is a nice description of techniques used by ATLAS to provide optimized performance for many platforms on [Wikipedia](https://en.wikipedia.org/wiki/Automatically_Tuned_Linear_Algebra_Software).

The three principles mentioned are:
* Parametrization
* Multiple implementation
* Code generation

### FFTW - Fastest Fourier Transform in the West

http://www.fftw.org/

FFTW's approach for speeding up computation is three-fold (as mentioned in their [FAQ](http://www.fftw.org/faq/section4.html#howworks))
* At runtime, based on compiler and machine characteristics, this library creates a “plan” for FFT computation among different techniques.
* They use code generation to optimize small routines
* They optimize computation based on a machine’s memory hierarchy.

FFTW accumulates "wisdom". That is, it can be run in modes where it tries many different possible "plans" of computation and learns which are optimal for a given data size on that particular machine. It can store this knowledge for future reuse. See [this page](http://www.fftw.org/fftw3_doc/Words-of-Wisdom_002dSaving-Plans.html) for more information.

A large amount of engineering effort has gone into this package.  It is unlikely that Paddle contributors have the bandwidth to take a similar approach, since we want to make computation on a variety of CPU/GPU models faster, and we also have more operation types.  Ideally, we can rely on a library to make the decisions at runtime on which implementations to use.

### CUB - CUDA Unbound

https://github.com/NVlabs/cub

## Low-level IR -> Machine code

### Halide

### NNVM

https://github.com/dmlc/nnvm

NNVM provides an intermediate representation for high-level graph compilation (e.g. operator scheduling and memory management). An open question is whether we can use NNVM/TLM for optimization rather than re-inventing the wheel.  We should weigh whether the invasive change to Paddle worth the performance benefits across platforms.  While we could keep the interface for users the same, using NNVM in Paddle would require rewriting significant parts of the framework.  [This repository](https://github.com/tqchen/tinyflow) contains a tutorial on building a framework using NNVM.  They include a couple benchmarks in their [press release](http://www.tvmlang.org/2017/10/06/nnvm-compiler-announcement.html); we should use the library more and benchmark our primary use cases before seriously considering switching to using NNVM.

### TVM

https://github.com/dmlc/tvm

TVM supports many platforms (GPU, CPU, Raspberry Pi, smart phones, etc).  It contains libraries that produce optimized machine code as well as platform-agnostic wrappers around those kernels.  Using TVM would require adding an additional set of implementations (alongside the Cuda and CPU kernels) that use TVM's abstraction layer.  [This tutorial](https://github.com/dmlc/tvm/blob/master/tutorials/get_started.py) contains example code for a vector addition (in Python, though they have a C++ endpoint as well).  We may still want to implement our own graph-based optimizations on top of TVM if we use TVM and not NNVM.

### XLA
