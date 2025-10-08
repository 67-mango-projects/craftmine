	
#include <spdlog.h>
#include <sinks/stdout_color_sinks.h>
#include <sinks/basic_file_sink.h>
#include "game.h"



/*
* MY COOL GAME FOR DIGI TECH, NO GAME ENGINE OR GAME LIBRARY ONLY OPENGL LIBRARIES
* not gonna add much comments since it takes too long, and most of the time the code is self explanatory
* or is too complicated to fit into a comment
*/



// NOT MINE MY FRIEND IMPLEMENTED THIS COOL LOGGING SYSTEM FOR ME
/**
 * Set up the global logger using spdlog.
 * Logs will be output to both the console and a file named "app.log".
 */
void setup_logger() {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("app.log", true);

	auto logger = std::make_shared<spdlog::logger>("global", spdlog::sinks_init_list{console_sink, file_sink});

	// Sets default global logger config
	spdlog::set_default_logger(logger);

	// Debug configuration
#if defined(_DEBUG)
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	// Log format configuration
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

	// Ensure every log message is flushed to disk immediately.
	// Flush on the 'trace' level forces a flush after every log call because 'trace' is the lowest severity.
	// logger->flush_on(spdlog::level::trace);

	// Also set the global flush level to 'trace' to ensure all loggers flush immediately.
	spdlog::flush_on(spdlog::level::trace);

	// Keep an explicit reference to sinks (optional): logger holds shared_ptrs to sinks, so they will remain alive.
	// English comments above explain the immediate-flush behavior.
}

/**
 * Enable virtual terminal processing for ANSI escape codes in the Windows console.
 */
void enable_virtual_terminal_processing() {
	// Function to enable virtual terminal processing for ANSI escape codes
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // Get the standard output handle
	DWORD dwMode = 0; // Variable to hold the console mode
	if (GetConsoleMode(hOut, &dwMode)) { // Check if we can get the console mode
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // Enable virtual terminal processing
		SetConsoleMode(hOut, dwMode); // Set the console mode with the new settings
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	enable_virtual_terminal_processing();
	setup_logger();

	Game a;
	return a.run();
 }
