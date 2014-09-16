#include "Application.h"
#include <SDL.h>
#include <gl\glew.h>

#include <iostream>
#include <stdio.h>
#include "SamplesHelper.h"

namespace
{
	/* A simple function that prints a message, the error code returned by SDL,
	* and quits the application */
	void sdldie(const char *msg)
	{
		printf("%s: %s\n", msg, SDL_GetError());
		SDL_Quit();
		exit(1);
	}


	void checkSDLError(int line = -1)
	{
#ifndef NDEBUG
		const char *error = SDL_GetError();
		if (*error != '\0')
		{
			printf("SDL Error: %s\n", error);
			if (line != -1)
				printf(" + line: %i\n", line);
			SDL_ClearError();
		}
#endif
	}

}

class Application::ApplicationImpl
{
public:
	ApplicationImpl();
	~ApplicationImpl();

	SDL_Window* mWindow;

	SDL_GLContext mMainContext;

	bool shouldClose;

	int glMajorVersion;
	int glMinorVersion;
};

Application::ApplicationImpl::ApplicationImpl()
	: mWindow(nullptr)
	, mMainContext(nullptr)
	, shouldClose(false)
{

}

Application::ApplicationImpl::~ApplicationImpl()
{

}

Application::Application()
{
	mImpl = new ApplicationImpl();
}

Application::~Application()
{
	destroy();
	delete mImpl;
}

void Application::init()
{
	SamplesHelper::setupTheCurrentDirectoryToAssets();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
	{
		sdldie("Unable to initialize SDL"); /* Or die on error */
	}

	/* Turn on double buffering with a 24bit Z buffer.
	* You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void Application::createWindow(const std::string& title, int width, int height)
{
	mImpl->mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
}

bool Application::isWindowCreated() const
{
	return mImpl->mWindow != nullptr;
}

void Application::setGLVersion(int major, int minor)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

	mImpl->glMajorVersion = major;
	mImpl->glMinorVersion = minor;
}

void Application::setWindowTitle(const std::string& title)
{
	SDL_assert(mImpl);
	SDL_assert(mImpl->mWindow);
	SDL_SetWindowTitle(mImpl->mWindow, title.c_str());
}

#ifdef WIN32
#define snprintf _snprintf
#endif

bool Application::initGL()
{
	mImpl->mMainContext = SDL_GL_CreateContext(mImpl->mWindow);

	// Load the OpenGL functions.
	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}

	char versionStr[128];
	snprintf(versionStr, 127, "GL_VERSION_%d_%d", mImpl->glMajorVersion, mImpl->glMinorVersion);

	if (!glewIsSupported(versionStr))
	{
		std::cerr << "Error: OpenGL version " << mImpl->glMajorVersion << "." << mImpl->glMinorVersion << " not supported" << std::endl;
		return false;
	}

	//print information on screen
	std::cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "\tVendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "\tRenderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "\tVersion: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	int width, height;
	SDL_GetWindowSize(mImpl->mWindow, &width, &height);

	glViewport(0, 0, width, height);

	return true;
}

void Application::destroy()
{
	SDL_assert(mImpl);
	SDL_assert(mImpl->mWindow);
	SDL_assert(mImpl->mMainContext);

	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(mImpl->mMainContext);
	SDL_DestroyWindow(mImpl->mWindow);
}

void Application::run()
{
	onBeforeRun();
	while (1)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				goto shouldEnd;
			}
		}

		onPreRender();
		onRender();
		onPostRender();
	}

shouldEnd:
	mImpl->shouldClose = true;
}

void Application::onPreRender()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::onRender()
{

}

void Application::onPostRender()
{
	SDL_GL_SwapWindow(mImpl->mWindow);
}

void Application::setVerticalSync(bool sync)
{
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(sync ? 1 : 0);
}
