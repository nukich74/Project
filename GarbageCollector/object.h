#ifndef _OBJECT_INHARITANCE_ 
#define _OBJECT_INHARITANCE_ 1
#include <utility>
#include <map>
#include <cstdio>
#include <algorithm>

namespace GarbageCollector {
		enum SpaceType { Heap, Stack };	
	std::map< void*, SpaceType > ObjectMap;
	
	class Object {
	private:	

	public:
		Object() {
			ObjectMap.insert(std::make_pair(this, Stack));
		}
		
		void *operator new(size_t size) {
			void* ptr = malloc(size); 
			ObjectMap.insert(std::make_pair(ptr, Heap));
			return ptr;
		}
		
		void operator delete(void* ptr) {
			free(ptr);
		}
	
		virtual ~Object() { }	
	};
	
	void CollectAllGarbage() {
		for (auto s = ObjectMap.begin(); s != ObjectMap.end(); ++s) {
			if (s->second != Stack) {
		/*DEBUT OUTPUT*/		std::cout << "Deleted " << s->second;
				delete (Object*)s->first;
			} 
		}
	}
}
#endif
