#include "Player.h"

Player::Player()
    : x(360.0f), y(380.0f), radius(16.0f), color(WHITE),  
      jumpCharge(0.0f), isCharging(false), maxJumpCharge(13.0f), 
      jumpSpeed(5.0f), velocityX(0.0f), velocityY(0.0f), 
      lineAngle(0.0f), isSwinging(false), swingSpeed(0.02f), 
      lastSwingAngle(0.0f), isSwingStopped(false), shouldDrawLine(false) {}

void Player::Update(MapChip& mapChip) {

   if (Novice::CheckHitKey(DIK_SPACE) && !isCharging  && !isSwinging && !isSwingStopped) {
        isSwinging = true;  
        shouldDrawLine = true; // 开始绘制红线
    } 
    // 停止摆动
    else if (isSwinging && !Novice::CheckHitKey(DIK_SPACE)) {
        isSwinging = false;
        isSwingStopped = true; // 记录红线停止摆动
        lastSwingAngle = lineAngle; // 记录当前摆动角度
    } 
    // 第二次按下空格键，蓄力跳跃
    else if (Novice::CheckHitKey(DIK_SPACE) && isSwingStopped && !isCharging ) {
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

      // 更新玩家位置
    float nextX = x + velocityX;
    float nextY = y + velocityY;

 // 处理四个角的组合碰撞
    bool collisionTopRight = mapChip.CheckCollision(static_cast<int>(nextX + radius), static_cast<int>(nextY - radius));
    bool collisionTopLeft = mapChip.CheckCollision(static_cast<int>(nextX - radius), static_cast<int>(nextY - radius));
    bool collisionBottomRight = mapChip.CheckCollision(static_cast<int>(nextX + radius), static_cast<int>(nextY + radius));
    bool collisionBottomLeft = mapChip.CheckCollision(static_cast<int>(nextX - radius), static_cast<int>(nextY + radius));

    // 如果右上角或左上角发生碰撞，重置垂直速度
    if (collisionTopRight || collisionTopLeft) {
        velocityY = 0.0f;
    }

    // 如果右下角或左下角发生碰撞，重置水平速度
    if (collisionBottomRight || collisionBottomLeft) {
        velocityX = 0.0f;
        velocityY = 0.0f;
    }

    // 添加重力效果
    bool isOnPlatform = false;

    // 检测平台的碰撞
    for (auto& platform : mapChip.platforms) {
        if (velocityY > 0) {
            bool collisionX = nextX + radius > platform.x && nextX - radius < platform.x + platform.width;
            bool collisionY = nextY + radius > platform.y && y + radius <= platform.y;

            if (collisionX && collisionY) {
                isOnPlatform = true;
                velocityY = 0.0f;
                velocityX = 0.0f;
                nextY = platform.y - radius;
               if (platform.velocityX > 0) {
            nextX += platform.velocityX+2;
        } 
        // 如果平台向左移动，减少玩家的X坐标
        else if (platform.velocityX < 0) {
            nextX += platform.velocityX-2; 
        }
                break;
            }
        }
        if (velocityY < 0) {
        bool collisionX = nextX + radius > platform.x && nextX - radius < platform.x + platform.width;
        bool collisionYBottom = nextY - radius < platform.y + platform.height && y - radius >= platform.y + platform.height;

        // 如果玩家的头部撞到平台的底部，则停止上升
        if (collisionX && collisionYBottom) {
            velocityY = 0.0f;  // 停止玩家上升
            nextY = platform.y + platform.height + radius;  // 调整玩家位置到平台底部以下
        }
    }
    }

    if (!isOnPlatform) {
        velocityY += 0.5f;  // 添加重力效果
    }
      // 如果没有碰撞，更新位置
    if (!collisionTopRight && !collisionTopLeft && !collisionBottomRight && !collisionBottomLeft) {
        x = nextX;
        y = nextY;
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
    if (lineAngle > 0.5f || lineAngle < -0.5f) {
        swingSpeed = -swingSpeed;  // 当到达极限角度时，反转摆动方向
    }
}