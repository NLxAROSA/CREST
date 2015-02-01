// Dependencies
#include "JsonResponseGenerator.h"
#include "sharedmemory.h"
#include "fossa.h"
#include <sstream>

JsonResponseGenerator::JsonResponseGenerator(){};

void sendServiceUnavailable(struct ns_connection *nc)    {
	ns_printf(nc, "%s", HTTP_RESPONSE_503);
	ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

void sendConflict(struct ns_connection *nc)    {
	ns_printf(nc, "%s", HTTP_RESPONSE_503);
	ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

void sendBuildInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"buildinfo\":{";
	ss << "\"mVersion\":" << sharedData->mVersion << ",";
	ss << "\"mBuildVersionNumber\":" << sharedData->mBuildVersionNumber << "},";
}

void sendGameStates(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"gameStates\":{";
	ss << "\"mGameState\":" << sharedData->mGameState << ",";
	ss << "\"mSessionState\":" << sharedData->mSessionState << ",";
	ss << "\"mRaceState\":" << sharedData->mRaceState << "},";
}

void sendParticipant(std::stringstream& ss, const ParticipantInfo participantInfo)	{
	ss << "{\"mIsActive\":" << (participantInfo.mIsActive ? "true" : "false") << ",";
	ss << "\"mName\":\"" << participantInfo.mName << "\",";
	ss << "\"mWorldPosition\":[" << participantInfo.mWorldPosition[0] << "," << participantInfo.mWorldPosition[1] << "," << participantInfo.mWorldPosition[2] << "],";
	ss << "\"mCurrentLapDistance\":" << participantInfo.mCurrentLapDistance << ",";
	ss << "\"mRacePosition\":" << participantInfo.mRacePosition << ",";
	ss << "\"mLapsCompleted\":" << participantInfo.mLapsCompleted << ",";
	ss << "\"mCurrentLap\":" << participantInfo.mCurrentLap << ",";
	ss << "\"mCurrentSector\":" << participantInfo.mCurrentSector << "}";
}

void sendParticipants(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"participants\":{";
	ss << "\"mViewedParticipantIndex\":%i,", sharedData->mViewedParticipantIndex;
	ss << "\"mNumParticipants\":%i", sharedData->mNumParticipants;

	if (sharedData->mNumParticipants > -1)	{
		ss << ",";
		ss << "\"mParticipantInfo\":[";

		for (int i = 0; i < sharedData->mNumParticipants; i++)	{
			sendParticipant(ss, sharedData->mParticipantInfo[i]);
			if (i < (sharedData->mNumParticipants - 1))	{
				ss << ",";
			}
		}
		ss << "]";
	}
	ss << "},";
}

void sendUnfilteredInput(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"unfilteredInput\":{";
	ss << "\"mUnfilteredThrottle\":" << sharedData->mUnfilteredThrottle << ",";
	ss << "\"mUnfilteredBrake\":" << sharedData->mUnfilteredBrake << ",";
	ss << "\"mUnfilteredSteering\":" << sharedData->mUnfilteredSteering << ",";
	ss << "\"mUnfilteredClutch\":" << sharedData->mUnfilteredClutch << "},";
}

void sendVehicleInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"vehicleInformation\":{";
	ss << "\"mCarName\":\"" << sharedData->mCarName << "\",";
	ss << "\"mCarClassName\":\""<< sharedData->mCarClassName << "\"},";
}

void sendEventInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"eventInformation\":{";
	ss << "\"mLapsInEvent\":" << sharedData->mLapsInEvent << ",";
	ss << "\"mTrackLocation\":\"" << sharedData->mTrackLocation << "\",";
	ss << "\"mTrackVariation\":\"" << sharedData->mTrackVariation << "\",";
	ss << "\"mTrackLength\":" << sharedData->mTrackLength << "},";
}

void sendTimings(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"timings\":{";
	ss << "\"mLapInvalidated\":" << (sharedData->mLapInvalidated ? "true" : "false") << ",";
	ss << "\"mBestLapTime\":" << sharedData->mBestLapTime << ",";
	ss << "\"mLastLapTime\":" << sharedData->mLastLapTime << ",";
	ss << "\"mCurrentTime\":" << sharedData->mCurrentTime << ",";
	ss << "\"mSplitTimeAhead\":" << sharedData->mSplitTimeAhead << ",";
	ss << "\"mSplitTimeBehind\":" << sharedData->mSplitTimeBehind << ",";
	ss << "\"mSplitTime\":" << sharedData->mSplitTime << ",";
	ss << "\"mEventTimeRemaining\":" << sharedData->mEventTimeRemaining << ",";
	ss << "\"mPersonalFastestLapTime\":" << sharedData->mPersonalFastestLapTime << ",";
	ss << "\"mWorldFastestLapTime\":" << sharedData->mWorldFastestLapTime << ",";
	ss << "\"mCurrentSector1Time\":" << sharedData->mCurrentSector1Time << ",";
	ss << "\"mCurrentSector2Time\":" << sharedData->mCurrentSector2Time << ",";
	ss << "\"mCurrentSector3Time\":" << sharedData->mCurrentSector3Time << ",";
	ss << "\"mFastestSector1Time\":" << sharedData->mFastestSector1Time << ",";
	ss << "\"mFastestSector2Time\":" << sharedData->mFastestSector2Time << ",";
	ss << "\"mFastestSector3Time\":" << sharedData->mFastestSector3Time << ",";
	ss << "\"mPersonalFastestSector1Time\":" << sharedData->mPersonalFastestSector1Time << ",";
	ss << "\"mPersonalFastestSector2Time\":" << sharedData->mPersonalFastestSector2Time << ",";
	ss << "\"mPersonalFastestSector3Time\":" << sharedData->mPersonalFastestSector3Time << ",";
	ss << "\"mWorldFastestSector1Time\":" << sharedData->mWorldFastestSector1Time << ",";
	ss << "\"mWorldFastestSector2Time\":" << sharedData->mWorldFastestSector2Time << ",";
	ss << "\"mWorldFastestSector3Time\":" << sharedData->mWorldFastestSector3Time << "},";
}

void sendFlags(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"flags\":{";
	ss << "\"mHighestFlagColour\":" << sharedData->mHighestFlagColour << ",";
	ss << "\"mHighestFlagReason\":" << sharedData->mHighestFlagReason << "},";
}

void sendPitInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"pitInfo\":{";
	ss << "\"mPitMode\":" << sharedData->mPitMode << ",";
	ss << "\"mPitSchedule\":" << sharedData->mPitSchedule << "},";
}

void sendCarState(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"carState\":{";
	ss << "\"mCarFlags\":" << sharedData->mCarFlags << ",";
	ss << "\"mOilTempCelsius\":" << sharedData->mOilTempCelsius << ",";
	ss << "\"mWaterTempCelsius\":" << sharedData->mWaterTempCelsius << ",";
	ss << "\"mWaterPressureKPa\":" << sharedData->mWaterPressureKPa << ",";
	ss << "\"mFuelPressureKPa\":" << sharedData->mFuelPressureKPa << ",";
	ss << "\"mFuelLevel\":" << sharedData->mFuelLevel << ",";
	ss << "\"mFuelCapacity\":" << sharedData->mFuelCapacity << ",";
	ss << "\"mSpeed\":" << sharedData->mSpeed << ",";
	ss << "\"mRpm\":" << sharedData->mRpm << ",";
	ss << "\"mMaxRPM\":" << sharedData->mMaxRPM << ",";
	ss << "\"mBrake\":" << sharedData->mBrake << ",";
	ss << "\"mThrottle\":" << sharedData->mThrottle << ",";
	ss << "\"mClutch\":" << sharedData->mClutch << ",";
	ss << "\"mSteering\":" << sharedData->mSteering << ",";
	ss << "\"mGear\":" << sharedData->mGear << ",";
	ss << "\"mNumGears\":" << sharedData->mNumGears << ",";
	ss << "\"mOdometerKM\":" << sharedData->mOdometerKM << ",";
	ss << "\"mAntiLockActive\":" << (sharedData->mAntiLockActive ? "true" : "false") << ",";
	ss << "\"mLastOpponentCollisionIndex\":" << sharedData->mLastOpponentCollisionIndex << ",";
	ss << "\"mLastOpponentCollisionMagnitude\":" << sharedData->mLastOpponentCollisionMagnitude << ",";
	ss << "\"mBoostActive\":" << (sharedData->mBoostActive ? "true" : "false") << ",";
	ss << "\"mBoostAmount\":" << sharedData->mBoostAmount << "},";
}

void sendMotionDeviceRelated(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"motionAndDeviceRelated\":{";
	ss << "\"mOrientation\":[" << sharedData->mOrientation[0] << "," << sharedData->mOrientation[1] << "," << sharedData->mOrientation[2] << "],";
	ss << "\"mLocalVelocity\":[" << sharedData->mLocalVelocity[0] << "," << sharedData->mLocalVelocity[1] << "," << sharedData->mLocalVelocity[2] << "],";
	ss << "\"mWorldVelocity\":[" << sharedData->mWorldVelocity[0] << "," << sharedData->mWorldVelocity[1] << "," << sharedData->mWorldVelocity[2] << "],";
	ss << "\"mAngularVelocity\":[" << sharedData->mAngularVelocity[0] << "," << sharedData->mAngularVelocity[1] << "," << sharedData->mAngularVelocity[2] << "],";
	ss << "\"mLocalAcceleration\":[" << sharedData->mLocalAcceleration[0] << "," << sharedData->mLocalAcceleration[1] << "," << sharedData->mLocalAcceleration[2] << "],";
	ss << "\"mWorldAcceleration\":[" << sharedData->mWorldAcceleration[0] << "," << sharedData->mWorldAcceleration[1] << "," << sharedData->mWorldAcceleration[2] << "],";
	ss << "\"mExtentsCentre\":[" << sharedData->mExtentsCentre[0] << "," << sharedData->mExtentsCentre[1] << "," << sharedData->mExtentsCentre[2] << "]},";
}

void sendWheelsTyres(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"wheelsAndTyres\":{";
	ss << "\"mTyreFlags\":[" << sharedData->mTyreFlags[0] << "," << sharedData->mTyreFlags[1] << "," << sharedData->mTyreFlags[2] << "," << sharedData->mTyreFlags[3] << "],";
	ss << "\"mTerrain\":[" << sharedData->mTerrain[0] << "," << sharedData->mTerrain[1] << "," << sharedData->mTerrain[2] << "," << sharedData->mTerrain[3] << "],";
	ss << "\"mTyreY\":[" << sharedData->mTyreY[0] << "," << sharedData->mTyreY[1] << "," << sharedData->mTyreY[2] << "," << sharedData->mTyreY[3] << "],";
	ss << "\"mTyreRPS\":[" << sharedData->mTyreRPS[0] << "," << sharedData->mTyreRPS[1] << "," << sharedData->mTyreRPS[2] << "," << sharedData->mTyreRPS[3] << "],";
	ss << "\"mTyreSlipSpeed\":[" << sharedData->mTyreSlipSpeed[0] << "," << sharedData->mTyreSlipSpeed[1] << "," << sharedData->mTyreSlipSpeed[2] << "," << sharedData->mTyreSlipSpeed[3] << "],";
	ss << "\"mTyreTemp\":[" << sharedData->mTyreTemp[0] << "," << sharedData->mTyreTemp[1] << "," << sharedData->mTyreTemp[2] << "," << sharedData->mTyreTemp[3] << "],";
	ss << "\"mTyreGrip\":[" << sharedData->mTyreGrip[0] << "," << sharedData->mTyreGrip[1] << "," << sharedData->mTyreGrip[2] << "," << sharedData->mTyreGrip[3] << "],";
	ss << "\"mTyreHeightAboveGround\":[" << sharedData->mTyreHeightAboveGround[0] << "," << sharedData->mTyreHeightAboveGround[1] << "," << sharedData->mTyreHeightAboveGround[2] << "," << sharedData->mTyreHeightAboveGround[3] << "],";
	ss << "\"mTyreLateralStiffness\":[" << sharedData->mTyreLateralStiffness[0] << "," << sharedData->mTyreLateralStiffness[1] << "," << sharedData->mTyreLateralStiffness[2] << "," << sharedData->mTyreLateralStiffness[3] << "],";
	ss << "\"mTyreWear\":[" << sharedData->mTyreWear[0] << "," << sharedData->mTyreWear[1] << "," << sharedData->mTyreWear[2] << "," << sharedData->mTyreWear[3] << "],";
	ss << "\"mBrakeDamage\":[" << sharedData->mBrakeDamage[0] << "," << sharedData->mBrakeDamage[1] << "," << sharedData->mBrakeDamage[2] << "," << sharedData->mBrakeDamage[3] << "],";
	ss << "\"mSuspensionDamage\":[" << sharedData->mSuspensionDamage[0] << "," << sharedData->mSuspensionDamage[1] << "," << sharedData->mSuspensionDamage[2] << "," << sharedData->mSuspensionDamage[3] << "],";
	ss << "\"mBrakeTempCelsius\":[" << sharedData->mBrakeTempCelsius[0] << "," << sharedData->mBrakeTempCelsius[1] << "," << sharedData->mBrakeTempCelsius[2] << "," << sharedData->mBrakeTempCelsius[3] << "],";
	ss << "\"mTyreTreadTemp\":[" << sharedData->mTyreTreadTemp[0] << "," << sharedData->mTyreTreadTemp[1] << "," << sharedData->mTyreTreadTemp[2] << "," << sharedData->mTyreTreadTemp[3] << "],";
	ss << "\"mTyreLayerTemp\":[" << sharedData->mTyreLayerTemp[0] << "," << sharedData->mTyreLayerTemp[1] << "," << sharedData->mTyreLayerTemp[2] << "," << sharedData->mTyreLayerTemp[3] << "],";
	ss << "\"mTyreCarcassTemp\":[" << sharedData->mTyreCarcassTemp[0] << "," << sharedData->mTyreCarcassTemp[1] << "," << sharedData->mTyreCarcassTemp[2] << "," << sharedData->mTyreCarcassTemp[3] << "],";
	ss << "\"mTyreRimTemp\":[" << sharedData->mTyreRimTemp[0] << "," << sharedData->mTyreRimTemp[1] << "," << sharedData->mTyreRimTemp[2] << "," << sharedData->mTyreRimTemp[3] << "],";
	ss << "\"mTyreInternalAirTemp\":[" << sharedData->mTyreInternalAirTemp[0] << "," << sharedData->mTyreInternalAirTemp[1] << "," << sharedData->mTyreInternalAirTemp[2] << "," << sharedData->mTyreInternalAirTemp[3] << "]},";
}

void sendCarDamage(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"carDamage\":{";
	ss << "\"mCrashState\":" << sharedData->mCrashState << ",";
	ss << "\"mAeroDamage\":" << sharedData->mAeroDamage << ",";
	ss << "\"mEngineDamage\":" << sharedData->mEngineDamage << "},";
}

void sendWeather(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"weather\":{";
	ss << "\"mAmbientTemperature\":" << sharedData->mAmbientTemperature << ",";
	ss << "\"mTrackTemperature\":" << sharedData->mTrackTemperature << ",";
	ss << "\"mRainDensity\":" << sharedData->mRainDensity << ",";
	ss << "\"mWindSpeed\":" << sharedData->mWindSpeed << ",";
	ss << "\"mWindDirectionX\":" << sharedData->mWindDirectionX << ",";
	ss << "\"mWindDirectionY\":" << sharedData->mWindDirectionY << ",";
	ss << "\"mCloudBrightness\":" << sharedData->mCloudBrightness << "}";
}

void sendSharedMemoryJson(struct ns_connection *nc, const SharedMemory* sharedData)  {
	/* Send HTTP OK response header */
	ns_printf(nc, "%s", HTTP_RESPONSE_200);

	std::stringstream ss;

	// Start of JSON output
	ss << "{";

	sendBuildInfo(ss, sharedData);
	sendGameStates(ss, sharedData);
	sendParticipants(ss, sharedData);
	sendUnfilteredInput(ss, sharedData);
	sendVehicleInformation(ss, sharedData);
	sendEventInformation(ss, sharedData);
	sendTimings(ss, sharedData);
	sendFlags(ss, sharedData);
	sendPitInfo(ss, sharedData);
	sendCarState(ss, sharedData);
	sendMotionDeviceRelated(ss, sharedData);
	sendWheelsTyres(ss, sharedData);
	sendCarDamage(ss, sharedData);
	sendWeather(ss, sharedData);
	ss << "}";
	ns_printf_http_chunk(nc, ss.str().c_str());
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