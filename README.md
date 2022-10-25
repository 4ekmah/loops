# Loops

This is instant cross-platform jit engine inspired by Xbyak

# Loopslayers

Small library of of generators for neural network layers.

Currently has :
1.) Depthwise convolution with padding. No strides, no dilations. Activatons: none, ReLU, ReLU6, LeakyReLU.

In progress:
1.) Maxpooling

## How to build loopslayers

Needs C++ compiler and cmake. 

```
cd <build_dir>
cmake <loops_dir> .
make
```

Library built can be linked to other project via appending include path <loops_dir>\include, link library path <build_dir>\ and linking <build_dir>\libloopslayers.a and <build_dir>\libloops.a

(Or .lib, if your system is Windows)