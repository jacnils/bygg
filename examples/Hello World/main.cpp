#include <fstream>
#include <bygg/HTML/HTML.hpp> // for all HTML related features
#include <bygg/CSS/property.hpp> // for bygg::CSS::Property(); could also use CSS.hpp for all CSS features

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
