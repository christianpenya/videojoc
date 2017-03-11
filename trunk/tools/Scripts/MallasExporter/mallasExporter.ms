fileIn "structMallas.ms"
fileIn "UABVertexNormals.ms"
fileIn "xml.ms"
fileIn "BinFileWritter.ms"

global ePosition = 0x0001
global eNormal = 0x0002
global eTangent = 0x0004
global eBinormal = 0x0008
global eUV = 0x0010
global eUV2 = 0x0020
global eColor = 0x0040
global ePosition4 = 0x0080
global eDummy = 0x0100

fn preProcess = (
	
	local lSelectionMaterial = gSelection.material
	
	if lSelectionMaterial == undefined then (
		lSelectionMaterial = StandardMaterial()
		lSelectionMaterial.name = gSelection.name+"Material"
		lSelectionMaterial.diffuse = gSelection.wirecolor
	)
	if classOf lSelectionMaterial == StandardMaterial then	(
		local tempMaterial = sMaterial()
		tempMaterial.m_name = lSelectionMaterial.name
		tempMaterial.setFlags(lSelectionMaterial)
		append gMatsArray tempMaterial
	)else if classOf lSelectionMaterial == Multimaterial then	(
		local lSubMaterialsCount = lSelectionMaterial.numsubs			
		for iSubMaterial =  1 to lSubMaterialsCount do(
			local lSubMaterial = lSelectionMaterial[iSubMaterial]
			local tempMaterial = sMaterial()
			tempMaterial.m_name = lSubMaterial.name
			tempMaterial.setFlags(lSubMaterial)
			append gMatsArray tempMaterial
		)
	)
)

fn process = (
	for nFace = 1 to gNFaces do (
		local IdxsFace = getFace gSelection nFace

		local Vtx1 = sVertex()
		local Vtx2 = sVertex()
		local Vtx3 = sVertex()

		local auxPos1 = (getVert gSelection IdxsFace.x) * gInverseTransform
		local auxPos2 = (getVert gSelection IdxsFace.y) * gInverseTransform
		local auxPos3 = (getVert gSelection IdxsFace.z) * gInverseTransform

		Vtx1.m_pos = TranslationToRH(auxPos1)
		Vtx2.m_pos = TranslationToRH(auxPos2)
		Vtx3.m_pos = TranslationToRH(auxPos3)

		--SAVING POINTS FOR BOUNDING BOX AND SPHERE		
		append gAX Vtx1.m_pos.x
		append gAX Vtx2.m_pos.x
		append gAX Vtx3.m_pos.x

		append gAY Vtx1.m_pos.y
		append gAY Vtx2.m_pos.y
		append gAY Vtx3.m_pos.y

		append gAZ Vtx1.m_pos.z
		append gAZ Vtx2.m_pos.z
		append gAZ Vtx3.m_pos.z


		UABCalcVertexsNormals gSelection
		local lFaceNormal = getFaceNormal gSelection nFace
		local lSmoothValue = getFaceSmoothGroup gSelection nFace

		local auxNorm1 = (UABGetVertexNormal gSelection IdxsFace.x lSmoothValue lFaceNormal) --* gInverseTransform
		local auxNorm2 = (UABGetVertexNormal gSelection IdxsFace.y lSmoothValue lFaceNormal) --* gInverseTransform
		local auxNorm3 = (UABGetVertexNormal gSelection IdxsFace.z lSmoothValue lFaceNormal) --* gInverseTransform

		Vtx1.m_norm = TranslationToRH(auxNorm1)
		Vtx2.m_norm = TranslationToRH(auxNorm2)
		Vtx3.m_norm = TranslationToRH(auxNorm3)

		local IdxsMap = meshop.getMapFace gSelection 1 nFace

		local auxUV_1 = meshop.getMapVert gSelection 1 IdxsMap.x
		local auxUV_2 = meshop.getMapVert gSelection 1 IdxsMap.y
		local auxUV_3 = meshop.getMapVert gSelection 1 IdxsMap.z

		Vtx1.m_uv = TranslationUV(auxUV_1)
		Vtx2.m_uv = TranslationUV(auxUV_2)
		Vtx3.m_uv = TranslationUV(auxUV_3)
		
		try
		(
			local IdxsMap2 = meshop.getMapFace gSelection 2 nFace

			local auxUV2_1 = meshop.getMapVert gSelection 2 IdxsMap2.x
			local auxUV2_2 = meshop.getMapVert gSelection 2 IdxsMap2.y
			local auxUV2_3 = meshop.getMapVert gSelection 2 IdxsMap2.z


			Vtx1.m_uv2 = TranslationUV(auxUV2_1)
			Vtx2.m_uv2 = TranslationUV(auxUV2_2)
			Vtx3.m_uv2 = TranslationUV(auxUV2_3)
		)
		catch
		(
		)

		local MaterialID = 0
		if gMatsArray.count == 1 then(
			MaterialID = 1
		)else(
			MaterialID = getFaceMatID gSelection nFace
		)

		--gMatsArray[MaterialID].addVertex(Vtx1)
		--gMatsArray[MaterialID].addVertex(Vtx3)
		--gMatsArray[MaterialID].addVertex(Vtx2)

		gMatsArray[MaterialID].DotNetAddVertex(Vtx1)
		gMatsArray[MaterialID].DotNetAddVertex(Vtx3)
		gMatsArray[MaterialID].DotNetAddVertex(Vtx2)
	)
)

fn writeToBin = (
	local header = 0xFE55
	local footer = 0x55FE

	local writter = BinFileWritter()
	writter.ChangeVerbose(true)
	writter.Open $.name "C:/mallas/" ".mesh"

	writter.PrintUnsignedShort(header)
	writter.PrintUnsignedShort(gMatsArray.count)
	for lMat in gMatsArray do (
		writter.PrintString(lMat.m_name)

  format "%\n" lMat.flag_value
		writter.PrintUnsignedShort(lMat.flag_value)
		writter.PrintUnsignedShort(lMat.m_aVB.count)
		for lVrtx in lMat.m_aVB do(
		 
			writter.PrintFloat(lVrtx.m_pos.x)
			writter.PrintFloat(lVrtx.m_pos.y)
			writter.PrintFloat(lVrtx.m_pos.z)

			writter.PrintFloat(lVrtx.m_norm.x)
			writter.PrintFloat(lVrtx.m_norm.y)
			writter.PrintFloat(lVrtx.m_norm.z)

			if(bit.and lMat.flag_value eTangent) == eTangent then(
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0) 
			)
			if(bit.and lMat.flag_value eBinormal) == eBinormal then (
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0)
				writter.PrintFloat(0.0)
				)
			if(bit.and lMat.flag_value eUV)  == eUV then (

				writter.PrintFloat(lVrtx.m_uv.x)
				writter.PrintFloat(lVrtx.m_uv.y)

				if(bit.and lMat.flag_value eUV2) == eUV2 then (
					writter.PrintFloat(lVrtx.m_uv2.x)
					writter.PrintFloat(lVrtx.m_uv2.y)
				)
			)

		)
		writter.PrintUnsignedShort(lMat.m_aIB.count)
		for idx in lMat.m_aIB do(
			writter.PrintUnsignedShort(idx)
		)
	)

	writter.PrintFloat(gBBox.m_min.x)
	writter.PrintFloat(gBBox.m_min.y)
	writter.PrintFloat(gBBox.m_min.z)

	writter.PrintFloat(gBBox.m_max.x)
	writter.PrintFloat(gBBox.m_max.y)
	writter.PrintFloat(gBBox.m_max.z)


	writter.PrintFloat(gBSphere.m_center.x)
	writter.PrintFloat(gBSphere.m_center.y)
	writter.PrintFloat(gBSphere.m_center.z)

	writter.PrintFloat(gBSphere.m_radius)
	writter.PrintFloat(0)
	writter.PrintFloat(0)

	writter.PrintUnsignedShort(footer)
	writter.Close()

)

fn boundingBox = (

	local lBBox = sBBox()

	lBBox.m_min.x = amin(gAX)
	lBBox.m_min.y = amin(gAY)
	lBBox.m_min.z = amin(gAZ)

	lBBox.m_max.x = amax(gAX)
	lBBox.m_max.y = amax(gAY)
	lBBox.m_max.z = amax(gAZ)

	return lBBox


)

fn boundingSphere = (
	local lBSphere = sBSphere()

	lBSphere.m_center.x = ((gBBox.m_max.x - gBBox.m_min.x) / 2)
	lBSphere.m_center.y = ((gBBox.m_max.y - gBBox.m_min.y) / 2)
	lBSphere.m_center.z = ((gBBox.m_max.z - gBBox.m_min.z) / 2)

	local tmpDistance = 0
	local auxDistance = 0
	local tmpPoint = [0,0,0]

	for i = 1 to gAX.count do (
		tmpPoint = point3 gAX[i] gAY[i] gAZ[i]
		auxDistance = abs ( distance lBSphere.m_center tmpPoint)
		if auxDistance > tmpDistance then (

			tmpDistance = auxDistance

		)

	)
	
	lBSphere.m_radius = tmpDistance


	return lBSphere

)

fn ExportSingleObjectMesh aObj =
(
	global gSelection = copy aObj

	global gMatsArray = #()

	global gAX = #()
	global gAY = #()
	global gAZ = #()

	convertToMesh gSelection
	global gInverseTransform = inverse gSelection.transform
	preProcess()

	if gMatsArray.count > 0 then (

			global gNFaces = getNumFaces gSelection
			global gNVertex = getNumVerts gSelection
			process()
			global gBBox = boundingBox()
			global gBSphere = boundingSphere()
			writeToBin()
	)	
	try
		delete gSelection
	catch (
		messageBox "Ha ocurrido un error al eliminar la copia: % del objeto: %" gSelection.name $.name
		results = undefined
	)
)

fn ExportMeshSelection =
(
	if  $ == undefined then
	(
		format "No hay objeto seleccionado\n"
	)
	else
	(
		try
		(
			format "Multi objects\n"
			local lSelection = $.count
			
      for obj in $ do (
				ExportSingleObjectMesh obj
				)
        
		)
		catch
		(
			format "Single Object\n"
			ExportSingleObjectMesh $
		)
	)
)

ExportMeshSelection()
