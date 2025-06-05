#pragma once

#include "String.h"

class Response
{
public:
	Response(bool success, const String& message = "")
		: success_(success), message_(message) {}

	bool isSuccessful() const { return this->success_; }
	const String& getMessage() const { return this->message_; }

private:
	bool success_;
	String message_;
};