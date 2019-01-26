#include "main.h"
#include "timer.h"
#include "ball.h"
#include "unistd.h"
#include "obstacles.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Ball ball2;
Barry streakfries;
Obstacles ground;
Obstacles ceiling;
Obstacles wall;
Obstacles* wallarr[2] = {&ground, &ceiling};
Pentatope coinarr[20];
Pentatope coin1;
Pentatope coin2;
Firebeam farr[5];
Firebeam rod[5];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
double xspeed = 0.5f;
double bspeed = 0.0f;
double byspeed = 0.0f;
double yspeed = 0;
float colltime = 0;
bool fall = false;
float zoom = 75;
bool sctog = false;
int score =0;
glm::vec3 test1;
bounding_box_t test2;
float screenspeed = 0.02f;
float scx = 0;
int ptime = 0;


Timer t60(1.0 / 60);

void drawrod(glm::mat4 VP);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( scx, 0, zoom );
    glm::vec3 eye1 ( scx-zoom, 0, 0 );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (scx, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    if (sctog) {
    	Matrices.view = glm::lookAt( eye1, target, up ); // Rotating Camera for 3D
	}
	else{
    	Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	}
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    Matrices.projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    ball2.draw(VP);
    streakfries.draw(VP);
    ground.draw(VP);
    wall.draw(VP);
    ceiling.draw(VP);
    for (int i = 0;i<20;i++)
    {
    	coinarr[i].draw(VP);
    }
    coin1.draw(VP);
    coin2.draw(VP);
    farr[(ptime/400)%5].draw(VP);
    drawrod(VP);

    streakfries.updatebound(VP);
    ground.updatebound(VP);
    ground.updatebound(VP);
    ceiling.updatebound(VP);
    ball1.update(VP);
    ball2.update(VP);
}

void drawrod(glm::mat4 VP)
{
	for (int i= 0; i< 5; i++)
	{
		rod[i].draw1(VP);
		rod[i].draw(VP);
	}
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int tpress = glfwGetKey(window, GLFW_KEY_T);
    int ypress = glfwGetKey(window, GLFW_KEY_Y);
    if (tpress)
    {
    	sctog = true;
    }
    if (ypress) sctog = false;
    if (right) {
    	bspeed += 0.02f;
    }
    if (left) {
    	bspeed -= 0.02f;
    }
    if (space + up) {
    	byspeed += 0.02f;
    	streakfries.jetscale = glm::vec3(1.0f,1.0f,1.0f);
    }
    else
    {
    	byspeed -= 0.02f;
       	streakfries.jetscale = glm::vec3(0.5f,0.5f,0.5f);
    }
}

void movefirebeam()
{
	for (int i=0;i<5;i++) {
		farr[i].location.x += screenspeed;
		rod[i].rotation += 2;
	}
}

void tick_elements() {
//    ball1.tick();
//    camera_rotation_angle += 1;
	scx += screenspeed;
	ptime+=1;
	ball1.position.x += xspeed;
	ball2.position.x -= xspeed;
	ball1.position.y -= yspeed;
	ball2.position.y -= yspeed;
	streakfries.location.x += bspeed;
	if (byspeed >= 0) streakfries.location.y += byspeed;
	else if (streakfries.location.y < -19) byspeed = 0;
	else streakfries.location.y += byspeed;
	coin1.rotation+=1;
	coin2.rotation-=1;
	for (int i = 0;i < 20; i++) coinarr[i].rotation += 3;
	movefirebeam();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(-30, 0,0,2,2,2, COLOR_RED);
    ball2       = Ball(30,0,0,2,2,2, COLOR_GREEN);
    streakfries = Barry(0,0);
    ground		= Obstacles(0,-6,0,5000,6,10, springgreen);
    wall		= Obstacles(0,0,-3,5000,50,2, lightblue );
    ceiling		= Obstacles(0,6,0,5000,4,10, bluecolor);
    coin1 		= Pentatope(0,0,0,gold);
    coin2 		= Pentatope(0,0,0,gold);
    for (int i = 0; i<20 ; i++) coinarr[i] = Pentatope(10+3*i , 0 , 0 , silver);
    // f1 			= Firebeam(0,-5);
    for (int i = 0; i<5; i++){
     farr[i] = Firebeam(0 ,5*i-15);
     rod[i]  = Firebeam(i*5, 15 - i*5, 4);
 	}

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

//    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
//    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
//    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
//    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

void wallcollisions()
{
	for (int i = 0; i< 2; i++)
	{
		for (int j=0;j<5;j++)
		{
			if (wallarr[i]->box.detect_collision(streakfries.ballarr[j]))
				{
					cout << i << " collision " << j << endl;
					cout << wallarr[i]->box.bound.coordinates.x << "," << wallarr[i]->box.bound.coordinates.y << "," << wallarr[i]->box.bound.coordinates.z << " <-wall | | player -> " << streakfries.ballarr[j]->bound.coordinates.x << "," << streakfries.ballarr[j]->bound.coordinates.y << "," << streakfries.ballarr[j]->bound.coordinates.z <<endl;
				}
		}
	}

	score += coin1.collect(ball1.position);
	score += coin2.collect(ball1.position);
	test2.coordinates.x = streakfries.ballarr[1]->bound.coordinates.x;
	test2.coordinates.y = streakfries.ballarr[1]->bound.coordinates.y;
	test2.coordinates.z = streakfries.ballarr[1]->bound.coordinates.z;
	test2.coordinates.w = 1;
	test2.width = 2;
	test2.height = 2;
	test2.depth = 2;
	coin1.collect(test2);
	for (int i=0;i<20;i++) 
	{
		for (int j=0;j<5;j++)
		{
			int increase = coinarr[i].collect(streakfries.ballarr[j]->bound);
			if (increase) 
			{
				score++;
				cout << i << " , " << j << endl;
			}
		}
	}
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1280;
    int height = 1280;
    // printf("\n\tImplemented Gravity and Camera. Change The value of 'zoom' to zoom in or zoom out.\n\n");

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
            if (fall)
            {
            	colltime += 1.0f;
            	yspeed = 0.05f * colltime;
            	// zoom += 1.5f;
            }
            if (ball2.detect_collision(&ball1))
            {
            	printf("\t\t\tCollision!\n");
            	cout << score << endl;
            	xspeed = -0.5f;
            	fall = true;
            }
            wallcollisions();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
        usleep(800);
    	// cout << score <<endl;
    }

    quit(window);
}

/*bool detect_collision(const Ball& a, const Ball& b) {
    return (abs(a.position.x - b.position.x) * 2 < (a.bound.width + b.bound.width)) &&
           (abs(a.position.y - b.position.y) * 2 < (a.bound.height + b.bound.height));
}*/

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
