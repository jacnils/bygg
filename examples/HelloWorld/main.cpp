#include <fstream>
#include <bygg/HTML/HTML.hpp> // for all HTML related features
#include <bygg/CSS/property.hpp> // for bygg::CSS::Property(); could also use CSS.hpp for all CSS features

int main() {
   using namespace bygg::HTML;
   using CSSProperty = bygg::CSS::Property;

   Section sect{Tag::Html, Property{"lang", "us"},
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
