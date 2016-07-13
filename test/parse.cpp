#include "libwebex/parse.hpp"
#include "libwebex/tag_visitor.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Tests the lexing and parsing modules.
int
main(int argc, char** argv)
{
	if (argc < 2)
		return -1;


	// Lex input file.
	std::string input;
	std::ifstream input_file(argv[1]);
	if (input_file.is_open()) {
		std::string buf;
		while (getline(input_file, buf)) {
			input += buf;
			input += '\n';
			buf.clear();
		}
		input_file.close();

		// Build a token list.
		std::vector<std::string> token_list = webex::lexer::build_tokens(input);
		if (token_list.empty()) {
			std::cout << "Error: No token list generated.\n";
			return 1;
		}

		// Parse the token list into a tree.
		webex::Source* tree = webex::parser::build_tree(token_list);
		if (!tree) {
			std::cout << "Error: No parse tree generated.\n";
			return 2;
		}

		// Print the contents of the tree.
		webex::Tag_print printer(std::cout);
		printer.eval(tree);

		// Clean up.
		delete tree;
	}
	else
		std::cout << "Error: failed to open file '" << argv[1] << '\n';

	return 0;
}
