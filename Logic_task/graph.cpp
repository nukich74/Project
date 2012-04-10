template< class Vertex, class vertexSet_Iter, class edgeList_Iter, class TopSortOut_Iter, class CondensationOut_Iter > 
class Graph {
private:
	typedef std::vector< Vertex > vVector;
	typedef std::set< Vertex > vSet;
	typedef std::multiset< std::pair< Vertex, Vertex > > eList;
	typedef std::vector< vSet > cVector;
	
	bool pred_val;
	std::pair< typename vSet::const_iterator, typename vSet::const_iterator > vset_base;
	std::pair< typename eList::const_iterator, typename eList::const_iterator > elist_base;
	vSet used;
	std::stack< Vertex > st;
	vVector in_sort, out_sort;
	cVector cond;
	
	Graph(const Graph&);
	Graph& operator=(const Graph&);
	
	struct Transposed {	
		std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex &b) 
			{ return std::make_pair(b, a); } 
	};
	struct Original { std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex& b) { return std::make_pair(a, b); } };
public:

	Graph() {}
	template< class TransformEdge >
	void dfs(const Vertex &v) {
		TransformEdge trans;
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
								
				std::for_each(vset_base.first, vset_base.second, [=, &used, &elist_base, &st, this, &trans] (const Vertex &q){
					if ((used.find(q) ==  used.end()) && std::binary_search(elist_base.first, elist_base.second, trans(tvert, q))) {
						st.push(q); 
					}
				});
								
			}
		}
	}

	vVector in_sort_result() {
		return in_sort;
	}

	vVector out_sort_result() {
		return out_sort;
	}
	
	template< class WorkSpaceGraph >
	vVector topolog_sort(const WorkSpaceGraph &graph) {
		vset_base.first = graph.vertexlist_iter_begin;
		vset_base.second = graph.vertexlist_iter_end;
		elist_base.first = graph.edgelist_iter_begin;
		elist_base.second = graph.edgelist_iter_end;
		
		used.clear();
		in_sort.clear();
		out_sort.clear();
		int time = 0;

		std::for_each(graph.vertexlist_iter_begin, graph.vertexlist_iter_end, [&used, this](const Vertex &v) {
			if (used.find(v) == used.end()) {
				this->dfs< Original >(v);
			}
		});
		return out_sort;
	}

	template< class WorkSpaceGraph >
	cVector build_condensation(const WorkSpaceGraph &graph) {
		vVector top_sort = topolog_sort< WorkSpaceGraph >(graph);
		
		in_sort.clear();
		out_sort.clear();
		used.clear();
		std::for_each(top_sort.rbegin(), top_sort.rend(), [&cond, &used, &in_sort, this] (const Vertex &v) {
			if (used.find(v) == used.end()) {
				this->dfs< Transposed >(v);
				cond.push_back(vSet(in_sort.begin(), in_sort.end()));
				in_sort.clear();
			}
		});

		return cond;
	}

};
