#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::uint32_t WIDTH = 800;
const std::uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers{ "VK_LAYER_KHRONOS_validation" };

// Debug��: ValidationLayer(���؃��C��) �L��
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
	// initWindow: GLFW���C�u������������
	void initWindow();

	void initVulkan();

	void mainLoop();

	void cleanup();

	// �C���X�^���X�쐬
	void createInstance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	void setupDebugMessenger();

	void pickPhysicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice device);

public:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	std::vector<const char*> getRequiredExtensions();

	// �v�����ꂽ���C���[���S�ė��p�\���m�F
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

	VkInstance instance;	// �C���X�^���X�ւ̃n���h���ێ��p
	VkDebugUtilsMessengerEXT debugMessenger;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};