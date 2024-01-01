#include "Server.hpp"

#include <common/Sugars.hpp>
#include <common/Variant.hpp>

#include <logger/formattedText/Formatter.hpp>
#include <logger/server/Utils.hpp>

#include <thread>

namespace leo::logger
{

Server::User::User(UserId userId, const std::string& name)
	: id{userId}
	, name{name}
	, queue{2048}
{}

Server::User::User(UserId userId)
	: id{userId}
	, name{std::nullopt}
	, queue{2048}
{}

Server::Server() = default;

Server::~Server() = default;

Server::UserId Server::registerUser(const std::string& name)
{
	std::scoped_lock guard{userMutex};

	const auto userId = nextUserId++;
	users.emplace(std::piecewise_construct,
				  std::forward_as_tuple(userId),
				  std::forward_as_tuple(userId, name));
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
		}();
	}
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

	out << '\n';
}

void Server::handleText(const protocol::Header& header,
						LogLevel level,
						const std::string_view formatStr,
						std::vector<std::string>::const_iterator paramsBegin,
						std::vector<std::string>::const_iterator paramsEnd)
{
	const auto finalText = formatted_text::formatString(formatStr, paramsBegin, paramsEnd);
	writeText(level, Nanoseconds{header.timestamp}, finalText, std::cout);
}

void Server::writeText(LogLevel level,
					   Nanoseconds timestamp,
					   const std::string_view text,
					   std::ostream& out)
{
	const auto userName =
		currentDecodedUser ? currentDecodedUser->name.value_or("Unnamed") : "Unkown";

	out << "[ ";
	utils::serializeTime(out, timestamp);
	out << " ][ " << userName << " ][ " << level << " ] " << text << " \n";
}

}; // namespace leo::logger