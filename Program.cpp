// Program.cpp - (c) 2024 Jordi Scharp <www.moonpirates.com>

// Includes
#include "Program.h"
#include "Macros.h"
#include <iostream>
#include <fstream>
#include <random>

/// <summary>
/// Constructor
/// </summary>
Program::Program() : mGraph()
{
}

/// <summary>
/// Opens inFilename and tries to parse its contents to the Graph.
/// </summary>
/// <param name="inFilename">Path to file we're parsing</param>
/// <returns>If parsing was succesful</returns>
bool Program::FileToGraph(const std::string inFilename)
{
	// Open filestream
	std::ifstream file(inFilename);

	// Validate path
	if (!file.is_open())
	{
		LOG_ERROR("File '" << inFilename << "' could not be found!" << std::endl)
		return false;
	}

	// Parse file line by line
	std::string line;
	while (std::getline(file, line))
		ParseLine(line);

	return true;
}

/// <summary>
/// Fills the graph with random data, based on the amount of nodes the user wants. 
/// The amount of nodes is not guaranteed, as the connections are randomized, and due to its random nature may not select all nodes.
/// </summary>
/// <param name="inNumNodes">The amount of nodes the user wishes to have</param>
void Program::GenerateRandomGraph(const int inNumNodes)
{
	// Setup randomization system
	std::random_device randomDevice;
	std::default_random_engine randomEngine(randomDevice());

	// Calculate, given the amount of nodes, how many connections we can have at most.
	const int maxNumConnections = inNumNodes * (inNumNodes - 1);

	// Generate a number between a third of the max number of connections and the max number of connections.
	// If we have a maximum of 30 connections, we shall generate a number in the range [15..30]
	std::uniform_int_distribution<int> connectionsDistribution(maxNumConnections / 3, maxNumConnections);
	int numConnections = std::max(1, connectionsDistribution(randomEngine));

	std::cout << "For " << inNumNodes << " nodes, we'll generate " << numConnections << " connections:" << std::endl;

	// Set up a randomizer to pick random nodes.
	std::uniform_int_distribution<int> nodesDistribution(0, inNumNodes - 1);
	do
	{
		// Offset int by 65 to start at 'A'
		char from = (char)(nodesDistribution(randomEngine) + 65);
		char to = (char)(nodesDistribution(randomEngine) + 65);

		// Ignore if we randomly select ourselves as a node cannot connect to itself.
		if (from == to)
			continue;

		// Ensure that this connection does not exist before we try to add it.
		if (!mGraph[from][to])
		{
			AddConnection(from, to);
			numConnections--;
		}

	} while (numConnections > 0);
}

/// <summary>
/// Parses single line inLine of text to the graph.
/// </summary>
/// <param name="inLine">The line to parse</param>
/// <returns>If parsing was succesful</returns>
bool Program::ParseLine(const std::string inLine)
{
	// Validate string length
	if (inLine.length() != 4)
	{
		LOG_ERROR("Encountered an issue parsing '" << inLine << "'. A line should consist of 4 characters, format should be as follows: <letter><connection><letter>, ie: A->B or C<>D. Ignoring." << std::endl)
		return false;
	}

	char leftID = inLine[0];
	char rightID = inLine[3];

	// Ensure we're dealing with alphabetical characters
	if (!std::isalpha(leftID))
	{
		LOG_ERROR("Left hand node '" << leftID << "'is not an alphabetical character. Please use [A-Z]. Ignoring '" << inLine << "'." << std::endl)
		return false;
	}

	if (!std::isalpha(rightID))
	{
		LOG_ERROR("Right hand node '" << rightID << "'is not an alphabetical character. Please use [A-Z]. Ignoring '" << inLine << "'." << std::endl)
		return false;
	}

	// Ensure case insensitfity
	leftID = toupper(leftID);
	rightID = toupper(rightID);

	// Never allow connections to self
	if (leftID == rightID)
	{
		LOG_ERROR("Cannot connect to self. Ignoring '" << inLine << "'." << std::endl)
		return false;
	}

	// Parse the edge characters
	std::string connection = inLine.substr(1, 2);
	if (connection == "->")
	{
		AddConnection(leftID, rightID);
	}
	else if (connection == "<-")
	{
		AddConnection(rightID, leftID);
	}
	else if (connection == "<>")
	{
		AddConnection(leftID, rightID);
		AddConnection(rightID, leftID);
	}
	else
	{
		LOG_ERROR("Encountered issue parsing '" << connection << "'. Unknown connection type. Ignoring." << std::endl)
		return false;
	}

	return true;
}

/// <summary>
/// Removes all nodes which have inNumConnectionsForRemoval incoming connections.
/// </summary>
/// <param name="inNumConnectionsForRemoval">The amount of incoming connections on a node which should flag it for removal.</param>
void Program::CleanGraph(const int inNumConnectionsForRemoval)
{
	for (int i = 0; i < MAX_NODES; i++)
	{
		std::vector<int> incomingConnections;
		GetIncomingConnections(i, incomingConnections);
		if (incomingConnections.size() == inNumConnectionsForRemoval)
		{
			std::cout << TEXT_COLOR(32) << "Removing '" << IntToID(i) << "'." << TEXT_RESET() << std::endl;

			for (int j = 0; j < MAX_NODES; j++)
			{
				mGraph[j][i] = false;
				mGraph[i][j] = false;
			}
		}
	}
}

/// <summary>
/// Prints a user friendly version of the graph to the console.
/// </summary>
void Program::PrintGraph() const
{
	for (int from = 0; from < MAX_NODES; from++)
	{
		for (int to = 0; to < MAX_NODES; to++)
		{
			if (mGraph[from][to])
			{
				bool isBidirectional = mGraph[to][from];
				std::string connection = isBidirectional ? "<>" : "->";

				if (isBidirectional && from > to)
					continue;

				std::cout << TEXT_COLOR(31) << IntToID(from) << connection << IntToID(to) << TEXT_RESET() << std::endl;
			}
		}
	}
}

/// <summary>
/// Returns a vector of connection indices coming into the node with index inToIndex.
/// </summary>
/// <param name="inToIndex">Index of node we're interested in</param>
/// <param name="outConnections">Out paramter vector of incoming connections</param>
void Program::GetIncomingConnections(const int inToIndex, std::vector<int>& outConnections) const
{
	for (int from = 0; from < MAX_NODES; from++)
		if (mGraph[from][inToIndex])
			outConnections.push_back(from);
}

/// <summary>
/// Adds a connection from inFromID to inToID to the Graph.
/// </summary>
/// <param name="inFromID">ID of the Node the connection should be coming from</param>
/// <param name="inToID">ID of the Node the connection should be going to</param>
void Program::AddConnection(const char inFromID, const char inToID)
{
	int from = IDToInt(inFromID);
	int to = IDToInt(inToID);

	mGraph[from][to] = true;
}