#pragma once
#include <string>
#include <queue>

// IMPORTANT: DO NOT MODIFY THE CONTENTS OF THIS FILE.

enum TokenType
{
	TOKENTYPE_UNKNOWN,   // The token type has not been set.
	TOKENTYPE_NUMBER,    // A positive or negative 32-bit integer
	TOKENTYPE_OP_PLUS,	 // A + operator
	TOKENTYPE_OP_MINUS,	 // A - operator
	TOKENTYPE_OP_TIMES,	 // A * operator
	TOKENTYPE_OP_DIVIDE, // A / operator
	TOKENTYPE_PAR_OPEN,	 // An opening parenthesis: (
	TOKENTYPE_PAR_CLOSE	 // A closing parenthesis: )
};

struct Token
{
	TokenType Type;
	int Value;

	Token() : Type(TOKENTYPE_UNKNOWN), Value(0) {}
	Token(TokenType t, int v = 0) : Type(t), Value(v) {}
};


class IExpressionManager
{
public:
	IExpressionManager() {}
	virtual ~IExpressionManager(){}

	////////////////////////////////////////////////////////
	// Extracts an integer value from the given starting position in a string.
	// 
	// Parameters:
	//    str: The input string.
	//    pos: The position in the string where you want to start scanning.
	//
	// Notes:
	//   * Throw an exception if pos is greater than or equal to the string's
	//     length.
	//   * Throw an exception if the first character is not a digit.  You do 
	//     not have to support negative numbers.
	//   * As this method scans in the integer, it should increment the value of
	//     pos until pos no longer points to a digit character or pos has gone off
	//     the end of the string.
	virtual Token ScanInt(const std::string &str, size_t &pos) = 0;


	////////////////////////////////////////////////////////
	// Finds all numbers, operators, and parentheses in an input string, 
	// converts each one into an instance of a Token struct, and inserts it into 
	// a queue that you provide.
	//
	// Parameters:
	//   * str: The input string.
	//   * tokenQueue: A queue where you want your tokens stored.  You must 
	//     declare the queue yourself and pass it into this method.
	//
	// Notes:
	//   * This method does not validate syntax in any way.  It only converts a 
	//     string to a list of Token instances, and stores them in the queue 
	//     that you provide.
	virtual void TokenizeString(const std::string &str, std::queue<Token> &tokenQueue) = 0;


	////////////////////////////////////////////////////////
	// Checks whether or not the parentheses in an expression are balanced.  
	// Return true if expression is balanced, return false if not.  
	//
	// Parameters:
	//   * inputQueue: a queue containing a tokenized expression.
	//
	// Notes:
	//   * This method should ignore all other tokens besides TOKENTYPE_PAR_OPEN
	//     and TOKENTYPE_PAR_CLOSE.
	//   * This method does not have to preserve the contents of the input 
	//     queue.
    virtual bool IsBalanced(std::queue<Token> &inputQueue) = 0;


	////////////////////////////////////////////////////////
	// Takes an input-queue of tokens that are in infix order, and copies them 
	// to an output-queue in postfix order.
	//
	// Parameters:
	//   * inputQueue: A queue of tokens in infix order that you want converted 
	//     to postfix order.
	//   * outputQueue: An output-queue to store the tokens.
	//
	// Notes:
	//   * Throw an exception if the tokens in the input queue are not in proper 
	//     infix order (i.e., parentheses not balanced, unexpected token, etc.)
    virtual void InfixToPostfix(std::queue<Token> &inputQueue, std::queue<Token> &outputQueue) = 0;


	////////////////////////////////////////////////////////
	// Evaluates a postfix expression returns the final result as an integer.
	// 
	// Parameters:
	//   * inputQueue: A stream of tokens in postfix order.
	//
	// Parameters:
	//   * Throw an exception if the tokens in the input queue are not in proper 
	//     postfix order
	virtual int PostfixEvaluate(std::queue<Token> &inputQueue) = 0;
};


class ExpressionManagerFactory
{
public:
	static IExpressionManager *GetExpressionManager();
};
