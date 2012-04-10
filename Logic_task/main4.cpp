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

struct BoolVertex;

typedef std::multiset< std::pair < struct BoolVertex, struct BoolVertex > > BoolVertexEdge_list;
typedef std::pair< BoolVertex, BoolVertex > Edge;
typedef std::set< BoolVertex > vSet;
typedef std::vector< BoolVertex > vVector;
typedef std::vector< vSet > cVector;
typedef std::map< vSet, bool > SlnTable;
typedef std::pair< vSet, vSet > vPair;
typedef std::set< vSet > cSet;

class func {
private:
	char variable1, variable2;
	std::string truth_table;
public:
	func(const char &s_, const char &t_, const std::string &str_) : variable1(s_), variable2(t_), truth_table(str_) {};
	char var1() const{
		return variable1;	
	}
	char var2() const {
		return variable2;
	}
	std::string table() {
		return truth_table;
	}
	char operator[](const int &index) const {
		return truth_table[index];
	}
};

class BoolVertex {
private:
	char name_;
	bool negation_;
public:
	explicit BoolVertex() : name_('#'), negation_(0) {};
	BoolVertex(const char nam, const bool &neg) : name_(nam), negation_(neg) {};
	BoolVertex(const BoolVertex &t) {
		name_ = t.name();
		negation_ = t.negation();
	}
	void assign(const char nam, const bool &neg) {
		name_ = nam;
		negation_ = neg;
	}
	BoolVertex inverse() const {
		return BoolVertex(name_, !negation_);
	}
	char name() const {
		return name_;
	}
	bool negation() const {
		return negation_;
	}
};


bool operator<(const BoolVertex &a, const BoolVertex &b) {
	return (a.name() < b.name()) || ((a.name() == b.name()) && (a.negation() < b.negation()));
}

bool operator==(const BoolVertex &a, const BoolVertex &b){
	return (a.name() == b.name()) && (a.negation() == b.negation());
}

bool operator>(const BoolVertex &a, const BoolVertex &b) {
	return (!(a < b) && !(a == b));
}

bool operator<=(const BoolVertex &a, const BoolVertex &b) {
	return ((a < b) || (a == b));
}

bool operator>=(const BoolVertex &a, const BoolVertex &b) {
	return ((a > b) || (a == b));
}



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
	
	struct Transposed {	std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex &b) { return std::make_pair(b, a); } };
	struct Original { std::pair< Vertex, Vertex > operator()(const Vertex& a, const Vertex& b) { return std::make_pair(a, b); } };
public:

	explicit Graph(const vertexSet_Iter &beg, const vertexSet_Iter &end, 
		 const edgeList_Iter &ebeg, const edgeList_Iter &eend) {
		vset_base.first = beg;
		vset_base.second = end;
		edgelist_base.first = ebeg;
		edgelist_base.second = eend;	
	}

	template< class TransformEdge >
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
								
				std::for_each(vset_base.first, vset_base.second, [&Pred, &used, &st, this, &trans, &tvert] (const Vertex &q){
					if ((used.find(q) ==  used.end()) && (Pred(trans(tvert, q)))) {
						st.push(q); 
					}
				}); 

			}
		}
	}

	vVector in_sort_result() const {
		return in_sort;
	}

	vVector out_sort_result() const {
		return out_sort;
	}
	
	TopSortOut topological_sort() {
		
		used.clear();
		in_sort.clear();
		out_sort.clear();

		std::for_each(vset_base.first, vset_base.second, [&used, this](const Vertex &v) {
			if (used.find(v) == used.end()) {
				this->dfs< Original >(v);
			}
		});
		return out_sort;
	}

	CondensationOut build_condensation() {
		std::vector< Vertex > top_sort = topological_sort();
		
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

BoolVertexEdge_list get_edges_BoolVertex(const func &f) {
	BoolVertexEdge_list table;
	for (std::bitset< 3 > mask(0); mask.to_ulong() != 4; mask = std::bitset< 3 >(mask.to_ullong() + 1)) {
		int p = mask.to_ulong();
		BoolVertex v1, v2;
		if (f[p] == '0') {
			if (mask[1] == 1) {
				v1.assign(f.var1(), 1);
			} else v1.assign(f.var1(), 0);
			
			if (mask[0] == 1) {
				v2.assign(f.var2(), 1);
			} else v2.assign(f.var2(), 0);
		
			table.insert(std::make_pair(v1.inverse(), v2));
			table.insert(std::make_pair(v2.inverse(), v1));		
		}
	}
	
	return table;
}

bool StrongComponentsAreOpposite(const std::pair< vSet, vSet > &p) {
	bool f = true;
	std::for_each(p.first.begin(), p.first.end(), [&](const BoolVertex &t) {
		if (p.second.find(BoolVertex(t.name(), !t.negation())) == p.second.end()) f = false;
	});
	
	return f;
}

class AreBoolVerticesConnected {
private:
	typename BoolVertexEdge_list::const_iterator edgelist_iter_begin, edgelist_iter_end;
	typename std::set< BoolVertex >::const_iterator vertexlist_iter_begin, vertexlist_iter_end;
public:
	AreBoolVerticesConnected(std:: pair< BoolVertexEdge_list::const_iterator , 
				    BoolVertexEdge_list::const_iterator > epair, 
				    std::pair < vSet::const_iterator, 
				    vSet::const_iterator >  vpair) { 
		vertexlist_iter_begin = vpair.first;
		vertexlist_iter_end = vpair.second;
		edgelist_iter_begin = epair.first;
		edgelist_iter_end = epair.second;
	}
	bool operator()(const std::pair< BoolVertex, BoolVertex > &p) const {
		return std::binary_search(edgelist_iter_begin, edgelist_iter_end, p);	
	}
};

class AreStrongComponentsVertexConnected { 
private:
	typename std::vector< std::set< BoolVertex > >::const_iterator vertexlist_iter_begin, vertexlist_iter_end;
	typename BoolVertexEdge_list::const_iterator edgelist_iter_begin, edgelist_iter_end;

public:	
	AreStrongComponentsVertexConnected(const std::pair< BoolVertexEdge_list::const_iterator, BoolVertexEdge_list::const_iterator > &epair, 
       const std::pair< std::vector< std::set< BoolVertex > >::const_iterator, std::vector< std::set< BoolVertex > >::const_iterator >  &vpair) {
		vertexlist_iter_begin = vpair.first;
		vertexlist_iter_end = vpair.second;
		edgelist_iter_begin = epair.first;
		edgelist_iter_end = epair.second;
	}
	
	bool operator()(const std::pair< std::set< BoolVertex >, std::set< BoolVertex > > &p) const {
		bool f = false;
		std::for_each(p.first.begin(), p.first.end(), [&f, &p, this](const BoolVertex &v1) {
			std::for_each(p.second.begin(), p.second.end(), [this, &v1, &f](const BoolVertex &v2) {
				if (std::binary_search(edgelist_iter_begin, edgelist_iter_end, std::make_pair(v1, v2)))
					f = true;
			});
		});
		return f; 	
	}
};

bool SolutionExists(std::vector< std::set< BoolVertex > >::const_iterator beg, std::vector< std::set< BoolVertex > >::const_iterator end) {
	return !(std::find_if(beg, end, [&](const vSet &s) -> bool { 
			return std::find_if(s.begin(), s.end(), [&s](const BoolVertex &v) -> bool { 
				return s.find(v.inverse()) != s.end(); }) != s.end();
     }) != end);
}

SlnTable MarkingBoolVertex(const BoolVertexEdge_list &main_table, const std::vector< std::set< BoolVertex > > &boolvertexcond) {

	Graph< std::set< BoolVertex >, AreStrongComponentsVertexConnected,  std::vector< std::set< BoolVertex > >::const_iterator, 
	       BoolVertexEdge_list::const_iterator > g2(boolvertexcond.begin(), boolvertexcond.end(), main_table.begin(), main_table.end());
	cVector temp = g2.topological_sort();
	std::deque< vSet > top_sort(temp.begin(), temp.end());

	SlnTable solution;
	cSet is_marked;

	while (!top_sort.empty()) {
	   if (is_marked.find(*top_sort.rbegin()) == is_marked.end()) {
		   solution[*top_sort.rbegin()] = 0;
		   is_marked.insert(*top_sort.rbegin());
	   }
	   if (is_marked.find(*(top_sort.rend() - 1)) == is_marked.end()) {
		   solution[*(top_sort.rend() - 1)] = 1;
		   is_marked.insert(*(top_sort.rend() - 1));
	   }
	   std::for_each(top_sort.rbegin(), top_sort.rend(), [&] (const vSet &v) {
		   if (StrongComponentsAreOpposite(std::make_pair(v, *top_sort.rbegin()))) {
				solution[v] = 1;
				is_marked.insert(v);
		   }
		   if (StrongComponentsAreOpposite(std::make_pair(v, *(top_sort.rend() - 1)))) {
				solution[v] = 0;
				is_marked.insert(v);
		   }
	   });
	   top_sort.pop_back();
	   top_sort.pop_front();
	}

	return solution;
}

void print_solution(std::ofstream& out, SlnTable::const_iterator beg, SlnTable::const_iterator end) {
	out << "Found Solution:\n";
	std::for_each(beg, end, [&](const std::pair< vSet, bool > &ts) {
		std::for_each(ts.first.begin(), ts.first.end(), [&](const BoolVertex &v) {
			if (!v.negation()) {
				out << v.name() << ' ' << (int)ts.second << std::endl;
			}
		}); 
	});
}

int main(const int argc, const char** argv) {
	if (argc == 1) {
		throw("No param\n");
	}
	
	int numberfunc;
	char var2, var1;
	std::string truth_table;

	vSet vertexList;
	BoolVertexEdge_list main_table;
	SlnTable solution;

	std::ifstream in(argv[1]);
	std::ofstream out(argv[2]);	
	 
	in >> numberfunc;
	
	for (int i = 0; i < numberfunc; i++) {
		in >> var1 >> var2 >> truth_table;
		func tempfunction(var1, var2, truth_table);

		BoolVertexEdge_list temp = get_edges_BoolVertex(tempfunction);

		for (const Edge &v: temp) {
			if (main_table.find(v) == main_table.end()) {
				main_table.insert(v);
			}
			vertexList.insert(v.first);
			vertexList.insert(v.second);
		}
	}

	Graph< BoolVertex, AreBoolVerticesConnected > g(vertexList.begin(), vertexList.end(), main_table.begin(), main_table.end());
	std::vector< std::set< BoolVertex > > cond = g.build_condensation();
		
	if (!SolutionExists(cond.begin(), cond.end())) {
		std::cout << "No solution\n";
		return 0;
	}

	solution = MarkingBoolVertex(main_table, cond);		   
	print_solution(out, solution.begin(), solution.end());

	return 0;
}
