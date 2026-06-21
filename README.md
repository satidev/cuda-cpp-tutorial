# Tutorial on CUDA C++ Programming and Performance Optimization

## Building Programs

Compilation requires a C++ compiler that supports C++23 and an NVIDIA CUDA compiler and toolkit that supports CUDA 13.3. Please follow the official [CUDA installation instructions](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/) for your operating system. Note that only NVIDIA GPUs of Turing, Ampere, Ada Lovelace, Hopper, and Blackwell architectures support CUDA 13.3.

Currently, the programs have been compiled and tested only on Ubuntu 26.04 LTS. Installation instructions for Windows will be added after testing.

## Ubuntu 26.04 LTS

Here are the installation instructions we followed for a laptop with an RTX 3070 GPU (Ampere architecture) running [Ubuntu 26.04 LTS](https://ubuntu.com/download/desktop). The following instructions assume that you have access to a computer with a freshly installed Ubuntu 26.04 LTS. Before executing the commands in the terminal, read all installation instructions to determine which packages you need to install (some may already be installed) and the best option for installation and program compilation where alternatives are available.

### 1. Installing the g++ Compiler and Other Build Tools

Verify whether the g++ compiler is already installed:

```console
g++ --version
```

In Ubuntu 26.04 LTS, the following commands will install g++ 15.2 and other packages for building C/C++ programs, such as `make`:

```console
sudo apt update
sudo apt install build-essential
```

### 2. Installing CMake (4.2.3)

```console
sudo apt-get install cmake
```

### 3. Installing the NVIDIA Driver

The minimum driver version required for CUDA 13.x is R580. You can install the driver with the following commands:

```console
ubuntu-drivers devices
# Make sure that the recommended driver version is greater than 580.0
sudo ubuntu-drivers install
```

Please reboot after installing the driver. For more information, see the [NVIDIA driver installation guide](https://docs.nvidia.com/datacenter/tesla/driver-installation-guide/).

### 4. Installing gedit for Text Editing

```console
sudo apt update
sudo apt install gedit
```

### 5. Installing CUDA Toolkit 13.3

Follow the installation instructions on the
[CUDA Downloads page](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=26.04&target_type=deb_network).

After installation, verify with the following command:

```console
nvcc --version
```

If the `nvcc` version is not displayed, the following environment variables need to be set by editing the `.bashrc` file located in the `$HOME` directory:

```
export CUDA_HOME=/usr/local/cuda-13.3
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```

```console
cd $HOME
gedit .bashrc
```

Add the above three lines to set the environment variables and close the file. To apply the changes in the current session:

```console
source ~/.bashrc
```

Then verify the `nvcc` version again:

```console
nvcc --version
```

You should see output similar to the following:

```console
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2026 NVIDIA Corporation
Built on Fri_Apr_24_07:22:02_PM_PDT_2026
Cuda compilation tools, release 13.3, V13.3.33
Build cuda_13.3.r13.3/compiler.37862127_0
```

After the above successful configuration, make sure the following commands print the full path to the `nvcc` executable:

```console
which nvcc
echo $CUDACXX
```

If not, set the `CUDACXX` environment variable by adding the following line to `/etc/environment`:

```
CUDACXX=/usr/local/cuda-13.3/bin/nvcc
```

```console
sudo gedit /etc/environment
```

Add the above line, then save and close the file.

### 6. Installing Git

```console
sudo apt-get install git-all
```

### 7. Downloading Source Files and Building the Program

```console
git clone --recursive https://github.com/satidev/cuda-cpp-tutorial.git
cd cuda-cpp-tutorial
mkdir build
cd build
cmake ..
make
```

Alternatively (and ideally), a popular IDE such as [Visual Studio Code](https://code.visualstudio.com/) can be used to edit source files and build the program.

#### Installing VS Code and Required/Optional Extensions

1. Install [Visual Studio Code](https://code.visualstudio.com/) via Ubuntu Software or directly from the website.
2. Launch VS Code.
3. Install the **C/C++** extension from Microsoft (click **Extensions** and search for `C/C++`).
4. Install the **CMake Tools** extension from Microsoft.
5. Optional: install the **Nsight Visual Studio Code Edition** extension from NVIDIA.

#### Compilation and Program Build in Visual Studio Code

1. Clone the git repository recursively to a local directory:

```console
mkdir cuda-cpp-tutorial
cd cuda-cpp-tutorial
git clone --recursive https://github.com/satidev/cuda-cpp-tutorial.git
```

2. Launch VS Code.
3. Click **File → Open Folder** and select the directory containing the main `CMakeLists.txt` (`cuda-cpp-tutorial`).
4. Select the host compiler (e.g., GCC 15.2).
5. Confirm that the configuration is successful and that build files have been written to the `build` directory.
6. Build the library and program: click **Terminal → Run Build Task → CMake: build**.
