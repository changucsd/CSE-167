//
//  scenegraph.hpp
//  proect 3.1
//
//  Created by He Chang on 2/27/18.
//  Copyright © 2018 He Chang. All rights reserved.
//


#ifndef scenegraph_hpp
#define scenegraph_hpp

#include <stdio.h>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"

#include <vector>
#include <string>
#include <list>

using namespace std;

class Node
{
public:

    virtual void draw(glm::mat4 C) = 0;
    virtual void update(glm::mat4 C) = 0;
};

class Transform : public Node
{
public:
    glm::mat4 M;
    std::list<Node*> children;
    
    Transform(glm::mat4);
    //~Transform();
    
    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    void addChild(Node *);
    void removeChild(Node *);
    
};

class Geometry : public Node
{
public:
    Cube* obj;
    GLint shaderProgram;
    
    Geometry(Cube *);
    //~Geometry();
    
    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    
};

class Group: public Node
{
   public:
    
    std::list<Node*> children;
    
    Group();
    void draw(glm::mat4 C);
    void update(glm::mat4 C);
    void addChild(Node *);
    void removeChild(Node *);
    
};
#endif /* scenegraph_hpp */
