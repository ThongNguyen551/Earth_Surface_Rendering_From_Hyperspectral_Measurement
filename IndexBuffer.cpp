#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* Idata, GLuint nelem)
{
    nIndexes = nelem;
    glGenBuffers(1, &RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndexes * sizeof(GLuint), Idata, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &RendererID);
}

void IndexBuffer::Attach()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
}

void IndexBuffer::Dettach()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
