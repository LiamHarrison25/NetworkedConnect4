//
// Created by Scott Barrett on 4/14/25.
//
#include "game.h"

std::unordered_map<int, Color> team_colors {
        {0, RED},
        {1, BLUE},
        {2, GREEN},
        {3, ORANGE},
        {4, YELLOW},
        {5, BLACK},
};

std::unordered_map<int, SpawnInfo> spawn_info {
            {0, {Vector2{0, 0}, 0}},
            {1, {Vector2{320, 460}, 0}},
            {2, {Vector2{550, 240}, -PI / 2}}
};


std::vector<std::unique_ptr<GameObject>> world;
std::vector<std::unique_ptr<GameObject>> spawn_queue;
std::set<GameObject*> destroy_set;

// std::vector<std::unique_ptr<GameObject>>& GameObject::world = world;
// std::vector<std::unique_ptr<GameObject>>& GameObject::spawn_queue = spawn_queue;
// std::set<GameObject*>& GameObject::destroy_set = destroy_set;

void Bullet::OnCollide(GameObject *other) {
    if (other->type == Player::kType && dynamic_cast<Player *>(other)->team != team) {
        Destroy();
    }
}

const float Explosion::LIFETIME = 2.0f;