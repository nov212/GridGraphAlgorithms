#include "PFStrategy.h"
#include "Heuristic.h"
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
	int visited = 0;
	int* prev = new int[grid->GetNumberOfPoints()]; //previous elements
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
		prev[i] = -1;

	idq.push(start);
	vtkIdType current = start;
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();
		visited++;
		if (current == end)
			break;

		//neighbours of current vertex
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current, cellPointIds);

		//pushing vertices adjacent to current
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
	std::cout << "BFS visited=" << visited << std::endl;
	return BuildPath(prev, start, end);
}

AStar::Node::Node(vtkIdType _id, Node *_prev, double _cost, double _priority) :id(_id), prev(_prev), cost(_cost), 
priority(_priority) {}

vtkSmartPointer<vtkIdList> AStar::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);
	int visited = 0;
	int next = 0;
	double gValue = 0;
	double fValue = 0;
	Heuristic heuristic;

	std::unordered_map<vtkIdType, Node*> closed; //used states
	std::unordered_map<vtkIdType, Node*> open;	//not used states

	auto cmp = [](const Node* n1, const Node* n2) {return n1->priority < n2->priority; };
	std::set<Node*, decltype(cmp)> idq(cmp);		//priority queue
	idq.emplace(new Node(start, NULL, 0, heuristic.presumptiveLength(grid, start, end)));
	Node* current = NULL;

	while (!idq.empty())
	{
		current = *idq.begin();
		idq.erase(idq.begin());

		closed.emplace(current->id, current);
		visited++;
		if (current->id == end)
			break;

		//neighbours of current vertex
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current->id, cellPointIds);

		//pushing vertices adjacent to current
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			next = cellPointIds->GetId(j);
			gValue = current->cost + 1;
			fValue = gValue + heuristic.presumptiveLength(grid, next, end);
			

			//skip visited vertices
			auto tmp = closed.find(next);
			if (tmp != closed.end())
				continue;	

			auto exist = open.find(next);
			if (exist == open.end() || gValue < (exist->second)->cost) 
			{
				Node* newNode = new Node(next, current, gValue, fValue);
				if (exist != open.end())
				{
					open.insert({ exist->first, newNode });
					idq.erase(exist->second);
				}
				idq.emplace(newNode);
			}
		}
	}

	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();

	//building path
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
	std::cout << "AStar visited=" << visited << std::endl;
	return result;
}

//double AStar::Heuristic(Graph *grid, vtkIdType start, vtkIdType target)
//{
//	double p1[3];
//	double p2[3];
//	grid->GetPoint(start, p1);
//	grid->GetPoint(target, p2);
//	double result = abs(p1[0]-p2[0])+ abs(p1[1] - p2[1])+ abs(p1[2] - p2[2]);	//manhattan distance	
//	return result;
//}

vtkSmartPointer<vtkIdList>BiDirectional::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);
	
	int visited = 0;
	int* prev = new int[grid->GetNumberOfPoints()]; //previous elements
	

	char* label= new char[grid->GetNumberOfPoints()]; 
	std::queue<int> frontq;
	std::queue<int> backq;

	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		label[i] = NONE;
	}
	vtkIdType current = start;
	vtkIdType intersec = end; //intersection vertex
	vtkIdType oncoming = start; //vertex adjacent to intersec
	bool solved = false;
	label[start] = FRONT;
	label[end] =BACK;

	frontq.push(start);
	backq.push(end);

	int pair[2] = { -1, -1 };
	while (!frontq.empty() || !backq.empty())
	{
		solved = BFSIter(label, prev, grid, &frontq, pair);
		visited++;
		if (solved)
			break;
		solved = BFSIter(label, prev, grid, &backq, pair);
		visited++;
		if (solved)
			break;
	}
	std::cout << "BiDir visited=" << visited << std::endl;
	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();

	//building path 
	if (solved)
	{
		oncoming = pair[0];
		intersec = pair[1];
		vtkSmartPointer<vtkIdList> firstHalf = vtkSmartPointer<vtkIdList>::New();
		vtkSmartPointer<vtkIdList> secondHalf = vtkSmartPointer<vtkIdList>::New();
		if (label[intersec] == FRONT)
		{
			//ids from start to intersec
			firstHalf = BuildPath(prev, start, intersec);

			//ids from end to oncoming
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

bool BiDirectional::BFSIter(char* label, int* prev, Graph *grid, std::queue<int> *idq, int* intersec)
{
	if (!idq->empty())
	{
		vtkIdType current = idq->front();
		idq->pop();

		//neighbours of current vertex
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current, cellPointIds);

		//pushing vertices adjacent to current
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			int next = cellPointIds->GetId(j);
			if (label[next] == NONE)
			{
				idq->push(next);
				label[next] = label[current];
				prev[next] = current;
			}
			else
				if (label[next] != label[current])
				{
					intersec[1] = next;		//intersec[0]- oncoming, intersec[1]-intersec
					intersec[0] = current;
					return true;
				}
		}
	}
	return false;
}
//vtkSmartPointer<vtkIdList> AStar::Solve(Graph *grid, vtkIdType start, vtkIdType end)
//{
//	if (start == end)
//		return OneVertexPath(start);
//
//	int next = 0;
//	double gValue = 0;
//	double fValue = 0;
//
//	std::unordered_map<vtkIdType, Node*> closed; //рассмотренные вершины
//
//	//элементы выстроены по возрастанию приоритета
//	auto cmp = [](const Node* n1, const Node* n2) {return n1->priority > n2->priority; };
//	std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> idq(cmp);
//
//	idq.emplace(new Node(start, NULL, 0, Heuristic(grid, start, end)));
//	Node* current = idq.top();
//	while (!idq.empty())
//	{
//		current = idq.top();
//		idq.pop();
//		closed.emplace(current->id, current);
//		if (current->id == end)
//			break;
//
//		//соседи текущей вершины
//		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
//		grid->GetAdj(current->id, cellPointIds);
//
//		//добавление в очередь точек, смежных с текущей
//		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
//		{
//			next = cellPointIds->GetId(j);
//			gValue = current->cost + 1;
//			fValue = Heuristic(grid, next, end);
//
//			//пропускаем рассмотренные вершины
//			auto tmp = closed.find(next);
//			if (tmp != closed.end())
//				continue;
//			idq.emplace(new Node(next, current, gValue, fValue));
//		}
//	}
//
//	vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
//	auto target = closed.find(end);
//	if (target != closed.end())
//	{
//		Node* curr = target->second;
//		while (curr->id != start)
//		{
//			result->InsertNextId(curr->id);
//			curr = curr->prev;
//		}
//		result->InsertNextId(curr->id);
//	}
//	return result;
//}