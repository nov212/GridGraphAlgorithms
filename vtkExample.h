#include <vtkPolyDataReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkIdTypeArray.h>
#include <vtkPoints.h>
#include <vtkPointSource.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkSmartPointer.h>

/***
 * \brief загрузка неструктурированной сетки
 */
vtkSmartPointer<vtkUnstructuredGrid> loadUnstructuredGrid(const char *fileName);

/***
* \brief загрузка структурированной сетки
*/
vtkSmartPointer<vtkPolyData> loadPolyData(const char *fileName);

/***
* \brief конвертация неструктурированной сетки в структурированную
*/
vtkSmartPointer<vtkPolyData> convertUnstructuredToPolyData(vtkSmartPointer<vtkUnstructuredGrid> mesh);

/***
 * \brief Пример создания сетки и записи ее в файл
 */
void createGridExample();

/***
 * \brief Пример работы с сеткой
 */
void meshUsageExample();