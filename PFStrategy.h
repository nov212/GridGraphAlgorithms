#pragma once
#include<vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>
#include <vtkCell.h>
/*
	PFStrategy-общий интерфейс для алгоритмов поиска:
		BFS-поиск в ширину
		AStar-алгоритм A*
		BiDirectional-двунаправленный поиск в ширину
*/

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
	struct Node
	{
		vtkIdType id;
		vtkIdType prev;
		double cost;
		Node(vtkIdType _id, vtkIdType _prev=-1, double _cost=0);
	};
	double Heuristic(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType target);
};

class BiDirectional:public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end) override;
};
