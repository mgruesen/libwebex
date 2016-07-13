#ifndef WEBEX_TAG_AST_HPP
#define WEBEX_TAG_AST_HPP

#include "type_ast.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace webex
{

struct Node;
struct Source;
struct Body;
struct Heading;
struct Literal;
struct Content;


// Base tag visitor class.
struct Tag_visitor
{
  //virtual Source* visit(Source const*) = 0;
  virtual void visit(Body const*) = 0;
  virtual void visit(Heading const*) = 0;
  virtual void visit(Literal const*) = 0;
};

// Base source class. Follows the given grammar below.
//
// source     =>  source + body     |
//                content           |
// body     =>    body + heading    |
//                body + content    |
//                heading           |
//                content
// heading  =>    heading + content |
//                content
struct Source
{
  // Source ctors.
  //
  Source()
    : Source(-1)
  { }

  Source(std::string const& label, int prec)
    : Source(label, prec, nullptr, nullptr)
  { }

  Source(int prec, Source* parent = nullptr, Content* content = nullptr)
    : Source("", prec, parent, content)
  { }

  Source(std::string const& label, int prec, Source* parent, Content* content)
    : data_(), parent_(parent), label_(label), prec_(prec), content_(content)
  { }

  // Virtual source dtor.
  virtual ~Source() { data_.clear(); }

  // Virtual accept function for visitor.
  virtual void accept(Tag_visitor&) const = 0;

  // Accessors.
  //
  // Returns an iterator to the sources data.
  std::vector<Source*> data() { return data_; }
  std::vector<Source*> data() const { return data_; };
  // Returns the sources label.
  std::string label() const { return label_; }
  // Returns the nodes precedence.
  int precedence() const { return prec_; }
  // Returns the nodes parent.
  Source* parent() { return parent_; }
  Source* parent() const { return parent_; }
  // Returns the sources content node.
  Content* content() { return content_; }
  Content* content() const { return content_; }

  // Mutators.
  //
  // Adds an entry to the sources data.
  void add_data(Source* n) { data_.push_back(n); }
  // Sets the nodes parent.
  void set_parent(Source* p) { parent_ = p; }
  // Data Members.
  //
  // The sources data.
  std::vector<Source*> data_;
  // The sources parent node.
  Source* parent_;
  // The sources value.
  std::string label_;
  // The sources precedence.
  int   prec_;
  // The sources content.
  Content* content_;
};


// The Body class.
//
// body     =>  body + heading    |
//              body + content    |
//              heading           |
//              content
struct Body : Source
{
  Body(std::string const& val, int prec = 0)
    : Source(val, prec)
  { }

  void accept(Tag_visitor& vis) const { return vis.visit(this); }
};


// The Heading class.
//
// heading  =>  heading + content |
//              content
struct Heading : Source
{
  Heading(std::string const& val, int prec)
    : Source(val, prec)
  { }

  void accept(Tag_visitor& vis) const { return vis.visit(this); }
};


// The literal class.
//
// literal => content.
struct Literal : Source
{
  Literal(Content* c)
    : Source(7, nullptr, c)
  { }

  void accept(Tag_visitor& vis) const { return vis.visit(this); }
};

} // end namespace webex

#endif
