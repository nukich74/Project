#ifndef _OBJECT_INHARITANCE_ 
#define _OBJECT_INHARITANCE_ 1

namespace GarbageCollector {
	enum SpaceType { Heap, Stack };	
	std::map< void*, SpaceType > ObjectMap;
	
	class Object {
	private:	
	
	public:
		void MarkObject(const std::list< std::pair< std::string, void* >  > &lst) {
			GarbageCollector::ObjectMap.insert(std::make_pair(*this, Stack));
		}
		
		void *operator new(const size_t &size) {
			GarbageCollector::ObjectMap.insert(std::make_pair(*this, Heap));
		}
		
		void operator delete(void* ptr) {
			free(p);
		}
	
		virtual ~Object() { }	
	};
}
#endif
