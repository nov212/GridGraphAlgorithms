#include "PFStrategy.h"
#include <queue>
#include <unordered_map>
#include <set>
#include <cmath>
#include <map>

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

	//������� � ������������, ������ ������� ��� ����:
	//	������ �������-��������� ����
	//	������ �������-����
	//�������� ��������� �� �������� ����������
	std::set<std::pair<double, Node*>> idq;
	idq.insert(std::make_pair(Heuristic(grid, start, end), new Node(start, NULL, 0, Heuristic(grid, start, end))));

	//Node* current = idq.begin()->second;
	while (!idq.empty())
	{
		Node* current = idq.begin()->second;
		std::cout << "current=" << current->id << " priority=" << fValue << std::endl;
		idq.erase(idq.begin());
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
		    fValue = gValue + Heuristic(grid, next, end);

			auto tmp = closed.find(next);
			if (tmp != closed.end())
				continue;

			//auto exist= idq.find(std::make_pair((tmp->second)->priority, tmp->second));
			auto exist = idq.find(std::make_pair((tmp->second)->priority, tmp->second));
			if (exist == idq.end() || fValue < exist->first)
			{
				if (exist != idq.end())
				{
					idq.erase(std::make_pair(exist->first, tmp->second));
					std::cout << "EREASE-------------------" << std::endl;
				}
				idq.insert(std::make_pair(fValue, new Node(next, current, gValue, fValue)));
				std::cout << "PUSHED next="<<next << " current="<<current->id << " priority="<<fValue<<std::endl;
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