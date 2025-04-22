#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::uint32_t WIDTH = 800;
const std::uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers{ "VK_LAYER_KHRONOS_validation" };

// Debug時: ValidationLayer(検証レイヤ) 有効
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class Application
{
public:
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	void run();

private:
	// initWindow: GLFWライブラリを初期化
	void initWindow();

	void initVulkan();

	void mainLoop();

	void cleanup();

	// インスタンス作成
	void createInstance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void setupDebugMessenger();

	void pickPhysicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice device);

public:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	std::vector<const char*> getRequiredExtensions();

	// 要求されたレイヤーが全て利用可能か確認
	bool checkValidationLayerSupport();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::stringstream log{};
		log << "validation layer: " << pCallbackData->pMessage << std::endl;
		OutputDebugStringA(log.str().c_str());
		return VK_FALSE;
	}

private:
	GLFWwindow* window;

	VkInstance instance;	// インスタンスへのハンドル保持用
	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};