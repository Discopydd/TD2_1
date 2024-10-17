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
    void Draw(int blockTextureHandle,float cameraOffsetX, float cameraOffsetY);
    bool CheckCollision(float playerX, float playerY, float playerRadius);
};

class MapChip {
public:
    std::vector<std::vector<int>> mapData; // 存储地图数据
    std::vector<Platform> platforms; // 存储平台数据
    int blockTextureHandle;

    MapChip();
    ~MapChip();
    void LoadMap(const std::string& filename);
    void DrawMap(float cameraOffsetX, float cameraOffsetY);
    bool CheckCollision(int nextX, int nextY);
    void UpdatePlatforms();
    void DrawPlatforms(float cameraOffsetX, float cameraOffsetY);
     int GetMapWidth() const {
        return static_cast<int>(mapData[0].size()) * 128;
    }

    int GetMapHeight() const {
       return static_cast<int>(mapData.size()) * 64;
    }
};
