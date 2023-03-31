#include <vector>
#include <string.h>
#include <stdio.h>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/Tema2/object3DMesh.h"

using namespace glm;

Mesh* object3DMesh::Create3DDiscLine(std::string name) {
    glm::vec3 color = glm::vec3(1, 1, 1);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0.02, 0), color),
        VertexFormat(glm::vec3(0, 0.02, 1.2), color),
        VertexFormat(glm::vec3(0.25, 0.02, 1.2), color),
        VertexFormat(glm::vec3(0.25, 0.02, 0), color),
    };

    std::vector<unsigned int> indices = {0,1,2,0,2,3};
    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3DMesh::Create3DStartLine(std::string name, Track track) {
    glm::vec3 outer1, outer2, inner1, inner2, line_color;
    outer1 = track.track.outerPoints[0];
    outer2 = track.track.outerPoints[30];
    inner1 = track.track.innerPoints[0];
    inner2 = track.track.innerPoints[30];
    outer1.y += 0.02;
    outer2.y += 0.02;
    inner1.y += 0.02;
    outer2.y += 0.02;

    line_color = glm::vec3(0.8, 0.72, 0.72);
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(outer1, line_color),
        VertexFormat(inner1, line_color),
        VertexFormat(outer2, line_color),
        VertexFormat(inner2, line_color),
    };

    std::vector<unsigned int> indices = {0,1,3,0,3,2};
    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3DMesh::Create3DMyCar(std::string name, glm::vec3 color) {
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-0.4,0.4,0.4),color),
        VertexFormat(glm::vec3(-0.4,-0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,-0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,0.4,-0.4), color),
        VertexFormat(glm::vec3(0.4,-0.4,-0.4), color),
        VertexFormat(glm::vec3(-0.4,-0.4,-0.4), color),
        VertexFormat(glm::vec3(-0.4,0.4,-0.4), color),
    };
    std::vector<unsigned int> indices = { 0,1,2,0,2,3,3,2,5,3,5,4,4,5,6,4,6,7,7,6,1,7,1,0,7,0,3,7,3,4,6,1,2,6,2,5 };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3DMesh::Create3DCar(std::string name, glm::vec3 color) {
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-0.4,0.4,0.4),color),
        VertexFormat(glm::vec3(-0.4,-0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,-0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,0.4,0.4), color),
        VertexFormat(glm::vec3(0.4,0.4,-0.4), color),
        VertexFormat(glm::vec3(0.4,-0.4,-0.4), color),
        VertexFormat(glm::vec3(-0.4,-0.4,-0.4), color),
        VertexFormat(glm::vec3(-0.4,0.4,-0.4), color),
    };
    std::vector<unsigned int> indices = { 0,1,2,0,2,3,3,2,5,3,5,4,4,5,6,4,6,7,7,6,1,7,1,0,7,0,3,7,3,4,6,1,2,6,2,5 };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

Mesh* object3DMesh::Create3DTree(std::string name) {
    glm::vec3 body_color = glm::vec3(0.59, 0.31, 0.06);
    glm::vec3 leaves_color = glm::vec3(0.02, 0.3, 0.02);

    float b_height = 2.0;
    float b_width = 1.0;
    float l_height = 0.5;
    float l_width = 0.7;

    //Fac logica trunchiului copacului.
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0,b_height,0), body_color),
        VertexFormat(glm::vec3(0,0,0), body_color),
        VertexFormat(glm::vec3(b_width, 0, 0), body_color),
        VertexFormat(glm::vec3(b_width, b_height, 0), body_color),
        VertexFormat(glm::vec3(b_width, b_height, -b_width), body_color),
        VertexFormat(glm::vec3(b_width, 0, -b_width), body_color),
        VertexFormat(glm::vec3(0,0,-b_width), body_color),
        VertexFormat(glm::vec3(0,b_height,-b_width), body_color),
    };
    std::vector<unsigned int> indices = {0,1,2,0,2,3,3,2,5,3,5,4,4,5,6,4,6,7,7,6,1,7,1,0,7,0,3,7,3,4,6,1,2,6,2,5};

    //Fac logica frunzelor copacului.
    vertices.push_back(VertexFormat(glm::vec3(0,2,0.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1,2,0.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1,2,-1.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(0,2,-1.7), leaves_color));
    indices.push_back(11);
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(11);
    indices.push_back(9);
    indices.push_back(10);
    vertices.push_back(VertexFormat(glm::vec3(0, 2.3, -1.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1, 2.3, -1.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1, 2.3, 0.7), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(0, 2.3, 0.7), leaves_color));
    indices.push_back(12);
    indices.push_back(15);
    indices.push_back(14);
    indices.push_back(12);
    indices.push_back(14);
    indices.push_back(13);
    //++++++++++++++++++++
    indices.push_back(15);
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(15);
    indices.push_back(9);
    indices.push_back(14);
    //++++++++++++++++++++
    indices.push_back(14);
    indices.push_back(9);
    indices.push_back(10);
    indices.push_back(14);
    indices.push_back(10);
    indices.push_back(13);
    //++++++++++++++++++++
    indices.push_back(13);
    indices.push_back(10);
    indices.push_back(11);
    indices.push_back(13);
    indices.push_back(11);
    indices.push_back(12);
    //++++++++++++++++++++
    indices.push_back(12);
    indices.push_back(11);
    indices.push_back(8);
    indices.push_back(12);
    indices.push_back(8);
    indices.push_back(15);

    vertices.push_back(VertexFormat(glm::vec3(-0.7, 2, 0), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(-0.7, 2, -1), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1.7, 2, -1), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1.7, 2, 0), leaves_color));
    indices.push_back(18);
    indices.push_back(17);
    indices.push_back(16);
    indices.push_back(18);
    indices.push_back(16);
    indices.push_back(19);
    vertices.push_back(VertexFormat(glm::vec3(1.7, 2.5, 0), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(1.7, 2.5, -1), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(-0.7, 2.5, -1), leaves_color));
    vertices.push_back(VertexFormat(glm::vec3(-0.7, 2.5, 0), leaves_color));
    indices.push_back(23);
    indices.push_back(20);
    indices.push_back(21);
    indices.push_back(23);
    indices.push_back(21);
    indices.push_back(22);
    //++++++++++++++++++++
    indices.push_back(23);
    indices.push_back(16);
    indices.push_back(19);
    indices.push_back(23);
    indices.push_back(19);
    indices.push_back(20);
    //++++++++++++++++++++
    indices.push_back(20);
    indices.push_back(19);
    indices.push_back(18);
    indices.push_back(20);
    indices.push_back(18);
    indices.push_back(21);
    //++++++++++++++++++++
    indices.push_back(21);
    indices.push_back(18);
    indices.push_back(17);
    indices.push_back(21);
    indices.push_back(17);
    indices.push_back(22);
    //++++++++++++++++++++
    indices.push_back(22);
    indices.push_back(17);
    indices.push_back(16);
    indices.push_back(22);
    indices.push_back(16);
    indices.push_back(23);

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    return mesh;
}

object3DMesh::Track object3DMesh::Create3DTrack(std::string name, glm::vec3 color) 
{
    std::vector<glm::vec3> medianPoints = {
        glm::vec3(-60,0, 0),
        glm::vec3(- 58.27,0, 5.52),
        glm::vec3(- 54.91,0, 11.24),
        glm::vec3(- 50.87,0, 16.63),
        glm::vec3(- 46.83,0, 20.67),
        glm::vec3(- 41.44,0, 23.19),
        glm::vec3(- 35.38,0, 24.54),
        glm::vec3(- 29.33,0, 24.54),
        glm::vec3(- 24.61,0, 23.02),
        glm::vec3(- 21.25,0, 19.99),
        glm::vec3(- 18.89,0, 16.29),
        glm::vec3(- 17.21,0, 11.24),
        glm::vec3(- 15.86,0, 5.85),
        glm::vec3(- 15.19,0, 0),
        glm::vec3(- 14.85,0, -6.08),
        glm::vec3(- 16.87,0, -10.96),
        glm::vec3(- 21.42,0, -15.00),
        glm::vec3(- 27.47,0, -18.37),
        glm::vec3(- 33.03,0, -21.06),
        glm::vec3(- 39.26,0, -23.25),
        glm::vec3(- 44.81,0, -25.78),
        glm::vec3(- 50,0, -30),
        glm::vec3(- 53.56,0, -35.20),
        glm::vec3(- 54.23,0, -41.26),
        glm::vec3(- 52.38,0, -46.14),
        glm::vec3(- 47.84,0, -47.82),
        glm::vec3(- 42.28,0, -48.33),
        glm::vec3(- 37.07,0, -48.33),
        glm::vec3(- 31.35,0, -47.99),
        glm::vec3(- 25.94,0, -47.99),
        glm::vec3(- 20.74,0, -48.16),
        glm::vec3(- 16.20,0, -47.99),
        glm::vec3(-10.48,0, -47.65),
        glm::vec3(-3.91,0, -47.49),
        glm::vec3(2.47,0, -47.32),
        glm::vec3(9.71,0, -46.64),
        glm::vec3(15.43,0, -45.30),
        glm::vec3(20.82,0, -41.93),
        glm::vec3(24.69,0, -35.87),
        glm::vec3(27.89,0, -28.97),
        glm::vec3(30.75,0, -22.24),
        glm::vec3(33.27,0, -14.16),
        glm::vec3(34.45,0, -6.93),
        glm::vec3(34.95,0, 0),
        glm::vec3(34.62,0, 7.03),
        glm::vec3(33.44,0, 14.44),
        glm::vec3(30,0, 20),
        glm::vec3(23.85,0, 25.38),
        glm::vec3(16.27,0, 32.11),
        glm::vec3(8.53,0, 38.00),
        glm::vec3(3.08,0, 40.41),
        glm::vec3(-3.07,0, 42.38),
        glm::vec3(-9.35,0, 43.80),
        glm::vec3(-15.02,0, 44.56),
        glm::vec3(-21.41,0, 44.93),
        glm::vec3(-27.98,0, 45.07),
        glm::vec3(-33.85,0, 44.93),
        glm::vec3(-40.26,0, 44.73),
        glm::vec3(-44.79,0, 44.56),
        glm::vec3(-50.70,0, 43.55),
        glm::vec3(-55.72,0, 42.29),
        glm::vec3(-62.99,0, 39.51),
        glm::vec3(-67.78,0, 37.502),
        glm::vec3(-72.24,0, 34.63),
        glm::vec3(-76.21,0, 32.07),
        glm::vec3(-80,0, 30),
        glm::vec3(-85.03,0, 25.71),
        glm::vec3(-88.40,0, 21.17),
        glm::vec3(-91.09,0, 14.77),
        glm::vec3(-92.77,0, 8.21),
        glm::vec3(-93.11,0, 0),
        glm::vec3(-92.44,0, -5.75),
        glm::vec3(-90.08,0, -11.64),
        glm::vec3(-85.71,0, -15.68),
        glm::vec3(-79.81,0, -17.87),
        glm::vec3(-72.41,0, -19.04),
        glm::vec3(-65.85,0, -19.21),
        glm::vec3(- 60.12,0, -14.67),
        glm::vec3(- 59.28,0, -7.43)
    };

    //Maresc densitatea de puncte:
    for (int i = 0; i < medianPoints.size(); i += 1) {
        glm::vec3 A = medianPoints[i];
        glm::vec3 B;
        if (i < medianPoints.size() - 1) {
            B = medianPoints[i + 1];
        } else {
            B = medianPoints[0];
        }
        glm::vec3 v = glm::normalize(B - A);
        float l = length(B - A);
        float step = l / 100;
        std::vector<glm::vec3> intermediate_points;
        for (int j = 1; j < 100; ++j) {
            intermediate_points.push_back(A + step * j * v);
        }
        auto iter = medianPoints.insert(medianPoints.begin() + i + 1, intermediate_points.begin(), intermediate_points.end());
        i += intermediate_points.size();
    }

    //Outer points and inner points:
    std::vector<glm::vec3> outerPoints;
    std::vector<glm::vec3> innerPoints;
    glm::vec3 outer; glm::vec3 inner;
    float dRed, dBlue;
    dRed = dBlue = 4;

    for (int i = 0; i < medianPoints.size(); ++i) {
        glm::vec3 A = medianPoints[i];
        glm::vec3 B;
        if (i < medianPoints.size() - 1) {
            B = medianPoints[i + 1];
        }
        else {
            B = medianPoints[0];
        }
        glm::vec3 D = B - A;
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        P = glm::normalize(P);
        outer = A + dRed * P;
        inner = A - dBlue * P;
        innerPoints.push_back(inner);
        outerPoints.push_back(outer);
    }

    Mesh* track = new Mesh(name);
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < medianPoints.size(); ++i) {
        vertices.push_back(VertexFormat(outerPoints[i], color));
        vertices.push_back(VertexFormat(innerPoints[i], color));
    }

    for (int i = 0; i <= (medianPoints.size() - 2) * 2; i += 2) {
        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+3);
        indices.push_back(i);
        indices.push_back(i+3);
        indices.push_back(i+2);
    }

    indices.push_back(medianPoints.size() * 2 - 2);
    indices.push_back(medianPoints.size() * 2 - 1);
    indices.push_back(1);
    indices.push_back(medianPoints.size() * 2 - 2);
    indices.push_back(1);
    indices.push_back(0);

    track->InitFromData(vertices, indices);

    object3DMesh::Track comp;
    comp.track.mesh = track;
    comp.track.innerPoints = innerPoints;
    comp.track.outerPoints = outerPoints;
    comp.track.medianPoints = medianPoints;

    return comp; 
}

Mesh* object3DMesh::Create3DSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, length), color),
        VertexFormat(corner + glm::vec3(0, 0, length), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}
