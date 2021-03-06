/**
 * @class	Model3D
 * @brief	Basic model 3D representation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Model3D.hpp"
#include <glm/glm.hpp>
#include <limits>

void Model3D::_calculateBoundingVolumes()
{
    std::vector<Asset3D::VertexData>::const_iterator it;
    float maxLength = 0.0f;
    float minX = 0.0f, maxX = 0.0f;
    float minY = 0.0f, maxY = 0.0f;
    float minZ = 0.0f, maxZ = 0.0f;

    /* Loop through all the vertices and find the maximum radius and the
     * axis-aligned bounding box. At this moment the object-oriented bounding box
     * will be the same as the axis-aligned bound box. Once calculated the user can
     * call updateBoundingVolumes() to update this values accordingly using the model's
     * rotation.
     *
     * The loop assumes the center of mass of the object is (0.0f, 0.0f, 0.0f) in local
     * coordinates. If it is not the user must call normalize() prior to this function
     */
    for (it = _asset->getVertexData().begin(); it != _asset->getVertexData().end(); ++it) {
        /* Calculate maximum length */
        float length = glm::length(it->vertex);
        if (length > maxLength) {
            maxLength = length;
            _maxLengthVertex = it->vertex;
        }

        /* Calculate the maximum and minimum for each axis */
        if (it->vertex.x < minX) {
            minX = it->vertex.x;
        } else if (it->vertex.x > maxX) {
            maxX = it->vertex.x;
        }
        if (it->vertex.y < minY) {
            minY = it->vertex.y;
        } else if (it->vertex.y > maxY) {
            maxY = it->vertex.y;
        }
        if (it->vertex.z < minZ) {
            minZ = it->vertex.z;
        } else if (it->vertex.z > maxZ) {
            maxZ = it->vertex.z;
        }
    }

    /* Set the final values */
    _oobb.setMin(glm::vec3(minX, minY, minZ));
    _oobb.setMax(glm::vec3(maxX, maxY, maxZ));

    _updateBoundingVolumes();
}
