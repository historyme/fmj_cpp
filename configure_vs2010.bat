mkdir build\win32
mkdir install\win32
mkdir release\win32
cd build\win32\
cmake.exe -G "Visual Studio 10 2010" --build ..\..\
pause