#include "board.h"

int Board::length_ = 10;
int Board::start_x = 10;
int Board::start_y = 10;

Board::Board() {
    srand((unsigned int)time(0));
    mouse_on_lightning = 0;
    mouse_on_diamond = 0;
    mouse_on_shuffle = 0;
    rest_lightning = 2;
    rest_diamond = 2;
    rest_shuffle = 2;
    Generate();
}

std::pair<int, int> Board::GetChosen() { return chosen_; }

/* 生成 */
void Board::Generate() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            positions_[i][j] = {start_x + j * length_, start_y + i * length_};
            stones_[i][j] = Stone(start_x + j * length_, start_y + i * length_);
            // animation : fall
        }
    }
    chosen_ = {-1, -1};
}

/* 交换两个宝石 */
void Board::Swap(Stone &a, Stone &b) {
    Stone tmp = a;
    b = a;
    a = tmp;
}

/* 检查是否有可以消除的宝石，若有则将位置记录在matches_中 */
bool Board::Check() {
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
                continue;
            }
            sum = 0;
            now = i - 1;
            while (now >= 0 && stones_[now][j].GetType() == type) {
                now--;
                sum++;
            }
            now = i;
            while (now < 8 && stones_[now][j].GetType() == type) {
                now++;
                sum++;
            }
            if (sum >= 3) {
                matches_.push_back({i, j});
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
    if (chosen_x == -1) {
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
        return;
    }
    if (chosen_x == chosen_.first && chosen_y == chosen_.second) {
        return;
    }
    if (abs(chosen_x - chosen_.first) + abs(chosen_y - chosen_.second) <= 1) {
        Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
        if (!Check()) {
            // animation setRotate
            Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
            chosen_ = {chosen_x, chosen_y};
            return;
        } else {
            chosen_ = {-1, -1};
            Refresh();
        }
    }
}

void Board::Refresh() {
    combo_base = 1.0;
    double accelerate_base = 0.2;
    while (Check() || !matches_.empty()) {
        Remove();
        Fall();
        combo_base += accelerate_base;
        accelerate_base += 0.1;
    };
}

/*消除matches_中的宝石*/
void Board::Remove() {
    for (const auto &match : matches_) {
        Remove(match.first, match.second);
    }
    matches_.clear();
}

void Board::Remove(int x, int y) {
    if (x < 0 || y < 0 || x > 7 || y > 7) return;
    if (stones_[x][y].Empty()) return;
    point_ += 2000.0 * combo_base;
    stones_[x][y].SetEmpty(0);
    // animation Remove
    if (stones_[x][y].GetType() == stones_[x][y].GetMaxType() + 1) {
        point_ += 10000.0 * combo_base;
        for (int i = x - 2; i <= x + 2; ++i) {
            for (int j = y - 2; j <= y + 2; ++j) {
                Remove(i, j);
                // animation Remove
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
            // animation swap
            Swap(stones_[i][j], stones_[i][now]);
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!stones_[i][j].Empty()) break;
            // animation generate
        }
    }
}
