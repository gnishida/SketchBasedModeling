#include "MainWindow.h"
#include <QtGui/QApplication>
#include "GLUtils.h"
int main(int argc, char *argv[])
{
	glm::vec3 intPt;
	bool hit1 = glutils::rayTriangleIntersection(glm::vec3(100, 100, 100), glm::vec3(-1, -1, -1), glm::vec3(50, 0, -20), glm::vec3(50, 20, -20), glm::vec3(50, 20, 20), intPt);
	bool hit2 = glutils::rayTriangleIntersection(glm::vec3(100, 100, 100), glm::vec3(-1, -1, -1), glm::vec3(50, 0, -20), glm::vec3(50, 20, 20), glm::vec3(50, 0, 20), intPt);
	bool hit3 = glutils::rayTriangleIntersection(glm::vec3(100, 100, 100), glm::vec3(-1, -1, -1), glm::vec3(0, 0, -20), glm::vec3(0, 20, -20), glm::vec3(0, 20, 20), intPt);
	bool hit4 = glutils::rayTriangleIntersection(glm::vec3(100, 100, 100), glm::vec3(-1, -1, -1), glm::vec3(0, 0, -20), glm::vec3(0, 20, 20), glm::vec3(0, 0, 20), intPt);



	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
