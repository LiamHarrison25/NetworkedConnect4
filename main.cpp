#include <iomanip>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include "game.h"
#include "serialization.h"
#include "GameLogic/board.h"
#include "Networking/Networking.h"

const int WIDTH = 960;
const int HEIGHT = 540;

const int ROWS = 6;
const int COLS = 7;

const int CELL_W = 100;
const int CELL_H = 80;

const int OFFSET_X = 125;
const int OFFSET_Y = 35;

bool gameOver = false;

//Forward declarations:

void RunGame(Board& board, int& currentPlayer);

int main(int argc, char* argv[])
{
    SockLibInit();
    atexit(SockLibShutdown);

    //TODO: Pick between server and client

    //NOTE: Using arguments for now

    bool isServer = false;

    NetworkedUser* networkedUser = nullptr;

    if (argc > 1)
    {
        // Run the server

        isServer = true;
   
        NetworkedServer* server = new NetworkedServer(localAddress, portNumber);

        server->run_server();

        networkedUser = server;
    }
    else
    {
        // Run the client

        isServer = false;

        NetworkedClient* client = new NetworkedClient(localAddress, portNumber);

        //client->run_client();

        //networkedUser = client;
    }

    InitWindow(WIDTH, HEIGHT, "ONLINE CONNECT 4");
    SetTargetFPS(60);

    Board board;
    int currentPlayer = 1;

    while (!WindowShouldClose()) 
    {

        RunGame(board, currentPlayer);

        if (isServer)
        {
            networkedUser->RunNetworkedUpdate();
        }
        else
        {
            //networkedUser->RunNetworkedUpdate();
        }

        // Undefined destruction order literally because it's easier
        //erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get());});
        destroy_set.clear();
        spawn_queue.clear();
    }

    if (networkedUser != NULL)
    {
        delete networkedUser;
        networkedUser = nullptr;
    }
   
    return 0;
}

void RunGame(Board& board, int& currentPlayer)
{
    std::string gameMessage = "";
    int mouseX = GetMouseX();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        int columnClicked = (mouseX - OFFSET_X) / CELL_W;
        if (columnClicked >= 0 && columnClicked < COLS && !gameOver)
        {
            if (board.PlayerMove(columnClicked, currentPlayer))
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }
    }

    {
        BeginDrawing();
        defer _endDrawing(EndDrawing);


        ClearBackground(DARKGREEN);
        DrawFPS(20, 20);

        DrawRectangle(125, 25, 700, 500, DARKBLUE);

        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                int centerX = OFFSET_X + col * CELL_W + CELL_W / 2;
                int centerY = OFFSET_Y + row * CELL_H + CELL_H / 2;
                switch (board.GetBoard()[col][row])
                {
                case 0:
                    DrawCircle(centerX, centerY, 30, GRAY);
                    break;
                case 1:
                    DrawCircle(centerX, centerY, 30, RED);
                    break;
                case 2:
                    DrawCircle(centerX, centerY, 30, YELLOW);
                    break;
                }
            }
        }

        switch (board.CheckWin())
        {
        case 1:
            gameOver = true;
            gameMessage = "PLAYER 1 WINS!";
            break;
        case 2:
            gameOver = true;
            gameMessage = "PLAYER 2 WINS!";
            break;
        case 3:
            gameOver = true;
            gameMessage = "DRAW :(";
            break;
        }

        int textWidth = MeasureText(gameMessage.c_str(), 100);
        if (!gameMessage.empty())
        {
            DrawText(gameMessage.c_str(), (WIDTH - textWidth) / 2, 225, 100, BLACK);
        }

        int hoverCol = (mouseX - OFFSET_X) / CELL_W;
        if (hoverCol >= 0 && hoverCol < COLS && !gameOver)
        {
            int hoverRow = board.GetNextOpenRow(hoverCol);
            if (hoverRow != -1)
            {
                int centerX = OFFSET_X + hoverCol * CELL_W + CELL_W / 2;
                int centerY = OFFSET_Y + hoverRow * CELL_H + CELL_H / 2;
                DrawCircle(centerX, centerY, 30, DARKGRAY);
            }
        }
    }
}