#include <iostream>
#include <bygg/HTML/HTML.hpp> // for all HTML related features

int main() {
   using namespace bygg::HTML;

   Section root{Tag::Html, Property{"lang", "en"},
         Section{Tag::Head,
             Element{Tag::Title, "Hello, world!"},
             Element{Tag::Meta, make_properties(Property{"name", "description"}, Property{"content", "This is a test page"})},
             Element{Tag::Meta, make_properties(Property{"name", "viewport"}, Property{"content", "width=device-width, initial-scale=1"})},
         },
         Section{Tag::Body,
             Section{Tag::Div, make_properties(Property{"id", "content"}, Property{"class", "content"}),
                 Element{Tag::H1, "Hello, world!"},
                 Element{Tag::P, "This is a test page."},
             },
         },
         Section{Tag::Footer,
             Section{Tag::Div, make_properties(Property{"id", "footer"}),
                    Element{Tag::P, "My example footer here."},
                },
         },
   };

   // Iterate through it. For this we're gonna use the default iterator (i.e. begin() and end())
   // In the Section class, begin() and end() return an interator in the form of a bygg::HTML::variant_t object.
   // This object can be one of the following types:
   // - bygg::HTML::Element
   // - bygg::HTML::Section
   // In order to determine which type it is, we can use std::holds_alternative<>().
   // If it's an Element, we can use std::get<bygg::HTML::Element>() to get the Element object.
   // If it's a Section, we can use std::get<bygg::HTML::Section>() to get the Section object.
   // This is identical behavior to the std::variant class.
   for (auto it = root.begin(); it != root.end(); ++it) {
         if (std::holds_alternative<Section>(*it)) {
              const Section& sect = std::get<Section>(*it);

              if (sect.get_tag() == "head") {
                  // We know that the Section object is a head tag, and we also
                  // know that it contains only Element objects.
                  // Therefore, we can iterate using the simpler `element_iterator` type.
                  // If you know that the object is a Section, you can use the `section_iterator` type.
                  // If you don't know what the object is, you can use the `variant_t` type.
                  for (auto it = sect.element_begin(); it != sect.element_end(); ++it) {
                      const Element& elem = *it;
                      std::cout << elem.get_tag() << ": " << (elem.get_data().empty() ? "Empty" : elem.get_data()) << "\n";
                      std::cout << "\tWith properties:\n";

                      bool s{false};
                      for (const auto& prop : elem.get_properties()) {
                          s = true;
                          std::cout << "\t\t" << prop.get_key() << ": " << prop.get_value() << "\n";
                      }

                      if (!s) {
                          std::cout << "\t\tNone\n";
                      }
                  }
              }

              break;
         }
   }

   // What this does is iterate through the root Section object, and if it finds a head tag, it will iterate through the
   // elements of the head tag and print out the tag and data of each element.
}
