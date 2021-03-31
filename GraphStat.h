#include "Graph.h"
class GraphStat
{
private:
	double edgeLength(Graph *grid, vtkIdType start, vtkIdType target);
	double averageLength;
public:
	GraphStat();
	void setGraph(Graph *grid);
	double getAverageEdgeLength();
};