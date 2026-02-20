#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

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
    
    // Читы
    bool godMode = false;
    bool infiniteAmmo = false;
    bool slowMotion = false;
    bool wallhack = false;
    int teleportX = 0;
    int teleportY = 0;
    bool teleportMode = false;
    
    cout << "Press any key to start...";
    cout << "\nCHITS:";
    cout << "\nF1 - God Mode (бессмертие)";
    cout << "\nF2 - Infinite Ammo (бесконечные патроны)";
    cout << "\nF3 - Slow Motion (замедление врагов)";
    cout << "\nF4 - Wallhack (неуязвимость + прохождение сквозь врагов)";
    cout << "\nF5 - Teleport (режим телепорта - выбери позицию)";
    _getch();
    
    while (gameRunning) {
        // Обработка клавиш
        while (_kbhit()) {
            char key = _getch();
            
            // F-клавиши для читов
            if (key == 0) {
                key = _getch();
                switch(key) {
                    case 59: // F1
                        godMode = !godMode;
                        cout << "\a"; // звуковой сигнал
                        break;
                    case 60: // F2
                        infiniteAmmo = !infiniteAmmo;
                        cout << "\a";
                        break;
                    case 61: // F3
                        slowMotion = !slowMotion;
                        cout << "\a";
                        break;
                    case 62: // F4
                        wallhack = !wallhack;
                        cout << "\a";
                        break;
                    case 63: // F5
                        teleportMode = true;
                        cout << "\nTeleport mode: Enter X (1-" << WIDTH-2 << "): ";
                        cin >> teleportX;
                        cout << "Enter Y (1-" << HEIGHT-2 << "): ";
                        cin >> teleportY;
                        if(teleportX > 0 && teleportX < WIDTH-1 && teleportY > 0 && teleportY < HEIGHT-1) {
                            player.x = teleportX;
                            player.y = teleportY;
                        }
                        teleportMode = false;
                        cout << "\a";
                        break;
                }
            }
            
            // Обработка стрелок
            if (key == -32) {
                key = _getch();
                switch(key) {
                    case 75:
                        if (player.x > 1 || wallhack) player.x--;
                        break;
                    case 77:
                        if (player.x < WIDTH - 2 || wallhack) player.x++;
                        break;
                }
            }
            else {
                switch(key) {
                    case 'a':
                    case 'A':
                    case 'ф':
                    case 'Ф':
                        if (player.x > 1 || wallhack) player.x--;
                        break;
                    case 'd':
                    case 'D':
                    case 'в':
                    case 'В':
                        if (player.x < WIDTH - 2 || wallhack) player.x++;
                        break;
                    case ' ':
                        Bullet newBullet;
                        newBullet.x = player.x;
                        newBullet.y = player.y - 1;
                        newBullet.active = true;
                        bullets.push_back(newBullet);
                        if(!infiniteAmmo) {
                            // Ограничение на количество пуль без чита
                            if(bullets.size() > 5) {
                                bullets.erase(bullets.begin());
                            }
                        }
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
        
        // Спавн врагов с учетом slow motion
        if(!slowMotion) {
            spawnCounter++;
        } else {
            spawnCounter += 0.5; // в два раза медленнее
        }
        
        if (spawnCounter > 3) {
            Enemy newEnemy;
            newEnemy.x = rand() % (WIDTH - 2) + 1;
            newEnemy.y = 1;
            newEnemy.active = true;
            enemies.push_back(newEnemy);
            spawnCounter = 0;
        }
        
        // Обновление пуль
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
        
        // Обновление врагов с учетом slow motion
        for (auto& enemy : enemies) {
            if(!slowMotion) {
                enemy.y++;
            } else {
                enemy.y += 0.5; // медленнее
            }
            if (enemy.y >= HEIGHT - 1) {
                enemy.active = false;
            }
        }
        
        // Проверка столкновений пуль с врагами
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
        
        // Проверка столкновения игрока с врагами (с учетом читов)
        for (auto& enemy : enemies) {
            if (enemy.active && 
                enemy.x == player.x && 
                enemy.y == player.y) {
                if(!godMode && !wallhack) {
                    gameRunning = false;
                } else {
                    // В режиме бога просто уничтожаем врага
                    enemy.active = false;
                    if(!wallhack) score += 5; // бонус за касание в годе
                }
            }
        }
        
        enemies.erase(
            remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return !e.active; }),
            enemies.end()
        );
        
        // Отрисовка
        gotoxy(0, 0);
        
        // Верхняя граница
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        // Игровое поле
        for (int y = 1; y <= HEIGHT; y++) {
            cout << "#";
            for (int x = 1; x <= WIDTH; x++) {
                bool drawn = false;
                
                if (x == player.x && y == player.y) {
                    if(godMode) cout << "@"; // символ бога
                    else if(wallhack) cout << "W"; // символ неуязвимости
                    else cout << "A";
                    drawn = true;
                }
                
                for (const auto& enemy : enemies) {
                    if (x == enemy.x && y == enemy.y) {
                        if(slowMotion) cout << "v"; // маленькие v в замедлении
                        else cout << "V";
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
        
        // Нижняя граница
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        
        // Информация
        cout << "Score: " << score << "  ";
        cout << "A/D: move, Space: shoot, Q: quit\n";
        
        // Статус читов
        cout << "CHITS: ";
        if(godMode) cout << " GOD";
        if(infiniteAmmo) cout << " INFAMMO";
        if(slowMotion) cout << " SLOW";
        if(wallhack) cout << " WALLHACK";
        if(!godMode && !infiniteAmmo && !slowMotion && !wallhack) cout << " OFF";
        cout << "   ";
        
        Sleep(50);
    }
    
    clearScreen();
    cout << "GAME OVER!" << endl;
    cout << "Your score: " << score << endl;
    cout << "Press any key to exit...";
    _getch();
    
    return 0;
}
