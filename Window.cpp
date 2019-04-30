#include "window.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
Geometry * teapot,* bunny;
GLint shaderProgram, toonShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shaders/shader.vert"
#define FRAGMENT_SHADER_PATH "../shaders/shader.frag"

#define VERTEX_TOON_PATH "../shaders/toonshader.vert"
#define FRAGMENT_TOON_PATH "../shaders/toonshader.frag"


#define TEAPOT_PATH "../models/teapot.obj"

#define BUNNY_PATH "../models/bunny.obj"
// Default camera parameters
glm::vec3 Window::cam_pos = { 0.0f, 20.0f, 5.0f };		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 0.0f, -1.0f);			// up | What orientation "up" is

float farPlane = 100.0f;
float nearPlane = 1.0f;
int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	cube = new Cube();
	teapot = new Geometry(TEAPOT_PATH, { 1.0f,0.7f,0.5f }, { 0.0f,0.0f,0.0f },true);
	bunny = new Geometry(BUNNY_PATH, { 0.8f,0.7f,0.8f }, { 5.0f,0.0f,5.0f },false);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	toonShaderProgram = LoadShaders(VERTEX_TOON_PATH, FRAGMENT_TOON_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(teapot);
	delete(bunny);
	glDeleteProgram(shaderProgram);
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
		P = glm::perspective(45.0f, (float)width / (float)height, nearPlane, farPlane);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
	teapot->update();
	bunny->update();
	teapot->isColliding(bunny);
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	cube->draw(shaderProgram);
	glUseProgram(toonShaderProgram);
	teapot->draw(toonShaderProgram);
	bunny->draw(toonShaderProgram);
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

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		//printf("Cursor Position at %f: %f \n", xpos, ypos);
		glm::vec3 new_dest = viewToWorldCoordTransform(xpos, ypos);
		teapot->setDestination(new_dest);
		float angle = glm::acos(glm::dot(glm::normalize(new_dest - teapot->currentPos), teapot->currentOri));
		//printf("rotate angle = %f \n", angle);
		glm::vec3 axis = glm::cross(teapot->currentOri, glm::normalize(new_dest - teapot->currentPos));
		teapot->rotate(angle, axis);
		//cam_pos += glm::vec3(new_dest.z, new_dest.y, new_dest.x);
		//cam_look_at += glm::vec3(new_dest.z, new_dest.y, new_dest.x);
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		//teapot->move();

		//std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
	}
}

// SCREEN SPACE: mouse_x and mouse_y are screen space
glm::vec3 Window::viewToWorldCoordTransform(int mouse_x, int mouse_y) {
	// NORMALISED DEVICE SPACE
	double x = 2.0 * mouse_x / Window::width -1;
	double y = 2.0 * mouse_y / Window::height - 1;
	//printf("normalized cursor to: %f %f \n", x,-y);
	// HOMOGENEOUS SPACE
	//glm::vec4 screenPos = glm::vec4(x, -y, -1.0f, 1.0f);
	double depth = (farPlane + nearPlane) / (farPlane-nearPlane) + (1 / pow(pow(cam_pos.y,2) + pow(cam_pos.z,2), 0.5) * (-2 * farPlane)) / (farPlane - nearPlane);
	glm::vec4 screenPos = glm::vec4(x, -y, depth, 1.0f);

	// Projection/Eye Space
	glm::mat4 ProjectView = Window::P * Window::V;
	glm::mat4 viewProjectionInverse = inverse(ProjectView);

	glm::vec4 worldPos = viewProjectionInverse * screenPos;
	//printf("world pos map to: %f %f %f\n", worldPos.x, worldPos.y, worldPos.z);
	glm::vec3 realPos = glm::vec3(worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w);
	printf("world pos remap to: %f %f %f\n", realPos.x, realPos.y, realPos.z);

	glm::vec3 dir = glm::normalize(realPos - cam_pos);
	float n = -cam_pos.y / dir.y;
	realPos.x = cam_pos.x + n * dir.x;
	realPos.y = 0;
	realPos.z = cam_pos.z + n * dir.z;


	return realPos;
	//return glm::vec3(worldPos.z, 0, worldPos.x);
}