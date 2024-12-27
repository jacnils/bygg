#include <iostream>
#include <bygg/HTML/HTML.hpp> // for all HTML related features

int main() {
   using namespace bygg::HTML;

   Section sect{Tag::Html, Property{"i-hope", "you-are-ready-for-a-thousand-paragraphs"},
      Section{Tag::Div, Property{"brace", "yourself"},
      },
   };

   // get a reference to the inner div
   try {
      // we can infer that the inner div is the first section of the first section of the root section
      auto& s = sect.at_section(0);

      // add a thousand paragraphs to the inner div
      for (int i = 1; i <= 1000; ++i) {
         s += Element{Tag::P, "This is paragraph " + std::to_string(i)};
      }

      std::cout << Document{sect}.get<std::string>(Formatting::Pretty) << "\n";
   } catch (bygg::out_of_range& e) {
      std::cerr << "Failed to get the inner div: " << e.what() << "\n";
      return 1;
   }
}
