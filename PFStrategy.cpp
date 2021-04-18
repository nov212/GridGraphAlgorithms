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

AStar::AStar()
{
	this->heuristic = new ManhattanHeuristic();
}

AStar::Node::Node(vtkIdType _id, Node *_prev, double _cost, double _priority) :id(_id), prev(_prev), cost(_cost), 
priority(_priority) {}

bool AStar::Node::operator()(const Node* n1, const Node* n2)
{
	if (n1->priority < n2->priority)
		return true;
		if (n1->priority == n2->priority)
			return n1->id < n2->id;
		return false;
}

AStar::Node::~Node()
{
	prev = NULL;
}

void AStar::setHeuristic(Heuristic* heuristic)
{
	this->heuristic = heuristic;
}

double AStar::Gvalue(Graph* grid, Node* current, vtkIdType next)
{
	//APPROACH 1, APPROACH 3
	 return 1;

	//APPROACH 2
	//returns euclidean distance between current and next node
	//return grid->GetDistance(current->id, next);
}

double* AStar::PathLengthFromSource(Graph* grid, Node* target)
{
	//result[0]-length of path going through start node and target node
	//result[1]-number of nodes in the path
	double* result = new double[2]{ 0,0 }; 
	double currentPathLength = 0;
	int nodeCounter=0;
	Node* startNode = target;
	while (startNode->prev != NULL)
	{
		currentPathLength += grid->GetDistance(startNode->id, startNode->prev->id);
		startNode = startNode->prev;
		nodeCounter++;
	}
	result[0] = currentPathLength;
	result[1] = nodeCounter;
	return result;
}

double AStar::Hvalue(Graph* grid, Node* current, vtkIdType next, vtkIdType end)
{
	double edgeLength = grid->GetDistance(current->id, next);
	double pathLengthFromNextToEnd = grid->GetDistance(next, end);
	double hValue = 0;			   //result
	int nodeCounter = 0;		  //number of passed nodes between start and next 
	double currentPathLength = 0; //path length between start node and current node 
	////////////////////////////////////////////////////////////////////
	//APPROACH 1
	// fValue if presumptive count of nodes between next and end node calculated by
	// the formula: DNE/AEL where
	//DNE-euclidean distance between next and end node
	// AEL-average edge length between start and next node
	// Approach 1 is used when gValue is number of passed nodes
	/////////////////////////////////////////////////////////////////////
	//double* distance = PathLengthFromSource(grid, current);

	////distance between current and next node
	//currentPathLength = distance[0] + edgeLength; 
	//nodeCounter = distance[1] + 1;
	//delete[] distance;
	//double averageLength = currentPathLength / nodeCounter;		//average edge length in path
	//hValue = pathLengthFromNextToEnd/averageLength;	

	//////////////////////////////////////////////////////////////////// 
	//APPROACH 2
	// hValue is euclidean distance between  next and end node
	// gValue is path length between current and next node
	////////////////////////////////////////////////////////////////////
	//hValue= grid->GetDistance(next, end);

	////////////////////////////////////////////////////////////////////
	//APPROACH 3
	//using average edge length in whole grid to predict a number of remaining vertices
	//use PathLengthMeter heuristic
	//gValue is number of passed nodes
	////////////////////////////////////////////////////////////////////
	hValue = heuristic->calculate(grid, next, end);
	
	return hValue;
}

vtkSmartPointer<vtkIdList> AStar::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
		return OneVertexPath(start);
	int visited = 0;
	vtkIdType next = 0;
	double gValue = 0;
	double fValue = 0;

	std::unordered_map<vtkIdType, Node*> closed; //used states
	std::unordered_map<vtkIdType, Node*> open;	//not used states

	auto cmp = [](const Node* n1, const Node* n2) { 
		if (n1->priority < n2->priority) 
			return true;
		if (n1->priority == n2->priority)
			return n1->id<n2->id;
		return false;
	};

	std::set < Node*, decltype(cmp)> idq(cmp);		//priority queue
	idq.emplace(new Node(start, NULL, 0, 0)); 

	while (!idq.empty())
	{
		Node* current = *idq.begin();
		idq.erase(idq.begin());

		closed.emplace(current->id, current);
		open.erase(current->id);

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

			//skip visited vertices
			auto tmp = closed.find(next);
			if (tmp != closed.end())
				continue;
			gValue = current->cost+Gvalue(grid, current, next);
			fValue = gValue + Hvalue(grid, current, next, end);
			
			auto exist = open.find(next);
			if (exist == open.end() || gValue < (exist->second)->cost)
			{
				Node* newNode = new Node(next, current, gValue, fValue);
				if (exist != open.end())
				{
					idq.erase(exist->second);
				}
				open.emplace(next, newNode);
				idq.insert(newNode);
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
	std::cout << "AStar visited=" << closed.size() << std::endl;
	return result;
}

AStar::~AStar()
{
	delete heuristic;
}

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