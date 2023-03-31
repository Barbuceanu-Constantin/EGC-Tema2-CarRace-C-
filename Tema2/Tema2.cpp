#include <vector>
#include <string>
#include <iostream>
#include <time.h>

#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/Tema2.h"

using namespace std;
using namespace m1;
using namespace object3DMesh;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

typedef struct cars_positions {
    std::vector<glm::vec3> positions;
    std::vector<unsigned int> median_point_indexes;
    std::vector<bool> outer_or_inner;                   //true = outer; false = inner;
    std::vector<float> lateral_distances;
    std::vector<glm::mat4> matrices;
    std::vector<float> angles;
}cars_pos;

cars_pos cars_p;
int nr_bot_cars = 15;
float angle_my_car;
glm::mat4 matrix_my_car = glm::mat4(1);
glm::mat4 first_vis_matrix = glm::mat4(1);
glm::mat4 projection_matrix = glm::mat4(1);
glm::vec3 position_my_car = glm::vec3(1);
glm::vec3 car_pos, dif, old_dif;
float car_speed = 10;
float scaleFactor = 0;
bool canMoveForward = false;
bool canMoveBackward = false;
bool canRotLeft = false;
bool canRotRight = false;
bool started = false;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

bool Tema2::verify_margin() {
    //Verific daca se ciocneste cu marginea.
    for (int i = 0; i < track.track.medianPoints.size(); ++i) {
        glm::vec3 outer = track.track.outerPoints[i];
        glm::vec3 inner = track.track.innerPoints[i];
        if (distance(outer, car_pos) <= 0.3) return true;
        if (distance(inner, car_pos) <= 0.3) return true;
    }

    return false;
}

//Asta eu zic ca e bine. Pentru ca pozitia masinii mele este pe spatele ei. Pozitia masinii adverse este in centrul ei.
//Prin urmare distanta maxima la care se pot afla cele 2 puncte este 3 * (latura/2). Latura este 4 pentru toate masinile.
bool Tema2::verify_collision() {
    int nr = cars_p.positions.size();

    //Verific daca se ciocneste cu o alta masina.
    for (int i = 0; i < nr; ++i) {
        glm::vec3 adv_pos = cars_p.positions[i];
        if (sqrt((adv_pos.x - car_pos.x) * (adv_pos.x - car_pos.x) + (adv_pos.y - car_pos.y) * (adv_pos.y - car_pos.y) + (adv_pos.z - car_pos.z) * (adv_pos.z - car_pos.z)) <= 3 * 0.4)
            return true;
    }

    return false;
}

bool Tema2::min_dif(unsigned int index) {
    for (int i = 0; i < cars_p.median_point_indexes.size(); ++i) {
        if (cars_p.median_point_indexes[i] - index <= 100)
            return false;
    }
    return true;
}

bool Tema2::min_dist(float last_dist) {
    for (int i = 0; i < cars_p.lateral_distances.size(); ++i) {
        if (cars_p.lateral_distances[i] - last_dist == 0 && cars_p.outer_or_inner[i] == cars_p.outer_or_inner[i + 1])
            return false;
    }
    return true;
}

void Tema2::Init()
{
    polygonMode = GL_FILL;

    for (int i = 0; i < nr_bot_cars; ++i) {
        cars_p.matrices.push_back(glm::mat4(1) * transform3D::Translate(0,0.15,0));
    }

    Mesh* mesh;

    mesh = object3DMesh::Create3DSquare("grass", glm::vec3(-25, 0, -25), 50, glm::vec3(0, 1, 0), true);
    AddMeshToList(mesh);

    track = object3DMesh::Create3DTrack("track", glm::vec3(0, 0, 0));
    AddMeshToList(track.track.mesh);

    mesh = object3DMesh::Create3DTree("tree");
    AddMeshToList(mesh);

    mesh = object3DMesh::Create3DStartLine("start_line", track);
    AddMeshToList(mesh);

    mesh = object3DMesh::Create3DDiscLine("median_line");
    AddMeshToList(mesh);

    srand((unsigned)time(NULL));

    //Creez 10 masini adversare
    for (int i = 0; i < nr_bot_cars; ++i) {
        float r = (float)rand() / RAND_MAX;
        float g = (float)rand() / RAND_MAX;
        float b = (float)rand() / RAND_MAX;
        char name[20];
        sprintf(name, "car%d", i + 1);
        mesh = object3DMesh::Create3DCar(name, glm::vec3(r, g, b));
        AddMeshToList(mesh);

        int index, outer;
        float last_dist;
        do {
            index = rand() % track.track.medianPoints.size();
        } while (min_dif(index) == false);

        outer = rand() % 2;
        if (outer == 1) cars_p.outer_or_inner.push_back(true);              //pushez booleanul de exterior/interior
        else cars_p.outer_or_inner.push_back(false);

        do {
            last_dist = ((float)rand() / RAND_MAX) * 3;
        } while (min_dist(last_dist) == false);

        cars_p.median_point_indexes.push_back(index);                       //pushez indexul median al masinii

        glm::vec3 D = track.track.medianPoints[index + 1] - track.track.medianPoints[index];
        glm::vec3 P = normalize(cross(D, glm::vec3(0, 1, 0)));
        glm::vec3 position;
        if (outer == 1)
            position = track.track.medianPoints[index] + last_dist * P;
        else
            position = track.track.medianPoints[index] - last_dist * P;

        cars_p.positions.push_back(position);                               //pushez positia initiala a masinii
        cars_p.lateral_distances.push_back(last_dist);                      //pushez distanta laterala a masinii

        //pushez matricea de miscare
        glm::vec3 P1 = track.track.medianPoints[index];
        glm::vec3 P2 = track.track.medianPoints[index + 1];
        glm::vec3 Dp = normalize(P2 - P1);
        glm::vec3 m1 = glm::vec3(0.5, 0, 0);
        glm::vec3 m2 = glm::vec3(0.5, 0, 0.5);
        glm::vec3 Dm = normalize(m2 - m1);
        float dotProduct = dot(Dp, Dm);
        float crossProduct = dot(normalize(cross(Dp, Dm)), glm::vec3(0, 1, 0));

        if (crossProduct < 0) {
            cars_p.matrices[i] = transform3D::Translate(position.x, position.y, position.z) * transform3D::RotateOY(acos(dotProduct)) * cars_p.matrices[i];
            cars_p.angles.push_back(acos(dotProduct));
        }
        else if (crossProduct > 0) {
            cars_p.matrices[i] = transform3D::Translate(position.x, position.y, position.z) * transform3D::RotateOY(-acos(dotProduct)) * cars_p.matrices[i];
            cars_p.angles.push_back(-acos(dotProduct));
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    mesh = object3DMesh::Create3DMyCar("my_car", glm::vec3(0.06, 0.02, 0.32));
    AddMeshToList(mesh);

    glm::vec3 P1 = track.track.medianPoints[0];
    glm::vec3 P2 = track.track.medianPoints[1];
    glm::vec3 Dp = normalize(P2 - P1);
    glm::vec3 m1 = glm::vec3(0.4, 0, 0);
    glm::vec3 m2 = glm::vec3(0.4, 0, 0.4);
    glm::vec3 Dm = normalize(m2 - m1);
    float dotProduct = dot(Dp, Dm);
    float crossProduct = dot(normalize(cross(Dp, Dm)), glm::vec3(0, 1, 0));

    if (crossProduct < 0) {
        matrix_my_car = transform3D::Translate(P1.x, P1.y, P1.z) * transform3D::RotateOY(acos(dotProduct)) * matrix_my_car;
        angle_my_car = acos(dotProduct);
    }
    else if (crossProduct > 0) {
        matrix_my_car = transform3D::Translate(P1.x, P1.y, P1.z) * transform3D::RotateOY(-acos(dotProduct)) * matrix_my_car;
        angle_my_car = -acos(dotProduct);
    }
    position_my_car = P1;
    
    //////////
    glm::vec3 position, center, up, PN;
    camera = new implemented::MyCamera();
    minimap_camera = new implemented::MyCamera();
    glm::vec3 D = P2 - P1;
    center = position_my_car;
    position = center + camera->distanceToTarget * normalize(P1 - P2);

    camera->Set(position, center, glm::vec3(0, 1, 0));
    minimap_camera->Set(glm::vec3(position_my_car.x, 10, position_my_car.z), glm::vec3(P1.x, -10, P1.z), normalize(P2 - P1));
    /////////////

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.58, 0.93, 0.93, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderTrees(implemented::MyCamera* c) {
    int nr_points = track.track.outerPoints.size();
    for (int i = 0; i < nr_points - 1; i += 150) {
        glm::vec3 D = track.track.outerPoints[i + 1] - track.track.outerPoints[i];
        glm::vec3 P = normalize(cross(D, glm::vec3(0, 1, 0)));
        float dist = 3;
        glm::vec3 out = track.track.outerPoints[i] + dist * P;
        glm::mat4 translate = transform3D::Translate(out.x, out.y, out.z);
        translate *= transform3D::Translate(0, -0.3, 0);
        RenderMesh(meshes["tree"], shaders["VertexColor"], projection_matrix * c->GetViewMatrix() * translate);
    }
    for (int i = 0; i < nr_points - 1; i += 50) {
        glm::vec3 D = track.track.innerPoints[i + 1] - track.track.innerPoints[i];
        glm::vec3 P = normalize(cross(D, glm::vec3(0, 1, 0)));
        float dist = 2.75;
        glm::vec3 out = track.track.innerPoints[i] - dist * P;
        glm::mat4 translate = transform3D::Translate(out.x, out.y, out.z);
        translate *= transform3D::Translate(0, -0.3, 0);
        RenderMesh(meshes["tree"], shaders["VertexColor"], projection_matrix * c->GetViewMatrix() * translate);
    }
}

void Tema2::RenderMedianLines(implemented::MyCamera* c) {
    int nr_points = track.track.medianPoints.size();
    for (int i = 20; i < nr_points - 1; i += 40) {
        glm::vec3 P1 = track.track.medianPoints[i];
        glm::vec3 P2 = track.track.medianPoints[i + 1];
        glm::vec3 Dp = normalize(P2 - P1);
        glm::vec3 m1 = glm::vec3(0.125, 0.02, 0);
        glm::vec3 m2 = glm::vec3(0.125, 0.02, 0.6);
        glm::vec3 Dm = normalize(m2 - m1);
        float dotProduct = dot(Dp, Dm);
        float crossProduct = dot(normalize(cross(Dp, Dm)), glm::vec3(0,1,0));

        glm::mat4 translate;
        if(crossProduct == -1)
            translate = transform3D::Translate(P1.x, P1.y, P1.z) * transform3D::RotateOY(acos(dotProduct));
        else if(crossProduct == 1)
            translate = transform3D::Translate(P1.x, P1.y, P1.z) * transform3D::RotateOY(-acos(dotProduct));
        translate *= transform3D::Translate(0, -0.3, 0);
        RenderMesh(meshes["median_line"], shaders["VertexColor"], projection_matrix * c->GetViewMatrix() * translate);
    }
}

void Tema2::RenderCars(implemented::MyCamera *c) {
    for (int i = 0; i < nr_bot_cars; ++i) {
        char name[20];
        sprintf(name, "car%d", i + 1);
        glm::mat4 translate = cars_p.matrices[i];

        unsigned int index = cars_p.median_point_indexes[i];            //Extrag indexul median al masinii.
        unsigned int nr_points = track.track.medianPoints.size();       //Extrag numarul total de puncte mediane.
        glm::vec3 P1 = track.track.medianPoints[index];
        glm::vec3 P2;
        if (index - 2 < 2) {
            P2 = track.track.medianPoints[track.track.medianPoints.size() - 1];
            index = track.track.medianPoints.size() - 1;
        }
        else {
            P2 = track.track.medianPoints[index - 2];
            index -= 2;
        }
        glm::vec3 D1 = P2 - P1;
        glm::vec3 position = cars_p.positions[i];

        //Actualizez matricea de miscare in vectorul de matrici.
        //Rotesc:
        float angle = -cars_p.angles[i] + atan((P2.z - P1.z)/(P2.x - P1.x));
        cars_p.matrices[i] = transform3D::Translate(position.x, position.y, position.z) * transform3D::RotateOY(angle) * transform3D::Translate(-position.x, -position.y, -position.z) * cars_p.matrices[i];
        cars_p.angles[i] = atan((P2.z - P1.z) / (P2.x - P1.x));

        //Translatez:
        cars_p.matrices[i] = transform3D::Translate(D1.x, D1.y, D1.z) * cars_p.matrices[i];

        //Actualizez indexul median al masinii.
        cars_p.median_point_indexes[i] = index;

        //Actualizez pozitia masinii.
        cars_p.positions[i] = position + D1;

        RenderMesh(meshes[name], shaders["VertexColor"], projection_matrix * c->GetViewMatrix() * translate);
    }
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int location = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    location = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    //glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));///!!!

    location = glGetUniformLocation(shader->program, "ScaleFactor");
    glUniform1f(location, scaleFactor);
    location = glGetUniformLocation(shader->program, "CarPos");
    glUniform3fv(location, 1, glm::value_ptr(car_pos));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glm::mat4 rotation_matrix = glm::mat4(1);

    bool is_rotated = false;

    if (canRotLeft || canRotRight) {
        is_rotated = true;
        glm::vec3 last_forward = camera -> forward;
        
        if (canRotLeft) camera->RotateThirdPerson_OY(-0.02);
        if (canRotRight) camera->RotateThirdPerson_OY(0.02);

        float dotProduct = dot(camera->forward, last_forward);
        float crossProduct = dot(normalize(cross(camera->forward, last_forward)), glm::vec3(0, 1, 0));

        if (canRotLeft) camera->RotateThirdPerson_OY(0.02);
        if (canRotRight) camera->RotateThirdPerson_OY(-0.02);
        if (crossProduct < 0) {
            camera->RotateThirdPerson_OY(acos(dotProduct));
            minimap_camera->RotateThirdPerson_OY(acos(dotProduct));
            angle_my_car = acos(dotProduct);
        }
        else if (crossProduct > 0) {
            camera->RotateThirdPerson_OY(-acos(dotProduct));
            minimap_camera->RotateThirdPerson_OY(-acos(dotProduct));
            angle_my_car = -acos(dotProduct);
        }
    }

    float speed = 0;
    if (canMoveForward || canMoveBackward) {
        dif = normalize(camera->forward);
        if (canMoveForward == true) {
            speed = deltaTimeSeconds * 8;
        }
        else if (canMoveBackward == true) {
            speed = -deltaTimeSeconds * 8;
        }
        minimap_camera->position.x += dif.x * speed;
        minimap_camera->position.z += dif.z * speed;
        matrix_my_car = transform3D::Translate(dif.x * speed, 0, dif.z * speed) * matrix_my_car;
    }

    if(canRotLeft || canRotRight)
        rotation_matrix = transform3D::Translate(car_pos.x + dif.x * speed, car_pos.y, car_pos.z + dif.z * speed) * transform3D::RotateOY(angle_my_car) * transform3D::Translate(-car_pos.x - dif.x*speed, -car_pos.y, -car_pos.z - dif.z * speed);

    matrix_my_car = rotation_matrix * matrix_my_car;

    float x = 10.0, y = 10.0;

    projectionMatrix = glm::perspective(RADIANS(45), window->props.aspectRatio, 1.0f, 200.0f);
    RenderScene();
    glViewport(x, y, width, height);
    glClearColor(0.58, 0.93, 0.93, 1);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    RenderMesh(meshes["grass"], shaders["VertexColor"], (projection_matrix * minimap_camera->GetViewMatrix() * transform3D::Translate(0, -0.31, 0) * transform3D::Scale(10, 1, 10)));
    RenderMesh(meshes["track"], shaders["VertexColor"], (projection_matrix * minimap_camera->GetViewMatrix()) * transform3D::Translate(0, -0.305, 0));
    RenderTrees(minimap_camera);
    RenderMesh(meshes["start_line"], shaders["VertexColor"], projection_matrix * minimap_camera->GetViewMatrix() * transform3D::Translate(0, -0.30, 0));
    RenderMedianLines(minimap_camera);
    RenderCars(minimap_camera);
    RenderMesh(meshes["my_car"], shaders["VertexColor"], projection_matrix * minimap_camera->GetViewMatrix() * matrix_my_car);

    canMoveForward = false;
    canMoveBackward = false;
    canRotRight = false;
    canRotLeft = false;
}

void Tema2::RenderScene()
{
    RenderMesh(meshes["grass"], shaders["VertexColor"], projection_matrix * camera->GetViewMatrix() * transform3D::Translate(0, -0.31, 0) * transform3D::Scale(10, 1, 10));
    RenderMesh(meshes["track"], shaders["VertexColor"], projection_matrix * camera->GetViewMatrix() * transform3D::Translate(0, -0.305, 0));
    RenderTrees(camera);
    RenderMesh(meshes["start_line"], shaders["VertexColor"], projection_matrix * camera->GetViewMatrix() * transform3D::Translate(0, -0.30, 0));
    RenderMedianLines(camera);
    RenderCars(camera);
    RenderMesh(meshes["my_car"], shaders["VertexColor"], projection_matrix * camera->GetViewMatrix() * matrix_my_car);
}

void Tema2::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

float blocked = false;
bool forw = false;
bool backward = false;

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    // Primul cub
    if (blocked == false) {
        if (window->KeyHold(GLFW_KEY_W) && !verify_collision()) {
            started = true;
            if (!verify_margin()) {
                camera->MoveForward(deltaTime * 8);
                car_pos = camera->GetTargetPosition();
                canMoveForward = true;
                blocked = false;
            }
            else {
                blocked = true;
                backward = true;
            }
        } else if (window->KeyHold(GLFW_KEY_S) && !verify_collision()) {
            started = true;
            if (!verify_margin()) {
                camera->MoveForward(-deltaTime * 8);
                car_pos = camera->GetTargetPosition();
                canMoveBackward = true;
                blocked = false;
            }
            else {
                blocked = true;
                forw = true;
            }
        }
        //Sunt inversate. Dar asa merge normal.
        if (started) {
            if (window->KeyHold(GLFW_KEY_A)) {
                canRotRight = true;
            }
            else if (window->KeyHold(GLFW_KEY_D)) {
                canRotLeft = true;
            }
        }
    }
    else {
        if (backward) {
            if (window->KeyHold(GLFW_KEY_S)) {
                    camera->MoveForward(-deltaTime * 8);
                    car_pos = camera->GetTargetPosition();
                    canMoveBackward = true;
                    blocked = false;
                    backward = false;
            }
        } else if (forw) {
            if (window->KeyHold(GLFW_KEY_W)) {
                camera->MoveForward(deltaTime * 8);
                car_pos = camera->GetTargetPosition();
                canMoveForward = true;
                blocked = false;
                forw = false;
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
