#include "SharedMemoryRenderer.h"
#include "Utils.h"

SharedMemoryRenderer::SharedMemoryRenderer(){};

void renderBuildInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"buildinfo\":{";
	ss << "\"mVersion\":" << sharedData->mVersion << ",";
	ss << "\"mBuildVersionNumber\":" << sharedData->mBuildVersionNumber << "}";
}

void renderGameStates(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"gameStates\":{";
	ss << "\"mGameState\":" << sharedData->mGameState << ",";
	ss << "\"mSessionState\":" << sharedData->mSessionState << ",";
	ss << "\"mRaceState\":" << sharedData->mRaceState << "}";
}

void renderParticipant(std::stringstream& ss, const ParticipantInfo participantInfo)	{
	ss << "{\"mIsActive\":" << (participantInfo.mIsActive ? "true" : "false") << ",";
	ss << "\"mName\":\"" << participantInfo.mName << "\",";
	ss << "\"mWorldPosition\":[" << participantInfo.mWorldPosition[0] << "," << participantInfo.mWorldPosition[1] << "," << participantInfo.mWorldPosition[2] << "],";
	ss << "\"mCurrentLapDistance\":" << participantInfo.mCurrentLapDistance << ",";
	ss << "\"mRacePosition\":" << participantInfo.mRacePosition << ",";
	ss << "\"mLapsCompleted\":" << participantInfo.mLapsCompleted << ",";
	ss << "\"mCurrentLap\":" << participantInfo.mCurrentLap << ",";
	ss << "\"mCurrentSector\":" << participantInfo.mCurrentSector << "}";
}

void renderParticipants(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"participants\":{";
	ss << "\"mViewedParticipantIndex\":" << sharedData->mViewedParticipantIndex << ",";
	ss << "\"mNumParticipants\":" << sharedData->mNumParticipants;

	if (sharedData->mNumParticipants > -1)	{
		ss << ",";
		ss << "\"mParticipantInfo\":[";

		for (int i = 0; i < sharedData->mNumParticipants; i++)	{
			renderParticipant(ss, sharedData->mParticipantInfo[i]);
			if (i < (sharedData->mNumParticipants - 1))	{
				ss << ",";
			}
		}
		ss << "]";
	}
	ss << "}";
}

void renderUnfilteredInput(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"unfilteredInput\":{";
	ss << "\"mUnfilteredThrottle\":" << sharedData->mUnfilteredThrottle << ",";
	ss << "\"mUnfilteredBrake\":" << sharedData->mUnfilteredBrake << ",";
	ss << "\"mUnfilteredSteering\":" << sharedData->mUnfilteredSteering << ",";
	ss << "\"mUnfilteredClutch\":" << sharedData->mUnfilteredClutch << "}";
}

void renderVehicleInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"vehicleInformation\":{";
	ss << "\"mCarName\":\"" << sharedData->mCarName << "\",";
	ss << "\"mCarClassName\":\"" << sharedData->mCarClassName << "\"}";
}

void renderEventInformation(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"eventInformation\":{";
	ss << "\"mLapsInEvent\":" << sharedData->mLapsInEvent << ",";
	ss << "\"mTrackLocation\":\"" << sharedData->mTrackLocation << "\",";
	ss << "\"mTrackVariation\":\"" << sharedData->mTrackVariation << "\",";
	ss << "\"mTrackLength\":" << sharedData->mTrackLength << "}";
}

void renderTimings(std::stringstream& ss, const SharedMemory* sharedData)	{
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
	ss << "\"mWorldFastestSector3Time\":" << sharedData->mWorldFastestSector3Time << "}";
}

void renderFlags(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"flags\":{";
	ss << "\"mHighestFlagColour\":" << sharedData->mHighestFlagColour << ",";
	ss << "\"mHighestFlagReason\":" << sharedData->mHighestFlagReason << "}";
}

void renderPitInfo(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"pitInfo\":{";
	ss << "\"mPitMode\":" << sharedData->mPitMode << ",";
	ss << "\"mPitSchedule\":" << sharedData->mPitSchedule << "}";
}

void renderCarState(std::stringstream& ss, const SharedMemory* sharedData)	{
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
	ss << "\"mBoostAmount\":" << sharedData->mBoostAmount << "}";
}

void renderMotionDeviceRelated(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"motionAndDeviceRelated\":{";
	ss << "\"mOrientation\":[" << sharedData->mOrientation[0] << "," << sharedData->mOrientation[1] << "," << sharedData->mOrientation[2] << "],";
	ss << "\"mLocalVelocity\":[" << sharedData->mLocalVelocity[0] << "," << sharedData->mLocalVelocity[1] << "," << sharedData->mLocalVelocity[2] << "],";
	ss << "\"mWorldVelocity\":[" << sharedData->mWorldVelocity[0] << "," << sharedData->mWorldVelocity[1] << "," << sharedData->mWorldVelocity[2] << "],";
	ss << "\"mAngularVelocity\":[" << sharedData->mAngularVelocity[0] << "," << sharedData->mAngularVelocity[1] << "," << sharedData->mAngularVelocity[2] << "],";
	ss << "\"mLocalAcceleration\":[" << sharedData->mLocalAcceleration[0] << "," << sharedData->mLocalAcceleration[1] << "," << sharedData->mLocalAcceleration[2] << "],";
	ss << "\"mWorldAcceleration\":[" << sharedData->mWorldAcceleration[0] << "," << sharedData->mWorldAcceleration[1] << "," << sharedData->mWorldAcceleration[2] << "],";
	ss << "\"mExtentsCentre\":[" << sharedData->mExtentsCentre[0] << "," << sharedData->mExtentsCentre[1] << "," << sharedData->mExtentsCentre[2] << "]}";
}

void renderWheelsTyres(std::stringstream& ss, const SharedMemory* sharedData)	{
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
	ss << "\"mTyreInternalAirTemp\":[" << sharedData->mTyreInternalAirTemp[0] << "," << sharedData->mTyreInternalAirTemp[1] << "," << sharedData->mTyreInternalAirTemp[2] << "," << sharedData->mTyreInternalAirTemp[3] << "]}";
}

void renderCarDamage(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"carDamage\":{";
	ss << "\"mCrashState\":" << sharedData->mCrashState << ",";
	ss << "\"mAeroDamage\":" << sharedData->mAeroDamage << ",";
	ss << "\"mEngineDamage\":" << sharedData->mEngineDamage << "}";
}

void renderWeather(std::stringstream& ss, const SharedMemory* sharedData)	{
	ss << "\"weather\":{";
	ss << "\"mAmbientTemperature\":" << sharedData->mAmbientTemperature << ",";
	ss << "\"mTrackTemperature\":" << sharedData->mTrackTemperature << ",";
	ss << "\"mRainDensity\":" << sharedData->mRainDensity << ",";
	ss << "\"mWindSpeed\":" << sharedData->mWindSpeed << ",";
	ss << "\"mWindDirectionX\":" << sharedData->mWindDirectionX << ",";
	ss << "\"mWindDirectionY\":" << sharedData->mWindDirectionY << ",";
	ss << "\"mCloudBrightness\":" << sharedData->mCloudBrightness << "}";
}

// Adds a comma, unless skipped
void addSeparator(std::stringstream& ss, bool skip)	{
	if (!skip)	{
		ss << ",";
	}
}

// Returns true if the given section should be rendered, based on the presence
// of the sections name parameter in the query string
bool shouldRender(std::string queryString, std::string sectionName)	{
	std::stringstream ss;
	ss << sectionName << "=true";
	return queryString.empty() || Utils::contains(queryString, ss.str());
}

std::string SharedMemoryRenderer::render(const SharedMemory* sharedData, std::string queryString)	{

	std::stringstream ss;

	ss << "{";
	bool skipSeparator = true;
	if (shouldRender(queryString, "buildInfo"))	{
		renderBuildInfo(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "gameStates"))	{
		addSeparator(ss, skipSeparator);
		renderGameStates(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "participants"))	{
		addSeparator(ss, skipSeparator);
		renderParticipants(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "unfilteredInputs"))	{
		addSeparator(ss, skipSeparator);
		renderUnfilteredInput(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "vehicleInformation"))	{
		addSeparator(ss, skipSeparator);
		renderVehicleInformation(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "eventInformation"))	{
		addSeparator(ss, skipSeparator);
		renderEventInformation(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "timings"))	{
		addSeparator(ss, skipSeparator);
		renderTimings(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "flags"))	{
		addSeparator(ss, skipSeparator);
		renderFlags(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "pitInfo"))	{
		addSeparator(ss, skipSeparator);
		renderPitInfo(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "carState"))	{
		addSeparator(ss, skipSeparator);
		renderCarState(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "motionDeviceRelated"))	{
		addSeparator(ss, skipSeparator);
		renderMotionDeviceRelated(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "wheelsAndTyres"))	{
		addSeparator(ss, skipSeparator);
		renderWheelsTyres(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "carDamage"))	{
		addSeparator(ss, skipSeparator);
		renderCarDamage(ss, sharedData);
		skipSeparator = false;
	}
	if (shouldRender(queryString, "weather"))	{
		addSeparator(ss, skipSeparator);
		renderWeather(ss, sharedData);
		skipSeparator = false;
	}
	ss << "}";

	return ss.str();
}