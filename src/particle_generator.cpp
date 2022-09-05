#include "particle_generator.h"

#include <tuple>

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount) : shader(shader), texture(texture), amount(amount) {
    this->init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles) {
    for (unsigned int i = 0; i < newParticles; ++i) {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object);
    }
    for (unsigned int i = 0; i < this->amount; ++i) {
        Particle& p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f) {
            p.Position += p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    std::vector<std::tuple<glm::mat4, glm::vec4>> tuples;

    for (const Particle& particle : this->particles) {
        if (particle.Life > 0.0f) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(particle.Position, 0.0f));
            model = glm::scale(model, glm::vec3(particle.Size, 1.0f));
            tuples.push_back(std::make_tuple(model, particle.Color));
        }
    }

    if (tuples.size()) {
        glBindVertexArray(this->VAO);

        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, tuples.size() * (sizeof(glm::mat4) + sizeof(glm::vec4)), &std::get<0>(tuples.front()), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::vec4), (void*)sizeof(glm::vec4));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4) + sizeof(glm::vec4), (void*)sizeof(glm::mat4));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        this->shader.Use();
        this->texture.Bind();

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tuples.size());

        glBindVertexArray(0);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    for (unsigned int i = 0; i < this->amount; ++i) {
        this->particles.push_back(Particle());
    }
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object) {
    float random = ((rand() % 100) - 50) / 15.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = object.Position + (object.Size - particle.Size) / 2.0f + random;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 0.5f;
    particle.Velocity = -object.Velocity * 0.1f;
}