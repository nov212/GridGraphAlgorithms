#include "Graph.h"
class GraphStat
{
private:
	static double edgeLength(Graph *grid, vtkIdType start, vtkIdType target);
public:
	static double getAverageEdgeLength(Graph *grid);
};