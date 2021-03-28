#include <vtkIdList.h>
#include "Graph.h"

static class Heuristic
{
public: 
	static double manhattan(Graph *grid, vtkIdType start, vtkIdType target);
	static double presumptiveLength(Graph *grid, vtkIdType start, vtkIdType target);
	static double euclidian(Graph *grid, vtkIdType start, vtkIdType target);
private:
	int* preprocess(Graph *grid);
};