#include "libwebex/parse.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


// Tests the lexing module.
int
main(int argc, char** argv)
{
	if (argc < 2)
		return -1;

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

		std::vector<std::string> token_list = webex::lexer::build_tokens(input);
		for (unsigned int i = 0; i < token_list.size(); i++)
			std::cout << token_list[i] << " ";
		std::cout << '\n';
	}
	else
		std::cout << "Error: failed to open file '" << argv[1] << '\n';

  return 0;
}
