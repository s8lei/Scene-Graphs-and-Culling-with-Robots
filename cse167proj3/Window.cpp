#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
PointCloud * Window::bunnyPoints;
PointCloud * Window::bearPoints;
PointCloud * Window::dragonPoints;
PointCloud * Window::lightPoints;
PointCloud * Window::spotPoints;
Cube * Window::cube;

Transform * Window::translations[100];
Transform * Window::quarters[4];
BoundSphere * Window::spheres[4];
Transform * Window::cameraView;
Transform * Window::robot;
BoundSphere * Window::sphere;
Geometry * Window::body;
Transform * Window::thead;
Geometry * Window::head;
Transform * Window::tleye;
Transform * Window::treye;
Geometry * Window::robeye;
Transform * Window::tlarm;
Transform * Window::trarm;
Geometry * Window::limb;
Transform * Window::tlleg;
Transform * Window::trleg;

float Window::legAngle = 0.0f;
bool Window::walkDirection = true;

// The object currently displaying.
Object * Window::currentObj;

float Window::fov = 45.0f;
float Window::nearDist = 1.0f;
float Window::farDist = 1000.0f;
glm::vec3 Window::nc;
glm::vec3 Window::fc;
glm::vec3 Window::rightNormal;
glm::vec3 Window::leftNormal;
glm::vec3 Window::upNormal;
glm::vec3 Window::bottomNormal;
std::vector<glm::vec3> Window::planeN;
std::vector<float> Window::planeD;
int Window::visibleCount = 0;
int Window::prevCount = -1;
int Window::cullCount = 0;
bool Window::debug = false;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 10, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::vec3 Window::direction = center - eye; // camera direction vector

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::colorModeLoc;
GLuint Window::isLightLoc;
GLuint Window::lightPosLoc;
GLuint Window::isDirLightLoc;
GLuint Window::isPtLightLoc;
GLuint Window::isSptLightLoc;
GLuint Window::dirLightDirLoc;
GLuint Window::sptLightDirLoc;
GLuint Window::materialALoc;
GLuint Window::materialDLoc;
GLuint Window::materialSLoc;
GLuint Window::materialShLoc;
GLuint Window::cutoffLoc;

bool Window::colorMode;
bool Window::isPtLight = true;
bool Window::isDirLight = false;
bool Window::isSptLight = false;
int Window::keyMode = 0;

bool Window::rotate = false;
glm::vec3 Window::lastPos;
glm::vec3 Window::curPos;

glm::vec2 Window::last;
glm::vec2 Window::cur;
float Window::yaw = -90.0f;
float Window::pitch = 0.0f;

glm::vec3 Window::lightPos = glm::vec3(1.2f, 1.0f, 15.0f);
glm::vec3 Window::dirLightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
glm::vec3 Window::sptLightDir = glm::vec3(1.2f, 1.0f, 15.0f);
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shader.vert", "shader.frag");

    // Check the shader program.
    if (!program)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    // Activate the shader program.
    glUseProgram(program);
    // Get the locations of uniform variables.
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    modelLoc = glGetUniformLocation(program, "model");
    colorLoc = glGetUniformLocation(program, "color");
    colorModeLoc = glGetUniformLocation(program, "colorMode");
    isLightLoc = glGetUniformLocation(program, "isLight");
    isDirLightLoc = glGetUniformLocation(program, "isDirLight");
    isPtLightLoc = glGetUniformLocation(program, "isPtLight");
    isSptLightLoc = glGetUniformLocation(program, "isSptLight");
    lightPosLoc = glGetUniformLocation(program, "lightPos");
    dirLightDirLoc = glGetUniformLocation(program, "dirLightDir");
    sptLightDirLoc = glGetUniformLocation(program, "sptLightDir");
    materialALoc = glGetUniformLocation(program, "material.ambient");
    materialDLoc = glGetUniformLocation(program, "material.diffuse");
    materialSLoc = glGetUniformLocation(program, "material.specular");
    materialShLoc = glGetUniformLocation(program, "material.shininess");
    cutoffLoc = glGetUniformLocation(program, "cutoff");
    return true;
}

bool Window::initializeObjects()
{
    // Create a point cloud consisting of cube vertices.
//    bunnyPoints = new PointCloud("bunny.obj", 1);
//    bearPoints = new PointCloud("bear.obj", 1);
//    dragonPoints = new PointCloud("dragon.obj", 1);
//    // Set cube to be the first to display
//    currentObj = bunnyPoints;
//    lightPoints = new PointCloud("sphere.obj", 1);
//    spotPoints = new PointCloud("sphere.obj", 1);
//    spotPoints->ambient = glm::vec3(0.0, 0.0, 0.0);
//    spotPoints->translate(glm::vec3(-3.6f, -3.0f, 0.0f));
    cube = new Cube(5.0f);
    
    cameraView = new Transform();
    cameraView->M = view;
    robot = new Transform();
    
    sphere = new BoundSphere("sphere.obj", 1.95f, 1);
    body = new Geometry("body_s.obj", -1);
    thead = new Transform();
    head = new Geometry("head_s.obj", -1);
    tleye = new Transform();
    treye = new Transform();
    robeye = new Geometry("eyeball_s.obj", -1);
    tlarm = new Transform(0);
    trarm = new Transform(1);
    limb = new Geometry("limb_s.obj", -1);
    tlleg = new Transform(2);
    trleg = new Transform(3);
    robot->M = glm::mat4( 1.0f );
    thead->M = glm::translate(glm::vec3(0.0f, 0.95f, 0.0f)) * glm::mat4( 1.0f );
    treye->M = glm::translate(glm::vec3(-0.5f, 0.4f, 0.9f)) * glm::mat4( 1.0f );
    tleye->M = glm::translate(glm::vec3(0.5f, 0.4f, 0.9f)) * glm::mat4( 1.0f );
    trarm->M = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(315.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::mat4( 1.0f );
    tlarm->M = glm::translate(glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::mat4( 1.0f );
    trleg->M = glm::translate(glm::vec3(-0.7f, -1.0f, 0.0f)) * glm::mat4( 1.0f );
    tlleg->M = glm::translate(glm::vec3(0.7f, -1.0f, 0.0f)) * glm::mat4( 1.0f );
    
    //cameraView->addChild(robot);
    robot->addChild(sphere);
    
    sphere->addChild(body);
    sphere->addChild(thead);
    sphere->addChild(tlarm);
    sphere->addChild(trarm);
    sphere->addChild(tlleg);
    sphere->addChild(trleg);
    thead->addChild(head);
    thead->addChild(tleye);
    thead->addChild(treye);
    treye->addChild(robeye);
    tleye->addChild(robeye);
    tlarm->addChild(limb);
    trarm->addChild(limb);
    tlleg->addChild(limb);
    trleg->addChild(limb);
    
    int index = 0;
    float offset = 5.0f;
    for (int i = 0; i < 4; i++) {
        quarters[i] = new Transform();
        spheres[i] = new BoundSphere("sphere.obj", 9.5f, 0);
        quarters[i]->addChild(spheres[i]);
        cameraView->addChild(quarters[i]);
    }
    spheres[0]->M = glm::translate(glm::vec3(-3.0*3.0, 0.0f, -3.0*3.0));
    spheres[1]->M = glm::translate(glm::vec3(-3.0*3.0, 0.0f, 2.0*3.0));
    spheres[2]->M = glm::translate(glm::vec3(2.0*3.0, 0.0f, -3.0*3.0));
    spheres[3]->M = glm::translate(glm::vec3(2.0*3.0, 0.0f, 2.0*3.0));
    
    for(int y = -5; y < 5; y += 1)
    {
        for(int x = -5; x < 5; x += 1)
        {
            Transform * transform = new Transform();
            BoundSphere * usphere = spheres[(x+5)/5*2 + (y+5)/5];
            transform->M = glm::translate(glm::vec3((float)x*3.0, 0.0f, (float)y*3.0));
            transform->addChild(robot);
            usphere->addChild(transform);
            //std::cout<<(x+5)/5*2 + (y+5)/5<<std::endl;
            //cameraView->addChild(transform);
            translations[index++] = transform;
        }
    }
    for (int i = 0; i < 4; i++) {
        std::cout<<spheres[i]->children.size()<<std::endl;
    }
//    std::cout<<"thead M is "<<std::endl<<thead->M[0][0]<<" "<<thead->M[0][1]<<" "<<thead->M[0][2]<<" "<<thead->M[0][3]<<std::endl<<thead->M[1][0]<<" "<<thead->M[1][1]<<" "<<thead->M[1][2]<<" "<<thead->M[1][3]<<std::endl<<thead->M[2][0]<<" "<<thead->M[2][1]<<" "<<thead->M[2][2]<<" "<<thead->M[2][3]<<std::endl<<thead->M[3][0]<<" "<<thead->M[3][1]<<" "<<thead->M[3][2]<<" "<<thead->M[3][3];
    calcFrustum();
    
    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.
//    delete bunnyPoints;
//    delete bearPoints;
//    delete dragonPoints;
//    delete lightPoints;
    delete robot;
    delete body;
    delete thead;
    delete head;
    delete tleye;
    delete treye;
    delete robeye;
    delete tlarm;
    delete trarm;
    delete limb;
    delete tlleg;
    delete trleg;
    

    // Delete the shader program.
    glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(fov), (float)width / (float)height, nearDist, farDist);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.
    //currentObj->update();
    cube->update();
    trarm->update();
    tlarm->update();
    tlleg->update();
    trleg->update();
        
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
//    glm::mat4 model = currentObj->getModel();
//    glm::vec3 color = currentObj->getColor();
//    glm::vec3 ambient = ((PointCloud*)currentObj)->ambient;
//    glm::vec3 diffuse = ((PointCloud*)currentObj)->diffuse;
//    glm::vec3 specular = ((PointCloud*)currentObj)->specular;
//    GLfloat shininess = ((PointCloud*)currentObj)->shininess;
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(currentModel));
//    glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0, 1, 0)));
//    glUniform1i(colorModeLoc, colorMode);
//    glUniform1i(isLightLoc, false);
//    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
//    glUniform3fv(materialALoc, 1, glm::value_ptr(ambient));
//    glUniform3fv(materialDLoc, 1, glm::value_ptr(diffuse));
//    glUniform3fv(materialSLoc, 1, glm::value_ptr(specular));
//    glUniform1f(materialShLoc, shininess);
//    // Render the object.
//    currentObj->draw();
//    glm::mat4 lightModel = lightPoints->getModel();
////    lightModel = glm::scale(glm::vec3(0.05f)) * lightModel;
////    lightModel = glm::translate(lightPos) * lightModel;
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));
//    glUniform1i(isLightLoc, true);
//    glUniform3fv(materialALoc, 1, glm::value_ptr(lightPoints->ambient));
//    glUniform3fv(materialDLoc, 1, glm::value_ptr(lightPoints->diffuse));
//    glUniform3fv(materialSLoc, 1, glm::value_ptr(lightPoints->specular));
//    glUniform1f(materialShLoc, 0.0f);
//    lightPoints->draw();
//
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(spotPoints->getModel()));
//    glUniform1i(isLightLoc, true);
//    glUniform3fv(materialALoc, 1, glm::value_ptr(spotPoints->ambient));
//    glUniform3fv(materialDLoc, 1, glm::value_ptr(spotPoints->diffuse));
//    glUniform3fv(materialSLoc, 1, glm::value_ptr(spotPoints->specular));
//    glUniform1f(materialShLoc, 0.0f);
//    glUniform1f(cutoffLoc, glm::cos(glm::radians(12.5f)));
//    spotPoints->draw();
    
    
//    glUniform1i(isDirLightLoc, isDirLight);
//    glUniform1i(isPtLightLoc, isPtLight);
//    glUniform1i(isSptLightLoc, isSptLight);
//    glUniform3fv(dirLightDirLoc, 1, glm::value_ptr(dirLightDir));
//    glUniform3fv(sptLightDirLoc, 1, glm::value_ptr(sptLightDir));
    //currentModel = glm::scale(glm::vec3(5))*glm::mat4( 1.0f );
    
//    head->draw(glm::scale(glm::vec3(5))*glm::mat4( 1.0f ));
//    setModel(glm::scale(glm::vec3(5))*glm::mat4( 1.0f ));
//    arm->draw(glm::mat4( 1.0f ));

    //body->draw(glm::mat4( 1.0f ));
//    if (visibleCount<100)
//    {
//        for (int i = 0; i < 100; i++) {
//            translations[i]->draw(glm::mat4(1.0f));
//        }
//    }
    visibleCount = 0;
    cullCount = 0;
    
//    if (visibleCount<100)
    cameraView->draw(glm::mat4( 1.0f ));

//    for (int i = 0; i < 4; i++) {
//        quarters[i]->draw(glm::mat4(1.0f));
//    }
    //robot->draw(view*glm::mat4(1.0f));
    
    //thead->rotate(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    //larm->draw(glm::mat4(1.0f));
//    tlarm->rotate(glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
//    if (legAngle>=0.5f) {
//        walkDirection = false;
//        //std::cout<<"backward"<<std::endl;
//    }
//    else if (legAngle<=-0.5f) {
//        walkDirection = true;
//        //std::cout<<"forward"<<std::endl;
//    }
//    if (walkDirection) {
//        legAngle+=0.005f;
//        //tlleg->rotate(glm::radians(0.1f), glm::vec3(0, 1, 0));
//        //tlleg->translate(-glm::vec3(lleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//        tlleg->rotate(0.005f, glm::vec3(0.001, 0, 0));
//        tlarm->rotate(0.005f, glm::vec3(0, 0, 0.001));
//        //tlleg->translate(glm::vec3(lleg->M*glm::vec4(lleg->objCenter, 1.0f)));
////        trleg->translate(-glm::vec3(rleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//        trleg->rotate(0.005f, glm::vec3(-0.001, 0, 0));
//        trarm->rotate(0.005f, glm::vec3(0, 0, -0.001));
////        trleg->translate(glm::vec3(rleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//    }
//    else {
//        legAngle-=0.005f;
//        //tlleg->translate(-glm::vec3(lleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//        tlleg->rotate(0.005f, glm::vec3(-0.001, 0, 0));
//        tlarm->rotate(0.005f, glm::vec3(0, 0, -0.001));
//        //tlleg->translate(glm::vec3(lleg->M*glm::vec4(lleg->objCenter, 1.0f)));
////        trleg->translate(-glm::vec3(rleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//        trleg->rotate(0.005f, glm::vec3(0.001, 0, 0));
//        trarm->rotate(0.005f, glm::vec3(0, 0, 0.001));
////        trleg->translate(glm::vec3(rleg->M*glm::vec4(lleg->objCenter, 1.0f)));
//    }
    //std::cout<<"legangle is "<<legAngle<<std::endl;
    
//    glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view * sphere->getModel()));
//    sphere->draw();
    
    //tlleg->rotate(glm::radians(0.1f), glm::vec3(0, 1, 0));
//    glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f))
    // Gets events, including input such as keyboard and mouse or window resizing.
    
    std::string str = "number of visible robots: " + std::to_string(visibleCount) + " number of cullings: " + std::to_string(cullCount);
    windowTitle = str.c_str();
    glfwSetWindowTitle(window, windowTitle);
    if (prevCount != visibleCount)
    {
        std::cout<< "number of robots is "<< cameraView->children.size();
        std::cout<< "number of robots drawn is "<< visibleCount << std::endl;
        prevCount = visibleCount;
    }
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
     * TODO: Modify below to add your key callbacks.
     */
    
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_F1:
                // Set currentObj to bunny
                currentObj = bunnyPoints;
                break;
            case GLFW_KEY_F2:
                // Set currentObj to bear
                currentObj = bearPoints;
                break;
            case GLFW_KEY_F3:
                // Set currentObj to dragon
                currentObj = dragonPoints;
                break;
            case GLFW_KEY_P:
                if (action == GLFW_PRESS && mods == GLFW_MOD_SHIFT)
                {
                    currentObj->updatePointSize(currentObj->getPointSize()+0.1);
                }
                else
                {
                    currentObj->updatePointSize(currentObj->getPointSize()-0.1);
                }
                break;
            case GLFW_KEY_N:
                colorMode = !colorMode;
                break;
            case GLFW_KEY_1:
                keyMode = 1;
                break;
            case GLFW_KEY_2:
                keyMode = 2;
                break;
            case GLFW_KEY_3:
                keyMode = 3;
                break;
            case GLFW_KEY_4:
                isPtLight = !isPtLight;
                lightPoints->ambient = isPtLight? glm::vec3(1.0f, 0.5f, 0.31f): glm::vec3(0.0, 0.0, 0.0);
                std::cout<< "point light is "<<isPtLight;
                break;
            case GLFW_KEY_5:
                isDirLight = !isDirLight;
                std::cout<< "dir light is "<<isDirLight;
                break;
            case GLFW_KEY_6:
                isSptLight = !isSptLight;
                spotPoints->ambient = isSptLight? glm::vec3(1.0f, 0.5f, 0.31f): glm::vec3(0.0, 0.0, 0.0);
                std::cout<< "spot light is "<<isSptLight;
                break;
            case GLFW_KEY_7:
                keyMode = 7;
                break;
            case GLFW_KEY_8:
                keyMode = 8;
                break;
            case GLFW_KEY_R:
                for (int i = 0; i < 4; i++)
                {
                    spheres[i]->ifRender = !spheres[i]->ifRender;
                }
                sphere->ifRender = !sphere->ifRender;
                break;
            case GLFW_KEY_C:
                for (int i = 0; i < 4; i++)
                {
                    spheres[i]->cull = !spheres[i]->cull;
                }
                sphere->cull = !sphere->cull;
                if (sphere->cull)
                    calcFrustum();
                break;
            case GLFW_KEY_D:
            {
                debug = !debug;
                if ( !debug )
                    calcFrustum();
                break;
            }
            default:
                break;
        }
    }
}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
    glm::vec3 v;
    float  d;
    
    v.x = (2.0f * point.x - 640) / 640;
    v.y = (480 - 2.0f * point.y) / 480;
    v.z = 0.0f;
    
    d = glm::length(v);
    
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrt(1.001f - d*d);
    v = glm::normalize(v);
    return v;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    float offset = (yoffset > 0) ? 1+yoffset: 1/(1-yoffset);
    switch (keyMode) {
        case 0:
        {
            //((PointCloud*)currentObj)->scale(offset);
            //if(fov >= 1.0f && fov <= 45.0f)
              fov -= yoffset;
//            if(fov <= 1.0f)
//              fov = 1.0f;
//            if(fov >= 45.0f)
//              fov = 45.0f;
            projection = glm::perspective(glm::radians(fov), (float)width / (float)height, nearDist, farDist);
            if (!debug)
                calcFrustum();
            break;
        }
        case 1:
            //((PointCloud*)currentObj)->scale(offset);
//            projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 10.0f, 1000.0f);
            break;
        case 2:
        {
            glm::vec3 center = glm::vec3(currentObj->getModel()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 prePos = lightPos;
            lightPos = glm::vec3(center + offset* (lightPos - center));
            lightPoints -> translate(lightPos - prePos);
            break;
        }
        case 3:
        {
            ((PointCloud*)currentObj)->scale(offset);
            glm::vec3 center = glm::vec3(currentObj->getModel()*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 prePos = lightPos;
            lightPos = glm::vec3(center + offset* (lightPos - center));
            lightPoints -> translate(lightPos - prePos);
            break;
        }
        default:
            break;
    }
}
void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 cursor = glm::vec2(xpos, ypos);
    cur = cursor;
    curPos = trackBallMapping(cursor);
    glm::vec3 dir;
    float pixel_diff;
    float rot_angle, zoom_factor;
    
    if(rotate)
    {
        dir = curPos - lastPos;
        float velocity = dir.length();
        if (velocity > 0.0001)
        {
            glm::vec3 rotAxis;
            rotAxis = glm::cross(lastPos, curPos);
            rot_angle = asin(sqrt(glm::dot(rotAxis,rotAxis))/(sqrt(glm::dot(curPos,curPos))*sqrt(glm::dot(lastPos,lastPos))));
//            rot_angle = acos(glm::dot(lastPos, curPos)/(sqrt(glm::dot(curPos,curPos))*sqrt(glm::dot(lastPos,lastPos))));
            switch (keyMode) {
                case 0:
                {
                    float xoffset = cur.x - last.x;
                    float yoffset = last.y - cur.y; // reversed since y-coordinates range from bottom to top
                    last = cur;

                    float sensitivity = 0.05f;
                    xoffset *= sensitivity;
                    yoffset *= sensitivity;
                    
                    yaw   += xoffset;
                    pitch += yoffset;
                    
                    if(pitch > 89.0f)
                      pitch =  89.0f;
                    if(pitch < -89.0f)
                      pitch = -89.0f;
                    
                    glm::vec3 front;
                    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
                    front.y = sin(glm::radians(pitch));
                    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
                    
                    //direction = glm::normalize(front);
                    direction = glm::vec3(glm::rotate(rot_angle, rotAxis)*glm::vec4(direction, 0.0f));
                    //center -= glm::normalize(glm::cross(direction, up));
                    //center = glm::vec3(glm::rotate(rot_angle, rotAxis)*glm::vec4(center, 1.0f));
                    //up = glm::vec3(0.0f, 0.0f, 1.0f);
                    view = glm::lookAt(eye, eye+direction, up);
                    cameraView->M = glm::lookAt(eye, eye+direction, up);
                    calcFrustum();
                    //view = glm::rotate(rot_angle, rotAxis)*view;
                    //robot->rotate(rot_angle, rotAxis);
                    break;
                }
                    
                case 1:
                {
                    //((PointCloud*)currentObj)->rotate(rot_angle, rotAxis);
                        //tlleg->rotate(glm::radians(0.1f), glm::vec3(0, 1, 0));
//                        tlleg->translate(-glm::vec3(leg->M*glm::vec4(leg->objCenter, 1.0f)));
//                        tlleg->rotate(rot_angle, rotAxis);
//                        tlleg->translate(glm::vec3(leg->M*glm::vec4(leg->objCenter, 1.0f)));
//                        trleg->translate(-glm::vec3(leg->M*glm::vec4(leg->objCenter, 1.0f)));
//                        trleg->rotate(rot_angle, rotAxis);
//                        trleg->translate(glm::vec3(leg->M*glm::vec4(lleg->objCenter, 1.0f)));
                    
                        std::cout<<"angle is "<<rot_angle<<std::endl;
                    std::cout<<"rotAxis is"<<rotAxis.x<<" "<<rotAxis.y<<" "<<rotAxis.z<<std::endl;
                    
                    break;
                }
                case 2:
                {
                    lightPoints->rotate(rot_angle, rotAxis);
                    lightPos = lightPoints->getModel()*glm::vec4(lightPos, 1.0f);
                    //std::cout<<"light pos: "<<lightPos.x<<" "<<lightPos.y<<" "<<lightPos.z;
                    break;
                }
                case 3:
                {
                    ((PointCloud*)currentObj)->rotate(rot_angle, rotAxis);
                    lightPoints->rotate(rot_angle, rotAxis);
                    lightPos = lightPoints->getModel()*glm::vec4(lightPos, 1.0f);
                }
                case 7:
                {
                    dirLightDir = glm::vec3(glm::rotate(rot_angle, rotAxis)* glm::vec4(dirLightDir, 1.0f));
                }
                case 8:
                {
                    sptLightDir = glm::vec3(glm::rotate(rot_angle, rotAxis)* glm::vec4(sptLightDir, 1.0f));
                }
                default:
                    break;
            }
        }
    }
    lastPos = curPos;
    last = cur;
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            rotate = true;
            lastPos = curPos;
            last = cur;
        }
        else
            rotate = false;
    }
}

void Window::setModelView(glm::mat4 M)
{
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(M));
//    std::cout<<"currentModel M is "<<std::endl<<M[0][0]<<" "<<M[0][1]<<" "<<M[0][2]<<" "<<M[0][3]<<std::endl<<M[1][0]<<" "<<M[1][1]<<" "<<M[1][2]<<" "<<M[1][3]<<std::endl<<M[2][0]<<" "<<M[2][1]<<" "<<M[2][2]<<" "<<M[2][3]<<std::endl<<M[3][0]<<" "<<M[3][1]<<" "<<M[3][2]<<" "<<M[3][3];
}

void Window::setColor(glm::vec3 color)
{
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
}

void Window::calcFrustum()
{
    planeN.clear();
    planeD.clear();
    glm::vec3 d = glm::normalize(direction);
    float Hnear = glm::tan(glm::radians(fov/2.0f)) * nearDist * 2;
    float Wnear = Hnear/(float)height * (float)width;
    nc = eye + d*nearDist;
    planeN.push_back(d);
    planeD.push_back(glm::dot(d, nc));
    fc = eye + d*farDist;
    planeN.push_back(-d);
    planeD.push_back(glm::dot(-d, fc));
    glm::vec3 right = glm::cross(d, up);
    glm::vec3 a = (nc + right*(float)Wnear/2.0f) - eye;
    a = a/ sqrt(glm::dot(a, a));
    rightNormal = glm::cross(up, a);
    planeN.push_back(rightNormal);
    planeD.push_back(glm::dot(rightNormal, eye));
    a = eye - (nc - right*(float)Wnear/2.0f);
    a = a/ sqrt(glm::dot(a, a));
    leftNormal = glm::cross(up, a);
    planeN.push_back(leftNormal);
    planeD.push_back(glm::dot(leftNormal, eye));
    a = eye - (nc + up*(float)Hnear/2.0f);
    a = a/ sqrt(glm::dot(a, a));
    upNormal = glm::cross(right, a);
    planeN.push_back(upNormal);
    planeD.push_back(glm::dot(upNormal, eye));
    a = (nc - up*(float)Hnear/2.0f) - eye;
    a = a/ sqrt(glm::dot(a, a));
    bottomNormal = glm::cross(right, a);
    planeN.push_back(bottomNormal);
    planeD.push_back(glm::dot(bottomNormal, eye));
    for (int i = 0; i< 6; i ++)
    {
//        std::cout<< "plane" << i << " normal is " << planeN[i].x <<" "<< planeN[i].y <<" "<< planeN[i].z << std::endl;
    }
}
