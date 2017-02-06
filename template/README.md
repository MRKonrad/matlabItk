Template to write ITK filters in Matlab.

Use:
```
cd bin
cmake ..
make && make install
```
In Matlab go to the output directory
```
b = template_matlabItk(ones(5),ones(5));
```
Inspired by <https://github.com/thewtex/matlab-cmake-hello-world/>
