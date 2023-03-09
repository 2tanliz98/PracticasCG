/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Tania Lizeth Peñaloza Lugo 316013929 ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;

//shader de vértices:
//primera etapa que recibe los datos que vamos a utilizar, se encarga de formar la geometría
//entrada en 0, recibe 3 datos y la guarda en aPos
//los 3 datos recibidos los guarda en un vector de 4 (el 4to es 1)                           
static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);						\n\
	ourColor = aColor;														\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderYellow = "								\n\
#version 330																\n\
in vec3 ourColor;																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(ourColor.x, ourColor.y, ourColor.z);					\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	float vertices[] = 
	{
		// positions         // x, y, z valores entre -1 a 1
		//verde
		-0.6f,  -0.8f, 0.0f,	0.0f, 1.0f, 0.0f,//0
		-0.2f,  -0.8f, 0.0f,	0.0f, 1.0f, 0.0f, //1
		-0.2f,  0.2f, 0.0f,		0.0f, 1.0f, 0.0f,//2

		//amarillo
		0.2f,  0.2f, 0.0f,		1.0f, 1.0f, 0.0f,//3
		0.2f, 0.4f, 0.0f,		1.0f, 1.0f, 0.0f,//4
		-0.2f, 0.4f, 0.0f,		1.0f, 1.0f, 0.0f,//5
		-0.2f, 0.6f, 0.0f,		1.0f, 1.0f, 0.0f,//6
		0.4f, 0.6f, 0.0f,		1.0f, 1.0f, 0.0f,//7

		//rojo
		0.4f, 0.8f, 0.0f,		1.0f, 0.0f, 0.0f,//8
		-0.6f, 0.8f, 0.0f,		1.0f, 0.0f, 0.0f,//9

		-0.2f,  -0.8f, 0.0f,	1.0f, 1.0f, 0.0f, //1 10 amarillo
		-0.2f, 0.4f, 0.0f,		1.0f, 1.0f, 0.0f,//5 11 amarillo
		-0.6f, 0.8f, 0.0f,		1.0f, 1.0f, 0.0f,//9 12 amarillo
		-0.2f, 0.4f, 0.0f,		1.0f, 0.0f, 0.0f,//5 13 rojo
		-0.2f, 0.6f, 0.0f,		1.0f, 0.0f, 0.0f,//6 14 rojo
		-0.6f,  -0.8f, 0.0f,	1.0f, 0.0f, 0.0f,//0 15 rojo

	};

	unsigned int indices[] =
	{
		6,8,9,	//rojo
		15,13,14, //rojo
		9,14,15, //rojo
		0,1,5, //verde
		6,7,8, //amarillos
		2,3,5,
		5,4,3,
	};

	glGenVertexArrays(2, VAO);	//contenedores de información en memoria de video
	glGenBuffers(2, VBO);	//espacio para operaciones
	glGenBuffers(2, EBO);	//



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//separación
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(9 * sizeof(float)));

	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	// Obtiene la resolución en pixeles del monitor principal ancho x altura
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);		//posición al ejecutar la aplicación
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//My Functions
	//Setup Data to use
	myData();
	//To Setup Shaders
	setupShaders();
    
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Display Section
		glUseProgram(shaderProgramYellow);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(10.0);
		//OPCION DE DIBUJO A OCUPAR, CUÁNTOS ÍNDICES VALORES, ENTEROS SIN SIGNO, INICIA DESDE EL VALOR 0
		glDrawElements(GL_TRIANGLES, 21, GL_UNSIGNED_INT, 0); 
		//gl_points = puntos, 0 = desde qué elemento comenzará a dibujar, 1 = cuántos elementos va a dibujar
		//GL_LINES = dibuja lineas independientes, con cada dos vértices
		//GL_LINE_STRIP = segmentos de línea consecutivos
		//GL_LINE_LOOP = figuras cerradas
		//GL_TRIANGLES = crea figuras con borde y relleno
		//GL_TRIANGLE_STRIP = de 3 a tres triángulos
		//GL_TRIANGLE_FAN = DIBUJO MUCHAS  lineas
		// dibujo de letra F
		//glDrawArrays(GL_TRIANGLE_FAN, 6, 4);
		//glDrawArrays(GL_TRIANGLE_FAN, 2, 4);
		//glDrawArrays(GL_TRIANGLE_FAN, 10, 6);



		glBindVertexArray(0);
		glUseProgram(0);

		//End of Display Section

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)++
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}