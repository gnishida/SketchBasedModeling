#pragma once

#include "glew.h"
#include <vector>
#include <QMap>
#include <QString>
#include "Vertex.h"

class GeometryObject {
public:
	GLuint vao;
	GLuint vbo;
	std::vector<Vertex> vertices;
	bool vaoCreated;
	bool vaoOutdated;

public:
	GeometryObject();
	GeometryObject(const std::vector<Vertex>& vertices);
	void addVertices(const std::vector<Vertex>& vertices);
	void createVAO();
};

class RenderManager {
public:
	GLuint program;
	QMap<QString, QMap<GLuint, GeometryObject> > objects;
	QMap<QString, GLuint> textures;

	GLuint fbo;
	GLuint texture;

public:
	RenderManager();

	void init(const std::string& vertex_file, const std::string& geometry_file, const std::string& fragment_file);
	void addObject(const QString& object_name, const QString& texture_file, const std::vector<Vertex>& vertices);
	void removeObjects();
	void removeObject(const QString& object_name);
	void renderAll(bool wireframe = false);
	void render(const QString& object_name, bool wireframe = false);


private:
	GLuint loadTexture(const QString& filename);
};

