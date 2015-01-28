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
#define HTTP_HEADER_NO_CONTENT "Content-Length:0\r\n"
#define HTTP_RESPONSE_503 "HTTP/1.1 503 Service Unavailable\r\n"
#define HTTP_RESPONSE_409 "HTTP/1.1 409 Conflict\r\n"
#define HTTP_RESPONSE_200 "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
#define HTTP_RESPONSE_404 "HTTP/1.1 404 Not found\r\n"

// Request handler
static void handle_request(struct ns_connection *nc, struct http_message *hm) {

	// Open the memory mapped file
	HANDLE fileHandle = OpenFileMapping(PAGE_READONLY, FALSE, MAP_OBJECT_NAME);
	if (fileHandle == NULL)	{
		// Send service unavailable response
		ns_printf(nc, "%s", HTTP_RESPONSE_503);
		ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
		printf("Could not open file mapping object (%d).\n", GetLastError());
	}
	else{

		// Get the data structure
		const SharedMemory* sharedData = (SharedMemory*)MapViewOfFile(fileHandle, PAGE_READONLY, 0, 0, sizeof(SharedMemory));
		if (sharedData == NULL)	{
			// Send service unavailable response
			ns_printf(nc, "%s", HTTP_RESPONSE_503);
			ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
			printf("Could not map view of file (%d).\n", GetLastError());
			CloseHandle(fileHandle);
		}
		else{

			// Ensure we're sync'd to the correct data version
			if (sharedData->mVersion != SHARED_MEMORY_VERSION)	{
				// Send conflict response
				ns_printf(nc, "%s", HTTP_RESPONSE_409);
				ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
				printf("Data version mismatch, upgrade your CREST API server\n");
			}
			else{
				/* Send OK response */
				ns_printf(nc, "%s", HTTP_RESPONSE_200);

				// Start printing the output
				ns_printf_http_chunk(nc, "{ Start of game data }");

				const bool isValidParticipantIndex = sharedData->mViewedParticipantIndex != -1 && sharedData->mViewedParticipantIndex < sharedData->mNumParticipants && sharedData->mViewedParticipantIndex < STORED_PARTICIPANTS_MAX;
				if (isValidParticipantIndex)	{
					const ParticipantInfo& viewedParticipantInfo = sharedData->mParticipantInfo[sharedData->mViewedParticipantIndex];
					ns_printf_http_chunk(nc, "mParticipantName: (%s)\n", viewedParticipantInfo.mName);
					ns_printf_http_chunk(nc, "mWorldPosition: (%f,%f,%f)\n", viewedParticipantInfo.mWorldPosition[0], viewedParticipantInfo.mWorldPosition[1], viewedParticipantInfo.mWorldPosition[2]);
				}
				ns_printf_http_chunk(nc, "mGameState: (%d)\n", sharedData->mGameState);
				ns_printf_http_chunk(nc, "mSessionState: (%d)\n", sharedData->mSessionState);
				ns_printf_http_chunk(nc, "mRaceState: (%d)\n", sharedData->mRaceState);

				ns_printf_http_chunk(nc, "{ End of game data }");

				// Cleanup file
				UnmapViewOfFile(sharedData);
				CloseHandle(fileHandle);
			}
		}
	}
	// Empty chunk marks the end of the response
	ns_send_http_chunk(nc, "", 0);

}

static void ev_handler(struct ns_connection *nc, int ev, void *ev_data) {
	struct http_message *hm = (struct http_message *) ev_data;

	switch (ev) {
	case NS_HTTP_REQUEST:
		if (ns_vcmp(&hm->uri, "/crest/v1/api") == 0) {
			handle_request(nc, hm);
		}else{
			ns_printf(nc, "%s", HTTP_RESPONSE_404);
			ns_printf(nc, "%s", HTTP_HEADER_NO_CONTENT);
			// Empty chunk marks the end of the response
			ns_send_http_chunk(nc, "", 0);
		}
		break;
	default:
		break;
	}
}

int main()	{

	// Server initialization
	struct ns_mgr mgr;
	struct ns_connection *nc;
	ns_mgr_init(&mgr, NULL);
	nc = ns_bind(&mgr, s_http_port, ev_handler);
	ns_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = ".";

	printf("CREST - CARS REST API - Server started on port %s\n", s_http_port);
	printf("Press ESC to terminate\n\n");
	// Keep polling until ESC is hit
	while (true)	{

		ns_mgr_poll(&mgr, POLL_TIME_IN_MILLIS);

		if (_kbhit() && _getch() == ESC_KEY)	{
			break;
		}
	}

	// Free server and exit
	ns_mgr_free(&mgr);
	return 0;
}
