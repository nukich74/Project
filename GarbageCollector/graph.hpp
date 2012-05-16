#ifndef _BASE_GRAPH_
#define _BASE_GRAPH_ 1

#include <iostream>
#include <fstream>
#include <cstdio>
#include <utility>
#include <deque>
#include <stack>
#include <set>
#include <algorithm>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <bitset>
#include <numeric>
#include <functional>
#include <iterator> 



template< class Vertex, class IsEdgePred,
				typename vertexSet_Iter = typename std::set< Vertex >::const_iterator, 
                        typename edgeList_Iter = typename std::multiset< std::pair < Vertex, Vertex > >::const_iterator,
                        typename TopSortOut = typename std::vector< Vertex >, 
                        typename CondensationOut = typename std::vector< std::set< Vertex > > >
class Graph {
private:
	
	bool pred_val;
	std::pair< vertexSet_Iter, vertexSet_Iter > vset_base;
	std::pair< edgeList_Iter, edgeList_Iter > edgelist_base;
	std::set< Vertex > used;
	std::stack< Vertex > st;
	TopSortOut in_sort, out_sort;
	CondensationOut cond;
	Graph(const Graph&);
	Graph& operator=(const Graph&);
	
public:
	struct Transposed {	std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex &b) { return std::make_pair(b, a); } };
	struct Original { std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex& b) { return std::make_pair(a, b); } };

	
	explicit Graph(const vertexSet_Iter &beg, const vertexSet_Iter &end, 
		const edgeList_Iter &ebeg, const edgeList_Iter &eend) {
		vset_base.first = beg;
		vset_base.second = end;
		edgelist_base.first = ebeg;
		edgelist_base.second = eend;	
	}

	template< class TransformEdge = Original >
	void dfs(const Vertex &v) {
		TransformEdge trans;
		IsEdgePred Pred(edgelist_base, vset_base);
		st.push(v);
		pred_val = true;
		while (!st.empty()) {
			Vertex tvert = st.top();
			
			if (used.find(tvert) != used.end()) {
				out_sort.push_back(tvert);
				st.pop();
			} else {
				in_sort.push_back(tvert);
				used.insert(tvert);  
								
				for (auto q = vset_base.first; q != vset_base.second; ++q) {
					if ((used.find(*q) ==  used.end()) && (Pred(trans(tvert, *q)))) {
						st.push(*q); 
					}
				}

			}
		}
	}

	std::set< Vertex > marking_result() const {
		return used;
	}

	std::vector< Vertex > in_sort_result() const {
		return in_sort;
	}

	std::vector< Vertex > out_sort_result() const {
		return out_sort;
	}
	
	TopSortOut topological_sort() {
		
		used.clear();
		in_sort.clear();
		out_sort.clear();
		std::cout << "used size" << used.size() << '\n';
		for (auto s = vset_base.first; s != vset_base.second; ++s) {
			if (used.find(*s) == used.end()) {
				this->dfs< Original >(*s);
			}
		}
		std::cout << "used size" << used.size() << '\n';
		return out_sort;
	}

	CondensationOut build_condensation() {
		std::vector< Vertex > top_sort = topological_sort();
		
		in_sort.clear();
		out_sort.clear();
		used.clear();
		cond.clear();
		for (auto v = top_sort.rbegin(); v != top_sort.rend(); ++v) {
			if (used.find(v) == used.end()) {
				this->dfs< Transposed >(v);
				cond.push_back(vSet(in_sort.begin(), in_sort.end()));
				in_sort.clear();
			}
		}
	
		return cond;
	}

};

#endif
