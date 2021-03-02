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


vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron();
vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedronGrid();
vtkSmartPointer<vtkUnstructuredGrid> MakePolyhedron();
vtkSmartPointer<vtkUnstructuredGrid> ReadFile(const char* path);
void TestOnPolyhedron();
void TryToReadFile();
void WriteFile(vtkSmartPointer<vtkUnstructuredGrid> grid, const char* path);
void PriorityQueueTest();
void Test();

class Node
{
private:
	vtkIdType id;
	Node* prev;
	double cost;
	double priority;
public:
	Node(vtkIdType _id, Node *_prev = NULL, double _cost = 0, double _priority = std::numeric_limits<double>::max())
	{
		id = _id;
		prev = _prev;
		cost = _cost;
		priority = _priority;
	}
	void PrintSelf()
	{
		std::cout << "id=" << id << " priority=" << priority << std::endl;
	}
	double GetPriority() const
	{
		return priority;
	}
};

int main(int argc, char **argv)
{
	//	vtkSmartPointer<vtkUnstructuredGrid> start = vtkUnstructuredGrid::New();
	//	vtkSmartPointer<vtkPoints> startPoints = vtkSmartPointer<vtkPoints>::New();
	//	startPoints->InsertNextPoint(0, 0, 0); //0
	//	startPoints->InsertNextPoint(0, 10, 0);//1
	//	startPoints->InsertNextPoint(0, 20, 0);//2
	//	startPoints->InsertNextPoint(0, 30, 0);//3
	//	startPoints->InsertNextPoint(10, 0, 0);//4
	//	startPoints->InsertNextPoint(10, 10, 0);//5
	//	startPoints->InsertNextPoint(10, 20, 0);//6
	//	startPoints->InsertNextPoint(10, 30, 0);//7
	//	startPoints->InsertNextPoint(20, 0, 0); //8
	//	startPoints->InsertNextPoint(20, 10, 0);//9
	//	startPoints->InsertNextPoint(20, 20, 0);//10
	//	startPoints->InsertNextPoint(20, 30, 0); //11 'Это точки для нижней части сетки (start)
	//
	//	startPoints->InsertNextPoint(0, 0, 50);//12
	//	startPoints->InsertNextPoint(0, 10, 50);//13
	//	startPoints->InsertNextPoint(0, 20, 50);//14
	//	startPoints->InsertNextPoint(0, 30, 50); //15
	//	startPoints->InsertNextPoint(10, 0, 50);//16
	//	startPoints->InsertNextPoint(10, 10, 50);//17
	//	startPoints->InsertNextPoint(10, 20, 50);//18
	//	startPoints->InsertNextPoint(10, 30, 50);//19
	//	startPoints->InsertNextPoint(20, 0, 50);//20
	//	startPoints->InsertNextPoint(20, 10, 50);//21
	//	startPoints->InsertNextPoint(20, 20, 50);//22
	//	startPoints->InsertNextPoint(20, 30, 50); //23 'Это точки для верхней части сетки (finish)
	//	start->SetPoints(startPoints);
	//	vtkSmartPointer<vtkIdList> sCell0 = vtkSmartPointer<vtkIdList>::New();
	//	sCell0->InsertNextId(0);
	//	sCell0->InsertNextId(1);
	//	sCell0->InsertNextId(5);
	//	sCell0->InsertNextId(4);
	//	start->InsertNextCell(VTK_QUAD, sCell0);
	//	vtkSmartPointer<vtkIdList> sCell1 = vtkSmartPointer<vtkIdList>::New();
	//	sCell1->InsertNextId(1);
	//	sCell1->InsertNextId(2);
	//	sCell1->InsertNextId(6);
	//	sCell1->InsertNextId(5);
	//	start->InsertNextCell(VTK_QUAD, sCell1);
	//	vtkSmartPointer<vtkIdList> sCell2 = vtkSmartPointer<vtkIdList>::New();
	//	sCell2->InsertNextId(2);
	//	sCell2->InsertNextId(3);
	//	sCell2->InsertNextId(7);
	//	sCell2->InsertNextId(6);
	//	start->InsertNextCell(VTK_QUAD, sCell2);
	//	vtkSmartPointer<vtkIdList> sCell3 = vtkSmartPointer<vtkIdList>::New();
	//	sCell3->InsertNextId(6);
	//	sCell3->InsertNextId(7);
	//	sCell3->InsertNextId(11);
	//	sCell3->InsertNextId(10);
	//	start->InsertNextCell(VTK_QUAD, sCell3);
	//	vtkSmartPointer<vtkIdList> sCell4 = vtkSmartPointer<vtkIdList>::New();
	//	sCell4->InsertNextId(5);
	//	sCell4->InsertNextId(6);
	//	sCell4->InsertNextId(10);
	//	sCell4->InsertNextId(9);
	//	start->InsertNextCell(VTK_QUAD, sCell4);
	//	vtkSmartPointer<vtkIdList> sCell5 = vtkSmartPointer<vtkIdList>::New();
	//	sCell5->InsertNextId(4);
	//	sCell5->InsertNextId(5);
	//	sCell5->InsertNextId(9);
	//	sCell5->InsertNextId(8);
	//	start->InsertNextCell(VTK_QUAD, sCell5);
	//
	//	vtkSmartPointer<vtkIdList> sCell6 = vtkSmartPointer<vtkIdList>::New();
	//	sCell6->InsertNextId(12);
	//	sCell6->InsertNextId(13);
	//	sCell6->InsertNextId(17);
	//	sCell6->InsertNextId(16);
	//	start->InsertNextCell(VTK_QUAD, sCell6);
	//	vtkSmartPointer<vtkIdList> sCell7 = vtkSmartPointer<vtkIdList>::New();
	//	sCell7->InsertNextId(13);
	//	sCell7->InsertNextId(14);
	//	sCell7->InsertNextId(18);
	//	sCell7->InsertNextId(17);
	//	start->InsertNextCell(VTK_QUAD, sCell7);
	//	vtkSmartPointer<vtkIdList> sCell8 = vtkSmartPointer<vtkIdList>::New();
	//	sCell8->InsertNextId(14);
	//	sCell8->InsertNextId(15);
	//	sCell8->InsertNextId(19);
	//	sCell8->InsertNextId(18);
	//	start->InsertNextCell(VTK_QUAD, sCell8);
	//	vtkSmartPointer<vtkIdList> sCell9 = vtkSmartPointer<vtkIdList>::New();
	//	sCell9->InsertNextId(18);
	//	sCell9->InsertNextId(19);
	//	sCell9->InsertNextId(23);
	//	sCell9->InsertNextId(22);
	//	start->InsertNextCell(VTK_QUAD, sCell9);
	//	vtkSmartPointer<vtkIdList> sCell10 = vtkSmartPointer<vtkIdList>::New();
	//	sCell10->InsertNextId(17);
	//	sCell10->InsertNextId(18);
	//	sCell10->InsertNextId(22);
	//	sCell10->InsertNextId(21);
	//	start->InsertNextCell(VTK_QUAD, sCell10);
	//	vtkSmartPointer<vtkIdList> sCell11 = vtkSmartPointer<vtkIdList>::New();
	//	sCell11->InsertNextId(16);
	//	sCell11->InsertNextId(17);
	//	sCell11->InsertNextId(21);
	//	sCell11->InsertNextId(20);
	//	start->InsertNextCell(VTK_QUAD, sCell11);
	//
	//	vtkSmartPointer<vtkIdList> sCell12 = vtkSmartPointer<vtkIdList>::New();
	//	sCell12->InsertNextId(20);
	//	sCell12->InsertNextId(21);
	//	sCell12->InsertNextId(9);
	//	sCell12->InsertNextId(8);
	//	start->InsertNextCell(VTK_QUAD, sCell12);
	//
	//	vtkSmartPointer<vtkIdList> sCell13 = vtkSmartPointer<vtkIdList>::New();
	//	sCell13->InsertNextId(21);
	//	sCell13->InsertNextId(22);
	//	sCell13->InsertNextId(10);
	//	sCell13->InsertNextId(9);
	//	start->InsertNextCell(VTK_QUAD, sCell13);
	//
	//	vtkSmartPointer<vtkIdList> sCell14 = vtkSmartPointer<vtkIdList>::New();
	//	sCell14->InsertNextId(22);
	//	sCell14->InsertNextId(23);
	//	sCell14->InsertNextId(11);
	//	sCell14->InsertNextId(10);
	//	start->InsertNextCell(VTK_QUAD, sCell14);
	//
	//	vtkSmartPointer<vtkIdList> sCell15 = vtkSmartPointer<vtkIdList>::New();
	//	sCell15->InsertNextId(23);
	//	sCell15->InsertNextId(19);
	//	sCell15->InsertNextId(7);
	//	sCell15->InsertNextId(11);
	//	start->InsertNextCell(VTK_QUAD, sCell15);
	//
	//	vtkSmartPointer<vtkIdList> sCell16 = vtkSmartPointer<vtkIdList>::New();
	//	sCell16->InsertNextId(19);
	//	sCell16->InsertNextId(15);
	//	sCell16->InsertNextId(3);
	//	sCell16->InsertNextId(7);
	//	start->InsertNextCell(VTK_QUAD, sCell16);
	//
	//	vtkSmartPointer<vtkIdList> sCell17 = vtkSmartPointer<vtkIdList>::New();
	//	sCell17->InsertNextId(12);
	//	sCell17->InsertNextId(13);
	//	sCell17->InsertNextId(1);
	//	sCell17->InsertNextId(0);
	//	start->InsertNextCell(VTK_QUAD, sCell17);
	//
	//	vtkSmartPointer<vtkIdList> sCell18 = vtkSmartPointer<vtkIdList>::New();
	//	sCell18->InsertNextId(13);
	//	sCell18->InsertNextId(14);
	//	sCell18->InsertNextId(2);
	//	sCell18->InsertNextId(1);
	//	start->InsertNextCell(VTK_QUAD, sCell18);
	//
	//	vtkSmartPointer<vtkIdList> sCell19 = vtkSmartPointer<vtkIdList>::New();
	//	sCell19->InsertNextId(14);
	//	sCell19->InsertNextId(15);
	//	sCell19->InsertNextId(3);
	//	sCell19->InsertNextId(2);
	//	start->InsertNextCell(VTK_QUAD, sCell19);
	//
	//	vtkSmartPointer<vtkIdList> sCell20 = vtkSmartPointer<vtkIdList>::New();
	//	sCell20->InsertNextId(12);
	//	sCell20->InsertNextId(16);
	//	sCell20->InsertNextId(4);
	//	sCell20->InsertNextId(0);
	//	start->InsertNextCell(VTK_QUAD, sCell20);
	//
	//	vtkSmartPointer<vtkIdList> sCell21 = vtkSmartPointer<vtkIdList>::New();
	//	sCell21->InsertNextId(16);
	//	sCell21->InsertNextId(20);
	//	sCell21->InsertNextId(8);
	//	sCell21->InsertNextId(4);
	//	start->InsertNextCell(VTK_QUAD, sCell21);

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
		//Graph g(start);
		/*vtkSmartPointer<vtkIdList> neighbours = vtkSmartPointer<vtkIdList>::New();
		g.GetAdj(2, neighbours);
		std::cout << neighbours->GetNumberOfIds()<<std::endl;

		for (vtkIdType i = 0; i < neighbours->GetNumberOfIds(); i++)
		{
			std::cout << neighbours->GetId(i) << ' ';
		}*/
		//std::cout << std::endl;
		//std::cout << g.GetNumberOfPoints() << ' '<<std::endl;

	Test();
	return 0;
}


vtkSmartPointer<vtkUnstructuredGrid> MakeHexahedron()
{
	// A regular hexagon (cube) with all faces square and three squares around
	// each vertex is created below.

	// Setup the coordinates of eight points
	// (the two faces must be in counter clockwise
	// order as viewed from the outside).

	// As an exercise you can modify the coordinates of the points to create
	// seven topologically distinct convex hexahedras.

	int numberOfVertices = 8;

	// Create the points
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 1.0, 0.0);
	points->InsertNextPoint(0.0, 1.0, 0.0);
	points->InsertNextPoint(0.0, 0.0, 1.0);
	points->InsertNextPoint(1.0, 0.0, 1.0);
	points->InsertNextPoint(1.0, 1.0, 1.0);
	points->InsertNextPoint(0.0, 1.0, 1.0);

	// Create a hexahedron from the points
	vtkSmartPointer<vtkHexahedron> hex =
		vtkSmartPointer<vtkHexahedron>::New();
	for (int i = 0; i < numberOfVertices; ++i)
	{
		hex->GetPointIds()->SetId(i, i);
	}

	// Add the points and hexahedron to an unstructured grid
	vtkSmartPointer<vtkUnstructuredGrid> uGrid =
		vtkSmartPointer<vtkUnstructuredGrid>::New();
	uGrid->SetPoints(points);
	uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

	return uGrid;
}

vtkSmartPointer<vtkUnstructuredGrid> MakePolyhedron()
{

	// Make a regular dodecahedron. It consists of twelve regular pentagonal
	// faces with three faces meeting at each vertex.
	int numberOfVertices = 20;
	int numberOfFaces = 12;
	int numberOfFaceVertices = 5;

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(1.21412, 0, 1.58931);
	points->InsertNextPoint(0.375185, 1.1547, 1.58931);
	points->InsertNextPoint(-0.982247, 0.713644, 1.58931);
	points->InsertNextPoint(-0.982247, -0.713644, 1.58931);
	points->InsertNextPoint(0.375185, -1.1547, 1.58931);
	points->InsertNextPoint(1.96449, 0, 0.375185);
	points->InsertNextPoint(0.607062, 1.86835, 0.375185);
	points->InsertNextPoint(-1.58931, 1.1547, 0.375185);
	points->InsertNextPoint(-1.58931, -1.1547, 0.375185);
	points->InsertNextPoint(0.607062, -1.86835, 0.375185);
	points->InsertNextPoint(1.58931, 1.1547, -0.375185);
	points->InsertNextPoint(-0.607062, 1.86835, -0.375185);
	points->InsertNextPoint(-1.96449, 0, -0.375185);
	points->InsertNextPoint(-0.607062, -1.86835, -0.375185);
	points->InsertNextPoint(1.58931, -1.1547, -0.375185);
	points->InsertNextPoint(0.982247, 0.713644, -1.58931);
	points->InsertNextPoint(-0.375185, 1.1547, -1.58931);
	points->InsertNextPoint(-1.21412, 0, -1.58931);
	points->InsertNextPoint(-0.375185, -1.1547, -1.58931);
	points->InsertNextPoint(0.982247, -0.713644, -1.58931);

	vtkIdType dodechedronPointsIds[20] =
	{ 0,   1,  2,  3,  4,  5,  6,  7,  8,  9,
	 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };


	vtkIdType dodechedronFace[12][5] = {
		{0, 1, 2, 3, 4},
		{0, 5, 10, 6, 1},
		{1, 6, 11, 7, 2},
		{2, 7, 12, 8, 3},
		{3, 8, 13, 9, 4},
		{4, 9, 14, 5, 0},
		{15, 10, 5, 14, 19},
		{16, 11, 6, 10, 15},
		{17, 12, 7, 11, 16},
		{18, 13, 8, 12, 17},
		{19, 14, 9, 13, 18},
		{19, 18, 17, 16, 15}
	};

	vtkSmartPointer<vtkCellArray> dodechedronFaces =
		vtkSmartPointer<vtkCellArray>::New();
	for (int i = 0; i < numberOfFaces; i++)
	{
		dodechedronFaces->InsertNextCell(numberOfFaceVertices, dodechedronFace[i]);
	}

	vtkSmartPointer<vtkUnstructuredGrid> uGrid =
		vtkSmartPointer<vtkUnstructuredGrid>::New();
	uGrid->InsertNextCell(VTK_POLYHEDRON,
		numberOfVertices, dodechedronPointsIds,
		numberOfFaces, dodechedronFaces->GetPointer());
	uGrid->SetPoints(points);

	return uGrid;
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

void TryToReadFile()
{
	const char* path = "D:/test2.vtk";
	vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile(path);
	std::cout << grid->GetNumberOfPoints();
}

void TestOnPolyhedron()
{
	vtkSmartPointer<vtkUnstructuredGrid> grid = MakePolyhedron();
	Graph g(grid);
	Solver solver;
	BiDirectional bd;
	solver.SetStrategy(bd);
	vtkSmartPointer<vtkIdList> result = solver.Solve(&g, 0, 18);
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
		std::cout << result->GetId(i) << ' ';
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
	WriteFile(grid, "D:/test3.vtu");
	return grid;
}

void Test()
{
	vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/test2.vtk");
	Graph graph(grid);
	Solver s;
	AStar star;
	BiDirectional bd;
	s.SetStrategy(star);
	vtkSmartPointer<vtkIdList> result = s.Solve(&graph, 28429, 471); //467
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
		std::cout << result->GetId(i) << ' ';
	return;
}

void PriorityQueueTest()
{
	auto cmp = [](const Node* n1, const Node* n2) {return (n1->GetPriority() > n2->GetPriority()); };
	std::priority_queue< Node*, std::vector<Node*>, decltype(cmp)> idq(cmp);
	idq.push(new Node(1, NULL, 4, 1));
	idq.push(new Node(2, NULL, 7, 8));
	idq.push(new Node(3, NULL, 2, 0));
	while (!idq.empty())
	{
		Node* n = idq.top();
		n->PrintSelf();
		idq.pop();
	}
	vtkSmartPointer<vtkUnstructuredGrid> grid = ReadFile("D:/test2.vtu");
	AStar star;
	Solver s;
	s.SetStrategy(star);
	Graph g(grid);
	vtkSmartPointer<vtkIdList> result = s.Solve(&g, 0, 23);
	for (vtkIdType i = 0; i < result->GetNumberOfIds(); i++)
	{
		std::cout << result->GetId(i) << ' ';
	}
}
