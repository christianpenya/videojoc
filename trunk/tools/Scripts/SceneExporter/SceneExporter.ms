fileIn "xml.ms"

fn GetCores =
(
	uniqueInstance = #()
	for obj in selection do
	(
		print("obj in selection: " + obj.name)
		--if (InstanceMgr.GetInstances obj &instances) > 1 then
		--(
			
			--if findItem uniqueInstance instances[1] == 0 do
				--append uniqueInstance instances[1]
		--)
		--else
		--(
			append uniqueInstance obj
		--)
	)
	return uniqueInstance
)

fn Point3ToString aPoint3 = 
(		
	local l_Text = stringstream ""
	format "% " -aPoint3.x to:l_Text
 	format "% " aPoint3.z to:l_Text
 	format "%" aPoint3.y to:l_Text
	return (l_Text as string)
)

fn ExportScene =
(
	local xmlDoc = XmlDocument()
	local rootNode = XmlNode tag:"scene"
	xmlDoc.rootNode = rootNode
	
	local f = getSaveFilename "scene.xml" filename:( (getdir #userScripts) + "\\" + (getFilenameFile maxFilename)) \
																 types:"XML File(.xml)|*.xml|All|*.*|"

	if (f!=undefined) then
	(
		xmlDoc.filename = f
		
		--print(LayerManager.count)
		
		local id_light = 0;
		
		for i = 0 to LayerManager.count - 1 do
		(
			--Layer tag
			local layerProperties = LayerManager.getLayer i 
			
			local layerNode = XmlNode tag:"layer" 
			rootNode.AddChild layerNode
			
			layerNode.AddAttribute "name" layerProperties.name
			
			-- Seleccionamos temporalmente los objetos de esta cap para encontrar solo estos objetos
			layerProperties.select true	
			local uniqueObjects = GetCores()			
			layerProperties.select false
			
 			for o in uniqueObjects do
 			(
			--print (superclassOf o)
			if (superclassOf o == GeometryClass) then 
			(
 					-- meshes
					local sceneMeshNode = XmlNode tag:"scene_mesh"
 					layerNode.AddChild sceneMeshNode
					sceneMeshNode.AddAttribute "name" o.name
					local filename = "data/meshes/reclusion/" + o.name + ".mesh"
					sceneMeshNode.AddAttribute "mesh" filename
					
					-- transform
					local transformNode = XmlNode tag:"transform"
 					sceneMeshNode.AddChild transformNode

					--print (classOf o.transform.position)
					--print (classOf o.transform.rotation)
					--print (classOf o.transform.scale)
					
					local tmp = Point3ToString(o.transform.position)
					transformNode.AddAttribute "position" tmp
					
					transformNode.AddAttribute "yaw" o.transform.rotation.x
					transformNode.AddAttribute "pitch" o.transform.rotation.z
					transformNode.AddAttribute "roll" o.transform.rotation.y
					
					tmp = Point3ToString (o.transform.scale)
					transformNode.AddAttribute "scale" tmp
				)
  				else if (superclassOf o == Light) then
  				(
					--lights
  					local sceneLightNode = XmlNode tag:"scene_light" 
  					layerNode.AddChild sceneLightNode
 					sceneLightNode.AddAttribute "name" o.name
 					sceneLightNode.AddAttribute "id_light" id_light
					id_light = id_light + 1
  				)
 			)
		)
	
		xmlDoc.SaveXmlDocument()
	)
)


ExportScene()


