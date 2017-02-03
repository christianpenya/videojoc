
-- Very Simple XML IO utilities
-- with no external dependencies
-- by Matt Spencer (soulcraftgames@gmail.com)
-- for usage, see the "XMLExample.ms" file


struct XmlAttributePair
(
	name = "name",
	value = "value"
)


struct XmlNode
(
	tag = "node",
	childNodes = #(),
	attributePairs = #(),
	text = "",
	parent = undefined,
	
	fn AddAttribute attTag attValue = append attributePairs (XmlAttributePair name:attTag value:attValue),
	fn AddColorAttribute attTag attColor =
	(
		local color = ( attColor.r / 255 ) as string + " " + ( attColor.g / 255 ) as string + " " + ( attColor.b / 255 ) as string + " " + ( attColor.a / 255 ) as string
		AddAttribute attTag color
	),
	fn AddVect3Attribute attTag attVect3 =
	(
		local vect3 = ( attVect3.x ) as string + " " + ( attVect3.y ) as string + " " + ( attVect3.z ) as string
		AddAttribute attTag vect3
	),
	fn AddChild newChild = append childNodes newChild,
	
	-- default parameter value for "numberOfIndents" throws an evaluation error?
	-- :probably because "fs" needs one too...  no matter
	fn SaveToFile fs numberOfIndents =
	(
		local indent = ""
		for i=1 to numberOfIndents do append indent "\t"
		
		-- opening tag
		format (indent + "<% ") tag to:fs
		
		-- attributes
		for a in attributePairs do format "%=\"%\" " a.name a.value to:fs
		
		-- close the tag if has no children
		if childNodes.count > 0 then format ">\n" to:fs
		else format "/>\n" to:fs
		
		-- text
		
		-- child nodes
		for c in childNodes do c.SaveToFile fs (numberOfIndents+1)
		
		-- closing tag
		if childNodes.count > 0 then format (indent + "</%>\n") tag to:fs
	)

)




struct XmlDocument
(
	rootNode,
	filename,
	
	fn AddRoot rtnNode =(rootNode = rtnNode),
	
	-- Save an Xml File with the root node as a parameter
	fn SaveXmlDocument = 
	(
		local fs = openFile filename mode:"wt"
		rootNode.SaveToFile fs 0
		close fs
	),
	
	-- Fills the root node structure from the given file
	fn LoadXmlDocument = 
	(
		local curNode = undefined
		local fs = openFile filename mode:"rt"
	
		while not(eof fs) do
		(
			-- tag
			skipToString fs "<"
			tag = readDelimitedString fs " >"
			if not (findString tag "/") == undefined then
			(
				curNode = curNode.parent
				continue
			)
			if tag.count == 0 then continue
			--if debugPrint = true then format "tag = %\n" tag

			-- create a new node and make it a child of the current node
			local newNode = XmlNode tag:tag
			if not curNode == undefined then 
			(
				append curNode.childNodes newNode
				newNode.parent = curNode
			)
			else
				newNode.parent = newNode	-- set the root node's parent to itself

			curNode = newNode
			
			-- attributes
			local attString = (readDelimitedString fs ">") as stringstream
			--format "ATT STRING = %\n" (attString as string)
			
			while not eof attString do
			(
				attName = trimLeft (readDelimitedString attString "=") " "
				if attName.count == 0 then exit
				-- opening tag was closed
				if not (findString (attName as string) "/") == undefined then
				(
					curNode = curNode.parent
					exit
				)
				
				-- name
				--if debugPrint = true then format "att = %\n" attName
				skipToString attString "\""
				attValue = readDelimitedString attString "\""
				-- value
				--if debugPrint = true then format "value = %\n" attValue
				
				curNode.AddAttribute attName attValue
			)

			-- text 
		)

		close fs
		rootNode = curNode
	)
		
)








