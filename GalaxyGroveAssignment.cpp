// GalaxyGroveAssignment.cpp - (c) 2024 Jordi Scharp <www.moonpirates.com>

// Includes
#include "Program.h"
#include "Macros.h"
#include <iostream>

// Forward declares
bool ShowAddConnectionMenu(Program* ioProgram);

/// <summary>
/// Clears the console, directly copy/pasted from https://stackoverflow.com/a/52895729
/// </summary>
void ClearConsole()
{
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}

/// <summary>
/// Tries to convert a string to an integer which is greater than or equal to 0.
/// If a problem occurs, it will be printed to cout.
/// </summary>
/// <param name="inString">The incoming string value.</param>
/// <param name="outValue">The outgoing converted value.</param>
/// <returns>Whether conversion was successfull.</returns>
bool StringToAbsInt(const std::string& inString, int& outValue)
{
	// std::stoi throws exceptions when problems occur, hence the try/catch.
	try
	{
		outValue = std::stoi(inString);

		// Ensure we're absolute
		if (outValue < 0)
		{
			LOG_ERROR("Please use a number >= 0" << std::endl)
			return false;
		}
	}
	catch (std::invalid_argument&)
	{
		LOG_ERROR("Invalid input, please use a number." << std::endl)
		return false;
	}
	catch (std::out_of_range&)
	{
		LOG_ERROR("Number out of range, please retry." << std::endl)
		return false;
	}

	return true;
}

/// <summary>
/// User facing method to allow the user to clean the graph.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowCleanGraphMenu(Program* ioProgram)
{
	ClearConsole();
	std::cout << "Next we will clean the graph from all nodes which have an N amount of connections coming in." << std::endl;

	while (true)
	{
		std::cout << "Which amount of incoming connections should be considered fatal? (default is 3)" << std::endl;
		std::cout << "p to print graph." << std::endl;
		std::cout << ">> ";
		
		// Read input
		std::string n;
		std::getline(std::cin, n);

		// Validate and handle input
		int nInt;
		if (n == "p")
		{
			ioProgram->PrintGraph();
			continue;
		}
		else if (n.empty())
		{
	 		nInt = 3;
		}
		else if (!StringToAbsInt(n, nInt))
		{
			continue;
		}
		
		ClearConsole();
		std::cout << "==================================================================================" << std::endl;
		std::cout << "Cleaning graph from nodes which have " << nInt << " incoming connection(s)." << std::endl;
		std::cout << "==================================================================================" << std::endl;
		
		// Clean graph, based on the user's input
		ioProgram->CleanGraph(nInt);
		break;
	}

	// Show the end results
	std::cout << "==================================================================================" << std::endl;
	std::cout << "Result:" << std::endl;
	std::cout << "==================================================================================" << std::endl;
	ioProgram->PrintGraph();
	std::cout << "==================================================================================" << std::endl;

	// Post clean-up menu
	while (true)
	{
		std::cout << "Please select one of the following options:" << std::endl;
		std::cout << " 1. Add connections" << std::endl;
		std::cout << " 2. Clean again" << std::endl;
		std::cout << " 3. Restart" << std::endl;
		std::cout << " 4. Quit" << std::endl;
		std::cout << ">> ";

		// Read input
		std::string choice;
		std::getline(std::cin, choice);

		// Validate and handle input
		if (choice == "1")
			return ShowAddConnectionMenu(ioProgram);
		if (choice == "2")
			return ShowCleanGraphMenu(ioProgram);
		else if (choice == "3")
			return true;
		else if (choice == "4")
			return false;
		else
			LOG_ERROR(std::endl << "Invalid option." << std::endl << std::endl)
	}
}

/// <summary>
/// User facing method to allow them to manually add connections.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowAddConnectionMenu(Program* ioProgram)
{
	ClearConsole();

	std::cout << "Do you want to manually add any connections?" << std::endl;
	std::cout << "These need to be a single alphabetical character, followed by a connection, followed by another alphabetical character." << std::endl;
	std::cout << "Connections can be either <- or -> (unidirectional) or <> (birectional)." << std::endl;
	std::cout << "For example: A<-B, C->F, G<>A" << std::endl << std::endl;
	std::cout << "If you're done, press enter." << std::endl;
	std::cout << "p to print graph." << std::endl;

	while (true)
	{
		std::cout << ">> ";

		// Read input
		std::string input;
		std::getline(std::cin, input);

		// Validate and handle input
		if (input.empty())
			return ShowCleanGraphMenu(ioProgram);
		if (input == "p")
			ioProgram->PrintGraph();
		else
			ioProgram->ParseLine(input);
	}
}

/// <summary>
/// User facing method to either add connections or jump to cleaning up the graph.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowPostDataMenu(Program* ioProgram)
{
	while (true)
	{
		std::cout << "Please select one of the following options:" << std::endl;
		std::cout << " 1. Add connections" << std::endl;
		std::cout << " 2. Clean graph" << std::endl;
		std::cout << ">> ";

		// Read input
		std::string choice;
		std::getline(std::cin, choice);

		// Validate and handle input
		if (choice == "1")
			return ShowAddConnectionMenu(ioProgram);
		else if (choice == "2")
			return ShowCleanGraphMenu(ioProgram);
		else
			LOG_ERROR(std::endl << "Invalid option." << std::endl << std::endl)
	}
}

/// <summary>
/// User facing method to allow them to parse data from a text file.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowParseFileMenu(Program* ioProgram)
{
	ClearConsole();

	while (true)
	{
		std::cout << "Please enter the filename/path (default: ./Graph.txt): " << std::endl;
		std::cout << ">> ";

		// Read input
		std::string filename;
		std::getline(std::cin, filename);

		// Default handling on enter
		if (filename.empty())
			filename = "Graph.txt";

		std::cout << "==================================================================================" << std::endl;
		std::cout << "Start reading from file: '" << filename << "'" << std::endl;
		std::cout << "==================================================================================" << std::endl;

		// Validate and handle input
		if (ioProgram->FileToGraph(filename))
			break;
	}
	
	// Show parsed reults
	std::cout << "Result:" << std::endl;
	std::cout << "==================================================================================" << std::endl;
	ioProgram->PrintGraph();
	std::cout << "==================================================================================" << std::endl;

	return ShowPostDataMenu(ioProgram);
}

/// <summary>
/// User facing method to allow them to generate a graph at random.
/// It asks the user for how many nodes they'd like, and from this we'll generate a random number of connections.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowRandomDataMenu(Program* ioProgram)
{
	ClearConsole();

	while (true)
	{
		std::cout << "How many random nodes would you like to see? (min 2, max 26)" << std::endl;
		std::cout << ">> ";

		// Read input
		int numNodes;
		std::string numNodesInput;
		std::getline(std::cin, numNodesInput);

		// Validate input
		if (!StringToAbsInt(numNodesInput, numNodes))
		{
			continue;
		}
		else if (numNodes < 2)
		{
			LOG_ERROR("We need at least 2 nodes, please retry." << std::endl)
			continue;
		}
		else if (numNodes > 26)
		{
			LOG_ERROR("There is a max of 26 nodes, please retry." << std::endl)
			continue;
		}

		// Generate our random graph and print the results
		ioProgram->GenerateRandomGraph(numNodes);
		ioProgram->PrintGraph();

		return ShowPostDataMenu(ioProgram);
	}
}

/// <summary>
/// User facing method to welcome them and give the first few choices.
/// </summary>
/// <param name="ioProgram">The Program instance we are currently working with.</param>
/// <returns>Whether we want to continue the application.</returns>
bool ShowMainMenu()
{
	Program program;

	ClearConsole();

	std::cout << TEXT_COLOR(33) << "Hi Galaxy Grove! " << TEXT_RESET() << "Welcome to my code assignment!" << std::endl;
	std::cout << "I hope you enjoy it as much as I had making it :)" << std::endl << std::endl;
	while (true)
	{
		std::cout << "Please select one of the following options:" << std::endl;
		std::cout << " 1. Parse graph from file" << std::endl;
		std::cout << " 2. Randomly generate graph" << std::endl;
		std::cout << " 3. Quit" << std::endl;
		std::cout << ">> ";
		
		// Read input
		std::string choice;
		std::getline(std::cin, choice);

		// Validate and handle input
		if (choice == "1")
			return ShowParseFileMenu(&program);
		else if (choice == "2")
			return ShowRandomDataMenu(&program);
		else if (choice == "3")
			return false;
		else
			LOG_ERROR(std::endl << "Invalid option." << std::endl << std::endl)
	}
}

int main()
{
	while (ShowMainMenu()) 
	{
	}

	std::cout << std::endl << TEXT_COLOR(31) << "g " << TEXT_COLOR(33) << "o " << TEXT_COLOR(32) << "o " << TEXT_COLOR(36) << "d " << TEXT_COLOR(34) << "b " << TEXT_COLOR(35) << "y " << TEXT_COLOR(31) << "e " << TEXT_RESET() << std::endl;
}