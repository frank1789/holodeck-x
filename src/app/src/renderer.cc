#include "renderer.hh"

#include <ranges>
#include <vector>

namespace {

constexpr std::uint32_t MAX_FRAMES = 2;
constexpr std::uint32_t MAX_TEXTURES = 128;

constexpr const char* ValidationLayer = "VK_LAYER_KHRONOS_validation";

const std::vector<const char*> DeviceExtenions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME};

[[nodiscard]] bool layer_available(std::string_view name) noexcept {
  std::uint32_t count = 0;
  if (vkEnumerateInstanceLayerProperties(&count, nullptr) != VK_SUCCESS) {
    return false;
  }
  std::vector<VkLayerProperties> props(count);
  if (vkEnumerateInstanceLayerProperties(&count, props.data()) != VK_SUCCESS) {
    return false;
  }
  return std::ranges::any_of(
      props, [&](const VkLayerProperties& p) { return name == p.layerName; });
}

[[nodiscard]] bool extension_available(std::string_view name) noexcept {
  std::uint32_t count = 0;
  if (vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr) !=
      VK_SUCCESS) {
    return false;
  }
  std::vector<VkExtensionProperties> props(count);
  if (vkEnumerateInstanceExtensionProperties(nullptr, &count, props.data()) !=
      VK_SUCCESS) {
    return false;
  }
  return std::ranges::any_of(props, [&](const VkExtensionProperties& p) {
    return name == p.extensionName;
  });
}

}  // namespace

class Renderer::VulkanContext {
  ~VulkanContext() noexcept { destroy(); }

  VulkanContext(VulkanContext&& o) noexcept
      : instance_(std::exchange(o.instance_, VK_NULL_HANDLE)),
        surface_(std::exchange(o.surface_, VK_NULL_HANDLE)),
        messenger_(std::exchange(o.messenger_, VK_NULL_HANDLE)),
        physical_(std::exchange(o.physical_, VK_NULL_HANDLE)),
        device_(std::exchange(o.device_, VK_NULL_HANDLE)),
        graphicsQueue_(std::exchange(o.graphicsQueue_, VK_NULL_HANDLE)),
        presentQueue_(std::exchange(o.presentQueue_, VK_NULL_HANDLE)),
        graphicsFamily_(std::exchange(o.graphicsFamily_, UINT32_MAX)),
        presentFamily_(std::exchange(o.presentFamily_, UINT32_MAX)),
        deviceExtensions_(std::move(o.deviceExtensions_)),
        deviceName_(std::move(o.deviceName_)),
        validationEnabled_(std::exchange(o.validationEnabled_, false)),
        errorsAreFatal_(std::exchange(o.errorsAreFatal_, false)),
        pfnSetObjectName_(std::exchange(o.pfnSetObjectName_, nullptr)),
        pfnCmdBeginLabel_(std::exchange(o.pfnCmdBeginLabel_, nullptr)),
        pfnCmdEndLabel_(std::exchange(o.pfnCmdEndLabel_, nullptr)) {}

  VulkanContext& operator=(VulkanContext&& o) noexcept {
    if (this != &o) {
      destroy();
      new (this) VulkanContext(std::move(o));
    }
    return *this;
  }

  auto destroy() noexcept -> void {
    // Reverse creation order. vkDestroy* accept VK_NULL_HANDLE, so no
    // branching.
    if (device_ != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(device_);
      vkDestroyDevice(device_, nullptr);
      device_ = VK_NULL_HANDLE;
    }
    if (instance_ != VK_NULL_HANDLE) {
      if (surface_ != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        surface_ = VK_NULL_HANDLE;
      }
      if (messenger_ != VK_NULL_HANDLE) {
        if (auto pfn = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(instance_,
                                      "vkDestroyDebugUtilsMessengerEXT"))) {
          pfn(instance_, messenger_, nullptr);
        }
        messenger_ = VK_NULL_HANDLE;
      }
      vkDestroyInstance(instance_, nullptr);
      instance_ = VK_NULL_HANDLE;
    }
  }

  auto create_or_throw(const VulkanContextCreateInfo& info)
      -> Renderer::VulkanContext {
    auto res = create(info);
    if (!res) {
      throw std::runtime_error("failed to create Vulkan context");
    }
    return std::move(*res);
  }
};

Result<VulkanContext> VulkanContext::create(
    const VulkanContextCreateInfo& info) noexcept {
  VulkanContext ctx;
  ctx.deviceExtensions_ = info.deviceExtensions;
  ctx.errorsAreFatal_ = info.validation.errorsAreFatal;

  // ---- extensions -------------------------------------------------------
  std::vector<const char*> extensions(info.instanceExtensions.begin(),
                                      info.instanceExtensions.end());

  const bool wantValidation = info.validation.enabled;
  const bool haveLayer = wantValidation && layer_available(ValidationLayer);
  const bool haveDebugUtils =
      wantValidation && extension_available(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  if (wantValidation && !haveLayer) {
    // Degrade, do not fail: a developer without the SDK installed should still
    // get a running program. The caller can query validation_enabled().
    ctx.sink_->emit(
        VulkanLogLevel::warning,
        "VK_LAYER_KHRONOS_validation not found — continuing without "
        "validation. "
        "Install the Vulkan SDK or the vulkan-validation-layers package.");
  }

  if (haveDebugUtils) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  std::vector<const char*> layers;
  if (haveLayer) {
    layers.push_back(ValidationLayer);
  }
  ctx.validationEnabled_ = haveLayer;

  // ---- VK_EXT_validation_features (sync / best-practices / GPU-assisted) --
  std::vector<VkValidationFeatureEnableEXT> enableFeatures;
  std::vector<VkValidationFeatureDisableEXT> disableFeatures;
  if (haveLayer) {
    const auto& v = info.validation;
    if (v.synchronization) {
      enableFeatures.push_back(
          VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT);
    }
    if (v.bestPractices) {
      enableFeatures.push_back(VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT);
    }
    // GPU-assisted and debugPrintf share hardware resources and must not both
    // be on; prefer whichever the caller asked for, printf wins ties.
    if (v.debugPrintf) {
      enableFeatures.push_back(VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT);
    } else if (v.gpuAssisted) {
      enableFeatures.push_back(VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT);
    }
  }

  VkValidationFeaturesEXT validationFeatures{};
  validationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
  validationFeatures.enabledValidationFeatureCount =
      static_cast<std::uint32_t>(enableFeatures.size());
  validationFeatures.pEnabledValidationFeatures = enableFeatures.data();
  validationFeatures.disabledValidationFeatureCount =
      static_cast<std::uint32_t>(disableFeatures.size());
  validationFeatures.pDisabledValidationFeatures = disableFeatures.data();

  // ---- messenger, chained into instance creation --------------------------
  // Chaining it into pNext is what makes vkCreateInstance / vkDestroyInstance
  // themselves validated. A messenger created afterwards misses those calls.
  VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
  messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  messengerInfo.messageSeverity = to_vk_severity(info.validation.level);
  messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  messengerInfo.pfnUserCallback = &debug_callback;
  messengerInfo.pUserData = ctx.sink_.get();

  const void* chain = nullptr;
  if (haveLayer && !enableFeatures.empty()) {
    validationFeatures.pNext = haveDebugUtils ? &messengerInfo : nullptr;
    chain = &validationFeatures;
  } else if (haveDebugUtils) {
    chain = &messengerInfo;
  }

  // ---- instance -----------------------------------------------------------
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = info.applicationName.c_str();
  appInfo.applicationVersion = info.applicationVersion;
  appInfo.pEngineName = "cgx";
  appInfo.engineVersion = VK_MAKE_VERSION(2, 23, 0);
  appInfo.apiVersion = info.apiVersion;

  VkInstanceCreateInfo instanceInfo{};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = chain;
  instanceInfo.pApplicationInfo = &appInfo;
  instanceInfo.enabledExtensionCount =
      static_cast<std::uint32_t>(extensions.size());
  instanceInfo.ppEnabledExtensionNames = extensions.data();
  instanceInfo.enabledLayerCount = static_cast<std::uint32_t>(layers.size());
  instanceInfo.ppEnabledLayerNames = layers.data();

  const auto res = vkCreateInstance(&instanceInfo, nullptr, &ctx.instance_);
  if (res != VK_SUCCESS) {
    return std::unexpected(status_from_vk(r, CGX_UI_STATUS_VK_INSTANCE_FAILED));
  }

  // ---- persistent messenger + debug-utils entry points --------------------
  if (haveDebugUtils) {
    if (auto pfn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(ctx.instance_,
                                  "vkCreateDebugUtilsMessengerEXT"))) {
      if (const VkResult r =
              pfn(ctx.instance_, &messengerInfo, nullptr, &ctx.messenger_);
          r != VK_SUCCESS)
        return std::unexpected(
            status_from_vk(r, CGX_UI_STATUS_VK_DEBUG_MESSENGER_FAILED));
    }
    ctx.pfnSetObjectName_ = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
        vkGetInstanceProcAddr(ctx.instance_, "vkSetDebugUtilsObjectNameEXT"));
    ctx.pfnCmdBeginLabel_ = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(
        vkGetInstanceProcAddr(ctx.instance_, "vkCmdBeginDebugUtilsLabelEXT"));
    ctx.pfnCmdEndLabel_ = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(
        vkGetInstanceProcAddr(ctx.instance_, "vkCmdEndDebugUtilsLabelEXT"));
  }

  return ctx;
}

ui::Status VulkanContext::adopt_surface(VkSurfaceKHR surface) noexcept {
  if (instance_ == VK_NULL_HANDLE) {
    return CGX_UI_STATUS_NOT_INITIALISED;
  }
  if (surface == VK_NULL_HANDLE) {
    return CGX_UI_STATUS_INVALID_ARGUMENT;
  }
  if (surface_ != VK_NULL_HANDLE) {
    return CGX_UI_STATUS_ALREADY_INITIALISED;
  }
  surface_ = surface;
  return CGX_UI_STATUS_OK;
}

ui::Status VulkanContext::select_device() noexcept {
  if (instance_ == VK_NULL_HANDLE) {
    return CGX_UI_STATUS_NOT_INITIALISED;
  }
  if (surface_ == VK_NULL_HANDLE) {
    return CGX_UI_STATUS_NOT_INITIALISED;
  }
  if (device_ != VK_NULL_HANDLE) {
    return CGX_UI_STATUS_ALREADY_INITIALISED;
  }

  std::uint32_t count = 0;
  vkEnumeratePhysicalDevices(instance_, &count, nullptr);
  if (count == 0) {
    return CGX_UI_STATUS_VK_NO_PHYSICAL_DEVICE;
  }
  std::vector<VkPhysicalDevice> devices(count);
  vkEnumeratePhysicalDevices(instance_, &count, devices.data());

  // Score: discrete > integrated > anything, and reject devices that cannot
  // both render and present on our surface.
  int bestScore = -1;
  for (const VkPhysicalDevice pd : devices) {
    std::uint32_t qCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &qCount, nullptr);
    std::vector<VkQueueFamilyProperties> families(qCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &qCount, families.data());

    std::uint32_t gfx = UINT32_MAX;
    std::uint32_t present = UINT32_MAX;
    for (std::uint32_t i = 0; i < qCount; ++i) {
      if ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u &&
          gfx == UINT32_MAX) {
        gfx = i;
      }
      VkBool32 supported = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, surface_, &supported);
      if (supported == VK_TRUE && present == UINT32_MAX) {
        present = i;
      }
      // A family that does both is strictly preferable — avoids ownership
      // transfers.
      if ((families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u &&
          supported == VK_TRUE) {
        break;
      }
    }
    if (gfx == UINT32_MAX || present == UINT32_MAX) {
      continue;
    }

    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(pd, &props);
    int score = 0;
    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      score = 1000
    } else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
      score = 500
    } else {
      score = 100;
    }
    score += static_cast<int>(props.limits.maxImageDimension2D / 1024);

    if (score > bestScore) {
      bestScore = score;
      physical_ = pd;
      graphicsFamily_ = gfx;
      presentFamily_ = present;
      deviceName_ = props.deviceName;
    }
  }
  if (physical_ == VK_NULL_HANDLE) {
    return CGX_UI_STATUS_VK_NO_QUEUE_FAMILY;
  }

  // ---- logical device -----------------------------------------------------
  const float priority = 1.0f;
  std::vector<VkDeviceQueueCreateInfo> queueInfos;
  for (const std::uint32_t family :
       (graphicsFamily_ == presentFamily_)
           ? std::vector<std::uint32_t>{graphicsFamily_}
           : std::vector<std::uint32_t>{graphicsFamily_, presentFamily_}) {
    VkDeviceQueueCreateInfo qi{};
    qi.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qi.queueFamilyIndex = family;
    qi.queueCount = 1;
    qi.pQueuePriorities = &priority;
    queueInfos.push_back(qi);
  }

  // Vulkan 1.3 core features cgx will want: dynamic rendering removes render
  // passes/framebuffers entirely, synchronization2 gives per-stage barriers
  // that the sync-validation layer can reason about precisely.
  VkPhysicalDeviceVulkan13Features features13{};
  features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
  features13.dynamicRendering = VK_TRUE;
  features13.synchronization2 = VK_TRUE;

  VkDeviceCreateInfo deviceInfo{};
  deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceInfo.pNext = &features13;
  deviceInfo.queueCreateInfoCount =
      static_cast<std::uint32_t>(queueInfos.size());
  deviceInfo.pQueueCreateInfos = queueInfos.data();
  deviceInfo.enabledExtensionCount =
      static_cast<std::uint32_t>(deviceExtensions_.size());
  deviceInfo.ppEnabledExtensionNames = deviceExtensions_.data();

  const VkResult res =
      vkCreateDevice(physical_, &deviceInfo, nullptr, &device_);
  if (res != VK_SUCCESS) {
    return status_from_vk(res, CGX_UI_STATUS_VK_DEVICE_FAILED);
  }

  vkGetDeviceQueue(device_, graphicsFamily_, 0, &graphicsQueue_);
  vkGetDeviceQueue(device_, presentFamily_, 0, &presentQueue_);

  set_object_name(VK_OBJECT_TYPE_DEVICE,
                  reinterpret_cast<std::uint64_t>(device_), "cgx.device");
  set_object_name(VK_OBJECT_TYPE_QUEUE,
                  reinterpret_cast<std::uint64_t>(graphicsQueue_),
                  "cgx.queue.graphics");
  return CGX_UI_STATUS_OK;
}

void VulkanContext::set_object_name(VkObjectType type, std::uint64_t handle,
                                    std::string_view name) const {
  if (pfnSetObjectName_ == nullptr || device_ == VK_NULL_HANDLE) {
    return;
  }
  const std::string owned(name);
  VkDebugUtilsObjectNameInfoEXT info{};
  info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
  info.objectType = type;
  info.objectHandle = handle;
  info.pObjectName = owned.c_str();
  (void)pfnSetObjectName_(device_, &info);
}

void VulkanContext::begin_label(VkCommandBuffer cmd, std::string_view name,
                                const float rgba[4]) const noexcept {
  if (pfnCmdBeginLabel_ == nullptr) {
    return;
  }
  const std::string owned(name);
  VkDebugUtilsLabelEXT label{};
  label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
  label.pLabelName = owned.c_str();
  if (rgba != nullptr) {
    std::memcpy(label.color, rgba, sizeof(label.color));
  }
  pfnCmdBeginLabel_(cmd, &label);
}

void VulkanContext::end_label(VkCommandBuffer cmd) const noexcept {
  if (pfnCmdEndLabel_ != nullptr) {
    pfnCmdEndLabel_(cmd);
  }
}

ui::Status VulkanContext::check_validation() const noexcept {
  if (!errorsAreFatal_ || sink_ == nullptr) {
    return CGX_UI_STATUS_OK;
  }
  return sink_->error_count() == 0 ? CGX_UI_STATUS_OK
                                   : CGX_UI_STATUS_VK_VALIDATION_ERROR;
}

Renderer::~Renderer() noexcept =default;

Renderer::Renderer(SDL_Window* window) {
  window_ = window;
  context_.max_frames = MAX_FRAMES;

#if NDEBUG
  context_.is_debug = true;
#else
  context_.is_debug = false;
#endif

  vulkan_context_ = std::make_unique<VulkanContext>();
}
