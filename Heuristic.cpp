#include "Heuristic.h"
#include <math.h>
#include <vtkExtractEdges.h>

double ManhattanHeuristic::calculate(Graph *grid, vtkIdType start, vtkIdType target)
{
	double p1[3];
	double p2[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double result = abs(p1[0] - p2[0]) + abs(p1[1] - p2[1]) + abs(p1[2] - p2[2]);
	return result;
}

double EuclideanHeuristic::calculate(Graph *grid, vtkIdType start, vtkIdType target)
{
	return grid->GetDistance(start, target);
}


PathLengthMeter::PathLengthMeter()
{
	graphStat = NULL;
}

double PathLengthMeter::calculate(Graph *grid, vtkIdType start, vtkIdType target)
{
	if (graphStat == NULL)
	{
		graphStat = new GraphStat();
		graphStat->setGraph(grid);
	}
	double average = graphStat->getAverageEdgeLength();
	double euclideanDistance = grid->GetDistance(start, target);
	//if (euclideanDistance == 0)
	//	return 0;
	double res = euclideanDistance / average;
	return res;

}