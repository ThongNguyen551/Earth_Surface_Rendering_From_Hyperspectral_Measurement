#include "VertexBuffer.h"
VertexBuffer::VertexBuffer(const std::vector<Vertex> Vdata, GLuint msize)
{
    glGenBuffers(1, &RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, RendererID);
    glBufferData(GL_ARRAY_BUFFER, msize, &Vdata[0], GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &RendererID);
}

void VertexBuffer::Attach()
{
    glBindBuffer(GL_ARRAY_BUFFER, RendererID);
}

void VertexBuffer::Dettach()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
