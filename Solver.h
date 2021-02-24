#pragma once
#include "PFStrategy.h"
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include "Graph.h"

class Solver
{
private:
	PFStrategy* strategy;
public:
	Solver();
	void SetStrategy(PFStrategy& _strategy);
	vtkSmartPointer<vtkIdList> Solve(Graph* g, vtkIdType start, vtkIdType end);
};