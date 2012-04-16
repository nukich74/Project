#include <iostream>
#include <cstddef>
#include <vector>
#include <cstdio>
#include <map>
#include <list>   ///напиши копирование и присваивание!!!!!
#include <set>
#include <functional>
#include <algorithm>
#include <fstream>
#include <memory>
#include <iterator>

template< class T > class SmartVertex {
public:
	
	unsigned int VertexID;
     unsigned int ConnectionsCount;

	virtual bool AddVertex(const std::pair< T, unsigned int >&) {}
	virtual bool DeleteVertex(const T&) {}
	virtual std::pair< T, unsigned int > NextConnectedVertex() const {}
	virtual ~SmartVertex() {}
	unsigned int ConnectionsNumber() const {
		return ConnectionsCount;
	}
	unsigned int ID() const {
		return VertexID;
	}
	
}; 

template< class T > 
class SmartVertexTwoConnections: public SmartVertex< T > {
private:
	bool AlreadyGaveFirstVertex;
	bool AlreadyGaveSecondVertex;
	std::pair< T, unsigned int > FirstConnectedVertex;
	std::pair< T, unsigned int > SecondConnectedVertex;
public:
	std::pair< T, unsigned int > FirstVertex() const {
		return FirstConnectedVertex;
	}	
	
	std::pair< T, unsigned int > SecondVertex() const {
		return SecondConnectedVertex;
	}
	
/*	
	SmartVertexTwoConnections(const SmartVertexTwoConnections &v) : FirstConnectedVertex(v.FirstVertex()), SecondConnectedVertex(v.SecondVertex()) {
		AlreadyGaveFirstVertex = AlreadyGaveSecondVertex = 0;
	}

	SmartVertexTwoConnections& operator=(const SmartVertexTwoConnections< T > &v) {
		FirstConnectedVertex = v.FirstVertex();
		SecondConnectedVertex = v.SecondVertex();
		AlreadyGaveFirstVertex = v.AlreadyGaveFirstVertex;
		AlreadyGaveSecondVertex = v.AlreadyGaveSecondVertex;
		return *this;	
	}
*/
	SmartVertexTwoConnections(const int &id_) { 
		this->VertexID = id_;
		this->ConnectionsCount = 0;
		AlreadyGaveFirstVertex = false;
		AlreadyGaveSecondVertex = false;
	}
	bool AddVertex(const std::pair< T, unsigned int > &v) {
		if (this->ConnectionsCount == 2) {
			return false;
		} else {
			if (this->ConnectionsCount == 0) {
				FirstConnectedVertex = std::make_pair(v.first, v.second);
				this->ConnectionsCount++;
			} else {
				SecondConnectedVertex = std::make_pair(v.first, v.second);
				this->ConnectionsCount++;
			}
		}
	}
	bool DeleteVertex(const unsigned int &vID) {
		if (vID == FirstConnectedVertex.first) {
			FirstConnectedVertex = SecondConnectedVertex;
			this->ConnectionsCount--;
			return true;
		} else
		if (vID == SecondConnectedVertex.first) {
			this->ConnectionsCount--;
			return true;
		}
		return false;
	}
	std::pair< T, unsigned int > NextConnectedVertex() {
		if (AlreadyGaveFirstVertex && !AlreadyGaveSecondVertex) {
			return SecondConnectedVertex;
			AlreadyGaveFirstVertex = true;
			AlreadyGaveSecondVertex = true; 
		} else 
		if (!AlreadyGaveFirstVertex && !AlreadyGaveSecondVertex) {
			return FirstConnectedVertex;
			AlreadyGaveFirstVertex = true;
			AlreadyGaveSecondVertex = false;
		} else {
			AlreadyGaveFirstVertex = false;
			AlreadyGaveSecondVertex = false;
			return 0;
		}
		return 0;
	}	
	bool operator==(const SmartVertexTwoConnections< T > &v) {
		return v.VertexID == this->VertexID;
	}
	bool operator<(const SmartVertexTwoConnections< T > &v) {
		return v.VertexID <= this->VertexID;
	}
	bool operator!=(const SmartVertexTwoConnections< T > &v) {
		return !(*this == v);
	} 
};

template< class T >
class SmartSetVertex : public SmartVertex< T > {
private:
	std::vector< std::pair< T, unsigned int > > VertexSet;
	typename std::vector< std::pair< T, unsigned int > >::const_iterator CurrentVertexIter;
public:
	SmartSetVertex(const SmartSetVertex< T > &v) {
		this->VertexSet.reserve(v.VertexSet.size());
		std::copy(v.VertexSet.begin(), v.VertexSet.end(), this->VertexSet.begin());
		
		this->ConnectionsCount = VertexSet.size();
		unsigned int dist = std::distance(v.VertexSet.begin(), v.CurrentVertexIter);
		this->CurrentVertexIter = VertexSet.begin();
		while (dist != 0) {
			this->ConnectionsCount++;
			dist--;
		}
	}
	
	SmartSetVertex& operator=(const SmartSetVertex< T > &v) {
		VertexSet.clear();
		this->SmartVertex(v);
		return *this;
	}
	
	SmartSetVertex(const int &id_) {
		this->VertexID = id_;
		CurrentVertexIter = VertexSet.begin();
	}
	bool AddVertex(const std::pair< T, unsigned int > &v) {
		VertexSet.push_back(v);
		this->ConnectionsCount = VertexSet.size();
		if (VertexSet.size() > 0) {
			this->CurrentVertexIter = VertexSet.begin();
		}
		return true;
	}
	bool DeleteVertex(const unsigned int &vID) {
		typename std::vector< std::pair< T, unsigned int > >::iterator beg = VertexSet.begin();
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
	std::pair< T, unsigned int > NextConnectedVertex() {
		if (CurrentVertexIter == VertexSet.end()) {
			CurrentVertexIter = VertexSet.begin();
			return 0;
		} else {
			std::pair< T, unsigned int > p(std::make_pair(*CurrentVertexIter.first, *CurrentVertexIter.second));
			CurrentVertexIter++;
			return p;
		}
	}
	~SmartSetVertex() {
		VertexSet.clear();
	}
};

std::vector< std::unique_ptr< SmartVertex< unsigned int > > > BuildGraph(const std::vector< std::vector< std::pair< unsigned int, unsigned int > > > &v) {
	std::vector< std::unique_ptr< SmartVertex< unsigned int > > > result;
	for (int i = 0; i < v.size(); i++) {
		SmartVertex< unsigned int >* ptr;
		if (v.size() > 2) {
			SmartSetVertex< unsigned int > vertex(i);
			for (const std::pair< unsigned int, unsigned int > &x: v[i]) {
				vertex.AddVertex(x);
			}
			ptr = new SmartSetVertex< unsigned int >(vertex);
		} else {
			SmartVertexTwoConnections< unsigned int > vertex(i);
			for (const std::pair< unsigned int, unsigned int > &x: v[i]) {
				vertex.AddVertex(x);
			}
			ptr = new SmartVertexTwoConnections< unsigned int >(vertex);
		}
		result.push_back(std::unique_ptr< SmartVertex< unsigned int > >(ptr));
	}
	
	return result;
}

template< class Vertex, class DistanceMeasure > 
class GraphHandler {
private:
	DistanceMeasure Measures;
	std::set< Vertex > used_list;
	std::map< Vertex, DistanceMeasure > DistanceList;
	std::vector< std::unique_ptr< Vertex > > SmartVertexList; 
public:	

	GraphHandler(typename std::vector< std::unique_ptr< Vertex > >::const_iterator beg, 
	             typename std::vector< std::unique_ptr< Vertex > >::const_iterator end) { 
		SmartVertexList.reserve(std::distance(beg, end));
		std::copy(beg, end, SmartVertexList.begin());
	}

	std::map< Vertex, DistanceMeasure > DoDijkstraAlgorithm(const Vertex &v) {
		used_list.clear();
		for_each(SmartVertexList.begin(), SmartVertexList.end(), [&DistanceList, &Measures](const Vertex& v) {
			DistanceList[v] = std::numeric_limits< DistanceMeasure >::max();
		});
		
		DistanceList[v] = std::numeric_limits< DistanceMeasure >::min();
		
		for_each(SmartVertexList.begin(), SmartVertexList.end(), [this, &Measures, &used_list, &DistanceList](const Vertex& v) {
			bool IsFoundVertexToGo = false;
			Vertex tempVertex = *(this->SmartVertexList.begin());
			for_each(this->SmartVertexList.begin(), this->SmartVertexList.end(), [&used_list, &DistanceList, &IsFoundVertexToGo, &tempVertex](const Vertex &v) {
				if (used_list.find(v) == used_list.end() && (!IsFoundVertexToGo || DistanceList[v] < DistanceList[tempVertex])) {
					v = tempVertex;
					IsFoundVertexToGo = true;
				}
			});
			
			if (DistanceList[tempVertex] == std::numeric_limits< DistanceMeasure >::max()) {
				return NULL;
			} else {
				std::pair< Vertex, DistanceMeasure > gotoVertex;					
				while (gotoVertex = tempVertex.NextConnectedVertex() != 0) {
					DistanceMeasure EdgeWeigth = gotoVertex.second;
					if (DistanceList[v] + EdgeWeigth < DistanceList[gotoVertex.first]) {
						DistanceList[gotoVertex.first] = DistanceList[v] + EdgeWeigth;
//						PreviousWaysVertexList[gotoVertex.first] = v;
					}	
				}		
			}
		});
		
	}

	
};

int main(const int argc, const char** argv) {
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");
	int n, s, f, w; 
	in >> n >> s >> f;
	std::vector< std::vector< std::pair< unsigned int, unsigned int > > > g(n + 1);	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			in >> w;
			if (w != 0) {
				g[i].push_back(std::make_pair(j, w));
			}
		}
	}
	
	std::vector< std::unique_ptr< SmartVertex< unsigned int > > > SmartGraph = BuidGraph(g);	
	GraphHandler< SmartVertex< unsigned int >, unsigned int > handle(SmartGraph.begin(), SmartGraph.end());		
	std::map< SmartVertex< unsigned int >, unsigned int > res = handle.DoDijkstraAlgorithm(*SmartGraph[0]);
	return 0;
}
