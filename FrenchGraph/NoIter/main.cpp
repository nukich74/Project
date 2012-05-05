#include <iostream>
#include <cstddef>
#include <vector>
#include <cstdio>
#include <map>
#include <list> 
#include <set>
#include <functional>
#include <algorithm>
#include <fstream>
#include <memory>
#include <iterator>

template< class T >
class SmartVertex {
public:
	
	T VertexID;
     size_t ConnectionsCount;

	virtual bool AddVertex(const std::pair< T, size_t >&) = 0;
	virtual bool DeleteVertex(const T&) = 0;
	virtual ~SmartVertex() {}
	virtual const std::pair< std::pair< T, size_t >*, std::pair< T, size_t >* > AllConnectedVertex() = 0;
	size_t ConnectionsNumber() const {
		return ConnectionsCount;
	}
	size_t ID() const {
		return VertexID;
	}
	
}; 

template< class T, size_t TSize > 
class SmartVertexConstConnections: public SmartVertex< T > {
private:
	
	typedef std::pair< T, size_t > Edge;
	std::pair< T, size_t > VertexSet[TSize + 1];
	
public:
	SmartVertexConstConnections(const SmartVertexConstConnections &v) {
		this->VertexID = v.ID();
		this->ConnectionsCount = v.ConnectionsNumber();
		for (size_t i = 0; i < v.ConnectionsNumber(); i++) {
			VertexSet[i] = v.VertexSet[i];
		}
	}
	
	SmartVertexConstConnections& operator= (SmartVertexConstConnections &&v) {
		this->VertexID.swap(v.VertexID);
		this->ConnectionsCount.swap(v.ConnectionsCount);
		for (size_t i = 0; i < v.ConnectionsNumber(); i++) {
			VertexSet[i].swap(v.VertexSet[i]);
		}
		return *this;
	}
	
	SmartVertexConstConnections& operator= (const SmartVertexConstConnections &v) {
		return *this = SmartVertexConstConnections(v);
	}
	
	explicit SmartVertexConstConnections(const T &id_) { 
		this->VertexID = id_;
		this->ConnectionsCount = 0;	
	}
	
	bool AddVertex(const std::pair< T, size_t > &v) {
		if (this->ConnectionsCount == TSize) {
			return false;
		}
		VertexSet[this->ConnectionsCount].first = v.first;
		VertexSet[this->ConnectionsCount].second = v.second;
		this->ConnectionsCount++;
		return true;
	}
	
	bool DeleteVertex(const T &vID) {
		for (size_t i = 0; i < this->ConnectionsCount; ++i) {
			if (VertexSet[i].first == vID) {
				std::swap(VertexSet[i], VertexSet[this->ConnectionsCount - 1]);
				this->ConnectionsCount--;
				return true;
			} 
		}
		return false;
	}
	
	const std::pair< Edge*, Edge* > AllConnectedVertex() const {
		return std::make_pair(VertexSet, VertexSet + this->ConnectionsCount);
	}	
	
	bool operator==(const SmartVertexConstConnections< T, TSize > &v) const {
		return v.VertexID == this->VertexID;
	}
	bool operator<(const SmartVertexConstConnections< T, TSize > &v) const {
		return v.VertexID <= this->VertexID;
	}
	bool operator!=(const SmartVertexConstConnections< T, TSize > &v) const {
		return !(*this == v);
	} 
};

template< class T >
class SmartVariableVertex : public SmartVertex< T > {
private:
	typedef std::pair< T, size_t > Edge;
	std::vector< std::pair< T, size_t > > VertexSet;
public:
	SmartVariableVertex(const SmartVariableVertex< T > &v) {
		std::copy(v.VertexSet.begin(), v.VertexSet.end(), std::back_inserter(VertexSet));
		this->ConnectionsCount = VertexSet.size();
		this->VertexID = v.ID();
	}
	
	void swap(SmartVariableVertex &v) {
		this->VertexID.swap(v.VertexID);
		this->ConnectionsCount.swap(v.ConnectionsCount);
		for (size_t i = 0; i < v.ConnectionsNumber(); i++) {
			VertexSet[i].swap(v.VertexSet[i]);
		}
		return *this;
	}
	
	SmartVariableVertex& operator=(const SmartVariableVertex< T > &v) {
		return *this;
	}
	
	SmartVariableVertex(const size_t&id_) {
		this->VertexID = id_;
	}
	
	bool AddVertex(const std::pair< T, size_t > &v) {
		VertexSet.push_back(v);
		return true;
	}
	
	bool DeleteVertex(const size_t &vID) {
		typename std::vector< std::pair< T, size_t > >::iterator beg = VertexSet.begin();
		while (beg != VertexSet.end()) {
			if (beg->first == vID) {
				VertexSet.erase(beg);
				this->ConnectionsCount = VertexSet.size();
				return true;
			}
			beg++;
		}
		return false;
	}

	const std::pair< std::pair< T, size_t >*, std::pair< T, size_t >* > AllConnectedVertex() const {
		return std::make_pair(&VertexSet[0], &VertexSet[0] + VertexSet.size());
	}				
};

template< class IdType, class Vertex >
std::map< IdType, size_t > DoDijkstraAlgorithm(const IdType &id, const 	std::vector< Vertex* > SmartVertexList) {
	std::set< IdType > UsedList;
	std::map< IdType, size_t > DistanceList;
	for_each(SmartVertexList.begin(), SmartVertexList.end(), [&DistanceList](const Vertex* v) {
		DistanceList.insert(std::make_pair(v->ID(), std::numeric_limits< size_t >::max()));
	});
	
	DistanceList[id] = std::numeric_limits< size_t >::min();
	
	for (size_t p = 0; p < SmartVertexList.size(); ++p) {		
		bool IsFoundVertexToGo = false;
		IdType vertexToGo = 0;
		int index = -1;
		for (size_t i = 0; i < SmartVertexList.size(); ++i) {
			IdType tname = SmartVertexList[i]->ID();
			if (UsedList.find(tname) == UsedList.end() && (!IsFoundVertexToGo || DistanceList[tname] < DistanceList[vertexToGo])) {
				vertexToGo = tname;
				index = i;
				IsFoundVertexToGo = true;
			}
		} 
					
		UsedList.insert(vertexToGo);
		
		if (DistanceList[vertexToGo] == std::numeric_limits< size_t >::max()) {
			return DistanceList;
		} else {
			std::pair< std::pair< IdType, size_t >*, std::pair< IdType, size_t >* > ConVertexlist = SmartVertexList[index]->AllConnectedVertex();
			
			for (std::pair< size_t, size_t >* ptr = ConVertexlist.first; ptr != ConVertexlist.second; ptr++) {
				if (DistanceList[vertexToGo] + ptr->second < DistanceList[ptr->first]) {
					DistanceList[ptr->first] = DistanceList[vertexToGo] + ptr->second;
				}
			}	
		}
	}		
	return DistanceList;
}	

std::vector< SmartVertex< size_t >* > BuildGraph(const std::vector< std::vector< std::pair< size_t, size_t > > > &v) {
	std::vector< SmartVertex< size_t >* > result;
	for (size_t i = 1; i < v.size(); i++) {
		SmartVertex< size_t >* ptr;
		if (v.size() > 2) {
			SmartVariableVertex< size_t > vertex(i);
			for (const std::pair< size_t, size_t > &x: v[i]) {
				vertex.AddVertex(x);
			}
			ptr = new SmartVariableVertex< size_t >(vertex);
		} else {
			SmartVertexConstConnections< size_t, 2 > vertex(i);
			for (const std::pair< size_t, size_t > &x: v[i]) {
				vertex.AddVertex(x);
			}
			ptr = new SmartVertexConstConnections< size_t, 2 >(vertex);
		}
		result.push_back(ptr);
	}

	return result;
}

int main(const int argc, const char** argv) {
	if (argc == 1) {
		throw("No enought params\n");
	}
	std::ifstream in(argv[1]);
	std::ofstream out(argv[2]);
	int w;
	size_t n, s, f; 
	in >> n >> s >> f;
	std::vector< std::vector< std::pair< size_t, size_t > > > g(n + 1);	
	for (size_t i = 1; i <= n; i++) {
		for (size_t j = 1; j <= n; j++) {
			in >> w;
			if (w >= 0) {
				g[i].push_back(std::make_pair(j, w));
			}
		}
	}
	
	std::vector< SmartVertex< size_t >* > SmartGraph = BuildGraph(g);	
	std::map< size_t , size_t > res = DoDijkstraAlgorithm(s, SmartGraph);
	out <<  (res[f] == std::numeric_limits< size_t >::max() ? (long long)-1 : (long long)res[f]) << '\n';
	return 0;
}
