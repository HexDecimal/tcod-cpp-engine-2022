# Libtcod C++ engine 2022

A modern roguelike engine loosely following the [Complete roguelike tutorial using C++ and libtcod](http://www.roguebasin.com/index.php?title=Complete_roguelike_tutorial_using_C%2B%2B_and_libtcod_-_part_1:_setting_up). As part of the [RoguelikeDev Does The Complete Roguelike Tutorial (2022) series](https://old.reddit.com/r/roguelikedev/wiki/python_tutorial_series).

## How to setup

* Make sure you have the correct tools ready.
  * For example on Windows:
    Make sure you install the C++ compiler tools from [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/),
    install the latest [CMake](https://cmake.org/download/),
    and install [Visual Studio Code](https://code.visualstudio.com/download).
* [Clone the repository](https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository-from-github/cloning-a-repository).
* Make sure to initialize the `vcpkg` and `libtcod` [submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules).  If either folder is empty then the build process will fail.
  * The command to do this is: `git submodule update --init --recursive`
* Load this project in VS Code and install any recommended plugins.
  Most importantly: *C/C++*, *C++ Intellisense*, and *CMake Tools*.
* Using the CMake Tools plugin you can now build and run this project.  Dependencies will automatically be installed via Vcpkg.
  * On Windows, when CMake Tools asks for a toolkit you should select `Visual Studio Community ... - amd64`.
* The project will now build and run with F5.

[![RoguelikeDev Does The Complete Roguelike Tutorial](https://i.imgur.com/FzwmyyE.png)](https://old.reddit.com/r/roguelikedev/wiki/python_tutorial_series)
