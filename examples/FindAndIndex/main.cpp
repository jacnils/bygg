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

   // Determine the target. For this example, we want the "This is a test page." paragraph.
   // We know that this can be found in the Body tag of the HTML document. This is a Section object.
   // Because we know that the root Section object only contains Section objects, we can use the section_iterator type.
   // If you aren't as lucky, you can use the variant_t type. See VariantIterator for an example of how to do this.
    for (auto it = root.section_begin(); it != root.section_end(); ++it) {
         const Section& sect = *it;

         if (sect.get_tag() == "body") {
              // We could iterate through the Section object. But then we have another layer of a Section object to
              // Let's use .find() and .at_section() to get the Section object we want.
              auto body = sect.find(Tag::Div);
              const Section& content = sect.at_section(body);

              // Now we can iterate through the content Section object.
              // But we can also use .find() and .at() to get the Element object we want.
              auto p = content.find("p");

              const Element& paragraph = content.at(p);
              std::cout << paragraph.get_data() << "\n";

              return 0;
              // Success! We found the paragraph.
         }
    }
}
