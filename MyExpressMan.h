//
//  MyExpressMan.h
//  10 - STL Expression Evaluation
//
//  Created by Andre Carrera on 11/29/16.
//  Copyright © 2016 Carrera. All rights reserved.
//

#ifndef MyExpressMan_h
#define MyExpressMan_h

#include <string>
#include <queue>
#include "ExpressionManager.h"//should this be here?



class MyExpressMan: public IExpressionManager
{
    
    

    
public:
    MyExpressMan() {}
    ~MyExpressMan(){}
    
    
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
    Token ScanInt(const std::string &str, size_t &pos);
    
    
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
    void TokenizeString(const std::string &str, std::queue<Token> &tokenQueue) ;
    
    
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
    bool IsBalanced(std::queue<Token> &inputQueue);
    
    
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
    void InfixToPostfix(std::queue<Token> &inputQueue, std::queue<Token> &outputQueue);
    
    
    ////////////////////////////////////////////////////////
    // Evaluates a postfix expression returns the final result as an integer.
    //
    // Parameters:
    //   * inputQueue: A stream of tokens in postfix order.
    //
    // Parameters:
    //   * Throw an exception if the tokens in the input queue are not in proper 
    //     postfix order
    int PostfixEvaluate(std::queue<Token> &inputQueue);
};

#endif /* MyExpressMan_h */
