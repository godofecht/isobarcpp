

To build and run the tests, please execute the following commands.

```bash
mkdir build
cd build
cmake ..
cmake --build . #This compiles and builds the tests
ctest -C Debug -V #This run the tests
```