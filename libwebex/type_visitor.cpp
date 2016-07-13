#include "type_visitor.hpp"

namespace webex
{

/////////////////////////////////////////////////////////////////////
//                    Content Print Visitor                        //
/////////////////////////////////////////////////////////////////////
//
// The print visitor writes the value of the content type to the 
// output stream assigned in the constructor.s

// Prints the table label and data.
void
Content_print::visit(Table const* tbl)
{
  // Print table label.
  this->ostream() << "<table>\n";

  auto data = tbl->data();
  if (tbl->orientation() == Table::Orientation::horizontal) {
    // Print each list in the table.
    for (List* l : data) {
      // Print the lists label (row title).
      this->ostream() << l->label() << " ";
      // Print each value in the list.
      for (Value* v : l->data())
        this->ostream() << v->data() << ' ';
      this->ostream() << "\n";
    }
  }
  else if (tbl->orientation() == Table::Orientation::vertical) {
    this->ostream() << '\n';
    int num_rows = data[0]->data().size();
    for (int i = 0; i < num_rows; i++) {
      for (List* l : data) {
        this->ostream() << l->label() << " : ";
        this->ostream() << l->data()[i]->data() << "\n";
      }
      this->ostream() << "\n\n";
    }
  }
}


// Prints the list label and data.
void
Content_print::visit(List const* lst)
{
  // Print the list label.
  this->ostream() << "[list]\n";

  // Visit each value in the list.
  for (Value* v : lst->data()) {
    this->ostream() << "-";
    this->visit(v);
  }
}


// Prints the value data.
void
Content_print::visit(Value const* val)
{
  this->ostream() << val->data() << "\n";
}


// Prints a content type.
void
Content_print::eval(Content const* con)
{
  con->accept(*this);
}


/////////////////////////////////////////////////////////////////////
//                    Content Archive Visitor                      //
/////////////////////////////////////////////////////////////////////
// The archive visitor serializes the content types value to a JSON 
// string and writes it to the output stream assigned in the 
// constructor.


// Prints the table label and data.
void
Content_archive::visit(Table const* tbl)
{
  // Print table label.
  this->ostream() << "< \"LABEL\":\"" << tbl->label() << "\", \"DATA\":";

  auto data = tbl->data();
  // Print each list in the table.
  for (unsigned int i = 0; i < data.size(); i++) {
    this->visit(data[i]);
    if (i + 1 < data.size())
      this->ostream() << ", ";
  }
  this->ostream() << " >";
}


// Prints the list label and data.
void
Content_archive::visit(List const* lst)
{
  // Print the list label.
  this->ostream() << "[ \"LABEL\":\"" << lst->label() << "\", \"DATA\":";
  auto data = lst->data();
  // Visit each value in the list.
  for (unsigned int i = 0; i < data.size(); i++) {
    this->visit(data[i]);
    if (i + 1 < data.size())
      this->ostream() << ", ";
  }
  this->ostream() <<  " ]";
}


// Prints the value data.
void
Content_archive::visit(Value const* val)
{
  this->ostream() << "\"" << val->data() << "\"";
}


// Prints a content type.
void
Content_archive::eval(Content const* con)
{
  con->accept(*this);
}


/////////////////////////////////////////////////////////////////////
//                    Content Search Visitor                       //
/////////////////////////////////////////////////////////////////////
// The search visitor checks each word in a content types value for
// a string matching the query given in the visitor's constructor.

// Searches the table label and data.
void
Content_search::visit(Table const* tbl)
{
  if (match(tbl->label()))
    found_ = true;
  else {
    for (List* l : tbl->data()) {
      eval(l);
      if (found_)
        break;
    }
  }
  return;
}


// Searches the list label and data.
void
Content_search::visit(List const* lst)
{
  if (match(lst->label()))
    found_ = true;
  else {
    for (Value* v : lst->data()) {
      eval(v);
      if (found_)
        break;
    }
  }
  return;
}


// Searches the value data.
void
Content_search::visit(Value const* val)
{
  found_ = match(val->data());
  return;
}


// Searches a content type.
void
Content_search::eval(Content const* con)
{
  con->accept(*this);
  return;
}


} // end namespace webex