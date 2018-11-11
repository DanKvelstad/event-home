#include "libtradfri.h"
#include <signal.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <atomic>
#include <map>
#include <functional>
#include <sstream>
#include <iterator>

std::atomic<bool> main_keep_alive(true);

int main(int argc, char *argv[])
{

	auto signal_handler(
		[](int signum)
		{
			std::cout << "Stopping because of " << strsignal(signum) << std::endl;
			main_keep_alive = false;
		}
	);

	struct sigaction signal_action = {};
	signal_action.sa_handler = signal_handler;
	sigaction(SIGINT, &signal_action, NULL);
	sigaction(SIGTERM, &signal_action, NULL);

	libtradfri tradfri;
	std::vector<std::string> command;
	command.reserve(32);

	for (int i = 1; i < argc; i++)
	{
		command.push_back(argv[i]);
	}

    std::map<
        std::string, 
        std::function<
			void(
				decltype(command)::const_iterator, 
				decltype(command)::const_iterator
			)
		>
    > commands 
	{
		std::make_pair(
    	    "help",
    	    [&](decltype(command)::const_iterator begin, decltype(command)::const_iterator end)
    	    {
    	        std::for_each(
					commands.begin(), commands.end(),
					[](decltype(commands)::const_reference element)
					{
						std::cout << "    " << element.first << '\n';
					}
				);
    	    }
    	),
    	std::make_pair(
    	    "speak",
    	    [&](decltype(command)::const_iterator begin, decltype(command)::const_iterator end)
    	    {
    	        if(begin == end)
    	        {
    	            tradfri.speak("    please provide args");
    	        }
    	        else
    	        {
					std::for_each(
						begin, end, 
						std::bind(&libtradfri::speak, &tradfri, std::placeholders::_1)
					);
    	        }
    	    }
    	),
    	std::make_pair(
    	    "exit",
    	    [&](decltype(command)::const_iterator begin, decltype(command)::const_iterator end)
    	    {
    	        main_keep_alive = false;
    	    }
    	)
	};

	auto interpreter(
		[](
			const decltype(commands)&         commands,
			decltype(command)::const_iterator begin, 
			decltype(command)::const_iterator end
		)
		{
		    auto command(commands.find(*begin));
		    if(commands.end() == command)
		    {
		        std::cout << "    faulty command: " << *begin << "\n";
		    }
		    else if(begin != end)
		    {
		        command->second(++begin, end);
		    }
			else
			{
		        command->second(begin, end);
			}
		}
	);

	if (!command.empty())
    {
        interpreter(commands, command.cbegin(), command.cend());
    }
    else
    {

        std::cout   << "\n"
                    << "IKEA Tradfri Console\n"
                    << "please enter a command e.g. help\n"; 

        try
        {
            std::string line;
            while(main_keep_alive)
            {
				std::cout << "> ";
                if(std::getline(std::cin, line).good())
                {
					std::stringstream ss(line);
					std::istream_iterator<std::string> begin(ss);
					std::istream_iterator<std::string> end;
					std::copy(begin, end, std::back_inserter(command));
                    interpreter(commands, command.cbegin(), command.cend());
                }
				command.clear();
            }
        }
        catch(const std::system_error& runtime_error)
        {
        }

    }

	std::cout.flush();
	
	return EXIT_SUCCESS;

}