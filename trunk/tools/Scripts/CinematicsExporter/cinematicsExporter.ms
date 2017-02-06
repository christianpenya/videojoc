fileIn "xml.ms"
fileIn "structCinematics.ms"


fn getUserPropertiesAnim = (

	return "animationRange"
	)
fn getUserPropertiesFR = (

	return "frameRate"
	)
fn getUserPropertiesTPF = (

	return "ticketsPerFrame"
	)


fn getUserPropertiesReverse = (

	return True
	)


fn getUserPropertiesLoop = (

	return True
	)


function main = (
	if $ != undefined then (

		global gCinematic = sCinematic()
		gCinematic.mLoop = getUserPropertiesLoop()
		gCinematic.mReverse = getUserPropertiesReverse()
		local animationRange = getUserPropertiesAnim()
		local ticketsPerFrame = getUserPropertiesTPF()
		local frameRate = getUserPropertiesFR
		gCinematic.mTotal_time = (((animationRange.end as float)/ticketsPerFrame)/frameRate)
		


		try	(
			format "Multi objects\n"
			local lSelection = $.count
			for obj in $ do (
				processObject obj
				)

		)
		catch
		(
			format "Single Object\n"
			processObject $			
		)		


			
	)


main()

