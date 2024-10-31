#include <Sites.hpp>

bygg::HTML::Section Sites::get_generic_header(const std::string& title, const std::string& description) {
    return bygg::HTML::Section{bygg::HTML::Tag::Head, {}, std::vector<bygg::HTML::Element> {
        bygg::HTML::Element{bygg::HTML::Tag::Title, {}, title},
        bygg::HTML::Element{bygg::HTML::Tag::Meta, bygg::HTML::make_properties(
            bygg::HTML::Property{"name", "description"},
            bygg::HTML::Property{"content", description}
            )},
        bygg::HTML::Element{bygg::HTML::Tag::Meta, bygg::HTML::make_properties(
            bygg::HTML::Property{"name", "viewport"},
            bygg::HTML::Property{"content", "width=device-width, initial-scale=1"}
            )}
    }};
}

bygg::HTML::Section Sites::get_generic_footer() {
    return bygg::HTML::Section{bygg::HTML::Tag::Footer, {}, std::vector<bygg::HTML::Section> {
        bygg::HTML::Section{bygg::HTML::Tag::Div, bygg::HTML::make_properties(bygg::HTML::Property{"id", "footer"}, bygg::HTML::Property{"class", "footer"}),
            std::vector<bygg::HTML::Element> {
                bygg::HTML::Element{bygg::HTML::Tag::P, {}, "My example footer here."}
            }
        }
    }};
}

bygg::HTML::Section Sites::create_body_container(const std::vector<bygg::HTML::Element>& elements) {
    return bygg::HTML::Section{bygg::HTML::Tag::Body, {}, std::vector<bygg::HTML::Section> {
        bygg::HTML::Section{bygg::HTML::Tag::Div, bygg::HTML::make_properties(bygg::HTML::Property{"id", "content"}, bygg::HTML::Property{"class", "content"}), elements}
    }};
}


bygg::HTML::Section Sites::get_index_site() {
    return Sites::create_body_container({
        bygg::HTML::Element{bygg::HTML::Tag::H1, {}, "Hello world!"},
        bygg::HTML::Element{bygg::HTML::Tag::P, {}, "Hello world! This is a test page, which was generated using what is probably the worst website generator in the entire world. But it's quite cool to be able to write a website in C++."},
    });
}
