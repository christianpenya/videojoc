#pragma once

#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "Utils\TemplatedMapVector.h"

#include "Texture.h"

class CTextureManager
{
public:
	CTextureManager();
	virtual ~CTextureManager();
	CTexture* GetTexture(const std::string& aFilename);
	bool Reload();
private:
	base::utils::CTemplatedMapVector<CTexture> mTextures;
	bool Load(const std::string& aFilename);
};

#endif