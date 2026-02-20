#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

struct Player {
    int x;
    int y;
};

struct Bullet {
    int x;
    int y;
    bool active;
};

struct Enemy {
    int x;
    int y;
    bool active;
};

void clearScreen() {
    system("cls");
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main() {
    srand(time(NULL));
    hideCursor();
    
    Player player;
    player.x = WIDTH / 2;
    player.y = HEIGHT - 2;
    
    vector<Bullet> bullets;
    vector<Enemy> enemies;
    
    int score = 0;
    int spawnCounter = 0;
    bool gameRunning = true;
    
    while (gameRunning) {
        while (_kbhit()) {
            char key = _getch();
            
            if (key == -32) {
                key = _getch();
                switch(key) {
                    case 75:
                        if (player.x > 1) player.x--;
                        break;
                    case 77:
                        if (player.x < WIDTH - 2) player.x++;
                        break;
                }
            }
            else {
                switch(key) {
                    case 'a':
                    case 'A':
                    case 'ф':
                    case 'Ф':
                        if (player.x > 1) player.x--;
                        break;
                    case 'd':
                    case 'D':
                    case 'в':
                    case 'В':
                        if (player.x < WIDTH - 2) player.x++;
                        break;
                    case ' ':
                        Bullet newBullet;
                        newBullet.x = player.x;
                        newBullet.y = player.y - 1;
                        newBullet.active = true;
                        bullets.push_back(newBullet);
                        break;
                    case 'q':
                    case 'Q':
                    case 'й':
                    case 'Й':
                        gameRunning = false;
                        break;
                }
            }
        }
        
        spawnCounter++;
        if (spawnCounter > 3) {
            Enemy newEnemy;
            newEnemy.x = rand() % (WIDTH - 2) + 1;
            newEnemy.y = 1;
            newEnemy.active = true;
            enemies.push_back(newEnemy);
            spawnCounter = 0;
        }
        
        for (auto& bullet : bullets) {
            bullet.y--;
            if (bullet.y <= 0) {
                bullet.active = false;
            }
        }
        
        bullets.erase(
            remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.active; }),
            bullets.end()
        );
        
        for (auto& enemy : enemies) {
            enemy.y++;
            if (enemy.y >= HEIGHT - 1) {
                enemy.active = false;
            }
        }
        
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (enemy.active && bullet.active &&
                    bullet.x == enemy.x && 
                    abs(bullet.y - enemy.y) <= 1) {
                    bullet.active = false;
                    enemy.active = false;
                    score += 10;
                    break;
                }
            }
        }
        
        for (auto& enemy : enemies) {
            if (enemy.active && 
                enemy.x == player.x && 
                enemy.y == player.y) {
                gameRunning = false;
            }
        }
        
        enemies.erase(
            remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.active; }),
            enemies.end()
        );
        
        gotoxy(0, 0);
        
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        for (int y = 1; y <= HEIGHT; y++) {
            cout << "#";
            for (int x = 1; x <= WIDTH; x++) {
                bool drawn = false;
                
                if (x == player.x && y == player.y) {
                    cout << "A";
                    drawn = true;
                }
                
                for (const auto& enemy : enemies) {
                    if (x == enemy.x && y == enemy.y) {
                        cout << "V";
                        drawn = true;
                        break;
                    }
                }
                
                for (const auto& bullet : bullets) {
                    if (x == bullet.x && y == bullet.y) {
                        cout << "|";
                        drawn = true;
                        break;
                    }
                }
                
                if (!drawn) cout << " ";
            }
            cout << "#" << endl;
        }
        
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        cout << "Score: " << score << "  ";
        cout << "A/D: move, Space: shoot, Q: quit" << endl;
        
        Sleep(50);
    }
    
    clearScreen();
    cout << "GAME OVER!" << endl;
    cout << "Your score: " << score << endl;
    cout << "Press any key to exit...";
    _getch();
    
    return 0;
}
