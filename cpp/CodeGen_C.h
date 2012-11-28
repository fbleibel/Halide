#ifndef CODEGEN_C_H
#define CODEGEN_C_H

#include "IRPrinter.h"
#include "Argument.h"
#include <string>
#include <vector>
#include <ostream>

namespace HalideInternal {

    using std::string;
    using std::vector;

    // This class emits C++ code equivalent to a halide Stmt. It's
    // mostly the same as an IRPrinter, but some things have to be
    // handled differently
    class CodeGen_C : public IRPrinter {
    public:
        CodeGen_C(ostream &);
        void compile(Stmt stmt, string name, const vector<Argument> &args);

        static void test();
    protected:

        void print_c_name(const string &);
        void print_c_type(Type);

        void visit(const Var *);
        void visit(const Cast *);
        void visit(const Select *);
        void visit(const Load *);
        void visit(const Store *);
        void visit(const Let *);
        void visit(const LetStmt *);
        void visit(const PrintStmt *);
        void visit(const AssertStmt *);
        void visit(const Pipeline *);
        void visit(const For *);
        void visit(const Provide *);
        void visit(const Allocate *);
        void visit(const Realize *);

    };
};

#endif
