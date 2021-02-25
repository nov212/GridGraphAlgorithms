#include "PFStrategy.h"
#include <queue>
#include <unordered_map>
#include <cmath>

vtkSmartPointer<vtkIdList> PFStrategy::BuildPath(int *prev, int start, int end)
{
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

vtkSmartPointer<vtkIdList> BFS::Solve(Graph *grid, vtkIdType start, vtkIdType end)
{
	if (start == end)
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertNextId(start);
		return result;
	}

	int* prev = new int[grid->GetNumberOfPoints()]; //������ ���������� ���������
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
		prev[i] = -1;

	idq.push(start);
	prev[start] = start;
	vtkIdType current = start;
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();
		if (current == end)
			break;

		//������ ������� �������
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current, cellPointIds);

		//���������� � ������� �����, ������� � �������
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
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertNextId(start);
		return result;
	}

	int next = 0;
	double gValue = 0;
	double fValue = 0;

	std::unordered_map<vtkIdType, Node*> closed; //������������� �������

	//������� � ������������, ������ ����� ������� � ���������� �����������
	auto cmp = [](const Node* n1, const Node* n2) {return (n1->priority > n2->priority); };
	std::priority_queue< Node*, std::vector<Node*>, decltype(cmp)> idq(cmp);

	idq.push(new Node(start, NULL, 0, Heuristic(grid, start, end)));

	Node* current = idq.top();
	while (!idq.empty())
	{
		current = idq.top();
		idq.pop();
		closed.emplace(current->id, current);
		if (current->id == end)
			break;

		//������ ������� �������
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current->id, cellPointIds);

		//���������� � ������� �����, ������� � �������
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			next = cellPointIds->GetId(j);
			gValue = current->cost + 1;
			auto tmp = closed.find(next);
			if (tmp == closed.end() || gValue < (tmp->second)->cost)
			{
				fValue = gValue + Heuristic(grid, next, end);
				idq.push(new Node(next, current, gValue, fValue));
			}
			
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
	{
		vtkSmartPointer<vtkIdList> result = vtkSmartPointer<vtkIdList>::New();
		result->InsertNextId(start);
		return result;
	}
	int* prev = new int[grid->GetNumberOfPoints()]; //������ ���������� ���������
	//����� ��� ������:
	//	N - ������� �� ��������
	//  F - ������� �������� � ������ �����������
	//	B - ������� �������� ��������� ������������
	char* label= new char[grid->GetNumberOfPoints()]; 
	std::queue<int> idq;
	for (vtkIdType i = 0; i < grid->GetNumberOfPoints(); i++)
	{
		prev[i] = -1;
		label[i] = 'N';
	}
	vtkIdType current = start;
	vtkIdType intersec = end; //����� �����������
	vtkIdType oncoming = start; //�������, � ������� ����� ����������� �����������
	bool solved = false;
	label[start] = 'F';
	label[end] ='B';
	prev[start] = start;
	prev[end] = end;
	idq.push(start);
	idq.push(end);
	while (!idq.empty())
	{
		current = idq.front();
		idq.pop();

		//������ ������� �������
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		grid->GetAdj(current, cellPointIds);

		//���������� � ������� �����, ������� � �������
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

	if (solved)
	{
		//���������� ���� ����� � ����� �����������
		int nextVert = prev[intersec];
		int prevVert = oncoming;
		int currVert = intersec;

		while (currVert != end)
		{
			nextVert = prev[currVert];
			prev[currVert] = prevVert;
			prevVert = currVert;
			currVert = nextVert;
		}
		prev[end] = prevVert;
	}
	return BuildPath(prev, start, end);
}