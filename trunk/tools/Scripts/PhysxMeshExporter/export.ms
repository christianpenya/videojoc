fileIn "Math.ms"
fileIn "Utils.ms"
fileIn "UABVertexNormals.ms"
fileIn "material.ms"
global isDebug = false

fn writeMesh arx VtxID matFlags vertexs indexs BBMin BBMax BSphere forPhysx =
(
	format "# Exportando Physx: %\n" forPhysx
	
	--por material
	for i=1 to vertexs.count do
	(
		format "#% -> % vertexes\n" i vertexs[i].count
		writeShort arx vertexs[i].count #unsigned--total vertices
		-- por cara
		if forPhysx == true then
		(
			-- por cara
			for j = 1 to vertexs[i].count do
			(
				-- por vertice
				local limit = 0
				
				for k = 1 to vertexs[i][j].count do
				(				
					--solo escribimos la info de los 3 primeros float, pos x y z
					if k < 4 then
					(
						print vertexs[i][j][k]
						--format "\n" 
						--format "vertexs[j] \n"
						--format "vertexs[k] \n"
						writeFloat arx vertexs[i][j][k] --info vertice
						
					)
					else
					(
						continue
					)
				)
			)
		)
		
		format "#% -> % indices\n" i indexs[i].count
		writeShort arx indexs[i].count #unsigned--indices
		for j = 1 to indexs[i].count do
		(
			print indexs[i][j]
			writeShort arx indexs[i][j] #unsigned
		)
	)
)

fn processFaces arx Obj forPhysx =
(
	if classof Obj != Editable_mesh then
	(
		local l_Obj = copy Obj
		convertToMesh l_Obj
	) else (
		local l_Obj = Obj
	)
	
	local Vt1 = undefined
	local Vt2 = undefined
	local Vt3 = undefined
	local Color1 = undefined
	local Color2 = undefined
	local Color3 = undefined
	local Nm1 = undefined
	local Nm2 = undefined
	local Nm3 = undefined
	local TUVMap1 = undefined
	local TUVMap2 = undefined
	local TUVMap3 = undefined
	local T2UVMap1 = undefined
	local T2UVMap2 = undefined
	local T2UVMap3 = undefined
	
	local BBMin = point3 0 0 0
	local BBMax = point3 0 0 0
	local BSphere = #()
	
	local PosID = 0x0001
	local ColorID = 0x0000
	local NmID = 0x0000
	local UVID = 0x0000
	local UV2ID = 0x0000
	local BnmID = 0x0000
	local TngID = 0x0000
	
	local InvTransform = inverse obj.transform
	local NumVtxs = getNumVerts l_Obj
	local nFaces = getNumFaces l_Obj
	local vertexs = #()
	local indexs = #()
	local vertexsDictionary = #()
	local matFlags = #()
	

	local UABVtxsNormals = #()
	for b = 1 to NumVtxs do 
	( 
		UABVtxsNormals[b] = #() 
		for t = 1 to 32 do 
		( 
			UABVtxsNormals[b][t] = point3 0 0 0  
		) 
	)
	
	UABCalcVertexsNormals l_Obj nFaces &UABVtxsNormals
	
	if l_Obj.material != undefined then
	(	
		NmID = 0x0002
		local l_HasMultimaterial = classof l_Obj.material == Multimaterial
		local l_TotalMaterials = 1
		if l_HasMultimaterial then
			l_TotalMaterials=l_Obj.material.count
		for i=1 to l_TotalMaterials do
		(
			append vertexs #()
			append indexs #()
			append matFlags #()
			append vertexsDictionary (dotnetobject "System.Collections.Generic.Dictionary`2[System.String, System.UInt32]")
		)		
	) else (
		append vertexs #()
		append indexs #()
		append matFlags #()
		append vertexsDictionary (dotnetobject "System.Collections.Generic.Dictionary`2[System.String, System.UInt32]")
	)
	
	local BBFace = getFace l_Obj 1
	local BBStart = (getVert l_Obj BBFace.x)*InvTransform
	TranslationToRH &BBFace
	BBMax.x = BBStart.x
	BBMax.y = BBStart.y
	BBMax.z = BBStart.z
	BBMin.x = BBStart.x
	BBMin.y = BBStart.y
	BBMin.z = BBStart.z
	
	for i=1 to nFaces do
	(
		local IdxsFace = getFace l_Obj i
		Vt1=(getVert l_Obj IdxsFace.x)*InvTransform
		Vt2=(getVert l_Obj IdxsFace.y)*InvTransform
		Vt3=(getVert l_Obj IdxsFace.z)*InvTransform
		getBoundingBox Vt1 &BBMax &BBMin
		getBoundingBox Vt2 &BBMax &BBMin
		getBoundingBox Vt3 &BBMax &BBMin
		TranslationToRH &Vt1
		TranslationToRH &Vt2
		TranslationToRH &Vt3
	)
	
    lerp BBMin BBMax .5 &BSphere[1]
    BSphere[2] = 0
	for i=1 to nFaces do
	(	
		local IdxsFace = getFace l_Obj i
		local FaceNormal = getFaceNormal l_Obj i
		local SmoothValue = getFaceSmoothGroup l_Obj i
		
		Vt1=(getVert l_Obj IdxsFace.x)*InvTransform
		Vt2=(getVert l_Obj IdxsFace.y)*InvTransform
		Vt3=(getVert l_Obj IdxsFace.z)*InvTransform
		getBoundingSphere Vt1 &BSphere
		getBoundingSphere Vt2 &BSphere
		getBoundingSphere Vt3 &BSphere
		TranslationToRH &Vt1
		TranslationToRH &Vt2
		TranslationToRH &Vt3
		
		if l_Obj.material != undefined then
		(
			UABGetVertexNormal l_Obj IdxsFace.x SmoothValue FaceNormal UABVtxsNormals &Nm1
			UABGetVertexNormal l_Obj IdxsFace.y SmoothValue FaceNormal UABVtxsNormals &Nm2
			UABGetVertexNormal l_Obj IdxsFace.z SmoothValue FaceNormal UABVtxsNormals &Nm3
			Nm1 = normalize(Nm1)
			Nm2 = normalize(Nm2)
			Nm3 = normalize(Nm3)
			TranslationToRH &Nm1
			TranslationToRH &Nm2
			TranslationToRH &Nm3
			
			local l_IdMaterial = (mod ((getFaceMatID l_Obj i)-1) l_TotalMaterials)+1

			local l_DiffMap = undefined
			local l_LightMap  = undefined
			local l_RefMap = undefined 
			local l_BumpMap  = undefined
			
			if l_HasMultimaterial then
			(
				l_DiffMap = l_Obj.material.materialList[l_IdMaterial].diffuseMap
				l_LightMap = l_Obj.material.materialList[l_IdMaterial].selfIllumMap
				l_RefMap = l_Obj.material.materialList[l_IdMaterial].reflectionMap
				l_BumpMap = l_Obj.material.materialList[l_IdMaterial].bumpMap

				if (classof l_Obj.material.materialList[l_IdMaterial].bumpMap == Bitmaptexture) then
				(
					if (classof l_Obj.material.materialList[l_IdMaterial].bumpMap.bitmap == Bitmap) then
					(
						BnmID = 0x0008
						TngID = 0x0004
					)
				) 
				else if (classof l_Obj.material.materialList[l_IdMaterial].bumpMap == Normal_Bump) then
				(
					if (classof l_Obj.material.materialList[l_IdMaterial].bumpMap.normal_map.bitmap == Bitmap) then
					(
						BnmID = 0x0008
						TngID = 0x0004
					)
				)
				else(
					BnmID = 0x0000
					TngID = 0x0000
				)
			) else (
				l_DiffMap = l_Obj.material.diffuseMap
				l_LightMap = l_Obj.material.selfIllumMap
				l_RefMap = l_Obj.material.reflectionMap
				l_BumpMap = l_Obj.material.bumpMap

				if (classof l_Obj.material.bumpMap == Bitmaptexture) then
				(
					if (classof l_Obj.material.bumpMap.bitmap == Bitmap) then
					(
						BnmID = 0x0008
						TngID = 0x0004
					)
				) else if (classof l_Obj.material.bumpMap == Normal_Bump) then
				(
					if (classof l_Obj.material.bumpMap.normal_map.bitmap == Bitmap) then
					(
						BnmID = 0x0008
						TngID = 0x0004
					)
				)
			)
			
			if ((classOf l_DiffMap == Bitmaptexture) or (classOf l_RefMap == Bitmaptexture) or (classOf l_BumpMap == Bitmaptexture)) then
			(
				UVID = 0x0010
				local IdxsMap = meshop.getMapFace l_Obj 1 i
				TUVMap1= meshop.getMapVert l_Obj 1 IdxsMap[1]
				TUVMap2= meshop.getMapVert l_Obj 1 IdxsMap[2]
				TUVMap3= meshop.getMapVert l_Obj 1 IdxsMap[3]
			)
			
			if (classOf l_LightMap != UndefinedClass) and (meshop.getMapSupport l_Obj 2) then
			(
				UV2ID = 0x0020
				local IdxsMap = meshop.getMapFace l_Obj 2 i
				T2UVMap1= meshop.getMapVert l_Obj 2 IdxsMap[1]
				T2UVMap2= meshop.getMapVert l_Obj 2 IdxsMap[2]
				T2UVMap3= meshop.getMapVert l_Obj 2 IdxsMap[3]
			)
			
		) else (
			local l_IdMaterial = 1
			ColorID = 0x0040
			defaultVCFaces l_Obj
			local ColorFace = getVCFace l_Obj i
			Color1 = getVertColor l_Obj ColorFace.x
			Color2 = getVertColor l_Obj ColorFace.y
			Color3 = getVertColor l_Obj ColorFace.z
		)
		
		local v1 = #()
		local v2 = #()
		local v3 = #()
		createVertex Vt1 Color1 Nm1 TUVMap1 T2UVMap1 BnmID TngID &v1
		createVertex Vt2 Color2 Nm2 TUVMap2 T2UVMap2 BnmID TngID &v2
		createVertex Vt3 Color3 Nm3 TUVMap3 T2UVMap3 BnmID TngID &v3
		
		local id1
		local id2
		local id3
		getIndex vertexs v1 l_IdMaterial vertexsDictionary &id1
		getIndex vertexs v2 l_IdMaterial vertexsDictionary &id2
		getIndex vertexs v3 l_IdMaterial vertexsDictionary &id3
		
		append indexs[l_IdMaterial] id1
		append indexs[l_IdMaterial] id2
		append indexs[l_IdMaterial] id3

		local VtxID = 0x0000
		VtxID = PosID+ColorID+NmID+UVID+UV2ID+BnmID+TngID
		append matFlags[l_IdMaterial] VtxID

		UVID = 0x0000
		UV2ID = 0x0000
		BnmID = 0x0000
		TngID = 0x0000
	)
	
	free UABVtxsNormals
	
	TranslationToRH &BBMin
	TranslationToRH &BBMax
	TranslationToRH &BSphere[1]

	writeMesh arx VtxID matFlags vertexs indexs BBMin BBMax BSphere forPhysx
	
	if classof Obj != Editable_mesh then ( delete l_Obj )
)

fn writeMeshFromObject arx Obj	forPhysx =
(
	WriteShort arx 0xFE55
	print 0xFE55
	local nMaterials
	--getMaterials arx Obj
    processFaces arx Obj forPhysx
	WriteShort arx 0x55FE
	print 0x55FE
	)

fn createMeshFromObjects  =
(
	
	if  $ == undefined then
	(
		format "No hay objeto seleccionado\n"
	)
	else
	(
		--try
		--(
			
		--if lSelection > 1 then
			--(
--			for obj in $ do (
--				file=createfile ("C:\\mallas\\"+obj.name)
	--			file=fopen ("C:\\mallas\\"+obj.name+".mesh") "wb"
		--		writeMeshFromObject file obj
			--	FClose file
			--)
		--)
		--else
			--(
			
			format "Single Object\n\n"
			hola = $.name

			--file=createfile ("C:\\mallas\\"+hola+".mesh")
			format ("C:\\mallas\\"+hola+".mesh\n")
			
			file = fopen ("C:\\mallas\\"+hola+".mesh") "wb"
			
			format "temare\n"
			
			writeMeshFromObject file $ true
			
			format "en bragues"
			
			fclose file
			--)
		--)
		--catch
		--(
		--	format "BOOM! -> CLEAN UP"
		--	deletefile file.
		--	fclose file
		--)
	)
)

createMeshFromObjects()
