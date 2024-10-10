#include "Player.h"

Player::Player()
    : x(360.0f), y(360.0f), radius(30.0f), color(WHITE),  
      jumpCharge(0.0f), isCharging(false), maxJumpCharge(13.0f), 
      jumpSpeed(5.0f), velocityX(0.0f), velocityY(0.0f), 
      lineAngle(0.0f), isSwinging(false), swingSpeed(0.02f), 
      lastSwingAngle(0.0f), isSwingStopped(false), shouldDrawLine(false) {}

void Player::Update() {
    bool isOnGround = (y >= 720 - radius);
   if (Novice::CheckHitKey(DIK_SPACE) && !isCharging && isOnGround && !isSwinging && !isSwingStopped) {
        isSwinging = true;  
        lineAngle = -0.5f;  // 开始摆动
        shouldDrawLine = true; // 开始绘制红线
    } 
    // 停止摆动
    else if (isSwinging && !Novice::CheckHitKey(DIK_SPACE)) {
        isSwinging = false;
        isSwingStopped = true; // 记录红线停止摆动
        lastSwingAngle = lineAngle; // 记录当前摆动角度
    } 
    // 第二次按下空格键，蓄力跳跃
    else if (Novice::CheckHitKey(DIK_SPACE) && isSwingStopped && !isCharging && isOnGround) {
        isCharging = true;
        jumpCharge = 0.0f;
    }

    // 如果正在充能
    if (isCharging) {
        jumpCharge += 0.5f; 
        if (jumpCharge > maxJumpCharge) {
            jumpCharge = maxJumpCharge;
        }
    }

    // 检测是否释放空格键并执行跳跃
    if (!Novice::CheckHitKey(DIK_SPACE) && isCharging) {
        Jump();  // 执行跳跃
        velocityX = static_cast<float>(sin(lastSwingAngle)) * jumpCharge; // 使用最后的角度
        isCharging = false;
        isSwingStopped = false; // 重置摆动状态
    }

    // 更新玩家位置和重力
    x += velocityX;
    y += velocityY;
    velocityY += 0.5f;

    // 碰撞检测
    if (y > 720 - radius) {
        y = 720 - radius;
        velocityY = 0.0f;
        velocityX = 0.0f;
    }

    // 如果正在摆动红线，更新角度
    if (isSwinging) {
        SwingLine();
    }
}

void Player::Draw() {
    // 绘制玩家为一个圆
    Novice::DrawEllipse(static_cast<int>(x), static_cast<int>(y), static_cast<int>(radius), static_cast<int>(radius), 0.0f, color, kFillModeSolid);
     if (shouldDrawLine) {
        // 计算红线终点的位置
        float lineLength = 100.0f;  // 红线的长度
        float endX = x + lineLength * static_cast<float>((sin(lineAngle)));
        float endY = y - lineLength * static_cast<float>((cos(lineAngle)));

        Novice::DrawLine(static_cast<int>(x), static_cast<int>(y), static_cast<int>(endX), static_cast<int>(endY), RED);
    }
}

void Player::Jump() {
    // 基于蓄力值和跳跃速度计算跳跃力度
    float finalJumpForce = jumpSpeed + jumpCharge;
    velocityY = -finalJumpForce;
}
void Player::SwingLine() {
    // 让红线的角度在一定范围内摆动
    lineAngle += swingSpeed;
    if (lineAngle > 1.0f || lineAngle < -1.0f) {
        swingSpeed = -swingSpeed;  // 当到达极限角度时，反转摆动方向
    }
}