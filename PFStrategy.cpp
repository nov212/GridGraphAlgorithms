#include "PFStrategy.h"
#include <queue>
#include <set>
#include <hash_map>
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
	if (start == end)
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertId(start);
		return result;
	}

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
			if (prev[next] == -1)
			{
				idq.push(next);
				prev[next] = current;
			}
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

AStar::Node::Node(vtkIdType _id, vtkIdType _prev, double _cost) :id(_id), prev(_prev), cost(_cost) {}

vtkSmartPointer<vtkIdList> AStar::Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertId(start);
		return result;
	}

	/*int *prev = new int[grid->GetNumberOfPoints()];
	int *cost = new int[grid->GetNumberOfPoints()];*/
	int next = 0;
	double gValue = 0;
	double fValue = 0;
	std::set<Node&> closed;
	auto cmp = [](const Node *n1, const Node *n2) {return (n1->cost > n2->cost); };
	std::priority_queue<const Node*, std::vector<Node*>, decltype(cmp)> idq(cmp);

	/*for (int i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		cost = 0;
	}*/

	Node* current = NULL;
	idq.emplace(new Node(start, Heuristic(grid, start, end)));
	while (!idq.empty())
	{
		current = idq.top();
		idq.pop();
		closed.emplace(current);
		if (current->id == end)
			break;

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		GetAdj(grid, current->id, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			if 
			next = cellPointIds->GetId(j);
			gValue = cost[next] + 1;
			fValue = gValue + Heuristic(grid, current->cost, end);

			////пропуск рассомтренных вершин или неперспективных 
			//if (prev[next] > -1 && tenativeScore > cost[next])
			//	continue;

			if (prev[next] == -1 || fValue < cost[next])
			{
				prev[next] = current->id;
				cost[next] = fValue;
				idq.emplace(new Node(next, fValue));
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

vtkSmartPointer<vtkIdList>BiDirectional::Solve(vtkSmartPointer<vtkUnstructuredGrid> grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertId(start);
		return result;
	}
	int* prev = new int[grid->GetNumberOfPoints()]; //массив предыдущих элементов
	//метка для вершин:
	//	N - вершина не помечена
	//  F - вершина помечена в прямом направлении
	//	B - вершина помечена встречным направлением
	char* label= new char[grid->GetNumberOfPoints()]; 
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		label[i] = 'N';
	}
	vtkIdType current = start;
	vtkIdType intersec = end; //точка пересечения
	vtkIdType oncoming = start; //вершина, у которой сосед принадлежит пересечению
	bool solved = false;
	label[start] = 'F';
	label[end] ='B';
	idq.push(start);
	idq.push(end);
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		GetAdj(grid, current, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			int next = cellPointIds->GetId(j);
			if (label[next] == 'N')
			{
				idq.push(next);
				label[next] = label[current];
				prev[next] = current;
			}
			else 
				if (label[next] !=label[current])
				{
					intersec = next;
					oncoming = current;
					solved = true;
					break;
				}
		}
		
		if (solved)
			break;
	}

}
