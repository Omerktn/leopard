#pragma once

#include <logger/user/Logger.hpp>

namespace leo
{
class LeoMain;
}

namespace leo::logger
{

class FreeLogger
{
public:
	static logger::Logger& getLogger()
	{
		return get().underlyingLogger;
	}

private:
	template <typename... Args>
	FreeLogger(Args&&... args)
		: underlyingLogger{std::forward<Args>(args)...}
	{}

	friend LeoMain; // Used this pattern to prevent everybody else initializing this singleton
	static void initialize(Server& server);

	static FreeLogger& get()
	{
		return *singleton;
	}

private:
	logger::Logger underlyingLogger;

	static FreeLogger* singleton;
};

} // namespace leo::logger

#define LOG_ERROR(...) leo::logger::FreeLogger::getLogger().logInfo(__VA_ARGS__);
#define LOG_WARN(...) leo::logger::FreeLogger::getLogger().logWarn(__VA_ARGS__);
#define LOG_INFO(...) leo::logger::FreeLogger::getLogger().logInfo(__VA_ARGS__);
#define LOG_DEBUG(...) leo::logger::FreeLogger::getLogger().logDebug(__VA_ARGS__);
#define FLUSH_FREE_LOGGER() leo::logger::FreeLogger::getLogger().flush();