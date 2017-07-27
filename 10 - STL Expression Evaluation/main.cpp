#include <string>
#include <iostream>
#include <sstream>


using namespace std;

// Include project header files
#include "TestCase.h"
#include "ExpressionManager.h"
#include "MyExpressMan.h"

void JoinTokens(queue<Token> &tokens, string &outputStr)
{
	bool first = true;
	stringstream ss;
	while (tokens.size() > 0)
	{
		Token t = tokens.front();
		tokens.pop();

		if (!first)
			ss << " ";
		first = false;

		if (t.Type == TOKENTYPE_NUMBER)
			ss << t.Value;
		else if (t.Type == TOKENTYPE_OP_PLUS)
			ss << "+";
		else if (t.Type == TOKENTYPE_OP_MINUS)
			ss << "-";
		else if (t.Type == TOKENTYPE_OP_TIMES)
			ss << "*";
		else if (t.Type == TOKENTYPE_OP_DIVIDE)
			ss << "/";
		else if (t.Type == TOKENTYPE_PAR_OPEN)
			ss << "(";
		else if (t.Type == TOKENTYPE_PAR_CLOSE)
			ss << ")";
	}

	outputStr = ss.str();
}

void TestScanInt(bool expectPass, char *expr, size_t pos, int expectedValue, size_t expectedEndPos)
{
	stringstream ss;
	ss << "Scanning \"" << expr << "\" at position " << pos << "\"";
	TestCase tc(ss.str().c_str());

	IExpressionManager *expMgr = NULL;
	try
	{
		expMgr = ExpressionManagerFactory::GetExpressionManager();

		Token t = expMgr->ScanInt(expr, pos);
		if (!expectPass)
		{
			tc.LogResult(RESULT_FAILED, "This should have failed and thrown an exception.");
			return;
		}

		tc.AssertEquals(expectedValue, t.Value, "Make sure you parsed it correctly.");
		tc.AssertEquals((int)expectedEndPos, (int)pos, "Make sure pos points to the right index.");
	}
	catch (exception& ex)
	{
		if (!expectPass)
		{
			tc.LogResult(RESULT_PASSED, "Parse failed as expected.");
			tc.LogResult(RESULT_PASSED, ex.what());
		}
		else
			tc.LogException(ex);
	}

	if (expMgr != NULL)
		delete expMgr;
}

void TestTokenizeString(bool expectPass, char *inputStr, char *expectedStr)
{
	stringstream ss;
	ss << "Tokenizing string \"" << inputStr << "\".";
	TestCase tc(ss.str().c_str());

	IExpressionManager *expMgr = NULL;
	try
	{
		expMgr = ExpressionManagerFactory::GetExpressionManager();

		queue<Token> output;
		expMgr->TokenizeString(inputStr, output);
		if (!expectPass)
		{
			tc.LogResult(RESULT_FAILED, "This should have failed and thrown an exception.");
			return;
		}

		string outputStr;
		JoinTokens(output, outputStr);
		tc.AssertEquals(expectedStr, outputStr, "Verify tokens.");
	}
	catch (exception& ex)
	{
		if (!expectPass)
		{
			tc.LogResult(RESULT_PASSED, "Parse failed as expected.");
			tc.LogResult(RESULT_PASSED, ex.what());
		}
		else
			tc.LogException(ex);
	}

	if (expMgr != NULL)
		delete expMgr;
}

void TestIsBalanced(bool expectPass, char *expr)
{
	stringstream ss;
	ss << "Verifying expression \"" << expr << "\"";
    TestCase tc(ss.str().c_str());

	IExpressionManager *expMgr = NULL;
	try
    {
		expMgr = ExpressionManagerFactory::GetExpressionManager();
		queue<Token> inputQueue;
		expMgr->TokenizeString(expr, inputQueue);

        bool isBalanced = expMgr->IsBalanced(inputQueue);
		tc.AssertEquals(expectPass, isBalanced, "Check IsBalanced return value.");
    }
    catch (exception& ex)
    {
        tc.LogException(ex);
    }

	if (expMgr != NULL)
		delete expMgr;
}

void TestInfixToPostfix(bool expectPass, char *inputStr, char *expectedStr)
{
	stringstream ss;
	ss << "Verifying expression \"" << inputStr << "\"";
	TestCase tc(ss.str().c_str());

	IExpressionManager *expMgr = NULL;
	try
	{
		expMgr = ExpressionManagerFactory::GetExpressionManager();

		queue<Token> input, output;
		expMgr->TokenizeString(inputStr, input);

		expMgr->InfixToPostfix(input, output);
		if (!expectPass)
		{
			tc.LogResult(RESULT_FAILED, "This should have failed and thrown an exception.");
			return;
		}

		string outputStr;
		JoinTokens(output, outputStr);
		tc.AssertEquals(expectedStr, outputStr, "Verify tokens.");
	}
	catch (exception& ex)
	{
		if (!expectPass)
		{
			tc.LogResult(RESULT_PASSED, "Parse failed as expected.");
			tc.LogResult(RESULT_PASSED, ex.what());
		}
		else
			tc.LogException(ex);
	}

	if (expMgr != NULL)
		delete expMgr;
}

void TestPostfixEvaluate(bool expectPass, char *inputStr, int expected)
{
	stringstream ss;
	ss << "Evaulate expression \"" << inputStr << "\"";
    TestCase tc(ss.str().c_str());

	IExpressionManager *expMgr = NULL;
	try
	{
		expMgr = ExpressionManagerFactory::GetExpressionManager();

		queue<Token> postfixTokens;
		expMgr->TokenizeString(inputStr, postfixTokens);

		int result = expMgr->PostfixEvaluate(postfixTokens);
		if (!expectPass)
		{
			tc.LogResult(RESULT_FAILED, "This should have failed and thrown an exception.");
			return;
		}

		tc.AssertEquals(expected, result, "Verify the correct result.");
    }
    catch (exception& ex)
    {
		if (!expectPass)
		{
			tc.LogResult(RESULT_PASSED, "Parse failed as expected.");
			tc.LogResult(RESULT_PASSED, ex.what());
		}
		else
			tc.LogException(ex);
	}

	if (expMgr != NULL)
		delete expMgr;
}

int main()
{
	/*******************************************************
	 *** Test ScanInt
	 *******************************************************/
	TestCase::PrintBanner("Test ScanInt");

	// All these should pass.
	TestScanInt(true, "0", 0, 0, 1);
	TestScanInt(true, "1", 0, 1, 1);
	TestScanInt(true, "10", 0, 10, 2);
	TestScanInt(true, "100", 0, 100, 3);
	TestScanInt(true, "1000", 0, 1000, 4);
	TestScanInt(true, "321", 0, 321, 3);
	TestScanInt(true, "123456789", 0, 123456789, 9);
	TestScanInt(true, "987654321", 0, 987654321, 9);
	TestScanInt(true, "(987 + 65 / 4321)", 1, 987, 4);
	TestScanInt(true, "(987 + 65 / 4321)", 7, 65, 9);
	TestScanInt(true, "(987 + 65 / 4321)", 12, 4321, 16);

	// All these should fail.
	TestScanInt(false, "", 0, 0, 0);
	TestScanInt(false, "Aaa", 0, 0, 0);
	TestScanInt(false, "123", 3, 0, 0);
	TestScanInt(false, "123", 10, 0, 0);


	/*******************************************************
	 *** Test TokenizeString
	 *******************************************************/
	TestCase::PrintBanner("Test TokenizeString");

	// All these should pass.
	TestTokenizeString(true, "1234", "1234");
	TestTokenizeString(true, "+", "+");
	TestTokenizeString(true, "-", "-");
	TestTokenizeString(true, "*", "*");
	TestTokenizeString(true, "/", "/");
	TestTokenizeString(true, "(", "(");
	TestTokenizeString(true, ")", ")");
	TestTokenizeString(true, " 1234 + - * / (  ) ", "1234 + - * / ( )");
	TestTokenizeString(true, "(987+65)/4321", "( 987 + 65 ) / 4321");

	// Make sure you successfully handle an empty string.
	TestTokenizeString(true, "", "");

	// All these should fail.
	TestTokenizeString(false, "1 + 2 = 3", "");
	TestTokenizeString(false, "10 * x", "");


	/*******************************************************
	 *** Test for balanced parentheses
	 *******************************************************/
	TestCase::PrintBanner("Test for balanced parentheses");

	// All these are balanced
    TestIsBalanced(true, "1 + 2");
    TestIsBalanced(true, "( 3 * 3 )");
    TestIsBalanced(true, "( 4 / 3 ) * ( 5 - 2 )");
    TestIsBalanced(true, "( 1 + 1 ) - ( 2 + 2 ) * ( 2 - 1 )");
    TestIsBalanced(true, "1 + ( 2 * ( 9 / 3 ) - ( 1 * 5 ) - 3 ) * 88");
    TestIsBalanced(true, "( ( ) ( ( ) ( ) ) )");
    TestIsBalanced(true, "2 3 4");
    TestIsBalanced(true, "1 + ( 2 + 4 )");

    // All these are NOT balanced
    TestIsBalanced(false, "(");
    TestIsBalanced(false, ")");
    TestIsBalanced(false, ") (");
    TestIsBalanced(false, "( ( (");
    TestIsBalanced(false, "( ( ( )");
    TestIsBalanced(false, "( ( ( ) ) ) )");
    TestIsBalanced(false, "( ) ) ( ) (");

        
    /*******************************************************
     *** Test Infix to Postfix
     *******************************************************/
	TestCase::PrintBanner("Test Infix to Postfix");

    // Test addition and subtraction operators
    TestInfixToPostfix(true, "10 + 20", "10 20 +");
    TestInfixToPostfix(true, "24 - 11", "24 11 -");
    TestInfixToPostfix(true, "61 + 72 + 39 + 43", "61 72 + 39 + 43 +");
    TestInfixToPostfix(true, "111 + 22 - 30 + 44", "111 22 + 30 - 44 +");

    // Test multiplication and division operators
    TestInfixToPostfix(true, "111 * 2222", "111 2222 *");
    TestInfixToPostfix(true, "9 / 333", "9 333 /");
    TestInfixToPostfix(true, "1 * 22 * 333 * 4444", "1 22 * 333 * 4444 *");
    TestInfixToPostfix(true, "1 * 22 / 333 * 4444", "1 22 * 333 / 4444 *");

    // Test mixing high and low priority operators without parenthesis
    TestInfixToPostfix(true, "11 + 2 * 333 - 4", "11 2 333 * + 4 -");
    TestInfixToPostfix(true, "123 + 23 + 3456 * 45 + 567 + 6", "123 23 + 3456 45 * + 567 + 6 +");
    TestInfixToPostfix(true, "1 + 12 + 123 * 34 * 2345 + 456", "1 12 + 123 34 * 2345 * + 456 +");
	
    // Parenthesis
    TestInfixToPostfix(true, "( 100 + 20 )", "100 20 +");
    TestInfixToPostfix(true, "( 1777 + 26 ) * 355", "1777 26 + 355 *");
    TestInfixToPostfix(true, "398 * ( 1 + 2 )", "398 1 2 + *");
    TestInfixToPostfix(true, "( 39482 - 92 ) / ( 19328 - 398 )", "39482 92 - 19328 398 - /");
    TestInfixToPostfix(true, "( 39482 - 92 ) * ( 19328 - 398 ) * ( 6 - 398 )", "39482 92 - 19328 398 - * 6 398 - *");
    TestInfixToPostfix(true, "19328 * ( 39482 - ( 2 + 6 ) / 81 )", "19328 39482 2 6 + 81 / - *");
    TestInfixToPostfix(true, "92 + ( 2 + ( 398 + ( 81 + ( 19328 + 6 ) ) ) )", "92 2 398 81 19328 6 + + + + +");
    TestInfixToPostfix(true, "92 + ( 2 - ( 398 * ( 81 / ( 19328 + 6 ) ) ) )", "92 2 398 81 19328 6 + / * - +");

    // infixToPostfix must validate matching parenthesis
	TestInfixToPostfix(false, "( 92 + 2", "");
	TestInfixToPostfix(false, ")", "");
	TestInfixToPostfix(false, "92 + 2 ) + 398", "");
	TestInfixToPostfix(false, "92 + 2 ( 81 + 398", "");
	TestInfixToPostfix(false, "92 + 2 ( 81 ( 6 + 398 )", "");
	TestInfixToPostfix(false, "92 + 2 ( 81 + ) 6 + 398 ) * 7", "");


    /*******************************************************
     *** Test Infix to Postfix
     *******************************************************/
    TestPostfixEvaluate(true, "398 8 +", 406);
    TestPostfixEvaluate(true, "19328 11 -", 19317);
    TestPostfixEvaluate(true, "6 7 *", 42);
    TestPostfixEvaluate(true, "39482 398 /", 99);

    TestPostfixEvaluate(true, "398 1 2 + *", 1194);
    TestPostfixEvaluate(true, "39482 12 - 19328 398 - /", 2);
    TestPostfixEvaluate(true, "39482 12 - 19328 398 - * 398 6 - /", 1906038);
    TestPostfixEvaluate(true, "12 382 300 24 + 81 / - *", 4536);
    TestPostfixEvaluate(true, "1 2 398 81 19328 6 + + + + +", 19816);
    TestPostfixEvaluate(true, "398 1568 112 1218 81 6 + / * - +", 398);

	// All of these postfix expressions are bad.
	TestPostfixEvaluate(false, "398 1568 43 + + / ", 0); // Too many operators
	TestPostfixEvaluate(false, "123 756 (", 0); // ( is not an operator
	TestPostfixEvaluate(false, "123 756 * 23", 0); // Extra values left on the stack

    TestCase::PrintSummary();
    
    return 0;
}


