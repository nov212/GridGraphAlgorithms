#include "vtkExample.h"

int main(int argc, char **argv)
{
	vtkSmartPointer<vtkUnstructuredGrid> start = vtkUnstructuredGrid::New();
	vtkSmartPointer<vtkPoints> startPoints = vtkSmartPointer<vtkPoints>::New();
	startPoints->InsertNextPoint(0, 0, 0);
	startPoints->InsertNextPoint(0, 10, 0);
	startPoints->InsertNextPoint(0, 20, 0);
    createGridExample();
    return 0;
}