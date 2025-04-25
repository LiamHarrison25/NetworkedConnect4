#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include "game.h"
#include "serialization.h"

int main() {

    InitWindow(640, 480, "ONLINE CONNECT 4");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        {
            BeginDrawing();
            defer _endDrawing(EndDrawing);

            ClearBackground(RAYWHITE);
            DrawFPS(20, 20);
        }

        // Undefined destruction order literally because it's easier
        //erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get());});
        destroy_set.clear();
        spawn_queue.clear();
    }
    return 0;
}
