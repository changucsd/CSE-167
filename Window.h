#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>


#include "shader.h"
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "stb_image.h"

//#include "scenegraph.hpp"
class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
    
    static GLint shaderProgram;
    static GLint Skyshader;
    static GLint Curveshader;
    
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static unsigned int loadCubemap(std::vector<std::string> faces);
    
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static glm::vec3 trackBallmapping(float x,float y);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    static void caculateXT();
    
};

#endif
