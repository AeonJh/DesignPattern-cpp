#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>

struct Tag;
struct HtmlElement;
struct HtmlBuilder;

struct Tag {
    std::string name;
    std::string text;
    std::vector<Tag> children;
    std::vector<std::pair<std::string, std::string>> attributes;
    friend std::ostream& operator<<(std::ostream& os, const Tag& tag) {
        os << "<" << tag.name;
        for (const auto& attr : tag.attributes) {
            os << " " << attr.first << "=\"" << attr.second << "\"";
        }
        os << ">";
        if (!tag.text.empty()) {
            os << tag.text;
        }
        for (const auto& child : tag.children) {
            os << child;
        }
        os << "</" << tag.name << ">";
        return os;
    }

protected:
    Tag(const std::string& name, const std::string& text)
        : name(name), text(text) { }
    Tag(const std::string& name, const std::vector<Tag>& children)
        : name(name), children(children) { }
};

struct P : Tag {
    explicit P(const std::string& text)
        : Tag("p", text) { }
    P(std::initializer_list<Tag> children)
        : Tag("p", std::vector<Tag>(children)) { }
};

struct IMG : Tag {
    explicit IMG(const std::string& url)
        : Tag("img", "") {
        attributes.emplace_back("src", url);
    }
};

struct HtmlElement {
    std::string name;
    std::string text;
    std::vector<HtmlElement> elements;
    const size_t indent_spaces = 2;

    static std::unique_ptr<HtmlBuilder> build(const std::string& root_name) {
        return std::make_unique<HtmlBuilder>(root_name);
    }

    friend struct HtmlBuilder;

    std::string str(int indent = 0) const {
        // pretty-print the contents
        std::ostringstream oss;
        oss << std::string(indent, ' ') << "<" << name << ">\n";
        if (text.size() > 0) {
            oss << std::string(indent + indent_spaces, ' ') << text << "\n";
        }
        for (const auto& e : elements) {
            oss << e.str(indent + indent_spaces);
        }
        oss << std::string(indent, ' ') << "</" << name << ">\n";
        return oss.str();
    }

protected:  // hide the constructor
    HtmlElement() { }
    HtmlElement(const std::string& name, const std::string& text)
        : name(name), text(text) { }
};

struct HtmlBuilder {
    HtmlElement root;

    operator HtmlElement() const { return root; }

    HtmlBuilder(const std::string& root_name) { root.name = root_name; }

    HtmlBuilder& add_child([[maybe_unused]] const std::string& child_name,  [[maybe_unused]] const std::string& child_text) {
        HtmlElement e(child_name, child_text);
        root.elements.emplace_back(e);
        return *this;
    }

    std::string str() const { return root.str(); }
};

int main() {
    auto html = (*HtmlElement::build("ul"))
                    .add_child("li", "hello")
                    .add_child("li", "world");
    std::cout << html.str() << std::endl;
    std::cout << P{IMG{"http://pokemon.com/pikachu.png"}} << std::endl;

    return 0;
}
