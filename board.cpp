#include "board.h"

int Board::length_ = 10;
int Board::start_x = 10;
int Board::start_y = 10;

Board::Board() { Generate(); }

std::pair<int, int> Board::GetChosen() { return chosen_; }

/* 生成 */
void Board::Generate() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            positions_[i][j] = {start_x + j * length_, start_y + i * length_};
            stones_[i][j] = Stone(start_x + j * length_, start_y + i * length_);
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
    if (chosen_.first == -1) {
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
            do {
                Remove();
                Fall();
            } while (Check());
        }
    }
}

/*消除matches_中的宝石*/
void Board::Remove() {
    for (const auto &match : matches_) {
        stones_[match.first][match.second].SetEmpty(0);
        // animation Remove
    }
}

// 提示
void Board::ShowHint() {
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
    // animation show_hint
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
