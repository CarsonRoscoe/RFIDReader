#ifndef CONNECTMODE_H
#define CONNECTMODE_H
#include<vector>
#include "Headers.h"
#include "SkyeTekAPI.h"
#include "SkyeTekProtocol.h"

//Prototype for a functionw hich is called whenever new tag is scanned.
unsigned char TagFound(LPSKYETEK_TAG tag, void* user);

class ConnectMode
{
	//Handle to the window
	HWND hwnd;

	//Bool over whether the program will loop indefinitely reading.
	bool keepLooping = true;

public:
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
	ConnectMode(HWND hw);

	//Default constructor.
	ConnectMode();

	//Destructor, which turns keepLooping to false which will, in turn, free the devices from the loop.
	~ConnectMode();

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
	void EstablishConnection();
};

#endif