#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "sharedmemory.h"
#include "fossa.h"

// Configuration properties
static const char *s_http_port = "8080";
static struct ns_serve_http_opts s_http_server_opts;
#define MAP_OBJECT_NAME "$pcars$"
#define POLL_TIME_IN_MILLIS 100
#define ESC_KEY 27
#define CREST_API_URL "/crest/v1/api"
#define HTTP_HEADER_NO_CONTENT "Content-Length:0\r\n"
#define HTTP_RESPONSE_503 "HTTP/1.1 503 Service Unavailable\r\n"
#define HTTP_RESPONSE_409 "HTTP/1.1 409 Conflict\r\n"
#define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 Not found\r\n"

static void sendServiceUnavailable(struct ns_connection *nc)    {
    ns_printf(nc, "%s", HTTP_RESPONSE_503);
    ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

static void sendConflict(struct ns_connection *nc)    {
    ns_printf(nc, "%s", HTTP_RESPONSE_503);
    ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
}

static void sendBuildInfo(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"buildinfo\":{");
	ns_printf_http_chunk(nc, "\"mVersion\":%u", sharedData->mVersion);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mBuildVersionNumber\":%u", sharedData->mBuildVersionNumber);
	ns_printf_http_chunk(nc, "}");
}

static void sendGameStates(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"gameStates\":{");
	ns_printf_http_chunk(nc, "\"mGameState\":%u", sharedData->mGameState);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mSessionState\":%u", sharedData->mSessionState);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mRaceState\":%u", sharedData->mRaceState);
	ns_printf_http_chunk(nc, "}");
}

static void sendParticipant(struct ns_connection *nc, const ParticipantInfo participantInfo)	{
	ns_printf_http_chunk(nc, "\"mIsActive\":%s", participantInfo.mIsActive ? "true" : "false");
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mName\":\"%s\"", participantInfo.mName);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldPositionX\":%f", participantInfo.mWorldPosition[0]);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldPositionY\":%f", participantInfo.mWorldPosition[1]);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldPositionZ\":%f", participantInfo.mWorldPosition[2]);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentLapDistance\":%f", participantInfo.mCurrentLapDistance);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mRacePosition\":%u", participantInfo.mRacePosition);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mLapsCompleted\":%u", participantInfo.mLapsCompleted);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentLap\":%u", participantInfo.mCurrentLap);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentSector\":%u", participantInfo.mCurrentSector);
	ns_printf_http_chunk(nc, "}");
}

static void sendParticipants(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"participants\":{");
	ns_printf_http_chunk(nc, "\"mViewedParticipantIndex\":%i", sharedData->mViewedParticipantIndex);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mNumParticipants\":%i", sharedData->mNumParticipants);

	if (sharedData->mNumParticipants > -1)	{
		ns_printf_http_chunk(nc, ",");
		ns_printf_http_chunk(nc, "\"mParticipantInfo\":[");

		for (size_t i = 0; i < sharedData->mNumParticipants; i++)	{
			sendParticipant(nc, sharedData->mParticipantInfo[i]);
		}
		ns_printf_http_chunk(nc, "]");

	}

	ns_printf_http_chunk(nc, "}");

}

static void sendUnfilteredInput(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"unfilteredInput\":{");
	ns_printf_http_chunk(nc, "\"mUnfilteredThrottle\":%f", sharedData->mUnfilteredThrottle);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mUnfilteredBrake\":%f", sharedData->mUnfilteredBrake);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mUnfilteredSteering\":%f", sharedData->mUnfilteredSteering);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mUnfilteredClutch\":%f", sharedData->mUnfilteredClutch);
	ns_printf_http_chunk(nc, "}");
}

static void sendVehicleInformation(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"vehicleInformation\":{");
	ns_printf_http_chunk(nc, "\"mCarName\":\"%s\"", sharedData->mCarName);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCarClassName\":\"%s\"", sharedData->mCarClassName);
	ns_printf_http_chunk(nc, "}");
}

static void sendEventInformation(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"eventInformation\":{");
	ns_printf_http_chunk(nc, "\"mLapsInEvent\":%u", sharedData->mLapsInEvent);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mTrackLocation\":%s", sharedData->mTrackLocation);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mTrackVariation\":%s", sharedData->mTrackVariation);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mTrackLength\":%f", sharedData->mTrackLength);
	ns_printf_http_chunk(nc, "}");
}

static void sendTimings(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"timings\":{");
	ns_printf_http_chunk(nc, "\"mLapInvalidated\":%s", sharedData->mLapInvalidated ? "true" : "false");
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mBestLapTime\":%f", sharedData->mBestLapTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mLastLapTime\":%f", sharedData->mLastLapTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentTime\":%f", sharedData->mCurrentTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mSplitTimeAhead\":%f", sharedData->mSplitTimeAhead);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mSplitTimeBehind\":%f", sharedData->mSplitTimeBehind);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mSplitTime\":%f", sharedData->mSplitTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mEventTimeRemaining\":%f", sharedData->mEventTimeRemaining);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mPersonalFastestLapTime\":%f", sharedData->mPersonalFastestLapTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldFastestLapTime\":%f", sharedData->mWorldFastestLapTime);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentSector1Time\":%f", sharedData->mCurrentSector1Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentSector2Time\":%f", sharedData->mCurrentSector2Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mCurrentSector3Time\":%f", sharedData->mCurrentSector3Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mFastestSector1Time\":%f", sharedData->mFastestSector1Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mFastestSector2Time\":%f", sharedData->mFastestSector2Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mFastestSector3Time\":%f", sharedData->mFastestSector3Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector1Time\":%f", sharedData->mPersonalFastestSector1Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector2Time\":%f", sharedData->mPersonalFastestSector2Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mPersonalFastestSector3Time\":%f", sharedData->mPersonalFastestSector3Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldFastestSector1Time\":%f", sharedData->mWorldFastestSector1Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldFastestSector2Time\":%f", sharedData->mWorldFastestSector2Time);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mWorldFastestSector3Time\":%f", sharedData->mWorldFastestSector3Time);
	ns_printf_http_chunk(nc, "}");
}

static void sendFlags(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"flags\":{");
	ns_printf_http_chunk(nc, "\"mHighestFlagColour\":%u", sharedData->mHighestFlagColour);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mHighestFlagReason\":%u", sharedData->mHighestFlagReason);
	ns_printf_http_chunk(nc, "}");
}

static void sendPitInfo(struct ns_connection *nc, const SharedMemory* sharedData)	{
	ns_printf_http_chunk(nc, "\"pitInfo\":{");
	ns_printf_http_chunk(nc, "\"mPitMode\":%u", sharedData->mPitMode);
	ns_printf_http_chunk(nc, ",");
	ns_printf_http_chunk(nc, "\"mPitSchedule\":%u", sharedData->mPitSchedule);
	ns_printf_http_chunk(nc, "}");
}

static void sendSharedMemoryJson(struct ns_connection *nc, const SharedMemory* sharedData)  {
    /* Send HTTP OK response header */
    ns_printf(nc, "%s", HTTP_RESPONSE_200);
    
    // Start of JSON output
    ns_printf_http_chunk(nc, "{");

	sendBuildInfo(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendGameStates(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendParticipants(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendUnfilteredInput(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendVehicleInformation(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendEventInformation(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendTimings(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendFlags(nc, sharedData);
	ns_printf_http_chunk(nc, ",");
	sendPitInfo(nc, sharedData);

	const bool isValidParticipantIndex = sharedData->mViewedParticipantIndex != -1 && sharedData->mViewedParticipantIndex < sharedData->mNumParticipants && sharedData->mViewedParticipantIndex < STORED_PARTICIPANTS_MAX;
    if (isValidParticipantIndex)	{
        const ParticipantInfo& viewedParticipantInfo = sharedData->mParticipantInfo[sharedData->mViewedParticipantIndex];
        //ns_printf_http_chunk(nc, "mParticipantName: (%s)\n", viewedParticipantInfo.mName);
        //ns_printf_http_chunk(nc, "mWorldPosition: (%f,%f,%f)\n", viewedParticipantInfo.mWorldPosition[0], viewedParticipantInfo.mWorldPosition[1], viewedParticipantInfo.mWorldPosition[2]);
    }
    //ns_printf_http_chunk(nc, "mGameState: (%d)\n", sharedData->mGameState);
    //ns_printf_http_chunk(nc, "mSessionState: (%d)\n", sharedData->mSessionState);
    //ns_printf_http_chunk(nc, "mRaceState: (%d)\n", sharedData->mRaceState);
    
	// End of JSON output
	ns_printf_http_chunk(nc, "}");

}

static void processSharedMemoryData(struct ns_connection *nc, const SharedMemory* sharedData)   {
    // Ensure we're sync'd to the correct data version
    if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
        // Send conflict response
        sendConflict(nc);
        printf("Data version mismatch, please make sure that your pCARS version matches your CREST version\n");
    }else{
        sendSharedMemoryJson(nc, sharedData);
    }

}

static void processFile(struct ns_connection *nc, HANDLE fileHandle)    {
    
    const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));
    
    if (sharedData == NULL)	{
        // File found, but could not be mapped to shared memory data
        sendServiceUnavailable(nc);
        printf("Could not map view of file (%d).\n", GetLastError());
    }else{
        // Process file
        processSharedMemoryData(nc, sharedData);
        // Unmap file
        UnmapViewOfFile(sharedData);
    }

}

static void handle_request(struct ns_connection *nc, struct http_message *hm) {

	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMapping(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);
    
	if (fileHandle == NULL)	{
		// File is not available, send service unavailable response
        sendServiceUnavailable(nc);
		printf("Could not open file mapping object (%d).\n", GetLastError());
	}else{
		// File is available, process the file
        processFile(nc, fileHandle);
        // Close the file
        CloseHandle(fileHandle);
	}
	// Send empty chunk to mark the end of the response
	ns_send_http_chunk(nc, "", 0);

}

static void ev_handler(struct ns_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) {
	case NS_HTTP_REQUEST:
        // Only handle requests on the API url
		if (ns_vcmp(&hm->uri, CREST_API_URL) == 0) {
			handle_request(nc, hm);
		}else{
            // Return a 404
			ns_printf(nc, "%s", HTTP_RESPONSE_404);
			ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
			ns_send_http_chunk(nc, "", 0);
		}
		break;
	default:
		break;
	}
}

int main()	{

	// Initialize the server
	struct ns_mgr mgr;
	struct ns_connection *nc;
	ns_mgr_init(&mgr, NULL);
	nc = ns_bind(&mgr, s_http_port, ev_handler);
	ns_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = ".";
    // Print some information on the console
	printf("CREST - CARS REST API - Server started on port %s\n", s_http_port);
	printf("Press ESC to terminate\n\n");
	// Keep polling until ESC is hit
	while (true)	{

		ns_mgr_poll(&mgr, POLL_TIME_IN_MILLIS);

		if (_kbhit() && _getch() == ESC_KEY)	{
			break;
		}
	}

	// We're done, free up the server and exit
	ns_mgr_free(&mgr);
	return 0;
}
