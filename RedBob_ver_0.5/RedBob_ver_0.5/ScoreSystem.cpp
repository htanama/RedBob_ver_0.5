#include "ScoreSystem.h"

ScoreSystem::ScoreSystem(): m_playerPoints(0)
{
	TTF_Init();
	m_font = TTF_OpenFont("assets/ariblk.ttf", 40);
	m_fontColor = { 255, 255, 255 };
	
	m_text = "Total Score= " + std::to_string(m_playerPoints);
	if (!m_font)
	{
		std::cout << "ERROR creating font " << SDL_GetError() << std::endl;
	}

	m_TextSurface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_fontColor);
	
	if (!m_TextSurface)
	{
		std::cout << "ERROR creating m_TextSurface " << SDL_GetError() << std::endl;
	}
	
	m_TextTexture = SDL_CreateTextureFromSurface(Game::GameRenderer, m_TextSurface);
	SDL_FreeSurface(m_TextSurface);
	
	SDL_QueryTexture(m_TextTexture, NULL, NULL, &textWidth, &textHeight);
	if (!m_TextTexture){
		std::cout << "ERROR creating m_TextTexture " << SDL_GetError() << std::endl;
	}	
	m_ShowText = { 10, 0, textWidth, textHeight};
}

ScoreSystem::~ScoreSystem()
{	
	if (m_TextTexture != NULL){
		SDL_DestroyTexture(m_TextTexture);
		m_TextSurface = NULL;
	}
	
	TTF_CloseFont(m_font);	
	m_font = nullptr;
}

void ScoreSystem::UpdateScoreSystem()
{
	m_text = "Total Score= " + std::to_string(m_playerPoints);
	if (!m_font)
	{
		std::cout << "ERROR creating font " << SDL_GetError() << std::endl;
	}

	m_TextSurface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_fontColor);

	if (!m_TextSurface)
	{
		std::cout << "ERROR creating m_TextSurface " << SDL_GetError() << std::endl;
	}

	m_TextTexture = SDL_CreateTextureFromSurface(Game::GameRenderer, m_TextSurface);
	if (!m_TextTexture)
	{
		std::cout << "ERROR creating m_TextTexture " << SDL_GetError() << std::endl;
	}	
	SDL_FreeSurface(m_TextSurface);
	m_isScoreRecorded = true; // score updated
}

void ScoreSystem::setTextLocation(int Xpos, int Ypos)
{
	this->m_ShowText.x = Xpos;
	this->m_ShowText.y = Ypos;	
}

void ScoreSystem::DrawScoreBox()
{
	SDL_RenderCopy(Game::GameRenderer, m_TextTexture, NULL, &m_ShowText);
	SDL_DestroyTexture(m_TextTexture);		    
}

void ScoreSystem::setTotalScore(int totalScore)
{
	m_playerPoints = totalScore;
}

int ScoreSystem::getCurrentScore()
{
	return m_playerPoints;
}

bool ScoreSystem::IsScoreRecorded()
{	
	return m_isScoreRecorded;
}