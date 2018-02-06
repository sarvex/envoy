# Steps to build on Windows
### Prerequisites
* Visual Studio 2017
    * Install VS2017 and the latest latest updates.
        * For build machines, installing the build tools. https://www.visualstudio.com/downloads/#other
        * For development, Community edition should suffice.
* CMake

Following are required to build boringssl. See https://boringssl.googlesource.com/boringssl/+/HEAD/BUILDING.md for details about the versions.
* Perl
* Yasm
* Ninja
* Go

### Steps to build

#### Open a Windows Command prompt
    Run vcvars64.bat
    Normally under - path-to-visual-studio-root\VC\Auxiliary\Build\vcvars64.bat"
#### Make sure following are installed and can be found on the path
    1. perl.exe
    2. ninja.exe
    3. go.exe
    4. yasm.exe
        save downloaded yasm binary as yasm.exe
        yasm will require msvcr100.dll. If you see an error indicating that msvcr100.dll is missing using following link to install x86 redistributables.
        https://www.microsoft.com/en-us/download/details.aspx?id=26999
    5. CMake
#### Set the following variable
    set CMAKE_ASM_NASM_COMPILER=yasm.exe
#### Set the following variable to the path the folder that should be used for downloading and building the dependencies.
    ENVOY_DEPENDENCIES_ROOT=_path-to-use-for-fetching-and-building-dependencies_
#### Fetch and build dependencies by running batch script
    envoy_deps.cmd %ENVOY_DEPENDENCIES_ROOT%
    example: envoy_deps.cmd d:\work\envoy_dependencies
#### Create folder for CMake cache and cd to it
    Example: 
        mkdir d:\work\envoy_build
        cd d:\work\envoy_build
#### Create CMake cache
    cmake path-to-envoy-enlistment -DENVOY_DEPENDENCIES_ROOT=%ENVOY_DEPENDENCIES_ROOT%  -DVCPKG_TARGET_TRIPLET=x64-windows-static -G "Visual Studio 15 2017 Win64"
    Example:
        cmake d:\work\envoy -DENVOY_DEPENDENCIES_ROOT=%ENVOY_DEPENDENCIES_ROOT%  -DVCPKG_TARGET_TRIPLET=x64-windows-static -G "Visual Studio 15 2017 Win64"
#### Build using cmake
    cmake --build . -- -m


