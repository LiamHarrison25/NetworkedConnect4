#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include "game.h"
#include "serialization.h"

void WriteString(const std::string& data, std::stringstream& stream);

std::string create_world_packet(const std::vector<std::unique_ptr<GameObject>>& objects) {
    // TODO: Implement this

    std::stringstream stream;

    // Get the number of objects in the world

    uint32_t objectCount = objects.size();

    std::string objectCountS = std::to_string(objectCount);

    WriteString(objectCountS, stream);

    // Loop through all the objects

    int i;
    for (i = 0; i < objectCount; i++)
    {
        // The integer type of the object (4 bytes)
        uint32_t objectType = objects[i].get()->type;

        std::string objectTypeS = std::to_string(objectType);

        WriteString(objectTypeS, stream);

        // The objects serialized data

        std::string objectData = "";

        std::ostringstream dataStream;
        objects[i].get()->Serialize(dataStream);

        objectData = dataStream.str();

        WriteString(objectData, stream);
    }

    return stream.str();
}

void WriteString(const std::string& data, std::stringstream& stream)
{
    //Writing the string to binary
    int i;
    for (i = 0; i < data.size(); i++)
    {
        const char* messageChar = &data[i];
        stream.write(messageChar, sizeof(char));
    }
}

void deserialize_packet(const std::string& data) {
    // TODO: Implement this
}

void GameObject::Deserialize(std::istream &is) {
    if (!should_serialize) {
        throw std::runtime_error("GameObject::Deserialize called on non-serializable object type");
    }

    // TODO: Implement this (leave code before this line intact)

    //position
    stream_read(is, position);

    //rotation
    stream_read(is, rotation);

     //collision_radius
    stream_read(is, collision_radius);

    //type
    //stream_read(is, type);
    
}

void GameObject::Serialize(std::ostream &os) {
    if (!should_serialize) {
        throw std::runtime_error("GameObject::Serialize called on non-serializable object type");
    }

    // TODO: Implement this (leave code before this line intact)

    //position
    stream_write(os, position);
    //rotation
    stream_write(os, rotation);

    //collision_radius
    stream_write(os, collision_radius);

    //type
    //stream_write(os, type);
    

}

void Bullet::Serialize(std::ostream &os) {
    // TODO: Implement this

    GameObject::Serialize(os);

    //team
    stream_write(os, team);

    //velocity
    stream_write(os, velocity);    

}

void Bullet::Deserialize(std::istream &is) {
    // TODO: Implement this

    GameObject::Deserialize(is);

    stream_read(is, team);

    stream_read(is, velocity);

}

void Player::Serialize(std::ostream &os) {
    // TODO: Implement this

    GameObject::Serialize(os);

    // team
    stream_write(os, team);

    // health
    stream_write(os, health);

    // spawn_point
    stream_write(os, spawn_point);

    // velocity
    stream_write(os, velocity);

    // frame_thrust
    //stream_write(os, frame_thrust);

    // is_net_controlled
    //stream_write(os, is_net_controlled);
}

void Player::Deserialize(std::istream &is) {
    // TODO: Implement this

    //stream_read(is, is_net_controlled);

    //stream_read(is, frame_thrust);

    GameObject::Deserialize(is);

    stream_read(is, team);

    stream_read(is, health);

    stream_read(is, spawn_point);

    stream_read(is, velocity);

}

void Explosion::Serialize(std::ostream &os) {
    // TODO: Implement this

    GameObject::Serialize(os);

    // alive_time
    stream_write(os, alive_time);

    // color
    stream_write(os, color);

}

void Explosion::Deserialize(std::istream &is) {
    // TODO: Implement this

    GameObject::Deserialize(is);

    //alive_time
    stream_read(is, alive_time);

    // color
    stream_read(is, color);
}

int run_all_tests();
void print_test_solution();

int main(int argc, char* argv[]) {
    //if (argc > 1)
    //{
        return run_all_tests();
    //}

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
