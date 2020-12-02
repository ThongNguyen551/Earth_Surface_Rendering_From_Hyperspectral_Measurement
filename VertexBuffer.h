#include <GL/glew.h>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "vertexstr.h"

class VertexBuffer
{
private:
	GLuint RendererID;
public:
	VertexBuffer(const std::vector<Vertex> Vdata, GLuint size);
	~VertexBuffer();

	void Attach();
	void Dettach();
};