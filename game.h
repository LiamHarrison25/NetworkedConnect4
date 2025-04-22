//
// Created by Scott Barrett on 4/14/25.
//

#ifndef GAME_H
#define GAME_H

#include <memory>
#include <unordered_map>
#include <set>

#include "raylib.h"
#include "socklib.h"
#include "defer.h"
#include "raymath.h"
#include "serialization.h"

class GameObject;

struct SpawnInfo {
    Vector2 pos;
    float rotation;
};

extern std::unordered_map<int, Color> team_colors;

extern std::vector<std::unique_ptr<GameObject>> world;
extern std::vector<std::unique_ptr<GameObject>> spawn_queue;
extern std::set<GameObject*> destroy_set;

extern std::unordered_map<int, SpawnInfo> spawn_info;

class GameObject {
public:
    Vector2 position;
    float rotation;
    int type;
    float collision_radius = -1;
    enum {
        kType = 'UKWN'
    };
    bool should_serialize = true;
    virtual void Update() {}
    virtual void Render() {}
    virtual void OnCollide(GameObject* other) {}
    virtual void Serialize(std::ostream& os);
    virtual void Deserialize(std::istream& is);
    virtual ~GameObject() = default;

    [[nodiscard]] Vector2 Up() const {
        Vector2 world_up{0, -1};
        return Vector2Rotate(world_up, rotation);
    }

    template <typename T, typename... Args>
    static T* Spawn(Args... args) {
        spawn_queue.push_back(std::make_unique<T>(args...));
        return static_cast<T *>(spawn_queue.back().get());
    }

    template <typename T>
    static T* SpawnFromStream(std::istream &is) {
        auto* obj = Spawn<T>();
        obj->Deserialize(is);
        return obj;
    }

    void Destroy() {
        for (auto & it : world) {
            if (it.get() == this) {
                destroy_set.insert(it.get());
                return;
            }
        }
        throw std::runtime_error("GameObject::Destroy() called more than once");
    }

protected:
    explicit GameObject(int type=kType, Vector2 position=Vector2{0,0}, float collision_radius = -1.f, float rotation=0) :
        position(position), rotation(rotation), type(type), collision_radius(collision_radius) {}

    explicit GameObject(int type, bool should_serialize) :
        position(Vector2{0,0}), rotation(0), type(type), should_serialize(should_serialize) {}
};



class Timer : public GameObject {
public:
    float time;
    std::function<void()> on_timeout;
    Timer(float time, std::function<void()> on_timeout) :
    GameObject('TIMR', false),
    time(time),
    on_timeout(on_timeout)
    {}

    void Update() override {
        time -= GetFrameTime();
        if (time <= 0) {
            Destroy();
            on_timeout();
        }
    }
};

class Bullet : public GameObject {
public:
    Vector2 velocity;
    int team;
    enum {
        kType = 'BLLT'
    };

    Bullet(int team=0, Vector2 position=Vector2{0,0}, Vector2 direction=Vector2{1, 0}, float speed=300) :
        team(team),
        GameObject(kType, position, 5.f),
        velocity(Vector2Scale(direction, speed))
    {}

    void Serialize(std::ostream &os) override;
    void Deserialize(std::istream &is) override;

    void Update() override {
        position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
        if (position.x > 800 || position.x < -200 || position.y > 600 || position.y < -600) {
            Destroy();
        }
    }

    void Render() override {
        DrawCircleV(position, 5, team_colors.at(team));
    }

    void OnCollide(GameObject *other) override;
};



class Explosion : public GameObject {
public:
    static const float LIFETIME;
    enum {
        kType = 'EXPL'
    };
    float alive_time;
    Color color;
    Explosion(Color color=RED, Vector2 position=Vector2{0, 0}):
    GameObject(kType, position),
    color(color),
    alive_time(LIFETIME)
    {}

    void Serialize(std::ostream &os) override;
    void Deserialize(std::istream &is) override;

    void Update() override {
        if (alive_time <= 0) {
            Destroy();
        }
        alive_time -= GetFrameTime();
    }

    void Render() override {
        constexpr int num_circles = 17;
        constexpr float arc_segment = 2 * PI / num_circles;
        for (int i = 0; i < num_circles; i++) {
            float t = (LIFETIME - alive_time) / LIFETIME;
            Vector2 dir = Vector2Rotate(Vector2{1, 0}, arc_segment * i);
            Vector2 end_point = dir * 250;
            Vector2 local_pos = Vector2Lerp(Vector2{0,0}, end_point, t);
            Vector2 pos = Vector2Add(local_pos, position);
            Color c = color;
            c.a = static_cast<unsigned char>(Lerp(255, 0, t));
            DrawRing(pos, 7.f, 10.f, 0, 360, 1, c);
        }
    }
};

class Player : public GameObject {
public:
    Vector2 velocity;
    Vector2 spawn_point;
    float frame_thrust;
    int team;
    int health=3;
    bool is_net_controlled;

    enum {
        kType = 'PLYR'
    };

    explicit Player(int team=0, bool is_net_controlled=false) :
        GameObject(kType, spawn_info.at(team).pos, 20.f, spawn_info.at(team).rotation),
        team(team),
        velocity(Vector2{0,0}),
        spawn_point(position),
        frame_thrust(0),
        is_net_controlled(is_net_controlled)
    {}

    void Serialize(std::ostream &os) override;
    void Deserialize(std::istream &is) override;

    static Player* SpawnFromStream(std::istream &is) {
        auto* player = Spawn<Player>();
        player->Deserialize(is);
        return player;
    }

    static float GetThrustScale() {
        float thrust_scale = 0;
        if (IsKeyDown(KEY_W)) {
            thrust_scale += 100;
        }
        if (IsKeyDown(KEY_S)) {
            thrust_scale -= 100;
        }
        return thrust_scale;
    }

    void Update() override {
        if (is_net_controlled) {
            return;
        }

        frame_thrust = GetThrustScale();

        if (IsKeyPressed(KEY_SPACE)) {
            Spawn<Bullet>(team, position, Up());
        }

        float rotation_speed = IsKeyDown(KEY_LEFT_SHIFT) ? 2 : 4;

        if (IsKeyDown(KEY_A)) {
            rotation -= rotation_speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_D)) {
            rotation += rotation_speed * GetFrameTime();
        }

        if (IsKeyPressed(KEY_Z)) {
            Destroy();
            Spawn<Explosion>(team_colors.at(team), position);
        }

        Vector2 thrustDir = Up();
        Vector2 thrust = Vector2Scale(thrustDir, frame_thrust);
        velocity = Vector2Add(velocity, Vector2Scale(thrust, GetFrameTime()));
        Vector2 bounce_normal{0, 0};
        bounce_normal.x = position.x > 640 ? -1.f : (position.x < 0 ? 1.f : 0);
        bounce_normal.y = position.y > 480 ? -1.f : (position.y < 0 ? -1.f : 0);
        if (Vector2LengthSqr(bounce_normal) > 0.1f) {
            velocity = Vector2Reflect(velocity, bounce_normal);
            velocity = Vector2Scale(velocity, 0.6f);
            position = Vector2Min(position, Vector2{640, 480});
            position = Vector2Max(position, Vector2{0, 0});
        }
        position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    }

    void Render() override {
        float theta = PI * 0.5f;
        const float R = 20.f;
        Vector2 points[3] = {};
        Vector2 fudge[] = {Vector2{0, -5}, Vector2{5, 0}, Vector2{-5, 0}};
        for (int i = 0; i < 3; i++) {
            points[i] = Vector2{(R * cosf(theta)), (R * -sinf(theta))};
            points[i] = Vector2Add(points[i], fudge[i]);
            points[i] = Vector2Rotate(points[i], rotation);
            points[i] = Vector2Add(points[i], position);
            theta += (2.f / 3.f) * PI;
        }
        DrawTriangle(points[0], points[1], points[2], team_colors.at(team));
    }

    void OnCollide(GameObject *other) override {
        if (other->type == Bullet::kType && dynamic_cast<Bullet *>(other)->team != team) {
            if (--health == 0) {
                Spawn<Timer>(5, [team = team, is_net_controlled = is_net_controlled] {
                    Spawn<Player>(team, is_net_controlled);
                });
                Destroy();
                Spawn<Explosion>(team_colors.at(team), position);
            }
        }
    }
};

#endif //GAME_H
