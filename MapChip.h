#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <Novice.h>

class MapChip {
public:
    std::vector<std::vector<int>> mapData; // 存储地图数据

    MapChip();
    void LoadMap(const std::string& filename);
    void DrawMap(); // 绘制地图
    bool CheckCollision(int nextX, int nextY); // 碰撞检测
};
