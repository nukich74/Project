#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <stdlib.h>

int main(const int argc, const char** argv) {
	if (argc == 1) {
		throw("No enought param\n");
	}
	
	srand(time(NULL));
	
	std::ofstream out(argv[1]);
	int p = atoi(argv[2]);
	int FuncCount = p;
	out << FuncCount << '\n';
	for (int i = 0; i < FuncCount; i++) {
		char c1 = (int)('a') + (rand() % 26);
		char c2 = (int)('a') + (rand() % 26);
		char ttable[4];
		ttable[0] = rand() % 2 == 0 ? '0' : '1';
		ttable[1] = rand() % 2 == 0 ? '0' : '1';
		ttable[2] = rand() % 2 == 0 ? '0' : '1';
		ttable[3] = rand() % 2 == 0 ? '0' : '1';
		out << c1 << ' ' << c2 << ' ' << ttable << '\n';
	}

	return 0;
}
