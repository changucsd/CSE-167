#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include <vector>

class Cube
{
public:
	Cube(const char* filepath);
	~Cube();

	glm::mat4 toWorld;

    glm::mat4 orgWorld;
    
	void draw(GLuint);
	void update();
	void spin(float);
    
    void parse(const char* filepath);

    
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    
    glm::vec3 position;
    glm::vec3 scale;
    glm::mat4 rotation;
    
    float maxX,minX,maxY,minY,maxZ,minZ;
    float dimensionX,dimensionY,dimensionZ,distanceX,distanceY,distanceZ;
    
	// These variables are needed for the shader program
	GLuint VBO,VBO2,VAO,EBO;
	GLuint uProjection, uModelview;
};



#endif

