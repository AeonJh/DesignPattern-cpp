#include <gtest/gtest.h>
#include <iostream>
#include <string>

// Legacy interface - Cannot be modified
class OldPrinter
{
public:
    void printOldFormat(const std::string& message)
    {
        std::cout << "[OLD] " << message << std::endl;
    }
};

// New interface that client expects
class ModernPrinter
{
public:
    virtual ~ModernPrinter() = default;
    virtual void print(const std::string& message) = 0;
    virtual void printWithTimestamp(const std::string& message) = 0;
};

// Adapter class that adapts OldPrinter to ModernPrinter interface
class PrinterAdapter : public ModernPrinter
{
private:
    OldPrinter* m_oldPrinter;
    
public:
    explicit PrinterAdapter(OldPrinter* oldPrinter) : m_oldPrinter(oldPrinter) {}
    
    void print(const std::string& message) override
    {
        m_oldPrinter->printOldFormat(message);
    }
    
    void printWithTimestamp(const std::string& message) override
    {
        std::string timestampedMessage = "[2024] " + message;
        m_oldPrinter->printOldFormat(timestampedMessage);
    }
};

// Client code that works with ModernPrinter interface
class Document
{
private:
    ModernPrinter* m_printer;
    
public:
    explicit Document(ModernPrinter* printer) : m_printer(printer) {}
    
    void printDocument(const std::string& content)
    {
        m_printer->print("Document: " + content);
    }
    
    void printWithDate(const std::string& content)
    {
        m_printer->printWithTimestamp("Document: " + content);
    }
};

// Another example: Shape drawing adapter
class LegacyRectangle
{
public:
    void drawRectangle(int x, int y, int width, int height)
    {
        std::cout << "Legacy Rectangle: (" << x << "," << y << ") - " 
                  << width << "x" << height << std::endl;
    }
};

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw() = 0;
    virtual void setBounds(int x, int y, int width, int height) = 0;
};

class RectangleAdapter : public Shape
{
private:
    LegacyRectangle* m_legacyRect;
    int m_x, m_y, m_width, m_height;
    
public:
    explicit RectangleAdapter(LegacyRectangle* legacyRect) 
        : m_legacyRect(legacyRect), m_x(0), m_y(0), m_width(0), m_height(0) {}
    
    void setBounds(int x, int y, int width, int height) override
    {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
    }
    
    void draw() override
    {
        m_legacyRect->drawRectangle(m_x, m_y, m_width, m_height);
    }
};

// Unit Tests
class AdapterPatternTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(AdapterPatternTest, PrinterAdapterBasicPrint)
{
    OldPrinter oldPrinter;
    PrinterAdapter adapter(&oldPrinter);
    
    testing::internal::CaptureStdout();
    adapter.print("Hello World");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "[OLD] Hello World\n");
}

TEST_F(AdapterPatternTest, PrinterAdapterTimestampPrint)
{
    OldPrinter oldPrinter;
    PrinterAdapter adapter(&oldPrinter);
    
    testing::internal::CaptureStdout();
    adapter.printWithTimestamp("Test Message");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "[OLD] [2024] Test Message\n");
}

TEST_F(AdapterPatternTest, DocumentWithAdapter)
{
    OldPrinter oldPrinter;
    PrinterAdapter adapter(&oldPrinter);
    Document doc(&adapter);
    
    testing::internal::CaptureStdout();
    doc.printDocument("Important content");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "[OLD] Document: Important content\n");
}

TEST_F(AdapterPatternTest, DocumentWithTimestamp)
{
    OldPrinter oldPrinter;
    PrinterAdapter adapter(&oldPrinter);
    Document doc(&adapter);
    
    testing::internal::CaptureStdout();
    doc.printWithDate("Dated content");
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "[OLD] [2024] Document: Dated content\n");
}

TEST_F(AdapterPatternTest, RectangleAdapter)
{
    LegacyRectangle legacyRect;
    RectangleAdapter adapter(&legacyRect);
    
    adapter.setBounds(10, 20, 100, 50);
    
    testing::internal::CaptureStdout();
    adapter.draw();
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "Legacy Rectangle: (10,20) - 100x50\n");
}

// Demonstration function
void demonstrateAdapter()
{
    std::cout << "\n=== Adapter Pattern Demonstration ===" << std::endl;
    
    // Printer example
    std::cout << "\n1. Printer Adapter Example:" << std::endl;
    OldPrinter oldPrinter;
    PrinterAdapter printerAdapter(&oldPrinter);
    Document document(&printerAdapter);
    
    document.printDocument("This is a test document");
    document.printWithDate("This document has a timestamp");
    
    // Shape example
    std::cout << "\n2. Shape Adapter Example:" << std::endl;
    LegacyRectangle legacyRect;
    RectangleAdapter shapeAdapter(&legacyRect);
    
    shapeAdapter.setBounds(0, 0, 200, 100);
    shapeAdapter.draw();
    
    std::cout << "\n=== End Demonstration ===" << std::endl;
}

int main(int argc, char** argv)
{
    // Run demonstration
    demonstrateAdapter();
    
    // Run tests
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}