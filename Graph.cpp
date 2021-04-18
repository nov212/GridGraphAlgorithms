#include "Graph.h"
#include <vtkSmartPointer.h>
#include <cmath>
#include <set>

Graph::Graph(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
	this->grid = grid;
}


vtkCellArray* Graph::GetCells()
{
	return grid->GetCells();
}

vtkIdType Graph::GetNumberOfPoints()
{
	return grid->GetNumberOfPoints();
}

vtkIdType Graph::GetNumberOfCells()
{
	return grid->GetNumberOfCells();
}
void Graph::GetPoint(vtkIdType id, double x[3])
{
	grid->GetPoint(id, x);
}


void Graph::GetAdj(vtkIdType node, vtkIdList *neighbours)
{
	std::set<vtkIdType> visited;
	//получение €чеек, в которые входит точка
	vtkSmartPointer<vtkIdList> cellIdList = vtkSmartPointer<vtkIdList>::New();
	grid->GetPointCells(node, cellIdList);

	//просмотр всех €чеек, в которые входит точка
	for (vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkCell> cell = grid->GetCell(cellIdList->GetId(i));

		//просмотр всех рЄбер, €чейки
		for (vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
		{
			vtkSmartPointer<vtkCell> edge = cell->GetEdge(e);
			vtkSmartPointer<vtkIdList> pointIdList = edge->GetPointIds();

			//получение id соседней с vertex вершиной
			if (pointIdList->GetId(0) == node || pointIdList->GetId(1) == node)
			{
				if (pointIdList->GetId(0) == node)
				{
					if (visited.find(pointIdList->GetId(1)) == visited.end())
					{
						neighbours->InsertNextId(pointIdList->GetId(1));
						visited.insert(pointIdList->GetId(1));
					}
				}
				else
				{
					if (visited.find(pointIdList->GetId(0)) == visited.end())
					{
						neighbours->InsertNextId(pointIdList->GetId(0));
						visited.insert(pointIdList->GetId(0));
					}
				}
			}
		}
	}
}

void Graph::LoadFile(const char* path)
{
	vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	reader->SetFileName(path);
	reader->Update();
	grid = reader->GetOutput();
}

double Graph::GetDistance(vtkIdType start, vtkIdType end)
{
	double* p1 = new double[3];
	double* p2 = new double[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(end, p2);
	double res = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
	delete[] p1;
	delete[] p2;
	return res;
}


vtkSmartPointer<vtkUnstructuredGrid> Graph::GetComponent()
{
	return grid;
}

