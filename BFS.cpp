#include "BFS.h"
#include <queue>

vtkSmartPointer<vtkIdList> BFS::Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)
{
	int* pred = new int[grid->GetNumberOfPoints()]; //массив предыдущих элементов
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
		pred[i] = -1;

	idq.push(start);
	vtkIdType current = start;
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();
		if (current == end)
			break;

		//получение €чеек, соседних с текущей
		vtkSmartPointer<vtkIdList> neighbourCells = vtkSmartPointer<vtkIdList>::New();
		grid->GetPointCells(current, neighbourCells);

		for (vtkIdType i = 0; i < neighbourCells->GetNumberOfIds(); i++)
		{
			//получение точек €чейки
			vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
			grid->GetCellPoints(neighbourCells->GetId(i), cellPointIds);

			//добавление в очередь точек, смежных с текущей
			for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
			{
				int next = cellPointIds->GetId(j);
				if (next != current)
				{
					idq.push(next);
					pred[next] = current;
				}
			}
		}
	}

	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	if (pred[end] > -1)
	{
		while (pred[current] != -1)
		{
			result->InsertNextId(current);
			current = pred[current];
		}
		result->InsertNextId(current);
	}
	return result;

}
