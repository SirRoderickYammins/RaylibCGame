#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>



/* Calculate the lengths of the sides of a right triangle between
the mouse cursor and the center of the player. */
Vector2 MouseDelta(Vector2 gunArcCenter){  

    Vector2 mousePosition = GetMousePosition();

    float dx = mousePosition.x - gunArcCenter.x;
    float dy = gunArcCenter.y - mousePosition.y;

    return (Vector2){dx, dy};

}

void WeaponFire(Circle gunArc, float rawMouseAngleRadians) {

    Vector2 mouseDelta = MouseDelta(gunArc.origin);

    float hypotenuse = gunArc.outerRadius;

    // Mouse Deltas
    float dx = mouseDelta.x;
    float dy = mouseDelta.y;

    // X and Y coordinates for triangle to calculate projectile vector
    float x;
    float y;

    if (dx > 0 && dy > 0) {
        x = hypotenuse * cosf(rawMouseAngleRadians) + gunArc.origin.x;
        y = gunArc.origin.y - hypotenuse * sinf(rawMouseAngleRadians);
    }

    if (dx < 0 && dy > 0) {
        x = gunArc.origin.x - hypotenuse * cosf(rawMouseAngleRadians);
        y = gunArc.origin.y + hypotenuse * sinf(rawMouseAngleRadians);
    }

    if (dx < 0 && dy < 0) {
        x = gunArc.origin.x - hypotenuse * cosf(rawMouseAngleRadians); 
        y = gunArc.origin.y + hypotenuse * sinf(rawMouseAngleRadians);
    }

    if (dx > 0 && dy < 0) {
        x = hypotenuse * cosf(rawMouseAngleRadians) + gunArc.origin.x;
        y = gunArc.origin.y - hypotenuse * sinf(rawMouseAngleRadians);
    }

    DrawRectangle(x, y, 20, 20, BLACK);





}




Vector2 RotationCalculator(Vector2 gunArcCenter){

    Vector2 mouseDelta = MouseDelta(gunArcCenter);

    float dx = mouseDelta.x;
    float dy = mouseDelta.y;

    /* Angle is a Vector2 because it returns an adjusted angle (for the weird Ring mechanic)
    and the raw angle from the arctan function. */

    Vector2 angle;



    // Quadrant 1 of unit cirlce
    if (dx > 0 && dy > 0) {
        angle = (Vector2){atanf(dy/dx) * (180/PI) + 90.0f, atanf(dy/dx)};
    }
    //Quadrant 2 of unit circle
    if (dx < 0 && dy > 0){
        angle = (Vector2){atanf(dy/dx) * (180/PI) - 90.0f, atanf(dy/dx)};
    }
    // Quadrant 3 of Unit Circle
    if (dx < 0 && dy < 0) {
        angle = (Vector2){atanf(dy/dx) * (180/PI) + 270.0f, atanf(dy/dx)};
    }
    // Quadrant 4 of Unit Circle
    if (dx > 0 && dy < 0) {
        angle = (Vector2){atanf(dy/dx) * (180/PI) + 90.0f, atanf(dy/dx)};
    }

    return angle;

}




void PlayerControl(Player *player, Rectangle RoomBoundaries, float frameTime) {
    // Establish coordinates for the CheckCollision fx to compare
    Vector2 playerPosLeft = {player->playerRect.x, player->playerRect.y + player->playerRect.height/2};
    Vector2 playerPosTop = {player->playerRect.x + player->playerRect.width/2, player->playerRect.y};
    Vector2 playerPosRight = {player->playerRect.x + player->playerRect.width, player->playerRect.y + player->playerRect.height/2};
    Vector2 playerPosBottom = {player->playerRect.x + player->playerRect.width/2, player->playerRect.y + player->playerRect.height};

    bool inOrOutLeft = CheckCollisionPointRec(playerPosLeft, RoomBoundaries);
    bool inOrOutTop = CheckCollisionPointRec(playerPosTop, RoomBoundaries);
    bool inOrOutRight = CheckCollisionPointRec(playerPosRight, RoomBoundaries);
    bool inOrOutBottom = CheckCollisionPointRec(playerPosBottom, RoomBoundaries);

    if(IsKeyDown(KEY_A) && inOrOutLeft){
        player->playerRect.x -= 500.0f * frameTime;
        player->gunArc.origin.x -= 500.0f * frameTime;
    }
    if(IsKeyDown(KEY_D) && inOrOutRight){
        player->playerRect.x += 500.0f * frameTime;
        player->gunArc.origin.x += 500.0f * frameTime;
    }
    if(IsKeyDown(KEY_W) && inOrOutTop){
        player->playerRect.y -= 500.0f * frameTime;
        player->gunArc.origin.y -= 500.0f * frameTime;
    }
    if(IsKeyDown(KEY_S) && inOrOutBottom){
        player->playerRect.y += 500.0f * frameTime;
        player->gunArc.origin.y += 500.0f * frameTime;
    }

    player->gunArc.endAngle = RotationCalculator(player->gunArc.origin).x;


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        WeaponFire(player->gunArc, RotationCalculator(player->gunArc.origin).y);
    }



    
    
}