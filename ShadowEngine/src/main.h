#include "core/Application.h"
#include "core/Utils.h"
#include <core\Log.h>

int main()
{
	Application* application = createApplication();
	delete application;

	return 0;
}