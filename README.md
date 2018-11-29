# Homomorphic-Encryption
Homomorphic Encryption test based on HElib.

## Build
This project depends on [HElib](https://github.com/shaih/HElib), which itself depends on [GMP](https://gmplib.org/) and [NTL](https://www.shoup.net/ntl/) libraries.

Here we install all dependencies in the project's `ext` directory.
 
For complete building instructions, please refer to HElib's [document](http://shaih.github.io/HElib/).

### Build GMP
```shell
cd gmp-6.1.2/
./configure --prefix=$EXT_ABSOLUTE_PATH
make && make install
```

### Build NTL
```
cd ntl-11.3.2/src
./configure NTL_THREADS=on NTL_THREAD_BOOST=on NTL_EXCEPTIONS=on PREFIX=$EXT_ABSOLUTE_PATH
make && make install
```

### Build HElib
```
export CPLUS_INCLUDE_PATH=$EXT_ABSOLUTE_PATH/include
export LD_LIBRARY_PATH=$EXT_ABSOLUTE_PATH/lib
cd HElib/src
make
```

### Build This Project
```
mkdir build && cd build/
cmake ..
make
```
