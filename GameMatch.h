// GameMatch.h
#ifndef GAMEMATCH_H
#define GAMEMATCH_H

//void DrawPlayerInfo(int x, int y, const PlayerInfo& player, bool isCurrentUser);

void ShowGameMatchPage(int matchId, int currentUserId, int & fd);

#endif
#pragma once

void drawWinButton(int x, int y);

void drawCloseButton(int x, int y);

bool isCloseButtonClicked(int x, int y, int buttonX, int buttonY);

bool isWinButtonClicked(int x, int y, int buttonX, int buttonY);

void showRulesDialog();

bool isRulesButtonClicked(int x, int y, int buttonX, int buttonY);
