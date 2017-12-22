#pragma once

#ifndef WEB_SERVER_REQUEST_MACROS_H
#define WEB_SERVER_REQUEST_MACROS_H

enum REQUEST_METHOD
{
	GET,
	POST,
	PUT,
	DElETE,
	none
};

enum CONTENT_TYPE
{
	JSON,
	X_WWW_FORM_URLENCODED,
	NONE
};

#define JSON_TYPE "application/json"
#define X_WWW_FORM_URLENCODED_TYPE "application/x-www-form-urlencoded"

#endif
