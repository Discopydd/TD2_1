#include "Player.h"

Player::Player()
    : x(400.0f), y(6800.0f), radius(16.0f), color(WHITE),  
      jumpCharge(0.0f), isCharging(false), maxJumpCharge(18.0f), 
      jumpSpeed(5.0f), velocityX(0.0f), velocityY(0.0f), 
      lineAngle(0.0f), isSwinging(false), swingSpeed(0.02f), 
      lastSwingAngle(0.0f), isSwingStopped(false), shouldDrawLine(false) , isJumping(false),cameraOffsetX(0.0f), cameraOffsetY(0.0f),    playerLife(true),  cooldownTimer(0.0f) {
  textureHandle = Novice::LoadTexture("shilaimu-Sheet-export.png");  // 加载纹理
    frameWidth = 64;  // 每帧的宽度（假设每帧32像素宽）
    frameHeight = 64;  // 每帧的高度（根据图像实际情况设置）
    frameIndex = 1;    // 当前帧的索引
    animationSpeed = 0.1f; // 控制帧切换的速度
    animationTimer = 0.0f; // 计时器
    fanTextureHandle=Novice::LoadTexture("Fan.png");
    changeTextureHandle=Novice::LoadTexture("chang.png");
}

Player::~Player()
{
   
}

void Player::Update(MapChip& mapChip) {

    if (playerLife) {
        if (cooldownTimer > 0.0f) {
            cooldownTimer -= 0.01f; // 逐帧递减冷却时间
            return; // 在冷却时间内忽略输入
        }
        animationTimer += 0.01f; // 控制动画播放速度
        if (animationTimer >= animationSpeed) {
            frameIndex = (frameIndex + 1) % 6;  // 每6帧循环一次
            animationTimer = 0.0f;
        }
        velocityY += 0.5f;


        if (Novice::CheckHitKey(DIK_SPACE) && !isCharging && !isSwinging && !isSwingStopped) {
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
        else if (Novice::CheckHitKey(DIK_SPACE) && isSwingStopped && !isCharging) {
            isCharging = true;
            jumpCharge = 0.0f;
            shouldDrawLine = false;
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
            isJumping = true;
        }
        if (isJumping) {
            if (velocityY > 0) { // 玩家开始下落
                isJumping = false;  // 重置跳跃状态
            }
        }
        // 更新玩家位置
        float nextX = x + velocityX;
        float nextY = y + velocityY;
        int screenWidth = 1020;
        int screenHeight = 720;
        int mapWidth = mapChip.GetMapWidth();  // 获取地图宽度，单位为像素
        int mapHeight = mapChip.GetMapHeight(); // 获取地图高度，单位为像素

        // 计算摄像机的偏移量，使其跟随玩家，但加上左右和底部限制
        cameraOffsetX = nextX - screenWidth / 2;
        cameraOffsetY = nextY - screenHeight / 2;
        // 限制左右范围，确保摄像机不超出地图边界
        if (cameraOffsetX < 0.0f) {
            cameraOffsetX = 0.0f;  // 限制在地图左侧
        }
        else if (cameraOffsetX > static_cast<float>(mapWidth - screenWidth)) {
            cameraOffsetX = static_cast<float>(mapWidth - screenWidth);  // 限制在地图右侧
        }

        // 限制底部范围，确保摄像机不会超出地图底部
        if (cameraOffsetY > static_cast<float>(mapHeight - screenHeight)) {
            cameraOffsetY = static_cast<float>(mapHeight - screenHeight);  // 限制在地图底部
        }

        for (auto& platform : mapChip.platforms) {
            // 检测右上角和左上角是否与平台碰撞
            bool collisionTopRightPlatform = platform.CheckCollision(nextX + radius, nextY - radius, radius);
            bool collisionTopLeftPlatform = platform.CheckCollision(nextX - radius, nextY - radius, radius);

            // 检测右下角和左下角是否与平台碰撞
            bool collisionBottomRightPlatform = platform.CheckCollision(nextX + radius, nextY + radius, radius);
            bool collisionBottomLeftPlatform = platform.CheckCollision(nextX - radius, nextY + radius, radius);

            if (!isJumping) {
                // 如果左下角或右下角与平台发生碰撞，跟随平台移动
                if (collisionBottomRightPlatform || collisionBottomLeftPlatform) {
                    if (platform.moveVertical) {
                        velocityX = 0.0f;
                        velocityY = platform.velocityY;  // 使用平台的垂直速度，而不是置零
                        nextY = platform.y - radius;
                    }
                    else {
                        velocityY = 0.0f;
                        velocityX = platform.velocityX;  // 叠加平台的水平速度
                        nextY = platform.y - radius;
                    }
                }
            }
            if (collisionTopRightPlatform || collisionTopLeftPlatform) {
                if (platform.moveVertical) {
                    velocityY = 0.0f;  // 垂直速度归零
                    nextY = y;  // 防止玩家继续上升
                }
                else {
                    velocityY = 0.0f;  // 垂直速度归零
                    nextY = y;  // 防止玩家继续上升
                }

            }
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

        // 如果没有碰撞，更新位置
        if (!collisionTopRight && !collisionTopLeft && !collisionBottomRight && !collisionBottomLeft) {
            x = nextX;
            y = nextY;
        }
       bool collisionWithDanger = mapChip.CheckDangerCollision(static_cast<int>(x), static_cast<int>(y));

// 如果玩家与危险区域碰撞，设置为死亡状态
if (collisionWithDanger) {
    playerLife = false; // 玩家死亡，触发相应的逻辑
    // 你可以在此处加入重置玩家位置、结束游戏或显示死亡界面的代码
}
        // 如果正在摆动红线，更新角度
        if (isSwinging) {
            SwingLine();
        }
    }
}

void Player::Draw() {
    if (playerLife) {
        // 计算当前帧的X坐标
        int srcX = frameIndex * frameWidth;
        int srcY = 0;  // 假设只有一行

        if (shouldDrawLine) {
            // 计算红线终点的位置
            float lineLength = 100.0f;  // 红线的长度
            float endX = x + lineLength * static_cast<float>((sin(lineAngle)));
            float endY = y - lineLength * static_cast<float>((cos(lineAngle)));
            Novice::DrawSpriteRect(
                static_cast<int>(x - cameraOffsetX - 32), static_cast<int>(y - cameraOffsetY - 60),
                0, 0, 64, 64, fanTextureHandle, 1.0f, 1.0f, 0.0f, WHITE);

            Novice::DrawLine(static_cast<int>(x - cameraOffsetX), static_cast<int>(y - cameraOffsetY), static_cast<int>(endX - cameraOffsetX), static_cast<int>(endY - cameraOffsetY), RED);
        }
        Novice::DrawSpriteRect(
            static_cast<int>(x - cameraOffsetX - frameWidth / 2 - radius), static_cast<int>(y - cameraOffsetY - frameHeight / 2),
            srcX, srcY, frameWidth, frameHeight, textureHandle, 0.25f, 1.0f, 0.0f, WHITE);
        if (isCharging) {
            Novice::DrawSpriteRect(
                static_cast<int>(x - cameraOffsetX), static_cast<int>(y - cameraOffsetY - 96),
                0, 0, 64, 64, changeTextureHandle, 2.0f, 2.0f, 0.0f, WHITE);
        }
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