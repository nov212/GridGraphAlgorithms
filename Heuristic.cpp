#include "Heuristic.h"
#include <math.h>

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
	grid->GetPoint(start, p2);
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

	vtkSmartPointer<vtkUnstructuredGrid> mesh = grid->GetComponent();
	extract
	for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); i++)
	{
		vtkCell* cell = mesh->GetCell(i);
		vtkSmartPointer<vtkEdge> points = cell->getEdge;
		cell->getEdg
	}

}