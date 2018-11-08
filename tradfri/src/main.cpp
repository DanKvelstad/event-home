#include "tradfri.h"
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

std::atomic<bool> keep_alive(true);

int main(int argc, char *argv[])
{

	tradfri library;

	auto signal_handler(
		[](int signum)
		{
			std::cout << "Stopping because of " << strsignal(signum) << std::endl;
			keep_alive = false;
		}
	);

	struct sigaction signal_action = {};
	signal_action.sa_handler = signal_handler;
	sigaction(SIGINT, &signal_action, NULL);
	sigaction(SIGTERM, &signal_action, NULL);

	std::vector<std::string> args;
	for (int i = 1; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

    std::map<
        std::string, 
        std::function<void(std::vector<std::string>)>
    > commands 
	{
		std::make_pair(
    	    "help",
    	    [](std::vector<std::string> args)
    	    {
    	        std::cout << "there is no help\n";
    	    }
    	),
    	std::make_pair(
    	    "speak",
    	    [&](std::vector<std::string> args)
    	    {
    	        if(args.empty())
    	        {
    	            library.speak("no args");
    	        }
    	        else
    	        {
    	            library.speak(args.at(0));
    	        }
    	    }
    	)
	};

	auto interpret(
		[&](std::vector<std::string> line)
		{
		    auto command(commands.find(line.at(0)));
		    if(commands.end()==command)
		    {
		        std::cout << "faulty command: " << line.at(0) << "\n";
		    }
		    else
		    {
		        command->second(
		            std::vector<std::string>(
		                ++line.begin(), line.end()
		            )
		        );
		    }
		}
	);

	if (!args.empty())
    {
        interpret(args);
    }
    else
    {

        std::cout   << "\n"
                    << "IKEA Tradfri Console\n"
                    << "please enter a command e.g. help\n"; 

        try
        {
            std::string line;
            while(keep_alive)
            {
				std::cout << "> ";
                if(std::getline(std::cin, line).good())
                {
					std::stringstream ss(line);
					std::istream_iterator<std::string> begin(ss);
					std::istream_iterator<std::string> end;
                    interpret(std::vector<std::string>(begin, end));
                }
            }
        }
        catch(const std::system_error& runtime_error)
        {
        }

    }

	std::cout.flush();
	
	return EXIT_SUCCESS;

}