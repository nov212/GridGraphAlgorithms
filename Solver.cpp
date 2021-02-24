#include "Solver.h"

Solver::Solver()
{
	strategy = new BFS();
}
void Solver::SetStrategy(PFStrategy& _strategy)
{
	strategy = &_strategy;
}

vtkSmartPointer<vtkIdList> Solver::Solve(Graph* g, vtkIdType start, vtkIdType end)
{
	return strategy->Solve(g, start, end);
}