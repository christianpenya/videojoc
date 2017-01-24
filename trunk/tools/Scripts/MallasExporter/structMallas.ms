global ePosition = 0x0001
global eNormal = 0x0002
global eTangent = 0x0004
global eBinormal = 0x0008
global eUV = 0x0010
global eUV2 = 0x0020
global eColor = 0x0040
global ePosition4 = 0x0080
global eDummy = 0x0100


struct sMaterial (
	m_name = "",
	m_aIB = #(),
	m_VBMap = #(),
	m_VtxsMap=dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]",
	m_aVB = #(),
	flag_value = 0,

	fn addVertex p_Vertex =(
		local vx = p_Vertex
		local vertexID = convertVtx(vx)
		local posInMap = finditem m_VBMap vertexID
		if posInMap == 0 then (
			append m_VBMap vertexID
			append m_aVB p_Vertex
			local posInMap2 = m_VBMap.count-1   --local posInMap2 = finditem m_VBMap vertexID
			append m_aIB posInMap2
		)else(
			append m_aIB posInMap
		)

	),

	function DotNetAddVertex p_Vertex = (
		local vx = p_Vertex
		local vertexID = convertVtx(vx)
		local l_Idx = 0
		if (m_VtxsMap.ContainsKey (vertexID))==false then
		(
			append m_aVB p_Vertex
			l_Idx=m_aVB.count-1
			m_VtxsMap.Add vertexID l_Idx
			append m_aIB l_Idx
		)else(
			l_Idx=m_VtxsMap.Item[vertexID]
			append m_aIB l_Idx
		)

	),


	fn setFlags mMaterial = (
		


		flag_value = bit.or ePosition eNormal
		


		
		local diffuseE = mMaterial.diffuseMapEnable
		if diffuseE == true then (
			diffBitmap = mMaterial.diffuseMap.bitmap 
			if diffBitmap == undefined then (
				diffuseE = false
			)
		)
		local bumpE = mMaterial.bumpMapEnable
		if bumpE == true then (
			bumpBitmap = mMaterial.bumpMap.bitmap
			if bumpBitmap == undefined then (
				bumpE = false
			)
		)
		local specularE = mMaterial.specularMapEnable
		if specularE == true then(
			specularBitmap = mMaterial.specularMap.bitmap
			if specularBitmap == undefined then (
				specularE = false
			)
		)
		local glossinessE = mMaterial.glossinessMapEnable
		if glossinessE == true then(
			glossineassBitmap = mMaterial.glossinessMap.bitmap
			if glossineassBitmap == undefined then (
				glossinessE = false
			)
		)
		local selfIllumE = mMaterial.selfIllumMapEnable
		if selfIllumE == true then (
			selfIllumBitmap = mMaterial.selfIllumMap.bitmap
			if selfIllumBitmap == undefined then (
				selfIllumE = false
			)
		)

		if specularE or bumpE or diffuseE or glossinessE then(
			format "UV\n"
			flag_value = bit.or flag_value eUV
			if selfIllumE then(
				format "UV2\n"
				flag_value = bit.or flag_value eUV2
			)
		)
		if bumpE then (
			format "Tang and Bin\n"
			flag_value = bit.or flag_value eTangent
			flag_value = bit.or flag_value eBinormal
		)

		
		
	)
)

fn convertVtx p_Vertex = (
			local l_Text = stringstream ""

			local pos = p_Vertex.m_pos
			format "%,%,%," pos.x pos.y pos.z to:l_Text

			local norm = p_Vertex.m_norm
			format "%,%,%," norm.x norm.y norm.z to:l_Text

			local uv = p_Vertex.m_uv
			format "%,%," uv.x uv.y to:l_Text

			return (l_Text as string)

)

fn TranslationToRH translation = (
	return point3 -translation.x translation.z translation.y
)

fn TranslationUV uv = (
	return point2 uv.x (1.0 - uv.y)
)




struct sVertex (
	m_pos = [0,0,0],
	m_norm = [0,0,0],
	m_uv = [0,0],
	m_uv2 = [0,0],
	m_binormal = [0,0,0],
	m_tangencial = [0,0,0]
	
)

struct sBBox (
	m_min = [0,0,0],
	m_max = [0,0,0]

)

struct sBSphere (
	m_center = [0,0,0],
	m_radius = 0
)



