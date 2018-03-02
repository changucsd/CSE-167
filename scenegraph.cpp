//
//  scenegraph.cpp
//  proect 3.1
//
//  Created by He Chang on 2/27/18.
//  Copyright © 2018 He Chang. All rights reserved.
//


#include "scenegraph.hpp"
#include "Window.h"

Transform::Transform(glm::mat4 C)
{
    Transform::M = C;
}

void Transform::draw(glm::mat4 C)
{
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it)
    {
        (*it)->draw(C * Transform::M);
    }
}

void Transform::update(glm::mat4 C)
{
    Transform::M = C;
}

void Transform::addChild(Node * toadd)
{
    Transform::children.push_back(toadd);
}

void Transform::removeChild(Node * toremove)
{
    Transform::children.remove(toremove);
}


Geometry::Geometry(Cube * theobj)
{
    Geometry::obj = theobj;
}

void Geometry::draw(glm::mat4 C)
{
    //printf("I will draw it\n");
    Geometry::obj->toWorld = C * Geometry::obj->toWorld;

    Geometry::obj->draw(Window::shaderProgram);
}

void Geometry::update(glm::mat4 C)
{
    
}

Group::Group()
{
    
}

void Group::addChild(Node * toadd)
{
    Group::children.push_back(toadd);
}

void Group::removeChild(Node * toremove)
{
    Group::children.remove(toremove);
}

void Group::draw(glm::mat4 C)
{
    for (std::list<Node*>::iterator it = Group::children.begin(); it != Group::children.end(); ++it)
    {
        (*it)->draw(C);
    }
}

void Group::update(glm::mat4 C)
{
 
}
