#ifndef GRAPHIX_APP_BACKEND_VULKAN_UTIL_HPP_
#define GRAPHIX_APP_BACKEND_VULKAN_UTIL_HPP_

// ---------------------------------------------------------------------------
//  Validation configuration
// ---------------------------------------------------------------------------

/// Severity bitmask, mirrors VkDebugUtilsMessageSeverityFlagBitsEXT but keeps
/// the public API free of Vulkan enums so callers can configure it from a
/// config file or an ImGui checkbox without including vulkan.h.
enum class VulkanLogLevel : std::uint32_t {
  none = 0u,
  error = 1u << 0,
  warning = 1u << 1,
  info = 1u << 2,
  verbose = 1u << 3,
  defaults = error | warning,
  all = error | warning | info | verbose,
};

[[nodiscard]] constexpr VulkanLogLevel operator|(VulkanLogLevel a, VulkanLogLevel b) noexcept {
  return static_cast<VulkanLogLevel>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}
[[nodiscard]] constexpr bool has(VulkanLogLevel set, VulkanLogLevel bit) noexcept {
  return (static_cast<std::uint32_t>(set) & static_cast<std::uint32_t>(bit)) != 0u;
}

struct VulkanValidationOptions {
  /// Master switch. Defaults to on in debug builds, off in release.
  bool enabled =
#ifdef NDEBUG
      false;
#else
      true;
#endif
  /// VK_LAYER_KHRONOS_validation feature toggles (VK_EXT_validation_features).
  bool synchronization = true;   ///< catches missing barriers / hazards — the #1 Vulkan bug class
  bool bestPractices = false;    ///< vendor-agnostic performance advice; noisy, enable on demand
  bool gpuAssisted = false;      ///< descriptor-indexing / buffer-OOB checks; costs real frame time
  bool debugPrintf = false;      ///< debugPrintfEXT() from shaders; mutually exclusive with gpuAssisted
  /// Turn the first validation ERROR into a CgxUiStatus failure instead of a log line.
  /// Invaluable in CI; leave off for interactive sessions.
  bool errorsAreFatal = false;
  VulkanLogLevel level = VulkanLogLevel::defaults;
};

struct VulkanContextCreateInfo {
  std::string applicationName = "CalculiX GraphiX";
  std::uint32_t applicationVersion = VK_MAKE_VERSION(2, 23, 0);
  std::uint32_t apiVersion = VK_API_VERSION_1_3;
  /// Extensions SDL_Vulkan_GetInstanceExtensions() reported. Copied, not aliased.
  std::span<const char* const> instanceExtensions{};
  std::vector<const char*> deviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  VulkanValidationOptions validation{};
};


#endif // GRAPHIX_APP_BACKEND_VULKAN_UTIL_HPP_
