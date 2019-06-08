#include <allegro.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <vector>
#include "mappyal.h"
#include "main.h"
#include "BitmapList.h"
#include "SoundList.h"
#include "Speechbubble.h"
#include "Enemy.h"
#include "Level.h"
#include "Player.h"
#include "App.h"

using namespace std;


std::vector<std::string> tokenize(const std::string & str, const std::string & delim)
{
	using namespace std;
	vector<string> tokens;

	size_t p0 = 0, p1 = string::npos;
	while(p0 != string::npos)
	{
		p1 = str.find_first_of(delim, p0);
		if(p1 != p0)
		{
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}

	return tokens;
}


// padding is the number of pixels of space between the text and the borders
Speechbubble::Speechbubble(std::string text, Enemy * speaker, int widthPixels, int padding=SPEECHBUBBLE_DEFAULT_PADDING, bool speakerIsToTheRight)
{
	enSpeaker = speaker;
	setLocationToFollowSpeaker();
	iPadding = padding;
	bSpeakerIsToTheRight = speakerIsToTheRight;

	vector<string> tokens = tokenize(text, " ");
	vector<string>::iterator it;
	int currentLineWidth = 0; // pixel width of the current line
	string currentLine = "";
	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		string currentToken = *it;
		int wordPixelLength = text_length(font, (currentToken + " ").c_str());
		if (currentLineWidth + wordPixelLength + padding*2 < widthPixels)
		{
			currentLine.append(currentToken);
			currentLine.append(" ");
			currentLineWidth += wordPixelLength;


		}
		else
		{
			strText.push_back(currentLine); // add line to the final vector of strings
			currentLineWidth = 0; // reset current line
			currentLine = "";
			currentLine.append(currentToken);
			currentLine.append(" ");
			currentLineWidth += text_length(font, currentLine.c_str());

			std::string s;
			std::stringstream out;
			out << (currentLineWidth + wordPixelLength + padding*2);
			s = out.str();
			s.append("\n");
			s.append(currentLine);

			write_to_file("debug.txt", s.c_str());
		}
	}
	strText.push_back(currentLine); // add line to the final vector of strings

	int textHeight = text_height(font);
	iWidth = widthPixels;
	iHeight = (iPadding*2 + strText.size()*(textHeight + SPEECHBUBBLE_PIXELS_BETWEEN_LINES) + textHeight);
	iUpperleftX = iPivotX - (iWidth/2) - (SPEECHBUBBLE_TILE_SIZE/2);
	iUpperleftY = iPivotY - iHeight - SPEECHBUBBLE_TILE_SIZE;

	bmpBackground = createBackground();
}

Speechbubble::Speechbubble(std::string text, int pivotX, int pivotY, int widthPixels, int padding=SPEECHBUBBLE_DEFAULT_PADDING, bool speakerIsToTheRight)
{
	enSpeaker = NULL;
	iPivotX = pivotX;
	iPivotY = pivotY;
	iPadding = padding;
	bSpeakerIsToTheRight = speakerIsToTheRight;

	vector<string> tokens = tokenize(text, " ");
	vector<string>::iterator it;
	int currentLineWidth = 0; // pixel width of the current line
	string currentLine = "";
	for (it = tokens.begin(); it != tokens.end(); it++)
	{
		string currentToken = *it;
		int wordPixelLength = text_length(font, (currentToken + " ").c_str());
		if (currentLineWidth + wordPixelLength + padding*2 < widthPixels)
		{
			currentLine.append(currentToken);
			currentLine.append(" ");
			currentLineWidth += wordPixelLength;


		}
		else
		{
			strText.push_back(currentLine); // add line to the final vector of strings
			currentLineWidth = 0; // reset current line
			currentLine = "";
			currentLine.append(currentToken);
			currentLine.append(" ");
			currentLineWidth += text_length(font, currentLine.c_str());

			std::string s;
			std::stringstream out;
			out << (currentLineWidth + wordPixelLength + padding*2);
			s = out.str();
			s.append("\n");
			s.append(currentLine);

			write_to_file("debug.txt", s.c_str());
		}
	}
	strText.push_back(currentLine); // add line to the final vector of strings

	int textHeight = text_height(font);
	iWidth = widthPixels;
	iHeight = (iPadding*2 + strText.size()*(textHeight + SPEECHBUBBLE_PIXELS_BETWEEN_LINES) + textHeight);
	iUpperleftX = iPivotX - (iWidth/2) - (SPEECHBUBBLE_TILE_SIZE/2);
	iUpperleftY = iPivotY - iHeight - SPEECHBUBBLE_TILE_SIZE;

	bmpBackground = createBackground();
}

void Speechbubble::draw(BITMAP * destination)
{
	// if following an object/enemy that is the speaker of this bubble, be sure to update the bubble's coordinates first in case the object/enemy moved
	if (enSpeaker != NULL)
		setLocationToFollowSpeaker();

	// draw the background image
	masked_blit(bmpBackground, destination, 0, 0, iUpperleftX, iUpperleftY, bmpBackground->w, bmpBackground->h);

	// draw the text of each line
	int textHeight = text_height(font);
	for (unsigned int i = 0; i < strText.size(); i++)
		textout_ex(destination, font, strText.at(i).c_str(), iUpperleftX + iPadding, iUpperleftY + iPadding + (i*(textHeight + SPEECHBUBBLE_PIXELS_BETWEEN_LINES)), makecol(0,0,0), -1); 
}

BITMAP * Speechbubble::createBackground()
{
	BITMAP * bmpSpeech = create_bitmap(iWidth, iHeight+SPEECHBUBBLE_TILE_SIZE);
	clear_bitmap(bmpSpeech);
	floodfill(bmpSpeech, 0, 0, makecol(255, 0, 255));

	// first, draw the inside bitmap (center)..

	// it must be drawn multiple times to fill the inside of the speech box
	for (int y = SPEECHBUBBLE_TILE_SIZE; y < iHeight - SPEECHBUBBLE_TILE_SIZE; y += SPEECHBUBBLE_TILE_SIZE)
		for (int x = SPEECHBUBBLE_TILE_SIZE; x < iWidth - SPEECHBUBBLE_TILE_SIZE; x += SPEECHBUBBLE_TILE_SIZE)
			if (app->bmpSpeechC)
				masked_blit(app->bmpSpeechC, bmpSpeech, 0, 0, x, y, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	// now, draw top border
	for (int x = SPEECHBUBBLE_TILE_SIZE; x < iWidth - SPEECHBUBBLE_TILE_SIZE; x += SPEECHBUBBLE_TILE_SIZE)
		if (app->bmpSpeechT)
			masked_blit(app->bmpSpeechT, bmpSpeech, 0, 0, x, 0, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	// now, draw bottom border
	for (int x = SPEECHBUBBLE_TILE_SIZE; x < iWidth - SPEECHBUBBLE_TILE_SIZE; x += SPEECHBUBBLE_TILE_SIZE)
		if (app->bmpSpeechB)
			masked_blit(app->bmpSpeechB, bmpSpeech, 0, 0, x, iHeight - SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	// now, draw left border
	for (int y = SPEECHBUBBLE_TILE_SIZE; y < iHeight - SPEECHBUBBLE_TILE_SIZE; y += SPEECHBUBBLE_TILE_SIZE)
		if (app->bmpSpeechL)
			masked_blit(app->bmpSpeechL, bmpSpeech, 0, 0, 0, y, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	// now, draw right border
	for (int y = SPEECHBUBBLE_TILE_SIZE; y < iHeight - SPEECHBUBBLE_TILE_SIZE; y += SPEECHBUBBLE_TILE_SIZE)
		if (app->bmpSpeechR)
			masked_blit(app->bmpSpeechR, bmpSpeech, 0, 0, iWidth - SPEECHBUBBLE_TILE_SIZE, y, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	// draw top-left corner
	masked_blit(app->bmpSpeechTL, bmpSpeech, 0, 0, 0, 0, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);
	// draw top-right corner
	masked_blit(app->bmpSpeechTR, bmpSpeech, 0, 0, iWidth - SPEECHBUBBLE_TILE_SIZE, 0, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);
	// draw bottom-left corner
	masked_blit(app->bmpSpeechBL, bmpSpeech, 0, 0, 0, iHeight - SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);
	// draw bottom-right corner
	masked_blit(app->bmpSpeechBR, bmpSpeech, 0, 0, iWidth - SPEECHBUBBLE_TILE_SIZE, iHeight - SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	BITMAP * pointyTile = NULL;
	if (!bSpeakerIsToTheRight)
		pointyTile = app->bmpSpeechPointRight;
	else
		pointyTile = app->bmpSpeechPointLeft;
	int pointyTileX = iWidth/2 - SPEECHBUBBLE_TILE_SIZE/2; // centered
	if (!bSpeakerIsToTheRight)
		pointyTileX -= iWidth/6;
	else
		pointyTileX += iWidth/6;
	masked_blit(pointyTile, bmpSpeech, 0, 0, pointyTileX, iHeight-1, SPEECHBUBBLE_TILE_SIZE, SPEECHBUBBLE_TILE_SIZE);

	return bmpSpeech;
}

void Speechbubble::setLocationToFollowSpeaker()
{
	if (enSpeaker == NULL)
		return;

	iPivotX = enSpeaker->iX + enSpeaker->bmpImage->w;
	iPivotY = enSpeaker->iY - 20;
	iUpperleftX = iPivotX - (iWidth/2) - (SPEECHBUBBLE_TILE_SIZE/2);
	iUpperleftY = iPivotY - iHeight - SPEECHBUBBLE_TILE_SIZE;
}