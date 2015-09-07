#include "PointCloud.h"
#include "GLUtils.h"

Face::Face(int p1, int p2, int p3) {
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
}

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

void PointCloud::clear() {
	points.clear();
	edges.clear();
	new_edges.clear();
	faces.clear();
}

/**
 * 四角形の一辺となるエッジを追加する。
 * もし面が追加されたら、trueを返却する。
 */
bool PointCloud::addQuadEdge(glm::vec3& p1, glm::vec3& p2) {
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

	bool faceAdded = false;
	for (int k = 0; k < new_edges.size(); ++k) {
		for (int i = 0; i < edges.size(); ++i) {
			// 2つのエッジが直角なら、四角形のfaceを作成する。
			if (edges[i].first == new_edges[k].first
					|| edges[i].first == new_edges[k].second
					|| edges[i].second == new_edges[k].first
					|| edges[i].second == new_edges[k].second) {
				glm::vec3 v1 = glm::normalize(points[new_edges[k].first] - points[new_edges[k].second]);
				glm::vec3 v2 = glm::normalize(points[edges[i].first] - points[edges[i].second]);
				//if (fabs(glm::dot(v1, v2)) < 0.2f) {
					if (!isFace(edges[i], new_edges[k])) {
						addFace(edges[i], new_edges[k], new_edges);
						faceAdded = true;
						continue;
					}
				//}
			}
		}
		edges.push_back(new_edges[k]);
	}

	//edges.insert(edges.end(), new_edges.begin(), new_edges.end());
	new_edges.clear();

	return faceAdded;
}

bool PointCloud::addTriangleEdge(glm::vec3& p1, glm::vec3& p2) {
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

	bool faceAdded = false;
	for (int k = 0; k < new_edges.size(); ++k) {
		for (int i = 0; i < edges.size(); ++i) {
			// 2つのエッジが直角なら、四角形のfaceを作成する。
			if (edges[i].first == new_edges[k].first
					|| edges[i].first == new_edges[k].second
					|| edges[i].second == new_edges[k].first
					|| edges[i].second == new_edges[k].second) {
				glm::vec3 v1 = glm::normalize(points[new_edges[k].first] - points[new_edges[k].second]);
				glm::vec3 v2 = glm::normalize(points[edges[i].first] - points[edges[i].second]);
				if (fabs(glm::dot(v1, v2)) < 0.1f) {
					if (!isFace(edges[i], new_edges[k])) {
						addFace(edges[i], new_edges[k], new_edges);
						faceAdded = true;
						continue;
					}
				}
			}

			// 2つのエッジが1つの頂点を共有し、もう1つの頂点間にエッジが存在するなら、三角形ののfaceを作成する。
			if (new_edges[k].first == edges[i].first) {
				if (hasEdge(new_edges[k].second, edges[i].second)) {
					if (!isFace(new_edges[k], edges[i])) {
						faces.push_back(Face(new_edges[k].first, new_edges[k].second, edges[i].second));
						faceAdded = true;
					}
				}
			} else if (new_edges[k].first == edges[i].second) {
				if (hasEdge(new_edges[k].second, edges[i].first)) {
					if (!isFace(new_edges[k], edges[i])) {
						faces.push_back(Face(new_edges[k].first, new_edges[k].second, edges[i].first));
						faceAdded = true;
					}
				}
			} else if (new_edges[k].second == edges[i].first) {
				if (hasEdge(new_edges[k].first, edges[i].second)) {
					if (!isFace(new_edges[k], edges[i])) {
						faces.push_back(Face(new_edges[k].second, new_edges[k].first, edges[i].second));
						faceAdded = true;
					}
				}
			} else if (new_edges[k].second == edges[i].second) {
				if (hasEdge(new_edges[k].first, edges[i].first)) {
					if (!isFace(new_edges[k], edges[i])) {
						faces.push_back(Face(new_edges[k].second, new_edges[k].first, edges[i].first));
						faceAdded = true;
					}
				}
			}
		}
		edges.push_back(new_edges[k]);
	}

	//edges.insert(edges.end(), new_edges.begin(), new_edges.end());
	new_edges.clear();

	return faceAdded;
}


bool PointCloud::hasEdge(int p1, int p2) {
	for (int i = 0; i < edges.size(); ++i) {
		if ((edges[i].first == p1 && edges[i].second == p2)
			|| (edges[i].second == p1 && edges[i].first == p2)) return true;
	}

	return false;
}

/**
 * 指定された点を、既存の点にsnapさせる。snapした場合には、trueを返却し、また、snap先の点のindexを返却する。
 *
 * @param point		指定された点の座標
 * @param threshold	しきい値
 * @param index		snap先の点のindex
 * @return			snapしたらtrueを返却する
 */
bool PointCloud::snapPoint(glm::vec3& point, float threshold, int& index) {
	float min_dist = (std::numeric_limits<float>::max)();

	for (int i = 0; i < points.size(); ++i) {
		float dist = glm::length(points[i] - point);
		if (dist < min_dist) {
			min_dist = dist;
			index = i;
		}
	}

	if (min_dist < threshold) {
		point = points[index];
		return true;
	} else {
		return false;
	}
}

bool PointCloud::snapPoint(glm::vec2& point, const glm::mat4& mvpMatrix, float threshold, glm::vec3& point3d, int& index) {
	float min_dist = (std::numeric_limits<float>::max)();
	for (int i = 0; i < points.size(); ++i) {
		glm::vec4 projected_pt = mvpMatrix * glm::vec4(points[i], 1);
		glm::vec2 p(projected_pt.x / projected_pt.w, projected_pt.y / projected_pt.w);

		float dist = glm::length(p - point);
		if (dist < min_dist) {
			min_dist = dist;
			index = i;
		}
	}

	if (min_dist < threshold) {
		point3d = points[index];
		return true;
	} else {
		return false;
	}
}

/**
 * 指定された2つのエッジを含むfaceが既に存在するかチェックする。
 */
bool PointCloud::isFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2) {
	for (int i = 0; i < faces.size(); ++i) {
		if (faces[i].contain(edge1, edge2)) return true;
	}

	return false;
}

void PointCloud::addFace(const std::pair<int, int>& edge1, const std::pair<int, int>& edge2, std::vector<std::pair<int, int> >& new_edges) {
	//glm::vec3 p1, p2, p3, p4;
	int p1, p2, p3, p4;

	if (edge1.first == edge2.first) {
		p1 = edge1.second;
		p2 = edge1.first;
		p3 = edge2.second;
	} else if (edge1.first == edge2.second) {
		p1 = edge1.second;
		p2 = edge1.first;
		p3 = edge2.first;
	} else if (edge1.second == edge2.first) {
		p1 = edge1.first;
		p2 = edge1.second;
		p3 = edge2.second;
	} else {
		p1 = edge1.first;
		p2 = edge1.second;
		p3 = edge2.first;
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

bool PointCloud::hitFace(const glm::vec3& p, const glm::vec3& v, const glm::mat4& mvpMatrix, int& index) {
	glm::vec3 intPt;
	float min_dist = (std::numeric_limits<float>::max)();
	index = -1;

	for (int i = 0; i < faces.size(); ++i) {
		for (int k = 1; k < faces[i].points.size() - 1; ++k) {
			if (glutils::rayTriangleIntersection(p, v, points[faces[i].points[0]], points[faces[i].points[k]], points[faces[i].points[k+1]], intPt)) {
				float dist = glm::length(intPt - p);
				if (dist < min_dist) {
					index = i;
				}
			}
		}
	}

	if (index >= 0) return true;
	else return false;
}

/**
 * 点p2を、点p1との相対位置がaxis alignedに近い場合は、axis alignedになるよう揃える。
 * さらに、点p2がgroundに近ければ、groundに揃える。
 */
void PointCloud::align(const glm::vec3& p1, glm::vec3& p2) {
	if (fabs(p1.x - p2.x) <= 5.0f) {
		p2.x = p1.x;
	}
	if (fabs(p1.y - p2.y) <= 5.0f) {
		p2.y = p1.y;
	}
	if (fabs(p1.z - p2.z) <= 5.0f) {
		p2.z = p1.z;
	}

	if (fabs(p2.y) <= 5.0f) p2.y = 0.0f;
}

void PointCloud::generate(RenderManager* renderManager) {
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
