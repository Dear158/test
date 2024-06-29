
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024年6月24日                                                                
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//扑克和玩家手牌类的实现
// Cards.cpp

#include "Cards.h"

std::ostream& operator<<(std::ostream& os, const SingleCard& card) {
    static const char* cardColors[] = { "Spade", "Heart", "Diamond", "Club" };
    os << cardColors[static_cast<int>(card.type)] << " " << card.key;
    return os;
}

bool SingleCard::operator<(const SingleCard& other) const {
    if (type != other.type) return type < other.type;
    return key < other.key;
}

bool SingleCard::operator==(const SingleCard& other) const {
    return (type == other.type) && (key == other.key);
}

bool Cards3::operator<(const Cards3& other) const {
    // 根据需要实现 Cards3 的比较操作
    return std::lexicographical_compare(cards.begin(), cards.end(), other.cards.begin(), other.cards.end());
}

bool Cards3::operator==(const Cards3& other) const {
    // 根据需要实现 Cards3 的相等比较操作
    return cards == other.cards;
}

Cards52::Cards52() {
    // 初始化 52 张牌
    for (int i = 0; i < 4; ++i) {
        for (char key = '2'; key <= '9'; ++key) {
            deck.push_back(SingleCard(static_cast<CardColor>(i), key));
        }
        deck.push_back(SingleCard(static_cast<CardColor>(i), 'A'));
        deck.push_back(SingleCard(static_cast<CardColor>(i), 'K'));
        deck.push_back(SingleCard(static_cast<CardColor>(i), 'Q'));
        deck.push_back(SingleCard(static_cast<CardColor>(i), 'J'));
    }
    currentIndex = 0;
}

void Cards52::Shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    currentIndex = 0; // 重新洗牌后，将当前索引重置为0
}

Cards3 Cards52::Deal3() {
    if (currentIndex + 3 > deck.size()) {
        // 如果牌堆不足以发出三张牌，可以抛出异常或者重新洗牌
        Shuffle(); // 重新洗牌以获取新的牌堆
    }
    Cards3 dealtCards;
    for (int i = 0; i < 3; ++i) {
        dealtCards.cards[i] = deck[currentIndex++];
    }
    return dealtCards;
}

PlayerHand::PlayerHand() {
    hand.fill(SingleCard()); // 初始化手牌为默认的 SingleCard
}

void PlayerHand::AddCard(const SingleCard& card) {
    for (int i = 0; i < 3; ++i) {
        if (hand[i].type == CardColor::SPADE && hand[i].key == 'A') {
            hand[i] = card;
            break;
        }
    }
}

void PlayerHand::Clear() {
    hand.fill(SingleCard());
}

void PlayerHand::ShowHand() const {
    std::cout << "玩家手牌: ";
    for (const auto& card : hand) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

Cards3 PlayerHand::GetHand() const {
    return Cards3(hand[0], hand[1], hand[2]);
}
