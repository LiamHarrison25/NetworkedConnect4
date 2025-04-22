#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "game.h"

std::string create_world_packet(const std::vector<std::unique_ptr<GameObject>>& objects);
void print_world_packet(const std::string& packet, std::ostream& os=std::cout);
void print_test_solution();
void deserialize_packet(const std::string& packet);

char test1_packet[] = {
    '\x00', '\x00', '\x00', '\x04', '\x50', '\x4c', '\x59',
    '\x52', '\x43', '\xa0', '\x00', '\x00', '\x43', '\xe6', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x41', '\xa0',
    '\x00', '\x00', '\x01', '\x02', '\x43', '\xa0', '\x00', '\x00', '\x43', '\xe6', '\x00', '\x00', '\x42', '\x00', '\x66',
    '\x66', '\x41', '\xda', '\x66', '\x66', '\x50', '\x4c', '\x59', '\x52', '\x44', '\x09', '\x80', '\x00', '\x43', '\x70',
    '\x00', '\x00', '\xbf', '\xc9', '\x0f', '\xdb', '\x41', '\xa0', '\x00', '\x00', '\x02', '\x03', '\x44', '\x09', '\x80',
    '\x00', '\x43', '\x70', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x45', '\x58',
    '\x50', '\x4c', '\x43', '\x96', '\x00', '\x00', '\x41', '\xa0', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\xbf',
    '\x80', '\x00', '\x00', '\x40', '\x00', '\x00', '\x00', '\xff', '\xa1', '\x00', '\xff', '\x42', '\x4c', '\x4c', '\x54',
    '\x43', '\x34', '\x00', '\x00', '\x43', '\xdd', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x40', '\xa0', '\x00',
    '\x00', '\x01', '\x43', '\x34', '\x00', '\x00', '\x43', '\x70', '\x00', '\x00'
};

std::string test1_string = std::string(test1_packet, sizeof(test1_packet));

struct offset_description {
    int len;
    std::string var_name;
};

const std::vector<offset_description> test1_descriptions = {
    {4, "Object Count"},
    {4, "[plyr1] Object type"},
    {4, "[plyr1] Position.X"},
    {4, "[plyr1] Position.Y"},
    {4, "[plyr1] Rotation"},
    {4, "[plyr1] Collision Radius"},
    {1, "[plyr1] Team"},
    {1, "[plyr1] Health"},
    {4, "[plyr1] Spawn Point.X"},
    {4, "[plyr1] Spawn Point.Y"},
    {4, "[plyr1] Velocity.X"},
    {4, "[plyr1] Velocity.Y"},
    {4, "[plyr2] Object type"},
    {4, "[plyr2] Position.X"},
    {4, "[plyr2] Position.Y"},
    {4, "[plyr2] Rotation"},
    {4, "[plyr2] Collision Radius"},
    {1, "[plyr2] Team"},
    {1, "[plyr2] Health"},
    {4, "[plyr2] Spawn Point.X"},
    {4, "[plyr2] Spawn Point.Y"},
    {4, "[plyr2] Velocity.X"},
    {4, "[plyr2] Velocity.Y"},
    {4, "[explosion] Object Type"},
    {4, "[explosion] Position.X"},
    {4, "[explosion] Position.Y"},
    {4, "[explosion] Rotation"},
    {4, "[explosion] Collision Radius"},
    {4, "[explosion] Alive Time"},
    {4, "[explosion] Color"},
    {4, "[bullet] Object type"},
    {4, "[bullet] Position.X"},
    {4, "[bullet] Position.Y"},
    {4, "[bullet] Rotation"},
    {4, "[bullet] Collision Radius"},
    {1, "[bullet] Team"},
    {4, "[bullet] Velocity.X"},
    {4, "[bullet] Velocity.Y"},
};

offset_description get_offset_description(const std::vector<offset_description>& offset_descriptions, int offset) {
    int marker = offset;
    for (int i = 0; i < offset_descriptions.size(); i++) {
        marker -= offset_descriptions[i].len;
        if (marker < 0) {
            return offset_descriptions[i];
            // std::cout << "Byte offset " << offset << " has variable " << offset_descriptions[i].var_name << std::endl;
        }
    }
    if (marker <= 0) {
        return offset_descriptions.back();
        // std::cout << "Byte offset " << offset << " has variable " << offset_descriptions[i].var_name << std::endl;
    }
    return {-1, "No offset found."};
}

std::string test1_answers = "Object Count: 4\n"
"Object type: PLYR\n"
"Position: [320 460]\n"
"Rotation: 0\n"
"Collision radius: 20\n"
"Team: 1\n"
"Health: 2\n"
"Spawn point: [320, 460]\n"
"Velocity: [32.1, 27.3]\n"
"Object type: PLYR\n"
"Position: [550 240]\n"
"Rotation: -1.5708\n"
"Collision radius: 20\n"
"Team: 2\n"
"Health: 3\n"
"Spawn point: [550, 240]\n"
"Velocity: [0, 0]\n"
"Object type: EXPL\n"
"Position: [300 20]\n"
"Rotation: 0\n"
"Collision radius: -1\n"
"Alive time: 2\n"
"Color: 255, 161, 0\n"
"Object type: BLLT\n"
"Position: [180 442]\n"
"Rotation: 0\n"
"Collision radius: 5\n"
"Team: 1\n"
"Speed: 180\n"
"Direction: [240, 7.93737e-41]\n";

inline bool operator==(const Color& color1, const Color& color2) {
    return color1.a == color2.a &&
        color1.b == color2.b &&
        color1.g == color2.g &&
        color1.r == color2.r;
}

bool compare_players(const Player* player1, const Player* player2) {
    return player1->health == player2->health &&
        player1->team == player2->team &&
        player1->spawn_point == player2->spawn_point &&
        player1->velocity == player2->velocity;
}

bool compare_bullets(const Bullet* bullet1, const Bullet* bullet2) {
    return bullet1->team == bullet2->team &&
        bullet1->velocity == bullet2->velocity;
}

bool compare_explosions(const Explosion* explosion1, const Explosion* explosion2) {
    return explosion1->color == explosion2->color &&
        explosion1->alive_time == explosion2->alive_time;
}

bool compare_gameobjects(const GameObject& first, const GameObject& second) {
    if (first.type != second.type) {
        return false;
    }

    if (first.collision_radius != second.collision_radius &&
            first.position != second.position &&
            first.rotation != second.rotation) {
        return false;
    }

    switch (first.type) {
        case Player::kType:
            return compare_players(dynamic_cast<const Player*>(&first), dynamic_cast<const Player*>(&second));
        case Bullet::kType:
            return compare_bullets(dynamic_cast<const Bullet*>(&first), dynamic_cast<const Bullet*>(&second));
        case Explosion::kType:
            return compare_explosions(dynamic_cast<const Explosion*>(&first), dynamic_cast<const Explosion*>(&second));
        default:
            throw std::runtime_error("Unknown type");
    }
}

std::vector<std::unique_ptr<GameObject>> create_test1_world() {
    std::vector<std::unique_ptr<GameObject>> new_world;

    auto p1 = std::make_unique<Player>(1);
    p1->health = 2;
    p1->velocity = Vector2{32.1, 27.3};
    new_world.push_back(std::move(p1));
    new_world.push_back(std::make_unique<Player>(2, true));
    new_world.push_back(std::make_unique<Explosion>(ORANGE, Vector2{300, 20}));
    new_world.push_back(std::make_unique<Bullet>(1, Vector2{180, 442}, Vector2{0.6, 0.8}));

    return std::move(new_world);
}

int test1() {
    bool passed = true;
    auto* p1 = GameObject::Spawn<Player>(1);
    p1->health = 2;
    p1->velocity = Vector2{32.1, 27.3};
    GameObject::Spawn<Player>(2, true);
    GameObject::Spawn<Explosion>(ORANGE, Vector2{300, 20});
    GameObject::Spawn<Bullet>(1, Vector2{180, 442}, Vector2{0.6, 0.8});

    for (auto& it : spawn_queue) {
        world.push_back(std::move(it));
    }
    spawn_queue.clear();

    std::string result = create_world_packet(world);
    if (result.size() != test1_string.size()) {
        std::cout << "Test 1 failed: Expected " << test1_string.size()
        << " bytes, but got " << result.size() << " bytes. " << std::endl;
        passed = false;
    }

    unsigned long min_size = std::min(result.size(), test1_string.size());
    std::cout << std::flush;

    for (unsigned long i = 0; i < min_size; i++) {
        if (result[i] != test1_string[i]) {
            offset_description desc = get_offset_description(test1_descriptions, i);
            printf("Test 1 failed!\nByte offset %lu, Variable = {%s}\n", i, desc.var_name.c_str());
            printf("Expected 0x%02x but got 0x%02x.\n", (unsigned char)test1_string[i], (unsigned char)result[i]);
            passed = false;
        }
    }
    fflush(stdout);
    if (!passed) {
        std::cout << "\n\n===============\nTest 1 failed! Here are the expected values:\n";
        std::cout << test1_answers << std::endl;
    }
    return passed ? 0 : 1;
}

int test2() {
    world.clear();
    spawn_queue.clear();
    destroy_set.clear();

    deserialize_packet(test1_string);

    // Undefined destruction order literally because it's easier
    erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get());});
    destroy_set.clear();

    for (auto& it : spawn_queue) {
        world.push_back(std::move(it));
    }
    spawn_queue.clear();

    std::vector<std::unique_ptr<GameObject>> expected_world = create_test1_world();

    if (expected_world.size() != world.size()) {
        std::cout << "Test 2 failed: Expected " << expected_world.size() << " elements, but got " << world.size() << std::endl;
        return 1;
    }

    for (int i = 0; i < world.size(); i++) {
        if (world.at(i)->type != expected_world.at(i)->type) {
            std::cout << "Test 2 failed!\nObjects at index " << i << ": Expected object of type " << expected_world.at(i)->type << " but got " << world.at(i)->type << std::endl;
            return 1;
        }

        if (!compare_gameobjects(*world.at(i), *expected_world.at(i))) {
            std::cout << "Test 2 failed!\nObjects at index " << i << ": Objects are not equal!\n";
            return 1;
        }
    }

    return 0;
}

struct TestInfo {
    std::string test_name;
    int points;
    int (*test_fn)();
    int points_earned;
} tests[] = {
    {"Serialization test", 15, test1, 0},
    {"Deserialization test", 15, test2, 0},
};



int run_all_tests() {
    bool all_passed = true;
    int total_possible_points = 0;
    int total_points = 0;
    for (auto& info : tests) {
        try {
            total_possible_points += info.points;
            bool passed = info.test_fn() == 0;
            if (passed) {
                total_points += info.points;
                info.points_earned = info.points;
                std::cout << "Test '" << info.test_name << "' passed.\n";
            } else {
                std::cout << "Test '" << info.test_name << "' failed!\n";
                all_passed = false;
            }
        } catch (std::runtime_error& e) {
            std::cout << "Exception occurred while running " << info.test_name << ": " << e.what() << std::endl;
        }
    }

    // This is the exact moment I got too fed up with cout to keep using it
    std::cout << std::flush;
    printf("==============\n%10s\n==============\n", "Summary");
    for (auto& info : tests) {
        printf("Test '%s': %2d / %2d\n", info.test_name.c_str(), info.points_earned, info.points);
    }
    printf("\nTotal: %2d / %2d\n", total_points, total_possible_points);
    return all_passed ? 0 : 1;
}