#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"

SpriteFontLoader::SpriteFontLoader()
{
}

SpriteFontLoader::~SpriteFontLoader()
{
}

SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto binReader = new BinaryReader();
	binReader->Open(assetFile);

	if (!binReader->Exists())
	{
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		return nullptr;
	}
	
	// See BMFont Documentation for Binary Layout
	
	// Parse the Identification bytes (B,M,F)
	char fileId1 = binReader->Read<char>();
	char fileId2 = binReader->Read<char>();
	char fileId3 = binReader->Read<char>(); 
	
	// If Identification bytes doesn't match B|M|F,
	// Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	// return nullptr
	if (fileId1 != 'B' || fileId2 != 'M' || fileId3 != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}
	// Parse the version (version 3 required)
	// If version is < 3,
	// Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	// return nullptr	
	int version = binReader->Read<char>();
	if (version != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}

	// Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	// SpriteFontLoader is a friend class of SpriteFont
	// That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	// **********
	// BLOCK 0 *
	// **********
	// Retrieve the blockId and blockSize
	int blockId = binReader->Read<char>();
	int blockSize = binReader->Read<int>();
	// Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	pSpriteFont->m_FontSize = binReader->Read<short>();
	// Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	// binReader->SetBufferPosition(blockSize);
	binReader->MoveBufferPosition(12);
	// Retrieve the FontName [SpriteFont::m_FontName]
	pSpriteFont->m_FontName = binReader->ReadNullString();	
	
	// **********
	// BLOCK 1 *
	// **********
	// Retrieve the blockId and blockSize
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	// Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	binReader->MoveBufferPosition(4);
	pSpriteFont->m_TextureWidth = binReader->Read<short>();
	pSpriteFont->m_TextureHeight = binReader->Read<short>();
	// Retrieve PageCount
	int pageCount = binReader->Read<short>();
	// > if pagecount > 1
	// > Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	if (pageCount > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
		return nullptr;
	}
	// Advance to Block2 (Move Reader)
	binReader->MoveBufferPosition(5);
	
	// **********
	// BLOCK 2 *
	// **********
	// Retrieve the blockId and blockSize
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	// Retrieve the PageName (store Local)
	wstring pageName = binReader->ReadNullString();
	//	> If PageName is empty
	if (pageName.empty())
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
		return nullptr;
	}
	// > Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	// > Retrieve texture filepath from the assetFile path
	// > (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	wstring filePath = assetFile;
	wstring searchName = pageName.substr(0, pageName.size() - 6);
	filePath = filePath.substr(0, filePath.rfind(searchName));
	// >Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	// > (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	filePath  += pageName;			
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(filePath);
	
	// **********
	// BLOCK 3 *
	// **********
	// Retrieve the blockId and blockSize
	blockId = binReader->Read<char>();
	blockSize = binReader->Read<int>();
	// Retrieve Character Count (see documentation)
	int numChars = blockSize/20;
	pSpriteFont->m_CharacterCount = numChars;
	// Retrieve Every Character, For every Character:
	for (int i = 0; i < numChars; ++i)
	{
		// > Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		UINT charachterId = binReader->Read<int>();
		// > Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		if (pSpriteFont->IsCharValid(charachterId))
		{
			// > Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
			FontMetric* fontMetric = &pSpriteFont->GetMetric(charachterId);
			// > Set IsValid to true [FontMetric::IsValid]
			fontMetric->IsValid = true;
			// > Set Character (CharacterId) [FontMetric::Character]
			fontMetric->Character = charachterId;
			// > Retrieve Xposition (store Local)
			UINT x = binReader->Read<wchar_t>();
			// > Retrieve Yposition (store Local)
			UINT y = binReader->Read<wchar_t>();
			// > Retrieve & Set Width [FontMetric::Width]
			fontMetric->Width = binReader->Read<wchar_t>();;
			// > Retrieve & Set Height [FontMetric::Height]
			fontMetric->Height = binReader->Read<wchar_t>();
			// > Retrieve & Set OffsetX [FontMetric::OffsetX]
			fontMetric->OffsetX = binReader->Read<wchar_t>();
			// > Retrieve & Set OffsetY [FontMetric::OffsetY]
			fontMetric->OffsetY = binReader->Read<wchar_t>();
			// > Retrieve & Set AdvanceX [FontMetric::AdvanceX]
			fontMetric->AdvanceX = binReader->Read<wchar_t>();
			// > Retrieve & Set Page [FontMetric::Page]
			fontMetric->Page = binReader->Read<char>();
			// > Retrieve Channel (BITFIELD!!!) 
			int channel = binReader->Read<char>();
			//	> See documentation for BitField meaning [FontMetrix::Channel]
			if (channel == 1)
			{ 
				fontMetric->Channel = 2;
			} 
			else if (channel == 2) 
			{ 
				fontMetric->Channel = 1;
			}
			else if (channel == 4)
			{ 
				fontMetric->Channel = 0;			 
			} 
			else if (channel == 8) 
			{ 
				fontMetric->Channel = 3;
			}
			// > Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
			fontMetric->TexCoord.x = (float)x/pSpriteFont->m_TextureWidth;
			fontMetric->TexCoord.y = (float)y/pSpriteFont->m_TextureHeight;
		}
		else
		{
			Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): CharacterId is not valid");
		}		
	}
		
	// DONE :)

	delete binReader;
	return pSpriteFont;
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
