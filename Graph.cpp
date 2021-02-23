#include "Graph.h"
#include <vtkSmartPointer.h>

Graph::Graph(vtkUnstructuredGrid *grid)
{
	this->grid = grid;
}

vtkPoints* Graph::GetPoints()
{
	return grid->GetPoints();
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
					neighbours->InsertNextId(pointIdList->GetId(1));
				}
				else
				{
					neighbours->InsertNextId(pointIdList->GetId(0));
				}
			}
		}
	}
}