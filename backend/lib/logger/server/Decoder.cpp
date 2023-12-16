#include "Decoder.hpp"

#include <logger/server/Server.hpp>
#include <logger/user/Protocol.hpp>

#include <limits>

namespace leo::logger
{

class Decoder::Impl
{
	static constexpr auto EVENT_ID_NO_MATCH = std::numeric_limits<size_t>::max();
	static constexpr auto NOT_ENOUGH_DATA = std::numeric_limits<size_t>::max() - 1;

public:
	Impl(Server& server)
		: server{server}
	{}

	Decoder::DecodeResult decode(Buffer& buffer)
	{
		while (buffer.getReadableSize() > 0)
		{
			const auto& header = buffer.get<logger::protocol::Header>();
			//server.handleHeader(header);

			if (!isInSequence(header.sequenceNumber))
			{
				std::cout << "SeqNum violation " << seqNum << " --> " << header.sequenceNumber
						  << '\n';
				//return DecodeResult::SEQ_NUM_VIOLATION;
			}

			const auto result = decodeEvent<AllLogEvents>(buffer, header);

			switch (result)
			{
			case EVENT_ID_NO_MATCH:
				return DecodeResult::UNKOWN_EVENT;
			case NOT_ENOUGH_DATA:
				return DecodeResult::NOT_ENOUGH_DATA;
			}

			++seqNum;
			buffer.consume(result);
		}

		return DecodeResult::SUCCESS;
	}

private:
	template <typename ListOfEvents>
	size_t decodeEvent(Buffer& buffer, const logger::protocol::Header& header)
	{
		if constexpr (IsEmptyV<ListOfEvents>)
		{
			return EVENT_ID_NO_MATCH;
		}
		else
		{
			using AnEvent = GetHeadT<ListOfEvents>;

			if (IndexOfV<AnEvent, AllLogEvents> == header.eventId)
			{
				return handleLogEvent<AnEvent>(buffer, header);
			}
			else
			{
				return decodeEvent<PopFrontT<ListOfEvents>>(buffer, header);
			}
		}
	}

	template <typename Event>
	size_t handleLogEvent(Buffer& buffer, const logger::protocol::Header& header)
	{
		const auto* eventPtr = buffer.peek<Event>();
		if (eventPtr)
		{
			server.handleEvent(header, *eventPtr);
			return sizeof(Event);
		}
		return NOT_ENOUGH_DATA;
	}

	bool isInSequence(protocol::SequenceNumber dataSeqNum) const
	{
		return seqNum == dataSeqNum;
	}

private:
	Server& server;
	protocol::SequenceNumber seqNum{0};
};

// Interface class

Decoder::Decoder(Server& server)
	: pimpl{std::make_unique<Impl>(server)}
{}

Decoder::~Decoder() = default;

Decoder::DecodeResult Decoder::decode(Buffer& buffer)
{
	return pimpl->decode(buffer);
}

} // namespace leo::logger