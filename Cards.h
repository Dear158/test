//扑克和玩家手牌的类定义
// Cards.h

#pragma once
#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <ctime>
#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>

enum class CardColor { SPADE, HEART, DIAMOND, CLUB };

struct SingleCard {
    CardColor type;
    char key;

    SingleCard() : type(CardColor::SPADE), key('A') {}
    SingleCard(CardColor _type, char _key) : type(_type), key(_key) {}

    friend std::ostream& operator<<(std::ostream& os, const SingleCard& card);
    bool operator<(const SingleCard& other) const;
    bool operator==(const SingleCard& other) const;
};

std::ostream& operator<<(std::ostream& os, const SingleCard& card);

struct Cards3 {
    std::array<SingleCard, 3> cards;

    Cards3() = default;
    Cards3(const SingleCard& card1, const SingleCard& card2, const SingleCard& card3)
        : cards{ card1, card2, card3 } {}

    bool operator<(const Cards3& other) const;
    bool operator==(const Cards3& other) const;
};

class Cards52 {
public:
    static Cards52& GetInstance() {
        static Cards52 instance;
        return instance;
    }

    void Shuffle();
    Cards3 Deal3();

private:
    Cards52();
    std::vector<SingleCard> deck;
    int currentIndex;
};

// 玩家手牌类
class PlayerHand {
public:
    PlayerHand();
    void AddCard(const SingleCard& card);
    void Clear();
    void ShowHand() const;
    Cards3 GetHand() const;

private:
    std::array<SingleCard, 3> hand;
};

#endif // CARDS_H
