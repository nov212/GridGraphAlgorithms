#pragma once
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGridReader.h>

class Graph
{
private:
	vtkSmartPointer<vtkUnstructuredGrid> grid;
public:
	Graph(vtkSmartPointer<vtkUnstructuredGrid> grid);
	vtkCellArray * GetCells();
	void GetAdj(vtkIdType node, vtkIdList *neighbours);
	vtkIdType GetNumberOfPoints();
	vtkIdType GetNumberOfCells();
	void GetPoint(vtkIdType id, double x[3]);
	void LoadFile(const char* path);
	vtkSmartPointer<vtkUnstructuredGrid> GetComponent();
};