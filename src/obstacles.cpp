#include "main.h"
#include "obstacles.h"

Obstacles::Obstacles(float x, float y, float z, float width, float height, float depth, color_t color)
{
	this->location = glm::vec3(x,y,z);
	this->box = Ball(x,y, z, width, height, depth, color);
	// this->box.position = this->location;
	this->scale = glm::vec3(width/2,height/2,depth/2);
}
void Obstacles::draw(glm::mat4 VP)
{
    glm::mat4 translate = glm::translate (this->location);   // glTranslatef
    box.draw(VP*translate*glm::scale(this->scale));
}

void Obstacles::updatebound(glm::mat4 view)
{
	this->box.update(view * (glm::translate (this->location)));
}




Pentatope::Pentatope(float x, float y, float z, color_t color)
{
    GLfloat vertexarr[] = {
         1.0f,  1.0f,    1.0f,
        -1.0f,  1.0f,   -1.0f,
        -1.0f,  -1.0f,  1.0f,
         1.0f,  1.0f,   1.0f,
        -1.0f,  1.0f,   -1.0f,
        1.0f,   -1.0f,  -1.0f,
        1.0f,   1.0f,   1.0f,
        -1.0f,  1.0f,   -1.0f,
        1.0f,   -1.0f,  -1.0f,
        -1.0f,  -1.0f,  1.0f,
        1.0f,   -1.0f,  -1.0f,
        -1.0f,  1.0f,   -1.0f,
    };
    this->position = glm::vec3(x,y,z);
    this->rotation = 1;
    // this->coin = Ball(x,y,0,vertexarr,4, lightgreen);
    this->object = create3DObject(GL_TRIANGLES, 4*3, vertexarr, color, GL_FILL);
    
}

void Pentatope::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    // glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(2, 1, 3));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

int Pentatope::collect(glm::vec3 position)
{
	if ((position.x == this->position.x) && (position.y == this->position.y) &&(position.z == this->position.z))
	{
		this->position.y += 10;
		return 1;
	}
	else return 0;
}

int Pentatope::collect(bounding_box_t bound)
{
	if (( bound.width/2 >= abs(bound.coordinates.x - this->position.x)) && (bound.height/2 >= abs(bound.coordinates.y - this->position.y)) && (bound.depth/2 >= (bound.coordinates.z - this->position.z)))
	{
		// cout << "collected a coin" << endl;
		this->position.y += 1;
		return 1;
	}
	else return 0;
}






Firebeam::Firebeam(float x, float y)
{
	this->beam = Ball(x,y, 0, 30, 0.5f,0.5f, lightred);
	this->location = glm::vec3(x,y,0);
	this->scale = glm::vec3(30,0.5f,0.5f);
	this->rotation = 0;
}
Firebeam::Firebeam(float x, float y, float width)
{
	this->beam = Ball(x,y, 0, width, 0.5f,0.5f, lightred);
	this->location = glm::vec3(x,y,0);
	this->scale = glm::vec3(width,0.5f,0.5f);
	this->rotation = 0;
}
void Firebeam::draw(glm::mat4 VP)
{
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0,0,1));
	beam.draw(VP*glm::translate(this->location)*glm::scale(this->scale));
}
void Firebeam::draw1(glm::mat4 VP)
{
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0,0,1));
	beam.draw(VP*glm::translate(this->location)*glm::scale(this->scale)*rotate);
}
void Firebeam::draw2(glm::mat4 VP)
{
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0,0,1));
	beam.draw(VP*glm::translate(this->location)*rotate*glm::scale(this->scale));
}