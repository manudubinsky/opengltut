#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

int main() {
    string vertexPath = "/home/manuel/doctorado/eclipse/opengl/opengl/shaders/3.3.shader.vs";
    string vertexCode;
    std::ifstream vShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath.c_str());
        std::stringstream vShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    cout << vertexCode << endl;
	return 0;
}




