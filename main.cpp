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

void RunGame(Board& board, int& currentPlayer, NetworkedUser* server, NetworkedUser* client, bool isServer);

int main(int argc, char* argv[])
{
    SockLibInit();
    atexit(SockLibShutdown);

    //TODO: Pick between server and client

    //NOTE: Using arguments for now

    bool isServer = false;

    // Network users
	NetworkedUser* networkedUser = nullptr;
	NetworkedUser* secondNetworkUser = nullptr;

    if (argc > 1) // Run the server
    {
        isServer = true;

		networkedUser = new NetworkedServer(localAddress, portNumber);

        networkedUser->RunUser();
    }
    else      // Run the client
    {
        isServer = false;

		networkedUser = new NetworkedClient(localAddress, portNumber);
        networkedUser->RunUser();

        networkedUser->SetSocket2(networkedUser->GetSocket());

        // Creates the server for the client
        secondNetworkUser = new NetworkedServer(localAddress, secondPortNumber);

        secondNetworkUser->RunUser();
    }

    InitWindow(WIDTH, HEIGHT, "ONLINE CONNECT 4");
    SetTargetFPS(60);

    Board board;
    int currentPlayer = 1;
    bool hasConnection = false;

    bool isPlaying = true;

    if (isServer) // Run the server loop
    {
        while (isPlaying)
        {
            Socket connection = networkedUser->GetSocket()->Accept(); // Waits here until it gets a connection

            networkedUser->SetSocket2(&connection);

            connection.SetNonBlockingMode(true); // Once the server gets a connection, it turns on non blocking mode

            hasConnection = true;

            secondNetworkUser = new NetworkedClient(localAddress, secondPortNumber);
            secondNetworkUser->RunUser();

            secondNetworkUser->SetSocket2(secondNetworkUser->GetSocket());

            while (!WindowShouldClose() || hasConnection)
            {
                RunGame(board, currentPlayer, networkedUser, secondNetworkUser, isServer);

                // Undefined destruction order literally because it's easier
                erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get());});
                destroy_set.clear();
                spawn_queue.clear();
            }
        }
    }
    else //Run the client loop
    {
        while (isPlaying)
        {
            Socket connection = secondNetworkUser->GetSocket()->Accept();

            secondNetworkUser->SetSocket2(&connection);

            connection.SetNonBlockingMode(true);

            while (!WindowShouldClose())
            {
                RunGame(board, currentPlayer, secondNetworkUser, networkedUser, isServer);

                // Undefined destruction order literally because it's easier
                erase_if(world, [](std::unique_ptr<GameObject>& it) {return destroy_set.contains(it.get()); });
                destroy_set.clear();
                spawn_queue.clear();
            }

            if (WindowShouldClose())
            {
                isPlaying = false;
            }
        }
    }    

    // Clear heap memory
    if (networkedUser != NULL)
    {
        delete networkedUser;
        networkedUser = nullptr;
    }

    if (secondNetworkUser != NULL)
    {
        delete networkedUser;
        networkedUser = nullptr;
    }
   
    return 0;
}

// Renders the game
void RunGame(Board& board, int& currentPlayer, NetworkedUser* server, NetworkedUser* client, bool isServer)
{
    std::string gameMessage = ""; // String that displays Win, Lose, or Draw end state
    int mouseX = GetMouseX(); // Int that gets the mouse's current X position

    int playerID = isServer ? 1 : 2; // Determines current player's turn based on if they're "Hosting"
    std::stringstream id("Player: ");
    std::string user = std::to_string(playerID);
    std::string userText = id.str() + user;

    // Checks to see if the player clicked the left mouse button and if it's their turn
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentPlayer == playerID)
    {
        // Checks to see what column the player is hovering over
        int columnClicked = (mouseX - OFFSET_X) / CELL_W;
        if (columnClicked >= 0 && columnClicked < COLS && !gameOver)
        {
            Piece piece;
            piece.PlacePiece(columnClicked, currentPlayer); // Sets the piece's column and player possesion
            if (board.PlayerMove(piece)) // Places piece on local board if valid
            {
                board.AddPiece(piece); // Adds piece to local player's pieces list
                std::stringstream stream;
                piece.Serialize(stream);
				server->SendMessage(stream, server->GetSocket2()); // Sends piece to the opponent
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Swaps currentPlayer turn
            }
        }
    }

    // Recieve input

    try
    {
        std::string output = client->RecieveMessage(*client->GetSocket2(), 4096); // Checks to see if opponent sent a message
        currentPlayer = (currentPlayer == 1) ? 2 : 1; // Swaps turns if message was sent

        std::stringstream stream(output);

        if (stream.str().length() == 1) // Checks to see if the message was to reset the game
        {
            gameOver = false;
            board.ResetGame();
            currentPlayer = 1;
            gameMessage = "";
            return;
        }

        Piece piece;
        piece.Deserialize(stream);
        board.AddPiece(piece); // Places opponent's piece to the local pieces list
        board.PlayerMove(piece); // Places opponent's piece on the local board
    }
    catch (connectionDisconnect& e)
    {
        std::cout << "The connection was closed";
    }
    catch (connectionTimeout& e)
    {
        std::cout << "The connection timed out";
    }

    {
        BeginDrawing();
        defer _endDrawing(EndDrawing);


        ClearBackground(DARKGREEN); // Background
        DrawFPS(20, 20);
        DrawText(userText.c_str(), 20, 5, 15, BLACK);

        DrawRectangle(125, 25, 700, 500, DARKBLUE); // Board frame

        // Visually sets circles to demonstrate the pieces in play
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

        // Checks to see if a player has Won or Drawn
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

        // Displays game message string when game reaches end state
        int textWidth = MeasureText(gameMessage.c_str(), 100);
        if (!gameMessage.empty())
        {
            DrawText(gameMessage.c_str(), (WIDTH - textWidth) / 2, 225, 100, BLACK);
        }

        // Highlights row that player is hovering over
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

        // Resets game when the game is in an end state
        if (gameOver && IsKeyDown(KEY_R))
        {
            gameOver = false;
            board.ResetGame();
            currentPlayer = 1;
            gameMessage = "";

            std::stringstream stream("0");
            server->SendMessage(stream, server->GetSocket2()); // Sends reset over to opponent
        }
    }
}