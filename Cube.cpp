#include "Cube.h"
#include "Window.h"

Cube::Cube(const char* filepath)
{
	toWorld = glm::mat4(1.0f);

    parse(filepath);
    
    //printf("Size is %lu \n", vertices.size());
    //printf("Size is %lu \n", normals.size());
    //printf("Size is %lu \n", indices.size());
    
    dimensionX = maxX + minX;
    dimensionY = maxY + minY;
    dimensionZ = maxZ + minZ;
    
    distanceX = maxX - minX;
    distanceY = maxY - minY;
    distanceZ = maxZ - minZ;
    
    //printf("%f, %f, %f\n",dimensionX,dimensionY,dimensionZ);
    
    float zoom;
    
    if(distanceX >= distanceY && distanceX >= dimensionZ)
    {
        zoom = distanceX;
        
    }
    else if(distanceY >= distanceX && distanceY >= distanceZ)
    {
        
        zoom = distanceY;
    }
    else
    {
        zoom = distanceZ;
    }
    
    //printf("Zoom is %f\n", zoom);
    

    for(int i = 0;  i < vertices.size(); i ++)
    {
        vertices[i].x = (vertices[i].x -(dimensionX/2.0f))*6.0F/zoom;
        vertices[i].y = (vertices[i].y -(dimensionY/2.0f))*6.0F/zoom;
        vertices[i].z = (vertices[i].z -(dimensionZ/2.0f))*6.0F/zoom;
    }
    
    position = glm::vec3(0.0f,0.0f,0.0f);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    //rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
    rotation = glm::rotate(glm::mat4(1.0f), 0.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*3*sizeof(float), normals.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 1 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(float), indices.data(), GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
     
}

Cube::~Cube()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw(GLuint shaderProgram)
{
    
    

    
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc  = glGetUniformLocation(shaderProgram,  "view");
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &Window::V[0][0]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &toWorld[0][0]);
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
    toWorld = orgWorld;
}

void Cube::update()
{
	//spin(1.0f);
    toWorld = glm::mat4(1.0f);
    toWorld = rotation * toWorld;
    toWorld = glm::scale(glm::mat4(1.0f),scale) * toWorld;
    toWorld = glm::translate(glm::mat4(1.0f), position) * toWorld;
    
    orgWorld = toWorld;
}

void Cube::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Cube::parse(const char* filepath)
{
    
    //TODO parse the OBJ file
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    
    unsigned int a,k,c,d,e,f;
    float x,y,z;
    float r,g,b,normal;
    char c1,c2;
    
    FILE *file = fopen(filepath,"rb");
    
    do{
        c1 = fgetc(file);
        if(feof(file) == 0 ) {
            c2 = fgetc(file);
            
            if(c1 == 'v' && c2 == ' ')
            {
                fscanf(file,"%f %f %f", &x, &y, &z);
                //printf("Vertex is %f %f %f \n", x, y, z);
                
                vertices.push_back(glm::vec3(x,y,z));
                
                if(x > maxX)
                {
                    maxX = x;
                }
                else if(x < minX)
                {
                    minX = x;
                }
                
                if(y > maxY)
                {
                    maxY = y;
                }
                else if(y < minY)
                {
                    minY = y;
                }
                
                if(z > maxZ)
                {
                    maxZ = z;
                }
                
                else if(z < minZ)
                {
                    minZ = z;
                }
                
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else if(c1 == 'v' && c2 == 'n')
            {
                fscanf(file,"%f %f %f",&r,&g,&b);
                //printf(" Prev Value is %f %f %f \n", r, g, b);
                
                normal = r*r + g*g + b*b;
                normal = sqrt(normal);
                
                r = r/normal;
                
                g = g/normal;
                
                b = b/normal;
                
                
                normals.push_back(glm::vec3(r,g,b));
                //printf(" After Value is %f %f %f \n", r, g, b);
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else if (c1 == 'f' && c2 == ' ')  // face line
            {
                //fscanf(file,"%f//%f %f//%f %f//%f",&x,&y,&z rgb);
                //printf("I am here\n");
                fscanf(file,"%d//%d %d//%d %d//%d",&a,&k,&c,&d,&e,&f);
                //printf(" The F value is %d %d %d %d %d %d\n", a,k,c,d,e,f);
                indices.push_back(a-1);
                indices.push_back(c-1);
                indices.push_back(e-1);
                //indices.push_back(d);
                //indices.push_back(e);
                //indices.push_back(f);
                while (c1 != '\n')   // read through this line
                {
                    c1 = fgetc(file);
                }
            }
            
            else
            {
                while (c1 != '\n')   // read through this line if it is comment line
                {
                    c1 = fgetc(file);
                }
            }
            
        }
        
        else{
            
            break;
        }
        
    } while(1);
    
    fclose(file);
    
}
