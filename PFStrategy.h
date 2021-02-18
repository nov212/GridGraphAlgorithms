#pragma once
#include<vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>
#include <vtkCell.h>

class PFStrategy
{
public:
	virtual vtkSmartPointer<vtkIdList> Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)=0;
protected:
	void PFStrategy::GetAdj(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType vertex, vtkSmartPointer<vtkIdList> neighbours);
};

class BFS :public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end) override;
};

class AStar : public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end);
private:
	double Heuristic(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType target);
};