#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<vector>
#include "camera.h"
// settings
const GLuint width = 1000;
const GLuint height = 1000;

// camera
static Camera camera(glm::vec3(0.00000081595f, 0.0f, 6.5f));
static float lastX = width / 2.0f;
static float lastY = height / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f;	// time between current frame and last frame
static float lastFrame = 0.0f;

void Light(const float time, const GLuint shaderprogram);
