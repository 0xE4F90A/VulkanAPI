#include "pch.h"
#include "Application.h"

// Vulkanデバッグメッセージャー作成
/* 検証レイヤとは
 * Vulkan APIはドライバのオーバーヘッドを最小限に抑えるというアイデアに基づいて設計されている。
 * デフォルトでAPIのエラーチェックが非常に限られている。*/
VkResult Application::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// vkCreateDebugUtilsMessengerEXTへのポインタ取得
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	// ポインタが有効であれば、デバッグメッセージャー作成
	if (func != nullptr)
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	else // が存在しない場合はエラー返す
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}

// Vulkanデバッグメッセージャー破棄
void Application::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	// vkDestroyDebugUtilsMessengerEXTへのポインタ取得
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	// ポインタが有効であれば、デバッグメッセージャー破棄
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

// GLFWを使ってウィンドウ初期化
void Application::initWindow()
{
	glfwInit(); // GLFWライブラリ初期化

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // OpenGLコンテキスト作成しない
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // ウィンドウサイズ固定

	// ウィンドウを作成 (WIDTH, HEIGHTはウィンドウの幅と高さ、"Hello Vulkan"はタイトル)
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Vulkan", nullptr, nullptr);
}

// Vulkan関連初期化
void Application::initVulkan()
{
	createInstance();       // Vulkanインスタンス作成
	setupDebugMessenger();  // デバッグメッセージャーセットアップ
}

// アプリケーション実行開始
void Application::run()
{
	initWindow();  // ウィンドウ初期化
	initVulkan();  // Vulkan初期化
	mainLoop();    // メインループ実行
	cleanup();     // リソース後始末
}

// メインループ実行
void Application::mainLoop()
{
	// ウィンドウが閉じられるまでループ

	while (!glfwWindowShouldClose(window))
		glfwPollEvents(); // イベント処理(例: マウス、キーボード入力)

}

// アプリケーション終了時リソース解放
void Application::cleanup()
{
	if (enableValidationLayers) // バリデーションレイヤーが有効であればデバッグメッセージャー破棄
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

	vkDestroyInstance(instance, nullptr); // Vulkanインスタンス破棄

	glfwDestroyWindow(window); // ウィンドウ破棄

	glfwTerminate(); // GLFWライブラリ終了
}

// Vulkanインスタンス作成
void Application::createInstance()
{
	// バリデーションレイヤーが有効でサポートされていない場合、エラースロー
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not availablel.\n");

	VkApplicationInfo appInfo{};                            // アプリケーション情報
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello, Triangle";           // アプリケーション名
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);  // アプリケーションバージョン
	appInfo.pEngineName = "No Engine";                      // エンジン名
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);       // エンジンバージョン
	appInfo.apiVersion = VK_API_VERSION_1_0;                // 使用するVulkanバージョン

	VkInstanceCreateInfo createInfo{};                      // インスタンス作成情報
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;                 // アプリケーション情報設定

	auto extensions = getRequiredExtensions();              // 必要な拡張機能取得
	createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()); // 拡張機能の数設定
	createInfo.ppEnabledExtensionNames = extensions.data(); // 拡張機能の名前設定

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};   // デバッグメッセージャー作成情報
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size()); // 有効にするバリデーションレイヤーの数を設定
		createInfo.ppEnabledLayerNames = validationLayers.data(); // バリデーションレイヤーの名前を設定

		populateDebugMessengerCreateInfo(debugCreateInfo);  // デバッグメッセージャー情報設定
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo; // 拡張情報として設定
	}
	else
	{
		createInfo.enabledExtensionCount = 0;
		createInfo.pNext = nullptr;
	}

	// Vulkanインスタンスを作成し、失敗した場合は例外スロー
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!\n");
}

// デバッグメッセージャーの作成情報設定
void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {}; // 構造体をゼロ初期化
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;     // 構造体の型
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT    // 許容するメッセージの深刻度
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT            // 許容するメッセージのタイプ
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;                                     // ユーザー定義のコールバック
}

// デバッグメッセージャーセットアップ
void Application::setupDebugMessenger()
{
	if (!enableValidationLayers) return; // バリデーションレイヤーが無効なら終了

	VkDebugUtilsMessengerCreateInfoEXT createInfo;  // 作成情報用変数
	populateDebugMessengerCreateInfo(createInfo);   // 作成情報初期化

	// デバッグメッセージャーを作成し、失敗した場合は例外スロー
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

// 必要な拡張機能取得
std::vector<const char*> Application::getRequiredExtensions()
{
	std::uint32_t glfwExtensionCount = 0; // 拡張機能の数を格納する変数
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // 必要な拡張機能取得

	// 拡張機能をstd::vectorに格納
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) // バリデーションレイヤーが有効ならデバッグ拡張追加
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	return extensions; // 拡張機能のリストを返す
}

// バリデーションレイヤーがサポートされているか確認
bool Application::checkValidationLayerSupport()
{
	std::uint32_t layerCount{}; // 利用可能なレイヤーの数用
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); // レイヤー数取得

	std::vector<VkLayerProperties> availableLayers(layerCount); // 利用可能なレイヤー情報を格納するベクター
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); // レイヤー情報取得

	// リクエストされたバリデーションレイヤーが使えるかチェック
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false; // レイヤーが見つかったかフラグ

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0) // 名前が一致するか
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound) // レイヤーが見つからなければfalse
			return false;
	}
	return true; // 全てのレイヤーが利用可能ならtrue
}
