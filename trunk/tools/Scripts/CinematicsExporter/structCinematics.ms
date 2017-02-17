

struct sCinematic(
	mName = "",
	mTotal_time= "",
	mLoop = false,
	mReverse = false,
	mCameras = #(),
	mObjects = #()
)

struct sCamera(	
	mKeys = #(),
	mName = ""
)

struct sCameraKey(

	mTime = 0,
	mPos= [0,0,0],
	mLookAt = [0,0,0],
	mUp = [0,1,0],
	mFov = [0,0,0],
	mNear_plane = 1,
	mFar_plane = 1000

)

struct sObject(
	mName = "",
	mSceneNode = "",
	mLayer = "",
	mScene="",
	mKeys = #()
	)

struct sObjectKey(
	mTime = 0,
	mPos= [0,0,0],
	mYaw = 0,
	mPitch = 0,
	mRoll = 0,
	mScale = [1,1,1]


	)

fn TranslationToRH translation = (
	return point3 -translation.x translation.z translation.y
)

fn Vect3ToStr vect3 = (

	local l_Text = stringstream ""
	local pos = vect3
	format "% % %" pos.x pos.y pos.z to:l_Text
	return (l_Text as string)
	)

