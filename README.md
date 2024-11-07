# bygg

![action](https://github.com/jcbnilsson/bygg/actions/workflows/cmake-multi-platform.yml/badge.svg)

Component-based HTML/CSS builder for C++17.

## Features

- HTML and CSS document generation and deserialization
- Component-based, modular design
- C++ pseudo-code generation
- Easy to use, with a modern C++ API
- Iterator- and reference-based design for easy manipulation and traversal
- Sensible indentation for pretty-formatting.
- HTML serialization*
- No dependencies, other than the standard library*
- Windows, macOS, Linux and *BSD support**

*Serialization requires the use of libxml2, which is not included in this repository.
Tests require the use of Catch2, which is not included in this repository.

**Not tested on BSD. bygg is actively tested on Windows, macOS and Linux through GitHub Actions.

## Dependencies

- C++17 compiler
- CMake
- libxml2 (*optional*, for serialization)
- Catch2 (*optional*, for tests)

## Installation

To install the library, you can utilize the provided CMakeLists.txt file:

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
cmake --install . --prefix /usr
```

This will install the library to `/usr/local/lib` and the headers to `/usr/local/include/bygg/...`
If you're packaging the library, `/usr` is probably a more suitable prefix.

If you don't need serialization, you can disable it along with the libxml2
dependency by passing `-DBYGG_USE_LIBXML2=OFF` to CMake. This will disable the
serialization features of the library and not install the headers for it.

If you want to enable the tests, you can pass `-DCMAKE_BUILD_TYPE=Debug` to 
CMake, and they will automatically be built.

## Usage

Just include the appropriate headers in your project and link against the library. 
If you are not sure which header(s) are appropriate, you can include all of them by 
including bygg.hpp. If you know that you, for example, only need the HTML builder,
you can include HTML.hpp. Examples can be found in the `examples` directory.

Hello world example:

```cpp
#include <fstream>
#include <bygg/HTML/HTML.hpp>
#include <bygg/CSS/property.hpp>

int main() {
   using namespace bygg::HTML;
   using CSSProperty = bygg::CSS::Property;

   Section sect{Tag::Html, make_properties(Property{"lang", "us"}), SectionList{
       Section{Tag::Head, ElementList {
           Element{Tag::Title, "Hello world!"},
           Element{Tag::Meta, make_properties(Property{"name", "description"}, Property{"content", "Hello world description!"})},
           Element{Tag::Meta, make_properties(Property{"name", "viewport"}, Property{"content", "width=device-width, initial-scale=1.0"})},
       }},
       Section{Tag::Body, SectionList {
       Section{Tag::Div, make_properties(Property{"class", "content"}), ElementList{
           Element{Tag::H1, "Hello world!"},
           Element{Tag::P, make_properties(Property{"style", CSSProperty("color", "red").get()}), "This is a red string of text."},
       }},
       }},
       Section{Tag::Footer, ElementList {
           Element{Tag::P, "This is a paragraph in a footer."},
       }},
   }};

   std::ofstream ofs("hello-world.html");

   ofs << sect.get<std::string>(Formatting::Pretty);

   ofs.close();
}
```

You can see that the library is heavily designed around avoiding strings as much as possible, in favor of
objects that represent the data you want to work with. This makes it easier to manipulate and traverse.
But if you find that you need to work with strings in your sections or elements, you can always use strings
to create them.

See the Doxygen documentation in `docs` for classes and functions that are exposed by the library.

## License

This project is licensed under the MIT license. See the included LICENSE file for more information.

The dependency `libxml2` is licensed under the MIT license and maintained by the GNOME project.
For issues regarding `libxml2`, please refer to the [libxml2 issue tracker](https://gitlab.gnome.org/GNOME/libxml2/-/issues).

## Packaging

There are no packages for bygg at the moment. If you'd like to add packaging for your distribution, 
please feel free to do so. I'm happy to help you with any questions you might have.

## What about the name?

I am Swedish and I want to name projects in Swedish, because it's fun. Bygg means build, and I think build is a fitting
word for a project like this, because it's all about building HTML and CSS documents.