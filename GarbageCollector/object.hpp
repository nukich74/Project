#ifndef _OBJECT_INHERITANCE_ 
#define _OBJECT_INHERITANCE_ 1
#include <utility>
#include <map>
#include <cstdio>
#include <exception>
#include <algorithm>
#include "graph.hpp"



namespace GarbageCollector {	
	enum SpaceType { Heap, Stack };
	class Object;	
	std::map< Object*, std::pair< SpaceType, bool /*IsMarked*/ > > ObjectMap;
	
	bool operator==(const std::pair< Object*, std::pair< SpaceType, bool > > &p1,
			    const std::pair< Object*, std::pair< SpaceType, bool > > &p2) {
		return p1.first == p2.first;
	}
	
		
	class Object {
	private:	
	public:
		Object() {
			if (ObjectMap.find(this) == ObjectMap.end()) {
				ObjectMap.insert(std::make_pair(this, std::make_pair(Stack, false)));
			}		
		}
		virtual std::vector< Object* > AllPointers() = 0;
		void* operator new(size_t size) {
			void* ptr = ::operator new(size); 
			ObjectMap.insert(std::make_pair(static_cast< Object* >(ptr), std::make_pair(Heap, false)));
			return ptr;
		}
		virtual ~Object() { 
			ObjectMap.erase(ObjectMap.find(this));	
		}	
	};

	class ArePointersConnected {
	private:
		typename std::map< Object*, std::pair< SpaceType, bool > >::const_iterator edgelist_iter_begin, edgelist_iter_end;
		typename std::map< Object*, std::pair< SpaceType, bool > >::const_iterator vertexlist_iter_begin, vertexlist_iter_end;
	public:
		ArePointersConnected(std:: pair< std::map< Object*, std::pair< SpaceType, bool > >::const_iterator , 
				     			  std::map< Object*, std::pair< SpaceType, bool > >::const_iterator > epair, 
				    std::pair < std::map< Object*, std::pair< SpaceType, bool > >::const_iterator, 
				                std::map< Object*, std::pair< SpaceType, bool > >::const_iterator >  vpair) { 
			vertexlist_iter_begin = vpair.first;
			vertexlist_iter_end = vpair.second;
			edgelist_iter_begin = epair.first;
			edgelist_iter_end = epair.second;
		}
	
		bool operator()(const std::pair< std::pair< Object*, std::pair< SpaceType, bool > >, 
							   std::pair< Object*, std::pair< SpaceType, bool > > > &p) const {
			std::map< Object*, std::pair< SpaceType, bool > >::const_iterator iter = ObjectMap.find(p.first.first);
			if (iter == ObjectMap.end()) {
				throw("There is unallocated object");
			} 
			std::vector< Object* > links = iter->first->AllPointers();
			return std::find(links.begin(), links.end(), p.second.first) != links.end();
		}
	};
	
	void CollectAllGarbage() {
		Graph< std::pair< Object*, std::pair< SpaceType, bool > >, ArePointersConnected, 
		          std::map< Object*, std::pair< SpaceType, bool > >::const_iterator, 
		          std::map< Object*, std::pair< SpaceType, bool > >::const_iterator > g(ObjectMap.begin(), ObjectMap.end(), ObjectMap.begin(), ObjectMap.end());
		
		for (auto s = ObjectMap.begin(); s != ObjectMap.end(); ++s) {
			if (s->second.first == Stack) {
				g.dfs(*s);
			}
		}

		std::set< std::pair< Object*, std::pair< SpaceType, bool > > > markedPointers = g.marking_result();

		for (auto s = markedPointers.begin(); s != markedPointers.end(); ++s) {
			std::map< Object*, std::pair< SpaceType, bool > >::iterator iter = ObjectMap.find(s->first);
			if (iter == ObjectMap.end()) {
				throw("AAAAA PANIKA GC NE RABOTAET!!!!");
			} else {
				iter->second.second = true;
//				std::cout << "Marking: " << iter->first << std::endl;
			}
		}
		
		for(auto s = ObjectMap.begin(); s != ObjectMap.end(); ++s) {
			if(!s->second.second) {
//				std::cout << "Deleting unused obj: " << s->first << std::endl;
				delete s->first;
			} else {
				s->second.second = false;
			}
		}
	}
	
	void FreeMemory() {
		for(auto s = ObjectMap.begin(); s != ObjectMap.end(); ++s) {
			if(!s->second.first == Stack) {
				delete s->first;
			}
		}
	}
}	
#endif
