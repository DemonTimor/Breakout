#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

const glm::vec2 PARTICLE_SIZE(10.0f, 10.0f);

struct Particle {
    glm::vec2 Position, Velocity, Size;
    glm::vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(PARTICLE_SIZE) {
    
    }
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    void Update(float dt, GameObject& object, unsigned int newParticles);
    void Draw();
private:
    std::vector<Particle> particles;
    unsigned int amount;
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, GameObject& object);
};

#endif