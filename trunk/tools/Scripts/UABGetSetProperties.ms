rollout UABGetSetProperties "UAB User Properties Modifier" width:400 height:400
(
	
	GroupBox 'GBAnimation' "Animation" pos:[8, 16] width:150 height:260 align:#left
	checkbox 'CHCinematic' "IsCinematic" pos:[16,40] width:140 height:15 align:#left
	checkbox 'CHReverse' "Reverse" pos:[16,64] width:140 height:15 align:#left
	checkbox 'CHLoop' "Loop" pos:[16,88] width:140 height:15 align:#left
	spinner 'SPFrame' "Frame Step" pos:[55, 112] range:[0,1000,0] width:80

	edittext 'ETSceneAnim' "Scene" pos:[20,136] width:120-- height:1 align:#left
	edittext 'ETLayerAnim' "Layer" pos:[20,176] width:120 --height:1 align:#left
	edittext 'ETNameAnim' "Animation Name" pos:[20,216] width:120 --height:1 align:#left
	
	
	button 'BGet' "Get" pos:[8,280] width:150 height:21 align:#left
	on BGet pressed do(
		
		CHCinematic.checked = getUserProp $ "IsCinematic" 
		CHReverse.checked = getUserProp $ "Reverse" 
		CHLoop.checked = getUserProp $ "Loop" 
		SPFrame.value = getUserProp $ "FrameStep"

		ETSceneAnim.text = getUserProp $ "SceneAnim"
		ETLayerAnim.text = getUserProp $ "LayerAnim"
		ETNameAnim.text = getUserProp $ "AnimName"
	)
		

	button 'BSet' "Set" pos:[200,280] width:150 height:21 align:#right
	on BSet pressed do(
		
		setUserProp $ "IsCinematic" CHCinematic.checked
		setUserProp $ "Reverse" CHReverse.checked
		setUserProp $ "Loop" CHLoop.checked
		setUserProp $ "FrameStep" SPFrame.value
		setUserProp $ "SceneAnim" ETSceneAnim.text
		setUserProp $ "LayerAnim" ETLayerAnim.text
		setUserProp $ "AnimName" ETNameAnim.text
	)
)