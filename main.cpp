#include "vtkExample.h"
#include <unordered_map>
#include "Graph.h"
#include "PFStrategy.h"
#include "Solver.h"

int main(int argc, char **argv)
{
	vtkSmartPointer<vtkUnstructuredGrid> start = vtkUnstructuredGrid::New();
	vtkSmartPointer<vtkPoints> startPoints = vtkSmartPointer<vtkPoints>::New();
	startPoints->InsertNextPoint(0, 0, 0); //0
	startPoints->InsertNextPoint(0, 10, 0);//1
	startPoints->InsertNextPoint(0, 20, 0);//2
	startPoints->InsertNextPoint(0, 30, 0);//3
	startPoints->InsertNextPoint(10, 0, 0);//4
	startPoints->InsertNextPoint(10, 10, 0);//5
	startPoints->InsertNextPoint(10, 20, 0);//6
	startPoints->InsertNextPoint(10, 30, 0);//7
	startPoints->InsertNextPoint(20, 0, 0); //8
	startPoints->InsertNextPoint(20, 10, 0);//9
	startPoints->InsertNextPoint(20, 20, 0);//10
	startPoints->InsertNextPoint(20, 30, 0); //11 'Это точки для нижней части сетки (start)

	startPoints->InsertNextPoint(0, 0, 50);//12
	startPoints->InsertNextPoint(0, 10, 50);//13
	startPoints->InsertNextPoint(0, 20, 50);//14
	startPoints->InsertNextPoint(0, 30, 50); //15
	startPoints->InsertNextPoint(10, 0, 50);//16
	startPoints->InsertNextPoint(10, 10, 50);//17
	startPoints->InsertNextPoint(10, 20, 50);//18
	startPoints->InsertNextPoint(10, 30, 50);//19
	startPoints->InsertNextPoint(20, 0, 50);//20
	startPoints->InsertNextPoint(20, 10, 50);//21
	startPoints->InsertNextPoint(20, 20, 50);//22
	startPoints->InsertNextPoint(20, 30, 50); //23 'Это точки для верхней части сетки (finish)
	start->SetPoints(startPoints);
	vtkSmartPointer<vtkIdList> sCell0 = vtkSmartPointer<vtkIdList>::New();
	sCell0->InsertNextId(0);
	sCell0->InsertNextId(1);
	sCell0->InsertNextId(5);
	sCell0->InsertNextId(4);
	start->InsertNextCell(VTK_QUAD, sCell0);
	vtkSmartPointer<vtkIdList> sCell1 = vtkSmartPointer<vtkIdList>::New();
	sCell1->InsertNextId(1);
	sCell1->InsertNextId(2);
	sCell1->InsertNextId(6);
	sCell1->InsertNextId(5);
	start->InsertNextCell(VTK_QUAD, sCell1);
	vtkSmartPointer<vtkIdList> sCell2 = vtkSmartPointer<vtkIdList>::New();
	sCell2->InsertNextId(2);
	sCell2->InsertNextId(3);
	sCell2->InsertNextId(7);
	sCell2->InsertNextId(6);
	start->InsertNextCell(VTK_QUAD, sCell2);
	vtkSmartPointer<vtkIdList> sCell3 = vtkSmartPointer<vtkIdList>::New();
	sCell3->InsertNextId(6);
	sCell3->InsertNextId(7);
	sCell3->InsertNextId(11);
	sCell3->InsertNextId(10);
	start->InsertNextCell(VTK_QUAD, sCell3);
	vtkSmartPointer<vtkIdList> sCell4 = vtkSmartPointer<vtkIdList>::New();
	sCell4->InsertNextId(5);
	sCell4->InsertNextId(6);
	sCell4->InsertNextId(10);
	sCell4->InsertNextId(9);
	start->InsertNextCell(VTK_QUAD, sCell4);
	vtkSmartPointer<vtkIdList> sCell5 = vtkSmartPointer<vtkIdList>::New();
	sCell5->InsertNextId(4);
	sCell5->InsertNextId(5);
	sCell5->InsertNextId(9);
	sCell5->InsertNextId(8);
	start->InsertNextCell(VTK_QUAD, sCell5);

	vtkSmartPointer<vtkIdList> sCell6 = vtkSmartPointer<vtkIdList>::New();
	sCell6->InsertNextId(12);
	sCell6->InsertNextId(13);
	sCell6->InsertNextId(17);
	sCell6->InsertNextId(16);
	start->InsertNextCell(VTK_QUAD, sCell6);
	vtkSmartPointer<vtkIdList> sCell7 = vtkSmartPointer<vtkIdList>::New();
	sCell7->InsertNextId(13);
	sCell7->InsertNextId(14);
	sCell7->InsertNextId(18);
	sCell7->InsertNextId(17);
	start->InsertNextCell(VTK_QUAD, sCell7);
	vtkSmartPointer<vtkIdList> sCell8 = vtkSmartPointer<vtkIdList>::New();
	sCell8->InsertNextId(14);
	sCell8->InsertNextId(15);
	sCell8->InsertNextId(19);
	sCell8->InsertNextId(18);
	start->InsertNextCell(VTK_QUAD, sCell8);
	vtkSmartPointer<vtkIdList> sCell9 = vtkSmartPointer<vtkIdList>::New();
	sCell9->InsertNextId(18);
	sCell9->InsertNextId(19);
	sCell9->InsertNextId(23);
	sCell9->InsertNextId(22);
	start->InsertNextCell(VTK_QUAD, sCell9);
	vtkSmartPointer<vtkIdList> sCell10 = vtkSmartPointer<vtkIdList>::New();
	sCell10->InsertNextId(17);
	sCell10->InsertNextId(18);
	sCell10->InsertNextId(22);
	sCell10->InsertNextId(21);
	start->InsertNextCell(VTK_QUAD, sCell10);
	vtkSmartPointer<vtkIdList> sCell11 = vtkSmartPointer<vtkIdList>::New();
	sCell11->InsertNextId(16);
	sCell11->InsertNextId(17);
	sCell11->InsertNextId(21);
	sCell11->InsertNextId(20);
	start->InsertNextCell(VTK_QUAD, sCell11);

	vtkSmartPointer<vtkIdList> sCell12 = vtkSmartPointer<vtkIdList>::New();
	sCell12->InsertNextId(20);
	sCell12->InsertNextId(21);
	sCell12->InsertNextId(9);
	sCell12->InsertNextId(8);
	start->InsertNextCell(VTK_QUAD, sCell12);

	vtkSmartPointer<vtkIdList> sCell13 = vtkSmartPointer<vtkIdList>::New();
	sCell13->InsertNextId(21);
	sCell13->InsertNextId(22);
	sCell13->InsertNextId(10);
	sCell13->InsertNextId(9);
	start->InsertNextCell(VTK_QUAD, sCell13);

	vtkSmartPointer<vtkIdList> sCell14 = vtkSmartPointer<vtkIdList>::New();
	sCell14->InsertNextId(22);
	sCell14->InsertNextId(23);
	sCell14->InsertNextId(11);
	sCell14->InsertNextId(10);
	start->InsertNextCell(VTK_QUAD, sCell14);

	vtkSmartPointer<vtkIdList> sCell15 = vtkSmartPointer<vtkIdList>::New();
	sCell15->InsertNextId(23);
	sCell15->InsertNextId(19);
	sCell15->InsertNextId(7);
	sCell15->InsertNextId(11);
	start->InsertNextCell(VTK_QUAD, sCell15);

	vtkSmartPointer<vtkIdList> sCell16 = vtkSmartPointer<vtkIdList>::New();
	sCell16->InsertNextId(19);
	sCell16->InsertNextId(15);
	sCell16->InsertNextId(3);
	sCell16->InsertNextId(7);
	start->InsertNextCell(VTK_QUAD, sCell16);

	vtkSmartPointer<vtkIdList> sCell17 = vtkSmartPointer<vtkIdList>::New();
	sCell17->InsertNextId(12);
	sCell17->InsertNextId(13);
	sCell17->InsertNextId(1);
	sCell17->InsertNextId(0);
	start->InsertNextCell(VTK_QUAD, sCell17);

	vtkSmartPointer<vtkIdList> sCell18 = vtkSmartPointer<vtkIdList>::New();
	sCell18->InsertNextId(13);
	sCell18->InsertNextId(14);
	sCell18->InsertNextId(2);
	sCell18->InsertNextId(1);
	start->InsertNextCell(VTK_QUAD, sCell18);

	vtkSmartPointer<vtkIdList> sCell19 = vtkSmartPointer<vtkIdList>::New();
	sCell19->InsertNextId(14);
	sCell19->InsertNextId(15);
	sCell19->InsertNextId(3);
	sCell19->InsertNextId(2);
	start->InsertNextCell(VTK_QUAD, sCell19);

	vtkSmartPointer<vtkIdList> sCell20 = vtkSmartPointer<vtkIdList>::New();
	sCell20->InsertNextId(12);
	sCell20->InsertNextId(16);
	sCell20->InsertNextId(4);
	sCell20->InsertNextId(0);
	start->InsertNextCell(VTK_QUAD, sCell20);

	vtkSmartPointer<vtkIdList> sCell21 = vtkSmartPointer<vtkIdList>::New();
	sCell21->InsertNextId(16);
	sCell21->InsertNextId(20);
	sCell21->InsertNextId(8);
	sCell21->InsertNextId(4);
	start->InsertNextCell(VTK_QUAD, sCell21);

//vtkSmartPointer<vtkIdList> cell = vtkSmartPointer<vtkIdList>::New();
//start->GetCellPoints(2, cell);
//for (vtkIdType i = 0; i < cell->GetNumberOfIds(); i++)
//	std::cout << cell->GetId(i)<<' ';
//std::cout<<std::endl;

	//double point[3];
	////список ячеек, в которые входит точка
	//vtkIdType id = 3;
	////vtkSmartPointer<vtkIdList> neighbourCells = vtkSmartPointer<vtkIdList>::New();
	////vtkSmartPointer<vtkIdList> nc = vtkSmartPointer<vtkIdList> ::New();
	////start->GetPointCells(id, nc);

	//std::cout<<"Number of cells " << start->GetNumberOfCells()<<std::endl;
	Graph g(start);
	/*vtkSmartPointer<vtkIdList> neighbours = vtkSmartPointer<vtkIdList>::New();
	g.GetAdj(2, neighbours);
	std::cout << neighbours->GetNumberOfIds()<<std::endl;

	for (vtkIdType i = 0; i < neighbours->GetNumberOfIds(); i++)
	{
		std::cout << neighbours->GetId(i) << ' ';
	}*/
	//std::cout << std::endl;
	//std::cout << g.GetNumberOfPoints() << ' '<<std::endl;

	AStar astar;
	BiDirectional bidir;
	Solver s;
	s.SetStrategy(astar);
	vtkSmartPointer<vtkIdList> solution=s.Solve(&g, 8, 15);
	for (vtkIdType i = 0; i < solution->GetNumberOfIds(); i++)
	{
		std::cout << solution->GetId(i) << ' ';
	}
    return 0;
}