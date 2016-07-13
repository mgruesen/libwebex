#ifndef WEBEX_PARSE_HPP
#define WEBEX_PARSE_HPP

// Webex Parse
//
// This module is responsible for the lexing and parsing of HTML web 
// pages from plain text source.

#include "tag_ast.hpp"
#include "type_ast.hpp"

#include <vector>
#include <string>

namespace webex
{


namespace parser
{

// Parses the input vector of nodes. Output is the head of the parse 
// tree.
Source* build_tree(std::vector<std::string>const &);

// Matches the given symbol and advances the lookahead iterator.
void match(std::string const&);

// Inserts a given node into the parse tree.
void insert_content(Source*, Source*);
void insert_section(Source*, Source*);

// Parses a 'title' expression.
Source* title();
// Parses a 'body' expression.
Source* body();
// Parses a 'heading' expression.
Source* heading();
// Parses a 'paragraph' expression.
Source* paragraph();
// Parses a 'list' expression.
Source* list();
// Parses a 'table' expression.
Source* table();

// Parsing error reporting.
void error(std::string const&);


} // end namespace parse

namespace lexer
{

// Scans the given input string and builds lexical tokens. Output is 
// a vector of Sources (tokens).
std::vector<std::string> build_tokens(std::string const&);

std::vector<std::string> build_html_tokens(std::string const&);
std::vector<std::string> build_json_tokens(std::string const&);

// Lexical error reporting.
void error(std::string const&);

} // end namespace lex

// Check if the given string is an opening tag.
inline bool is_identifier(std::string const&);

// Check if the given identifier is a container.
inline bool is_container(std::string const&);

// Check if the given character is an alphabetical character.
inline bool is_letter(char);

// Check if the given character is a numerical character.
inline bool is_num(char);

} // end namespace wxl

#endif
