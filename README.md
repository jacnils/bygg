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
- HTML serialization\*
- No dependencies, other than the standard library\*
- Command-line tool for generating HTML or pseudocode from HTML or Markdown files\*
- Windows, macOS, Linux and \*BSD support\*\*

*Serialization and CLI requires the use of libxml2, which is not included in this repository.
Tests require the use of Catch2, which is not included in this repository.
Markdown support requires `pandoc` to be installed on the system. Pandoc is not included in this
repository.

**Not tested on BSD. bygg is actively tested on Windows, macOS and Linux through GitHub Actions.

## Dependencies

- C++17 compiler
- CMake
- libxml2 (*optional*, for CLI and serialization)
- Catch2 (*optional*, for tests)
- Pandoc (*optional*, for Markdown support when using the CLI, not required at compile-time)

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
Note that you must disable the CLI as well.

If you don't need the CLI, you can disable it by passing `-DBYGG_BUILD_CLI=OFF` to CMake.
This will disable the CLI utility.

If you want to enable the tests, you can pass `-DBYGG_BUILD_TESTS=ON` to 
CMake, and they will automatically be built.

If you want to download and build Catch2, you can pass `-DBYGG_BUILD_CATCH2=ON` to CMake.
This will download and build Catch2 as part of the build process. If you want to build
libxml2, you can pass `-DBYGG_BUILD_LIBXML2=ON` to CMake. This will download and build
libxml2 as part of the build process.

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

   Section sect{Tag::Html, Properties{Property{"lang", "us"}},
      Section{Tag::Head,
         Element{Tag::Title, "Hello world!"},
         Element{Tag::Meta, Properties{Property{"name", "description"}, Property{"content", "Hello world description!"}}},
         Element{Tag::Meta, Properties{Property{"name", "viewport"}, Property{"content", "width=device-width, initial-scale=1.0"}}},
      },
      Section{Tag::Body,
         Section{Tag::Div, Properties{Property{"class", "content"}},
            Element{Tag::H1, "Hello world!"},
            Element{Tag::P, Properties{Property{"style", static_cast<std::string>(CSSProperty("color", "red"))}}, "This is a red string of text."},
         },
      },
      Section{Tag::Footer,
         Element{Tag::P, "This is a paragraph in a footer."},
      },
   };

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

## CLI

The CLI utility can be used to generate HTML or pseudocode from HTML or Markdown files.  This may be useful 
if you want to quickly generate `bygg` code from an existing HTML or even Markdown document. Note that you 
can also read from stdin.

If you enabled the CLI during the build process and executed `cmake --install . --prefix /usr`, you should
be able to simply run `bygg-cli` from the command line. In addition, if you enabled `BYGG_INSTALL_DOCS`,
you can run `man bygg-cli` (provided you have `man` installed) to see the full documentation for the CLI.

Example usage:

```sh
bygg-cli --formatting pretty --input html input.html # generates pretty-formatted HTML from input.html
bygg-cli --formatting pretty --input markdown input.md # generates pretty-formatted HTML from input.md
bygg-cli --formatting pseudo --input html input.html # generates pseudo-code from input.html
bygg-cli --formatting pseudo --input markdown input.md # generates pseudo-code from input.md
cat input.html | bygg-cli --formatting pretty --input html # generates pretty-formatted HTML from stdin
cat input.md | bygg-cli --formatting pretty --input markdown # generates pretty-formatted HTML from stdin
```

See `bygg-cli --help` or the aforementioned man page for more information.

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