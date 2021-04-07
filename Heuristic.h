#pragma once
#include <vtkIdList.h>
#include "Graph.h"
#include "GraphStat.h"

class Heuristic
{
public: 
	virtual double calculate(Graph *grid, vtkIdType start, vtkIdType target)=0;
};

class ManhattanHeuristic: public Heuristic
{
public:
	virtual double calculate(Graph *grid, vtkIdType start, vtkIdType target) override;
};

class EuclideanHeuristic : public Heuristic
{
public:
	virtual double calculate(Graph *grid, vtkIdType start, vtkIdType target) override;
};

class PathLengthMeter : public Heuristic
{
public:
	PathLengthMeter();
	virtual double calculate(Graph *grid, vtkIdType start, vtkIdType target) override;
private:
	GraphStat* graphStat;
	double euclidean(Graph *grid, vtkIdType start, vtkIdType target);
};