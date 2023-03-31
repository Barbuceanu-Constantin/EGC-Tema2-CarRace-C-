#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/object3DMesh.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        bool isOrtho = false;
        float orthoRight = 8.0f, orthoUp = 4.5f, orthoDown = -4.5f, orthoLeft = -8.0f;

        object3DMesh::Track track;

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderScene();
        void RenderTrees(implemented::MyCamera *c);
        void RenderCars(implemented::MyCamera* c);
        void RenderMedianLines(implemented::MyCamera* c);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        bool min_dif(unsigned int index);
        bool min_dist(float last_dist);
        bool verify_collision();
        bool verify_margin();

     protected:
        implemented::MyCamera *camera;
        implemented::MyCamera *minimap_camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        float fov;

        int nr_of_track_triangles;

        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        float x, y, width = 300, height = 200;
        float rotateBonus;
        GLenum polygonMode;
        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1
