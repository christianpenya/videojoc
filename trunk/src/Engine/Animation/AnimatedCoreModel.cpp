#include "AnimatedCoreModel.h"
#include "Base\XML\tinyxml2\tinyxml2.h"
#include "Base/XML/XML.h"

CAnimatedCoreModel::CAnimatedCoreModel(const std::string& aName)
    : CName(aName)
{}

CAnimatedCoreModel::~CAnimatedCoreModel() {}


void CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;

    //buscar calCoreModel?

    //LoadMesh(lFilename);
    //LoadSkeleton(lFilename);
    //LoadAnimation(lFilename);

    // Com carreguem els materials?

}
