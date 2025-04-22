#include "pch.h"
#include "Application.h"

//--------- Drawing a triangle ---------//
// setup
// Base code
// Instance
// Validation layers - 2025/01/28

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
//int main(int argc, char* argv[])
{
	auto app = std::make_unique<Application>();

	try
	{
		app->run();
	}
	catch (const std::exception& e)
	{
		std::stringstream log{};
		log << e.what() << std::endl;
		OutputDebugStringA(log.str().c_str());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		OutputDebugStringA("UNKNOWN ERROR.");
		//std::cerr <<  << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}