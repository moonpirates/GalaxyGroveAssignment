// Program.h - (c) 2024 Jordi Scharp <www.moonpirates.com>
#pragma once

// Includes
#include <string> 
#include <vector> 

/// <summary>
/// This class holds a single graph and provides the means to the user to modify its relations.
/// </summary>
class Program
{
public:
						Program();																				///< Constructor

	bool				FileToGraph(const std::string inFilename);												///< Opens inFilename and tries to parse its contents to the Graph.
	void				GenerateRandomGraph(const int inNumNodes);												///< Fills the graph with random data, based on the amount of nodes the user wants. 
	bool				ParseLine(const std::string inLine);													///< Parses single line inLine of text to the graph.
	void				CleanGraph(const int inNumConnectionsForRemoval);										///< Removes all nodes which have inNumConnectionsForRemoval incoming connections.
	void				PrintGraph() const;																		///< Prints a user friendly version of the graph to the console.
	void				GetIncomingConnections(const int inToIndex, std::vector<int>& outConnections) const;	///< Returns a vector of connection indices coming into the node with index inToIndex.
	void				AddConnection(const char inFromID, const char inToID);									///< Adds a connection from inFromID to inToID to the Graph.

private:
	static int			IDToInt(const char inID) { return (int)(inID) - 65; }									///< Converts read friendly ID to integer.
	static char			IntToID(const int inInt) { return (char)(inInt) + 65; }									///< Converts integer to read friendly ID.

	static const int	MAX_NODES = 26;																			///< The vertical and horizontal sizes of our matrix.
	
	bool				mGraph[MAX_NODES][MAX_NODES];															///< Our graph holding all connections, stored in a [from][to] manner.
};