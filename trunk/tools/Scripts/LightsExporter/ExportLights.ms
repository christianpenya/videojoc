fileIn "xml.ms"

fn ExportLight =
(	
	local xmlDoc = XmlDocument()
	local rootNode = XmlNode tag:"lights"
	xmlDoc.rootNode = rootNode
	local f = getSaveFilename "lights.xml" filename:( (getdir #userScripts) + "\\" + (getFilenameFile maxFilename)) \
																 types:"XML File(.xml)|*.xml|All|*.*|"
	if (f!=undefined) then
	(
		xmlDoc.filename = f
		for s in lights do
		(
			if (superClassOf s == light) then
			(
				local type = ""
				local intensidad= 1.0
				local aux = ""
	
				local lightNode = XmlNode tag:"light"
				rootNode.AddChild lightNode
					
				print (ClassOf s as string)
				
				if (type == #targetDirect or type == #freeDirect) then
				(
					type = "directional"
				)else if (ClassOf s  == Omnilight) then
				(
					print ("omni!")
					type = "point"
				)else if (type == #targetSpot or ClassOf s == freeSpot) then
				(
					print ("spot!")
					lightNode.AddAttribute "angle" s.hotspot
					lightNode.AddAttribute "fall_off" s.falloff
					type = "spot"
				)
				else
				(
					type = ""
				)
				
				print (type)
				if (type != "") then (
					print("OK")
					lightNode.AddAttribute "type" type
					lightNode.AddAttribute "name" s.name
					local l_Text = stringstream "" 
					format "% % %" (s.rgb.r/255.0) (s.rgb.g/255.0) (s.rgb.b/255.0)  to:l_Text 
					aux =  (l_Text as string) 
					lightNode.AddAttribute "color" aux
					lightNode.AddAttribute "enabled" s.enabled
					lightNode.AddAttribute "intensity" intensidad
					
					l_Text = stringstream "" 
					format "% %" s.farAttenStart s.farAttenEnd to:l_Text 
					aux =  (l_Text as string)
					lightNode.AddAttribute "attenuation_range" aux

					local transformNode = XmlNode tag:"transform"
					lightNode.AddChild transformNode

					l_Text = stringstream "" 
					format "% % %" s.position.x s.position.z s.position.y to:l_Text 
					aux =  (l_Text as string)
					transformNode.AddAttribute "position" aux
					
					l_Text = stringstream "" 
					format "% % %" -s.dir.x -s.dir.z -s.dir.y to:l_Text 
					aux =  (l_Text as string)
					transformNode.AddAttribute "forward" aux		
				)
			)
		)
		
		xmlDoc.SaveXmlDocument()
	)
)

ExportLight()