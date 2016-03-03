/**
 * @class	OpenGLUniformBlock
 * @brief	Manages the access to a uniform block in a shader. It takes care
 *          of finding out the block size and offsets, then allows to set the
 *          values in the block and bind it
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLUniformBlock.hpp"

OpenGLUniformBlock::OpenGLUniformBlock() : _linkedToShader(false),
                                           _blockIndex(0),
                                           _blockSize(0),
                                           _paramsBuffer(NULL),
                                           _uniformBufferObj(0)
{
}

OpenGLUniformBlock::~OpenGLUniformBlock()
{
    if (_linkedToShader) {
        glDeleteBuffers(1, &_uniformBufferObj);
        delete[] _paramsBuffer;
    }
}

void OpenGLUniformBlock::setBlockName(const std::string &name)
{
    _blockName = name;
}

void OpenGLUniformBlock::addParamName(const std::string &name)
{
    _paramsOffsets[name] = -1;
    _paramsFullName.push_back(_blockName + std::string(".") + name);
}

bool OpenGLUniformBlock::prepareForShader(GLuint programID)
{
    const GLchar *names[_paramsFullName.size()];
    GLuint indices[_paramsFullName.size()];
    GLint offsets[_paramsFullName.size()];
    std::map<std::string, GLint>::iterator it;
    int i;

    /* Prepare array of names pointers */
    for (i=0; i<_paramsFullName.size(); ++i) {
        names[i] = _paramsFullName[i].c_str();
    }

    GL( _blockIndex = glGetUniformBlockIndex(programID, _blockName.c_str()) );
    if (_blockIndex == GL_INVALID_INDEX) {
        return false;
    }
    GL( glGetActiveUniformBlockiv(programID, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize) );

    GL( glGetUniformIndices(programID, _paramsFullName.size(), names, indices) );
    for (i=0; i<_paramsOffsets.size(); ++i) {
        if (indices[i] == GL_INVALID_INDEX) {
            return false;
        }
    }

    /* Allocate the offsets and the data buffer */
    _paramsBuffer = new GLubyte[_blockSize];

    /* Retrieve the offsets */
    GL( glGetActiveUniformsiv(programID, _paramsFullName.size(), indices, GL_UNIFORM_OFFSET, offsets) );

    /* Associate them with their names */
    for (i=0; i<_paramsFullName.size(); ++i) {
        std::string baseName = _paramsFullName[i].substr(_blockName.length() + 1);
        _paramsOffsets[baseName] = offsets[i];
    }

    GL( glGenBuffers(1, &_uniformBufferObj) );
    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferObj) );
    GL( glBufferData(GL_UNIFORM_BUFFER, _blockSize, NULL, GL_DYNAMIC_DRAW) );
    GL( glBindBufferBase(GL_UNIFORM_BUFFER, _blockIndex, _uniformBufferObj) );

    _linkedToShader = true;

    return true;
}

void OpenGLUniformBlock::bind()
{
    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferObj) );
    GL( glBufferSubData(GL_UNIFORM_BUFFER, 0, _blockSize, _paramsBuffer) );
}