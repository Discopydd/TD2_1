#include "MapChip.h"

MapChip::MapChip() {}

void MapChip::LoadMap(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    mapData.clear(); // 清空旧数据

    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string tile;

        while (std::getline(ss, tile, ',')) {
            row.push_back(std::stoi(tile)); // 将字符串转换为整数
        }
        mapData.push_back(row);
    }
}

void MapChip::DrawMap() {
    for (size_t row = 0; row < mapData.size(); ++row) {
        for (size_t col = 0; col < mapData[row].size(); ++col) {
            if (mapData[row][col] == 1) {
                // 在地图上绘制方块，假设每个 tile 的大小为 32
                Novice::DrawBox(static_cast<int>(col * 32), static_cast<int>(row * 32), 32, 32, 0.0f, BLACK, kFillModeSolid);
            }
        }
    }
}

bool MapChip::CheckCollision(int nextX, int nextY) {
    // 确定玩家的网格位置
    int gridX = nextX / 32; // tile size 为 32
    int gridY = nextY / 32;

    // 检查是否在地图范围内
    if (gridX < 0 || gridX >= mapData[0].size() || gridY < 0 || gridY >= mapData.size()) {
        return true; // 超出地图范围
    }

    // 检查是否与地图的障碍物碰撞（假设 1 表示障碍物）
    return mapData[gridY][gridX] == 1;
}
