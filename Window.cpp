#include "Window.h"
#include "scenegraph.hpp"
#include "Cube.h"
#include <vector>
#include <string>

const char* window_title = "GLFW Starter Project";

Cube * body;
Cube * head;
Cube * limbR;
Cube * limbL;
Cube* eyeR;
Cube* eyeL;
Cube * footR;
Cube * footL;


// geometry
Geometry * myhead;
Geometry * mybody;
Geometry * mylimbR;
Geometry * mylimbL;
Geometry * myfootR;
Geometry * myfootL;
Geometry * myeyeR;
Geometry * myeyeL;

// transformation

Transform* hmtx;
Transform* bmtx;
Transform* emtx;
Transform* limbLT;
Transform* limbLR;
Transform* limbRT;
Transform* limbRR;
Transform* footRT;
Transform* footRR;
Transform* footLT;
Transform* footLR;
Transform* modelmtx;

// army
Group * myarmy;
Group * party;

std::vector<Transform*> armypos;

std::vector<glm::vec3> curvesPoints;

GLint Window::shaderProgram;
GLint Window::Skyshader;
GLint Window::Curveshader;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

#define VERTEX_Sky_PATH "Skyshader.vert"
#define FRAGMENT_Sky_PATH "Skyshader.frag"

#define VERTEX_curve_PATH "./Curveshader.vert"
#define FRAGMENT_curve_PATH "./Curveshader.frag"

// Default camera parameters

glm::vec3 cam_pos(50.0f, 30.0f, 100.0f);		// e  | Position of camera
glm::vec3 cam_front(-50.0f, -30.0f, -100.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
 



int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

GLuint skyboxVAO, skyboxVBO;

GLuint curveVAO, curveVBO;

float lastX,lastY;
bool firstMouse;
bool leftPress, rightPress;

float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float fov   =  45.0f;

float angle;
float sign;

int myIndex;

unsigned int cubemapTexture;

std::vector<std::string> faces
{
    /*
    "moondust_right.tga",
    "moondust_left.tga",
    "moondust_up.tga",
    "moondust_down.tga",
    "moondust_front.tga",
    "moondust_back.tga"
     */
    
    "moondust_ft.jpg",
    "moondust_bk.jpg",
    "moondust_up.jpg",
    "moondust_dn.jpg",
    "moondust_rt.jpg",
    "moondust_lf.jpg"
    
    
};


float skyboxVertices[] = {
    // positions
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    
    -1000.0f,  1000.0f, -1000.0f,
    1000.0f,  1000.0f, -1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
    1000.0f, -1000.0f,  1000.0f
};

void Window::initialize_objects()
{
	body = new Cube("body.obj");
    limbR = new Cube("limb.obj");
    limbL = new Cube("limb.obj");
    head = new Cube("head.obj");
    
    eyeR = new Cube("eyeball.obj");
    eyeL = new Cube("eyeball.obj");
    
    footR = new Cube("limb.obj");
    footL = new Cube("limb.obj");
    
    
    body->rotation = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    body->scale = glm::vec3(0.9f,0.9f,0.9f);
    body->position = glm::vec3(0.0f,0.0f,-2.0f);
    body->update();
    
    head->rotation = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    head->scale = glm::vec3(1.2f,1.2f,1.2f);
    head->position = glm::vec3(0.0f,-1.9f,-2.0f);
    head->update();
    
    limbR->rotation = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    limbR->position = glm::vec3(2.6f,0.0f,-2.0f);
    limbR->update();
    
    limbL->rotation = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    limbL->position = glm::vec3(-1.0f,0.0f,-2.0f);
    limbL->update();
    
    footR->rotation = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    footR->position = glm::vec3(1.8f,-2.5f,-2.0f);
    footR->update();
    
    footL->rotation = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
    footL->position = glm::vec3(-0.1f,-2.5f,-2.0f);
    footL->update();
    
    eyeR->position = glm::vec3(0.5f,1.2f,-0.7f);
    eyeR->scale = glm::vec3(0.2f,0.2f,0.2f);
    eyeR->update();
    
    eyeL->position = glm::vec3(-0.5f,1.2f,-0.7f);
    eyeL->scale = glm::vec3(0.2f,0.2f,0.2f);
    eyeL->update();
    
    // scene graph
    myhead = new Geometry(head);
    mybody = new Geometry(body);
    mylimbR = new Geometry(limbR);
    mylimbL = new Geometry(limbL);
    myfootR = new Geometry(footR);
    myfootL = new Geometry(footL);
    myeyeR = new Geometry(eyeR);
    myeyeL = new Geometry(eyeL);
    
    
    hmtx = new Transform(glm::mat4(1.0f));
    bmtx = new Transform(glm::mat4(1.0f));
    emtx = new Transform(glm::mat4(1.0f));
    limbLT = new Transform(glm::mat4(1.0f));
    limbLR = new Transform(glm::mat4(1.0f));
    limbRT = new Transform(glm::mat4(1.0f));
    limbRR = new Transform(glm::mat4(1.0f));
    footRT = new Transform(glm::mat4(1.0f));
    footRR = new Transform(glm::mat4(1.0f));
    footLT = new Transform(glm::mat4(1.0f));
    footLR = new Transform(glm::mat4(1.0f));
    modelmtx = new Transform(glm::mat4(1.0f));
    
    myarmy = new Group();
    party = new Group();
    
    // build the graph
    hmtx->addChild(myhead);
    bmtx->addChild(mybody);
    
    emtx->addChild(myeyeR);
    emtx->addChild(myeyeL);
    
    limbLT->addChild(limbLR);
    limbLR->addChild(mylimbL);
    
    limbRT->addChild(limbRR);
    limbRR->addChild(mylimbR);
    
    footRT->addChild(footRR);
    footRR->addChild(myfootR);
    
    footLT->addChild(footLR);
    footLR->addChild(myfootL);
    
    modelmtx->addChild(hmtx);
    modelmtx->addChild(bmtx);
    modelmtx->addChild(emtx);
    modelmtx->addChild(limbLT);
    modelmtx->addChild(limbRT);
    modelmtx->addChild(footLT);
    modelmtx->addChild(footRT);
    
    myarmy->addChild(modelmtx);
    
    
    // army position
    
    for(float i = 0; i < 5; i ++)
    {
        for(float j = 0; j < 5; j ++)
        {
            Transform* newtrans = new Transform((glm::translate(glm::mat4(1.0f), glm::vec3(- i * 6.0f, 0.0f,-j * 6.0f))));
            armypos.push_back(newtrans);
            newtrans->addChild(myarmy);
            party->addChild(newtrans);
        }
    }
    
    
    
    //curves
    caculateXT();
    
    //printf("%lu \n", curvesPoints.size());
    
    firstMouse = true;
    leftPress = false;
    rightPress = false;
    angle = 0.0f;
    sign = 1.0f;
    
    myIndex = 0;
    
    P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1500.0f);
    V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
    
    
    cubemapTexture = loadCubemap(faces);
    
    // skybox and shader
    glEnable(GL_DEPTH_TEST);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    Skyshader = LoadShaders(VERTEX_Sky_PATH, FRAGMENT_Sky_PATH);
    Curveshader = LoadShaders(VERTEX_curve_PATH, FRAGMENT_curve_PATH);
    
 
    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &curveVAO);
    glGenBuffers(1, &curveVBO);
    
    glBindVertexArray(curveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
    glBufferData(GL_ARRAY_BUFFER, curvesPoints.size()*3*sizeof(float), curvesPoints.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(body);
    delete(limbR);
    delete(limbL);
    delete(head);
    
    delete(eyeL);
    delete(eyeR);
    
    
	glDeleteProgram(shaderProgram);
    glDeleteProgram(Skyshader);
    glDeleteProgram(Curveshader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
    
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1500.0f);
		V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
  
    P = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1500.0f);
    V = glm::lookAt(cam_pos, cam_pos + cam_front, cam_up);
    //V = glm::mat4(glm::mat3(glm::lookAt(cam_pos, cam_pos + cam_front, cam_up)));
    //body->update();
    //V = glm::mat4(glm::mat3(glm::lookAt(cam_pos, cam_pos + cam_front, cam_up)));
    
    myIndex ++;
    if (myIndex >= (int)curvesPoints.size())
    {
        myIndex = 0;
    }
    
    //std::cout<< myIndex << std::endl;
    
    if(angle > 60.0f || angle < -60.0f)
    {
        sign = -sign;
    }
    
    angle = angle + sign* 10.0f;
    
    limbRR->update(
    glm::translate(glm::mat4(1.0f), glm::vec3(3.0f,0.0f,-2.0f))*
    glm::rotate(glm::mat4(1.0f), angle/ 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*
    glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f,0.0f,2.0f)));
    
    footLR->update(
    glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f,-2.5f,-2.0f))*
    glm::rotate(glm::mat4(1.0f), angle/ 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*
    glm::translate(glm::mat4(1.0f), glm::vec3(0.1f,2.5f,2.0f)));
    
    limbLR->update(
                   glm::translate(glm::mat4(1.0f), glm::vec3(-1.3f,0.0f,-2.0f))*
                   glm::rotate(glm::mat4(1.0f), -angle/ 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*
                   glm::translate(glm::mat4(1.0f), glm::vec3(1.3f,0.0f,2.0f)));
    
    footRR->update(
                   glm::translate(glm::mat4(1.0f), glm::vec3(1.8f,-2.5f,-2.0f))*
                   glm::rotate(glm::mat4(1.0f), -angle/ 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*
                   glm::translate(glm::mat4(1.0f), glm::vec3(-1.8f,2.5f,2.0f)));
}

void Window::display_callback(GLFWwindow* window)
{
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
	// Use the shader of programID
	glUseProgram(shaderProgram);
    party->draw(glm::translate(glm::mat4(1.0f),curvesPoints[myIndex] + glm::vec3(2 * 6.0f, 0.0f,2 * 6.0f)));
    
    /*
	// Render the cube
	body->draw(shaderProgram);
    head->draw(shaderProgram);
    limbR->draw(shaderProgram);
    limbL->draw(shaderProgram);
    
    eyeR->draw(shaderProgram);
    eyeL->draw(shaderProgram);
    
    footR->draw(shaderProgram);
    footL->draw(shaderProgram);
    */
    //modelmtx->draw(glm::mat4(1.0f));
    
    
    glUseProgram(Curveshader);
    GLint curveProjection = glGetUniformLocation(Curveshader, "projection");
    GLint curveModelview  = glGetUniformLocation(Curveshader,  "modelview");
    
    glUniformMatrix4fv(curveProjection, 1, GL_FALSE, &P[0][0]);
    glUniformMatrix4fv(curveModelview, 1, GL_FALSE, &V[0][0]);
    glBindVertexArray(curveVAO);
    glLineWidth(5.0f);
    glDrawArrays(GL_LINE_LOOP, 0, (int)curvesPoints.size());
    glBindVertexArray(0);
    
    
    
    glDepthFunc(GL_LEQUAL);
    
    glUseProgram(Skyshader);
    
    GLint projectLoc = glGetUniformLocation(Skyshader, "projection");
    GLint viewLoc  = glGetUniformLocation(Skyshader,  "view");
    GLint skybox = glGetUniformLocation(Skyshader,  "skybox");

    glUniformMatrix4fv(projectLoc, 1, GL_FALSE, &P[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &V[0][0]);
    glUniform1i(skybox, 0);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
    
    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}



glm::vec3 Window::trackBallmapping(float x,float y)
{
    //printf("width and height is %d %d\n", width, height);
    glm::vec3 vector;
    float d;
    vector.x = (2.0f*x - width)/width;
    vector.y = (height - 2.0f*y)/height;
    vector.z = 0.0f;
    
    d = glm::length(vector);
    if(d > 1.0f)
    {
        d = 1.0f;
    }
    
    vector.z = sqrt(1.001 - d*d);
    vector = glm::normalize(vector);
    
    return vector;
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //printf("Mouse is called");
    if(leftPress == true)
    {
  
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        
        //float xoffset = xpos - lastX;
        //float yoffset = lastY - ypos;
    

        //glm::vec3 lastPoint = trackBallmapping(lastX, lastY);
        //glm::vec3 newPoint = trackBallmapping((float)xpos, (float)ypos);
        
       // if(lastPoint != newPoint)
       // {
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

            
            float sensitivity = 0.2f; // change this value to your liking
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            
            yaw += xoffset;
            pitch += yoffset;
            
            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
            
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cam_front = glm::normalize(front);
        //}
        
        lastX = xpos;
        lastY = ypos;
    }
    
    else if (rightPress == true)
    {
        glfwGetCursorPos(window, &xpos, &ypos);
        
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        glm::vec3 lastPoint = trackBallmapping(lastX, lastY);
        glm::vec3 newPoint = trackBallmapping((float)xpos, (float)ypos);
        
        if(lastPoint != newPoint)
        {
            glm::vec3 axis = glm::cross(lastPoint,newPoint);
            float c = glm::dot(lastPoint,newPoint);
            float d = glm::acos(c);
        
            glm::vec4 newcampos = glm::rotate(glm::mat4(1.0f), d*2.0f, axis) * glm::vec4(cam_pos,1.0f);
        
            //std::cout << newcampos.x << std::endl;
        
            cam_pos.x = newcampos.x;
            cam_pos.y = newcampos.y;
            cam_pos.z = newcampos.z;
           
            
            cam_front.x = -cam_pos.x;
            cam_front.y = -cam_pos.y;
            cam_front.z = -cam_pos.z;
            
            
            lastX = xpos;
            lastY = ypos;
        }
    }
 }

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //printf("hahaha\n");
        leftPress = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        //printf("lololo\n");
        leftPress = false;
        firstMouse = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightPress = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightPress = false;
        firstMouse = true;
    }
    
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    //if (fov >= 1.0f && fov <= 45.0f)
    if (fov >= 1.0f && fov <= 90.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 90.0f)
        fov = 90.0f;
    
}


unsigned int Window::loadCubemap(std::vector<std::string> faces)
{
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                         );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

void Window::caculateXT()
{

    glm::mat4 toMult = glm::mat4(glm::vec4(-1.0f,3.0f,-3.0f,1.0f),glm::vec4(3.0f,-6.0f,3.0f,0.0f),glm::vec4(-3.0f,3.0f,0.0f,0.0f),glm::vec4(1.0f,0.0f,0.0f,0.0f));
    
  
    //glm::mat4 newmat41 = glm::mat4(glm::vec4(1,0,-1,1),glm::vec4(16,-9,11,1),glm::vec4(9,13,-3,1),glm::vec4(1,0,-1,1)) * toMult;
    glm::mat4 newmat41 = glm::mat4(glm::vec4(1.0f,0.0f,-1.0f,1.0f),glm::vec4(-3.0f,-9.0f,-17.0f,1.0f),glm::vec4(9.0f,13.0f,-3.0f,1.0f),glm::vec4(1.0f,10.0f,-7.0f,1.0f)) * toMult;
    glm::mat4 newmat42 = glm::mat4(glm::vec4(1.0f,10.0f,-7.0f,1.0f),glm::vec4(7.0f,7.0f,-11.0f,1.0f),glm::vec4(-7.0f,1.0f,11.0f,1.0f),glm::vec4(5.0f,8.0f,6.0f,1.0f)) * toMult;
    glm::mat4 newmat43 = glm::mat4(glm::vec4(5.0f,8.0f,6.0f,1.0f),glm::vec4(17.0f,15.0f,1.0f,1.0f),glm::vec4(-1.0f,-19.0f,-9.0f,1.0f),glm::vec4(7.0f,0.0f,4.0f,1.0f)) * toMult;
    glm::mat4 newmat44 = glm::mat4(glm::vec4(7.0f,0.0f,4.0f,1.0f),glm::vec4(15.0f,19.0f,17.0f,1.0f),glm::vec4(4.0f,-10.0f,11.0f,1.0f),glm::vec4(10.0f,-2.0f,16.0f,1.0f)) * toMult;
    glm::mat4 newmat45 = glm::mat4(glm::vec4(10.0f,-2.0f,16.0f,1.0f),glm::vec4(16.0f,6.0f,21.0f,1.0f),glm::vec4(5.0f,9.0f,15.0f,1.0f),glm::vec4(1.0f,0.0f,-1.0f,1.0f)) * toMult;
    
    glm::vec4 tostore;
    for(float i = 0; i < 150 ; i ++)
    {
        tostore = newmat41 * glm::vec4((i/150)*(i/150)*(i/150), (i/150)*(i/150), (i/150),1);
        
        //std::cout<< tostore.x << "x" <<tostore.y << "y"<< tostore.z << "z" << std::endl;
        
        curvesPoints.push_back(glm::vec3(tostore.x,tostore.y,tostore.z)*3.0f);
    }
    
    
    for(float i = 0; i < 150 ; i ++)
    {
        tostore = newmat42 * glm::vec4((i/150)*(i/150)*(i/150), (i/150)*(i/150), (i/150),1);
        curvesPoints.push_back(glm::vec3(tostore.x,tostore.y,tostore.z)*3.0f);
    }
    
    for(float i = 0; i < 150 ; i ++)
    {
        tostore = newmat43 * glm::vec4((i/150)*(i/150)*(i/150), (i/150)*(i/150), (i/150),1);
        curvesPoints.push_back(glm::vec3(tostore.x,tostore.y,tostore.z)*3.0f);
    }
    
    for(float i = 0; i < 150 ; i ++)
    {
        tostore = newmat44 * glm::vec4((i/150)*(i/150)*(i/150), (i/150)*(i/150), (i/150),1);
        curvesPoints.push_back(glm::vec3(tostore.x,tostore.y,tostore.z)*3.0f);
    }
 
    for(float i = 0; i < 150 ; i ++)
    {
        tostore = newmat45 * glm::vec4((i/150)*(i/150)*(i/150), (i/150)*(i/150), (i/150),1);
        curvesPoints.push_back(glm::vec3(tostore.x,tostore.y,tostore.z)*3.0f);
    }
    
    
}
