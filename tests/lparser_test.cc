
#include "gtest/gtest.h"
#include "parser/lparser.h"
#include "parser/lstmt_to_string.h"

static void test_parse_stmt(const char* input, const char* expected, ltoken_type_t type) {
  lparser_t p;
  lstmt_t* stmt = NULL;
  lstr_t* s = lstr_create(100);

  lparser_init(&p, input);
  stmt = lparser_parse_declaration(&p);
  ASSERT_EQ(stmt->type, type);

  ASSERT_EQ(lstmt_to_string(stmt, s), RET_OK);
  ASSERT_STREQ(s->str, expected);

  LOBJECT_UNREF(s);
  lstmt_destroy(stmt);
  lparser_deinit(&p);
}

static void test_parse_expr(const char* input, const char* expected, ltoken_type_t type) {
  lparser_t p;
  lexpr_t* expr = NULL;
  lstr_t* s = lstr_create(100);

  lparser_init(&p, input);
  expr = lparser_parse_expr(&p);
  ASSERT_EQ(expr->type, type);

  ASSERT_EQ(lexpr_to_string(expr, s), RET_OK);
  ASSERT_STREQ(s->str, expected);

  LOBJECT_UNREF(s);
  lexpr_destroy(expr);
  lparser_deinit(&p);
}

TEST(LParser, true) {
  test_parse_expr("true", "true", LTOKEN_TRUE);
}

TEST(LParser, false) {
  test_parse_expr("false", "false", LTOKEN_FALSE);
}

TEST(LParser, LTOKEN_PLUS_EQUAL) {
  test_parse_expr("a+=1", "a=a+1", LTOKEN_EQUAL);
}

TEST(LParser, LTOKEN_MINUS_EQUAL) {
  test_parse_expr("a-=1", "a=a-1", LTOKEN_EQUAL);
}

TEST(LParser, LTOKEN_STAR_EQUAL) {
  test_parse_expr("a*=1", "a=a*1", LTOKEN_EQUAL);
}

TEST(LParser, LTOKEN_SLASH_EQUAL) {
  test_parse_expr("a/=1", "a=a/1", LTOKEN_EQUAL);
}

TEST(LParser, nil) {
  test_parse_expr("nil", "nil", LTOKEN_NIL);
}

TEST(LParser, number) {
  test_parse_expr("123", "123", LTOKEN_NUMBER);
}

TEST(LParser, string) {
  test_parse_expr("\"abc\"", "\"abc\"", LTOKEN_STRING);
}

TEST(LParser, id) {
  test_parse_expr("abc", "abc", LTOKEN_IDENTIFIER);
}

TEST(LParser, this) {
  test_parse_expr("this", "this", LTOKEN_THIS);
}

TEST(LParser, super) {
  test_parse_expr("super.test", "super.test", LTOKEN_GET);
}

TEST(LParser, super_set) {
  test_parse_expr("super.test=1", "super.test=1", LTOKEN_SET);
}

TEST(LParser, call1) {
  test_parse_expr("eat( )", "eat()", LTOKEN_CALL_FUNC);
}

TEST(LParser, call2) {
  test_parse_expr("eat(11)", "eat(11)", LTOKEN_CALL_FUNC);
}

TEST(LParser, call3) {
  test_parse_expr("eat(11,22)", "eat(11,22)", LTOKEN_CALL_FUNC);
}

TEST(LParser, obj_call1) {
  test_parse_expr("person.eat()", "person.eat()", LTOKEN_CALL_FUNC);
}

TEST(LParser, unary) {
  test_parse_expr("-123", "-123", LTOKEN_MINUS);
}

TEST(LParser, unary1) {
  test_parse_expr("!-123", "!-123", LTOKEN_NOT);
}

TEST(LParser, factor) {
  test_parse_expr("123*456", "123*456", LTOKEN_STAR);
}

TEST(LParser, term) {
  test_parse_expr("123+456", "123+456", LTOKEN_PLUS);
}

TEST(LParser, factor_term) {
  test_parse_expr("123*2+456*3", "123*2+456*3", LTOKEN_PLUS);
}

TEST(LParser, group_factor_term) {
  test_parse_expr("(123+2)*(456+3)", "(123+2)*(456+3)", LTOKEN_STAR);
}

TEST(LParser, group) {
  test_parse_expr("(123)", "(123)", LTOKEN_GROUP);
}

TEST(LParser, group1) {
  test_parse_expr("(123*456*789)", "(123*456*789)", LTOKEN_GROUP);
}

TEST(LParser, compare) {
  test_parse_expr("(1+2)<(3+4)", "(1+2)<(3+4)", LTOKEN_LESS);
}

TEST(LParser, compare1) {
  test_parse_expr("(1+2)<=(3+4)", "(1+2)<=(3+4)", LTOKEN_LESS_EQUAL);
}

TEST(LParser, equality1) {
  test_parse_expr("(1+2)<(3+4)==(1+2)<(3+4)", "(1+2)<(3+4)==(1+2)<(3+4)", LTOKEN_EQUAL_EQUAL);
}

TEST(LParser, equality2) {
  test_parse_expr("(1+2)<(3+4)!=(1+2)<(3+4)", "(1+2)<(3+4)!=(1+2)<(3+4)", LTOKEN_NOT_EQUAL);
}

TEST(LParser, and) {
  test_parse_expr("(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)", "(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)", LTOKEN_AND);
}

TEST(LParser, or) {
  test_parse_expr("(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)||(1+2)<(3+4)!=(1+2)",
                  "(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)||(1+2)<(3+4)!=(1+2)", LTOKEN_OR);
}

TEST(LParser, asign1) {
  test_parse_expr("a=123", "a=123", LTOKEN_EQUAL);
}

TEST(LParser, asign2) {
  test_parse_expr("a.b=123", "a.b=123", LTOKEN_SET);
}

TEST(LParser, expr) {
  test_parse_expr("a.b=(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)||(1+2)<(3+4)!=(1+2)",
                  "a.b=(1+2)<(3+4)!=(1+2)<(3+4)&&(1<2)||(1+2)<(3+4)!=(1+2)", LTOKEN_SET);
}

TEST(LParser, var) {
  test_parse_stmt("{\nvar a=123}\n", "{\nvar a=123\n}\n", LTOKEN_BLOCK);
}

TEST(LParser, if) {
  test_parse_stmt("if(1) {\nvar a=123\n}\n", "if(1) {\nvar a=123\n}\n", LTOKEN_IF);
}

TEST(LParser, if_else) {
  test_parse_stmt("if(1) {\nvar a=123\n} else {\n var b=123\n}\n",
                  "if(1) {\nvar a=123\n}\n else {\nvar b=123\n}\n", LTOKEN_IF);
}

TEST(LParser, while) {
  test_parse_stmt("while(1) {\nvar a=123\n}", "while(1) {\nvar a=123\n}\n", LTOKEN_WHILE);
}

TEST(LParser, while_break) {
  test_parse_stmt("while(1) {\nvar a=123\nbreak}", "while(1) {\nvar a=123\nbreak\n}\n",
                  LTOKEN_WHILE);
}

TEST(LParser, while_continue) {
  test_parse_stmt("while(1) {\nvar a=123\ncontinue}", "while(1) {\nvar a=123\ncontinue\n}\n",
                  LTOKEN_WHILE);
}

TEST(LParser, block) {
  test_parse_stmt("{\nvar a=123\n}", "{\nvar a=123\n}\n", LTOKEN_BLOCK);
}

TEST(LParser, for0) {
  test_parse_stmt("for(;;) {\nvar a=123\n}", "for(;;) {\nvar a=123\n}\n", LTOKEN_FOR);
}

TEST(LParser, for1) {
  test_parse_stmt("for(i=0;;) {\nvar a=123\n}", "for(i=0;;) {\nvar a=123\n}\n", LTOKEN_FOR);
}

TEST(LParser, for2) {
  test_parse_stmt("for(i=0;i<100;) {\nvar a=123\n}", "for(i=0;i<100;) {\nvar a=123\n}\n",
                  LTOKEN_FOR);
}

TEST(LParser, for3) {
  test_parse_stmt("for(i=0;i<100;i=i+1) {\nvar a=123\n}", "for(i=0;i<100;i=i+1) {\nvar a=123\n}\n",
                  LTOKEN_FOR);
}

TEST(LParser, return ) {
  test_parse_stmt("return 1", "return 1", LTOKEN_RETURN);
}

TEST(LParser, return1) {
  test_parse_stmt("return", "return ", LTOKEN_RETURN);
}

TEST(LParser, return2) {
  test_parse_stmt("return;", "return ", LTOKEN_RETURN);
}

TEST(LParser, func0) {
  test_parse_stmt("fun test() {return 0};", "fun test(){\nreturn 0\n}\n", LTOKEN_FUN);
}

TEST(LParser, func1) {
  test_parse_stmt("fun test(a) {return a};", "fun test(a){\nreturn a\n}\n", LTOKEN_FUN);
}

TEST(LParser, func2) {
  test_parse_stmt("fun test(a, b) {return a + b};", "fun test(a,b){\nreturn a+b\n}\n", LTOKEN_FUN);
}

TEST(LParser, class0) {
  test_parse_stmt("class test {};", "class test{\n}\n", LTOKEN_CLASS);
}

TEST(LParser, class1) {
  test_parse_stmt("class test < base {};", "class test<base{\n}\n", LTOKEN_CLASS);
}

TEST(LParser, class2) {
  test_parse_stmt("class test < base {func1(a, b){return a+b}};",
                  "class test<base{\nfunc1(a,b){\nreturn a+b\n}\n\n}\n", LTOKEN_CLASS);
}

TEST(LParser, class3) {
  test_parse_stmt("class test < base {func1(a, b){return a+b}func2(a){return a}};",
                  "class test<base{\nfunc1(a,b){\nreturn a+b\n}\n\nfunc2(a){\nreturn a\n}\n\n}\n",
                  LTOKEN_CLASS);
}
