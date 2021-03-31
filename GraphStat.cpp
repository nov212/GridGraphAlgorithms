#include "GraphStat.h"
#include <set>
#include <vtkIdTypeArray.h>


GraphStat::GraphStat()
{
	averageLength = -1;
}

void GraphStat::setGraph(Graph *grid)
{
	std::set<std::pair<vtkIdType, vtkIdType>> edges;

	double length = 0;
	double totalLength = 0;
	int edgeCount = 0;

	vtkSmartPointer<vtkUnstructuredGrid> mesh = grid->GetComponent();

	//просмотр каждой €чейки сетки
	for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); i++)
	{
		//подсчЄт длины каждого ребра €чейки
		vtkSmartPointer<vtkCell> cell = mesh->GetCell(i);

		for (vtkIdType j = 0; j < cell->GetNumberOfEdges(); j++)
		{
			vtkSmartPointer<vtkCell> edge = cell->GetEdge(j);
			edgeCount++;
			length = GraphStat::edgeLength(grid, edge->GetPointId(0), edge->GetPointId(1));
			totalLength += length;
		}
	}
	averageLength = totalLength / edgeCount;
}

double GraphStat::getAverageEdgeLength()
{
	return averageLength;
}

double GraphStat::edgeLength(Graph *grid, vtkIdType start, vtkIdType target)
{
	double* p1 = new double[3];
	double* p2 = new double[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double res = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
	return res;
}