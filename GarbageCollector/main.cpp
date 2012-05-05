#include <utility>
#include <map>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <assert.h>
#include <memory>
#include "object.hpp"

size_t g_mem_usage = 0;
void* operator new(size_t size) {
	void *p = malloc(size + sizeof(size_t));
	*(size_t*)p = size;
	g_mem_usage += size;
	return (size_t*)p + 1;
}

void operator delete(void *p) {
	size_t* smart_point = (size_t*) p;
	g_mem_usage -= *--smart_point;
	free(smart_point);
}

class BadBadObject: public GarbageCollector::Object {
private:
	int *i;
public:
	BadBadObject() {
		i = (int *)malloc(sizeof(int) * 100);
	}
	std::vector< GarbageCollector::Object* > AllPointers() {
		return std::vector< GarbageCollector::Object* >();
	}
	~BadBadObject() {
		free(i);
	}
};

class BadObject: public GarbageCollector::Object {
public:
	BadBadObject *tmp;
	BadObject() {
		tmp = new BadBadObject();
	}
	std::vector< GarbageCollector::Object* > AllPointers() {
		std::vector< GarbageCollector::Object* > l;
		l.push_back(tmp);
		return l;
	}
	~BadObject() {
	 
	}
};

class VeryBadObject: public GarbageCollector::Object {
public:
	std::vector< GarbageCollector::Object* > AllPointers() {
		std::vector< GarbageCollector::Object* > tmp;
		tmp.push_back(obj);
		return tmp;
	}
	VeryBadObject *obj;
};

class NotBadObject:public GarbageCollector::Object {
private:
	VeryBadObject *obj1;
	VeryBadObject *obj2;
public:
	std::vector< GarbageCollector::Object* > AllPointers() {
		std::vector< GarbageCollector::Object* > tmp;
		tmp.push_back(obj1);
		tmp.push_back(obj2);
		return tmp;
	}
	NotBadObject() {
		obj1 = new VeryBadObject();
		obj2 = new VeryBadObject();
		obj1->obj = obj2;
		obj2->obj = obj1;
	}
};

void test1() {
	BadObject tmp;
	GarbageCollector::CollectAllGarbage();
	assert(g_mem_usage != 0);
}

/*void test2() {
	if(1) {
	BadObject tmp;
	}
	assert(g_mem_usage != 0);
	g_collector.run();
	assert(g_mem_usage == 0);
}

void test3() {
	if(1) {
		NotBadObject obj;
		g_collector.run();
		assert(g_mem_usage != 0);
	}
	g_collector.run();
	assert(g_mem_usage == 0);
}*/

int main() {
	test1();
	GarbageCollector::FreeMemory();
	return 0;
}
