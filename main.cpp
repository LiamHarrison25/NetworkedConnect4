#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include "game.h"
#include "serialization.h"



std::string create_world_packet(const std::vector<std::unique_ptr<GameObject>>& objects) {
    // TODO: Implement this
    return "";
}

void deserialize_packet(const std::string& data) {
    // TODO: Implement this
}

void GameObject::Deserialize(std::istream &is) {
    if (!should_serialize) {
        throw std::runtime_error("GameObject::Deserialize called on non-serializable object type");
    }

    // TODO: Implement this (leave code before this line intact)
}

void GameObject::Serialize(std::ostream &os) {
    if (!should_serialize) {
        throw std::runtime_error("GameObject::Serialize called on non-serializable object type");
    }

    // TODO: Implement this (leave code before this line intact)
}

void Bullet::Serialize(std::ostream &os) {
    // TODO: Implement this
}

void Bullet::Deserialize(std::istream &is) {
    // TODO: Implement this
}

void Player::Serialize(std::ostream &os) {
    // TODO: Implement this
}

void Player::Deserialize(std::istream &is) {
    // TODO: Implement this
}

void Explosion::Serialize(std::ostream &os) {
    // TODO: Implement this
}

void Explosion::Deserialize(std::istream &is) {
    // TODO: Implement this
}

int run_all_tests();
void print_test_solution();

int main(int argc, char* argv[]) {
    if (argc > 1) {
        return run_all_tests();
    }

    InitWindow(640, 480, "Object State Replication");
    SetTargetFPS(60);

    GameObject::Spawn<Player>(1);
    GameObject::Spawn<Player>(2, true);

    while (!WindowShouldClose()) {
        for (auto & it : world) {
            it->Update();
        }

        for (int i = 0; i < world.size()-1; i++) {
            for (int j = i+1; j < world.size(); j++) {
                auto& a = world[i];
                auto& b = world[j];
                if (a->collision_radius > 0 && b->collision_radius > 0) {
                    if (CheckCollisionCircles(a->position, a->collision_radius, b->position, b->collision_radius)) {
                        a->OnCollide(b.get());
                        b->OnCollide(a.get());
                    }
                }
            }
        }

        {
            BeginDrawing();
            defer _endDrawing(EndDrawing);

            ClearBackground(RAYWHITE);
            for (const auto & it : world) {
                it->Render();
            }
            DrawText(("Live Objects: " + std::to_string(world.size())).c_str(), 20, 50, 20, DARKGRAY);
            DrawFPS(20, 20);
        }

        // Undefined destruction order literally because it's easier
        erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get());});
        destroy_set.clear();

        for (auto& it : spawn_queue) {
            world.push_back(std::move(it));
        }
        spawn_queue.clear();
    }
    return 0;
}
