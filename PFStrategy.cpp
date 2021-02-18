#include "PFStrategy.h"
#include <queue>
#include <cmath>

void PFStrategy::GetAdj(vtkSmartPointer<vtkUnstructuredGrid> graph, vtkIdType vertex, 
	vtkSmartPointer<vtkIdList> neighbours)
{	
	//получение ячеек, в которые входит точка
	vtkSmartPointer<vtkIdList> cellIdList =vtkSmartPointer<vtkIdList>::New();
	graph->GetPointCells(vertex, cellIdList);

	//просмотр всех ячеек, в которые входит точка
	for (vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkCell> cell = graph->GetCell(cellIdList->GetId(i));

		//просмотр всех рёбер, ячейки
		for (vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
		{
			vtkSmartPointer<vtkCell> edge = cell->GetEdge(e);
			vtkSmartPointer<vtkIdList> pointIdList = edge->GetPointIds();

			//получение id соседней с vertex вершиной
			if (pointIdList->GetId(0) == vertex || pointIdList->GetId(1) == vertex)
			{
				if (pointIdList->GetId(0) == vertex)
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
vtkSmartPointer<vtkIdList> BFS::Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end) 
{
	int* prev = new int[grid->GetNumberOfPoints()]; //массив предыдущих элементов
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
		prev[i] = -1;

	idq.push(start);
	vtkIdType current = start;
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();
		if (current == end)
			break;

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		GetAdj(grid, current, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			int next = cellPointIds->GetId(j);
			idq.push(next);
			prev[next] = current;
		}

	}

	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	if (prev[end] > -1)
	{
		while (prev[current] != -1)
		{
			result->InsertNextId(current);
			current = prev[current];
		}
		result->InsertNextId(current);
	}
	return result;

}

vtkSmartPointer<vtkIdList> AStar::Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)
{
	int *prev = new int[grid->GetNumberOfPoints()];
	int *cost = new int[grid->GetNumberOfPoints()];
	int next = 0;
	double tenativeScore = 0;
	std::priority_queue<int, double, std::greater<double>> idq;

	for (int i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		cost = 0;
	}

	vtkIdType current = start;
	idq.emplace(start, 0);
	while (!idq.empty())
	{
		current = idq.top();
		idq.pop();
		if (current == end)
			break;

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		GetAdj(grid, current, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			next = cellPointIds->GetId(j);
			tenativeScore = cost[next] + 1;

			//пропуск рассомтренных вершин или неперспективных 
			if (prev[next] > -1 && tenativeScore > cost[next])
				continue;

			if (prev[next] == -1 || tenativeScore < cost[next])
			{
				prev[next] = current;
				cost[next] = tenativeScore;
			}
		}
	}
}

double AStar::Heuristic(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType target)
{
	double p1[3];
	double p2[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double result = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
	return result;
}
