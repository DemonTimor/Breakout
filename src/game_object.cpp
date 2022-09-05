#include "game_object.h"


GameObject::GameObject() : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity) : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {

}

void GameObject::Move(float dt, unsigned int window_width) {
    this->Position += this->Velocity * dt;

    if (this->Position.x <= 0.0f) {
        this->Velocity.x = 0.0f;
        this->Position.x = 0.0f;
    }
    else if (this->Position.x + this->Size.x >= window_width) {
        this->Velocity.x = 0.0f;
        this->Position.x = window_width - this->Size.x;
    }
}

void GameObject::Draw(SpriteRenderer& renderer) {
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}