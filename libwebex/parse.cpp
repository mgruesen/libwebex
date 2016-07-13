#include "parse.hpp"

#include <sstream>
#include <cctype>
#include <algorithm>
#include <iostream>

namespace webex
{


// Supported identifiers.
const std::string identifiers[] =
{
  "title",
  "body",
  "h1",
  "h2",
  "h3",
  "h4",
  "h5",
  "h6",
  "table",
  "tbody",
  "ul",
  "ol",
  "li",
  "p",
  "table",
  "tr",
  "td",
  "strong",
  "b",
};


// Containers.
const std::string containers[] =
{
  "body",
  "table",
  "tbody",
  "tr",
  "ol",
  "ul",
};


// Number of supported identifiers.
const int identifiers_size = 19;


// Number of supported containers.
const int containers_size = 6;


// Checks if the given string is a supported identifier.
inline bool
is_identifier(std::string const& str)
{
  for (int i = 0; i < identifiers_size; i++)
    if (str == identifiers[i])
      return true;
  return false;
}


// Check if the given identifier is a container.
inline bool
is_container(std::string const& str)
{
  for (int i = 0; i < containers_size; i++)
    if (str == containers[i])
      return true;
  return false;
}


// Check if the given character is an alphabetical character.
inline bool
is_letter(char c)
{
  return ::isalpha(c);
}


// Check if the given character is a numerical character.
inline bool
is_num(char c)
{
  return ::isdigit(c);
}


// Builds a vector of tokens from either a raw http request or a
// libwebex archive file.
std::vector<std::string>
lexer::build_tokens(std::string const& input)
{
  if (input[0] == '{')
    return build_json_tokens(input);
  else if (input[0] == '<')
    return build_html_tokens(input);
  else 
    throw std::string("Error (lexer.build_tokens): Unknown format detected. Expected '{' or '<', got '" +
      std::to_string(input[0]) +  "'");
}


// Builds a vector of tokens from a raw http request.
std::vector<std::string>
lexer::build_html_tokens(std::string const& input)
{
  // The input stream.
  std::stringstream stream(input);

  // Token list.
  std::vector<std::string> tokens;

  // Current character.
  char c;

  // Index of current character.
  int i = 0;

  // Buffer.
  std::string buf;

  while (stream.get(c)) {
    buf.clear();
    // Check if delim, (\n).
    if (c == '\n')
      continue;
    // Check if tag.
    if (c == '<') {
      // Get the tag identifier.
      while (stream.get(c)) {
        i++;
        if (is_letter(c) || is_num(c))
          buf += c;
        else
          break;
      }

      // Check if the found identifier is supported.
      if (is_identifier(buf)) {
        // Add new identifier to token list.
        bool container = is_container(buf);
        std::transform(buf.begin(), buf.end(), buf.begin(), [](unsigned char z){
          return std::toupper(z);
        });
        tokens.push_back(buf);

        // If a container type, restart loop.
        if (container)
          continue;

        // Clear buffer.
        buf.clear();

        // Skip to the end of the opening tag.
        while (c != '>') {
          stream.get(c);
          i++;
        }

        // Extract data until another closing or nested tag is found.
        while (stream.get(c)) {
          i++;
          if (c == '<') {
            // If closing tag, break.
            if (stream.get(c) && c == '/') {
              i++;
              break;
            }
            // Else it's a new, nested tag. Replace extracted character and '<',
            // process next tag.
            else {
              stream.unget();
              stream.putback('<');
              break;
            }
          } // end if tag.
          buf += c;
        }

        // Add the extracted data to the token list.
        tokens.push_back(buf);

        // Skip to the end of the closing tag.
        while (stream.get(c)) {
          i++;
          if (c == '>')
            break;
          if (c == '<') {
            stream.unget();
            break;
          }
        }
      } // end if identifier.
    } // end if tag.
  } // end input stream.

  return tokens;
} 


// Builds tokens from a libwebex archive file.
std::vector<std::string>
lexer::build_json_tokens(std::string const& input)
{
  // The input stream.
  std::stringstream stream(input);

  // Token list.
  std::vector<std::string> tokens;

  // Current character.
  char c;

  // Index of current character.
  int i = 0;

  // Buffer.
  std::string buf;

  while (stream.get(c)) {
    // Skip formatting characters.
    if (c == '\n' || c == ' ' || c == ',')
      continue;
    buf.clear();
    // Check if Section.
    if (c == '{') {
      // Skip to label value.
      while (stream.get(c) && c != ':') {
        i++;
      }
      // Consume opening '"' char.
      if (stream.get(c) && c == '\"') {
        i++;
        // Extract the label value.
        while (stream.get(c) && c != '\"') {
          i++;
          buf += c;
        }
      } // end label value.
      // Skip to precedence value.
      while (stream.get(c) && c != ':') {
        i++;
      }
      // Get precedence value.
      stream.get(c);
      i++;
      int prec = c - '0';
      // Generate proper section token.
      if (!prec)
        tokens.push_back("TITLE");
      else
        tokens.push_back(std::string("H" + std::to_string(prec)));
      tokens.push_back(buf);
      // Skip to data values.
      while (stream.get(c) && c != ':') {
        i++;
      }
    } // end if-section.
    // Check if List.
    else if (c == '[') {
      while (stream.get(c) && c != ':') {
        i++;
      }
      // Consume opening '"' char.
      if (stream.get(c) && c == '\"') {
        i++;
        while (stream.get(c) && c != '\"') {
          i++;
          buf += c;
        }
        // Generate a list token.
        tokens.push_back("UL");
        // Skip to list values.
        while (stream.get(c) && c != ':') {
          i++;
        }
        // Extract and generate list value tokens.
        while (stream.get(c) && c != ']') {
          i++;
          // Skip white space and commas.
          if (c == ' ' || c == ',')
            continue;
          buf.clear();
          if (c == '\"') {
            while (stream.get(c) && c != '\"') {
              i++;
              buf += c;
            }
            tokens.push_back("LI");
            tokens.push_back(buf);
          }
        } // end list-values.
      } // end if-list.
    }
  } // end input stream.

  return tokens;
}


namespace parser
{

// The parsing look ahead iterator.
std::vector<std::string>::const_iterator look_;

// Parses the input vector of nodes. Output is the head of the parse tree.
Source* build_tree(std::vector<std::string>const & input)
{
  Source* head;
  Source* curr;
  look_ = input.begin();
  try
  {
    head = title();
    curr = head;
  }
  catch (std::string msg) {
    std::cerr << msg << '\n';
    return nullptr;
  }
  catch (...) {
    std::cerr << "Unknown fatal error occurred.\n";
    return nullptr;
  }
  while (look_ != input.end()) {
    try
    {
      std::string token = *look_;
      if (token == "BODY")
        match("BODY");
      else if (token == "P") {
        Source* p = paragraph();
        if (p) {
          p->set_parent(curr);
          insert_content(curr, p);
        }
        else
          break;
      }
      else if (token[0] == 'H' && is_num(token[1])) {
        Source* h = heading();
        if (h) {
          insert_section(curr, h);
          curr = h;
        }
        else
          break;
      }
      else if (token == "UL" || token == "OL"){
        Source* lst = list();
        if (lst) {
          lst->set_parent(curr);
          insert_content(curr, lst);
        }
        else
          break;
      }
      else if (token == "TABLE"){
        Source* tbl = table();
        if (tbl) {
          tbl->set_parent(curr);
          insert_content(curr, tbl);
        }
        else
          break;
      }
      else
        throw std::string("Parsing Error: unknown expression '" + *look_ + "'");
    }
    catch (std::string msg) {
      std::cerr << msg << '\n';
      return nullptr;
    }
    catch (...) {
      std::cerr << "Unknown fatal error occurred.\n";
      return nullptr;
    }
  }

  return head;
}


// Matches the given symbol and advances the lookahead iterator.
void
match(std::string const& str)
{
  if (*look_ == str)
    ++look_;
  else
    throw std::string("Could not match '" + *look_ + "' with '" + str + "'");
}


// Adds content to the current section in scope.
void
insert_content(Source* curr, Source* cont)
{
  curr->add_data(cont);
}


// Adds a new section to the tree. If the new sections precedence is less than
// the current, we move up the tree to find the section with a precedence that
// is 1 less than the new section.
void
insert_section(Source* curr, Source* sect)
{
  while (curr->precedence() >= sect->precedence()) {
    curr = curr->parent();
  }
  sect->set_parent(curr);
  curr->add_data(sect);
}


// Parses a 'title' expression.
Source*
title()
{
  if (*look_ == "TITLE") {
    match("TITLE");
    return new Body(*look_++);
  }
  return nullptr;
}


// Parses a 'body' expression.
//
// TODO: Delete this.
Source* body() { return nullptr; }


// Parses a 'heading' expression.
Source*
heading()
{
  std::string token = *look_;
  if (token[0] == 'H' && is_num(token[1])) {
    int p = token[1] - '0';
    match(*look_);
    return new Heading(*look_++, p);
  }
  else {
    error("Parsing error: Expected 'H#', got '" + token + "'");
    return nullptr;
  }
}


// Parses a 'paragraph' expression.
Source*
paragraph()
{
  if (*look_ == "P") {
    match("P");
    return new Literal(new Value(*look_++));
  }
  else {
    error("Parsing error: Expected 'P', got '" + *look_ + "'");
    return nullptr;
  }
}


// Parses a 'list' expression.
Source*
list()
{
  if (*look_ == "UL") 
    match("UL");
  else if(*look_ == "OL")
    match("OL");
  else {
    error("Parsing error: Expected 'UL' or 'OL', got '" + *look_ + "'");
    return nullptr;
  }

  // Build a local buffer of list items.
  std::string buf;
  while (*look_ == "LI") {
    match("LI");
    buf += *look_++ + ";";
  }
  
  // Return a new list type.  
  return new Literal(new List("", buf));
}


// Parses a 'table' expression.
Source* table()
{
  if (*look_ == "TABLE") {
    match("TABLE");
    match("TBODY");
    std::vector<List*> data;
    int hdr_cols = 0;
    match("TR");
    int row = 1;
    while (*look_ == "TD") {
      match("TD");
      // FIXME: Lexer is producing an empty token.
      look_++;
      if (*look_ == "STRONG") {
        match("STRONG");
        data.push_back(new List(*look_++));
        hdr_cols++;
      }
      else {
        Value* v = new Value(*look_++);
        data[0]->add_data(v);
      }
    }
    while (*look_ == "TR") {
      match("TR");
      int col = 0;
      while (*look_ == "TD") {
        match("TD");
        if (*look_ == "STRONG")
          data.push_back(new List(*look_++));
        else {
          if (hdr_cols > 1)
            data[col]->add_data(new Value(*look_++));
          else
            data[row]->add_data(new Value(*look_++));
        }
        col++;
      }
      row++;
    }
    Table::Orientation orient = hdr_cols > 1 ? Table::Orientation::vertical : Table::Orientation::horizontal;
    return new Literal(new Table("", data, orient));
  }
  else {
    error("Parsing error: Expected 'TABLE', got '" + *look_ + "'");
    return nullptr;
  }
  return nullptr;
}


// Parsing error reporting.
void
error(std::string const& err)
{
  std::cerr << err << '\n';
}

} // end namespace parser.

} // end namespace webex
