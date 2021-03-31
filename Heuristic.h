#include <vtkIdList.h>
#include "Graph.h"

static class Heuristic
{
public: 
	double manhattan(Graph *grid, vtkIdType start, vtkIdType target);
	double presumptiveLength(Graph *grid, vtkIdType start, vtkIdType target);
	double euclidian(Graph *grid, vtkIdType start, vtkIdType target);
	double* preprocess(Graph *grid);
private:
	double* stat=NULL;
};