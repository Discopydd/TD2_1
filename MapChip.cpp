#include "MapChip.h"

MapChip::MapChip() {}

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

void Platform::Draw() {
    Novice::DrawBox(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height), 0.0f, BLUE, kFillModeSolid);
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
                platforms.emplace_back(col * 32.0f, row * 32.0f, 32.0f, 32.0f, 2.0f,0.0f,128.0f,false);
            }
            if (tileValue == 3) {
                platforms.emplace_back(col * 32.0f, row * 32.0f, 32.0f, 32.0f, 0.0f, 2.0f, 128.0f, true);
            }
        }
        mapData.push_back(mapRow);
    }
}

void MapChip::DrawMap() {
    for (size_t row = 0; row < mapData.size(); ++row) {
        for (size_t col = 0; col < mapData[row].size(); ++col) {
            if (mapData[row][col] == 1) {
                Novice::DrawBox(static_cast<int>(col * 32), static_cast<int>(row * 32), 32, 32, 0.0f, BLACK, kFillModeSolid);
            }
        }
    }
}

bool MapChip::CheckCollision(int nextX, int nextY) {
    int gridX = nextX / 32;
    int gridY = nextY / 32;

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

void MapChip::DrawPlatforms() {
    for (auto& platform : platforms) {
        platform.Draw();
    }
}
