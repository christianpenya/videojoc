struct sMaterial (
	m_name = "name",
	
	m_opacity = 0,
	
	m_diffuseEnabled = false,
	m_diffuseMap = undefined,
	m_diffuseBitmap = undefined,
	m_diffuseBitmapTexture = undefined,
	m_diffuseAmount = 0,
	m_diffuseColor = 0,
	
	m_bumpEnabled = false,
	m_bumpMap = undefined,
	m_bumpBitmap = undefined,
	m_bumpAmount = 0,

	m_selfIllumEnabled = false,
	m_selfIllumMap = undefined,
	m_selfIllumBitmap = undefined,
	m_selfIllumAmount = 0,
	
	m_ambientEnabled = false,
	m_ambientMap = undefined,
	m_ambientBitmap = undefined,
	m_ambientAmount = 0,
	m_ambientColor = undefined	,
	
	m_specularLevelEnabled = false,
	m_specularLevelMap = undefined,
	m_specularLevelBitmap = undefined,
	m_specularLevelAmount = 0,
	
	m_specularEnabled = false,
	m_specularMap = undefined,
	m_specularBitmap = undefined,
	m_specularAmount = 0,
	m_specularColor = undefined,
	
	m_glossinessEnabled = false,
	m_glossinessMap = undefined,
	m_glossinessBitmap = undefined,
	m_glossinessAmount = 0

	/*m_emissionEnabled = false,
	m_emissionMap = undefined,
	m_emissionBitmap = undefined,
	m_emissionAmount = 0*/

	
)


fn colorToString color =(

	local l_Text = stringstream ""
	format "%," color.r to:l_Text
	format "%," color.r to:l_Text
	format "%," color.r to:l_Text
	format "%," color.r to:l_Text
	return (l_Text as string)

)