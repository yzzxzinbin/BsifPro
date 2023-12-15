#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// 二维区域大小
const int ROWS = 48;
const int COLS = 29;

// 迷宫单元格状态
enum class CellState {
    Wall,
    Path
};

// 迷宫生成器类
class MazeGenerator {
public:
    MazeGenerator(int rows, int cols) : rows(rows), cols(cols) {
        // 初始化迷宫矩阵
        maze.resize(rows, std::vector<CellState>(cols, CellState::Wall));
    }

    // 生成迷宫
    void generateMaze() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        for (int i = 1; i < rows - 1; i += 2) {
            for (int j = 1; j < cols - 1; j += 2) {
                maze[i][j] = CellState::Path;

                if (i > 1) {
                    maze[i - 1][j] = CellState::Path;
                }
                if (i < rows - 2) {
                    maze[i + 1][j] = CellState::Path;
                }
                if (j > 1) {
                    maze[i][j - 1] = CellState::Path;
                }
                if (j < cols - 2) {
                    maze[i][j + 1] = CellState::Path;
                }
            }
        }

        std::shuffle(maze.begin(), maze.end(), gen);
        for (auto& row : maze) {
            std::shuffle(row.begin(), row.end(), gen);
        }
    }

    // 打印迷宫
    void printMaze() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << (maze[i][j] == CellState::Wall ? "#" : " ");
            }
            std::cout << std::endl;
        }
    }

private:
    int rows;
    int cols;
    std::vector<std::vector<CellState>> maze;
};

int main() {
    MazeGenerator generator(ROWS, COLS);
    generator.generateMaze();
    generator.printMaze();
    std::cin.get();
    return 0;
}