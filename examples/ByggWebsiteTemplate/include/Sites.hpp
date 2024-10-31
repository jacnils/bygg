#ifndef SITES_HPP
#define SITES_HPP

#include <bygg/bygg.hpp>

struct PageProperties {
    std::string name{};
    std::string description{};
    std::string lang{};
};

namespace Sites {
    bygg::HTML::Section get_generic_header(const std::string&, const std::string& = {});
    bygg::HTML::Section get_generic_footer();
    bygg::HTML::Section get_index_site();
    bygg::HTML::Section create_body_container(const std::vector<bygg::HTML::Element>&);
}

#endif //SITES_HPP
