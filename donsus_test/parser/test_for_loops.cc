#include "parser.h"
#include <gtest/gtest.h>

#include <iostream>

TEST(ForLoopTest, WholeForloopsTest) {
  std::string a = R"(
    for 1..10 {
        printf("in  loop: "); # it = current number
    }
  )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  std::string loop_variable =
      result->get_nodes()[0]->get<donsus_ast::for_loop>().loop_variable;
  std::string start = result->get_nodes()[0]
                          ->get<donsus_ast::for_loop>()
                          .iterator->get<donsus_ast::range_expr>()
                          .start->get<donsus_ast::number_expr>()
                          .value.value;
  std::string end = result->get_nodes()[0]
                        ->get<donsus_ast::for_loop>()
                        .iterator->get<donsus_ast::range_expr>()
                        .end->get<donsus_ast::number_expr>()
                        .value.value;

  std::vector<utility::handle<donsus_ast::node>> body =
      result->get_nodes()[0]->get<donsus_ast::for_loop>().body;

  EXPECT_EQ("it", loop_variable);
  EXPECT_EQ("1", start);
  EXPECT_EQ("10", end);
  EXPECT_EQ(1, body.size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION,
            body[0]->type.type);
}

TEST(ForLoopTest, ForRangeLoopWithName) {
  std::string a = R"(
    for number: 2..10 {
        printf("in  loop: "); # it = current number
    }
  )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  std::string loop_variable =
      result->get_nodes()[0]->get<donsus_ast::for_loop>().loop_variable;
  std::string start = result->get_nodes()[0]
                          ->get<donsus_ast::for_loop>()
                          .iterator->get<donsus_ast::range_expr>()
                          .start->get<donsus_ast::number_expr>()
                          .value.value;
  std::string end = result->get_nodes()[0]
                        ->get<donsus_ast::for_loop>()
                        .iterator->get<donsus_ast::range_expr>()
                        .end->get<donsus_ast::number_expr>()
                        .value.value;

  std::vector<utility::handle<donsus_ast::node>> body =
      result->get_nodes()[0]->get<donsus_ast::for_loop>().body;

  EXPECT_EQ("number", loop_variable);
  EXPECT_EQ("2", start);
  EXPECT_EQ("10", end);
  EXPECT_EQ(1, body.size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION,
            body[0]->type.type);
}

TEST(ForLoopTest, ForArrayLoopWithoutName) {
  std::string a = R"(
    my_array: int[2] = [1, 2];

    for my_array {
        printf("We got a : ", number);
    }
  )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  std::string loop_variable =
      result->get_nodes()[1]->get<donsus_ast::for_loop>().loop_variable;

  std::string array_name = result->get_nodes()[1]
                               ->get<donsus_ast::for_loop>()
                               .iterator->get<donsus_ast::identifier>()
                               .identifier_name;
  std::vector<utility::handle<donsus_ast::node>> array_body =
      result->get_nodes()[1]->get<donsus_ast::for_loop>().body;

  EXPECT_EQ("it", loop_variable);
  EXPECT_EQ("my_array", array_name);
  EXPECT_EQ(1, array_body.size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION,
            array_body[0]->type.type);
}

TEST(ForLoopTest, ForArrayLoopWithtName) {
  std::string a = R"(
    my_array: int[2] = [1, 2];

    for number: my_array {
        printf("We got a : ", number);
    }
  )";

  DonsusAstFile file;
  DonsusParser parser = Du_Parse(a, file);
  DonsusParser::end_result result = parser.donsus_parse();

  std::string loop_variable =
      result->get_nodes()[1]->get<donsus_ast::for_loop>().loop_variable;

  std::string array_name = result->get_nodes()[1]
                               ->get<donsus_ast::for_loop>()
                               .iterator->get<donsus_ast::identifier>()
                               .identifier_name;

  std::vector<utility::handle<donsus_ast::node>> array_body =
      result->get_nodes()[1]->get<donsus_ast::for_loop>().body;

  EXPECT_EQ("number", loop_variable);
  EXPECT_EQ("my_array", array_name);
  EXPECT_EQ(1, array_body.size());
  EXPECT_EQ(donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION,
            array_body[0]->type.type);
}