#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3DMesh
{

    typedef struct track_components {
        std::vector<glm::vec3> outerPoints;
        std::vector<glm::vec3> innerPoints;
        std::vector<glm::vec3> medianPoints;
        Mesh* mesh;
    }track_comp;

    class Track {
        public:
            track_comp track;
    };

    // Create square with given bottom left corner, length and color
    Mesh* Create3DSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);
    Track Create3DTrack(const std::string name, glm::vec3 color);
    Mesh* Create3DTree(const std::string name);
    Mesh* Create3DStartLine(const std::string name, Track track);
    Mesh* Create3DDiscLine(const std::string name);
    Mesh* Create3DCar(const std::string name, glm::vec3 color);
    Mesh* Create3DMyCar(const std::string name, glm::vec3 color);
}
