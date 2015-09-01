#include "GLWidget3D.h"
#include <iostream>
#include "GLUtils.h"

//Face::Face(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4) {
Face::Face(int p1, int p2, int p3, int p4) {
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);
}

bool Face::contain(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2) {
	if (std::find(points.begin(), points.end(), edge1.first) == points.end()) return false;
	if (std::find(points.begin(), points.end(), edge1.second) == points.end()) return false;
	if (std::find(points.begin(), points.end(), edge2.first) == points.end()) return false;
	if (std::find(points.begin(), points.end(), edge2.second) == points.end()) return false;
	return true;
}

void PointList::addEdge(glm::vec3& p1, glm::vec3& p2) {
	int e1, e2;
	if (!snapPoint(p1, 2.0f, e1)) {
		e1 = points.size();
		points.push_back(p1);
	}
	align(p1, p2);

	if (!snapPoint(p2, 2.0f, e2)) {
		e2 = points.size();
		points.push_back(p2);
	}

	new_edges.push_back(std::make_pair(e1, e2));

	// if two edges are perpendicular and have a common vertex, then create a face!
	for (int k = 0; k < new_edges.size(); ++k) {
		for (int i = 0; i < edges.size(); ++i) {	
			if (glm::dot(points[new_edges[k].first] - points[new_edges[k].second], points[edges[i].first] - points[edges[i].second]) < 0.1f) {
				if (edges[i].first == new_edges[k].first
					|| edges[i].first == new_edges[k].second
					|| edges[i].second == new_edges[k].first
					|| edges[i].second == new_edges[k].second) {
					if (!isFace(edges[i], new_edges[k])) {
						addFace(edges[i], new_edges[k], new_edges);
					}
				}
			}
		}
		edges.push_back(new_edges[k]);
	}

	//edges.insert(edges.end(), new_edges.begin(), new_edges.end());
	new_edges.clear();
}

bool PointList::snapPoint(glm::vec3& point, float threshold, int& index) {
	float min_dist = (std::numeric_limits<float>::max)();

	for (int i = 0; i < points.size(); ++i) {
		float dist = glm::length(points[i] - point);
		if (dist < min_dist) {
			min_dist = dist;
			index = i;
		}
	}

	if (min_dist < threshold) {
		return true;
	} else {
		return false;
	}
}

bool PointList::isFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2) {
	for (int i = 0; i < faces.size(); ++i) {
		if (faces[i].contain(edge1, edge2)) return true;
	}

	return false;
}

void PointList::addFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2, std::vector<std::pair<int, int> >& new_edges) {
	//glm::vec3 p1, p2, p3, p4;
	int p1, p2, p3, p4;

	if (edge1.first == edge2.first) {
		p1 = edge1.second;
		p2 = edge1.first;
		p3 = edge2.second;
		/*
		std::pair<int, int> new_e1(edge1.second, points.size());
		std::pair<int, int> new_e2(edge2.second, points.size());

		new_edges.push_back(new_e1);
		new_edges.push_back(new_e2);
		*/
	} else if (edge1.first == edge2.second) {
		p1 = edge1.second;
		p2 = edge1.first;
		p3 = edge2.first;
		/*
		std::pair<int, int> new_e1(edge1.second, points.size());
		std::pair<int, int> new_e2(edge2.first, points.size());

		new_edges.push_back(new_e1);
		new_edges.push_back(new_e2);
		*/
	} else if (edge1.second == edge2.first) {
		p1 = edge1.first;
		p2 = edge1.second;
		p3 = edge2.second;
		/*
		std::pair<int, int> new_e1(edge1.first, points.size());
		std::pair<int, int> new_e2(edge2.second, points.size());

		new_edges.push_back(new_e1);
		new_edges.push_back(new_e2);
		*/
	} else {
		p1 = edge1.first;
		p2 = edge1.second;
		p3 = edge2.first;
		/*
		std::pair<int, int> new_e1(edge1.first, points.size());
		std::pair<int, int> new_e2(edge2.first, points.size());

		new_edges.push_back(new_e1);
		new_edges.push_back(new_e2);
		*/
	}

	glm::vec3 new_pt = points[p1] + points[p3] - points[p2];
	if (!snapPoint(new_pt, 2.0f, p4)) {
		p4 = points.size();
		points.push_back(points[p1] + points[p3] - points[p2]);
	}

	if (std::find(edges.begin(), edges.end(), std::make_pair(p1, p4)) == edges.end()
		&& std::find(new_edges.begin(), new_edges.end(), std::make_pair(p1, p4)) == new_edges.end()) {
		new_edges.push_back(std::make_pair(p1, p4));
	}
	if (std::find(edges.begin(), edges.end(), std::make_pair(p3, p4)) == edges.end()
		&& std::find(new_edges.begin(), new_edges.end(), std::make_pair(p3, p4)) == new_edges.end()) {
		new_edges.push_back(std::make_pair(p3, p4));
	}

	faces.push_back(Face(p1, p2, p3, p4));
}

void PointList::align(const glm::vec3& p1, glm::vec3& p2) {
	if (fabs(p1.x - p2.x) < 2.0f) {
		p2.x = p1.x;
	}
	if (fabs(p1.y - p2.y) < 2.0f) {
		p2.y = p1.y;
	}
	if (fabs(p1.z - p2.z) < 2.0f) {
		p2.z = p1.z;
	}
}

void PointList::generate(RenderManager* renderManager) {
	renderManager->removeObject("face");

	std::vector<Vertex> vertices;
	for (int i = 0; i < faces.size(); ++i) {
		std::vector<glm::vec3> pts;
		for (int k = 0; k < faces[i].points.size(); ++k) {
			pts.push_back(points[faces[i].points[k]]);
		}
		glutils::drawPolygon(pts, glm::vec3(1, 1, 1), glm::mat4(), vertices);
	}
	renderManager->addObject("face", "", vertices);
}

GLWidget3D::GLWidget3D(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	setAutoFillBackground(false);

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-0.1, -0.2, -1));

	// シャドウマップ用のmodel/view/projection行列を作成
	glm::mat4 light_pMatrix = glm::ortho<float>(-200, 200, -200, 200, 0.1, 5000);
	glm::mat4 light_mvMatrix = glm::lookAt(-light_dir * 200.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	light_mvpMatrix = light_pMatrix * light_mvMatrix;

	// initialize keys
	ctrlPressed = false;
}

void GLWidget3D::keyPressEvent(QKeyEvent *e) {
	ctrlPressed = false;

	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = true;
		break;
	default:
		break;
	}
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* e) {
	ctrlPressed = false;
}

/**
 * Draw the scene.
 */
void GLWidget3D::drawScene(int drawMode) {
	if (drawMode == 0) {
		glUniform1i(glGetUniformLocation(renderManager.program,"shadowState"), 1);
	} else {
		glUniform1i(glGetUniformLocation(renderManager.program,"shadowState"), 2);
	}

	//renderManager.renderAll(true);
	renderManager.render("axis");
	renderManager.render("grid");
	renderManager.render("face", true);
}

void GLWidget3D::drawLineTo(const QPoint &endPoint) {
	currentStroke->points.push_back(glm::vec2(endPoint.x(), endPoint.y()));

	lastPoint = endPoint;
}

void GLWidget3D::resizeSketch(int width, int height) {
/*	for (int i = 0; i < parametriclsystem::NUM_LAYERS; ++i) {
		QImage newImage(width, height, QImage::Format_ARGB32);
		newImage.fill(qRgba(255, 255, 255, 0));
		QPainter painter(&newImage);

		QImage img = sketch[i].scaled(width, height);
		painter.drawImage(0, 0, img);
		sketch[i] = newImage;
	}*/
}

void GLWidget3D::compute3dCoordinates(Stroke* stroke) {
	if (fabs(stroke->points[0].x - stroke->points.back().x) < 20) {
		// vertical line
		if (stroke->points[0].y > stroke->points.back().y) { // stroke->points[0]が地面にある
			int e1, e2;
			glm::vec3 p1 = unproject(stroke->points[0]);
			points.snapPoint(p1, 2.0f, e1);
			glm::vec3 p2 = unproject(stroke->points.back(), p1);
			points.addEdge(p1, p2);
		} else { // stroke->points.back()が地面にある
			int e1, e2;
			glm::vec3 p2 = unproject(stroke->points.back());
			points.snapPoint(p2, 2.0f, e2);
			glm::vec3 p1 = unproject(stroke->points[0], p2);
			points.snapPoint(p1, 2.0f, e1);
			points.addEdge(p1, p2);
		}
	} else { // horizontal line
		int e1, e2;
		glm::vec3 p1 = unproject(stroke->points[0]);
		glm::vec3 p2 = unproject(stroke->points.back());
		//PointList::align(p1, p2);
		points.addEdge(p1, p2);
	}

	points.generate(&renderManager);
}

glm::vec3 GLWidget3D::unproject(const glm::vec2& point) {
	glm::vec3 cameraPos = camera.cameraPosInWorld();
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * camera.aspect(), (height() * 0.5f - point.y) * 2.0f / height(), -camera.f());
	dir = glm::vec3(glm::inverse(camera.mvMatrix) * glm::vec4(dir, 0));

	//dir = glm::normalize(dir);
	glm::vec3 intPt = glutils::rayTriangleIntersection(cameraPos, dir, glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	intPt.y = 0;
	return intPt;
}

glm::vec3 GLWidget3D::unproject(const glm::vec2& point, const glm::vec3& verticalRefPt) {
	glm::vec3 cameraPos = camera.cameraPosInWorld();
	glm::vec3 dir((point.x - width() * 0.5f) * 2.0f / width() * camera.aspect(), (height() * 0.5f - point.y) * 2.0f / height(), -camera.f());
	dir = glm::vec3(glm::inverse(camera.mvMatrix) * glm::vec4(dir, 0));

	float t1 = (verticalRefPt.x - cameraPos.x) / dir.x;
	float t2 = (verticalRefPt.z - cameraPos.z) / dir.z;
	float t = (t1 + t2) * 0.5f;

	return glm::vec3(verticalRefPt.x, cameraPos.y + dir.y * t, verticalRefPt.z);
}


void GLWidget3D::resizeGL(int width, int height) {
	// sketch imageを更新
	resizeSketch(width, height);

	// OpenGLの設定を更新
	height = height ? height : 1;
	glViewport(0, 0, width, height);
	camera.updatePMatrix(width, height);
}

void GLWidget3D::mousePressEvent(QMouseEvent *e) {
	if (ctrlPressed) { // move camera
		camera.mousePress(e->x(), e->y());
	} else { // sketch
		lastPoint = e->pos();
		currentStroke = new Stroke(glm::vec2(e->x(), e->y()));
	}

	update();
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *e) {
	if (currentStroke != NULL) {
		// compute 3d coordinates of user stroke
		compute3dCoordinates(currentStroke);

		strokes.push_back(*currentStroke);
		delete currentStroke;
		currentStroke = NULL;
	}

	update();
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *e) {
	if (ctrlPressed) {
		if (e->buttons() & Qt::LeftButton) { // Rotate
			camera.rotate(e->x(), e->y());
		} else if (e->buttons() & Qt::MidButton) { // Move
			camera.move(e->x(), e->y());
		} else if (e->buttons() & Qt::RightButton) { // Zoom
			camera.zoom(e->x(), e->y());
		}
	} else { // sketch
		drawLineTo(e->pos());
	}

	update();
}

void GLWidget3D::initializeGL() {
	renderManager.init("../shaders/vertex.glsl", "../shaders/geometry.glsl", "../shaders/fragment.glsl", 4096);

	// 光源位置をセット
	// ShadowMappingは平行光源を使っている。この位置から原点方向を平行光源の方向とする。
	light_dir = glm::normalize(glm::vec3(-0.1, -0.2, -1));

	// set the clear color for the screen
	qglClearColor(QColor(224, 224, 224));

	std::vector<Vertex> vertices;
	glutils::drawAxes(1, 20, glm::mat4(), vertices);
	renderManager.addObject("axis", "", vertices);
	vertices.clear();
	glutils::drawGrid(200, 200, 10, glm::vec3(0.8, 0.8, 0.8), glm::vec3(1, 1, 1), glm::rotate(glm::mat4(), (float)(M_PI * 0.5f), glm::vec3(1, 0, 0)), vertices);
	renderManager.addObject("grid", "", vertices);

	currentStroke = NULL;
}

void GLWidget3D::paintEvent(QPaintEvent *event) {
	// OpenGLで描画
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(renderManager.program);
	renderManager.updateShadowMap(this, light_dir, light_mvpMatrix);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// Model view projection行列をシェーダに渡す
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvpMatrix"),  1, GL_FALSE, &camera.mvpMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(renderManager.program, "mvMatrix"),  1, GL_FALSE, &camera.mvMatrix[0][0]);

	// pass the light direction to the shader
	glUniform1fv(glGetUniformLocation(renderManager.program, "lightDir"), 3, &light_dir[0]);
	
	drawScene(0);

	// OpenGLの設定を元に戻す
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// QPainterで描画
	QPainter painter(this);
	painter.setPen(QPen(QColor(0, 0, 0)));
	painter.setOpacity(0.5);
	for (int i = 0; i < strokes.size(); ++i) {
		for (int k = 0; k < strokes[i].points.size() - 1; ++k) {
			painter.drawLine(strokes[i].points[k].x, strokes[i].points[k].y, strokes[i].points[k+1].x, strokes[i].points[k+1].y);
		}
	}
	if (currentStroke != NULL) {
		for (int k = 0; k < currentStroke->points.size() - 1; ++k) {
			painter.drawLine(currentStroke->points[k].x, currentStroke->points[k].y, currentStroke->points[k+1].x, currentStroke->points[k+1].y);
		}
	}
	painter.end();

	glEnable(GL_DEPTH_TEST);
}
