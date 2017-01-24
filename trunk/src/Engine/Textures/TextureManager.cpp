#include "TextureManager.h"

CTextureManager::CTextureManager()
{

}

CTextureManager::~CTextureManager()
{

}

CTexture* CTextureManager::GetTexture(const std::string& aFilename)
{
	if (!mTextures.Exist(aFilename))
	{
		Load(aFilename);
	}
	
	CTexture* lTexture = mTextures(aFilename);

	// log that ltexture does not exist
	return lTexture;
}

bool CTextureManager::Load( const std::string& aFilename)
{
	bool lOk = false;
	CTexture * lTexture = new CTexture(aFilename);
	if (lTexture->Load())
	{
		lOk = mTextures.Add(aFilename, lTexture);
	}
	else
	{
		base::utils::CheckedDelete(lTexture);
	}

	return lOk;
}

/*bool CTextureManager::Reload()
{
	mTextures.Clear();
	return true;
}*/
