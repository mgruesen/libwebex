#ifndef WEBEX_TYPE_VISITOR_HPP
#define WEBEX_TYPE_VISITOR_HPP

#include "type_ast.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

namespace webex
{

// Printing content visitor.
struct Content_print : Content_visitor
{
  // Ctor. Takes an output stream reference that gets written to.
  Content_print(std::ostream& ostream)
    : ostream_(ostream)
  { }

  std::ostream& ostream() { return ostream_; }

  void visit(Table const*);
  void visit(List const*);
  void visit(Value const*);
  void eval(Content const*);

  std::ostream& ostream_;
};

// Archiving content visitor.
struct Content_archive : Content_visitor
{
  // Ctor. Takes an output stream reference that gets written to.
  Content_archive(std::ostream& ostream)
    : ostream_(ostream)
  { }

  std::ostream& ostream() { return ostream_; }

  void visit(Table const*);
  void visit(List const*);
  void visit(Value const*);
  void eval(Content const*);

  // The visitor's output stream.
  std::ostream& ostream_;
};


// Searching content visitor.
struct Content_search : Content_visitor
{
  // Ctor. Takes a string that acts as the query parameter.
  Content_search(std::string const& query)
    : query_(query), found_(false)
  { }

  std::string query() const { return query_; }

  // Checks each word in the given string for a match to the query.
  bool match(std::string const& str) 
  { 
    std::stringstream stream(str);
    std::string word;
    while (stream >> word) {
      if (!strcasecmp(word.c_str(), query_.c_str())) 
        return true;
    }
    return false;
  }

  // Returns the current match state.
  bool found() const { return found_; }

  void visit(Table const*);
  void visit(List const*);
  void visit(Value const*);
  void eval(Content const*);

  // The query given.
  std::string query_;
  // The current match state.
  bool        found_;
};


} // end namespace webex

#endif