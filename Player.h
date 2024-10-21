#pragma once
#include <Novice.h>
#include <math.h>
#include "MapChip.h"
class Player {
private:
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
    float lastSwingAngle; // 记录最后的摆动角度
    bool isSwingStopped; // 红线是否已停止摆动
    bool shouldDrawLine; // 控制红线是否被绘制
     bool isJumping; // 表示是否正在跳跃

     int textureHandle;    // 纹理句柄
    int frameIndex;       // 当前帧索引
    int frameWidth, frameHeight;  // 每帧的宽度和高度
    float animationTimer; // 动画计时器
    float animationSpeed; // 动画速度
    int fanTextureHandle;    // 纹理句柄
     int changeTextureHandle;    // 纹理句柄
    
public:
     float cameraOffsetX, cameraOffsetY;
     bool playerLife;
     float cooldownTimer;
    Player();
    ~Player();

    void Update(MapChip& mapChip);
    void Draw();
    void Jump();
    void SwingLine();
};
