-- example.lua
var1 = 12
var2 = 25
result = 150

bar = {}
bar[3] = "hi"
bar["key"] = "there"

f_ShaderManager = "data/shaders.xml"
f_Effects = "data/effects.xml"
f_TPManager = "data/techniques_pool.xml"
f_LevelMaterial = "data/materials/materiales_zona_administracion.xml"
f_DefaultMaterial = "data/materials/default.xml"
f_Texture = "data/textures/BARK5.jpg"
f_AnimatedModel = "data/animated_models.xml"
f_LightManager = "data/lights.xml"
f_SceneManager = "data/scenes.xml"
f_ActionManager = "data/config/input_config.xml"
f_RenderPipeline = "data/render_pipeline.xml"
f_ParticleManager = "data/particles.xml"

f_CinematicsManager = "data/cinematics.xml";
f_SoundPath = "data/sound/";
f_SoundBankFile = "SoundbanksInfo.xml";
f_SpeakersFile = "speakers.xml";
f_EventManager = "data/scenes/events.xml";
f_NavMeshManager = "data/navMesh.xml"
f_EnemiesManager = "data/enemies.xml"
f_LaserManager = "data/laser.xml"

function getFileEventManager()
	return f_EventManager
end

function getFileCinematicManager()
	return f_CinematicsManager
end

function getSoundFilesPath()
	return f_SoundPath
end

function getSoundSpeakersFile()
	return f_SpeakersFile
end

function getSoundBankFile()
	return f_SoundBankFile
end

function getFileShaderManager()
	return f_ShaderManager
end

function getFileEffects()
	return f_Effects
end

function getFileTechniquePoolManager()
	return f_TPManager
end

function getFileLevelMaterial()
	return f_LevelMaterial
end

function getFileDefaultMaterial()
	return f_DefaultMaterial
end

function getFileTexture()
	return f_Texture
end

function getFileAnimatedModel()
	return f_AnimatedModel
end

function getFileLightManager()
	return f_LightManager
end

function getFileSceneManager()
	return f_SceneManager
end

function getActionManager()
	return f_ActionManager
end

function getRenderPipeline()
	return f_RenderPipeline
end

function getFileParticleManager()
	return f_ParticleManager
end

function getFileNavMeshManager()
	return f_NavMeshManager
end

function getFileEnemiesManager()
	return f_EnemiesManager
end

function getFileLaserManager()
	return f_LaserManager
end
