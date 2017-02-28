#include "Scripts/lua_utils.h"
#include "Scripts/Script.h"
#include "Engine/Engine.h"
#include "Base/Utils/Logger.h"

namespace lua
{
void LogError(const std::string & error )
{
    LOG_ERROR_APPLICATION("[LUA]%s", error.c_str());
}
void LogWarning(const std::string & war)
{
    LOG_WARNING_APPLICATION("[LUA]%s", war.c_str());
}
void LogInfo(const std::string & info)
{
    LOG_INFO_APPLICATION("[LUA]%s", info.c_str());
}

//template <> void BindLibrary<Engine>(lua_State *aLua)

/*template <CCameraController> void OpenLibraries<CCameraController>(lua_State *aLua)
//template <> void BindClass<CTransform>(lua_State *aLua)
{
    module(aLua)
    [
        class_<CCameraController>("CameraController")
        .def(constructor<>())
        .def_readwrite("position", &CCameraController::m_Position)
        /*.def_readwrite("rotation", &CCameraController::m_Rotation)
        .def_readwrite("scale", &CTransform::m_Scale)*/
/*    ];
}*/

//template <> void BindClass<CEngine>(lua_State *aLua)
template <> void OpenLibraries(lua_State *aLua)
{
    REGISTER_LUA_FUNCTION(aLua, "log_error", &LogError);
    REGISTER_LUA_FUNCTION(aLua, "log_warning", &LogWarning);
    REGISTER_LUA_FUNCTION(aLua, "log_info", &LogInfo);

    module(aLua)
    [
        class_<CEngine>("Engine")
        .def(constructor<>())
        .def_readwrite("m_FileAnimatedModelManager", &CEngine::m_FileAnimatedModelManager)
        .def_readwrite("m_FileEffectManager", &CEngine::m_FileEffectManager)
        .def_readwrite("m_FileMaterialManager", &CEngine::m_FileMaterialManager)
        .def_readwrite("m_FileLightManager", &CEngine::m_FileLightManager)
        .def_readwrite("m_FileSceneManager", &CEngine::m_FileSceneManager)
        .def_readwrite("m_FileShaderManager", &CEngine::m_FileShaderManager)
        .def_readwrite("m_FileTechniquePoolManager", &CEngine::m_FileTechniquePoolManager)
        .def_readwrite("m_FileTextureManager", &CEngine::m_FileTextureManager)
        .def_readwrite("m_FileActionManager", &CEngine::m_FileActionManager)
        .def_readwrite("m_FileRenderPipeline", &CEngine::m_FileRenderPipeline)
        .def_readwrite("m_FileDefaultMaterial", &CEngine::m_FileDefaultMaterial)

    ];
}

}