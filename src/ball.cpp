#include "ball.h"
#include "main.h"

static const GLfloat vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    this->bound = bounding_box_t{glm::vec4(this->position,0), 2.f, 2.f, 2.f};
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

/*Ball::Ball(float x, float y , float z, float vertexarr[], int tcount, color_t color)
{
    this->position = glm::vec3(x,y,z);
    // this->object = create3DObject(GL_TRIANGLES, tcount*3, vertexarr[], color, GL_FILL);
}*/

Ball::Ball(float x, float y , float z, float width, float height, float depth, color_t color)
{
    this->position = glm::vec3(x,y,z);
    this->bound.coordinates = glm::vec4(this->position,0);
    this->bound.width =width;
    this->bound.height = height;
    this->bound.depth = depth;
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}


void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::update(glm::mat4 view)
{
    this->bound.coordinates = (view * glm::vec4(this->position,0));
}

void Ball::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Ball::tick() {
    this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bool Ball::detect_collision(Ball* a) {
    return ((abs(a->bound.coordinates.x - this->bound.coordinates.x) * 2 < (a->bound.width + this->bound.width)) &&
           (abs(a->bound.coordinates.y - this->bound.coordinates.y) * 2 < (a->bound.height + this->bound.height)) &&
           (abs(a->bound.coordinates.z - this->bound.coordinates.z) * 2 < (a->bound.depth + this->bound.depth)));
}



Barry::Barry(float x, float y)
{
    this->location = glm::vec3(x,y,0);
    // this->jet = 0.0f;
    this->eyescale = glm::vec3(0.25f,0.25f,0.25f);
    this->jetscale=glm::vec3(0.5f,0.5f,0.5f);
    this->skull = Ball(0,3, brown);
    this->thorax = Ball(0,0,COLOR_RED);
    this->jetpack = Ball(-2,0,slategrey);
    this->eye1 = Ball(3,3, bluecolor);
    this->eye2 = Ball(3,3, COLOR_GREEN);
    eye1.set_position(4,12,3);
    eye2.set_position(4,12,-3);
    // this->ballarr = {&(this->skull), &(this->thorax), &(this->jetpack), &(this->eye1), &(this->eye2)};
    this->ballarr[0] = &(this->skull);
    this->ballarr[1] = &(this->thorax);
    this->ballarr[2] = &(this->jetpack);
    this->ballarr[3] = &(this->eye1);
    this->ballarr[4] = &(this->eye2);
}

void Barry::draw(glm::mat4 VP)
{
    glm::mat4 myScalingMatrix = glm::scale(glm::vec3(2.0f,2.0f,2.0f));
    glm::mat4 translate = glm::translate (this->location);   // glTranslatef
    skull.draw(VP*translate);
    thorax.draw(VP*translate*myScalingMatrix);
    jetpack.draw(VP*translate*glm::scale(this->jetscale));
    eye1.draw(VP*translate*glm::scale(this->eyescale));
    eye2.draw(VP*translate*glm::scale(this->eyescale));
}

void Barry::updatebound(glm::mat4 view)
{
    glm::mat4 translate = glm::translate (this->location);   // glTranslatef
    for(int i=0;i<5;i++)
    {
        (this->ballarr[i])->update(view*translate); 
    }
}



