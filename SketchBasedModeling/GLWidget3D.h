#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderManager.h"
#include <QPen>
#include <QGLWidget>
#include <QtGui>
#include "Camera.h"

class Stroke {
public:
	std::vector<glm::vec2> points;

public:
	Stroke() {}
	Stroke(const glm::vec2& point) { points.push_back(point); }
};

class Face {
public:
	//std::vector<glm::vec3> points;
	std::vector<int> points;

public:
	Face() {}
	//Face(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
	Face(int p1, int p2, int p3, int p4);

	bool contain(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2);
};

class PointList {
public:
	std::vector<glm::vec3> points;
	std::vector<std::pair<int, int> > edges;
	std::vector<std::pair<int, int> > new_edges;
	std::vector<Face> faces;

public:
	PointList() {}
	void addEdge(glm::vec3& p1, glm::vec3& p2);
	bool snapPoint( glm::vec3& point, float threshold, int& index);
	bool isFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2);
	void addFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2, std::vector<std::pair<int, int> >& new_edges);
	static void align(const glm::vec3& p1, glm::vec3& p2);
	void generate(RenderManager* renderManager);
};

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	static enum { MODE_SKETCH = 0, MODE_3DVIEW };

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	RenderManager renderManager;

	bool ctrlPressed;
	QPoint lastPoint;
	std::vector<Stroke> strokes;
	Stroke* currentStroke;
	PointList points;
	std::vector<std::pair<int, int> > edges;

public:
	GLWidget3D(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void drawScene(int drawMode);
	void drawLineTo(const QPoint &endPoint);
	void resizeSketch(int width, int height);
	void compute3dCoordinates(Stroke* stroke);
	glm::vec3 unproject(const glm::vec2& point);
	glm::vec3 unproject(const glm::vec2& point, const glm::vec3& verticalRefPt);

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void initializeGL();
	void paintEvent(QPaintEvent *e);
};

#endif