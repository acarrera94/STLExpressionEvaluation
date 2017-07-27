#include <exception>

#include "ExpressionManager.h"

// TODO: Put the name of your header file here:
#include "MyExpressMan.h"


IExpressionManager *ExpressionManagerFactory::GetExpressionManager()
{
    // TODO: Return a new instance of your expression manager class.
	return new MyExpressMan();
}
