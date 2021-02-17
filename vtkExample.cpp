#include "vtkExample.h"

/***
 * \brief загрузка неструктурированной сетки
 */
vtkSmartPointer<vtkUnstructuredGrid> loadUnstructuredGrid(const char *fileName)
{
	vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	reader->SetFileName(fileName);
	reader->Update();
	return reader->GetOutput();
}

/***
* \brief загрузка структурированной сетки
*/
vtkSmartPointer<vtkPolyData> loadPolyData(const char *fileName)
{
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName(fileName);
	reader->Update();
	return reader->GetOutput();
}

/***
* \brief конвертация неструктурированной сетки в структурированную
*/
vtkSmartPointer<vtkPolyData> convertUnstructuredToPolyData(vtkSmartPointer<vtkUnstructuredGrid> mesh)
{
	vtkSmartPointer<vtkPolyData> polyMesh = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> points = vtkPoints::New(VTK_DOUBLE);

	for (vtkIdType i = 0; i < mesh->GetNumberOfPoints(); i++)
	{
		points->InsertNextPoint(mesh->GetPoint(i));
	}
	polyMesh->SetPoints(points);
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	for (vtkIdType i = 0; i < mesh->GetNumberOfCells(); i++)
	{
		vtkCell * cell = mesh->GetCell(i);
		cells->InsertNextCell(cell->GetNumberOfPoints());
		for (vtkIdType j = 0; j < cell->GetNumberOfPoints(); j++)
		{
			cells->InsertCellPoint(cell->GetPointId(j));
		}
	}
	polyMesh->SetPolys(cells);

	return polyMesh;
}

/***
 * \brief Пример создания сетки и записи ее в файл
 */
void createGridExample()
{
	vtkSmartPointer<vtkUnstructuredGrid> start = vtkUnstructuredGrid::New();
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
	startPoints->InsertNextPoint(20, 30, 0); // 'Это точки для нижней части сетки (start)

	startPoints->InsertNextPoint(0, 0, 50);
	startPoints->InsertNextPoint(0, 10, 50);
	startPoints->InsertNextPoint(0, 20, 50);
	startPoints->InsertNextPoint(0, 30, 50);
	startPoints->InsertNextPoint(10, 0, 50);
	startPoints->InsertNextPoint(10, 10, 50);
	startPoints->InsertNextPoint(10, 20, 50);
	startPoints->InsertNextPoint(10, 30, 50);
	startPoints->InsertNextPoint(20, 0, 50);
	startPoints->InsertNextPoint(20, 10, 50);
	startPoints->InsertNextPoint(20, 20, 50);
	startPoints->InsertNextPoint(20, 30, 50); // 'Это точки для верхней части сетки (finish)
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

	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkUnstructuredGridWriter::New();
	writer->SetFileName("D:/tmp/mygrid1.vtu");
	writer->SetFileTypeToASCII();
	//writer->SetFileTypeToBinary();
	writer->SetInput(start);
	writer->Update();
}

void meshUsageExample()
{
	// загрузка сетки из файла
	vtkSmartPointer<vtkUnstructuredGrid> Mesh = loadUnstructuredGrid("D:/tmp/mesh.vtu");

	// число точек в сетке
	vtkIdType pointCount = Mesh->GetNumberOfPoints();

	// число ячеек в сетке
	vtkIdType cellCount = Mesh->GetNumberOfCells();

	// получение координат точки по ее идентификатору
	double point[3]; // координаты точки
	vtkIdType pointId = 42; // идентификатор точки
	if (pointId < pointCount)
	{
		Mesh->GetPoint(pointId, point);
	}

	// получение идентификаторов ячеек, в которые входит точка
	vtkSmartPointer<vtkIdList> neighbourCells = vtkSmartPointer<vtkIdList>::New(); // инициализация коллекции идентификаторов ячеек
	Mesh->GetPointCells(pointId, neighbourCells);

	// получение идентификаторов точек, входящих в ячейку
	vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New(); // инициализация коллекции идентификаторов точек
	Mesh->GetCellPoints(neighbourCells->GetId(0), cellPointIds);
}