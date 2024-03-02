// Declarations for parser.cc
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <memory>
#include <variant>
#include <vector>

#include "../src/ast/tree.h"
#include "../src/utility/exception.h"
#include "../src/utility/handle.h"
#include "../Include/parser_util.h"
#include "token.h"
class DonsusParser;

extern std::map<std::string, donsus_token_kind> DONSUS_TYPES_LEXER;

/**
 *  \brief Lexer API
 */
struct donsus_lexer {
  donsus_lexer(std::string input)
      : string(input), cur_pos(0), cur_line(1), cur_char(input[0]) {}
  donsus_lexer() = default;

  std::string string;
  char cur_char;
  unsigned int cur_pos, cur_line;
};

donsus_token donsus_lexer_next(DonsusParser &parser); // forward reference

class DonsusParser {
public:
  using parse_result = utility::handle<donsus_ast::node>;
  using end_result = utility::handle<donsus_ast::tree>;

  DonsusParser(donsus_lexer &lexer);

  // move to the next token
  donsus_token donsus_parser_next();
  // move to the next token and see if the provided type is correct
  void donsus_parser_except(donsus_token_kind type);

  /**
   *  \brief Parse down:
   *  function definitions:
   *      - statements
   *
   */
  auto donsus_parse() -> end_result;

  void print_token();
  donsus_token donsus_peek();

  // create node
  // parsing expression
  auto donsus_expr() -> parse_result;
  // parsing number expressions
  auto donsus_number_expr(unsigned int ptp) -> parse_result;
  auto donsus_number_primary(donsus_ast::donsus_node_type type,
                             uint64_t child_count) -> parse_result;

  // parsing variable declaration
  auto donsus_variable_decl() -> parse_result;
  auto create_variable_declaration(donsus_ast::donsus_node_type type,
                                   u_int64_t child_count) -> parse_result;

  // parsing variable definition
  auto donsus_variable_definition(parse_result &declaration) -> parse_result;

  auto create_number_expression(donsus_ast::donsus_node_type type,
                                u_int64_t child_count) -> parse_result;

  // Functions
  auto donsus_function_decl() -> parse_result;
  auto create_function_decl(donsus_ast::donsus_node_type type,
                            u_int64_t child_count) -> parse_result;

  auto donsus_function_definition() ->parse_result;

  auto donsus_function_signature() ->   std::vector<NAME_DATA_PAIR>;
  // peeking functions
  auto peek_is_function_definition() -> bool;
  auto peek_is_function_declaration() -> bool;

  donsus_token cur_token;
  donsus_lexer lexer;
  utility::handle<donsus_ast::tree> donsus_tree; // holds top level ast nodes
private:
  DonsusException error;
};
// debug
std::string de_get_name_from_token(donsus_token_kind kind);
#endif // PARSER_H