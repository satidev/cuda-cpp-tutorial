# Tutorial on CUDA C++ programming and performance optimization.

# Building programs

The compilation of program requires a C++ compiler that supports C++ 23 and an NVIDIA CUDA compiler and toolkit that supports CUDA 13.3. Please check the official CUDA installation instructions for your operating system. It is to be noted that only NVIDIA GPUs of Turing, Ampere, Ada Lovelace, Hopper and Blackwell architectures support CUDA 13.3.

Currently, the programs has been compiled and tested only in Ubuntu 26.04 LTS. We will add installation 
instructions for the Windows operating system after testing.

## Ubuntu 26.04 LTS
Here are the installation instructions we followed for the laptop with RTX 3070 GPU (Ampere architecture) running Ubuntu 26.04 LTS. The following instructions are based on the assumption that you have access to a computer with freshly installed Ubuntu 26.04 LTS. Before executing the commands in the terminal right away,
read all installation instructions to determine the packages that you need to install (some packages may have already been installed), and the best option for
installation and program compilation if there are alternatives available.

### 1. Installing g++ compiler and other tools for building C++ programs

Verify whether the g++ compiler has already been installed:

```console
g++ --version
```

In Ubuntu 26.04 LTS, the following commands will install g++ 15.2 and other packages for building C/C++ programs
such as make:

```console
sudo apt update
sudo apt install build-essential
```

### 2. Installing cmake (4.2.3)

```console
sudo apt-get install cmake
```
### 3. Installing NVIDIA driver

The minimum driver version required for CUDA 13.x is R580. You can install the driver with the following commands. 

```console
ubuntu-drivers devices
# Make sure that the recommended driver version is greater than 580.0
sudo ubuntu-drivers install
```
Please reboot after installing the driver.

### 4. Installing gedit for text editing

```console
sudo apt update
sudo apt install gedit
```

### 5. Installing CUDA Toolkit 13.3

Follow the installation instructions listed in
[CUDA website](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=26.04&target_type=deb_network).


After installation, verify with the following command:
```console
nvcc --version
```
If the nvcc version is not displayed, the following environment variables are needed to
set by editing the .bashrc file located in $HOME directory.<br />
export CUDA_HOME=/usr/local/cuda-13.3<br />
export PATH=$CUDA_HOME/bin:$PATH<br />
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH<br />

```console
cd $HOME
gedit .bashrc
```
Add the above two lines to set the environment variables and close the file.
To apply the change in the current session:
```console
source ~/.bashrc
```
And try again to check the nvcc version:
```console
nvcc --version
```
For the above command, you may see similar output as following:
```console
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2026 NVIDIA Corporation
Built on Fri_Apr_24_07:22:02_PM_PDT_2026
Cuda compilation tools, release 13.3, V13.3.33
Build cuda_13.3.r13.3/compiler.37862127_0
```
After the above successful configurations, please make sure that the following two commands prints the full path to the nvcc executable:
```console
which nvcc
echo $CUDACXX
```
If not, set the environment variable CUDACXX by adding the following line the /etc/environment file.<br />
CUDACXX=/usr/local/cuda-13.3/bin/nvcc<br />

```console
sudo gedit /etc/environment
```
Add the above line to set the environment variable and close the file.

### 6. Installing git

```console
sudo apt-get install git-all
```

### 7. Download source files and build program
```console
git clone --recursive https://github.com/satidev/cuda-cpp-tutorial.git
cd cuda-cpp-tutorial
mkdir build
cd build
cmake ..
make 
```
Alternatively (also ideally!), popular IDEs like [Visual Studio Code](https://code.visualstudio.com/) can be used to edit source files and build program. 

#### Installation of VS Code and necessary and optional extensions
1. Install Visual Studio Code (via Ubuntu Software or from the link https://code.visualstudio.com/).
2. Launch VS Code.
3. Install C/C++ extension from Microsoft (click Extensions and search for C/C++).
4. Install CMake Tools extension from Microsoft.
5. Optional: install Nsight Visual Studio Code Edition from NVIDIA.

#### Compilation and program build in visual studio code
1. Clone git repository recursively to a local directory.
```console
mkdir cuda-cpp-tutorial
cd cuda-cpp-tutorial
git clone --recursive https://github.com/satidev/cuda-cpp-tutorial.git
```
2. Launch VS Code.
3. Click File -> Open Folder and select the directory where the main CMakeLists.txt exists.
   (cuda-cpp-tutorial)
4. Select host compiler (e.g., GCC 15.2 )
5. Make sure that the configuration is successful and build files have been written to build 
   directory.
6. Build the library and program: click Terminal -> Run Build Task -> CMake: build.



