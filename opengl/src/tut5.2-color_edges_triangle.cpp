//https://learnopengl.com/#!Getting-started/Shaders

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
//#include <sstring>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int VERTEX_SIZE = 6; //x, y, z, r, g, b
const int TRIANG_POINTS = 3;

unsigned int getVertexIndex(int p1, int p2, std::map<std::string, int> map,
							int &next, float vertices[], const float orig_vertices[]) {
	std::map<std::string,int>::iterator it;
	std::stringstream convert;
	convert << p1 << "_" << p2;
	std::string key = convert.str();
	//std::string key = "pepe";
	it = map.find(key);
	if (it == map.end()) {
		vertices[VERTEX_SIZE*next] = (orig_vertices[VERTEX_SIZE*p1] + orig_vertices[VERTEX_SIZE*p2]) / 2; //x
		vertices[VERTEX_SIZE*next+1] = (orig_vertices[VERTEX_SIZE*p1+1] + orig_vertices[VERTEX_SIZE*p2+1]) / 2; //y
		vertices[VERTEX_SIZE*next+2] = (orig_vertices[VERTEX_SIZE*p1+2] + orig_vertices[VERTEX_SIZE*p2+2]) / 2; //z
		vertices[VERTEX_SIZE*next+3] = 0.5f; //r
		vertices[VERTEX_SIZE*next+4] = 0.5f; //g
		vertices[VERTEX_SIZE*next+5] = 0.5f; //b
		map[key] = next++;
	}

	return map[key];
}

void addLines(int p1, int p2, int midVertex, unsigned int indices[], int &lineIndex) {
	indices[lineIndex++] = p1; indices[lineIndex++] = midVertex; //p1 -> o
	indices[lineIndex++] = p2; indices[lineIndex++] = midVertex; //p2 -> o
}

void buildIndices(const float orig_vertices[],
		const unsigned int orig_indices[],
		float vertices[],
		unsigned int indices[],
		int cantVertices,
		int cantTriang) {

	//std::cout << "Inicio" << std::endl;
	//vertices originales
	for (int i = 0; i < cantVertices; i++) {
		for (int j = 0; j < VERTEX_SIZE; j++) {
			vertices[VERTEX_SIZE*i+j] = orig_vertices[VERTEX_SIZE*i+j];
			//std::cout << i << ", " << j << ": " << vertices[VERTEX_SIZE*i+j] << std::endl;;
		}
	}

	int vertexIndex = cantVertices;
	int lineIndex = 0;
	std::map<std::string, int> verticesMap;
	unsigned int midVertex;
	for (int i = 0; i < cantTriang; i++) {
		int p1 = orig_indices[TRIANG_POINTS*i];
		int p2 = orig_indices[TRIANG_POINTS*i+1];
		int p3 = orig_indices[TRIANG_POINTS*i+2];
		//p1 <-> p2
		midVertex = getVertexIndex(p1, p2, verticesMap, vertexIndex, vertices, orig_vertices);
		addLines(p1, p2, midVertex, indices, lineIndex);

		//p1 <-> p3
		midVertex = getVertexIndex(p1, p3, verticesMap, vertexIndex, vertices, orig_vertices);
		addLines(p1, p3, midVertex, indices, lineIndex);

		//p2 <-> p3
		midVertex = getVertexIndex(p2, p3, verticesMap, vertexIndex, vertices, orig_vertices);
		addLines(p2, p3, midVertex, indices, lineIndex);
	}
}

#define MAIN

#ifdef MAIN
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // link shaders
    Shader ourShader("/home/manuel/doctorado/opengl/opengltut/opengl/shaders/5.1_traiang_colored_edges.shader.vs",
    					"/home/manuel/doctorado/opengl/opengltut/opengl/shaders/5.1_traiang_colored_edges.shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float orig_vertices[] = {
        // positions         // colors
         -0.8f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // bottom right
         -0.6f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // bottom left
         -0.7f, 0.2f, 0.0f,  0.0f, 0.0f, 0.0f,   // top
		 //-0.7f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,   // replica
		 //-0.75f,0.1f, 0.0f,  0.0f, 1.0f, 0.0f,
		 //-0.65f,0.1f, 0.0f,  0.0f, 0.0f, 1.0f   // top
         -0.4f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  // bottom left
         -0.5f, 0.2f, 0.0f,  0.0f, 0.0f, 0.0f,  // bottom left
    };

    unsigned int orig_indices[] = {  // note that we start from 0!
        0, 1, 2,
        1, 3, 4,
        1, 2, 4,
        0, 2, 3
    };

    const unsigned int VERTICES = 5;
    const unsigned int TRIANG = 4;
    const unsigned int LINES_PER_TRIANG = 6;
    const unsigned int POINTS_PER_LINE = 2;

    float vertices[(TRIANG * TRIANG_POINTS + VERTICES) * VERTEX_SIZE];
    unsigned int indices[TRIANG * LINES_PER_TRIANG * POINTS_PER_LINE];

    buildIndices(orig_vertices,
    		orig_indices,
    		vertices,
    		indices, VERTICES, TRIANG);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to
    ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

/*
        float timeValue = glfwGetTime();
        vertices[10] = vertices[17] = sin(timeValue) / 2.0f + 0.5f;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
*/
        // render the triangle
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_LINES, TRIANG * LINES_PER_TRIANG * POINTS_PER_LINE, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

#endif

/*

void buildIndices(const float orig_vertices[],
		const unsigned int orig_indices[],
		float vertices[],
		unsigned int indices[],
		int cantVertices,
		int cantTriang) {
	//std::cout << "Inicio" << std::endl;
	for (int i = 0; i < cantVertices; i++) {
		for (int j = 0; j < VERTEX_SIZE; j++) {
			vertices[VERTEX_SIZE*i+j] = orig_vertices[VERTEX_SIZE*i+j];
			//std::cout << i << ", " << j << ": " << vertices[VERTEX_SIZE*i+j] << std::endl;;
		}
	}

	int j = cantVertices;
	int lineIndex = 0;
	for (int i = 0; i < cantTriang; i++) {
		int p1 = orig_indices[TRIANG_POINTS*i];
		int p2 = orig_indices[TRIANG_POINTS*i+1];
		int p3 = orig_indices[TRIANG_POINTS*i+2];
		//p1 <-> p2
		vertices[VERTEX_SIZE*j] = (orig_vertices[VERTEX_SIZE*p1] + orig_vertices[VERTEX_SIZE*p2]) / 2; //x
		vertices[VERTEX_SIZE*j+1] = (orig_vertices[VERTEX_SIZE*p1+1] + orig_vertices[VERTEX_SIZE*p2+1]) / 2; //y
		vertices[VERTEX_SIZE*j+2] = (orig_vertices[VERTEX_SIZE*p1+2] + orig_vertices[VERTEX_SIZE*p2+2]) / 2; //z
		vertices[VERTEX_SIZE*j+3] = 1.0f; //r
		vertices[VERTEX_SIZE*j+4] = 0.0f; //g
		vertices[VERTEX_SIZE*j+5] = 0.0f; //b
		// p1 <-> o <-> p2
		indices[lineIndex++] = p1; indices[lineIndex++] = j; //p1 -> o
		indices[lineIndex++] = p2; indices[lineIndex++] = j; //p2 -> o
		//std::cout << indices[lineIndex-4] << " <-> " << indices[lineIndex-3] << std::endl;
		//std::cout << indices[lineIndex-2] << " <-> " << indices[lineIndex-1] << std::endl;
		j++;
		//p1 <-> p3
		vertices[VERTEX_SIZE*j] = (orig_vertices[VERTEX_SIZE*p1] + orig_vertices[VERTEX_SIZE*p3]) / 2; //x
		vertices[VERTEX_SIZE*j+1] = (orig_vertices[VERTEX_SIZE*p1+1] + orig_vertices[VERTEX_SIZE*p3+1]) / 2; //y
		vertices[VERTEX_SIZE*j+2] = (orig_vertices[VERTEX_SIZE*p1+2] + orig_vertices[VERTEX_SIZE*p3+2]) / 2; //z
		vertices[VERTEX_SIZE*j+3] = 0.0f; //r
		vertices[VERTEX_SIZE*j+4] = 1.0f; //g
		vertices[VERTEX_SIZE*j+5] = 0.0f; //b
		// p1 <-> o <-> p3
		indices[lineIndex++] = p1; indices[lineIndex++] = j; //p1 -> o
		indices[lineIndex++] = p3; indices[lineIndex++] = j; //p3 -> o
		//std::cout << indices[lineIndex-4] << " <-> " << indices[lineIndex-3] << std::endl;
		//std::cout << indices[lineIndex-2] << " <-> " << indices[lineIndex-1] << std::endl;
		j++;
		//p2 <-> p3
		vertices[VERTEX_SIZE*j] = (orig_vertices[VERTEX_SIZE*p2] + orig_vertices[VERTEX_SIZE*p3]) / 2; //x
		vertices[VERTEX_SIZE*j+1] = (orig_vertices[VERTEX_SIZE*p2+1] + orig_vertices[VERTEX_SIZE*p3+1]) / 2; //y
		vertices[VERTEX_SIZE*j+2] = (orig_vertices[VERTEX_SIZE*p2+2] + orig_vertices[VERTEX_SIZE*p3+2]) / 2; //z
		vertices[VERTEX_SIZE*j+3] = 0.0f; //r
		vertices[VERTEX_SIZE*j+4] = 0.0f; //g
		vertices[VERTEX_SIZE*j+5] = 1.0f; //b
		// p2 <-> o <-> p3
		indices[lineIndex++] = p2; indices[lineIndex++] = j; //p1 -> o
		indices[lineIndex++] = p3; indices[lineIndex++] = j; //p3 -> o
		//std::cout << indices[lineIndex-4] << " <-> " << indices[lineIndex-3] << std::endl;
		//std::cout << indices[lineIndex-2] << " <-> " << indices[lineIndex-1] << std::endl;
		j++;
	}
}

 */

