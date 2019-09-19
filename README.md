# 如何编译和执行

本工程使用CMake来编译，CMake的好处是方便跨平台执行。关于CMake，可以访问(https://cmake.org/)。

在Linux环境下，请先创建build目录，并到目录build下，执行cmake
```bash
mkdir build && cd build
cmake ../
```

再执行
```bash
make
```

如果你的Linux中无法运行cmake或make命令，可以按照如下命令安装它
```bash
sudo apt-get install cmake g++ make
```

所有的可执行程序在目录build/bin下面


如果你有CLion或者其他可视化的IDE，可以直接在IDE中执行"Build Project"来完成整个项目的编译。


