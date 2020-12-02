#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#undef main
#include "Shader.h"
#include "camera.h"
#include "render.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
using namespace std;

glm::vec3 lightColor1 = glm::vec3(1.f, 1.f, 1.0f);
glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition = glm::vec3(15.0f, 15.0f, 15.0f);
float currentX = camera.Position[0];
float currentY = camera.Position[1];
float currentZ = camera.Position[2];
int num_error = 0;
bool isRotate = false;
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 1080;


void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

int initializeWindowAndRenderContext(GLFWwindow*& window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::vector<Vertex> LoadObj(const std::string path, int& size);
void Light(const float time, Shader myshader, Camera cam, glm::vec3 lightColor);
void processInput(GLFWwindow* window);

int main(void)
{
    //Window object
    GLFWwindow* window;

    //Create Window
    num_error = initializeWindowAndRenderContext(window);
    if (num_error != 0) {
        glfwTerminate();
        cout << "Failed to initialzie window and render context" << endl;
    }

    // Eanable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    //for 3D, enable depth testing with z-buffer to avoid over-drawing of pixels
    glEnable(GL_DEPTH_TEST);

#pragma region Load Object 
    //Load the object model
    int size = 0;
    auto objVertices = LoadObj("Object\\", size);


    /*Generate your vertex array object and
    link all the required buffers and shaders
    */
    ////////////////////////////
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer vb(objVertices, sizeof(Vertex) * size);
    vb.Attach();

    //shaders
    std::string spath = "Shader\\shaderprogram.shader";
    Shader shader(spath);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shader.RendererID, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    GLint normAttrib = glGetAttribLocation(shader.RendererID, "normals");
    glEnableVertexAttribArray(normAttrib);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(float) * 3));

    GLint texAttrib = glGetAttribLocation(shader.RendererID, "texcord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(float) * 6));

#pragma endregion Load Object 

#pragma region Create Texture
    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("Texture\\texture1.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // load and create a texture 
// -------------------------
    unsigned int texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("Texture\\texture2.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.Attach(); 
    glUniform1i(glGetUniformLocation(shader.RendererID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.RendererID, "texture2"), 1);
#pragma endregion Create Texture

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // render the model
    glBindVertexArray(vao);

    // initialize timing
    float currentTime = 0.0f;
    glfwSetTime(0.0);

    // Show first information
    cout << "------------------------------------ First Initialization ------------------------------------" << endl;
    cout << "Light color -- R:" << lightColor1[0] << " G:" << lightColor1[1] << " B:" << lightColor1[2] << endl;
    cout << "Object color -- R:" << objectColor[0] << " G:" << objectColor[1] << " B:" << objectColor[2] << endl;
    cout << "View position = Camera position -- X:" << camera.Position[0] << " Y:" << camera.Position[1] << " Z:" << camera.Position[2] << endl;
    cout << "Light position X:" << lightPosition[0] << " Y:" << lightPosition[1] << " Z:" << lightPosition[2] << endl;
    cout << "Camera up -- X:" << camera.Up[0] << " Y:" << camera.Up[1] << " Z:" << camera.Up[2] << endl;
    cout << "Camera right -- X:" << camera.Right[0] << " Y:" << camera.Right[1] << " Z:" << camera.Right[2] << endl;
    cout << "----------------------------------------------------------------------------------------------" << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        currentTime = currentFrame;

        currentX = camera.Position[0];
        currentY = camera.Position[1];
        currentZ = camera.Position[2];
        // input keyboard
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        // also clear the depth buffer. 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Setingup transformation
        // pass projection matrix to shader    
        shader.Attach();       

        //fov
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float)width / (float)height, 0.1f, 100.0f);
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        // pass transformation matrices to the shader
        shader.SetUniformMatrix4fv("projection", 1, GL_FALSE, projection);
        shader.SetUniformMatrix4fv("view", 1, GL_FALSE, view);
        // render the model
        glBindVertexArray(vao);
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        float angle = 10.0f * float(currentTime);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.SetUniformMatrix4fv("model", 1, GL_FALSE, model);
        Light(float(currentTime), shader, camera, lightColor1);
        glDrawArrays(GL_TRIANGLES, 6, size);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }
    }
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    return 0;
}

int initializeWindowAndRenderContext(GLFWwindow*& window)
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Set window hints, set up the OpenGL context
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IDIG4002 Final Project - Group 5: Earth Surface Rendering with Hyperspectral Measurement", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;

    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /*Linking the mouse event call back functions for
    Left button click and scrolling actions/events
    */

    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //Error handling
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

std::vector<Vertex> LoadObj(const std::string path, int& size)
{
    /*Function adapted from PROG2002 course: Writen by Leon
    ----------------------------------------------------------------------*/
    //We create a vector of Vertex structs. OpenGL can understand these, and so will accept them as input.
    std::vector<Vertex> vertices;

    //Some variables that we are going to use to store data from tinyObj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; //This one goes unused for now, seeing as we don't need materials for this model.

    //Some variables incase there is something wrong with our obj file
    std::string warn;
    std::string err;

    //We use tinobj to load our models. Feel free to find other .obj files and see if you can load them.
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (path + "Mountain.obj").c_str(), path.c_str());

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    //For each shape defined in the obj file
    for (auto shape : shapes)
    {
        //We find each mesh
        for (auto meshIndex : shape.mesh.indices)
        {
            //And store the data for each vertice, including normals
            glm::vec3 vertice = {
                attrib.vertices[meshIndex.vertex_index * 3],
                attrib.vertices[(meshIndex.vertex_index * 3) + 1],
                attrib.vertices[(meshIndex.vertex_index * 3) + 2]
            };
            glm::vec3 normal = {
                attrib.normals[meshIndex.normal_index * 3],
                attrib.normals[(meshIndex.normal_index * 3) + 1],
                attrib.normals[(meshIndex.normal_index * 3) + 2]
            };
            glm::vec2 textureCoordinate = {                         //These go unnused, but if you want textures, you will need them.
                attrib.texcoords[meshIndex.texcoord_index * 2],
                attrib.texcoords[(meshIndex.texcoord_index * 2) + 1]
            };

            vertices.push_back({ vertice, normal, textureCoordinate }); //We add our new vertice struct to our vector

        }
    }
    size = vertices.size();

    return vertices;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (currentX != camera.Position[0] || currentY != camera.Position[1] || currentZ != camera.Position[2])
        cout << "View position = Camera position -- X:" << camera.Position[0] << " Y:" << camera.Position[1] << " Z:" << camera.Position[2] << endl;

    // Adjust light position to get different reflected color from object
    if (((glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_X)) || (glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_Y)) || (glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_Z))) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            if (lightPosition[0] < 100) {
                lightPosition[0] = lightPosition[0] + 1;
            }
            else
            {
                lightPosition[0] = 100;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            if (lightPosition[1] < 100) {
                lightPosition[1] = lightPosition[1] + 1;
            }
            else
            {
                lightPosition[1] = 100;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            if (lightPosition[2] < 100) {
                lightPosition[2] = lightPosition[2] + 1;
            }
            else
            {
                lightPosition[2] = 100;
            }
        }

        cout << "Light position -- X:" << lightPosition[0] << " Y:" << lightPosition[1] << " Z:" << lightPosition[2] << endl;
    }
    if (((glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_X)) || (glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_Y)) || (glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_Z))) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            if (lightPosition[0] >= -100 && lightPosition[0] <= 100) {
                lightPosition[0] = lightPosition[0] - 1;
            }
            else
            {
                lightPosition[0] = -100;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            if (lightPosition[1] >= -100 && lightPosition[1] <= 100) {
                lightPosition[1] = lightPosition[1] - 1;
            }
            else
            {
                lightPosition[1] = -100;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            if (lightPosition[2] >= -100 && lightPosition[2] <= 100) {
                lightPosition[2] = lightPosition[2] - 1;
            }
            else
            {
                lightPosition[2] = -100;
            }
        }

        cout << "Light position -- X:" << lightPosition[0] << " Y:" << lightPosition[1] << " Z:" << lightPosition[2] << endl;
    }

    // Adjust object color to get different reflected color from object
    if ((glfwGetKey(window, GLFW_KEY_1) || glfwGetKey(window, GLFW_KEY_2) || glfwGetKey(window, GLFW_KEY_3)) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (objectColor[0] < 1) {
                objectColor[0] = objectColor[0] + 0.005;
            }
            else
            {
                objectColor[0] = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (objectColor[1] < 1) {
                objectColor[1] = objectColor[1] + 0.005;
            }
            else
            {
                objectColor[1] = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            if (objectColor[2] < 1) {
                objectColor[2] = objectColor[2] + 0.005;
            }
            else
            {
                objectColor[2] = 1;
            }
        }

        cout << "Object color -- R:" << objectColor[0] << " G:" << objectColor[1] << " B:" << objectColor[2] << endl;
    }
    if ((glfwGetKey(window, GLFW_KEY_4) || glfwGetKey(window, GLFW_KEY_5) || glfwGetKey(window, GLFW_KEY_6)) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            if (objectColor[0] >= 0 && objectColor[0] <= 1) {
                objectColor[0] = objectColor[0] - 0.005;
            }
            else
            {
                objectColor[0] = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            if (objectColor[1] >= 0 && objectColor[1] <= 1) {
                objectColor[1] = objectColor[1] - 0.005;
            }
            else
            {
                objectColor[1] = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            if (objectColor[2] >= 0 && objectColor[2] <= 1) {
                objectColor[2] = objectColor[2] - 0.005;
            }
            else
            {
                objectColor[2] = 0;
            }
        }
        cout << "Object color -- R:" << objectColor[0] << " G:" << objectColor[1] << " B:" << objectColor[2] << endl;
    }
    
    // Adjust light color to get different reflected color from object
    if (((glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_R)) || (glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_G)) || (glfwGetKey(window, GLFW_KEY_UP) && glfwGetKey(window, GLFW_KEY_B))) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            if (lightColor1[0] < 1) {
                lightColor1[0] = lightColor1[0] + 0.005;
            }
            else
            {
                lightColor1[0] = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            if (lightColor1[1] < 1) {
                lightColor1[1] = lightColor1[1] + 0.005;
            }
            else
            {
                lightColor1[1] = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            if (lightColor1[2] < 1) {
                lightColor1[2] = lightColor1[2] + 0.005;
            }
            else
            {
                lightColor1[2] = 1;
            }
        }

        cout << "Light color -- R:" << lightColor1[0] << " G:" << lightColor1[1] << " B:" << lightColor1[2] << endl;
    }
    if (((glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_R)) || (glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_G)) || (glfwGetKey(window, GLFW_KEY_DOWN) && glfwGetKey(window, GLFW_KEY_B))) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            if (lightColor1[0] >= 0 && lightColor1[0] <= 1) {
                lightColor1[0] = lightColor1[0] - 0.005;
            }
            else
            {
                lightColor1[0] = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            if (lightColor1[1] >= 0 && lightColor1[1] <= 1) {
                lightColor1[1] = lightColor1[1] - 0.005;
            }
            else
            {
                lightColor1[1] = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            if (lightColor1[2] >= 0 && lightColor1[2] <= 1) {
                lightColor1[2] = lightColor1[2] - 0.005;
            }
            else
            {
                lightColor1[2] = 0;
            }
        }
        cout << "Light color -- R:" << lightColor1[0] << " G:" << lightColor1[1] << " B:" << lightColor1[2] << endl;
    }
}

void Light(const float time, Shader myshader, Camera cam, glm::vec3 lightColor)
{
    lightColor1 = lightColor;

    glm::vec3 objectColor1 = objectColor;
    glm::vec3 viewPosition1 = cam.Position;
    glm::vec3 lightPosition1 = lightPosition;

    myshader.SetUniform3f("objectColor", objectColor1);
    myshader.SetUniform3f("lightColor", lightColor1);
    myshader.SetUniform3f("lightPos", lightPosition1);
    myshader.SetUniform3f("viewPos", viewPosition1);
}


