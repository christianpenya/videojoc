#include "TextureManager.h"

CTextureManager::CTextureManager() {}

CTextureManager::~CTextureManager()
{
    mTextures.Destroy();
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

bool CTextureManager::Reload()
{
    // #TODO
    return false;
}

bool CTextureManager::AddTexture(CTexture &aTexture)
{
    return mTextures.Add(aTexture.GetName(), &aTexture);
}
