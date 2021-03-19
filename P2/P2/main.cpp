#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, objectMatrix, ballMatrix, projectionMatrix;

// Start at 0, 0, 0
glm::vec3 player1_position = glm::vec3(5, 0, 0);
// Don’t go anywhere (yet).
glm::vec3 player1_movement = glm::vec3(0, 0, 0);


glm::vec3 player2_position = glm::vec3(-5, 0, 0);

glm::vec3 player2_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);

glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float player1_speed = 1.0f;
float player2_speed = 1.0f;
float ball_speed = 1.0f;
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Triangle!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    objectMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ProcessInput() {

    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);
    ball_movement = glm::vec3(0);

    

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_DOWN]) {
        player1_movement.y = -8.0f;
    }
    else if (keys[SDL_SCANCODE_UP]) {
        player1_movement.y = 8.0f;
    }
    if (keys[SDL_SCANCODE_S]) {
        player2_movement.y = -8.0f;
    }
    else if (keys[SDL_SCANCODE_W]) {
        player2_movement.y = 8.0f;
    }
    if (keys[SDL_SCANCODE_SPACE]) {
        ball_movement.x = 8.0f;
    }
    /*if (glm::length(player1_movement) > 1.0f) {
        player1_movement = glm::normalize(player1_movement);
    }*/
}
float lastTicks = 0.0f;

bool checkWindowCollision(glm::vec3 object) {
    //float xdist = fabs(x2 - x1) - ((w1 + w2) / 2.0f);
    float ydist = fabs(3.75 - object.y) - ((h1 + h2) / 2.0f);
    if (ydist < 0) { // Colliding!
        
    }
}

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    // Add (direction * units per second * elapsed time)
    player1_position += player1_movement * player1_speed * deltaTime;
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 2.0f, 1.0f));
    
    modelMatrix = glm::translate(modelMatrix, player1_position);
    player2_position += player2_movement * player2_speed * deltaTime;
    objectMatrix = glm::mat4(1.0f);
    objectMatrix = glm::scale(objectMatrix, glm::vec3(1.0f, 2.0f, 1.0f));
    objectMatrix = glm::translate(objectMatrix, player2_position);
    ball_position += ball_movement * ball_speed * deltaTime;
    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
 
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    /*glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);*/
    program.SetModelMatrix(modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program.SetModelMatrix(objectMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
    //DisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
