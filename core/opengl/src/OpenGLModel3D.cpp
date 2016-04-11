/**
 * @class	OpenGLModel3D
 * @brief	OpengGL model 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "OpenGLModel3D.hpp"
#include "OpenGL.h"

bool OpenGLModel3D::init(const Model3D &model)
{
    uint32_t offset;

	/* Generate a vertex array to reference the attributes */
	GL( glGenVertexArrays(1, &_gVAO) );
	GL( glBindVertexArray(_gVAO) );
    {
        /* Generate a buffer model for the vertices positions */
        GL( glGenBuffers(1, &_vertexDataVBO) );
        GL( glBindBuffer(GL_ARRAY_BUFFER, _vertexDataVBO) );
        {
            /* Upload the data for this buffer */
            GL( glBufferData(GL_ARRAY_BUFFER,
                             model.getVertexData().size() * sizeof model.getVertexData()[0],
                             &(model.getVertexData()[0]),
                             GL_STATIC_DRAW) );

            /* First attribute contains the vertex coordinates */
            GL( glEnableVertexAttribArray(0) );
            GL( glVertexAttribPointer(
                    0,                             // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Model3D::VertexData),// stride
                    (void*)0                       // array buffer offset
                    ) );

            /* Second attibute contains the normals */
            offset = 12;
            GL( glEnableVertexAttribArray(1) );
            GL( glVertexAttribPointer(
                    1,                             // attribute
                    3,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Model3D::VertexData),  // stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
                    ) );

            /* Third attribute contains the UV coordinates */
            offset = 24;
            GL( glEnableVertexAttribArray(2) );
            GL( glVertexAttribPointer(
                    2,                             // attribute
                    2,                             // size
                    GL_FLOAT,                      // type
                    GL_FALSE,                      // normalized?
                    sizeof(Model3D::VertexData),// stride
                    reinterpret_cast<void*>(offset)                  // array buffer offset
                    ) );
        }

        /* Generate the buffer models for the indices */
        GL( glGenBuffers(1, &_indicesBO) );
        GL( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesBO) );
        {
            /* Upload the data */
            GL( glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             model.getIndexData().size() * sizeof(model.getIndexData()[0]),
                             &(model.getIndexData()[0]),
                             GL_STATIC_DRAW) );
        }
    }
    GL( glBindVertexArray(0) );

    /* TODO: Upload the textures */
    const std::vector< Texture > &textures = model.getTextures();

    _texturesIDs.resize(textures.size());
    GL( glGenTextures(textures.size(), &_texturesIDs[0]) );

    for(size_t i = 0; i < textures.size(); ++i) {
        /* TODO: Once we use our own format, this should not
         * be needed */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GL( glBindTexture(GL_TEXTURE_2D, _texturesIDs[i]) );
        {
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
            GL( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
            GL( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                             textures[i]._width,
                             textures[i]._height,
                             0, GL_RGB, GL_UNSIGNED_BYTE,
                             textures[i]._texture) );
        }

    }
    GL( glBindTexture(GL_TEXTURE_2D, 0) );

    _materials = model.getMaterials();
    _indicesOffsets = model.getIndicesOffsets();
    _indicesCount = model.getIndicesCount();

    return true;
}

bool OpenGLModel3D::destroy()
{
	GL( glDeleteBuffers(1, &_vertexDataVBO) );
	GL( glDeleteVertexArrays(1, &_gVAO) );
    return true;
}