#include <iostream>
#include <vector>
#include <cstdio>
#include <map>
#include <list>
#include <set>
#include <functional>
#include <algorithm>




template< class Name, class DistanceMeasure >
class SmartVertex {
private:
	Name VertexID;
	int ConnectionsCount;
	bool CanGiveNextConnectedVertex;
public:

	SmartVertex(const std:map< std::pair< Name, Name >, DistanceMeasure > &edgeList) {
		
	}

	virtual DistanceMeasure NextConnectedVertexWeight();
	virtual SmartVertex NextConnectedVertex();
	int CountConnections() {
		return ConnectionsCount;
	}
};

template< class DistanceMeasure >
class SmartVertexTwoConnections : SmartVertex {
private:
	
	SmartVertex FirstConnectedVertex, SecondConnectedVertex;
};

template< class DistanceMeasure > 
class SmartVertexManyConnections : SmartVertex {
private:
	std::vector< SmartVertex > ConnectedVertexList;
};

template< class Vertex, class DistanceMeasure > 
class GraphHandler {
private:
	typename DistanceMeasure Measures;
	std::set< Vertex > used_list;
	std::map< Vertex, DistanceMeasure > DistanceList;
	std::vector< Vertex > SmartVertexList; 
public:	

	GraphHandler(std::vector< Vertex >::const_iterator beg, std::vector< Vertex >::const_iterator end) : SmartVertexList(beg, end) {}
	

	std::map< Vertex, DistanceMeasure > DoDijkstraAlgorithm(const Vertex &v) {
		used_list.clear();
		for_each(SmartVertexList.begin(), SmartVertexList.end(), [&DistanceList, &Measures](const Vertex& v) {
			DistanceList[v] = Measures.INF;
		});
		
		DistanceList[v] = Measures.ZERO;
		
		for_each(SmartVertexList.begin(), SmartVertexList.end(), [&DistanceList, &Measures](const Vertex& v) {
			bool IsFoundVertexToGo = false;
			Vertex tempVertex = VertexList.begin();
			for_each(SmartVertexList.begin(), SmartVertexList.end(), [&](const Vertex &v) {
				if (used_list.find(v) == used_list.end() && (!IsFoundVertexToGo || DistanceList[v] < DistanceList[tempVertex])) {
					v = tempVertex;
					IsFoundVertexToGo = true;
				}
			});
			
			if (DistanceList[tempVertex] == Measures.INF) {
				return NULL;
			} else {
				while (tempVertex.CanGetNextVertex()) {
					Vertex gotoVertex = tempVertex.NextConnectedVertex();
					DistanceMeasure EdgeWeigth = gotoVertex.NextConnectedVertexWeight();
					if (DistanceList[v] + EdgeWeigth < DistanceList[gotoVertex]) {
						DistanceList[gotoVertex] = DistanceList[v] + EdgeWeigth;
						PreviousWaysVertexList[toVertex] = v;
					}	
				}		
			}
		});
		
	}
	
	
	
};



int main() {



	std::pair< std::vector< std:weak_ptr< SmartVertex > >::const_iterator, std::vector< std::weak_ptr< SmartVertex > >::const_iterator > p;
	CreateGraph(p);
	GraphHandler< SmartVertex, int > handle(vertex_set.begin(), vertex_set.end());		
	return 0;
}
