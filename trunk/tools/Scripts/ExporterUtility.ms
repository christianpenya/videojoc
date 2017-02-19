fileIn "UABGetSetProperties.ms"

utility UABExporterUtility "UAB Exporter" width:192 height:376
(
	
	GroupBox 'GBExportSettings' "Export Settings" pos:[8,16] width:150 height:140 align:#left
	checkbox 'CHMaterials' "Materials" pos:[16,40] width:140 height:15 align:#left
	checkbox 'CHCopyTextures' "Copy Textures" pos:[16,64] width:140 height:15 align:#left
	checkbox 'CHMeshes' "Meshes" pos:[16,88] width:140 height:15 align:#left
	checkbox 'CHScene' "Scene" pos:[16,112] width:140 height:15 align:#left
	checkbox 'CHAnimation' "Animation" pos:[16,136] width:140 height:15 align:#left
	button 'BExportPath' "Export Path" pos:[8,176] width:150 height:21 align:#left
	label 'LBExportPath' "C:/mvuab/data" pos:[8,200] width:150 height:21 align:#left
	button 'BExport' "Export" pos:[8,280] width:150 height:21 align:#left
	combobox 'CBScene' "Scene" pos:[8,224] width:150 height:1 align:#left
	button 'BGet_SetProperties' "Get/Set Properties" pos:[8,320] width:150 height:21 align:#left
	on BGet_SetProperties pressed do
		createDialog UABGetSetProperties
)
