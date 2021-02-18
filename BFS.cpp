#include "PFStrategy.h"
#include <queue>
#include <set>

void PFStrategy::GetAdj(vtkSmartPointer<vtkUnstructuredGrid> graph, vtkIdType vertex, 
	vtkSmartPointer<vtkIdList> neighbours)
{	
	//��������� �����, � ������� ������ �����
	vtkSmartPointer<vtkIdList> cellIdList =vtkSmartPointer<vtkIdList>::New();
	graph->GetPointCells(vertex, cellIdList);

	//�������� ���� �����, � ������� ������ �����
	for (vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkCell> cell = graph->GetCell(cellIdList->GetId(i));

		//�������� ���� ����, ������
		for (vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
		{
			vtkSmartPointer<vtkCell> edge = cell->GetEdge(e);
			vtkSmartPointer<vtkIdList> pointIdList = edge->GetPointIds();

			//��������� id �������� � vertex ��������
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
	int* prev = new int[grid->GetNumberOfPoints()]; //������ ���������� ���������
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

		//������ ������� �������
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		GetAdj(grid, current, cellPointIds);

		//���������� � ������� �����, ������� � �������
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
	double tenativeScore = 0;
	std::set<int> closed;
	std::priority_queue<int, double> idq;

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
		closed.insert(current);
		if (current == end)
			break;
		//������ ������� �������
		vtkSmartPointer<vtkIdList> cellPointIds = GetAdj(grid, current);
		//���������� � ������� �����, ������� � �������
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			int next = cellPointIds->GetId(j);
			tenativeScorecost[next]
		}
	}
}
