//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   parser/Expression.cc
 * \author Kent Budge
 * \date   Wed Jul 26 07:53:32 2006
 * \brief  Implementation of class Expression
 * \note   Copyright 2016-2017 Los Alamos National Security, LLC.
 */
//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

#include "Constant_Expression.hh"

namespace rtt_parser {
using namespace rtt_dsxx;

typedef SP<Expression> pE;
typedef map<string, pair<unsigned, Unit>> Variable_Map;

//---------------------------------------------------------------------------//
/*!
 *
 * The and operator implicitly converts its operands to bool. Hence no unit
 * compatibility of the operands is required, and the result is dimensionless.
 */
class And_Expression : public Expression {
public:
  And_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) && evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > AND_PRECEDENCE) {
      out << '(';
    }
    e1_->write(AND_PRECEDENCE, vars, out);
    out << "&&";
    e2_->write(AND_PRECEDENCE, vars, out);
    if (p > AND_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Cos_Expression : public Expression {
public:
  Cos_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), dimensionless),

        expression_(expression) {
    Require(expression);
    Require(is_compatible(expression_->units(), dimensionless));

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           is_compatible(expression_->units(), dimensionless) &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return cos(evaluate_def_(expression_, x));
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > FUNCTION_PRECEDENCE) {
      out << '(';
    }
    out << "cos(";
    expression_->write(COMMA_PRECEDENCE, vars, out);
    out << ")";
    if (p > FUNCTION_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Difference_Expression : public Expression {
public:
  Difference_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), e1->units()),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           is_compatible(e1_->units(), e2_->units()) &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) - evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > DIFFERENCE_PRECEDENCE) {
      out << '(';
    }
    e1_->write(DIFFERENCE_PRECEDENCE, vars, out);
    out << "-";
    e2_->write(DIFFERENCE_PRECEDENCE, vars, out);
    if (p > DIFFERENCE_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Exp_Expression : public Expression {
public:
  Exp_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), dimensionless),

        expression_(expression) {
    Require(expression);
    Require(is_compatible(expression_->units(), dimensionless));

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           is_compatible(expression_->units(), dimensionless) &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return exp(evaluate_def_(expression_, x));
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > FUNCTION_PRECEDENCE) {
      out << '(';
    }
    out << "exp(";
    expression_->write(COMMA_PRECEDENCE, vars, out);
    out << ")";
    if (p > FUNCTION_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Greater_Expression : public Expression {
public:
  Greater_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           is_compatible(e1_->units(), e2_->units()) &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) > evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > GREATER_PRECEDENCE) {
      out << '(';
    }
    e1_->write(GREATER_PRECEDENCE, vars, out);
    out << ">";
    e2_->write(GREATER_PRECEDENCE, vars, out);
    if (p > GREATER_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class GE_Expression : public Expression {
public:
  GE_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           is_compatible(e1_->units(), e2_->units()) &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) >= evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > GE_PRECEDENCE) {
      out << '(';
    }
    e1_->write(GE_PRECEDENCE, vars, out);
    out << ">=";
    e2_->write(GE_PRECEDENCE, vars, out);
    if (p > GE_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Less_Expression : public Expression {
public:
  Less_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           is_compatible(e1_->units(), e2_->units()) &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) < evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > LESS_PRECEDENCE) {
      out << '(';
    }
    e1_->write(LESS_PRECEDENCE, vars, out);
    out << "<";
    e2_->write(LESS_PRECEDENCE, vars, out);
    if (p > LESS_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class LE_Expression : public Expression {
public:
  LE_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           is_compatible(e1_->units(), e2_->units()) &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) <= evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > LE_PRECEDENCE) {
      out << '(';
    }
    e1_->write(LE_PRECEDENCE, vars, out);
    out << "<=";
    e2_->write(LE_PRECEDENCE, vars, out);
    if (p > LE_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Log_Expression : public Expression {
public:
  Log_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), dimensionless),

        expression_(expression) {
    Require(expression);
    Require(is_compatible(expression_->units(), dimensionless));

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           is_compatible(expression_->units(), dimensionless) &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return log(evaluate_def_(expression_, x));
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > FUNCTION_PRECEDENCE) {
      out << '(';
    }
    out << "log(";
    expression_->write(COMMA_PRECEDENCE, vars, out);
    out << ")";
    if (p > FUNCTION_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Negate_Expression : public Expression {
public:
  Negate_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), expression->units()),

        expression_(expression) {
    Require(expression);

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return -evaluate_def_(expression_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > NEGATE_PRECEDENCE) {
      out << '(';
    }
    out << "-";
    expression_->write(NEGATE_PRECEDENCE, vars, out);
    if (p > NEGATE_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Not_Expression : public Expression {
public:
  Not_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), dimensionless),

        expression_(expression) {
    Require(expression);

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return !evaluate_def_(expression_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > NOT_PRECEDENCE) {
      out << '(';
    }
    out << "!";
    expression_->write(NOT_PRECEDENCE, vars, out);
    if (p > NOT_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Or_Expression : public Expression {
public:
  Or_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), dimensionless),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) || evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > OR_PRECEDENCE) {
      out << '(';
    }
    e1_->write(OR_PRECEDENCE, vars, out);
    out << "||";
    e2_->write(OR_PRECEDENCE, vars, out);
    if (p > OR_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Power_Expression : public Expression {
public:
  Power_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(),
                   pow(e1->units(),
                       (*e2)(vector<double>(e2->number_of_variables(), 0.0)))),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(dimensionless, e2->units()));
    Require(e2->is_constant() || is_compatible(dimensionless, e1->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return std::pow(evaluate_def_(e1_, x), evaluate_def_(e2_, x));
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > FUNCTION_PRECEDENCE) {
      out << '(';
    }
    out << "pow(";
    e1_->write(COMMA_PRECEDENCE, vars, out);
    out << ",";
    e2_->write(COMMA_PRECEDENCE, vars, out);
    out << ")";
    if (p > FUNCTION_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Product_Expression : public Expression {
public:
  Product_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), e1->units() * e2->units()),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) * evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > PRODUCT_PRECEDENCE) {
      out << '(';
    }
    e1_->write(PRODUCT_PRECEDENCE, vars, out);
    out << "*";
    e2_->write(PRODUCT_PRECEDENCE, vars, out);
    if (p > PRODUCT_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Quotient_Expression : public Expression {
public:
  Quotient_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), e1->units() / e2->units()),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) / evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > QUOTIENT_PRECEDENCE) {
      out << '(';
    }
    e1_->write(PRODUCT_PRECEDENCE, vars, out);
    out << "/";
    e2_->write(QUOTIENT_PRECEDENCE, vars, out);
    if (p > QUOTIENT_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Sin_Expression : public Expression {
public:
  Sin_Expression(pE const &expression)
      : Expression(expression->number_of_variables(), dimensionless),

        expression_(expression) {
    Require(expression);
    Require(is_compatible(expression_->units(), dimensionless));

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return expression_ != SP<Expression>() &&
           is_compatible(expression_->units(), dimensionless) &&
           expression_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return sin(evaluate_def_(expression_, x));
  }

  virtual bool is_constant_(unsigned const i) const {
    return expression_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > FUNCTION_PRECEDENCE) {
      out << '(';
    }
    out << "sin(";
    expression_->write(COMMA_PRECEDENCE, vars, out);
    out << ")";
    if (p > FUNCTION_PRECEDENCE) {
      out << ')';
    }
  }

  pE expression_;
};

//---------------------------------------------------------------------------//
class Sum_Expression : public Expression {
public:
  Sum_Expression(pE const &e1, pE const &e2)
      : Expression(e1->number_of_variables(), e1->units()),

        e1_(e1), e2_(e2) {
    Require(e1);
    Require(e2);
    Require(is_compatible(e1->units(), e2->units()));
    Require(e1->number_of_variables() == e2->number_of_variables());

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const {
    return e1_ != SP<Expression>() && e2_ != SP<Expression>() &&
           e1_->number_of_variables() == number_of_variables() &&
           e2_->number_of_variables() == number_of_variables();
  }

private:
  virtual double evaluate_(double const *const x) const {
    return evaluate_def_(e1_, x) + evaluate_def_(e2_, x);
  }

  virtual bool is_constant_(unsigned const i) const {
    return e1_->is_constant(i) && e2_->is_constant(i);
  }

  virtual void write_(Precedence const p, vector<string> const &vars,
                      ostream &out) const {
    if (p > SUM_PRECEDENCE) {
      out << '(';
    }
    e1_->write(SUM_PRECEDENCE, vars, out);
    out << "+";
    e2_->write(SUM_PRECEDENCE, vars, out);
    if (p > SUM_PRECEDENCE) {
      out << ')';
    }
  }

  pE e1_, e2_;
};

//---------------------------------------------------------------------------//
class Variable_Expression : public Expression {
public:
  Variable_Expression(unsigned const index, unsigned const number_of_variables,
                      Unit const &units)
      : Expression(number_of_variables, units),

        index_(index) {
    Require(index < number_of_variables);

    Ensure(check_class_invariant());
  }

  //! Check the class invariant
  bool check_class_invariant() const { return index_ < number_of_variables(); }

private:
  virtual double evaluate_(double const *const x) const { return x[index_]; }

  virtual bool is_constant_(unsigned const i) const { return i != index_; }

  virtual void write_(Precedence, vector<string> const &vars,
                      ostream &out) const {
    Require(index_ < vars.size());

    out << vars[index_];
  }

  unsigned index_;
};

//---------------------------------------------------------------------------//
static pE parse_primary(unsigned const number_of_variables,
                        Variable_Map const &variable_map,
                        Token_Stream &tokens) {
  if (at_real(tokens)) {
    return pE(new Constant_Expression(number_of_variables, parse_real(tokens)));
  } else if (tokens.lookahead().text() == "(") {
    tokens.shift();
    pE Result = Expression::parse(number_of_variables, variable_map, tokens);
    if (tokens.shift().text() != ")") {
      tokens.report_syntax_error("missing ')'?");
    } else {
      return Result;
    }
  } else {
    Token const variable = tokens.shift();
    if (variable.type() != KEYWORD) {
      tokens.report_syntax_error("expression syntax error");
    }

    if (tokens.lookahead().text() == "(")
    // a function call
    {
      string name = variable.text();
      pE const argument =
          parse_primary(number_of_variables, variable_map, tokens);
      switch (name[0]) {
      case 'e':
        if (name == "exp") {
          return pE(new Exp_Expression(argument));
        } else {
          tokens.report_semantic_error("unrecognized function");
          return pE(new Constant_Expression(number_of_variables, 0.0));
        }
      case 'c':
        if (name == "cos") {
          return pE(new Cos_Expression(argument));
        } else {
          tokens.report_semantic_error("unrecognized function");
          return pE(new Constant_Expression(number_of_variables, 0.0));
        }
      case 'l':
        if (name == "log") {
          return pE(new Log_Expression(argument));
        } else {
          tokens.report_semantic_error("unrecognized function");
          return pE(new Constant_Expression(number_of_variables, 0.0));
        }
      case 's':
        if (name == "sin") {
          return pE(new Sin_Expression(argument));
        } else {
          tokens.report_semantic_error("unrecognized function");
          return pE(new Constant_Expression(number_of_variables, 0.0));
        }
      default:
        tokens.report_semantic_error("unrecognized function");
        return pE(new Constant_Expression(number_of_variables, 0.0));
      }
    } else
    // a variable or constant name
    {

      Variable_Map::const_iterator i = variable_map.find(variable.text());

      if (i != variable_map.end()) {
        return pE(new Variable_Expression(i->second.first, number_of_variables,
                                          (unit_expressions_are_required()
                                               ? i->second.second
                                               : dimensionless)));
      } else {
        static map<string, Unit> unit_map;
        if (unit_map.size() == 0) {
          unit_map["m"] = m;
          unit_map["kg"] = kg;
          unit_map["s"] = s;
          unit_map["A"] = A;
          unit_map["K"] = K;
          unit_map["mol"] = mol;
          unit_map["cd"] = cd;
          unit_map["rad"] = rad;
          unit_map["sr"] = sr;

          unit_map["C"] = C;
          unit_map["Hz"] = Hz;
          unit_map["N"] = N;
          unit_map["J"] = J;
          unit_map["Pa"] = Pa;
          unit_map["W"] = W;
          unit_map["V"] = V;
          unit_map["F"] = F;
          unit_map["ohm"] = ohm;
          unit_map["S"] = S;
          unit_map["Wb"] = Wb;
          unit_map["T"] = T;
          unit_map["H"] = H;
          unit_map["lm"] = lm;
          unit_map["lx"] = lx;

          unit_map["cm"] = cm;
          unit_map["g"] = g;
          unit_map["dyne"] = dyne;
          unit_map["erg"] = erg;

          unit_map["inch"] = inch;
          unit_map["foot"] = foot;
          unit_map["lbm"] = lbm;
          unit_map["pound"] = pound;

          unit_map["keV"] = keV;
        }

        map<string, Unit>::const_iterator i = unit_map.find(variable.text());

        if (i != unit_map.end()) {
          Unit units = i->second;
          units.conv *= conversion_factor(units, get_internal_unit_system());
          if (!unit_expressions_are_required()) {
            units = units.conv * dimensionless;
          }
          return pE(new Constant_Expression(number_of_variables, units));
        } else {
          tokens.report_semantic_error("undefined variable or unit");
          return pE(new Constant_Expression(number_of_variables, 0.0));
        }
      }
    }
  }
  // not reached
  return pE();
}

//---------------------------------------------------------------------------//
static pE parse_power(unsigned const number_of_variables,
                      Variable_Map const &variable_map, Token_Stream &tokens) {
  pE Result = parse_primary(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "^") {
    tokens.shift();
    pE exponent = parse_primary(number_of_variables, variable_map, tokens);
    if (!is_compatible(dimensionless, exponent->units())) {
      tokens.report_semantic_error("exponent must be dimensionless");
    } else if (!exponent->is_constant() &&
               !is_compatible(dimensionless, Result->units())) {
      tokens.report_semantic_error("base of non-constant exponent must"
                                   " be dimensionless");
    } else {
      Result.reset(new Power_Expression(Result, exponent));
    }
  }
  return Result;
}

//---------------------------------------------------------------------------//
static pE parse_unary(unsigned const number_of_variables,
                      Variable_Map const &variable_map, Token_Stream &tokens) {
  if (tokens.lookahead().text() == "+") {
    tokens.shift();
    // a no-op
    return parse_unary(number_of_variables, variable_map, tokens);
  } else if (tokens.lookahead().text() == "-") {
    tokens.shift();
    return pE(new Negate_Expression(
        parse_unary(number_of_variables, variable_map, tokens)));
  } else if (tokens.lookahead().text() == "!") {
    tokens.shift();
    return pE(new Not_Expression(
        parse_unary(number_of_variables, variable_map, tokens)));
  } else {
    return parse_power(number_of_variables, variable_map, tokens);
  }
}

//---------------------------------------------------------------------------//
static pE parse_multiplicative(unsigned const number_of_variables,
                               Variable_Map const &variable_map,
                               Token_Stream &tokens) {
  pE Result = parse_unary(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "*" || tokens.lookahead().text() == "/") {
    if (tokens.lookahead().text() == "*") {
      tokens.shift();
      Result.reset(new Product_Expression(
          Result, parse_unary(number_of_variables, variable_map, tokens)));
    } else {
      Check(tokens.lookahead().text() == "/");
      tokens.shift();
      Result.reset(new Quotient_Expression(
          Result, parse_unary(number_of_variables, variable_map, tokens)));
    }
  }
  return Result;
}

//---------------------------------------------------------------------------//
static pE parse_additive(unsigned const number_of_variables,
                         Variable_Map const &variable_map,
                         Token_Stream &tokens) {
  pE Result = parse_multiplicative(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "+" || tokens.lookahead().text() == "-") {
    if (tokens.lookahead().text() == "+") {
      tokens.shift();
      pE const Right =
          parse_multiplicative(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for +");
      } else {
        Result.reset(new Sum_Expression(Result, Right));
      }
    } else {
      Check(tokens.lookahead().text() == "-");
      tokens.shift();
      pE const Right =
          parse_multiplicative(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for -");
      } else {
        Result.reset(new Difference_Expression(Result, Right));
      }
    }
  }
  return Result;
}

//---------------------------------------------------------------------------//
static pE parse_relational(unsigned const number_of_variables,
                           Variable_Map const &variable_map,
                           Token_Stream &tokens) {
  pE Result = parse_additive(number_of_variables, variable_map, tokens);

  Token token = tokens.lookahead();
  while (token.text() == "<" || token.text() == ">" || token.text() == "<=" ||
         token.text() == ">=") {
    if (token.text() == "<") {
      tokens.shift();
      pE const Right =
          parse_additive(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for <");
      } else {
        Result.reset(new Less_Expression(Result, Right));
      }
    } else if (token.text() == "<=") {
      tokens.shift();
      pE const Right =
          parse_additive(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for <=");
      } else {
        Result.reset(new LE_Expression(Result, Right));
      }
    } else if (token.text() == ">") {
      tokens.shift();
      pE const Right =
          parse_additive(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for >");
      } else {
        Result.reset(new Greater_Expression(Result, Right));
      }
    } else {
      Check(token.text() == ">=");
      tokens.shift();
      pE const Right =
          parse_additive(number_of_variables, variable_map, tokens);
      if (!is_compatible(Result->units(), Right->units())) {
        tokens.report_semantic_error("unit incompatibility for >=");
      } else {
        Result.reset(new GE_Expression(Result, Right));
      }
    }
    token = tokens.lookahead();
  }
  return Result;
}

//---------------------------------------------------------------------------//
static pE parse_and(unsigned const number_of_variables,
                    Variable_Map const &variable_map, Token_Stream &tokens) {
  pE Result = parse_relational(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "&&") {
    tokens.shift();
    Result.reset(new And_Expression(
        Result, parse_relational(number_of_variables, variable_map, tokens)));
  }
  return Result;
}

//---------------------------------------------------------------------------//
static pE parse_or(unsigned const number_of_variables,
                   Variable_Map const &variable_map, Token_Stream &tokens) {
  pE Result = parse_and(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "||") {
    tokens.shift();
    Result.reset(new Or_Expression(
        Result, parse_relational(number_of_variables, variable_map, tokens)));
  }
  return Result;
}

//---------------------------------------------------------------------------//
/*!
 * \param x Variable values to apply to the expression. The values must be in
 * SI units.
 *
 * \return Evaluated value of the expression. The dimensions of this value are
 * specified by Expression::unit(). The value is returned in SI units.
 */

double Expression::operator()(vector<double> const &x) const {
  Insist(x.size() > 0,
         std::string("Expression::operator() requires a non-zero length ") +
             std::string("vector as an argument."));
  Require(x.size() == number_of_variables());
  return evaluate_(&x[0]);
}

//---------------------------------------------------------------------------//
/*!
 * \param number_of_variables Number of distinct independent variables in the
 * expression.
 *
 * \param variable_map Map specifying variable names and the associated index
 * and units. It is acceptable to alias variable names. The unit dimensions
 * must be identical for aliases. The conversion factor is ignored but should
 * be nonzero. 
 *
 * \param tokens Token stream from which to parse an Expression.
 *
 * \return Pointer to the Expression. If null, the expression was empty or
 * grammatically ill-formed.
 */

SP<Expression> Expression::parse(unsigned const number_of_variables,
                                 Variable_Map const &variable_map,
                                 Token_Stream &tokens) {
  // No index in the variable map can be greater than or equal to the number
  // of variables.

  // The top expression is the or expression, which we anticipate
  // will be useful for piecewise functions.

  SP<Expression> Result = parse_or(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "|") {
    tokens.shift();
    Result.reset(new Or_Expression(
        Result, parse_or(number_of_variables, variable_map, tokens)));
  }
  return Result;
}

//---------------------------------------------------------------------------//
/*!
 * \param number_of_variables Number of distinct independent variables in the
 * expression.
 *
 * \param variable_map Map specifying variable names and the associated index
 * and units. It is acceptable to alias variable names. The unit dimensions
 * must be identical for aliases. The conversion factor is ignored but should
 * be nonzero.
 *
 * \param expected_units Unit dimensions the final expression is expected to
 * have. If the final expression does not have these units, and if
 * unit checking is not disabled (as determined by a call to
 * rtt_parser::are_units_disabled()), then a semantic error will be reported
 * to tokens.
 *
 * \paran expected_units_text Human-friendly description of the units that
 * were expected, e.g. "force", "energy density"
 *
 * \param tokens Token stream from which to parse an Expression.
 *
 * \return Pointer to the Expression. If null, the expression was empty or
 * grammatically ill-formed.
 */

SP<Expression> Expression::parse(unsigned const number_of_variables,
                                 Variable_Map const &variable_map,
                                 Unit const &expected_units,
                                 string const &expected_units_text,
                                 Token_Stream &tokens) {
  // No index in the variable map can be greater than or equal to the number
  // of variables.

  // The top expression is the or expression, which we anticipate
  // will be useful for piecewise functions.

  SP<Expression> Result = parse_or(number_of_variables, variable_map, tokens);
  while (tokens.lookahead().text() == "|") {
    tokens.shift();
    Result.reset(new Or_Expression(
        Result, parse_or(number_of_variables, variable_map, tokens)));
  }
  if (unit_expressions_are_required() ||
      !is_compatible(Result->units(), dimensionless)) {
    tokens.check_semantics(
        is_compatible(Result->units(), expected_units),
        ("expected units of " + expected_units_text).c_str());
  }
  return Result;
}

//---------------------------------------------------------------------------//
void Expression::write(Precedence const p, vector<string> const &vars,
                       ostream &out) const {
  Require(vars.size() == number_of_variables());

  write_(p, vars, out);
}

} // end namespace rtt_parser

//---------------------------------------------------------------------------//
//                 end of Expression.cc
//---------------------------------------------------------------------------//
