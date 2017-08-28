struct BinFileWritter
(
	mFile=undefined,
	mPath= undefined,
	mVerbose = true,
	fn ChangeVerbose aVerbose =
	(
		mVerbose = aVerbose
		if mVerbose == true then
		( 
			format "Verbose enabled\n"
		)
		else
		(
			format "Verbose disabled\n"
		)
	),
	fn Open aName aDirectory aExtension = 
	(
		
		if aName != undefined and aDirectory != undefined then
		(
			if not doesFileExist aDirectory then
			(
				if mVerbose == true then
				(
					format "New directory %\n" aDirectory
				)
				makeDir aDirectory
			)
			
			mPath = aDirectory + aName + aExtension
			if mVerbose == true then
			(
				format "New file %\n" mPath
			)
			if doesFileExist mPath  then
			(
				if mVerbose == true then
				(
					format "Old % deleted\n" mPath
				)
				deleteFile mPath
			)
		
			mFile = fopen mPath "wb"
			if mVerbose == true then
			(
				format "% opened in wb\n" mPath
			)
			return true
		)
		
		if mVerbose == true then
		(
			format "% not opened\n" mPath
		)
		
		return false
	),
	fn Close =
	(
		if mVerbose == true then
		(
			format "% closed\n" mPath
		)
		FClose mFile
	),
	fn  PrintShort aShort =
	(
		if mVerbose == true then
		(
			format "PrintShort %\n" aShort
		)
		WriteShort  mFile aShort
	),
	fn  PrintUnsignedShort aShort =
	(
		if mVerbose == true then
		(
			format "PrintUnsignedShort %\n" aShort
		)
		WriteShort  mFile aShort #unsigned
	),
	fn  PrintLong aLong =
	(
		if mVerbose == true then
		(
			format "PrintLong %\n" aLong
		)
		WriteShort  mFile aLong
	),
	fn  PrintUnsignedLong aLong =
	(
		if mVerbose == true then
		(
			format "PrintUnsignedLong %\n" aLong
		)
		WriteShort  mFile aLong #unsigned
	),
	fn  PrintFloat aFloat =
	(
		if mVerbose == true then
		(
			format "PrintFloat %\n" aFloat
		)
		WriteFloat mFile aFloat
	),
	fn PrintString aString =
	(
		if mVerbose == true then
		(
			format "PrintString % count % \n" aString aString.count
		)
		WriteShort mFile aString.count #unsigned
		WriteString mFile aString
	)
)