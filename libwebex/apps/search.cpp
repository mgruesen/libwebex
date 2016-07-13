#include "libwebex/parse.hpp"
#include "libwebex/tag_visitor.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


// Reports appropriate application usage.
void
usage(char* appname)
{
	std::cout << "Error: Invalid arguments. Usage is...\n";
	if (*appname == '.')
		appname++;
	if (*appname == '/')
		appname++;
	std::cout << "	" << appname << " [ input_file.txt | input_file.html | input_file.wbxa ] [\"string to search for\"]\n";
	std::cout << "	" << appname << " [http request host] [http request path] [\"string to search for\"]\n"; 
}


// Parses an input file.
bool
parse_input_file(char* fname, std::string& input)
{
	std::ifstream input_file(fname);
	if (input_file.is_open()) {
		std::string buf;
		while (getline(input_file, buf)) {
			input += buf;
			input += '\n';
			buf.clear();
		}
		input_file.close();
		return true;
	}
	else
		return false;
}


// Fetches an http request.
bool
get_http_request(char* host, char* path, std::string& input)
{
	std::cout << "Error: Not currently implemented.\n";
	return false;
}


// The main search driver.
int
main(int argc, char** argv)
{
	if (argc < 3) {
		usage(argv[0]);
		return -1;
	}
	
	// Web page input source.
	std::string input;
	std::string query;
	if (argc == 3) {
		// Read from file.
		if (!parse_input_file(argv[1], input)) {
			std::cout << "Error: Unable to open file '" << argv[1] << "'\n";
			return 1;
		}
		query = argv[2];
	}
	else if (argc == 4) {
		// Fetch an http request.
		if (!get_http_request(argv[1], argv[2], input)) {
			std::cout << "Error: Unable to get path '" << argv[2] << "' from host '" << argv[1] << "'\n";
			return 1;
		}
		query = argv[3];
	}
	
	// Build a token list.
	std::vector<std::string> token_list = webex::lexer::build_tokens(input);
	if (token_list.empty()) {
		std::cout << "Error: No token list generated.\n";
		return 2;
	}
	std::cout << "Token list generated.\n";

	// Parse the token list into a tree.
	webex::Source* tree = webex::parser::build_tree(token_list);
	if (!tree) {
		std::cout << "Error: No parse tree generated.\n";
		return 3;
	}
	std::cout << "Parse tree generated.\n";
	
	// Print the contents of the tree.
	webex::Tag_search searcher(query);
	std::cout << "Searching tree for content related to '" << query << "'...";
	searcher.eval(tree);
	auto results = searcher.results();
	std::cout << "done!\nFound " << results.size() << " possible hits:\n";
	webex::Tag_print printer(std::cout);
	for (unsigned i = 0; i < results.size(); i++) {
		std::cout << "Hit[" << i + 1 << "]:\n";
		printer.eval(results[i]);
		std::cout << "\n\n";
	}

	// Clean up.
	delete tree;

	return 0;
}
