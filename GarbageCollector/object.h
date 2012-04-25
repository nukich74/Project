#ifndef _OBJECT_INHARITANCE_ 
#define _OBJECT_INHARITANCE_ 1

namespace GarbageCollect {
	enum AchieveType { };	
	std::map< std::string, void* > ObjectMap;
	
	class Object {
	private:	
	
	public:
		
		void *operator new(const size_t &size) {
			
		}
		
		void operator delete(void* ptr) {
			free(p);
		}
	
		virtual ~Object() { }	
	};
}
#endif
