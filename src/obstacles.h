#include "main.h"
#include "ball.h"

#ifndef OBSTACLES_H
#define OBSTACLES_H

class Obstacles
{
public:
	Obstacles(){}
	Obstacles(float x, float y, float z, float width, float height, float depth, color_t color);
	glm::vec3 location;
	Ball box;
	void draw(glm::mat4 VP);
	void updatebound(glm::mat4 view);
	glm::vec3 scale;
};


class Pentatope
{
public:
	Pentatope(){}
	Pentatope(float x, float y, float z, color_t color);
	// Ball coin;
	glm::vec3 position;
	float rotation;
	void draw(glm::mat4 VP);
	int collect(bounding_box_t bound);
	int collect(glm::vec3 position);
private:
	VAO* object;
};


class Firebeam
{
public:
	Firebeam(){};
	Firebeam(float x, float y);
	Firebeam(float x, float y, float width);
	void updatebound(glm::mat4 view);
	void draw1(glm::mat4 VP);
	void draw2(glm::mat4 VP);
	void draw(glm::mat4 VP);
	Ball beam;
	glm::vec3 location;
	glm::vec3 scale;
	float rotation;
};


#endif 