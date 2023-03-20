/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- Alumno Peñaloza Lugo Tania Lizeth  ---------------*/
/*------------- No. de cuenta: 316013929 ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f;
//For rotate
float	anguloX = 0.0f,
		anguloY = 0.0f,
		anguloZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
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
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2023-2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 cuellosOp = glm::mat4(1.0f);	//nueva, inicia en el origen
	glm::mat4 brazoizq = glm::mat4(1.0f);
	glm::mat4 brazoder = glm::mat4(1.0f);
	glm::mat4 espada = glm::mat4(1.0f);
	glm::mat4 viewOp = glm::mat4(1.0f);	
	glm::mat4 piernaizq  = glm::mat4(1.0f);
	glm::mat4 piernader = glm::mat4(1.0f);	//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection
	glm::mat4 pechoModel2 = glm::mat4(1.0f);
	glm::mat4 cabezaModel2 = glm::mat4(1.0f);
	glm::mat4 pechoModel3 = glm::mat4(1.0f);
	glm::mat4 cuerpo3 = glm::mat4(1.0f);
	glm::mat4 cabezaModel3 = glm::mat4(1.0f);

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		glm::mat4 modelOp2 = glm::mat4(1.0f);
		glm::mat4 modelOp3 = glm::mat4(1.0f);
		//Use "view" in order to affect all 
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(anguloX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(anguloY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(anguloZ), glm::vec3(0.0f, 0.0f, 1.0f));
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model INICIA LA CONSTRUCCIÓN DEL MODELO
		// factor de escala = valor final / valor inicial = ?/1
		glBindVertexArray(VAO);
		modelOp = glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 5.0f, 1.0f)); //pecho origen el centro del pecho
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		cuellosOp = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.75f, 0.0f)); //
		modelOp = glm::scale(cuellosOp, glm::vec3(0.5f, 0.5f, 1.0f)); //cuello
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(cuellosOp, glm::vec3(0.0f, 1.5f, 0.0f)); //
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f)); //cabeza
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f)); //cinturon 
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f)); 
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		brazoder = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.75f, 2.25f, 0.0f)); //brazo der
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(brazoder, glm::vec3(0.25f, -2.0f, 0.0f)); //brazo der
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		espada = modelOp = glm::translate(brazoder, glm::vec3(0.25f, -3.9f, 0.0f)); //espada
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(espada, glm::vec3(0.55f, 0.0f, 0.0f)); //espada
		modelOp = glm::scale(modelOp, glm::vec3(0.3f, 0.6f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(espada, glm::vec3(2.5f, 0.0f, 0.0f)); //espada
		modelOp = glm::scale(modelOp, glm::vec3(3.5f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		brazoizq= modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, 2.25f, 0.0f)); //brazo izq
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(brazoizq, glm::vec3(-0.25f, -2.0f, 0.0f)); //brazo izq
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		piernader = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(1.25f, -5.25f, 0.0f)); //pierna der
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(piernader, glm::vec3(0.5f, -2.125f, 0.0f)); //pie der
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		piernaizq = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.25f, -5.25f, 0.0f)); //pierna izq
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp = glm::translate(piernaizq, glm::vec3(-0.5f, -2.125f, 0.0f)); //pie izq
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		///
		pechoModel2 = modelOp2 = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, 3.0f, 1.0f)); //pecho origen el centro del pecho
		modelOp2 = glm::scale(modelOp2, glm::vec3(3.0f, 4.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(0.0f, 2.25f, 0.0f)); //cuello negro
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cabezaModel2 = modelOp2 = glm::translate(pechoModel2, glm::vec3(0.0f, 3.5f, 0.0f)); //cabeza negro
		modelOp2 = glm::scale(modelOp2, glm::vec3(2.0f, 2.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(cabezaModel2, glm::vec3(0.5f, 0.25f, 0.1f)); //ojos
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.75f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(cabezaModel2, glm::vec3(0.5f, 0.25f, 0.2f)); //ojos
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.4f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.0f, 0.4f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(cabezaModel2, glm::vec3(-0.5f, 0.25f, 0.1f)); //ojos
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.75f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(cabezaModel2, glm::vec3(-0.5f, 0.25f, 0.2f)); //ojos
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.4f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.0f, 0.4f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(1.75f, 1.75f, 0.0f)); //brazo der
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.25f, 0.25f, 1.0f));
		myShader.setMat4("model", modelOp2);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(2.0f, -1.0f, 0.0f)); //brazo der
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.5f, 6.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(-1.75f, 1.75f, 0.0f)); //brazo izq
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.25f, 0.25f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(-2.0f, -1.0f, 0.0f)); //brazo izq
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.5f, 6.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(-1.0f, -5.5f, 0.0f)); //pierna 
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.5f, 7.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp2 = glm::translate(pechoModel2, glm::vec3(1.0f, -5.5f, 0.0f)); //pierna 
		modelOp2 = glm::scale(modelOp2, glm::vec3(0.5f, 7.0f, 1.0f));
		myShader.setMat4("model", modelOp2);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		/// FIGURA 3 
		pechoModel3 = modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f)); //pecho origen el centro del pecho
		modelOp3 = glm::scale(modelOp3, glm::vec3(3.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(pechoModel3, glm::vec3(0.0f, 2.0f, 0.1f)); //cuello
		modelOp3 = glm::scale(modelOp3, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.7f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cabezaModel3 = modelOp3 = glm::translate(pechoModel3, glm::vec3(0.0f, 3.0f, 0.0f)); //cabeza
		modelOp3 = glm::scale(modelOp3, glm::vec3(2.0f, 2.0f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.7f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cabezaModel3, glm::vec3(0.5f, 0.25f, 0.1f)); //ojos
		modelOp3 = glm::scale(modelOp3, glm::vec3(0.4f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cabezaModel3, glm::vec3(-0.5f, 0.25f, 0.1f)); //ojos
		modelOp3 = glm::scale(modelOp3, glm::vec3(0.4f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp3);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cabezaModel3, glm::vec3(0.0f, -0.25f, 0.1f)); //nariz
		modelOp3 = glm::scale(modelOp3, glm::vec3(0.4f, 0.4f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.3f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, 0.8f, 0.0f)); //brazo der
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.0f, 1.8f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-8.0f, -0.8f, 0.0f)); //brazo der
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.0f, 1.8f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.7f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, 0.8f, 0.0f)); //brazo izq
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.0f, 1.8f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f,1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, -0.8f, 0.0f)); //brazo izq
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.0f, 1.8f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.7f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		cuerpo3 = modelOp3 = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -2.0f, 0.0f)); //cuerpo
		modelOp3 = glm::scale(modelOp3, glm::vec3(3.0f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.1f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cuerpo3, glm::vec3(0.75f, -2.25f, 0.0f)); //pierna der
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.5f, 4.0f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.8f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cuerpo3, glm::vec3(0.75f, -4.5f, 0.0f)); //zapato der
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cuerpo3, glm::vec3(-0.75f, -2.25f, 0.0f)); //pierna izq
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.5f, 4.0f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 0.9f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		modelOp3 = glm::translate(cuerpo3, glm::vec3(-0.75f, -4.5f, 0.0f)); //zapato izq
		modelOp3 = glm::scale(modelOp3, glm::vec3(1.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp3);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.1f, 0.1f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
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
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		anguloX -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		anguloX += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		anguloY -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		anguloY += 0.5f;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		anguloZ -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		anguloZ += 0.5f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}