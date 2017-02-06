Attempting to use ITK b-spline registration in Matlab.

Use:
```
cd bin
cmake ..
make && make install
```
In Matlab go to the output directory
```
[registeredImage, displacementField] = itkBplineReg(imageMoving,imageStatic);
```
