#include "Heuristic.h"
#include <math.h>
#include <vtkExtractEdges.h>

double Heuristic::manhattan(Graph *grid, vtkIdType start, vtkIdType target)
{
	double p1[3];
	double p2[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double result = abs(p1[0] - p2[0]) + abs(p1[1] - p2[1]) + abs(p1[2] - p2[2]);
	return result;
}

double Heuristic::euclidian(Graph *grid, vtkIdType start, vtkIdType target)
{
	double* p1 = new double[3];
	double* p2 = new double[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double res = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
	return res;
}

double* Heuristic::preprocess(Graph *grid)
{
	//stat[0]-average edge length
	//stat[1]-max edge length
	//stat[2]-min edge length
	double* stat = new double[3];
	stat[0] = 0;
	stat[1] = -1;
	stat[2] = VTK_DOUBLE_MAX;

	double length=0;
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
			length = euclidian(grid, edge->GetPointId(0), edge->GetPointId(1));

			if (length > stat[1])
				stat[1] = length;
			if (length<stat[2])
				stat[2] = length;
			stat[0] += length;
		}
	}
	stat[0]=stat[0] / edgeCount;
	return stat;
}


double Heuristic::presumptiveLength(Graph *grid, vtkIdType start, vtkIdType target)
{
	if (stat==NULL)
		stat= preprocess(grid);
	double average = stat[0];
	return euclidian(grid, start, target) / average;

}