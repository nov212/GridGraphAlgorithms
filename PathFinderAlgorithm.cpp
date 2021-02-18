#pragma once
#include<vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>

class PathFinderAlgorithm
{
public:
	virtual vtkSmartPointer<vtkIdList> Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)=0;
};