#pragma once
#include "Graph.h"
#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>
#include <vtkCell.h>
#include <queue>
/*
	PFStrategy-interface for path finding algorithms:
		BFS-breadth first search
		AStar-A* algirithm
		BiDirectional-bidirectional BFS
*/

class PFStrategy
{
public:
	virtual vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end)=0;
protected:
	vtkSmartPointer<vtkIdList> BuildPath(int *prev, int start, int end);
	vtkSmartPointer<vtkIdList> OneVertexPath(vtkIdType vert);
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
	//double Heuristic(Graph *grid, vtkIdType start, vtkIdType target);
};

class BiDirectional:public PFStrategy
{
public:
	vtkSmartPointer<vtkIdList> Solve(Graph *grid, vtkIdType start, vtkIdType end) override;
private:
	//labels:
	//	N - vertex is not labeled
	//  F - labeled by forward direction
	//	B - labeled by backward direction
	const char FRONT = 'F';
	const char BACK = 'B';
	const char NONE = 'N';
	vtkSmartPointer<vtkIdList> Merge(vtkSmartPointer<vtkIdList> firstHalf, vtkSmartPointer<vtkIdList> secondHalf);
	bool BFSIter(char* label, int* prev, Graph *grid, std::queue<int> *idq, int* intersec);
};
