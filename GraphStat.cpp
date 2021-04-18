#include "GraphStat.h"
#include <set>
#include <vtkIdTypeArray.h>
#include <vtkExtractEdges.h>

GraphStat::GraphStat()
{
	averageLength = -1;
}

void GraphStat::setGraph(Graph *grid)
{
	double length = 0;
	double totalLength = 0;
	int edgeCount = 0;


	//USING vtkExtractEdges

	//vtkSmartPointer<vtkExtractEdges> extractEdgesAlg = vtkSmartPointer<vtkExtractEdges>::New();
	//extractEdgesAlg->SetInput(grid->GetComponent());
	//extractEdgesAlg->Update();

	////getting result
	//vtkSmartPointer<vtkPolyData> mesh = extractEdgesAlg->GetOutput();

	////for each line in mesh
	//for (vtkIdType i = 0; i < mesh->GetNumberOfLines(); i++)
	//{
	//	//get ids of points of edge
	//	vtkSmartPointer<vtkIdList> points = vtkSmartPointer<vtkIdList>::New();
	//	mesh->GetCellPoints(i, points);
	//	length= GraphStat::edgeLength(grid, points->GetId(0), points->GetId(1));
	//	totalLength += length;
	//	//std::cout << i << ' ' << points->GetId(0) << ' ' << points->GetId(1) << std::endl;
	//}
	//edgeCount = mesh->GetNumberOfLines();

	vtkSmartPointer<vtkUnstructuredGrid> mesh = grid->GetComponent();
	std::set<std::pair<vtkIdType, vtkIdType>> edgeList;

	//обходим все €чейки сетки
	for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); i++)
	{
		vtkSmartPointer<vtkCell> cell = mesh->GetCell(i);

		//рассматриваем все грани €чейки
		for (vtkIdType j = 0; j < cell->GetNumberOfEdges(); j++)
		{
			vtkIdType p1 = cell->GetEdge(j)->GetPointId(0);
			vtkIdType p2 = cell->GetEdge(j)->GetPointId(1);

			//если грань ещЄ не рассматривалась вычисл€ем еЄ длину
			if (edgeList.find(std::make_pair(p1, p2)) == edgeList.end())
			{
				edgeList.emplace(p1, p2);
				edgeCount++;
				//length = GraphStat::edgeLength(grid, p1, p2);
				length = grid->GetDistance(p1, p2);
				totalLength += length;
			}
		}
	}
	averageLength = totalLength / (edgeCount); 
}

double GraphStat::getAverageEdgeLength()
{
	return averageLength;
}

//double GraphStat::edgeLength(Graph *grid, vtkIdType start, vtkIdType target)
//{
//	double* p1 = new double[3];
//	double* p2 = new double[3];
//	grid->GetPoint(start, p1);
//	grid->GetPoint(target, p2);
//	double res = sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
//	delete[] p1;
//	delete[] p2;
//	return res;
//}