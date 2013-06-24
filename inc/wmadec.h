
typedef enum tagWMAFileStatus
{
	cWMA_NoErr,                 /* -> always first entry */
	/* remaining entry order is not guaranteed */
	cWMA_Failed,
	cWMA_BadArgument,
	cWMA_BadAsfHeader,
	cWMA_BadPacketHeader,
	cWMA_BrokenFrame,
	cWMA_NoMoreFrames,
	cWMA_BadSamplingRate,
	cWMA_BadNumberOfChannels,
	cWMA_BadVersionNumber,
	cWMA_BadWeightingMode,
	cWMA_BadPacketization,

	cWMA_BadDRMType,
	cWMA_DRMFailed,
	cWMA_DRMUnsupported,

	cWMA_DemoExpired,

	cWMA_BadState,
	cWMA_Internal,               /* really bad */
	cWMA_NoMoreDataThisTime
} tWMAFileStatus;
