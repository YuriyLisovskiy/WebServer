#pragma once

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
	TEXT_HTML,
	X_WWW_FORM_URLENCODED,
	FORM_DATA,
	NONE
};

#define JSON_TYPE "application/json"
#define TEXT_HTML_TYPE "text/html"
#define X_WWW_FORM_URLENCODED_TYPE "application/x-www-form-urlencoded"
#define FORM_DATA_TYPE "multipart/form-data"
