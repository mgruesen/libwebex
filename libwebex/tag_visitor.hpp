#ifndef WEBEX_TAG_VISITOR_HPP
#define WEBEX_TAG_VISITOR_HPP

#include "tag_ast.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

namespace webex
{

// Print tag visitor class.
struct Tag_print : Tag_visitor
{
  Tag_print(std::ostream& os)
    : ostream_(os)
  { }

  std::ostream& ostream() { return ostream_; }

  void eval(Source const*);
  void visit(Body const*);
  void visit(Heading const*);
  void visit(Literal const*);

  std::ostream& ostream_;
};


// Archive tag visitor class.
struct Tag_archive : Tag_visitor
{
  Tag_archive(std::ostream& os)
    : ostream_(os)
  { }

  std::ostream& ostream() { return ostream_; }

  void eval(Source const*);
  void visit(Body const*);
  void visit(Heading const*);
  void visit(Literal const*);

  std::ostream& ostream_;
};


// Search tag visitor class.
struct Tag_search : Tag_visitor
{
  using Result_type = std::vector<const Source*>;

  Tag_search(std::string const& query)
    : query_(query), results_()
  { }

  // Checks earch word in the given string for a match to the query.
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
  
  // Current result set.
  Result_type results() const { return results_; }

  void eval(Source const*);
  void visit(Body const*);
  void visit(Heading const*);
  void visit(Literal const*);

  std::string query_;
  Result_type results_;
};


} // end namespace webex

#endif
