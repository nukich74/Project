#include <iostream>
#include <fstream>

int main(const int argc, const char** argv) {
	if (argc <= 1) {
		throw "No enought params";
	} 
	
	std::ifstream in(argv[1]);
	std::ifstream ans(argv[2]);
	
	long long q1, q2;
	in >> q1;
	ans >> q2;
	if (q1 == q2) {
		std::cout << "OK expected: " << q2 << " printed: "<< q1 << '\n';
	} else {
		std::cout << "Error expected:" << q2 << " printed: " << q1 << '\n';
	}
	return 0;
}
