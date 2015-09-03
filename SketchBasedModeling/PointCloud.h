#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Face {
public:
	//std::vector<glm::vec3> points;
	std::vector<int> points;

public:
	Face() {}
	Face(int p1, int p2, int p3);
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
	void addTriangleEdge(glm::vec3& p1, glm::vec3& p2);
	bool hasEdge(int p1, int p2);
	bool snapPoint(glm::vec3& point, float threshold, int& index);
	bool snapPoint(glm::vec2& point, const glm::mat4& mvpMatrix, float threshold, glm::vec3& point3d, int& index);
	bool isFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2);
	void addFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2, std::vector<std::pair<int, int> >& new_edges);
	bool hitFace(const glm::vec3& p, const glm::vec3& v, const glm::mat4& mvpMatrix, int& index);
	static void align(const glm::vec3& p1, glm::vec3& p2);
	void generate(RenderManager* renderManager);
};