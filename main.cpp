#include "vtkExample.h"
#include <unordered_map>
#include "Graph.h"
#include "PFStrategy.h"
#include "Solver.h"
#include <set>
#include <vtkHexahedron.h>
#include <vtkPolyhedron.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <queue>
#include <chrono>


vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedronGrid();		//construct grid built from VTK_HEXAHEDRON cells
vtkSmartPointer<vtkUnstructuredGrid> ReadFile(const char* path);	//read .vtu file
void WriteFile(vtkSmartPointer<vtkUnstructuredGrid> grid, const char* path); //convert grid to .vtu file
void Performance(PFStrategy &algorithm, Graph &g, vtkIdType start, vtkIdType finish); //print time and path length
void Test();

//simple tests on hexahedron grid
void TestBFS();	
void TestBiDir();	
void TestAstar();
void TestExample(const char*, vtkIdType, vtkIdType);



int main(int argc, char **argv)
{
	TestExample("D:/meshPathFind/cube_10_layer5_auto.vtk", 10830, 510);
	TestExample("D:/meshPathFind/cube_17_hexahedron.vtk", 4, 6);
	TestExample("D:/meshPathFind/cube_17_tetra.vtk", 8265, 4319);
	TestExample("D:/meshPathFind/cuboid_10x1000_auto.vtk", 34, 161354);
	TestExample("D:/meshPathFind/gear.vtk", 5074, 10054);
	TestExample("D:/meshPathFind/quad_10_quad.vtk", 1, 2);
	TestExample("D:/meshPathFind/quad_10_quad_auto.vtk", 6686, 7682);
	TestExample("D:/meshPathFind/solid-loft.vtk", 0, 101);
	TestExample("D:/meshPathFind/solid-loft.vtk", 0, 786);
	TestExample("D:/meshPathFind/solid-loft.vtk", 222, 1200);
	TestExample("D:/meshPathFind/quad_10_quad.vtk", 2827, 7183);
	TestExample("D:/meshPathFind/quad_10_quad_auto.vtk", 0, 186);
	TestExample("D:/meshPathFind/cube_10_layer5_auto.vtk", 11506, 12670);
	TestExample("D:/meshPathFind/cube_10_layer5_auto.vtk", 11506, 187);
	TestExample("D:/meshPathFind/cube_17_hexahedron.vtk", 4, 2);
	TestExample("D:/meshPathFind/cube_17_hexahedron.vtk", 15896, 6295);
	TestExample("D:/meshPathFind/cube_17_tetra.vtk", 8265, 239);
	TestExample("D:/meshPathFind/cube_17_tetra.vtk", 15935, 19648);
	TestExample("D:/meshPathFind/cuboid_10x1000_auto.vtk", 84, 134308);
	TestExample("D:/meshPathFind/cuboid_10x1000_auto.vtk", 84, 279);
	TestExample("D:/meshPathFind/gear.vtk", 669, 6459);
	return 0;
}


void WriteFile(vtkSmartPointer<vtkUnstructuredGrid> grid, const char* path)
{
	// Write file
	vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
		vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
	writer->SetFileName(path);
#if VTK_MAJOR_VERSION <= 5
	writer->SetInput(grid);
#else
	writer->SetInputData(grid);
#endif
	writer->Write();
}

vtkSmartPointer<vtkUnstructuredGrid> ReadFile(const char* path)
{
	vtkSmartPointer<vtkUnstructuredGridReader> reader =
		vtkSmartPointer<vtkUnstructuredGridReader>::New();
	reader->SetFileName(path);
	reader->Update();
	return reader->GetOutput();
}

vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedronGrid()
{
	vtkSmartPointer<vtkUnstructuredGrid> grid = vtkUnstructuredGrid::New();
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
	grid->SetPoints(startPoints);

	vtkSmartPointer<vtkIdList> sCell0 = vtkSmartPointer<vtkIdList>::New();
	sCell0->InsertNextId(0);
	sCell0->InsertNextId(4);
	sCell0->InsertNextId(5);
	sCell0->InsertNextId(1);
	sCell0->InsertNextId(12);
	sCell0->InsertNextId(16);
	sCell0->InsertNextId(17);
	sCell0->InsertNextId(13);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell0);

	vtkSmartPointer<vtkIdList> sCell1 = vtkSmartPointer<vtkIdList>::New();
	sCell1->InsertNextId(1);
	sCell1->InsertNextId(5);
	sCell1->InsertNextId(6);
	sCell1->InsertNextId(2);
	sCell1->InsertNextId(13);
	sCell1->InsertNextId(17);
	sCell1->InsertNextId(18);
	sCell1->InsertNextId(14);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell1);

	vtkSmartPointer<vtkIdList> sCell2 = vtkSmartPointer<vtkIdList>::New();
	sCell2->InsertNextId(2);
	sCell2->InsertNextId(6);
	sCell2->InsertNextId(7);
	sCell2->InsertNextId(3);
	sCell2->InsertNextId(14);
	sCell2->InsertNextId(18);
	sCell2->InsertNextId(19);
	sCell2->InsertNextId(15);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell2);

	vtkSmartPointer<vtkIdList> sCell3 = vtkSmartPointer<vtkIdList>::New();
	sCell3->InsertNextId(6);
	sCell3->InsertNextId(10);
	sCell3->InsertNextId(11);
	sCell3->InsertNextId(7);
	sCell3->InsertNextId(18);
	sCell3->InsertNextId(22);
	sCell3->InsertNextId(23);
	sCell3->InsertNextId(19);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell3);

	vtkSmartPointer<vtkIdList> sCell4 = vtkSmartPointer<vtkIdList>::New();
	sCell4->InsertNextId(5);
	sCell4->InsertNextId(9);
	sCell4->InsertNextId(10);
	sCell4->InsertNextId(6);
	sCell4->InsertNextId(17);
	sCell4->InsertNextId(21);
	sCell4->InsertNextId(22);
	sCell4->InsertNextId(18);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell4);

	vtkSmartPointer<vtkIdList> sCell5 = vtkSmartPointer<vtkIdList>::New();
	sCell5->InsertNextId(4);
	sCell5->InsertNextId(8);
	sCell5->InsertNextId(9);
	sCell5->InsertNextId(5);
	sCell5->InsertNextId(16);
	sCell5->InsertNextId(20);
	sCell5->InsertNextId(21);
	sCell5->InsertNextId(17);
	grid->InsertNextCell(VTK_HEXAHEDRON, sCell5);
	return grid;
}

void Test()
{
	vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/meshPathFind/cube_17_tetra.vtk");
	std::cout << grid->GetNumberOfPoints() << std::endl;
	Graph graph(grid);
	AStar star;
	BiDirectional bd;
	BFS bfs;

	std::cout << "gear.vtk" << std::endl;
	vtkIdType start = 8265;//28429
	vtkIdType end = 4319; //741-path exists	//471-path not exists
	std::cout << "BFS TEST" << std::endl;
	Performance(bfs, graph, start, end);

	std::cout << "BiDir TEST" << std::endl;
	Performance(bd, graph, start, end);

	std::cout << "A* TEST" << std::endl;
	Performance(star, graph, start, end);
}

void TestBFS()
{
	//vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/test3.vtu");
	vtkSmartPointer<vtkUnstructuredGrid> grid = MakeHexahedronGrid();
	Graph graph(grid);
	Solver s;
	vtkSmartPointer<vtkIdList> result = s.Solve(&graph, 0,23);
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
		std::cout << result->GetId(i) << ' ';
	std::cout << std::endl;
	std::cout << "length=" << result->GetNumberOfIds()<<std::endl;
}

void TestBiDir()
{
	//vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/test3.vtu");
	vtkSmartPointer<vtkUnstructuredGrid> grid = MakeHexahedronGrid();
	Graph graph(grid);
	Solver s;
	BiDirectional bd;
	s.SetStrategy(bd);
	vtkSmartPointer<vtkIdList> result = s.Solve(&graph, 0,23);
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
		std::cout << result->GetId(i) << ' ';
	std::cout << std::endl;
	std::cout << "length=" << result->GetNumberOfIds()<<std::endl;
}

void TestAstar()
{
	//vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/test3.vtu");
	vtkSmartPointer<vtkUnstructuredGrid> grid = MakeHexahedronGrid();
	Graph graph(grid);
	Solver s;
	AStar astar;
	s.SetStrategy(astar);
	vtkSmartPointer<vtkIdList> result = s.Solve(&graph, 0, 23);
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
		std::cout << result->GetId(i) << ' ';
	std::cout << std::endl;
	std::cout << "length=" << result->GetNumberOfIds()<<std::endl;
}

void Performance(PFStrategy &algorithm, Graph &g, vtkIdType start, vtkIdType finish)
{
	auto begin = std::chrono::steady_clock::now();
	vtkSmartPointer<vtkIdList> result1 = algorithm.Solve(&g, start, finish);
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "path length=" << result1->GetNumberOfIds() << std::endl;
	std::cout << "The time: " << elapsed_ms.count() << " ms" << std::endl;
	for (vtkIdType i = 0; i < result1->GetNumberOfIds(); i++)
		std::cout << result1->GetId(i) << ", ";
	std::cout << std::endl;
}

void TestExample(const char* str, vtkIdType start, vtkIdType end)
{
	vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile(str);
	Graph graph(grid);
	AStar star;
	BiDirectional bd;
	BFS bfs;

	std::string s = str;
	std::string fileName = s.substr(s.find_last_of('/') + 1);

	std::cout << "FILE NAME: " << fileName << std::endl;
	std::cout << "NUMBER OF POINTS: " << grid->GetNumberOfPoints() << std::endl;
	std::cout << "START POINT: " << start << std::endl;
	std::cout << "END POINT: " << end << std::endl;
	std::cout << std::endl;

	std::cout << "BFS TEST" << std::endl;
	Performance(bfs, graph, start, end);

	std::cout << "BiDir TEST" << std::endl;
	Performance(bd, graph, start, end);

	std::cout << "A* TEST" << std::endl;
	Performance(star, graph, start, end);
}