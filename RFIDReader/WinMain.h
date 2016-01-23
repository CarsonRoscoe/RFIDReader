#pragma once
#define IDM_TX			100
#define IDM_RX			101
#define IDM_Port		102
#define IDM_Speed		103
#define IDM_Word		104
#define IDM_ZETA		105
#define IDM_Save		106
#define IDM_THETA		107
#define IDM_HELP		108
#define IDM_COM1		109
#define IDM_COM2		110
#define IDM_9600		111
#define IDM_32000		112
#define IDM_110000		113
#define IDM_CONNECT		114
#define IDM_DISCONNECT	115
#include <vector>

/*------------------------------------------------------------------------------------------------------------------
-- STRUCTURE: Tag
--
-- DATE: October 19th, 2015
--
-- REVISIONS: N/A
--
-- DESIGNER: Carson Roscoe / Dhivya Manohar
--
-- PROGRAMMER: Carson Roscoe / Dhivya Manohar
--
-- MEMBERS: string friendly is a string which is treated as the name/identifier of the Tag.
--			string type which holds the string to be drawn which represents this Tag's type.
--			Constructor which sets friendly and type.
--
-- NOTES:
-- This method discovers the devices and readers on the devices. if any are found, this assigns the reader
-- the method to be called whenever something is read. Upon compelting, it frees the devices.
----------------------------------------------------------------------------------------------------------------------*/
struct Tag {
	std::string friendly;
	std::string type;
	Tag(std::string f, std::string t) {
		friendly = f;
		type = t;
	}
};

void drawTags();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ConnectModeThread(LPVOID n);
void drawTag();
void drawTag(std::vector<Tag> tTags);
void drawTag(std::vector<Tag> tTags, int index);