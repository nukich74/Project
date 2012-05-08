#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <map>

int main(const int argc, const char** argv) {
	if (argc == 0) {
		throw("No input params\n");
	}
	char istr[256];
	std::ifstream tocheak(argv[2]);
	std::ifstream in(argv[1]);
	in.getline(istr, 256);
	if (strcmp(istr, "No solution") == 0) {
		std::cout << "Can't check solution\n";
	} else {
		std::map< char, char > table;
		table[0] = istr[2];
		while (in.getline(istr, 256)) {
			table[0] = istr[1];
		}
		int n;
		tocheak >> n;
		for (int i = 0; i < n; i++) {
			char v1, v2;
			std::string t;
			tocheak >> v1 >> v2 >> t;
			
			if (table[(v1 - '0') * 2 + (v2 - '0') * 1] == '0') {
				std::cout << "Error in solution\n";
			}
		}
		std::cout << "Correct solution\n";
	}
	return 0;
}
