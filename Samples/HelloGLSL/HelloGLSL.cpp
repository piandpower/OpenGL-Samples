#include "framework.h"
#include "HelloGLSL.h"

const std::string g_programName = "HelloGLSL";

const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"}\n"
	);

const std::string strFragmentShader(
	"#version 330\n"
	"layout(location = 0) out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
	);

GLSLProgram glslProgram;

const float vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;

HelloGLSL::HelloGLSL()
{

}

HelloGLSL::~HelloGLSL()
{

}

void HelloGLSL::init()
{
	Application::init();
}

void HelloGLSL::onRender()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glslProgram.use();

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glslProgram.unUse();
	glBindVertexArray(0);
}

void HelloGLSL::onPreRender()
{
}

void HelloGLSL::onPostRender()
{
	Application::onPostRender();
}

void HelloGLSL::onBeforeRun()
{
	try
	{
		glslProgram.loadShader(GLSLShaderType::VERTEX, g_programName + "/" + "HelloGLSL.vert");
		glslProgram.loadShader(GLSLShaderType::FRAGMENT, g_programName + "/" + "HelloGLSL.frag");
	}
	catch (GLSLProgramException& e)
	{
		printf("%s\n", e.what());
		system("pause");
		exit(EXIT_FAILURE);
	}

	glslProgram.link();

	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint positionLoc = glslProgram.getAttributeLocation("position");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* Our program's entry point */
int main(int argc, char *argv[])
{
	bool result;
	HelloGLSL app;

	app.init();
	app.createWindow("HelloGLSL", 640, 480);
	app.setGLVersion(3, 3);
	result = app.initGL();

	if (!result)
		return -1;

	app.run();

	return 0;
}
