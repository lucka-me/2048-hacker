//
//  2048-lab.c
//  2048 Lab
//
//  Created by Lucka.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN_FIELD 4

void getNewTile (int Field[LEN_FIELD][LEN_FIELD]);
int Down (int Field[LEN_FIELD][LEN_FIELD]);
int Right (int Field[LEN_FIELD][LEN_FIELD]);
int Left (int Field[LEN_FIELD][LEN_FIELD]);
int Up (int Field[LEN_FIELD][LEN_FIELD]);

int isLost (int Field[LEN_FIELD][LEN_FIELD]);
int isWin (int Field[LEN_FIELD][LEN_FIELD]);

double getRandom(void);
void PrintField(int Field[LEN_FIELD][LEN_FIELD]);

int main(int argc, const char * argv[]) {
    // 初始化随机数种子
    srand((unsigned int)time(NULL));

    int Field[LEN_FIELD][LEN_FIELD] = {0};

    printf("> Start:\n");
    getNewTile(Field);
    getNewTile(Field);
    PrintField(Field);

    // 开始解
    int Score = 0;
    int DownScore, RightScore, LeftScore, UpScore;
    do {

        DownScore = 0;
        RightScore = 0;
        LeftScore = 0;
        UpScore = 0;

        DownScore = Down(Field);
        if (!DownScore) {
            RightScore = Right(Field);
            if (!RightScore) {
                LeftScore = Left(Field);
                if (!LeftScore) {
                    UpScore = Up(Field);
                }
            }
        }

        DownScore = DownScore == 1 ? 0 : DownScore;
        RightScore = RightScore == 1 ? 0 : RightScore;
        LeftScore = LeftScore == 1 ? 0 : LeftScore;
        UpScore = UpScore == 1 ? 0 : UpScore;

        Score += (DownScore + RightScore + LeftScore + UpScore);

        getNewTile(Field);
    } while (!isLost(Field) && !isWin(Field));

    // 输出结果
    printf("> Result:\n");
    PrintField(Field);
    if (isWin(Field)) {
        printf("> You win.\n");
    } else {
        printf("> Game over.\n");
    }
    printf("> Score: %d\n", Score);
    return 0;
}

// 在随机位置加入随机的格子
void getNewTile (int Field[LEN_FIELD][LEN_FIELD]) {
    int x = 0;
    int y = 0;

    // 获取随机位置
    int cntBlank = 0;
    for (int scan_x = 0; scan_x < LEN_FIELD; scan_x++) {
        for (int scan_y = 0; scan_y < LEN_FIELD; scan_y++) {
            if (!Field[scan_x][scan_y]) {
                cntBlank++;
            }
        }
    }

    // 获取随机位置的序号
    int index = getRandom() * cntBlank + 1;

    // 找到随机位置的坐标
    int isPositionFound = 0;
    for (int scan_x = 0; scan_x < LEN_FIELD; scan_x++) {
        for (int scan_y = 0; scan_y < LEN_FIELD; scan_y++) {
            if (Field[scan_x][scan_y]) continue;
            index--;
            if (index == 0) {
                isPositionFound = 1;
                x = scan_x;
                y = scan_y;
                break;
            }
        }
        if (isPositionFound) break;
    }

    // 加入随机数
    Field[x][y] = getRandom() < 0.9 ? 2 : 4;
}

// 向下移动
int Down (int Field[LEN_FIELD][LEN_FIELD]) {
    int isMoved = 0;
    int Score = 0;
    int isMerged[LEN_FIELD][LEN_FIELD] = {0};
    for (int x = LEN_FIELD - 1; x >= 0; x--) {
        for (int y = 0; y < LEN_FIELD; y++) {
            if (!Field[x][y]) continue;

            int MoveX = x;
            // 移动
            while (MoveX + 1 < LEN_FIELD && !Field[MoveX + 1][y]) {
                MoveX++;
            }
            if (MoveX != x) {
                Field[MoveX][y] = Field[x][y];
                Field[x][y] = 0;
                isMoved = 1;
            }
            // 合并
            if (MoveX + 1 < LEN_FIELD && Field[MoveX + 1][y] == Field[MoveX][y] && !isMerged[MoveX + 1][y]) {
                Field[MoveX + 1][y] *= 2;
                Field[MoveX][y] = 0;
                isMerged[MoveX + 1][y] = 1;
                Score += Field[MoveX + 1][y];
            }
        }
    }
    if (Score == 0) {
        return isMoved;
    } else {
        return Score;
    }
}

// 向右移动
int Right (int Field[LEN_FIELD][LEN_FIELD]) {
    int isMoved = 0;
    int Score = 0;
    int isMerged[LEN_FIELD][LEN_FIELD] = {0};
    for (int y = LEN_FIELD - 1; y >= 0; y--) {
        for (int x = 0; x < LEN_FIELD; x++) {
            if (!Field[x][y]) continue;

            int MoveY = y;
            // 移动
            while (MoveY + 1 < LEN_FIELD && !Field[x][MoveY + 1]) {
                MoveY++;
            }
            if (MoveY != y) {
                Field[x][MoveY] = Field[x][y];
                Field[x][y] = 0;
                isMoved = 1;
            }
            // 合并
            if (MoveY + 1 < LEN_FIELD && Field[x][MoveY + 1] == Field[x][MoveY] && !isMerged[x][MoveY + 1]) {
                Field[x][MoveY + 1] *= 2;
                Field[x][MoveY] = 0;
                isMerged[x][MoveY + 1] = 1;
                Score += Field[x][MoveY + 1];
            }
        }
    }
    if (Score == 0) {
        return isMoved;
    } else {
        return Score;
    }
}

// 向左移动
int Left (int Field[LEN_FIELD][LEN_FIELD]) {
    int isMoved = 0;
    int Score = 0;
    int isMerged[LEN_FIELD][LEN_FIELD] = {0};
    for (int y = 1; y < LEN_FIELD; y++) {
        for (int x = 0; x < LEN_FIELD; x++) {
            if (!Field[x][y]) continue;

            int MoveY = y;
            // 移动
            while (MoveY - 1 >= 0 && !Field[x][MoveY - 1]) {
                MoveY--;
            }
            if (MoveY != y) {
                Field[x][MoveY] = Field[x][y];
                Field[x][y] = 0;
                isMoved = 1;
            }
            // 合并
            if (MoveY - 1 >= 0 && Field[x][MoveY - 1] == Field[x][MoveY] && !isMerged[x][MoveY - 1]) {
                Field[x][MoveY - 1] *= 2;
                Field[x][MoveY] = 0;
                isMerged[x][MoveY - 1] = 1;
                Score += Field[x][MoveY - 1];
            }
        }
    }
    if (Score == 0) {
        return isMoved;
    } else {
        return Score;
    }
}

// 向上移动
int Up (int Field[LEN_FIELD][LEN_FIELD]) {
    int isMoved = 0;
    int Score = 0;
    int isMerged[LEN_FIELD][LEN_FIELD] = {0};
    for (int x = 1; x < LEN_FIELD; x++) {
        for (int y = 0; y < LEN_FIELD; y++) {
            if (!Field[x][y]) continue;

            int MoveX = x;
            // 移动
            while (MoveX - 1 >= 0 && !Field[MoveX - 1][y]) {
                MoveX--;
            }
            if (MoveX != x) {
                Field[MoveX][y] = Field[x][y];
                Field[x][y] = 0;
                isMoved = 1;
            }
            // 合并
            if (MoveX - 1 >= 0 && Field[MoveX - 1][y] == Field[MoveX][y] && !isMerged[MoveX - 1][y]) {
                Field[MoveX - 1][y] *= 2;
                Field[MoveX][y] = 0;
                isMerged[MoveX - 1][y] = 1;
                Score += Field[MoveX - 1][y];
            }
        }
    }
    if (Score == 0) {
        return isMoved;
    } else {
        return Score;
    }
}

// 检查游戏是输
int isLost (int Field[LEN_FIELD][LEN_FIELD]) {
    // 检查是否被充满
    int cntBlank = 0;
    for (int scan_x = 0; scan_x < LEN_FIELD; scan_x++) {
        for (int scan_y = 0; scan_y < LEN_FIELD; scan_y++) {
            if (!Field[scan_x][scan_y]) {
                cntBlank++;
            }
        }
    }
    if (cntBlank > 0) {
        return 0;
    }

    // 当填满时检查周围是否有相同的方块
    int x, y;
    // 检查第二行的上、下、右
    x = 1;
    for (y = 0; y < LEN_FIELD - 1; y++) {
        if (Field[x][y] == Field[x - 1][y]) return 0;
        if (Field[x][y] == Field[x + 1][y]) return 0;
        if (Field[x][y] == Field[x][y + 1]) return 0;
    }
    if (Field[x][y] == Field[x - 1][y]) return 0;
    if (Field[x][y] == Field[x + 1][y]) return 0;
    // 检查第三行的下、右
    x = 3;
    for (y = 0; y < LEN_FIELD - 1; y++) {
        if (Field[x][y] == Field[x + 1][y]) return 0;
        if (Field[x][y] == Field[x][y + 1]) return 0;
    }
    if (Field[x][y] == Field[x + 1][y]) return 0;

    return 1;
}

// 检查是否赢
int isWin (int Field[LEN_FIELD][LEN_FIELD]) {
    for (int x = 0; x < LEN_FIELD; x++) {
        for (int y = 0; y < LEN_FIELD; y++) {
            if (Field[x][y] == 2048) return 1;
        }
    }

    return 0;
}

// 获取 0~1 之间的随机数，不包括 1
double getRandom(void) {
    return rand() / (double)RAND_MAX;
}

// 输出宫格
void PrintField(int Field[LEN_FIELD][LEN_FIELD]) {
    for (int x = 0; x < LEN_FIELD; x++) {
        for (int y = 0; y < LEN_FIELD; y++) {
            printf("%d", Field[x][y]);
            if (y != LEN_FIELD - 1) {
                putchar('\t');
            }
        }
        putchar('\n');
    }
}
