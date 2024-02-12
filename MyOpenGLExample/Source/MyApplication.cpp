#include <iostream>
#include <fstream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


/// <summary>
/// Cube definition
/// </summary>
const GLfloat m_cubeVertices[] = 
{
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
};

const GLuint m_numberOfCubeVertices = 8;
const GLushort m_cubeStrips[] = { 7,6,3,2,1,0,5,4,0,2,4,6,5,7,1,3 };
const GLuint m_numberOfCubeStrips = 8;

/// <summary>
/// Cube color per vertex
/// </summary>
const GLfloat m_cubeVertexColor[] = 
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};

/// <summary>
/// Proyection Matrix
/// </summary>
GLfloat m_proyectionMatrix[] = 
{ 
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f 
};

/// <summary>
/// View matrix
/// </summary>
GLfloat m_view[] =
{ 
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,-10.0f,1.0f 
};

/// <summary>
/// Model vector
/// </summary>
GLfloat m_model[] = { 1.0f,0.0f,0.0f,0.0f };
/// <summary>
/// Current angle
/// </summary>
GLfloat m_angle = 0.0f;

//Shaders
GLuint m_vertexShaderID = 0;
GLuint m_fragmentShaderID = 0;
GLuint m_programID = 0;

//Variables Uniform
GLint m_uniformTransparencyID = -1;
GLint m_uniformProyectionID = -1;
GLint m_uniformViewID = -1;
GLint m_uniformModelID = -1;

//Attributes
GLint m_inColorID = -1;
GLint m_inVertexID = -1;

//Vertex Array Object
GLuint m_vao;

//Vertex Buffer Object
GLuint m_vbo[3];

#define vbuffer m_vbo[0]
#define cbuffer m_vbo[1]
#define pbuffer m_vbo[2]

void DebugLog(const char* _log)
{
    std::cout << _log << std::endl;
}


void DebugLog(std::string _log)
{
    std::cout << _log << std::endl;
}

/// <summary>
/// Initialize the OpenGL libraries!
/// </summary>
/// <returns></returns>
int InitLibraries()
{
    if (!glfwInit())
        return -1;

    return 0;
}

/// <summary>
/// Initialize Extensions: GLEW library!
/// </summary>
/// <returns></returns>
int InitGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    return 0;
}

/// <summary>
/// Create the window and the context for OpenGL
/// </summary>
/// <param name="_title"></param>
/// <param name="_width"></param>
/// <param name="_height"></param>
/// <returns></returns>

GLFWwindow * InitWindowContext(const char * _title, int _width, int _height)
{
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow * _window = glfwCreateWindow(_width, _height, _title, NULL, NULL);
    if (!_window)
    {
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    return _window;
}

/// <summary>
/// We build our projection matrix based on our camera setup
/// </summary>
/// <param name="fov"></param>
/// <param name="ratio"></param>
/// <param name="nearPlane"></param>
/// <param name="farPlane"></param>
void BuildProjectionMatrix(float fov, float ratio, float nearPlane, float farPlane)
{
    float f = 1.0f / tan(fov * (3.141599f / 360.0f));

    m_proyectionMatrix[0] = f / ratio;
    m_proyectionMatrix[1 * 4 + 1] = f;
    m_proyectionMatrix[2 * 4 + 2] = (farPlane + nearPlane) / (nearPlane - farPlane);
    m_proyectionMatrix[3 * 4 + 2] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
    m_proyectionMatrix[2 * 4 + 3] = -1.0f;
    m_proyectionMatrix[3 * 4 + 3] = 0.0f;
}

/// <summary>
/// If we want to reescale the viewport, we can call it from funcs, but remember GLFW doesn't work with callbacks
/// </summary>
/// <param name="_window"></param>
/// <param name="w"></param>
/// <param name="h"></param>
void WindowRescaling(GLFWwindow* _window, GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    BuildProjectionMatrix(45.0f, h / w, 0.1f, 50.0f);
}

/// <summary>
/// Let's add some rotation to our model
/// </summary>
/// <param name="_window"></param>
void IdleMovement(GLFWwindow* _window)
{
    m_angle = (m_angle < 3.141599f * 2.0f) ? m_angle + 0.003f : 0.0f;
    m_model[0] = (GLfloat)((1.0f / sqrt(2.0f)) * sin((float)m_angle / 2.0f));
    m_model[1] = (GLfloat)((1.0f / sqrt(2.0f)) * sin((float)m_angle / 2.0f));
    m_model[2] = (GLfloat)0.0f;
    m_model[3] = (GLfloat)cos((float)m_angle / 2.0f);
}

/// <summary>
/// Generic method to check keyboard key press
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <returns></returns>
bool IsKeyPressed(GLFWwindow* window, int key)
{
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

/// <summary>
/// Repaint of our scene (only render the vertices if we are using the shaders to avoid crashes with the program)
/// </summary>
/// <param name="_window"></param>
/// <param name="_loadedShaders"></param>
void Repaint(GLFWwindow * _window, bool _loadedShaders)
{
    /* Clear last frame */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_loadedShaders)
    {
        glUseProgram(m_programID);
        glUniform1f(m_uniformTransparencyID, 1.0f);
        glUniform4fv(m_uniformModelID, 1, m_model);
        glUniformMatrix4fv(m_uniformViewID, 1, GL_FALSE, m_view);
        glUniformMatrix4fv(m_uniformProyectionID, 1, GL_FALSE, m_proyectionMatrix);

        /*Paint the buffer */
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLE_STRIP, m_numberOfCubeStrips, GL_UNSIGNED_SHORT, (void*)0);
        glDrawElements(GL_TRIANGLE_STRIP, m_numberOfCubeStrips, GL_UNSIGNED_SHORT, (void*)(m_numberOfCubeStrips * sizeof(GLushort)));
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);
}

/// <summary>
/// GLFW loop check
/// </summary>
/// <param name="_window"></param>
/// <returns></returns>
bool IsApplicationRunning(GLFWwindow* _window)
{
    return !glfwWindowShouldClose(_window);
}

/// <summary>
/// Manage key events
/// </summary>
/// <param name="_window"></param>
void ManageEvents(GLFWwindow* _window)
{
    if (IsKeyPressed(_window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(_window, true);


    /* Poll for and process events */
    glfwPollEvents();
}

/// <summary>
/// Load the shader from file. We could add the string directly but w/e
/// </summary>
/// <param name="_fileName"></param>
/// <param name="_type"></param>
/// <returns></returns>
GLuint LoadShader(const char * _fileName, GLenum _type)
{
    /* We load the shader */
    std::ifstream file;
    file.open(_fileName, std::ios::in);
    
    if (!file)
    {
        DebugLog("Shader file not found " + std::string(_fileName));
        return 0;
    }

    file.seekg(0, std::ios::end);
    unsigned int fileLen = file.tellg();
    file.seekg(std::ios::beg);
    
    //Se lee el fichero
    char * source = new char[fileLen + 1];
    int i = 0;
    
    while (file.good())
    {
        source[i] = file.get();
        if (!file.eof()) i++;
        else fileLen = i;
    }
    source[fileLen] = '\0';
    file.close();

    // Creation and compilation of the shaders
    GLuint shader;
    shader = glCreateShader(_type);
    glShaderSource(shader, 1, (const GLchar**) &source, (const GLint*)&fileLen);
    glCompileShader(shader);
    delete[] source;

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&logLen);
        char* logString = new char[logLen];
        glGetShaderInfoLog(shader, logLen, NULL, logString);
        std::cout << "Error: " << logString << std::endl;
        delete[] logString;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/// <summary>
/// Initialization of the shaders 
/// </summary>
/// <returns></returns>
bool InitializeShaders()
{
    //We compile our vertex and fragment shaders
    m_vertexShaderID = LoadShader("Shaders/vshader.glsl", GL_VERTEX_SHADER);
    m_fragmentShaderID = LoadShader("Shaders/fshader.glsl", GL_FRAGMENT_SHADER);
    
    if (m_vertexShaderID == 0 || m_fragmentShaderID == 0)
        return false;

    //Link then to our program
    m_programID = glCreateProgram();
    
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);
    
    glBindAttribLocation(m_programID, 0, "inVertex");
    glBindAttribLocation(m_programID, 1, "inColor");
    glLinkProgram(m_programID);

    //Error debugging
    int linked;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
    
    if (!linked)
    {
        // Error msg length
        GLint logLen;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLen);
        char* logString = new char[logLen];
        glGetProgramInfoLog(m_programID, logLen, NULL, logString);
        std::cout << "Error: " << logString << std::endl;
        delete[] logString;
        glDeleteProgram(m_programID);
        m_programID = 0;
        return false;
    }

    //uniform variables
    m_uniformTransparencyID = glGetUniformLocation(m_programID, "transparency");
    m_uniformProyectionID = glGetUniformLocation(m_programID, "proy");
    m_uniformViewID = glGetUniformLocation(m_programID, "view");
    m_uniformModelID = glGetUniformLocation(m_programID, "rot");
    
    //Attributes
    m_inColorID = glGetAttribLocation(m_programID, "inColor");
    m_inVertexID = glGetAttribLocation(m_programID, "inVertex");

    return true;
}

/// <summary>
/// Initialization of the cube VBO and VAO
/// </summary>
void InitializeSceneObjects()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    BuildProjectionMatrix(45.0f, 4.0f / 3.0f, 0.1f, 50.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(3, m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices), m_cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(m_inVertexID, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_inVertexID);
    glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertexColor), m_cubeVertexColor, GL_STATIC_DRAW);
    glVertexAttribPointer(m_inColorID, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_inColorID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_cubeStrips), m_cubeStrips,GL_STATIC_DRAW);
}

/// <summary>
/// Free OpenGL libraries
/// </summary>
void FreeLibraries()
{
    glfwTerminate();
}

/// <summary>
/// Free buffers, shaders, program and ofc, OpenGL context
/// </summary>
/// <param name="_loadedShaders"></param>
void FreeResources(bool _loadedShaders)
{
    if (_loadedShaders)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(3, m_vbo);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_vao);
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
        glDeleteProgram(m_programID);
    }

    FreeLibraries();
}

/// <summary>
/// Our main ;)))
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int main(void)
{
    /* Initialize GLFW (OpenGL library) */
    if (InitLibraries() == -1)
        return -1;

    GLFWwindow * window = InitWindowContext("Hello World!", 640, 480);

    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }

    // Remember to initialize the extensions AFTER we initialize OpenGL context!
    if (InitGLEW() == -1)
        return -1;

    bool loadedShaders = InitializeShaders();

    if (loadedShaders)
        InitializeSceneObjects();

    /* Loop until the user closes the window */
    while (IsApplicationRunning(window))
    {
        IdleMovement(window);
        Repaint(window, loadedShaders);
        ManageEvents(window);
    }

    FreeResources(loadedShaders);

    return 0;
}