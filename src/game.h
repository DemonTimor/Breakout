#ifndef GAME_H
#define GAME_H

#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <irrklang/irrKlang.h>
using namespace irrklang;

#include "game_level.h"
#include "power_up.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "text_renderer.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

const float BALL_RADIUS = 12.5f;

class Game {
public:
    bool Keys[1024];
    bool KeysProcessed[1024];

    Game(unsigned int width, unsigned int height);
    ~Game();
    void Init();

    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    
    void ResetLevel();
    void ResetPlayer();
    
    void SpawnPowerUps(GameObject& block);
    void UpdatePowerUps(float dt);

private:
    GameState State;
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    std::vector<PowerUp> PowerUps;
    unsigned int Level;
    unsigned int Lives;

    SpriteRenderer* Renderer;
    GameObject* Player;
    BallObject* Ball;
    ParticleGenerator* Particles;
    PostProcessor* Effects;
    ISoundEngine* SoundEngine;
    TextRenderer* Text;

    void ActivatePowerUp(PowerUp& powerUp);
};

#endif