#include "Player.h"

Player::Player()
    : x(1360.0f), y(780.0f), radius(16.0f), color(WHITE),  
      jumpCharge(0.0f), isCharging(false), maxJumpCharge(25.0f), 
      jumpSpeed(5.0f), velocityX(0.0f), velocityY(0.0f), 
      lineAngle(0.0f), isSwinging(false), swingSpeed(0.02f), 
      lastSwingAngle(0.0f), isSwingStopped(false), shouldDrawLine(false) , cameraOffsetX(0.0f), cameraOffsetY(0.0f) {}

void Player::Update(MapChip& mapChip) {
    velocityY += 0.5f;
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
    int screenWidth = 1980;
    int screenHeight = 1080;
    int mapWidth = mapChip.GetMapWidth();  // 获取地图宽度，单位为像素
    int mapHeight = mapChip.GetMapHeight(); // 获取地图高度，单位为像素

    // 计算摄像机的偏移量，使其跟随玩家，但加上左右和底部限制
    cameraOffsetX = nextX - screenWidth / 2;
    cameraOffsetY = nextY - screenHeight / 2;
    // 限制左右范围，确保摄像机不超出地图边界
if (cameraOffsetX < 0.0f) {
    cameraOffsetX = 0.0f;  // 限制在地图左侧
} else if (cameraOffsetX > static_cast<float>(mapWidth - screenWidth)) {
    cameraOffsetX = static_cast<float>(mapWidth - screenWidth);  // 限制在地图右侧
}

// 限制底部范围，确保摄像机不会超出地图底部
if (cameraOffsetY > static_cast<float>(mapHeight - screenHeight)) {
    cameraOffsetY = static_cast<float>(mapHeight - screenHeight);  // 限制在地图底部
}


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



    // 检测平台的碰撞
    for (auto& platform : mapChip.platforms) {
        // 检测右上角和左上角是否与平台碰撞
        bool collisionTopRightPlatform = platform.CheckCollision(nextX, nextY, radius);
        bool collisionTopLeftPlatform = platform.CheckCollision(nextX, nextY, radius);


        bool collisionBottomRightPlatform = platform.CheckCollision(nextX, nextY, radius);
        bool collisionBottomLeftPlatform = platform.CheckCollision(nextX, nextY, radius);

        // 如果左下角或右下角与平台发生碰撞，跟随平台移动
        if (collisionBottomRightPlatform || collisionBottomLeftPlatform) {
            velocityY = 0.0f;  // 垂直速度归零
             velocityX = 0.0f;
            // 玩家跟随平台的移动
          if (platform.velocityX > 0.0f) {
        nextX += 2.0f;  // 当平台向右移动时，调整玩家的 x 坐标
    } else if (platform.velocityX < 0.0f) {
        nextX -= 2.0f;  // 当平台向左移动时，调整玩家的 x 坐标
    }
            nextY += platform.velocityY; // 平台的垂直速度影响玩家的位置
        }

        if (collisionTopRightPlatform || collisionTopLeftPlatform) {
            velocityY = 0.0f;  // 垂直速度归零
            nextY = y;  // 防止玩家继续上升
        }
         
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
    Novice::DrawEllipse(static_cast<int>(x - cameraOffsetX), static_cast<int>(y - cameraOffsetY), static_cast<int>(radius), static_cast<int>(radius), 0.0f, color, kFillModeSolid);
     if (shouldDrawLine) {
        // 计算红线终点的位置
        float lineLength = 100.0f;  // 红线的长度
        float endX = x + lineLength * static_cast<float>((sin(lineAngle)));
        float endY = y - lineLength * static_cast<float>((cos(lineAngle)));

        Novice::DrawLine(static_cast<int>(x- cameraOffsetX), static_cast<int>(y- cameraOffsetY), static_cast<int>(endX- cameraOffsetX), static_cast<int>(endY- cameraOffsetY), RED);
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