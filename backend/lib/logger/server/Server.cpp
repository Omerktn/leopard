#include "Server.hpp"

#include <common/Sugars.hpp>
#include <common/Variant.hpp>

#include <logger/server/Utils.hpp>

#include <thread>

namespace leo::logger
{

Server::Server() = default;

Server::~Server() = default;

void Server::run()
{
	using namespace std::chrono_literals;

	std::cout << "Logger Server is starting" << std::endl;

	while (true)
	{
		auto& buffer = queue.takeBuffer();

		const auto hasAnyData = buffer.getReadableSize() > 0;

		if (hasAnyData)
		{
			std::cout << "[Server] Received " << buffer.getReadableSize() << " bytes:\n";
		}

		const auto result = decoder.decode(buffer);

		if (result != Decoder::DecodeResult::SUCCESS)
		{
			std::cout << "[Server] Decode unsuccessful = " << castToUnderlying(result) << "\n";
		}

		if (!hasAnyData)
		{
			std::this_thread::sleep_for(1ms);
		}
	}
}

concurrent::BufferQueue& Server::getQueue()
{
	return queue;
}

void Server::writeEventFields(const std::string_view eventName,
							  Nanoseconds timestamp,
							  const logger::Fields& fields,
							  std::ostream& out)
{
	out << "[ ";
	utils::serializeTime(out, timestamp);
	out << " ][ " << eventName << " ]";

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

}; // namespace leo::logger