/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/HTML/tag.hpp>
#include <algorithm>

std::unordered_map<bygg::HTML::Tag, std::pair<bygg::string_type, bygg::HTML::Type>> bygg::HTML::get_tag_map() {
    return {
        {Tag::Empty, {"", Type::Text}},
        {Tag::Empty_No_Formatting, {"", Type::Text_No_Formatting}},
        {Tag::Abbreviation, {"abbr", Type::Non_Self_Closing}},
        {Tag::Abbr, {"abbr", Type::Non_Self_Closing}},
        {Tag::Acronym, {"acronym", Type::Non_Self_Closing}},
        {Tag::Address, {"address", Type::Non_Self_Closing}},
        {Tag::Applet, {"applet", Type::Non_Self_Closing}},
        {Tag::Anchor, {"a", Type::Non_Self_Closing}},
        {Tag::A, {"a", Type::Non_Self_Closing}},
        {Tag::Article, {"article", Type::Non_Self_Closing}},
        {Tag::Area, {"area", Type::Self_Closing}},
        {Tag::Aside, {"aside", Type::Non_Self_Closing}},
        {Tag::Audio, {"audio", Type::Non_Self_Closing}},
        {Tag::Base, {"base", Type::Self_Closing}},
        {Tag::Basefont, {"basefont", Type::Self_Closing}},
        {Tag::Bdi, {"bdi", Type::Non_Self_Closing}},
        {Tag::Bdo, {"bdo", Type::Non_Self_Closing}},
        {Tag::Bgsound, {"bgsound", Type::Non_Self_Closing}},
        {Tag::Big, {"big", Type::Non_Self_Closing}},
        {Tag::Blockquote, {"blockquote", Type::Non_Self_Closing}},
        {Tag::Body, {"body", Type::Non_Self_Closing}},
        {Tag::B, {"b", Type::Non_Self_Closing}},
        {Tag::Bold, {"b", Type::Non_Self_Closing}},
        {Tag::Br, {"br", Type::Self_Closing}},
        {Tag::Break, {"br", Type::Self_Closing}},
        {Tag::Button, {"button", Type::Non_Self_Closing}},
        {Tag::Caption, {"caption", Type::Non_Self_Closing}},
        {Tag::Canvas, {"canvas", Type::Non_Self_Closing}},
        {Tag::Center, {"center", Type::Non_Self_Closing}},
        {Tag::Cite, {"cite", Type::Non_Self_Closing}},
        {Tag::Code, {"code", Type::Non_Self_Closing}},
        {Tag::Colgroup, {"colgroup", Type::Non_Self_Closing}},
        {Tag::Column, {"col", Type::Self_Closing}},
        {Tag::Col, {"col", Type::Self_Closing}},
        {Tag::Data, {"data", Type::Non_Self_Closing}},
        {Tag::Datalist, {"datalist", Type::Non_Self_Closing}},
        {Tag::Dd, {"dd", Type::Non_Self_Closing}},
        {Tag::Dfn, {"dfn", Type::Non_Self_Closing}},
        {Tag::Define, {"dfn", Type::Non_Self_Closing}},
        {Tag::Delete, {"del", Type::Non_Self_Closing}},
        {Tag::Del, {"del", Type::Non_Self_Closing}},
        {Tag::Details, {"details", Type::Non_Self_Closing}},
        {Tag::Dialog, {"dialog", Type::Non_Self_Closing}},
        {Tag::Dir, {"dir", Type::Non_Self_Closing}},
        {Tag::Div, {"div", Type::Non_Self_Closing}},
        {Tag::Dl, {"dl", Type::Non_Self_Closing}},
        {Tag::Dt, {"dt", Type::Non_Self_Closing}},
        {Tag::Embed, {"embed", Type::Self_Closing}},
        {Tag::Fieldset, {"fieldset", Type::Non_Self_Closing}},
        {Tag::Figcaption, {"figcaption", Type::Non_Self_Closing}},
        {Tag::Figure, {"figure", Type::Non_Self_Closing}},
        {Tag::Font, {"font", Type::Non_Self_Closing}},
        {Tag::Footer, {"footer", Type::Non_Self_Closing}},
        {Tag::Form, {"form", Type::Non_Self_Closing}},
        {Tag::Frame, {"frame", Type::Self_Closing}},
        {Tag::Frameset, {"frameset", Type::Non_Self_Closing}},
        {Tag::Head, {"head", Type::Non_Self_Closing}},
        {Tag::Header, {"header", Type::Non_Self_Closing}},
        {Tag::H1, {"h1", Type::Non_Self_Closing}},
        {Tag::H2, {"h2", Type::Non_Self_Closing}},
        {Tag::H3, {"h3", Type::Non_Self_Closing}},
        {Tag::H4, {"h4", Type::Non_Self_Closing}},
        {Tag::H5, {"h5", Type::Non_Self_Closing}},
        {Tag::H6, {"h6", Type::Non_Self_Closing}},
        {Tag::Hr, {"hr", Type::Self_Closing}},
        {Tag::Hgroup, {"hgroup", Type::Non_Self_Closing}},
        {Tag::Html, {"html", Type::Non_Self_Closing}},
        {Tag::Iframe, {"iframe", Type::Non_Self_Closing}},
        {Tag::Image, {"img", Type::Self_Closing}},
        {Tag::Img, {"img", Type::Self_Closing}},
        {Tag::Input, {"input", Type::Self_Closing}},
        {Tag::Ins, {"ins", Type::Non_Self_Closing}},
        {Tag::Isindex, {"isindex", Type::Self_Closing}},
        {Tag::Italic, {"i", Type::Non_Self_Closing}},
        {Tag::I, {"i", Type::Non_Self_Closing}},
        {Tag::Kbd, {"kbd", Type::Non_Self_Closing}},
        {Tag::Keygen, {"keygen", Type::Self_Closing}},
        {Tag::Label, {"label", Type::Non_Self_Closing}},
        {Tag::Legend, {"legend", Type::Non_Self_Closing}},
        {Tag::List, {"li", Type::Non_Self_Closing}},
        {Tag::Li, {"li", Type::Non_Self_Closing}},
        {Tag::Link, {"link", Type::Self_Closing}},
        {Tag::Main, {"main", Type::Non_Self_Closing}},
        {Tag::Mark, {"mark", Type::Non_Self_Closing}},
        {Tag::Marquee, {"marquee", Type::Non_Self_Closing}},
        {Tag::Menuitem, {"menuitem", Type::Non_Self_Closing}},
        {Tag::Meta, {"meta", Type::Self_Closing}},
        {Tag::Meter, {"meter", Type::Non_Self_Closing}},
        {Tag::Nav, {"nav", Type::Non_Self_Closing}},
        {Tag::Nobreak, {"nobr", Type::Non_Self_Closing}},
        {Tag::Nobr, {"nobr", Type::Non_Self_Closing}},
        {Tag::Noembed, {"noembed", Type::Non_Self_Closing}},
        {Tag::Noscript, {"noscript", Type::Non_Self_Closing}},
        {Tag::Object, {"object", Type::Non_Self_Closing}},
        {Tag::Optgroup, {"optgroup", Type::Non_Self_Closing}},
        {Tag::Option, {"option", Type::Non_Self_Closing}},
        {Tag::Output, {"output", Type::Non_Self_Closing}},
        {Tag::Paragraph, {"p", Type::Non_Self_Closing}},
        {Tag::P, {"p", Type::Non_Self_Closing}},
        {Tag::Param, {"param", Type::Self_Closing}},
        {Tag::Phrase, {"phrase", Type::Non_Self_Closing}},
        {Tag::Pre, {"pre", Type::Non_Self_Closing}},
        {Tag::Progress, {"progress", Type::Non_Self_Closing}},
        {Tag::Quote, {"q", Type::Non_Self_Closing}},
        {Tag::Q, {"q", Type::Non_Self_Closing}},
        {Tag::Rp, {"rp", Type::Non_Self_Closing}},
        {Tag::Rt, {"rt", Type::Non_Self_Closing}},
        {Tag::Ruby, {"ruby", Type::Non_Self_Closing}},
        {Tag::Outdated, {"s", Type::Non_Self_Closing}},
        {Tag::S, {"s", Type::Non_Self_Closing}},
        {Tag::Sample, {"samp", Type::Non_Self_Closing}},
        {Tag::Samp, {"samp", Type::Non_Self_Closing}},
        {Tag::Script, {"script", Type::Non_Self_Closing}},
        {Tag::Section, {"section", Type::Non_Self_Closing}},
        {Tag::Small, {"small", Type::Non_Self_Closing}},
        {Tag::Source, {"source", Type::Non_Self_Closing}},
        {Tag::Spacer, {"spacer", Type::Non_Self_Closing}},
        {Tag::Span, {"span", Type::Non_Self_Closing}},
        {Tag::Strike, {"strike", Type::Non_Self_Closing}},
        {Tag::Strong, {"strong", Type::Non_Self_Closing}},
        {Tag::Style, {"style", Type::Non_Self_Closing}},
        {Tag::Sub, {"sub", Type::Non_Self_Closing}},
        {Tag::Subscript, {"sub", Type::Non_Self_Closing}},
        {Tag::Sup, {"sup", Type::Non_Self_Closing}},
        {Tag::Superscript, {"sup", Type::Non_Self_Closing}},
        {Tag::Summary, {"summary", Type::Non_Self_Closing}},
        {Tag::Svg, {"svg", Type::Self_Closing}},
        {Tag::Table, {"table", Type::Non_Self_Closing}},
        {Tag::Tbody, {"tbody", Type::Non_Self_Closing}},
        {Tag::Td, {"td", Type::Non_Self_Closing}},
        {Tag::Template, {"template", Type::Non_Self_Closing}},
        {Tag::Tfoot, {"tfoot", Type::Non_Self_Closing}},
        {Tag::Th, {"th", Type::Non_Self_Closing}},
        {Tag::Tr, {"tr", Type::Non_Self_Closing}},
        {Tag::Thead, {"thead", Type::Non_Self_Closing}},
        {Tag::Time, {"time", Type::Non_Self_Closing}},
        {Tag::Title, {"title", Type::Non_Self_Closing}},
        {Tag::Track, {"track", Type::Self_Closing}},
        {Tag::Tt, {"tt", Type::Non_Self_Closing}},
        {Tag::Underline, {"u", Type::Non_Self_Closing}},
        {Tag::U, {"u", Type::Non_Self_Closing}},
        {Tag::Var, {"var", Type::Non_Self_Closing}},
        {Tag::Video, {"video", Type::Non_Self_Closing}},
        {Tag::Wbr, {"wbr", Type::Self_Closing}},
        {Tag::Xmp, {"xmp", Type::Non_Self_Closing}},
    };
}

std::pair<bygg::string_type, bygg::HTML::Type> bygg::HTML::resolve_tag(const Tag tag) {
    const std::unordered_map<bygg::HTML::Tag, std::pair<bygg::string_type, bygg::HTML::Type>> tag_map{get_tag_map()};

    if (tag_map.find(tag) != tag_map.end()) {
        return tag_map.at(tag);
    }

    const string_type throwmsg{"Invalid tag: " + std::to_string(static_cast<bygg::integer_type>(tag))};
    throw invalid_argument{throwmsg.c_str()};
}

bygg::HTML::Tag bygg::HTML::resolve_tag(const bygg::string_type& tag) {
    const std::unordered_map<bygg::HTML::Tag, std::pair<bygg::string_type, bygg::HTML::Type>> tag_map{get_tag_map()};

    for (const auto& it : tag_map) {
        if (it.second.first == tag) {
            return it.first;
        }
    }

    throw invalid_argument{"Invalid tag"};
}

bool bygg::HTML::is_container(const string_type& tag) {
    static const std::vector<std::string> container_elements = {
        "div",
        "section",
        "article",
        "nav",
        "aside",
        "head",
        "header",
        "footer",
        "foot",
        "main",
        "ul",
        "ol",
        "li",
        "body",
        "table",
        "tbody",
        "thead",
        "tfoot",
        "tr",
        "td",
        "th",
        "form",
        "fieldset",
        "details",
        "summary",
        "html",
    };

    return std::find(container_elements.begin(), container_elements.end(), tag) != container_elements.end();
}

bool bygg::HTML::is_container(Tag tag) {
    return is_container(resolve_tag(tag).first);
}