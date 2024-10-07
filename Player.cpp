#include "Player.h"

Player::Player()
    : x(360.0f), y(360.0f), radius(30.0f), color(WHITE),  
      jumpCharge(0.0f), isCharging(false), maxJumpCharge(13.0f), 
      jumpSpeed(5.0f), velocityX(0.0f), velocityY(0.0f),lineAngle(0.0f), isSwinging(false), swingSpeed(0.02f) {}

void Player::Update() {
    bool isOnGround = (y >= 720 - radius);
    if (Novice::CheckHitKey(DIK_SPACE) && !isCharging && isOnGround) {
        isCharging = true;
        isSwinging = true;  // 开始摆动
        jumpCharge = 0.0f;
        lineAngle = -0.5f;  // 初始角度，向左摆
    }

    // 如果正在蓄力
    if (isCharging) {
        jumpCharge += 0.5f;  // 蓄力增加速度可以根据需要调整
        if (jumpCharge > maxJumpCharge) {
            jumpCharge = maxJumpCharge;
        } 
        if (isSwinging) {
        SwingLine();
    }
    }

    // 检测是否释放空格键并执行跳跃
    if (!Novice::CheckHitKey(DIK_SPACE) && isCharging) {
        Jump();  // 执行跳跃
        velocityX = static_cast<float>(sin(lineAngle)) * jumpCharge;
        isCharging = false;
        isSwinging = false; 
    }

    // 更新玩家的位置
    x += velocityX;
    y += velocityY;

    // 简单的重力模拟
    velocityY += 0.5f;  // 可以调整为更合适的重力值

    // 确保玩家不超出窗口边界并调整碰撞检测
    if (y > 720 - radius) {
        y = 720 - radius;
        velocityY = 0.0f;
         velocityX = 0.0f;
    }
}

void Player::Draw() {
    // 绘制玩家为一个圆
    Novice::DrawEllipse(static_cast<int>(x), static_cast<int>(y), static_cast<int>(radius), static_cast<int>(radius), 0.0f, color, kFillModeSolid);
     if (isSwinging) {
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