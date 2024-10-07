#pragma once
#include <Novice.h>
#include <math.h>

class Player {
public:
    float x, y;          // 玩家的位置
    float radius;        // 圆的半径
    unsigned int color;  // 圆的颜色
    float jumpCharge;    // 当前的跳跃蓄力值
    bool isCharging;     // 是否在蓄力跳跃
     float maxJumpCharge; // 最大蓄力值
    float jumpSpeed;     // 跳跃的基础速度
    float velocityX, velocityY; // 玩家跳跃后的速度

     float lineAngle;     // 红线的角度，用于控制摆动
    bool isSwinging;     // 红线是否在摆动
    float swingSpeed;    // 红线的摆动速度
    Player();

    void Update();
    void Draw();
    void Jump();
    void SwingLine();
};
