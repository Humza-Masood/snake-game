#include "raylib.h"
#include <cstdlib>
#include <ctime>

#include "gameMechs.h"
#include "player.h"
#include "objPos.h"
#include "objPosArrayList.h"

static float speedToMovesPerSecond(int speed)
{
    if (speed < 1) speed = 1;
    if (speed > 10) speed = 10;
    return 2.5f + 1.55f * speed;
}

int main()
{
    const int boardX = 20;
    const int boardY = 20;

    const int tile = 28;
    const int hudH = 70;

    InitWindow(boardX * tile, boardY * tile + hudH, "Snake (raylib)");
    SetTargetFPS(60);

    std::srand((unsigned)std::time(nullptr));

    // Use pointers so restart is safe (no shallow-copy pointer bugs)
    GameMechs* gm = new GameMechs(boardX, boardY);
    gm->setSpeed(6);

    Player* player = new Player(gm);

    // Spawn food not on snake
    gm->generateFoodPos(player->getPlayerPos());

    float accumulator = 0.0f;

    while (!WindowShouldClose() && !gm->getExitFlagStatus())
    {
        // Quit anytime
        if (IsKeyPressed(KEY_ESCAPE))
        {
            gm->setExitTrue();
        }

        // Restart only when dead
        if (gm->getLoseFlagStatus() && IsKeyPressed(KEY_R))
        {
            int savedSpeed = gm->getSpeed();

            // cleanly destroy and rebuild objects
            delete player;
            delete gm;

            gm = new GameMechs(boardX, boardY);
            gm->setSpeed(savedSpeed);

            player = new Player(gm);
            gm->generateFoodPos(player->getPlayerPos());

            accumulator = 0.0f;
        }

        // Input only when alive
        if (!gm->getLoseFlagStatus() && !gm->getExitFlagStatus())
        {
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    
            {
                gm->setInput('w');
            }
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
            {
                gm->setInput('s');
            }
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
            {
                gm->setInput('a');
            }
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
            {
                gm->setInput('d');
            }

            // optional speed controls
            if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD))
            {
                gm->setSpeed(gm->getSpeed() + 1);
            }

            if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT))
            {
                gm->setSpeed(gm->getSpeed() - 1);
            }
        }

        // Update only when alive
        if (!gm->getLoseFlagStatus() && !gm->getExitFlagStatus())
        {
            float dt = GetFrameTime();
            accumulator += dt;

            float step = 1.0f / speedToMovesPerSecond(gm->getSpeed());

            while (accumulator >= step && !gm->getLoseFlagStatus() && !gm->getExitFlagStatus())
            {
                player->updatePlayerDir();
                player->movePlayer();     // IMPORTANT: should set loseFlag only (not exitFlag) on death

                if (gm->getLoseFlagStatus())
                {
                    accumulator = 0.0f;
                    break;
                }

                objPos head;
                player->getHeadPos(head);

                objPos food = gm->getFoodPos();

                if (head.isPosEqual(&food))
                {
                    gm->incrementScore();
                    gm->generateFoodPos(player->getPlayerPos());
                    // no tail removal => growth
                }
                else
                {
                    player->removeTail();
                }

                accumulator -= step;
            }
        }
        else
        {
            accumulator = 0.0f;
        }

        // Draw always
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, boardX * tile, hudH, (Color){20, 20, 24, 255});
        DrawText(TextFormat("Score: %d", gm->getScore()), 14, 12, 24, RAYWHITE);
        DrawText(TextFormat("Speed: %d", gm->getSpeed()), 14, 38, 20, (Color){200, 200, 210, 255});

        if (!gm->getLoseFlagStatus())
        {
            DrawText("ESC=Quit   +/-=Speed", 200, 38, 18, (Color){200, 200, 210, 255});
        }
        else
        {
            DrawText("GAME OVER", 200, 10, 28, (Color){255, 90, 90, 255});
            DrawText("Press R to restart, ESC to quit", 150, 40, 18, (Color){230, 230, 230, 255});
        }

        const int ox = 0;
        const int oy = hudH;

        for (int y = 0; y < boardY; y++)
        {
            for (int x = 0; x < boardX; x++)
            {
                bool border = (x == 0 || y == 0 || x == boardX - 1 || y == boardY - 1);
                Color c = border ? (Color){120, 120, 130, 255} : (Color){30, 30, 34, 255};
                DrawRectangle(ox + x * tile, oy + y * tile, tile, tile, c);
            }
        }

        objPos food = gm->getFoodPos();
        DrawCircle(ox + food.pos->x * tile + tile / 2,
                   oy + food.pos->y * tile + tile / 2,
                   tile * 0.33f,
                   (Color){255, 140, 0, 255});

objPosArrayList* body = player->getPlayerPos();

// Draw BODY as squares 
for (int i = 1; i < body->getSize(); i++)
{
    objPos seg = body->getElement(i);

    DrawRectangle(ox + seg.pos->x * tile + 2,
                  oy + seg.pos->y * tile + 2,
                  tile - 4,
                  tile - 4,
                  (Color){60, 170, 95, 255});
}


// Draw HEAD as a semicircle (index 0)
if (body->getSize() > 0)
{
    objPos headSeg = body->getElement(0);

    float left   = (float)(ox + headSeg.pos->x * tile);
    float top    = (float)(oy + headSeg.pos->y * tile);
    float cx     = left + tile * 0.5f;
    float cy     = top  + tile * 0.5f;

    float radius = tile * 0.45f;

    // You need a direction getter: Player::Dir Player::getDir() const;
    Player::Dir d = player->getDir();

    // We'll set start/end angles so the FILLED half faces the movement direction.
    float startDeg = 0.0f;
    float endDeg   = 180.0f;

    if (d == Player::RIGHT || d == Player::STOP)
    {
        // Facing right: keep right half -> angles from 270 to 90
        startDeg = -90.0f;
        endDeg   = 90.0f;
    }
    else if (d == Player::LEFT)
    {
        // Facing left: keep left half -> angles from 90 to 270
        startDeg = 90.0f;
        endDeg   = 270.0f;
    }
    else if (d == Player::UP)
    {
        // Facing up: keep top half -> angles from 180 to 360
        startDeg = 180.0f;
        endDeg   = 360.0f;
    }
    else if (d == Player::DOWN)
    {
        // Facing down: keep bottom half -> angles from 0 to 180
        startDeg = 0.0f;
        endDeg   = 180.0f;
    }

    // Draw filled semicircle. "segments" controls smoothness.
    DrawCircleSector({cx, cy}, radius, startDeg, endDeg, 24, (Color){80, 220, 120, 255});
}



        EndDrawing();
    }

    // cleanup
    delete player;
    delete gm;

    CloseWindow();
    return 0;
}
