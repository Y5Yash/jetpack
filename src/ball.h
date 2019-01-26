#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    Ball(float x, float y, float z, float width, float height, float depth, color_t color);
    // Ball(float x, float y , float z, float vertexarr[], int tcount, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void update(glm::mat4 view);
    void set_position(float x, float y, float z);
    void set_position(float x, float y, float z,float width, float height, float depth);
    void tick();
    bool detect_collision(Ball* a);
    double speed;
    bounding_box_t bound;
private:
    VAO *object;
};

class Barry
{
public:
	Barry(){}
	Barry(float x, float y);
	glm::vec3 location,eyescale,jetscale;
	void draw(glm::mat4 VP);
	void updatebound(glm::mat4 view);
	Ball skull;
	Ball thorax;
	Ball jetpack;
	Ball eye1;
	Ball eye2;
	Ball* ballarr[5];
	// float jet;
};


#endif // BALL_H
