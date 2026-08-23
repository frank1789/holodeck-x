#ifndef UI_RENDERER_HH_
#define UI_RENDERER_HH_

#include <vulkan/vulkan_core.h>

#include <vector>

#include "renderer.hh"

class UIRenderer {
 public:
  ~UIRenderer() noexcept;
  UIRenderer();

  auto cleanup() -> void;

  auto begin_frame() -> void;

  VkCommandBuffer PrepareCommandBuffer(int img_index);

 private:
  auto create_descriptor_pool() -> void;
  auto setup_imgui() -> void;

  Renderer* renderer_{nullptr};

  VkFormat color_format_{VK_FORMAT_UNDEFINED};
  VkFormat depth_format_{VK_FORMAT_UNDEFINED};
  VkExtent2D frame_buffer_size_{0, 0};
  VkDescriptorPool descriptor_pool_{VK_NULL_HANDLE};

  std::vector<VkCommandBuffer> command_buffers_;
};

#endif  // UI_RENDERER_HH_
