#include <Endpoint.hpp>
#include <Sites.hpp>
#include <bygg/bygg.hpp>

int main() {
    static const std::vector<std::tuple<std::string, bygg::HTML::Section, PageProperties>> website_tree{
        {"index.html", Sites::get_index_site(), PageProperties{"Example.com", "This is a test description", "en"}}
    };

    for (const auto& it : website_tree) {
        Endpoint endpoint(std::get<0>(it));

        bygg::HTML::Section root{bygg::HTML::Tag::Html, bygg::HTML::make_properties(bygg::HTML::Property{"lang", std::get<2>(it).lang})};

        root += Sites::get_generic_header(std::get<2>(it).name, std::get<2>(it).description);
        root += std::get<1>(it);
        root += Sites::get_generic_footer();

        endpoint.open();
        endpoint.append_string(bygg::HTML::Document(root).get<std::string>(bygg::HTML::Formatting::Pretty));
        endpoint.close();
    }
}