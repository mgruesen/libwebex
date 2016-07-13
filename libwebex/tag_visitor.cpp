#include "tag_visitor.hpp"
#include "type_visitor.hpp"

namespace webex
{

/////////////////////////////////////////////////////////////////////
//                         Tag Print Visitor                       //
/////////////////////////////////////////////////////////////////////
// The print visitor writes the nodes label and content values to the
// output stream assigned in the constructor.

// Prints a source node.
void
Tag_print::eval(Source const* s)
{
  s->accept(*this);
}


// Prints the title and content of the source page.
void
Tag_print::visit(Body const* b)
{
  std::cout << "{" << b->label() << ":" << b->precedence() << "}\n";
  for (auto data : b->data())
    eval(data);
}


// Prints the title of the section and its content.
void
Tag_print::visit(Heading const* h)
{
  std::cout << "{" << h->label() << ":" << h->precedence() << "}\n";
  for (auto data : h->data())
    eval(data);
}


// Prints the value of the content type.
void
Tag_print::visit(Literal const* l)
{
  Content_print cv(this->ostream());
  cv.eval(l->content());
}


/////////////////////////////////////////////////////////////////////
//                         Tag Archive Visitor                     //
/////////////////////////////////////////////////////////////////////
// The archive visitor serializes the nodes label and content values
// to a JSON string and writes them to the output stream given in
// the constructor.

// Serializes a source node to a JSON string.
void
Tag_archive::eval(Source const* s)
{
  s->accept(*this);
}


// Serializes the title and content of the source page.
void
Tag_archive::visit(Body const* b)
{
  this->ostream() << "{ \"LABEL\":\"" << b->label() << "\", \"PREC\":" << 
    b->precedence() << ", \"DATA\":";
  auto data = b->data();
  for (unsigned int i = 0; i < data.size(); i++) {
    eval(data[i]);
    if (i + 1 < data.size())
      this->ostream() << ", ";
  }
  this->ostream() << " }";
}


// Seralizes the title and content of the section.
void
Tag_archive::visit(Heading const* h)
{
  this->ostream() << "{ \"LABEL\":\"" << h->label() << "\", \"PREC\":" << 
    h->precedence() << ", \"DATA\":";
  auto data = h->data();
  for (unsigned int i = 0; i < data.size(); i++) {
    eval(data[i]);
    if (i + 1 < data.size())
      this->ostream() << ", ";
  }
  this->ostream() << " }";
}


// Serializes the value of the content type.
void
Tag_archive::visit(Literal const* l)
{
  Content_archive cv(this->ostream());
  cv.eval(l->content());
}


/////////////////////////////////////////////////////////////////////
//                         Tag Search Visitor                      //
/////////////////////////////////////////////////////////////////////
// The search visitor checks the label and content values for each
// node for a string matching the query given in the constructor.

// Searches a source node for a match.
void
Tag_search::eval(Source const* s)
{
  s->accept(*this);
  return;
}


// Search the title and content of the source page.
void
Tag_search::visit(Body const* b)
{
  if (match(b->label()))
    results_.push_back(b);
  else {
    for (Source* s : b->data())
      eval(s);
  }
  return;
}


// Searches the title and content of a section.
void
Tag_search::visit(Heading const* h)
{
  if (match(h->label()))
    results_.push_back(h);
  else {
    for (Source* s : h->data())
      eval(s);
  }
  return;
}


// Searches the value of the content type.
void
Tag_search::visit(Literal const* l)
{
  Content_search cs(query_);
  cs.eval(l->content());
  if (cs.found())
    results_.push_back(l);
  return;
}


} // end namespace webex