#include "board.h"

#include <windows.h>

#include <iostream>

int Board::length_ = 530 / 8;
int Board::start_x = 35;
int Board::start_y = 35;

Board::Board() {}

Board::Board(int difficulty) {
    std::cerr << "Into second constructor\n";
    SetDifficulty(difficulty);
    std::cout << difficulty << std::endl;
    mouse_on_lightning = 0;
    mouse_on_diamond = 0;
    mouse_on_shuffle = 0;
    add_tools = 0;
    cnt_ = 0;
    stop_ = 0;
    rest_lightning = 2;
    rest_diamond = 2;
    rest_shuffle = 2;
    point_ = 0;
    std::cerr << "generate start" << std::endl;
    Generate(1);
    std::cerr << "generate end" << std::endl;

    // 创建timer
    timer_ = new QTimer();
    QObject::connect(timer_, &QTimer::timeout, [&]() {
        std::cout << "haha" << std::endl;
    });
    timer_->start(10);
}

void Board::SetHypercube(Hypercube::Hypercube *hypercube) { hypercube_ = hypercube; }

void Board::InitHypercube() {
    Sleep(100);

    hypercube_->GetStoneManager()->Init(8, 8);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // std::cout << i << " " << j << " " << stones_[i][j].GetType() << std::endl;
            int ret = hypercube_->GetStoneManager()->Generate(stones_[i][j].GetId(), i, j, stones_[i][j].GetType(),
                                                              300 + rand() % 500);
            if (ret != Hypercube::StoneManager::kSuccess) std::cout << i << " " << j << " " << ret << std::endl;
        }
    }
}

std::pair<int, int> Board::GetChosen() { return chosen_; }

void Board::SetDifficulty(int difficulty) {
    difficulty_ = difficulty;
    if (difficulty == 1) {
        Stone::SetMaxType(4);
    }
    if (difficulty == 2) {
        Stone::SetMaxType(6);
    }
    if (difficulty == 3) {
        Stone::SetMaxType(8);
    }
}
/* 生成 */
void Board::Generate(bool start) {
    if (!start) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                hypercube_->GetStoneManager()->Remove(stones_[i][j].GetId());
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            positions_[i][j] = {start_x + i * length_, start_y + j * length_};
            stones_[i][j] = Stone(++cnt_);
            if (!start) hypercube_->GetStoneManager()->Generate(cnt_, i, j, stones_[i][j].GetType());
        }
    }
    if (start) {
        while (Check()) {
            for (auto match : matches_) {
                stones_[match.first][match.second].SetType(rand() % Stone::GetMaxType() + 1);
            }
            add_tools = 0;
        }
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                stones_[i][j].SetId(++cnt_);
            }
        }
    }
    hint_[0] = hint_[1] = chosen_ = {-1, -1};
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
    if (stop_) return;
    if (hypercube_->GetStoneManager()->isPlayingAnimation()) return;

    std::cout << "Board:Clicked" << x << " " << y << "\n";
    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            std::cerr << stones_[i][j].GetType() << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";

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

    std::cerr << x << " " << y << " " << chosen_x << " " << chosen_y << std::endl;
    if (chosen_x == -1) {
        if (chosen_.first != -1) {
            chosen_ = {-1, -1};
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                     Hypercube::StoneManager::kRotate);
        }
        return;
    }
    CancelHint();
    BGM::GetInstance()->PlaySwitchTask();
    if (chosen_.first == -1) {
        if (mouse_on_diamond) {
            for (int i = chosen_x - 2; i <= chosen_x + 2; ++i) {
                for (int j = chosen_y - 2; j <= chosen_y + 2; ++j) {
                    matches_.push_back({i, j});
                }
            }
            Refresh();
            mouse_on_diamond = 0;
            rest_diamond--;
            return;
        }
        if (mouse_on_lightning) {
            int type = stones_[chosen_x][chosen_y].GetType();
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (stones_[i][j].GetType() == type) matches_.push_back({i, j});
                }
            }
            Refresh();
            mouse_on_lightning = 0;
            rest_lightning--;
            return;
        }
        if (mouse_on_shuffle) {
            Generate(0);
            Refresh();
            mouse_on_shuffle = 0;
            rest_shuffle--;
            return;
        }
        chosen_ = {chosen_x, chosen_y};

        std::cerr << "Roatate: " << stones_[chosen_x][chosen_y].GetId() << " "
                  << hypercube_->GetStoneManager()->SetRotate(stones_[chosen_x][chosen_y].GetId(),
                                                              Hypercube::StoneManager::kRotateFastInverse)
                  << "\n";

        return;
    }
    if (chosen_x == chosen_.first && chosen_y == chosen_.second) {
        hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                 Hypercube::StoneManager::kRotate);
        chosen_ = {-1, -1};
        return;
    }
    if (abs(chosen_x - chosen_.first) + abs(chosen_y - chosen_.second) <= 1) {
        Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
        if (!Check()) {
            Swap(stones_[chosen_x][chosen_y], stones_[chosen_.first][chosen_.second]);
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                     Hypercube::StoneManager::kRotate);
            chosen_ = {chosen_x, chosen_y};
            // animation setRotate
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                     Hypercube::StoneManager::kRotateFastInverse);

            return;
        } else {
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                     Hypercube::StoneManager::kRotate);
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_x][chosen_y].GetId(),
                                                     Hypercube::StoneManager::kRotate);
            hypercube_->GetStoneManager()->SwapStone(stones_[chosen_.first][chosen_.second].GetId(),
                                                     stones_[chosen_x][chosen_y].GetId());

            std::cerr << "swap:" << chosen_.first << " " << chosen_.second << " " << chosen_x << " " << chosen_y
                      << "\n";
            chosen_ = {-1, -1};
            Refresh();
        }
        return;
    }
    hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                             Hypercube::StoneManager::kRotate);
    chosen_ = {chosen_x, chosen_y};
    hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                             Hypercube::StoneManager::kRotateFastInverse);
}

int Board::GetScore() { return point_; }

void Board::ClickedOnStop() {
    if (stop_ == 1) {
        // hypercube->stop(0)
        stop_ = 0;
    } else if (stop_ == 0) {
        // hypercube->stop(1)
        stop_ = 1;
    }
    // std::cout << "set stop: " << (stop_ ? "true" : "false") << std::endl;
}

void Board::ClickedOnDiamond() {
    CancelHint();
    if (mouse_on_diamond == 1) {
        mouse_on_diamond = 0;
        return;
    }
    if (rest_diamond) {
        mouse_on_diamond = 1;
    }
    return;
}

void Board::ClickedOnShuffle() {
    CancelHint();
    if (mouse_on_shuffle == 1) {
        mouse_on_shuffle = 0;
        return;
    }
    if (rest_shuffle) {
        mouse_on_shuffle = 1;
    }
    return;
}

void Board::ClickedOnLightning() {
    CancelHint();
    if (mouse_on_lightning == 1) {
        mouse_on_lightning = 0;
        return;
    }
    if (rest_lightning) {
        mouse_on_lightning = 1;
    }
    return;
}

void Board::Refresh() {
    CancelHint();
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
        flag++;
        if (flag == 1) {
            BGM::GetInstance()->PlayMatch1();
        } else if (flag == 2) {
            BGM::GetInstance()->PlayMatch2();
        } else {
            BGM::GetInstance()->PlayMatch3();
        }
        BGM::GetInstance()->PlayFall();
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
    while (false && hypercube_->GetStoneManager()->isPlayingAnimation()) {
        Sleep(20);
    };
}

void Board::Remove(int x, int y) {
    if (x < 0 || y < 0 || x > 7 || y > 7) return;
    if (stones_[x][y].Empty()) return;
    point_ += 2000.0 * combo_base;
    stones_[x][y].SetEmpty(1);
    // animation Remove
    std::cerr << "Remove:" << x << " " << y << " " << hypercube_->GetStoneManager()->Remove(stones_[x][y].GetId());
    return;
}

void Board::ClickedOnHint() { ShowHint(1); }

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
                    hint_[1] = {i, j + 1};
                    get_hint = 1;
                    break;
                }
            }
        }
        if (get_hint) break;
    }
    if (show) {
        if (chosen_.first != -1) {
            hypercube_->GetStoneManager()->SetRotate(stones_[chosen_.first][chosen_.second].GetId(),
                                                     Hypercube::StoneManager::kRotate);
        }
        chosen_ = {-1, -1};
        hypercube_->GetStoneManager()->SetRotate(stones_[hint_[0].first][hint_[0].second].GetId(),
                                                 Hypercube::StoneManager::kRotateFast);
        hypercube_->GetStoneManager()->SetRotate(stones_[hint_[1].first][hint_[1].second].GetId(),
                                                 Hypercube::StoneManager::kRotateFast);
    }
    return get_hint;
}

void Board::CancelHint() {
    if (hint_[0].first == -1) return;
    hypercube_->GetStoneManager()->SetRotate(stones_[hint_[0].first][hint_[0].second].GetId(),
                                             Hypercube::StoneManager::kRotate);
    hypercube_->GetStoneManager()->SetRotate(stones_[hint_[1].first][hint_[1].second].GetId(),
                                             Hypercube::StoneManager::kRotate);
    hint_[0] = hint_[1] = {-1, -1};
    return;
}

bool Board::IsGameOver() {
    if (!rest_shuffle && !ShowHint(0)) {
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

            hypercube_->GetStoneManager()->FallTo(stones_[i][j].GetId(), now);
            Swap(stones_[i][j], stones_[i][now]);
        }
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (!stones_[i][j].Empty()) break;
            // animation generate

            stones_[i][j] = Stone(++cnt_);
            stones_[i][j].SetEmpty(0);
            hypercube_->GetStoneManager()->Generate(stones_[i][j].GetId(), i, j, stones_[i][j].GetType());
        }
    }
    while (false && hypercube_->GetStoneManager()->isPlayingAnimation()) {
        Sleep(20);
    };
}
