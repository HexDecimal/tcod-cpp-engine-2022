# How to setup

* Make sure you have the correct tools ready.
  * For example on Windows:
    Make sure you install the C++ compiler tools from [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/),
    install the latest [CMake](https://cmake.org/download/),
    and install [Visual Studio Code](https://code.visualstudio.com/download).
* [Clone this repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository-from-github/cloning-a-repository).
* Make sure to initilize the `vcpkg` [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules).  If the `vcpkg` folder is empty then the build process will fail.
  * The command to do this is: `git submodule update --init --recursive`
* Load this project in VS Code and install any recommended plugins.
  Most importantly: *C/C++*, *C++ Intellisense*, and *CMake Tools*.
* Using the CMake Tools plugin you can now build and run this project.  Dependences will automatically be installed via Vcpkg.
  * On Windows, when CMake Tools asks for a toolkit you should select `Visual Studio Community ... - amd64`.
  * When CMake Tools asks for a project select `libtcod-vcpkg-template`.
* The project will now build and run with F5.
  You should edit the `CMakeLists.txt` script as needed to expand your project.
