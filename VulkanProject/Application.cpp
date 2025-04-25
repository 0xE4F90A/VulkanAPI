#include "pch.h"
#include "Application.h"

// Vulkan�f�o�b�O���b�Z�[�W���[�쐬
/* ���؃��C���Ƃ�
 * Vulkan API�̓h���C�o�̃I�[�o�[�w�b�h���ŏ����ɗ}����Ƃ����A�C�f�A�Ɋ�Â��Đ݌v����Ă���B
 * �f�t�H���g��API�̃G���[�`�F�b�N�����Ɍ����Ă���B*/
VkResult Application::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// vkCreateDebugUtilsMessengerEXT�ւ̃|�C���^�擾
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	// �|�C���^���L���ł���΁A�f�o�b�O���b�Z�[�W���[�쐬
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else // �����݂��Ȃ��ꍇ�̓G���[�Ԃ�
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

// Vulkan�f�o�b�O���b�Z�[�W���[�j��
void Application::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	// vkDestroyDebugUtilsMessengerEXT�ւ̃|�C���^�擾
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	// �|�C���^���L���ł���΁A�f�o�b�O���b�Z�[�W���[�j��
	if (func != nullptr)
		func(instance, debugMessenger, pAllocator);
}

struct QueueFamilyIndices
{
	std::optional<std::uint32_t> graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value();
	}
};

// GLFW���g���ăE�B���h�E������
void Application::initWindow()
{
	glfwInit(); // GLFW���C�u����������

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // OpenGL�R���e�L�X�g�쐬���Ȃ�
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // �E�B���h�E�T�C�Y�Œ�

	// �E�B���h�E���쐬 (WIDTH, HEIGHT�̓E�B���h�E�̕��ƍ����A"Hello Vulkan"�̓^�C�g��)
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Vulkan", nullptr, nullptr);
}

// Vulkan�֘A������
void Application::initVulkan()
{
	createInstance();       // Vulkan�C���X�^���X�쐬
	setupDebugMessenger();  // �f�o�b�O���b�Z�[�W���[�Z�b�g�A�b�v
}

// �A�v���P�[�V�������s�J�n
void Application::run()
{
	initWindow();  // �E�B���h�E������
	initVulkan();  // Vulkan������
	mainLoop();    // ���C�����[�v���s
	cleanup();     // ���\�[�X��n��
}

// ���C�����[�v���s
void Application::mainLoop()
{
	// �E�B���h�E��������܂Ń��[�v

	while (!glfwWindowShouldClose(window))
		glfwPollEvents(); // �C�x���g����(��: �}�E�X�A�L�[�{�[�h����)

}

// �A�v���P�[�V�����I�������\�[�X���
void Application::cleanup()
{
	if (enableValidationLayers) // �o���f�[�V�������C���[���L���ł���΃f�o�b�O���b�Z�[�W���[�j��
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

	vkDestroyInstance(instance, nullptr); // Vulkan�C���X�^���X�j��

	glfwDestroyWindow(window); // �E�B���h�E�j��

	glfwTerminate(); // GLFW���C�u�����I��
}

// Vulkan�C���X�^���X�쐬
void Application::createInstance()
{
	// �o���f�[�V�������C���[���L���ŃT�|�[�g����Ă��Ȃ��ꍇ�A�G���[�X���[
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not availablel.\n");

	VkApplicationInfo appInfo{};                            // �A�v���P�[�V�������
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello, Triangle";           // �A�v���P�[�V������
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // �A�v���P�[�V�����o�[�W����
	appInfo.pEngineName = "No Engine";                      // �G���W����
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);       // �G���W���o�[�W����
	appInfo.apiVersion = VK_API_VERSION_1_0;                // �g�p����Vulkan�o�[�W����

	VkInstanceCreateInfo createInfo{};                      // �C���X�^���X�쐬���
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;                 // �A�v���P�[�V�������ݒ�

	auto extensions = getRequiredExtensions();              // �K�v�Ȋg���@�\�擾
	createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()); // �g���@�\�̐��ݒ�
	createInfo.ppEnabledExtensionNames = extensions.data(); // �g���@�\�̖��O�ݒ�

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};   // �f�o�b�O���b�Z�[�W���[�쐬���
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size()); // �L���ɂ���o���f�[�V�������C���[�̐���ݒ�
		createInfo.ppEnabledLayerNames = validationLayers.data(); // �o���f�[�V�������C���[�̖��O��ݒ�

		populateDebugMessengerCreateInfo(debugCreateInfo);  // �f�o�b�O���b�Z�[�W���[���ݒ�
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo; // �g�����Ƃ��Đݒ�
	}
	else
	{
		createInfo.enabledExtensionCount = 0;
		createInfo.pNext = nullptr;
	}

	// Vulkan�C���X�^���X���쐬���A���s�����ꍇ�͗�O�X���[
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!\n");
}

// �f�o�b�O���b�Z�[�W���[�̍쐬���ݒ�
void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {}; // �\���̂��[��������
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;     // �\���̂̌^
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT    // ���e���郁�b�Z�[�W�̐[���x
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT            // ���e���郁�b�Z�[�W�̃^�C�v
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;                                     // ���[�U�[��`�̃R�[���o�b�N
}

// �f�o�b�O���b�Z�[�W���[�Z�b�g�A�b�v
void Application::setupDebugMessenger()
{
	if (!enableValidationLayers) return; // �o���f�[�V�������C���[�������Ȃ�I��

	VkDebugUtilsMessengerCreateInfoEXT createInfo;  // �쐬���p�ϐ�
	populateDebugMessengerCreateInfo(createInfo);   // �쐬��񏉊���

	// �f�o�b�O���b�Z�[�W���[���쐬���A���s�����ꍇ�͗�O�X���[
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to set up debug messenger!\n");
}

void Application::pickPhysicalDevice()
{
	std::uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Failed to find a suitable GPU.");
}

bool Application::isDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = findQueueFamilies(device);
	return indices.isComplete();
}

QueueFamilyIndices Application::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	std::uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		if (indices.isComplete())
			break;

		++i;
	}

	return indices;
}

// �K�v�Ȋg���@�\�擾
std::vector<const char*> Application::getRequiredExtensions()
{
	std::uint32_t glfwExtensionCount = 0; // �g���@�\�̐����i�[����ϐ�
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // �K�v�Ȋg���@�\�擾

	// �g���@�\��std::vector�Ɋi�[
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) // �o���f�[�V�������C���[���L���Ȃ�f�o�b�O�g���ǉ�
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions; // �g���@�\�̃��X�g��Ԃ�
}

// �o���f�[�V�������C���[���T�|�[�g����Ă��邩�m�F
bool Application::checkValidationLayerSupport()
{
	std::uint32_t layerCount{}; // ���p�\�ȃ��C���[�̐��p
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); // ���C���[���擾

	std::vector<VkLayerProperties> availableLayers(layerCount); // ���p�\�ȃ��C���[�����i�[����x�N�^�[
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); // ���C���[���擾

	// ���N�G�X�g���ꂽ�o���f�[�V�������C���[���g���邩�`�F�b�N
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false; // ���C���[�������������t���O

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0) // ���O����v���邩
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound) // ���C���[��������Ȃ����false
			return false;
	}
	return true; // �S�Ẵ��C���[�����p�\�Ȃ�true
}
