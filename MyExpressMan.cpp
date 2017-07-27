//
//  MyExpressMan.cpp
//  10 - STL Expression Evaluation
//
//  Created by Andre Carrera on 11/29/16.
//  Copyright © 2016 Carrera. All rights reserved.
//

#include "MyExpressMan.h"
#include <exception>
#include <ctype.h>
#include <stack>
#include <string>
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
Token MyExpressMan::ScanInt(const std::string &str, size_t &pos)
{
    int answer = 0;
    if (pos>= str.length()|| isdigit(str[pos])==false) {//maybe only check first for digit
        throw std::out_of_range ("Index out of bounds.");
    }
    
    while (str[pos]>= '0' && str[pos] <= '9') {
        answer =answer * 10 + str[pos] - '0';
        pos = pos +1;
    }
    
    Token intok;
    intok.Type = TOKENTYPE_NUMBER;
    intok.Value = answer;
    return intok;
    
}


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
void MyExpressMan::TokenizeString(const std::string &str, std::queue<Token> &tokenQueue)
{
    
    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit(str[i])==true) {
            Token token(TOKENTYPE_NUMBER);
            token = ScanInt(str, i);
            tokenQueue.push(token);
            i--;
    
        }
        else if (str[i] == '+') {
            Token token(TOKENTYPE_OP_PLUS);
            tokenQueue.push(token);
        }
        else if (str[i] == '-') {
            Token token(TOKENTYPE_OP_MINUS);
            tokenQueue.push(token);
        }
        else if (str[i] == '*') {
            Token token(TOKENTYPE_OP_TIMES);
            tokenQueue.push(token);
        }
        else if (str[i] == '(') {
            Token token(TOKENTYPE_PAR_OPEN);
            tokenQueue.push(token);
        }
        else if (str[i] == ')') {
            Token token(TOKENTYPE_PAR_CLOSE);
            tokenQueue.push(token);
        }
        else if (str[i] == '/') {
            Token token(TOKENTYPE_OP_DIVIDE);
            tokenQueue.push(token);
        }
        else if(iswspace(str[i])==true){
            
        }
        else{
            throw std::out_of_range ("Index out of bounds.");
        }
        
    }

    
}


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
bool MyExpressMan::IsBalanced(std::queue<Token> &inputQueue)
{
//figure out size of queue
    
    
    
    std::queue<Token> newQueue;
    
    
    while(inputQueue.size()> 0) {
        Token t = inputQueue.front();
        if (t.Type == TOKENTYPE_PAR_OPEN){
            //push onto the stack
            Token token(TOKENTYPE_PAR_OPEN);
            newQueue.push(token);
        }
        else if (t.Type == TOKENTYPE_PAR_CLOSE){
            if (newQueue.size()==0) {
                return false;
            }
            else{
                newQueue.pop();
            }
        }
        inputQueue.pop();
    }
    if (newQueue.size()==0) {
        return true;
    }
    else return false;
}


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



void MyExpressMan::InfixToPostfix(std::queue<Token> &inputQueue, std::queue<Token> &outputQueue)
{
    std::stack <Token> operationStack;
    
    while (inputQueue.size()!= 0) {
        Token t  = inputQueue.front();
        
        inputQueue.pop();
        
        if (t.Type == TOKENTYPE_NUMBER) {
            outputQueue.push(t);
        }
        
        else if (t.Type == TOKENTYPE_OP_PLUS ||t.Type == TOKENTYPE_OP_MINUS || t.Type == TOKENTYPE_OP_TIMES || t.Type == TOKENTYPE_OP_DIVIDE){
            while (operationStack.size()!= 0) {//make sure it's not empty
                Token op = operationStack.top();
                if (op.Type == TOKENTYPE_PAR_OPEN) {
                    break;
                }
                int tPres = 0;
                int opPres = 0;
                
                if (t.Type == TOKENTYPE_OP_DIVIDE || t.Type == TOKENTYPE_OP_TIMES) {
                    tPres = 2;
                }
                else if (t.Type == TOKENTYPE_OP_MINUS || t.Type == TOKENTYPE_OP_PLUS) {
                    tPres = 1;
                }
                if (op.Type == TOKENTYPE_OP_DIVIDE || op.Type == TOKENTYPE_OP_TIMES) {
                    opPres = 2;
                }
                else if (op.Type == TOKENTYPE_OP_MINUS || op.Type == TOKENTYPE_OP_PLUS) {
                    opPres = 1;
                }
                
                if (tPres> opPres) {
                    break;
                }
                operationStack.pop();
                outputQueue.push(op);
                
            }
            operationStack.push(t);
        }
        else if (t.Type == TOKENTYPE_PAR_OPEN){
            operationStack.push(t);
        }
        else if (t.Type == TOKENTYPE_PAR_CLOSE){

            while (true) {
                if (operationStack.size()==0) {
                    throw std::out_of_range ("Mismatched parentheses");
                }
                Token op  = operationStack.top();
                operationStack.pop();
                if (op.Type == TOKENTYPE_PAR_OPEN) {
                    break;
                }
                outputQueue.push(op);
                
            }
        }
        
    }
    while (operationStack.size()!= 0) {
        Token op  = operationStack.top();
        operationStack.pop();
        if (op.Type == TOKENTYPE_PAR_OPEN) {
            throw std::out_of_range ("Mismatched parentheses");
        }
        outputQueue.push(op);
    }
}


////////////////////////////////////////////////////////
// Evaluates a postfix expression returns the final result as an integer.
//
// Parameters:
//   * inputQueue: A stream of tokens in postfix order.
//
// Parameters:
//   * Throw an exception if the tokens in the input queue are not in proper
//     postfix order
int MyExpressMan::PostfixEvaluate(std::queue<Token> &inputQueue)
{

    std::stack <Token> evaluationStack;
    
    while (inputQueue.size() != 0) {
        
        
        
        Token t  = inputQueue.front();
        inputQueue.pop();
        
        if (t.Type == TOKENTYPE_NUMBER) {//or variable?
            evaluationStack.push(t);
        }
        else if (t.Type == TOKENTYPE_OP_PLUS ||t.Type == TOKENTYPE_OP_MINUS || t.Type == TOKENTYPE_OP_TIMES || t.Type == TOKENTYPE_OP_DIVIDE) {
            if (evaluationStack.size() < 2) {
                throw std::out_of_range ("Too many operators.");
            }
            Token right = evaluationStack.top();
            evaluationStack.pop();
            Token left = evaluationStack.top();
            evaluationStack.pop();
            
            int result = 0;
            if (t.Type == TOKENTYPE_OP_PLUS) {
                result =  left.Value + right.Value;
            }
            else if (t.Type == TOKENTYPE_OP_MINUS) {
                result = left.Value - right.Value;
            }
            else if (t.Type == TOKENTYPE_OP_TIMES) {
                result =  left.Value * right.Value;
            }
            else if (t.Type == TOKENTYPE_OP_DIVIDE) {
                result = left.Value / right.Value;
            }
            
            Token intok;
            intok.Type = TOKENTYPE_NUMBER;
            intok.Value = result;
            
            evaluationStack.push(intok);
        }
        else throw std::out_of_range ("Invalid token.");
    }
    if (evaluationStack.size() > 1) {
        throw std::out_of_range ("missing an operator");
    }
    Token finalResult = evaluationStack.top();
    evaluationStack.pop();
    return finalResult.Value;

}
