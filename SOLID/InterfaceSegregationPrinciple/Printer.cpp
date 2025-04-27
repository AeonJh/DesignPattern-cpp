/**
 * @file Printer.cpp
 * 
 * @brief Example of Interface Segregation Principle (ISP)
 * 
 * @details The code demonstrates the Interface Segregation Principle (ISP) by defining interfaces for different functionalities
 *
 * @Interface Segregation Principle (ISP): No client should be forced to depend on methods it does not use.
 *
 * @author AeonJh
 * @date 2025-04-09
 */


#include <vector>
#include <gtest/gtest.h>


class Document { };

struct IPrinter {
    virtual void print(std::vector<Document*> docs) = 0;
    virtual ~IPrinter() = default;
};

struct IScanner {
    virtual void scan(std::vector<Document*> docs) = 0;
    virtual ~IScanner() = default;
};

struct IFax {
    virtual void fax(std::vector<Document*> docs) = 0;
    virtual ~IFax() = default;
};

struct Printer : IPrinter {
    void print(std::vector<Document*> docs) override { };
};

struct Scanner : IScanner {
    void scan(std::vector<Document*> docs) override { };
};

struct Fax : IFax {
    void fax(std::vector<Document*> docs) override { };
};

// struct IMachine
// {
//     virtual void print(std::vector<Document*> docs) = 0;
//     virtual void fax(std::vector<Document*> docs) = 0;
//     virtual void scan(std::vector<Document*> docs) = 0;
// };
//
// struct MyFavouritePrinter : IMachine 
// {
//     void print(std::vector<Document*> docs) override;
//     void fax(std::vector<Document*> docs) override;
//     void scan(std::vector<Document*> docs) override;
// };

struct IMachine : IPrinter, IScanner /* IFax and so no */ {

};

struct Machine : IMachine
{
    IPrinter& printer;
    IScanner& scanner;

    Machine(IPrinter& printer, IScanner& scanner)
        : printer{printer}, scanner{scanner} { }

    void print(std::vector<Document*> docs) override {
        printer.print(docs);
    }

    void scan(std::vector<Document*> docs) override {
        scanner.scan(docs);
    }
};

class PrinterTest : public ::testing::Test {
protected:
    void SetUp() override {
        doc1 = new Document();
        doc2 = new Document();
        docs = {doc1, doc2};
    }

    void TearDown() override {
        delete doc1;
        delete doc2;
    }

    Document* doc1;
    Document* doc2;
    std::vector<Document*> docs;
};

TEST_F(PrinterTest, PrinterPrintFunction) {
    Printer printer;
    EXPECT_NO_THROW(printer.print(docs));
}

TEST_F(PrinterTest, ScannerScanFunction) {
    Scanner scanner;
    EXPECT_NO_THROW(scanner.scan(docs));
}

TEST_F(PrinterTest, FaxFaxFunction) {
    Fax fax;
    EXPECT_NO_THROW(fax.fax(docs));
}

TEST_F(PrinterTest, MachineWithPrinterAndScanner) {
    Printer printer;
    Scanner scanner;
    Machine machine(printer, scanner);

    EXPECT_NO_THROW(machine.print(docs));
    EXPECT_NO_THROW(machine.scan(docs));
}

TEST_F(PrinterTest, MachineUsesPrinterReference) {
    class MockPrinter : public IPrinter {
    public:
        bool printed = false;
        void print(std::vector<Document*> docs) override {
            printed = true;
        }
    };

    MockPrinter printer;
    Scanner scanner;
    Machine machine(printer, scanner);

    machine.print(docs);
    EXPECT_TRUE(printer.printed);
}

TEST_F(PrinterTest, MachineUsesScannerReference) {
    class MockScanner : public IScanner {
    public:
        bool scanned = false;
        void scan(std::vector<Document*> docs) override {
            scanned = true;
        }
    };

    Printer printer;
    MockScanner scanner;
    Machine machine(printer, scanner);

    machine.scan(docs);
    EXPECT_TRUE(scanner.scanned);
}

TEST_F(PrinterTest, MultipleMachineInstances) {
    Printer printer1, printer2;
    Scanner scanner1, scanner2;

    Machine machine1(printer1, scanner1);
    Machine machine2(printer2, scanner2);

    EXPECT_NO_THROW(machine1.print(docs));
    EXPECT_NO_THROW(machine1.scan(docs));
    EXPECT_NO_THROW(machine2.print(docs));
    EXPECT_NO_THROW(machine2.scan(docs));
}

TEST_F(PrinterTest, DifferentDocumentSizes) {
    Printer printer;
    Scanner scanner;
    Machine machine(printer, scanner);

    std::vector<Document*> single_doc = {doc1};
    std::vector<Document*> empty_docs = {};

    EXPECT_NO_THROW(machine.print(single_doc));
    EXPECT_NO_THROW(machine.print(empty_docs));
    EXPECT_NO_THROW(machine.scan(single_doc));
    EXPECT_NO_THROW(machine.scan(empty_docs));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
