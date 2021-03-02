#include "PFStrategy.h"
#include <queue>
#include <unordered_map>
#include <set>
#include <cmath>
#include <map>

vtkSmartPointer<vtkIdList> PFStrategy::BuildPath(int *prev, int start, int end)
{
	if (start==end)
		return OneVertexPath(start);
	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	if (prev[end] > -1)
	{
		int current = end;
		while (current != start)
		{
			result->InsertNextId(current);
			current = prev[current];
		}
		result->InsertNextId(current);
	}
	return result;
}

vtkSmartPointer<vtkIdList> PFStrategy::OneVertexPath(vtkIdType vert)
{
	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	result->InsertNextId(vert);
	return result;
}

vtkSmartPointer<vtkIdList> BFS::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);

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
		grid->GetAdj(current, cellPointIds);

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
	return BuildPath(prev, start, end);
}

AStar::Node::Node(vtkIdType _id, Node *_prev, double _cost, double _priority) :id(_id), prev(_prev), cost(_cost), 
priority(_priority) {}

vtkSmartPointer<vtkIdList> AStar::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);

	int next = 0;
	double gValue = 0;
	double fValue = 0;

	std::unordered_map<vtkIdType, Node*> closed; //рассмотренные вершины

	//очередь с приоритетами, каждый элемент это пара:
	//	первый элемент-приоритет узла
	//	второй элемент-узел

	//элементы выстроены по убыванию приоритета
	auto cmp = [](const Node* n1, const Node* n2) {return n1->priority > n2->priority; };
	std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> idq(cmp);

	idq.emplace(new Node(start, NULL, 0, Heuristic(grid, start, end)));
	Node* current = idq.top();
	while (!idq.empty())
	{
		current = idq.top();
		idq.pop();
		closed.emplace(current->id, current);
		if (current->id == end)
			break;

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current->id, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			next = cellPointIds->GetId(j);
			gValue = current->cost + 1;
		    fValue = Heuristic(grid, next, end);

			//пропускаем рассмотренные вершины
			auto tmp = closed.find(next);
			if (tmp != closed.end())
				continue;
			idq.emplace(new Node(next, current, gValue, fValue));
		}
	}

	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	auto target = closed.find(end);
	if (target!=closed.end())
	{
		Node* curr = target->second;
		while (curr->id != start)
		{
			result->InsertNextId(curr->id);
			curr = curr->prev;
		}
		result->InsertNextId(curr->id);
	}
	return result;
}

double AStar::Heuristic(Graph *grid, vtkIdType start, vtkIdType target)
{
	double p1[3];
	double p2[3];
	grid->GetPoint(start, p1);
	grid->GetPoint(target, p2);
	double result = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
	return result;
}

vtkSmartPointer<vtkIdList>BiDirectional::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);
	

	int* prev = new int[grid->GetNumberOfPoints()]; //массив предыдущих элементов
	//метка для вершин:
	//	N - вершина не помечена
	//  F - вершина помечена в прямом направлении
	//	B - вершина помечена встречным направлением
	const char FRONT = 'F';
	const char BACK = 'B';
	const char NONE = 'N';

	char* label= new char[grid->GetNumberOfPoints()]; 
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		label[i] = NONE;
	}
	vtkIdType current = start;
	vtkIdType intersec = end; //точка пересечения
	vtkIdType oncoming = start; //вершина, у которой сосед принадлежит пересечению
	bool solved = false;
	label[start] = FRONT;
	label[end] =BACK;

	idq.push(start);
	idq.push(end);
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();

		//соседи текущей вершины
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current, cellPointIds);

		//добавление в очередь точек, смежных с текущей
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			int next = cellPointIds->GetId(j);
			if (label[next] == NONE)
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

	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	if (solved)
	{
		vtkSmartPointer<vtkIdList> firstHalf = vtkSmartPointer<vtkIdList>::New();
		vtkSmartPointer<vtkIdList> secondHalf = vtkSmartPointer<vtkIdList>::New();
		if (label[intersec] == FRONT)
		{
			firstHalf = BuildPath(prev, start, intersec);
			secondHalf = BuildPath(prev, end, oncoming);
		}
		if (label[intersec] == BACK)
		{
			firstHalf = BuildPath(prev, start, oncoming);
			secondHalf = BuildPath(prev, end, intersec);
		}
		result = Merge(firstHalf, secondHalf);
	}
	return result;
}

vtkSmartPointer<vtkIdList> BiDirectional::Merge(vtkSmartPointer<vtkIdList> firstHalf, vtkSmartPointer<vtkIdList> secondHalf)
{
	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
	for (vtkIdType i = secondHalf->GetNumberOfIds() - 1; i >= 0; --i)
		result->InsertNextId(secondHalf->GetId(i));

	for (vtkIdType i = 0; i < firstHalf->GetNumberOfIds(); ++i)
		result->InsertNextId(firstHalf->GetId(i));
	return result;
}