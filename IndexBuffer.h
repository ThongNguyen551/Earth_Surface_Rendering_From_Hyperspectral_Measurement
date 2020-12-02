#pragma once
#include<GL/glew.h>
class IndexBuffer
{
private:
	GLuint RendererID;
	GLuint nIndexes;
public:
	IndexBuffer(const GLuint* Idata, GLuint nelem);
	~IndexBuffer();

	void Attach();
	void Dettach();
};