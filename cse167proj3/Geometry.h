//
//  Geometry.h
//  cse167proj3
//
//  Created by LSQ on 10/24/19.
//  Copyright © 2019 LSQ. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <map>

#include "Node.h"

class Geometry: public Node
{
public:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    GLuint vao;
    GLuint vbos[2];
    GLuint ebo;
    static std::map<std::string, int> colorForObj;


    Geometry(std::string filename, GLfloat radius);
    glm::mat4 M;
    std::string filename;
    void setModelMatrix(glm::mat4 C);
    void setColor();
    void draw(glm::mat4 C);
    void render();
    void init(std::string filename);
    void update();
    glm::vec3 top();
    glm::vec3 objCenter;
    void findCenter();
    void setRadius(GLfloat radius);
};

#endif /* Geometry_h */
