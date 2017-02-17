fileIn "xml.ms"
fileIn "structCinematics.ms"


fn getUserNFrames obj = (
	return getUserProp obj "FrameStep"

)

fn getUserPropertiesAnimationName obj = (
	return getUserProp obj "AnimName"

)

fn getUserPropertiesScene obj = (
	return getUserProp obj "SceneAnim"
)

fn getUserPropertiesLayer obj = (
	return getUserProp obj "LayerAnim"
)

fn getUserPropertiesIsCinematic obj = (
	return getUserProp obj "IsCinematic"
)

fn getUserPropertiesReverse obj = (
	return getUserProp obj "Reverse"

)


fn getUserPropertiesLoop obj = (
	return getUserProp obj "Loop"


)

fn processCamera cam  lCinematic = (

	local lCamera = sCamera()

	local lNFrames = getUserNFrames cam
	lCamera.mName = cam.name
	for t = animationRange.start to animationRange.end by lNFrames do(
		animate on(
			at time t(
			
				local lCameraKey = sCameraKey()
				lCameraKey.mTime = (((t as float) / ticksPerFrame) / framerate)
				lCameraKey.mPos = TranslationToRH(cam.transform.translation) --Pasar a RH
				lCameraKey.mFov=cam.curFOV	
				lCameraKey.mLookAt = -cam.transform[1] --
				lCameraKey.mUp = cam.transform[2] --
				lCameraKey.mNear_plane = cam.nearrange
				lCameraKey.mFar_plane = cam.farrange
				append lCamera.mKeys lCameraKey
			)
		)

	)

	append lCinematic.mCameras lCamera
)

fn processObject obj lCinematic = (

	local lObject = sObject()
	local lNFrames = getUserNFrames obj
	lObject.mName = obj.name
	lObject.mLayer = getUserPropertiesLayer obj
	lObject.mScene = getUserPropertiesScene obj
	for t = animationRange.start to animationRange.end by lNFrames do(
		animate on(
			at time t(

				local lObjectKey = sObjectKey()
				lObjectKey.mTime = (((t as float) / ticksPerFrame) / framerate)
				lObjectKey.mPos = TranslationToRH(obj.transform.translation) --Pasar a RH
				lObjectKey.mYaw = (obj.transform as eulerangles).z
				lObjectKey.mPitch = (obj.transform as eulerangles).y
				lObjectKey.mRoll = (obj.transform as eulerangles).x
				lObjectKey.mScale = TranslationTORH(obj.transform.scale)

				append lObject.mKeys lObjectKey
			)
		)
	)
	
	append lCinematic.mObjects lObject
	format "end\n"
)

fn write = (

	local rootNode = XmlNode tag:"cinematics"
	xmlDoc.rootNode = rootNode
	for cinematic in gCinematics do (
		format "WRITING"
		local cinematicNode = XmlNode tag:"cinematic"
		rootNode.addChild cinematicNode
		cinematicNode.addAttribute "name" cinematic.mName
		cinematicNode.addAttribute "total_time" cinematic.mTotal_time
		cinematicNode.addAttribute "loop" cinematic.mLoop
		cinematicNode.addAttribute "reverse" cinematic.mReverse
		for cam in cinematic.mCameras do(
			format "writ2\n"
			local camNode = XmlNode tag:"cinematic_camera_player"
			cinematicNode.addChild camNode
			camNode.addAttribute "name" cam.mName
			for camKey in cam.mKeys do(
			
				local keyNode = XmlNode tag:"key"
				camNode.addChild keyNode				
				keyNode.addAttribute "time" camKey.mTime
				local str = Vect3ToStr(camKey.mPos)
				keyNode.addAttribute "pos"  str
				str = Vect3ToStr(camKey.mLookAt)
				keyNode.addAttribute "look_at" str
				str =   Vect3ToStr(camKey.mUp)
				keyNode.addAttribute "up" str
				keyNode.addAttribute "fov" camKey.mFov
				keyNode.addAttribute "near_plane" camKey.mNear_plane
				keyNode.addAttribute "far_plane" camKey.mFar_plane
				

			)
		)

		for obj in cinematic.mObjects do(
			local objNode = XmlNode tag:"cinematic_object_player"
			cinematicNode.addChild objNode
			objNode.addAttribute "name" obj.mName
			objNode.addAttribute "scene_node" obj.mSceneNode
			objNode.addAttribute "layer" obj.mLayer
			objNode.addAttribute "scene" obj.mScene
			for objKey in obj.mKeys do(
				local keyNode = XmlNode tag:"key"
				objNode.addChild keyNode
				keyNode.addAttribute "time" objKey.mTime
				
				keyNode.addAttribute "pos"  str
				keyNode.addAttribute "yaw" objKey.mYaw
				keyNode.addAttribute "pitch" objKey.mPitch
				keyNode.addAttribute "roll" objKey.mRoll
				str = Vect3ToStr(objKey.mScale)
				keyNode.addAttribute "scale" str
			)
		)
	)
	format "EndWRITING"
)


function main = (
	for item in $ do(
			format item.name
	)
	
	if $ != undefined then (
		global gCinematics = #()
		global gCinematicsName = #()
		
		global file = getSaveFilename "Save XML File" filename:( (getdir #userscripts) + "\\" + (getFilenameFile "cinematic.xml")) types:"XML File(.xml)|*.xml|All|*.*|"
		global xmlDoc = XmlDocument()
		if file != undefined then (
				if doesFileExist file then(
					deleteFile file
				)
				xmlDoc.filename = file
				try	(
					format "Multi objects\n"				
					local lSelection = $.count
					for obj in $ do (
						format obj.name 
						format "HI\n"
						local lCinematic = sCinematic()
						
						local l_Name = getUserPropertiesAnimationName obj
						local idx = findItem gCinematicsName l_Name
						if  idx == 0 then(
							format "idx == 0"
							append gCinematicsName l_Name
							lCinematic.mName = l_Name
							lCinematic.mLoop = getUserPropertiesLoop obj
							lCinematic.mReverse = getUserPropertiesReverse obj
							local animRange = animationrange
							local ticketsPerFrame = ticksperframe
							local lFrameRate = framerate
							lCinematic.mTotal_time = (((animRange.end as float)/ticketsPerFrame)/lFrameRate)
						)else(
							lCinematic = gCinematics[idx]
						)

						if SuperclassOf obj == Camera then(
							format "CAM\n"
							processCamera obj lCinematic
							format "outCam"

						)else (
							format "OBJ\n"
							format obj.name
							processObject obj lCinematic
						)
					
						if idx == 0 then(
							format "HI0"
							append gCinematics lCinematic
						)else (
							gCinematics[idx] = lCinematic
						)

					)
					write()
					xmlDoc.SaveXmlDocument()
				)
				catch
				(
					
					format "Single Object\n"
					local lCinematic = sCinematic()
					lCinematic.mName = getUserPropertiesAnimationName $
					lCinematic.mLoop = getUserPropertiesLoop $
					lCinematic.mReverse = getUserPropertiesReverse $
					local animRange = animationrange
					local ticketsPerFrame = ticksperframe
					local lFrameRate = framerate
					
					lCinematic.mTotal_time = (((animRange.end as float)/ticketsPerFrame)/lFrameRate)
					if SuperclassOf ojb == Camera then(
						format "Cam\n"
						processCamera $ lCinematic
					)else (
						format "Object\n"
						processObject $ lCinematic
					)
					append gCinematics lCinematic
					write()	
					xmlDoc.SaveXmlDocument()
				)
		)	
			
	)
)


main()

