#include "Headers.h"
#include "SkyeTekAPI.h"
#include "SkyeTekProtocol.h"
#include "ConnectMode.h"
#include "WinMain.h" 

/*------------------------------------------------------------------------------------------------------------------
-- CLASS: ConnectMode.cpp - This class is used by the ConnectMode thread to handle reading from the RFID.
--
-- FUNCTIONS:
--		void EstablishConnection(Void) used to connect to the RFID.
--		Constructor which gets a handler to the window passed into it.
--		unsigned char TagFound(LPSKYETEK_TAG tag, void* user) is called by the SkyeTek API when a read is found.
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe, Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe, Dhivya Manohar
--
-- NOTES:
--
----------------------------------------------------------------------------------------------------------------------*/

//Tags to date that have been scanned.
std::vector<Tag> tags;

//Count of all readers found when calling Skyeteks API call for discover readers.
UINT readerCount = 0;

//Pointer to the SkyeTek readers discovered.
LPSKYETEK_READER* readers = NULL;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TagFound
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe / Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe / Dhivya Manohar
--
-- INTERFACE: void TagFound(
--					LPSKYETEK_TAG tag recently read that is to be parsed and drawn to the screen.
--					void* user, unused parameter required by the SkyeTek API .)
--
-- RETURNS: unsigned char which is used as a boolean by the SkyeTek API.
--
-- NOTES:
-- Called byt he SkyeTek API once a tag has been scanned.
----------------------------------------------------------------------------------------------------------------------*/
unsigned char TagFound(LPSKYETEK_TAG tag, void* user) {
	if (tag == NULL) {
		return true;
	}
	string temp;
	string type;
	type += "Type: ";
	TCHAR* s = tag->friendly;
	while (*s != '\0') {
		temp += *s;
		s++;
	}
	TCHAR* s2 = SkyeTek_GetTagTypeNameFromType(tag->type);
	while (*s2 != '\0') {
		type += *s2;
		s2++;
	}

	if (tags.size() == 0) {
		Tag t(temp, type);
		tags.push_back(t);
		drawTag(tags);
		return true;
	}

	for (int i = 0; i < tags.size(); i++) {
		if (temp.compare(tags[i].friendly) == 0) {
			drawTag(tags, i);
			return true;
		}
	}
	Tag t(temp, type);
	tags.push_back(t);

	drawTag(tags);
	return true;
}

//Default constructor.
ConnectMode::ConnectMode() {}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: ConnectMode Constructor
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe / Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe / Dhivya Manohar
--
-- INTERFACE: ConnectMode(HWND hw\)
--
-- NOTES:
-- This constructor is used to instantiate the ConnectMode object and pass in the handle to the applications UI Window.
----------------------------------------------------------------------------------------------------------------------*/
ConnectMode::ConnectMode(HWND hw) {
	hwnd = hw;
}

//Destructor, shuts off infinite loop before deleting itself.
ConnectMode::~ConnectMode() {
	//Stops the infinite loop that is keeping the thread alive.
	keepLooping = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- METHOD: EstablishConnection
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe / Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe / Dhivya Manohar
--
-- INTERFACE: void EstablishConnection()
--
-- RETURNS: void.
--
-- NOTES:
-- This method discovers the devices and readers on the devices. if any are found, this assigns the reader
-- the method to be called whenever something is read. Upon compelting, it frees the devices.
----------------------------------------------------------------------------------------------------------------------*/
void ConnectMode::EstablishConnection() {
	LPSKYETEK_DEVICE* devices = NULL;
	UINT deviceCount = 0;

	if ((deviceCount = SkyeTek_DiscoverDevices(&devices)) > 0) {
		if (readerCount > 0 || (readerCount = SkyeTek_DiscoverReaders(devices, deviceCount, &readers)) > 0) {
			LPSKYETEK_TAG tag = NULL;
			OutputDebugStringA("Cycling through tags\n");
			SkyeTek_SelectTags(readers[0], AUTO_DETECT, TagFound, 0, 1, NULL);
			OutputDebugStringA("Reading finished. Stay alive.\n");
		}
	}

	//While the thread needs to be kept alive.
	while (keepLooping);
	
	//Once thread no longer needs to be alive, free readers.
	SkyeTek_FreeDevices(devices, deviceCount);
}

