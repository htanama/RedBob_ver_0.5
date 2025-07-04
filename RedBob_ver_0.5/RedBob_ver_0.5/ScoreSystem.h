#pragma once
#include <SDL_ttf.h>
#include <string>
#include "Game.h"

class ScoreSystem
{
public:
	ScoreSystem();
	~ScoreSystem();
	void UpdateScoreSystem();
    void setTextLocation(int Xpos, int Ypos);
    void DrawScoreBox();
    void setTotalScore(int);
    int getCurrentScore();
	bool IsScoreRecorded();
	SDL_Texture *getSDLTextureStatus() const;

private:
	TTF_Font* m_font = nullptr;
	SDL_Color m_fontColor;
	SDL_Surface* m_TextSurface = nullptr;
	SDL_Texture* m_TextTexture = nullptr;
	SDL_Rect m_ShowText;

	std::string m_text;
	int textWidth = 0;
	int textHeight = 0;
	int m_playerPoints;
	bool m_isScoreRecorded = false;
};

