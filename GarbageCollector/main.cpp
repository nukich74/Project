#include <utility>
#include <map>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include "object.h"



class A : public Object {
private:
	int Value;
public:
	A(const int &a): Object(),  Value(a) { }
};

int main() {
	int c = 100;
	A q(c);
	A* ptr = new A(q);
	CollectAllGarbage();
	return 0;
}
