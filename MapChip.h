#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <Novice.h>

class Platform {
public:
    float x, y;       // 平台的位置
    float startX, startY;     // 平台的初始位置
    float width, height; // 平台的大小
    float velocityX, velocityY;  // 平台的水平和垂直移动速度
    float range;      // 平台的移动范围
    bool moveVertical; // 表示平台是否是上下移动

    Platform(float x, float y, float width, float height, float velocityX, float velocityY, float range, bool moveVertical);
    void Update();
    void Draw();
};

class MapChip {
public:
    std::vector<std::vector<int>> mapData; // 存储地图数据
    std::vector<Platform> platforms; // 存储平台数据

    MapChip();
    void LoadMap(const std::string& filename);
    void DrawMap();
    bool CheckCollision(int nextX, int nextY);
    void UpdatePlatforms();
    void DrawPlatforms();
};
