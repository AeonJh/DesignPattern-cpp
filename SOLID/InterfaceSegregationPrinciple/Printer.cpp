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
    void print([[maybe_unused]] std::vector<Document*> docs) override { };
};

struct Scanner : IScanner {
    void scan([[maybe_unused]] std::vector<Document*> docs) override { };
};

struct Fax : IFax {
    void fax([[maybe_unused]] std::vector<Document*> docs) override { };
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

int main() {
    // Example usage
    Document doc1, doc2;
    std::vector<Document*> docs = { &doc1, &doc2 };

    IPrinter* printer = new Printer();
    IScanner* scanner = new Scanner();
    Machine machine(*printer, *scanner);
    machine.print(docs);

    delete printer;
    delete scanner;

    return 0;
}
