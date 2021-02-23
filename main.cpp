#include "vtkExample.h"

int main(int argc, char **argv)
{

	vtkSmartPointer<vtkUnstructuredGrid> start1 = loadUnstructuredGrid("D:/tmp/mygrid1.vtu");
	convertUnstructuredToPolyData(start1);
	vtkSmartPointer<vtkUnstructuredGrid> start = vtkSmartPointer <vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkPoints> startPoints = vtkSmartPointer<vtkPoints>::New();
	startPoints->InsertNextPoint(0, 0, 0);
	startPoints->InsertNextPoint(0, 10, 0);
	startPoints->InsertNextPoint(0, 20, 0);
	startPoints->InsertNextPoint(0, 30, 0);
	startPoints->InsertNextPoint(10, 0, 0);
	startPoints->InsertNextPoint(10, 10, 0);
	startPoints->InsertNextPoint(10, 20, 0);
	startPoints->InsertNextPoint(10, 30, 0);
	startPoints->InsertNextPoint(20, 0, 0);
	startPoints->InsertNextPoint(20, 10, 0);
	startPoints->InsertNextPoint(20, 20, 0);
	startPoints->InsertNextPoint(20, 30, 0);
	start->SetPoints(startPoints);

	//cell6=0
	vtkSmartPointer<vtkIdList> sCell6 = vtkSmartPointer<vtkIdList>::New();
	sCell6->InsertNextId(12);
	sCell6->InsertNextId(13);
	sCell6->InsertNextId(17);
	sCell6->InsertNextId(16);
	start->InsertNextCell(VTK_QUAD, sCell6);

	//cell7=1
	vtkSmartPointer<vtkIdList> sCell7 = vtkSmartPointer<vtkIdList>::New();
	sCell7->InsertNextId(13);
	sCell7->InsertNextId(14);
	sCell7->InsertNextId(18);
	sCell7->InsertNextId(17);
	start->InsertNextCell(VTK_QUAD, sCell7);

	//cell8=2
	vtkSmartPointer<vtkIdList> sCell8 = vtkSmartPointer<vtkIdList>::New();
	sCell8->InsertNextId(14);
	sCell8->InsertNextId(15);
	sCell8->InsertNextId(19);
	sCell8->InsertNextId(18);
	start->InsertNextCell(VTK_QUAD, sCell8);

	//cell9=3
	vtkSmartPointer<vtkIdList> sCell9 = vtkSmartPointer<vtkIdList>::New();
	sCell9->InsertNextId(18);
	sCell9->InsertNextId(19);
	sCell9->InsertNextId(23);
	sCell9->InsertNextId(22);
	start->InsertNextCell(VTK_QUAD, sCell9);


	double point[3];
	//список €чеек, в которые входит точка
	vtkIdType id = 3;
	vtkSmartPointer<vtkIdList> neighbourCells = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList> nc = vtkSmartPointer<vtkIdList> ::New();
	start->GetPointCells(id, nc);
	start->Update();
	std::cout << start->GetNumberOfCells();

	for (vtkIdType i = 0; i < neighbourCells->GetNumberOfIds(); i++)
	{
		//список всех точек, куда входит €чейка
		vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
		start->GetCellPoints(neighbourCells->GetId(i), cellPointIds);

		//вывод на экран всех точек €чейки
		for (vtkIdType j = 0; j < cellPointIds->GetNumberOfIds(); j++)
		{
			start->GetPoint(cellPointIds->GetId(j), point);
			std::cout << point[0] << ' ' << point[1] << ' ' << point[2] << std::endl;
		}

		vtkSmartPointer<vtkPolyData> sg = vtkSmartPointer<vtkPolyData>::New();
	}
    return 0;
}