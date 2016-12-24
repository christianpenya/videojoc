fileIn "xml.ms"
fileIn "structMaterial.ms"


function preProcess = (
	for object in $ do (
		iMaterial = object.material
		if classOf iMaterial == StandardMaterial then	(	
			appendIfUnique mArrayMaterial iMaterial
		)else if classOf iMaterial == Multimaterial then	(
			local lSubMaterialsCount = iMaterial.numsubs			
			for iSubMaterial =  1 to lSubMaterialsCount do(
				local lSubMaterial = iMaterial[iSubMaterial]
				appendIfUnique mArrayMaterial lSubMaterial
			)
		)
	)
)

function process = (
	for mMaterial in mArrayMaterial do (
		
		--Creamos un objeto con la estructura y añadimos el nombre
		local lSMaterial = sMaterial()
		lSMaterial.m_name = mMaterial.name
		
		lSMaterial.m_opacity = mMaterial.opacity
		
		--Obtenemos los datos referentes al diffuse del material
		lSMaterial.m_diffuseEnabled = mMaterial.diffuseMapEnable
		if lSMaterial.m_diffuseEnabled == true then (
			lSMaterial.m_diffuseMap = mMaterial.diffuseMap
			lSMaterial.m_diffuseBitmap = mMaterial.diffuseMap.bitmap
			if lSMaterial.m_diffuseBitmap != undefined then(
				--lSMaterial.m_diffuseBitmapTexture = iMaterial.diffuseMap.bitmap .filename
				lSMaterial.m_diffuseColor = mMaterial.diffuse
				lSMaterial.m_diffuseAmount = mMaterial.diffuseMapAmount
			)else (
				lSMaterial.m_diffuseEnabled == false
			)
		)	
		
		--Datos bump
		lSMaterial.m_bumpEnabled = mMaterial.bumpMapEnable
		if lSMaterial.m_bumpEnabled == true then (
			lSMaterial.m_bumpMap = mMaterial.bumpMap
			lSMaterial.m_bumpBitmap = mMaterial.bumpMap.bitmap
			if lSMaterial.m_bumpBitmap != undefined then (				
					lSMaterial.m_bumpAmount = mMaterial.bumpMapAmount				
			)else (
					lSMaterial.m_bumpEnabled = false
			)			
			
		)
		--Datos self illum
		lSMaterial.m_selfIllumEnabled = mMaterial.selfIllumMapEnable
		if lSMaterial.m_selfIllumEnabled == true then (
			lSMaterial.m_selfIllumMap = mMaterial.selfIllumMap
			lSMaterial.m_selfIllumBitmap = mMaterial.selfIllumMap.bitmap
			if lSMaterial.m_selfIllumBitmap != undefined then(
				lSMaterial.m_selfIllumAmount = mMaterial.selfIllumMapAmount
			)else (
				lSMaterial.m_selfIllumEnabled = false
			)
		)
		
		--Datos ambient map
		lSMaterial.m_ambientEnabled = mMaterial.ambientMapEnable
		if lSMaterial.m_ambientEnabled == true then (
			lSMaterial.m_ambientMap = mMaterial.ambientMap
			lSMaterial.m_ambientBitmap = mMaterial.ambientMap.bitmap
			if lSMaterial.m_ambientBitmap != undefined then(
				lSMaterial.m_ambientAmount = mMaterial.ambientMapAmount
				lSMaterial.m_ambientColor = mMaterial.ambient
			)else(
				lSMaterial.m_ambientMapEnabled = false
			)
		)		
		
		--Datos specular level
		lSMaterial.m_specularLevelEnabled = mMaterial.specularLevelMapEnable
		if lSMaterial.m_specularLevelEnabled == true then (
			lSMaterial.m_specularLevelMap = mMaterial.specularLevelMap
			lSMaterial.m_specularLevelBitmap = mMaterial.specularLevelMap.bitmap
			if lSMaterial.m_specularLevelBitmap != undefined then(
				lSMaterial.m_specularLevelAmount = mMaterial.specularLevelMapAmount
			)else(
				lSMaterial.m_specularLevelEnabled = false
			)
		)		
		
		--Datos specular
		lSMaterial.m_specularEnabled = mMaterial.specularMapEnable
		if lSMaterial.m_specularEnabled == true then (
			lSMaterial.m_specularMap = mMaterial.specularMap
			lSMaterial.m_specularBitmap = mMaterial.specularMap.bitmap
			if lSMaterial.m_specularBitmap != undefined then(
				lSMaterial.m_specularAmount = mMaterial.specularMapAmount
				lSMaterial.m_specularColor = mMaterial.specular
			)else(
				lSMaterial.m_specularEnabled = false
			)
		)	
		
		--Datos glossiness
		lSMaterial.m_glossinessEnabled = mMaterial.glossinessMapEnable
		if lSMaterial.m_glossinessEnabled == true then (
			lSMaterial.m_glossinessMap = mMaterial.glossinessMap
			lSMaterial.m_glossinessBitmap = mMaterial.glossinessMap.bitmap
			if lSMaterial.m_glossinessBitmap != undefined then(
				lSMaterial.m_glossinessAmount = mMaterial.glossinessMapAmount
			)else(
				lSMaterial.m_glossinessEnabled = false
			)
		)	
		
		append mArrayMaterialStruct lSMaterial 
	)	
)


function parse outPath = (
	local rootNode = XmlNode tag:"materials"
	xmlDoc.rootNode = rootNode
	for object in mArrayMaterialStruct do(
		local matNode = XmlNode tag:"material"
		rootNode.addChild matNode
		matNode.AddAttribute "name" object.m_name
		if object.m_diffuseEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "diffuse"			
			local filename = copyTexture object.m_diffuseBitmap outPath
			textNode.addAttribute "filename" filename
			
			local colorNode = XmlNode tag:"color"
			matNode.addChild colorNode
			colorNode.addAttribute "type" "diffuse"
			local strColor = colorToString(object.m_diffuseColor)
			colorNode.addAttribute "value" strColor
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "diffuse"
			paramNode.addAttribute "value" object.m_diffuseAmount			
		)
		if object.m_bumpEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "bump"
			local filename = copyTexture object.m_bumpBitmap outPath
			textNode.addAttribute "filename" filename
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "bump"
			paramNode.addAttribute "value" object.m_bumpAmount
		)
		if object.m_selfIllumEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "selfIllum"
			local filename = copyTexture object.m_selfIllumBitmap outPath
			textNode.addAttribute "filename" filename
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "selfIllum"
			paramNode.addAttribute "value" object.m_selfIllumAmount

		)
		
		if object.m_ambientEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "ambient"
			local filename = copyTexture object.m_ambientBitmap outPath
			textNode.addAttribute "filename" filename

			local colorNode = XmlNode tag:"color"
			matNode.addChild colorNode
			colorNode.addAttribute "type" "ambient"
			local strColor = colorToString(object.m_ambientColor)
			colorNode.addAttribute "value" strColor
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "ambient"
			paramNode.addAttribute "value" object.m_ambientAmount
		)

		if object.m_specularLevelEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "specularLevel"
			local filename = copyTexture object.m_specularLevelBitmap outPath
			textNode.addAttribute "filename" filename
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "specularLevel"
			paramNode.addAttribute "value" object.m_specularLevelAmount
		)

		if object.m_specularEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "specular"
			local filename = copyTexture object.m_specularBitmap outPath
			textNode.addAttribute "filename" filename

			local colorNode = XmlNode tag:"color"
			matNode.addChild colorNode
			colorNode.addAttribute "type" "specular"
			local strColor = colorToString(object.m_specularColor)
			colorNode.addAttribute "value" strColor
			
			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "specular"
			paramNode.addAttribute "value" object.m_specularAmount
		)

		if object.m_glossinessEnabled == true then(
			local textNode = XmlNode tag:"texture"
			matNode.addChild textNode
			textNode.addAttribute "type" "glossiness"
			local filename = copyTexture object.m_glossinessBitmap outPath
			textNode.addAttribute "filename" filename

			local paramNode = XmlNode tag:"parameter"
			matNode.addChild paramNode
			paramNode.addAttribute "type" "float"
			paramNode.addAttribute "name" "glossiness"
			paramNode.addAttribute "value" object.m_glossinessAmount
		)


	
	)
	
)

function makeData outPath = (

	local finalPath = outPath + "\\textures"
	if not doesFileExist outPath then(
		makeDir outPath	
	)
	if not doesFileExist finalPath then(
		makeDir finalPath	
	)	
	
)

function copyTexture lDiffuseBitmap outPath = (	
	local lTextureName = filenameFromPath(lDiffuseBitmap .filename)
	local finalPath = outPath + "\\textures\\" + lTextureName
	copyFile (openBitMap lDiffuseBitmap .filename).filename finalPath
	return "textures\\" + lTextureName
)


function main = (
	if $ != undefined then (				
		global mArrayMaterial = #()
		preProcess()
		if mArrayMaterial.count > 0 then(			
			global mArrayMaterialStruct = #()
			global file = getSaveFilename "Save XML File" filename:( (getdir #userscripts) + "\\" + (getFilenameFile "materials.xml")) \
			types:"XML File(.xml)|*.xml|All|*.*|"
			global xmlDoc = XmlDocument()
			if file != undefined then (
				if doesFileExist file then(
					deleteFile file
				)
				xmlDoc.filename = file
				makeData(getFilenamePath file)
				process()
				parse(getFilenamePath file)
				xmlDoc.SaveXmlDocument()
			)		
		)
	)
)


main()










