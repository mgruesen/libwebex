#ifndef WEBEX_TYPE_AST_HPP
#define WEBEX_TYPE_AST_HPP

#include <string>
#include <vector>
#include <iostream>

namespace webex
{

// Forward decls.
struct Content;
struct Table;
struct List;
struct Value;


// Base content visitor class.
struct Content_visitor
{
  virtual void visit(Table const*) = 0;
  virtual void visit(List const*) = 0;
  virtual void visit(Value const*) = 0;
  virtual void eval(Content const*) = 0;
};


// The base content class.
// content  =>  table     |
//              value
// table    =>  list+     |
//              list
// list     =>  value+    |
//              value
// value    =>  string
struct Content
{
  // Content ctor, virtual dtor.
  Content() { }
  virtual ~Content() { }

  // Virtual accept function for visitor.
  virtual void accept(Content_visitor&) const = 0;
};


// The List content class.
struct List : Content
{
  // List ctors.
  List(std::string const&);
  List(std::string const&, std::string const&);

  // Accept function for visitor.
  void accept(Content_visitor& cv) const { return cv.visit(this); }

  // Accessors.
  //
  // Returns the list's label.
  std::string label() const { return label_; }
  // Returns the list's data.
  std::vector<Value*> data() const { return data_; }

  // Mutators.
  //
  // Construct a new list of items from the raw data.
  void normalize();
  // Adds a new item to the list.
  void add_data(Value*);

  // Data Members.
  //
  // The list's label.
  std::string       label_;
  // The list's raw, unprocessed data.
  std::string       raw_;
  // The list's normalized data.
  std::vector<Value*> data_;
};


// The Table content class.
struct Table : Content
{
  enum Orientation { horizontal, vertical };
  Table(std::string const& lbl)
    : label_(lbl), data_()
  { }

  Table(std::string const& lbl, std::vector<List*>& data, Orientation orient)
    : label_(lbl), data_(data), orient_(orient)
  { 
    normalize();
  }

  // Accept function for visitor.
  void accept(Content_visitor& cv) const { return cv.visit(this); }

  // Accessors.
  //
  // Returns the table's label.
  std::string label() const { return label_; }
  // Returns the table's data.
  std::vector<List*> data() { return data_; }
  std::vector<List*> data() const { return data_; }
  // Returns the table's orientation.
  Orientation orientation() const { return orient_; }

  // Mutators.
  //
  // Adds an item to the list at the given index.
  void add_data(int idx, Value* v) { data_[idx]->add_data(v); }
  // Construct a new table from the raw data.
  void normalize();

  // Data Members.
  //
  // The table's label.
  std::string        label_;
  // The table's normalized data.
  std::vector<List*> data_;
  // The table's orientation.
  Orientation orient_;
};





// The Value content class.
struct Value : Content
{
  // Value ctor.
  Value(std::string const& val)
    : data_(val)
  { }

  // Accept function for visitor.
  void accept(Content_visitor& cv) const { return cv.visit(this); }

  // Accessors.
  //
  // Returns the value's data.
  std::string data() const { return data_; }

  // Data Members.
  //
  // The values data.
  std::string data_;
};

} // end namespace webex

#endif
