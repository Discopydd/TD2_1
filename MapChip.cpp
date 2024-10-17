#include "MapChip.h"

MapChip::MapChip() {
    blockTextureHandle = Novice::LoadTexture("block.png");
}
MapChip::~MapChip() {
    Novice::UnloadTexture(blockTextureHandle);
}
Platform::Platform(float x, float y, float width, float height, float velocityX, float velocityY, float range, bool moveVertical)
    : x(x), y(y), width(width), height(height), velocityX(velocityX), velocityY(velocityY), range(range), moveVertical(moveVertical) {
    startX = x; // 记录平台的初始水平位置
    startY = y; // 记录平台的初始垂直位置

}


void Platform::Update() {
  if (moveVertical) {
        y += velocityY;
        // 检查平台是否超出垂直移动范围
        if (y < startY - range || y > startY + range) {
            velocityY = -velocityY; // 反转垂直方向
        }
    } else {
        x += velocityX;
        // 检查平台是否超出水平移动范围
        if (x < startX - range || x > startX + range) {
            velocityX = -velocityX; // 反转水平方向
        }
    }
}

void Platform::Draw(int blockTextureHandle,float cameraOffsetX, float cameraOffsetY) {
    Novice::DrawSpriteRect(static_cast<int>(x- cameraOffsetX), static_cast<int>(y- cameraOffsetY), 0, 0, 128, 64, blockTextureHandle, 1.0f, 1.0f, 0.0f, WHITE);
}

bool Platform::CheckCollision(float playerX, float playerY, float playerRadius)
{
    // 平台四个角的坐标
    float left = x;
    float right = x + width;
    float top = y;
    float bottom = y + height;

    // 玩家和平台的碰撞检测，检查玩家的四个角与平台的四边是否相交
    bool collisionX = playerX + playerRadius > left && playerX - playerRadius < right;
    bool collisionY = playerY + playerRadius > top && playerY - playerRadius < bottom;

    return collisionX && collisionY;
}

void MapChip::LoadMap(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    mapData.clear();
    platforms.clear();

    for (size_t row = 0; std::getline(file, line); ++row) {
        std::vector<int> mapRow;
        std::stringstream ss(line);
        std::string tile;

        for (size_t col = 0; std::getline(ss, tile, ','); ++col) {
            int tileValue = std::stoi(tile);
            mapRow.push_back(tileValue);

            if (tileValue == 2) {
                platforms.emplace_back(col * 128.0f, row * 64.0f, 128.0f, 64.0f, 2.0f,0.0f,128.0f,false);
            }
            if (tileValue == 3) {
                platforms.emplace_back(col * 128.0f, row * 64.0f, 128.0f, 64.0f, 0.0f, 2.0f, 128.0f, true);
            }
        }
        mapData.push_back(mapRow);
    }
}

void MapChip::DrawMap(float cameraOffsetX, float cameraOffsetY) {
    for (size_t row = 0; row < mapData.size(); ++row) {
        for (size_t col = 0; col < mapData[row].size(); ++col) {
            if (mapData[row][col] == 1) {
                 Novice::DrawSpriteRect(static_cast<int>(col * 128- cameraOffsetX), static_cast<int>(row * 64- cameraOffsetY), 0, 0, 128, 64, blockTextureHandle, 1.0f, 1.0f, 0.0f, WHITE);
            }
        }
    }
}

bool MapChip::CheckCollision(int nextX, int nextY) {
    int gridX = nextX / 128;
    int gridY = nextY / 64;

    if (gridX < 0 || gridX >= mapData[0].size() || gridY < 0 || gridY >= mapData.size()) {
        return true;
    }

    return mapData[gridY][gridX] == 1;
}

void MapChip::UpdatePlatforms() {
    for (auto& platform : platforms) {
        platform.Update();
    }
}

void MapChip::DrawPlatforms(float cameraOffsetX, float cameraOffsetY) {
    for (auto& platform : platforms) {
        platform.Draw(blockTextureHandle, cameraOffsetX, cameraOffsetY);
    }
}
