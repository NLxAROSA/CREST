# README #

# Introducing CREST - also known as the CARS REST API #

This project provides a way for developers to have access to the data provided by Project CARS Shared Memory in a more portable and accessible format than a mapped memory file.

It's a windows native command line application (written in C++ and a fair bit of plain old C) that will act as a local web/application server and serve the data via JSON over HTTP by accessing a URL.

Any web browser or HTTP client will be able to access the data both locally and inside the local network, which facilitates easy add-on development in any language (e.g. JavaScript, Java, C, C++, C#, etc. etc.) and platform (e.g. web browser, iOS, Android, Windows Phone or other) with a vast array of client REST libraries at your disposal.

* See http://www.projectcarsgame.com for more info on Project CARS.
* Latest binary download is always available at http://cars-rest-api.com/#downloads

# Building from source

1. Download the source code
2. Open it up in Visual Studio 2013

The project won't build without the proper setup of zlib. To properly enable zlib:

1. Download and extract the compiled zlib.dll from http://www.zlib.net
2. Follow the directions in USAGE.txt to enable it in the project. More specifically this means:

  * Install the supplied header files "zlib.h" and "zconf.h" into a directory found in the INCLUDE path list.
  * Install the supplied library file "zdll.lib" into a directory found in the LIB path list.
  * Add "zdll.lib" to your project.

# Running CREST

1. Download the executable from our website (http://www.cars-rest-api.com) at http://cars-rest-api.com/#downloads or build it yourself.
2. Start the application by double-clicking it.
3. Windows will complain about security (coming from an unknown/unverified source) both when downloading and starting the application. Please allow/unblock the application in order to allow it to start. Windows Firewall will also ask for permission to access the local network. Please allow it access for proper functioning.
4. If Windows complains about missing DLLs (e.g. MSVCR120.DLL or similar), please download and install the Visual C++ Redistributable Packages for Visual Studio 2013 from http://www.microsoft.com/en-us/download/details.aspx?id=40784 (vcredist_x86.exe), then start again.
5. Console will show a notification about the server being up and running and display port and URL information.

# Connecting to Project CARS

1. Start Project CARS.
2. Enable Shared Memory in Options -> Visual -> Hardware
3. Open up a browser (Chrome for best results) and go to http://localhost:8080/crest/v1/api
4. Profit! F5/Refresh until you drop!

Be sure to checkout the website at http://www.cars-rest-api.com as well!

# Advanced usage

The basic URL (http://localhost:8080/crest/v1/api) will expose the full set of data. If parameters are added to the request URL, the set will be limited to the parameters you specify.

The following parameters are available:

1. buildInfo
2. gameStates
3. participants
4. unfilteredInputs
5. vehicleInformation
6. eventInformation
7. timings
8. flags
9. pitInfo
10. carState
11. motionDeviceRelated
12. wheelsAndTyres
13. carDamage
14. weather

Example:  http://localhost:8080/crest/v1/api?buildInfo=true will give you just the buildInfo.

Example2: http://localhost:8080/crest/v1/api?timings=true&carState=true&weather=true will give you timings, carState and weather.

Example3: http://localhost:8080/crest/v1/api will give you the full set.

NOTE: Sections are always in the order listed, never in the order of the request parameters.

# Compression

CREST can gzip the response body. Just add the following header to the HTTP request: "Accept-Encoding: gzip" and CREST will gzip the response and set the "Content-Encoding: gzip" HTTP response header. Regardless of the header, CREST will not compress output if the response is smaller than 128 bytes, to prevent overhead.
