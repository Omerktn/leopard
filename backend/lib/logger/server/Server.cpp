#include "Server.hpp"

#include <common/Sugars.hpp>
#include <common/Variant.hpp>

#include <logger/formattedText/Formatter.hpp>
#include <logger/server/Utils.hpp>

#include <cassert>
#include <thread>

namespace leo::logger
{

Server::User::User(UserId userId, const std::string& name, bool isFreeLogger)
	: id{userId}
	, name{name}
	, isFreeLogger{isFreeLogger}
	, queue{2048}
{}

Server::User::User(UserId userId, bool isFreeLogger)
	: id{userId}
	, name{std::nullopt}
	, isFreeLogger{isFreeLogger}
	, queue{2048}
{}

void Server::LineContainer::append(Nanoseconds timestamp, std::string&& text)
{
	auto line = Line{timestamp, std::move(text)};
	lines.insert(std::upper_bound(lines.begin(), lines.end(), line), std::move(line));
}

Server::LineContainer::ConstIterator Server::LineContainer::begin() const
{
	return lines.cbegin();
}

Server::LineContainer::ConstIterator Server::LineContainer::end() const
{
	return lines.cend();
}

bool Server::LineContainer::empty() const
{
	return lines.empty();
}

void Server::LineContainer::clear()
{
	lines.clear();

	static constexpr auto MAX_DESIRED_CAPACITY = 32'768u;
	if (lines.capacity() > MAX_DESIRED_CAPACITY)
	{
		lines.shrink_to_fit();
	}
}

Server::Server() = default;

Server::~Server() = default;

Server::UserId Server::registerUser(const std::string& name, bool isFreeLogger)
{
	std::scoped_lock guard{userMutex};

	const auto userId = nextUserId++;
	users.emplace(std::piecewise_construct,
				  std::forward_as_tuple(userId),
				  std::forward_as_tuple(userId, name, isFreeLogger));
	return userId;
}

void Server::unregisterUser(UserId userId)
{
	std::scoped_lock guard{userMutex};
	users.erase(userId);
}

concurrent::BufferQueue& Server::getUserQueue(UserId userId)
{
	const auto it = users.find(userId);
	if (it == users.end())
	{
		throw std::runtime_error("There is no user with id " + std::to_string(userId));
	}
	return it->second.queue;
}

void Server::flushLinesIfTimeOut(Nanoseconds currentTime)
{
	if (lineContainer.empty() || lineContainer.begin()->timestamp + LINES_BUFFER_TIME > currentTime)
	{
		return;
	}

	for (const auto& line : lineContainer)
	{
		std::cout << line.text << '\n';
	}
	std::cout << std::flush;

	lineContainer.clear();
}

void Server::run()
{
	using namespace std::chrono_literals;

	std::cout << "Logger Server is starting" << std::endl;

	while (true)
	{
		[&]() {
			std::scoped_lock guard{userMutex};

			Nanoseconds currentTime = Clock::now();

			for (auto& userEntry : users)
			{
				auto& logUser = userEntry.second;

				if (!logUser.hadDataLastTime &&
					currentTime - logUser.lastTimeRead < MIN_WAIT_BEFORE_READ)
				{
					continue;
				}

				auto& buffer = logUser.queue.takeBuffer();
				const auto hasData = buffer.getReadableSize() > 0;

				logUser.lastTimeRead = currentTime;
				logUser.hadDataLastTime = hasData;

				if (!hasData)
				{
					continue;
				}

				currentDecodedUser = &logUser;
				const auto result = decoder.decode(buffer);

				if (result != Decoder::DecodeResult::SUCCESS)
				{
					std::cout << "[Server] [" << logUser.name.value_or("Unnamed")
							  << "] Decode unsuccessful = " << castToUnderlying(result) << "\n";
				}
			}

			flushLinesIfTimeOut(currentTime);
		}();
	}
}

bool Server::checkAndSetSequenceNumber(protocol::SequenceNumber seqNum)
{
	assert(currentDecodedUser && "CurrentDecodedUser is not set.");

	const auto result = currentDecodedUser->sequenceNumber == seqNum;
	++currentDecodedUser->sequenceNumber;
	return result;
}

void Server::writeEventFields(const std::string_view eventName,
							  Nanoseconds timestamp,
							  const logger::Fields& fields,
							  std::ostream& out)
{
	const auto userName =
		currentDecodedUser ? currentDecodedUser->name.value_or("Unnamed") : "Unkown";

	out << "[ ";
	utils::serializeTime(out, timestamp);
	out << " ][ " << userName << " ][ " << eventName << " ]";

	for (const auto& field : fields)
	{
		const auto& fieldName = field.first;
		const auto& fieldVariant = field.second;

		out << "[ " << fieldName << " = ";

		std::visit(
			compose([&out](std::reference_wrapper<const std::string> arg) { out << arg.get(); },
					[&out](auto&& arg) { out << arg; }),
			fieldVariant);

		out << " ]";
	}
}

void Server::handleText(const protocol::Header& header,
						LogLevel level,
						const std::string_view formatStr,
						std::vector<std::string>::const_iterator paramsBegin,
						std::vector<std::string>::const_iterator paramsEnd)
{
	const auto numberOfParams = std::distance(paramsBegin, paramsEnd);
	const auto formatValidated = utils::validateFormatString(formatStr, numberOfParams);

	const auto timestamp = Nanoseconds{header.timestamp};

	const auto readyText = formatted_text::formatString(formatStr, paramsBegin, paramsEnd);

	std::ostringstream oss;
	writeText(level, timestamp, readyText, formatValidated, oss);

	lineContainer.append(timestamp, oss.str());
}

void Server::writeText(LogLevel level,
					   Nanoseconds timestamp,
					   const std::string_view text,
					   bool hasValidFormat,
					   std::ostream& out)
{
	const bool shouldWriteUserName = currentDecodedUser ? !currentDecodedUser->isFreeLogger : true;
	const auto userName =
		currentDecodedUser ? currentDecodedUser->name.value_or("Unnamed") : "Unknown";

	out << "[ ";
	utils::serializeTime(out, timestamp);
	out << " ]";

	if (shouldWriteUserName)
	{
		out << "[ " << userName << " ]";
	}

	out << "[ " << level << " ] ";

	if (!hasValidFormat)
	{
		out << " *MALFORMED* | ";
	}

	out << text;
}

}; // namespace leo::logger