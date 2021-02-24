#pragma once
#include "Graph.h"
#include <vtkPoints.h>
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
	virtual vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end)=0;
protected:
	vtkSmartPointer<vtkIdList> BuildPath(int *prev, int start, int end);
};

class BFS :public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end) override;
};

class AStar : public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end) override;
private:
	struct Node
	{
		vtkIdType id;
		Node* prev;
		double cost;
		double priority;
		Node(vtkIdType _id, Node *_prev=NULL, double _cost=0, double _priority=std::numeric_limits<double>::max());
	};
	double Heuristic(Graph *grid, vtkIdType start, vtkIdType target);
};

class BiDirectional:public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end) override;
};
