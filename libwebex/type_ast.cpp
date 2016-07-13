#include "type_ast.hpp"

namespace webex
{


// List ctors.
List::List(std::string const& lbl)
  : label_(lbl), raw_()
{ }

List::List(std::string const& lbl, std::string const& val)
  : label_(lbl), raw_(val)
{ 
  normalize();
}


// Converts the raw input string into a list of content values.
void
List::normalize()
{ 
  std::size_t beg = 0;
  std::size_t end = 0;
  while ((end = raw_.find(";", beg)) != std::string::npos) {
    data_.push_back(new Value(raw_.substr(beg, end - beg)));
    beg = end + 1;
  }
}


// Adds a content value to the list.
void
List::add_data(Value* val)
{
  data_.push_back(val);
}


// TODO: Delete this.
void
Table::normalize()
{
  
}




} // end namespace webex
