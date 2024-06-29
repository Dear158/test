
///****************************************************************************
/// @author  : Dongyang Wang                                                         
/// @date    : 2024��6��24��                                                                
/// @file    :                                                                 
/// @brief   :                                                                 
///----------------------------------------------------------------------------
///                           Change History                                   
///----------------------------------------------------------------------------
///                              
///****************************************************************************

//�˿˺�����������ʵ��
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
    // ������Ҫʵ�� Cards3 �ıȽϲ���
    return std::lexicographical_compare(cards.begin(), cards.end(), other.cards.begin(), other.cards.end());
}

bool Cards3::operator==(const Cards3& other) const {
    // ������Ҫʵ�� Cards3 ����ȱȽϲ���
    return cards == other.cards;
}

Cards52::Cards52() {
    // ��ʼ�� 52 ����
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
    currentIndex = 0; // ����ϴ�ƺ󣬽���ǰ��������Ϊ0
}

Cards3 Cards52::Deal3() {
    if (currentIndex + 3 > deck.size()) {
        // ����ƶѲ����Է��������ƣ������׳��쳣��������ϴ��
        Shuffle(); // ����ϴ���Ի�ȡ�µ��ƶ�
    }
    Cards3 dealtCards;
    for (int i = 0; i < 3; ++i) {
        dealtCards.cards[i] = deck[currentIndex++];
    }
    return dealtCards;
}

PlayerHand::PlayerHand() {
    hand.fill(SingleCard()); // ��ʼ������ΪĬ�ϵ� SingleCard
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
    std::cout << "�������: ";
    for (const auto& card : hand) {
        std::cout << card << " ";
    }
    std::cout << std::endl;
}

Cards3 PlayerHand::GetHand() const {
    return Cards3(hand[0], hand[1], hand[2]);
}
