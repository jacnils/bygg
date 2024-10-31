# bygg

![action](https://github.com/jcbnilsson/bygg/actions/workflows/cmake-multi-platform.yml/badge.svg)

Component-based HTML/CSS builder for C++17.

## Features

- HTML and CSS document generation and deserialization
- Sensible indentation for pretty-formatting.
- Modern C++ API
- No dependencies, other than the standard library
- Windows, macOS, Linux and *BSD support

## Installation

To install the library, you can utilize the provided CMakeLists.txt file:

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
cmake --build .
cmake --install . --prefix /usr
```

This will install the library to /usr/lib and the headers to /usr/include/bygg.
If you're using macOS, you should use `/usr/local` as your prefix instead.

## Usage

Just include the appropriate headers in your project and link against the library. 
If you are not sure which header(s) are appropriate, you can include all of them by 
including bygg.hpp. If you know that you, for example, only need the HTML builder,
you can include HTML.hpp. Examples can be found in the examples directory.

## License

This project is licensed under the MIT license. See the included LICENSE file for more information.

## What about the name?

I am Swedish and I want to name projects in Swedish, because it's fun. Bygg means build, and I think build is a fitting
word for a project like this.
