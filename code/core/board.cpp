#include "board.h"

#include <windows.h>

#include <iostream>
int Board::length_ = 530 / 8;
int Board::start_x = 35;
int Board::start_y = 35;

Board::Board() {
    srand((unsigned int)time(0));
    mouse_on_lightning = 0;
    mouse_on_diamond = 0;
    mouse_on_shuffle = 0;
    add_tools = 0;
    rest_lightning = 2;
    rest_diamond = 2;
    rest_shuffle = 2;
    std::cerr << "generate start" << std::endl;
    Generate(1);
    std::cerr << "generate end" << std::endl;
}

void Board::SetHypercube(Hypercube::Hypercube *hypercube) { hypercube_ = hypercube; }

void Board::InitHypercube() {
    Sleep(100);
    hypercube_->stone_manager_->Init(8, 8);
    hypercube_->stone_manager_->Start();

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // std::cout << i << " " << j << " " << stones_[i][j].GetType() << std::endl;
            int ret = hypercube_->stone_manager_->Generate(i, j, stones_[i][j].GetType(), rand() % 500);
            if (ret != Hypercube::StoneManager::kSuccess) std::cout << i << " " << j << " " << ret << std::endl;
        }
    }
}

std::pair<int, int> Board::GetChosen() { return chosen_; }

/* 生成 */
void Board::Generate(bool start) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            positions_[i][j] = {start_x + i * length_, start_y + j * length_};
            stones_[i][j] = Stone(start_x + i * length_, start_y + j * length_);
            // if(!start) animation : falll
        }
    }
    if (start) {
        while (Check()) {
            for (auto match : matches_) {
                stones_[match.first][match.second].SetType(rand() % Stone::GetMaxType() + 1);
            }
            // Fall2();
            add_tools = 0;
        }
    }
    chosen_ = {-1, -1};
}

/* 交换两个宝石 */
void Board::Swap(Stone &a, Stone &b) {
    Stone tmp = a;
    a = b;
    b = tmp;
}

/* 检查是否有可以消除的宝石，若有则将位置记录在matches_中 */
bool Board::Check() {
    add_tools = 0;
    matches_.clear();
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int now = j - 1, type = stones_[i][j].GetType();
            int sum = 1;
            while (now >= 0 && stones_[i][now].GetType() == type) {
                now--;
                sum++;
            }
            now = j + 1;
            while (now < 8 && stones_[i][now].GetType() == type) {
                now++;
                sum++;
            }
            if (sum >= 3) {
                matches_.push_back({i, j});
                if (sum >= 5) {
                    add_tools = 1;
                }
                continue;
            }
            sum = 1;
            now = i - 1;
            while (now >= 0 && stones_[now][j].GetType() == type) {
                now--;
                sum++;
            }
            now = i + 1;
            while (now < 8 && stones_[now][j].GetType() == type) {
                now++;
                sum++;
            }
            if (sum >= 3) {
                matches_.push_back({i, j});
                if (sum >= 5) {
                    add_tools = 1;
                }
                continue;
            }
        }
    }
    if (!matches_.empty()) {
        return 1;
    }
    return 0;
}

/* 鼠标点击坐标(x,y) */
void Board::Clicked(int x, int y) {
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            std::cerr << stones_[i][j].GetType() << " ";
        }
        std::cerr << "\n";
    }
    int chosen_x = -1, chosen_y = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (x >= positions_[i][j].first && x <= positions_[i][j].first + length_ && y >= positions_[i][j].second &&
                y <= positions_[i][j].second + length_) {
                chosen_x = i;
                chosen_y = j;
                break;
            }
        }
    }

    // 点击其它判断
    /*
        // 如果点击到了钻石道具(坐标判断)
        if () {
            chosen_ = {-1, -1};
            mouse_on_diamond = 1;
            return;
        }
        // 如果点击到了闪电道具(坐标判断)
        if () {
            chosen_ = {-1, -1};
            rest_lightning--;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    int dice = rand()%10;
                    if(dice <= 1) matches_.push_back({i, j});
                }
            }
            Refresh();
            return;
        }
        //如果点到了刷新棋盘(坐标判断)
        if() {
            Generate();
        }
    */
    std::cerr << x << " " << y << " " << chosen_x << " " << chosen_y << std::endl;
    if (chosen_x == -1) {
        if (chosen_.first != -1) {
            hypercube_->stone_manager_->SetRotate(chosen_.first, chosen_.second, Hypercube::StoneManager::kRotate);
        }
        return;
    }
    if (chosen_.first == -1) {
        if (mouse_on_diamond) {
            stones_[chosen_x][chosen_y].SetType(stones_[chosen_x][chosen_y].GetMaxType() + 1);
            mouse_on_diamond = 0;
            rest_diamond--;
            return;
        }
        chosen_ = {chosen_x, chosen_y};

        hypercube_->stone_manager_->SetRotate(chosen_x, chosen_y, Hypercube::StoneManager::kRotateFastInverse);

        return;
    }
    if (chosen_x == chosen_.first && chosen_y == chosen_.second) {
        hypercube_->stone_manager_->SetRotate(chosen_.first, chosen_.second, Hypercube::StoneManager::kRotate);
        return;
    }
    if (abs(chosen_x - chosen_.first) + abs(chosen_y - chosen_.second) <= 1) {
        Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
        if (!Check()) {
            Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
            hypercube_->stone_manager_->SetRotate(chosen_.first, chosen_.second, Hypercube::StoneManager::kRotate);
            chosen_ = {chosen_x, chosen_y};
            // animation setRotate
            hypercube_->stone_manager_->SetRotate(chosen_x, chosen_y, Hypercube::StoneManager::kRotateFastInverse);

            return;
        } else {
            hypercube_->stone_manager_->SetRotate(chosen_.first, chosen_.second, Hypercube::StoneManager::kRotate);
            std::cout << hypercube_->stone_manager_->SwapStone(chosen_.first, chosen_.second, chosen_x, chosen_y) << "\n";
            std::cerr << "swap:" << chosen_.first << " " << chosen_.second << " " << chosen_x << " " << chosen_y << "\n";
            while (false && hypercube_->stone_manager_->isPlayingAnimation()) {
                Sleep(20);
            };
            chosen_ = {-1, -1};
            Refresh();
        }
        return;
    }
    hypercube_->stone_manager_->SetRotate(chosen_.first, chosen_.second, Hypercube::StoneManager::kRotate);
    chosen_ = {chosen_x, chosen_y};
    // animation setRotate
    hypercube_->stone_manager_->SetRotate(chosen_x, chosen_y, Hypercube::StoneManager::kRotateFastInverse);
}

void Board::Refresh() {
    combo_base = 1.0;
    double accelerate_base = 0.2;
    std::cerr << "in Refresh"
              << "\n";
    int flag = 0;
    while (!matches_.empty() || Check()) {
        if (add_tools) {
            rest_diamond++;
            rest_shuffle++;
            rest_lightning++;
            add_tools = 0;
        }
        std::cerr << flag << "\n";
        Remove();
        Fall();
        combo_base += accelerate_base;
        accelerate_base += 0.1;
        matches_.clear();
    };
    std::cerr << "out Refresh"
              << "\n";
}

/*消除matches_中的宝石*/
void Board::Remove() {
    for (const auto &match : matches_) {
        Remove(match.first, match.second);
        std::cerr << "match: " << match.first << " " << match.second << "\n";
    }
    matches_.clear();
    while (false && hypercube_->stone_manager_->isPlayingAnimation()) {
        Sleep(20);
    };
}

void Board::Remove(int x, int y) {
    if (x < 0 || y < 0 || x > 7 || y > 7) return;
    if (stones_[x][y].Empty()) return;
    point_ += 2000.0 * combo_base;
    stones_[x][y].SetEmpty(1);
    // animation Remove
    hypercube_->stone_manager_->Remove(x, y);
    if (stones_[x][y].GetType() == stones_[x][y].GetMaxType() + 1) {
        point_ += 10000.0 * combo_base;
        for (int i = x - 2; i <= x + 2; ++i) {
            for (int j = y - 2; j <= y + 2; ++j) {
                Remove(i, j);
            }
        }
    }
    return;
}

// 提示
bool Board::ShowHint(bool show) {
    bool get_hint = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (i < 7) {
                bool flag = 0;
                Swap(stones_[i][j], stones_[i + 1][j]);
                flag = Check();
                Swap(stones_[i][j], stones_[i + 1][j]);
                if (flag) {
                    hint_[0] = {i, j};
                    hint_[1] = {i + 1, j};
                    get_hint = 1;
                    break;
                }
            }
            if (j < 7) {
                bool flag = 0;
                Swap(stones_[i][j], stones_[i][j + 1]);
                flag = Check();
                Swap(stones_[i][j], stones_[i][j + 1]);
                if (flag) {
                    hint_[0] = {i, j};
                    hint_[1] = {i + 1, j};
                    get_hint = 1;
                    break;
                }
            }
        }
        if (get_hint) break;
    }
    if (show) {
        // animation show_hint
    }
    return get_hint;
}

bool Board::Check_Game_Over() {
    if (ShowHint(0)) {
        return 1;
    }
    return 0;
}

/* 宝石掉落并补齐 */
void Board::Fall() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 7; j >= 0; --j) {
            if (stones_[i][j].Empty()) continue;
            int now = j + 1;
            while (now < 8 && stones_[i][now].Empty()) {
                now++;
            }
            now--;
            if (now == j) continue;

            hypercube_->stone_manager_->FallTo(i, j, now);
            Swap(stones_[i][j], stones_[i][now]);
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!stones_[i][j].Empty()) break;
            // animation generate

            stones_[i][j] = Stone(positions_[i][j].first, positions_[i][j].second);
            stones_[i][j].SetEmpty(0);
            hypercube_->stone_manager_->Generate(i, j, stones_[i][j].GetType());
        }
    }
    while (false && hypercube_->stone_manager_->isPlayingAnimation()) {
        Sleep(20);
    };
}
