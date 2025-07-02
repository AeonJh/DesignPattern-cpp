#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>

enum class OutputFormat { markdown, html };

struct ListStrategy {
    virtual void start(std::ostringstream& oss){};
    virtual void end(std::ostringstream& oss){};
    virtual void add_list_item(std::ostringstream& oss, const std::string& item){};
};

struct HtmlListStrategy : ListStrategy {
    void start(std::ostringstream& oss) override { oss << "<ul>\n"; }
    void end(std::ostringstream& oss) override { oss << "</ul>\n"; }
    void add_list_item(std::ostringstream& oss, const std::string& item) override {
        oss << "<li>" << item << "</li> \n";
    }
};

struct MarkdownListStrategy : ListStrategy {
    void add_list_item(std::ostringstream& oss, const std::string& item) override { oss << "*" << item << "\n"; }
};

struct TextProcessor {
    void append_list(const std::vector<std::string> items) {
        listStrategy->start(oss);

        for (auto &&item : items) {
            listStrategy->add_list_item(oss, item);
        }

        listStrategy->end(oss);
    }

    void set_output_format(OutputFormat format) {
        switch (format) {
            case OutputFormat::markdown:
                listStrategy = std::make_unique<MarkdownListStrategy>();
            break;
            case OutputFormat::html:
                listStrategy = std::make_unique<HtmlListStrategy>();
            break;
        }
    }

    std::string str() const { return oss.str(); }
    void clear() { oss.str(""); oss.clear(); }

private:
    std::ostringstream oss;
    std::unique_ptr<ListStrategy> listStrategy;
};

template <typename LS>
struct TextProcessor2 {
    void append_list(const std::vector<std::string> items) {
        listStrategy.start(oss);

        for (auto &&item : items) {
            listStrategy.add_list_item(oss, item);
        }

        listStrategy.end(oss);
    }

    std::string str() const { return oss.str(); }
    void clear() { oss.str(""); oss.clear(); }

private:
    std::ostringstream oss;
    LS listStrategy;
};

// Unit tests for TextProcessor and TextProcessor2
TEST(TextProcessorTest, MarkdownOutput) {
    TextProcessor tp;
    tp.set_output_format(OutputFormat::markdown);
    tp.append_list({"foo", "bar", "baz"});
    EXPECT_EQ(tp.str(), "*foo\n*bar\n*baz\n");
}

TEST(TextProcessorTest, HtmlOutput) {
    TextProcessor tp;
    tp.set_output_format(OutputFormat::html);
    tp.append_list({"foo", "bar"});
    EXPECT_EQ(tp.str(), "<ul>\n<li>foo</li> \n<li>bar</li> \n</ul>\n");
}

TEST(TextProcessorTest, ClearWorks) {
    TextProcessor tp;
    tp.set_output_format(OutputFormat::markdown);
    tp.append_list({"a"});
    tp.clear();
    EXPECT_EQ(tp.str(), "");
}

TEST(TextProcessor2Test, MarkdownListStrategy) {
    TextProcessor2<MarkdownListStrategy> tp2;
    tp2.append_list({"x", "y"});
    EXPECT_EQ(tp2.str(), "*x\n*y\n");
}

TEST(TextProcessor2Test, HtmlListStrategy) {
    TextProcessor2<HtmlListStrategy> tp2;
    tp2.append_list({"1", "2"});
    EXPECT_EQ(tp2.str(), "<ul>\n<li>1</li> \n<li>2</li> \n</ul>\n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
