# UICore SDK
UICore is a C++ toolkit library focusing on user interfaces and graphics:
* Features a Model/View/Controller (MVC) design for building cross platform UI applications
* Uses modern CSS principles for layout such as flex-box and CSS properties for styling
* No javascript. No UI worker processes as needed when using WebKit 2 or Chromium. No binding code. No HTML legacy.
* Hi-DPI support built-in. Rendering and layout are done using device independent pixels
* Fast hardware accelerated 2D canvas
* Low-level cross platform OpenGL/Direct3D abstraction for custom 3D or specialized 2D operations
* Direct3D 11 and OpenGL 4 support
* Designed for modern C++ (C++11, C++14)
* Many useful matrix, vector and other graphics related math classes
* Various commonly useful utility classes such as network, XML, JSON, file and zip (deflate) classes
* Supports Windows, Linux, BSD and Mac OS X

## Building and Installing UICore
UICore uses a configure application which generates project templates for the various platforms supported. Basically same principle as CMake, except the solutions/workspaces generated for Visual Studio and Xcode are less terrible (from our point of view!).

The build instructions for each platform are as follows:
- *Windows*: Run configure.exe, select the configuration types you're interested in and enter the correct header/lib search and install paths. If you're not fully comfortable with opening configure.exe, you can build it yourself by opening Setup.sln first in Setup/Win32.
- *Linux*: Run ./configure. Make sure you have GCC installed first. This will generate a CMakeLists.txt file and a KDevelop project. You can either then use CMake from the command line to build and install, or you can use the KDevelop project it generated.
- *OS X*: Run ./configure. Make sure you have Xcode and the command line tools installed first. This will generate a Xcode project that you can use to build from the command line or from within Xcode. It will output the include and library files into the Build directory after a successful build.
 
Only the Linux build has external dependencies at the moment. CMake should detect those and complain if they aren't installed. If you find any bugs in our build scripts, please help out by sending a pull request.

## Documentation
The latest build of the documentation can be found at https://dpjudas.github.io/UICore

You can also build it manually by running doxygen from the Documentation folder.
