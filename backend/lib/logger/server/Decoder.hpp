#pragma once

#include <common/Buffer.hpp>

#include <functional>

namespace leo::logger
{

class Server;

class Decoder
{
public:
	enum class DecodeResult
	{
		SUCCESS,
		NOT_ENOUGH_DATA,
		MALFORMED_DATA,
		SEQ_NUM_VIOLATION,
		UNKOWN_EVENT,
	};

public:
	Decoder(Server& server);

	~Decoder();

	DecodeResult decode(Buffer& buffer);

public:
	class Impl; // Had to be public to mark as friend in Server.hpp

private:
	std::unique_ptr<Impl> pimpl;
};

} // namespace leo::logger