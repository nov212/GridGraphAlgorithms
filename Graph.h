#pragma once
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>

class Graph
{
private:
	vtkUnstructuredGrid *grid;
public:
	Graph(vtkUnstructuredGrid *grid);
	vtkPoints * GetPoints();
	vtkCellArray * GetCells();
	void GetAdj(vtkIdType node, vtkIdList *neighbours);
	vtkIdType GetNumberOfPoints();
	vtkIdType GetNumberOfCells();
	void GetPoint(vtkIdType id, double x[3]);
};