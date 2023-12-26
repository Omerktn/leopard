#include "Decoder.hpp"

#include <logger/formattedText/Decoder.hpp>
#include <logger/server/Server.hpp>
#include <logger/user/Protocol.hpp>

#include <cassert>
#include <limits>

namespace leo::logger
{

namespace
{

}

class Decoder::Impl
{
	static constexpr auto EVENT_ID_NO_MATCH = std::numeric_limits<size_t>::max();
	static constexpr auto NOT_ENOUGH_DATA = std::numeric_limits<size_t>::max() - 1;

	static constexpr auto FORMATTED_TEXT_FAILED = std::numeric_limits<size_t>::max() - 2;
	static constexpr auto FORMATTED_TYPE_ID_NO_MATCH = std::numeric_limits<size_t>::max() - 3;

public:
	Impl(Server& server)
		: server{server}
	{}

	Decoder::DecodeResult decode(Buffer& buffer)
	{
		while (buffer.getReadableSize() > 0)
		{
			const auto& header = buffer.get<protocol::Header>();

			if (!isInSequence(header.sequenceNumber))
			{
				std::cout << "SeqNum violation " << seqNum << " --> " << header.sequenceNumber
						  << '\n';
				return DecodeResult::SEQ_NUM_VIOLATION;
			}

			const auto result = decodeEvent<AllLogEvents>(buffer, header);

			switch (result)
			{
			case EVENT_ID_NO_MATCH:
				return DecodeResult::UNKOWN_EVENT;
			case NOT_ENOUGH_DATA:
				return DecodeResult::NOT_ENOUGH_DATA;
			case FORMATTED_TEXT_FAILED:
				return DecodeResult::FORMATTED_TEXT_FAILED;
			}

			++seqNum;
			buffer.consume(result);
		}

		return DecodeResult::SUCCESS;
	}

private:
	template <typename ListOfEvents>
	size_t decodeEvent(Buffer& buffer, const protocol::Header& header)
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
				if constexpr (std::is_same_v<AnEvent, log::special::FormattedText>)
				{
					return decodeFormattedText(buffer, header);
				}
				else
				{
					return handleLogEvent<AnEvent>(buffer, header);
				}
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

	size_t decodeFormattedText(Buffer& buffer, const protocol::Header& topHeader)
	{
		const auto& fmtTextHeader = buffer.get<log::special::FormattedText>();

		if (formattedParams.size() < fmtTextHeader.paramCount)
		{
			formattedParams.resize(fmtTextHeader.paramCount);
		}

		for (auto i = 0u; i < fmtTextHeader.paramCount; ++i)
		{
			const auto& formatParamHeader = buffer.get<log::special::FormatParameter>();

			const auto result = decodeFormattedParam<log::special::FormattedParameterTypes>(
				buffer, formatParamHeader.typeId, formattedParams.at(i));

			switch (result)
			{
			case FORMATTED_TYPE_ID_NO_MATCH:
				return FORMATTED_TEXT_FAILED;
			}
		}

		server.handleText(topHeader,
						  static_cast<LogLevel>(fmtTextHeader.level),
						  fmtTextHeader.formatString,
						  formattedParams.cbegin(),
						  formattedParams.cbegin() + fmtTextHeader.paramCount);
		return 0;
	}

	bool isInSequence(protocol::SequenceNumber dataSeqNum) const
	{
		return seqNum == dataSeqNum;
	}

	template <typename ListOfTypes>
	std::size_t decodeFormattedParam(Buffer& buffer, uint8_t typeId, std::string& out)
	{
		if constexpr (IsEmptyV<ListOfTypes>)
		{
			assert(!"Could not find typeId in formatted parameter.");
			return FORMATTED_TYPE_ID_NO_MATCH;
		}
		else
		{
			using Type = GetHeadT<ListOfTypes>;

			if (IndexOfV<Type, log::special::FormattedParameterTypes> == typeId)
			{
				convertFormattedParam<Type>(buffer, out);
				return sizeof(Type);
			}
			else
			{
				return decodeFormattedParam<PopFrontT<ListOfTypes>>(buffer, typeId, out);
			}
		}
	}

	template <typename ParamType>
	void convertFormattedParam(Buffer& buffer, std::string& out)
	{
		const auto& value = buffer.get<ParamType>();
		formatted_text::serialize<ParamType>(out, value);
	}

private:
	Server& server;
	protocol::SequenceNumber seqNum{0};
	std::vector<std::string> formattedParams{};
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