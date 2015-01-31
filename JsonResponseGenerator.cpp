// Dependencies
#include "JsonResponseGenerator.h"
#include "sharedmemory.h"
#include "fossa.h"

JsonResponseGenerator::JsonResponseGenerator(){};

void sendServiceUnavailable(struct ns_connection *nc)    {
	ns_printf(nc, "%s", HTTP_RESPONSE_503);
	ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

void sendConflict(struct ns_connection *nc)    {
	ns_printf(nc, "%s", HTTP_RESPONSE_503);
	ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

void sendBuildInfo(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"buildinfo\":{");
	ns_printf_http_chunk(nc, "\"mVersion\":%u,", sharedData->mVersion);
	ns_printf_http_chunk(nc, "\"mBuildVersionNumber\":%u},", sharedData->mBuildVersionNumber);
}

void sendGameStates(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"gameStates\":{");
	ns_printf_http_chunk(nc, "\"mGameState\":%u,", sharedData->mGameState);
	ns_printf_http_chunk(nc, "\"mSessionState\":%u,", sharedData->mSessionState);
	ns_printf_http_chunk(nc, "\"mRaceState\":%u},", sharedData->mRaceState);
}

void sendParticipant(struct ns_connection *nc, const ParticipantInfo participantInfo)	{
	ns_printf_http_chunk(nc, "{\"mIsActive\":%s,", participantInfo.mIsActive ? "true" : "false");
	ns_printf_http_chunk(nc, "\"mName\":\"%s\",", participantInfo.mName);
	ns_printf_http_chunk(nc, "\"mWorldPosition\":[%f,%f,%f],", participantInfo.mWorldPosition[0], participantInfo.mWorldPosition[1], participantInfo.mWorldPosition[2]);
	ns_printf_http_chunk(nc, "\"mCurrentLapDistance\":%f,", participantInfo.mCurrentLapDistance);
	ns_printf_http_chunk(nc, "\"mRacePosition\":%u,", participantInfo.mRacePosition);
	ns_printf_http_chunk(nc, "\"mLapsCompleted\":%u,", participantInfo.mLapsCompleted);
	ns_printf_http_chunk(nc, "\"mCurrentLap\":%u,", participantInfo.mCurrentLap);
	ns_printf_http_chunk(nc, "\"mCurrentSector\":%u}", participantInfo.mCurrentSector);
}

void sendParticipants(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"participants\":{");
	ns_printf_http_chunk(nc, "\"mViewedParticipantIndex\":%i,", sharedData->mViewedParticipantIndex);
	ns_printf_http_chunk(nc, "\"mNumParticipants\":%i", sharedData->mNumParticipants);

	if (sharedData->mNumParticipants > -1)	{
		ns_printf_http_chunk(nc, ",");
		ns_printf_http_chunk(nc, "\"mParticipantInfo\":[");

		for (int i = 0; i < sharedData->mNumParticipants; i++)	{
			sendParticipant(nc, sharedData->mParticipantInfo[i]);
			if (i < (sharedData->mNumParticipants - 1))	{
				ns_printf_http_chunk(nc, ",");
			}
		}
		ns_printf_http_chunk(nc, "]");
	}
	ns_printf_http_chunk(nc, "},");
}

void sendUnfilteredInput(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"unfilteredInput\":{");
	ns_printf_http_chunk(nc, "\"mUnfilteredThrottle\":%f,", sharedData->mUnfilteredThrottle);
	ns_printf_http_chunk(nc, "\"mUnfilteredBrake\":%f,", sharedData->mUnfilteredBrake);
	ns_printf_http_chunk(nc, "\"mUnfilteredSteering\":%f,", sharedData->mUnfilteredSteering);
	ns_printf_http_chunk(nc, "\"mUnfilteredClutch\":%f},", sharedData->mUnfilteredClutch);
}

void sendVehicleInformation(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"vehicleInformation\":{");
	ns_printf_http_chunk(nc, "\"mCarName\":\"%s\",", sharedData->mCarName);
	ns_printf_http_chunk(nc, "\"mCarClassName\":\"%s\"},", sharedData->mCarClassName);
}

void sendEventInformation(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"eventInformation\":{");
	ns_printf_http_chunk(nc, "\"mLapsInEvent\":%u,", sharedData->mLapsInEvent);
	ns_printf_http_chunk(nc, "\"mTrackLocation\":\"%s\",", sharedData->mTrackLocation);
	ns_printf_http_chunk(nc, "\"mTrackVariation\":\"%s\",", sharedData->mTrackVariation);
	ns_printf_http_chunk(nc, "\"mTrackLength\":%f},", sharedData->mTrackLength);
}

void sendTimings(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"timings\":{");
	ns_printf_http_chunk(nc, "\"mLapInvalidated\":%s,", sharedData->mLapInvalidated ? "true" : "false");
	ns_printf_http_chunk(nc, "\"mBestLapTime\":%f,", sharedData->mBestLapTime);
	ns_printf_http_chunk(nc, "\"mLastLapTime\":%f,", sharedData->mLastLapTime);
	ns_printf_http_chunk(nc, "\"mCurrentTime\":%f,", sharedData->mCurrentTime);
	ns_printf_http_chunk(nc, "\"mSplitTimeAhead\":%f,", sharedData->mSplitTimeAhead);
	ns_printf_http_chunk(nc, "\"mSplitTimeBehind\":%f,", sharedData->mSplitTimeBehind);
	ns_printf_http_chunk(nc, "\"mSplitTime\":%f,", sharedData->mSplitTime);
	ns_printf_http_chunk(nc, "\"mEventTimeRemaining\":%f,", sharedData->mEventTimeRemaining);
	ns_printf_http_chunk(nc, "\"mPersonalFastestLapTime\":%f,", sharedData->mPersonalFastestLapTime);
	ns_printf_http_chunk(nc, "\"mWorldFastestLapTime\":%f,", sharedData->mWorldFastestLapTime);
	ns_printf_http_chunk(nc, "\"mCurrentSector1Time\":%f,", sharedData->mCurrentSector1Time);
	ns_printf_http_chunk(nc, "\"mCurrentSector2Time\":%f,", sharedData->mCurrentSector2Time);
	ns_printf_http_chunk(nc, "\"mCurrentSector3Time\":%f,", sharedData->mCurrentSector3Time);
	ns_printf_http_chunk(nc, "\"mFastestSector1Time\":%f,", sharedData->mFastestSector1Time);
	ns_printf_http_chunk(nc, "\"mFastestSector2Time\":%f,", sharedData->mFastestSector2Time);
	ns_printf_http_chunk(nc, "\"mFastestSector3Time\":%f,", sharedData->mFastestSector3Time);
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector1Time\":%f,", sharedData->mPersonalFastestSector1Time);
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector2Time\":%f,", sharedData->mPersonalFastestSector2Time);
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector3Time\":%f,", sharedData->mPersonalFastestSector3Time);
	ns_printf_http_chunk(nc, "\"mWorldFastestSector1Time\":%f,", sharedData->mWorldFastestSector1Time);
	ns_printf_http_chunk(nc, "\"mWorldFastestSector2Time\":%f,", sharedData->mWorldFastestSector2Time);
	ns_printf_http_chunk(nc, "\"mWorldFastestSector3Time\":%f},", sharedData->mWorldFastestSector3Time);
}

void sendFlags(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"flags\":{");
	ns_printf_http_chunk(nc, "\"mHighestFlagColour\":%u,", sharedData->mHighestFlagColour);
	ns_printf_http_chunk(nc, "\"mHighestFlagReason\":%u},", sharedData->mHighestFlagReason);
}

void sendPitInfo(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"pitInfo\":{");
	ns_printf_http_chunk(nc, "\"mPitMode\":%u,", sharedData->mPitMode);
	ns_printf_http_chunk(nc, "\"mPitSchedule\":%u},", sharedData->mPitSchedule);
}

void sendCarState(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"carState\":{");
	ns_printf_http_chunk(nc, "\"mCarFlags\":%u,", sharedData->mCarFlags);
	ns_printf_http_chunk(nc, "\"mOilTempCelsius\":%f,", sharedData->mOilTempCelsius);
	ns_printf_http_chunk(nc, "\"mWaterTempCelsius\":%f,", sharedData->mWaterTempCelsius);
	ns_printf_http_chunk(nc, "\"mWaterPressureKPa\":%f,", sharedData->mWaterPressureKPa);
	ns_printf_http_chunk(nc, "\"mFuelPressureKPa\":%f,", sharedData->mFuelPressureKPa);
	ns_printf_http_chunk(nc, "\"mFuelLevel\":%f,", sharedData->mFuelLevel);
	ns_printf_http_chunk(nc, "\"mFuelCapacity\":%f,", sharedData->mFuelCapacity);
	ns_printf_http_chunk(nc, "\"mSpeed\":%f,", sharedData->mSpeed);
	ns_printf_http_chunk(nc, "\"mRpm\":%f,", sharedData->mRpm);
	ns_printf_http_chunk(nc, "\"mMaxRPM\":%f,", sharedData->mMaxRPM);
	ns_printf_http_chunk(nc, "\"mBrake\":%f,", sharedData->mBrake);
	ns_printf_http_chunk(nc, "\"mThrottle\":%f,", sharedData->mThrottle);
	ns_printf_http_chunk(nc, "\"mClutch\":%f,", sharedData->mClutch);
	ns_printf_http_chunk(nc, "\"mSteering\":%f,", sharedData->mSteering);
	ns_printf_http_chunk(nc, "\"mGear\":%i,", sharedData->mGear);
	ns_printf_http_chunk(nc, "\"mNumGears\":%i,", sharedData->mNumGears);
	ns_printf_http_chunk(nc, "\"mOdometerKM\":%f,", sharedData->mOdometerKM);
	ns_printf_http_chunk(nc, "\"mAntiLockActive\":%s,", sharedData->mAntiLockActive ? "true" : "false");
	ns_printf_http_chunk(nc, "\"mLastOpponentCollisionIndex\":%i,", sharedData->mLastOpponentCollisionIndex);
	ns_printf_http_chunk(nc, "\"mLastOpponentCollisionMagnitude\":%f,", sharedData->mLastOpponentCollisionMagnitude);
	ns_printf_http_chunk(nc, "\"mBoostActive\":%s,", sharedData->mBoostActive ? "true" : "false");
	ns_printf_http_chunk(nc, "\"mBoostAmount\":%f},", sharedData->mBoostAmount);
}

void sendMotionDeviceRelated(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"motionAndDeviceRelated\":{");
	ns_printf_http_chunk(nc, "\"mOrientation\":[%f,%f,%f],", sharedData->mOrientation[0], sharedData->mOrientation[1], sharedData->mOrientation[2]);
	ns_printf_http_chunk(nc, "\"mLocalVelocity\":[%f,%f,%f],", sharedData->mLocalVelocity[0], sharedData->mLocalVelocity[1], sharedData->mLocalVelocity[2]);
	ns_printf_http_chunk(nc, "\"mWorldVelocity\":[%f,%f,%f],", sharedData->mWorldVelocity[0], sharedData->mWorldVelocity[1], sharedData->mWorldVelocity[2]);
	ns_printf_http_chunk(nc, "\"mAngularVelocity\":[%f,%f,%f],", sharedData->mAngularVelocity[0], sharedData->mAngularVelocity[1], sharedData->mAngularVelocity[2]);
	ns_printf_http_chunk(nc, "\"mLocalAcceleration\":[%f,%f,%f],", sharedData->mLocalAcceleration[0], sharedData->mLocalAcceleration[1], sharedData->mLocalAcceleration[2]);
	ns_printf_http_chunk(nc, "\"mWorldAcceleration\":[%f,%f,%f],", sharedData->mWorldAcceleration[0], sharedData->mWorldAcceleration[1], sharedData->mWorldAcceleration[2]);
	ns_printf_http_chunk(nc, "\"mExtentsCentre\":[%f,%f,%f]},", sharedData->mExtentsCentre[0], sharedData->mExtentsCentre[1], sharedData->mExtentsCentre[2]);
}

void sendWheelsTyres(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"wheelsAndTyres\":{");
	ns_printf_http_chunk(nc, "\"mTyreFlags\":[%u,%u,%u,%u],", sharedData->mTyreFlags[0], sharedData->mTyreFlags[1], sharedData->mTyreFlags[2], sharedData->mTyreFlags[3]);
	ns_printf_http_chunk(nc, "\"mTerrain\":[%u,%u,%u,%u],", sharedData->mTerrain[0], sharedData->mTerrain[1], sharedData->mTerrain[2], sharedData->mTerrain[3]);
	ns_printf_http_chunk(nc, "\"mTyreY\":[%f,%f,%f,%f],", sharedData->mTyreY[0], sharedData->mTyreY[1], sharedData->mTyreY[2], sharedData->mTyreY[3]);
	ns_printf_http_chunk(nc, "\"mTyreRPS\":[%f,%f,%f,%f],", sharedData->mTyreRPS[0], sharedData->mTyreRPS[1], sharedData->mTyreRPS[2], sharedData->mTyreRPS[3]);
	ns_printf_http_chunk(nc, "\"mTyreSlipSpeed\":[%f,%f,%f,%f],", sharedData->mTyreSlipSpeed[0], sharedData->mTyreSlipSpeed[1], sharedData->mTyreSlipSpeed[2], sharedData->mTyreSlipSpeed[3]);
	ns_printf_http_chunk(nc, "\"mTyreTemp\":[%f,%f,%f,%f],", sharedData->mTyreTemp[0], sharedData->mTyreTemp[1], sharedData->mTyreTemp[2], sharedData->mTyreTemp[3]);
	ns_printf_http_chunk(nc, "\"mTyreGrip\":[%f,%f,%f,%f],", sharedData->mTyreGrip[0], sharedData->mTyreGrip[1], sharedData->mTyreGrip[2], sharedData->mTyreGrip[3]);
	ns_printf_http_chunk(nc, "\"mTyreHeightAboveGround\":[%f,%f,%f,%f],", sharedData->mTyreHeightAboveGround[0], sharedData->mTyreHeightAboveGround[1], sharedData->mTyreHeightAboveGround[2], sharedData->mTyreHeightAboveGround[3]);
	ns_printf_http_chunk(nc, "\"mTyreLateralStiffness\":[%f,%f,%f,%f],", sharedData->mTyreLateralStiffness[0], sharedData->mTyreLateralStiffness[1], sharedData->mTyreLateralStiffness[2], sharedData->mTyreLateralStiffness[3]);
	ns_printf_http_chunk(nc, "\"mTyreWear\":[%f,%f,%f,%f],", sharedData->mTyreWear[0], sharedData->mTyreWear[1], sharedData->mTyreWear[2], sharedData->mTyreWear[3]);
	ns_printf_http_chunk(nc, "\"mBrakeDamage\":[%f,%f,%f,%f],", sharedData->mBrakeDamage[0], sharedData->mBrakeDamage[1], sharedData->mBrakeDamage[2], sharedData->mBrakeDamage[3]);
	ns_printf_http_chunk(nc, "\"mSuspensionDamage\":[%f,%f,%f,%f],", sharedData->mSuspensionDamage[0], sharedData->mSuspensionDamage[1], sharedData->mSuspensionDamage[2], sharedData->mSuspensionDamage[3]);
	ns_printf_http_chunk(nc, "\"mBrakeTempCelsius\":[%f,%f,%f,%f],", sharedData->mBrakeTempCelsius[0], sharedData->mBrakeTempCelsius[1], sharedData->mBrakeTempCelsius[2], sharedData->mBrakeTempCelsius[3]);
	ns_printf_http_chunk(nc, "\"mTyreTreadTemp\":[%f,%f,%f,%f],", sharedData->mTyreTreadTemp[0], sharedData->mTyreTreadTemp[1], sharedData->mTyreTreadTemp[2], sharedData->mTyreTreadTemp[3]);
	ns_printf_http_chunk(nc, "\"mTyreLayerTemp\":[%f,%f,%f,%f],", sharedData->mTyreLayerTemp[0], sharedData->mTyreLayerTemp[1], sharedData->mTyreLayerTemp[2], sharedData->mTyreLayerTemp[3]);
	ns_printf_http_chunk(nc, "\"mTyreCarcassTemp\":[%f,%f,%f,%f],", sharedData->mTyreCarcassTemp[0], sharedData->mTyreCarcassTemp[1], sharedData->mTyreCarcassTemp[2], sharedData->mTyreCarcassTemp[3]);
	ns_printf_http_chunk(nc, "\"mTyreRimTemp\":[%f,%f,%f,%f],", sharedData->mTyreRimTemp[0], sharedData->mTyreRimTemp[1], sharedData->mTyreRimTemp[2], sharedData->mTyreRimTemp[3]);
	ns_printf_http_chunk(nc, "\"mTyreInternalAirTemp\":[%f,%f,%f,%f]},", sharedData->mTyreInternalAirTemp[0], sharedData->mTyreInternalAirTemp[1], sharedData->mTyreInternalAirTemp[2], sharedData->mTyreInternalAirTemp[3]);
}

void sendCarDamage(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"carDamage\":{");
	ns_printf_http_chunk(nc, "\"mCrashState\":%u,", sharedData->mCrashState);
	ns_printf_http_chunk(nc, "\"mAeroDamage\":%f,", sharedData->mAeroDamage);
	ns_printf_http_chunk(nc, "\"mEngineDamage\":%f},", sharedData->mEngineDamage);
}

void sendWeather(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"weather\":{");
	ns_printf_http_chunk(nc, "\"mAmbientTemperature\":%f,", sharedData->mAmbientTemperature);
	ns_printf_http_chunk(nc, "\"mTrackTemperature\":%f,", sharedData->mTrackTemperature);
	ns_printf_http_chunk(nc, "\"mRainDensity\":%f,", sharedData->mRainDensity);
	ns_printf_http_chunk(nc, "\"mWindSpeed\":%f,", sharedData->mWindSpeed);
	ns_printf_http_chunk(nc, "\"mWindDirectionX\":%f,", sharedData->mWindDirectionX);
	ns_printf_http_chunk(nc, "\"mWindDirectionY\":%f,", sharedData->mWindDirectionY);
	ns_printf_http_chunk(nc, "\"mCloudBrightness\":%f}", sharedData->mCloudBrightness);
}

void sendSharedMemoryJson(struct ns_connection *nc, const SharedMemory* sharedData)  {
	/* Send HTTP OK response header */
	ns_printf(nc, "%s", HTTP_RESPONSE_200);

	// Start of JSON output
	ns_printf_http_chunk(nc, "{");

	sendBuildInfo(nc, sharedData);
	sendGameStates(nc, sharedData);
	sendParticipants(nc, sharedData);
	sendUnfilteredInput(nc, sharedData);
	sendVehicleInformation(nc, sharedData);
	sendEventInformation(nc, sharedData);
	sendTimings(nc, sharedData);
	sendFlags(nc, sharedData);
	sendPitInfo(nc, sharedData);
	sendCarState(nc, sharedData);
	sendMotionDeviceRelated(nc, sharedData);
	sendWheelsTyres(nc, sharedData);
	sendCarDamage(nc, sharedData);
	sendWeather(nc, sharedData);

	ns_printf_http_chunk(nc, "}");

}

void processSharedMemoryData(struct ns_connection *nc, const SharedMemory* sharedData)   {
	// Ensure we're sync'd to the correct data version
	if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
		// Send conflict response
		sendConflict(nc);
		printf("Data version mismatch, please make sure that your pCARS version matches your CREST version\n");
	}
	else{
		sendSharedMemoryJson(nc, sharedData);
	}

}

void processFile(struct ns_connection *nc, HANDLE fileHandle)    {

	const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));

	if (sharedData == NULL)	{
		// File found, but could not be mapped to shared memory data
		sendServiceUnavailable(nc);
	}
	else{
		// Process file
		processSharedMemoryData(nc, sharedData);
		// Unmap file
		UnmapViewOfFile(sharedData);
	}

}


void JsonResponseGenerator::generateResponse(struct ns_connection *nc)	{
	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMappingA(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);

	if (fileHandle == NULL)	{
		// File is not available, send service unavailable response
		sendServiceUnavailable(nc);
	}
	else{
		// File is available, process the file
		processFile(nc, fileHandle);
		// Close the file
		CloseHandle(fileHandle);
	}
	// Send empty chunk to mark the end of the response
	ns_send_http_chunk(nc, "", 0);

}