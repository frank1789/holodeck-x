#include "ui_renderer.hh"

#include <fmt/format.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>
#include <imgui.h>

#include <stdexcept>

#ifndef IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE
#define IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE 500
#endif

UIRenderer::~UIRenderer() noexcept {}

UIRenderer::UIRenderer() {}

UIRenderer::UIRenderer(Renderer* renderer) {
  renderer_ = renderer;
  frame_buffer_size_ = renderer_->get_frame_buffer_size();

  create_descriptor_pool();
  setup_imgui();
}

auto UIRenderer::cleanup() -> void {
  auto& context = renderer_->get_context();
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  if (descriptor_pool_ != VK_NULL_HANDLE) {
    vkDestroyDescriptorPool(context.device, descriptor_pool_, nullptr);
    descriptor_pool_ = VK_NULL_HANDLE;
  }

  command_buffers_.clear();
}

auto UIRenderer::begin_frame() -> void {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

auto UIRenderer::PrepareCommandBuffer(int img_index) -> VkCommandBuffer {
  auto& context = renderer_->get_context();
  frame_buffer_size_ = renderer_->get_frame_buffer_size();

  ImGuiIO& io = ImGui::GetIO();
  io.DisplaySize.x = static_cast<float>(frame_buffer_size_.width);
  io.DisplaySize.y = static_cast<float>(frame_buffer_size_.height);

  ImGui::Render();
  VkCommandBuffer cmd_buffer = command_buffers_.at(img_index);
  VkImage img = renderer_->get_swap_chain_image(img_index);
  VkImageView img_view = renderer_->get_swap_chain_image_view(img_index);

  VkCommandBufferBeginInfo begin_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  vkBeginCommandBuffer(cmd_buffer, &begin_info);

  RenderUtils::AddImageMemoryBarrier(cmd_buffer, img,
                                     VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

  VkRenderingAttachmentInfoKHR colour_attachment {
    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
    .imageView = img_view,
    .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE
  };

  VkRenderingInfoKHR rendering_info {
    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
    .renderArea.offset = {0, 0},
    .renderArea.extent = frame_buffer_size_,
    .layerCount = 1,
    .colorAttachmentCount = 1,
    .pColorAttachments = &colour_attachment
  };

  context.vkCmdbeginRenderingKHR(cmd_buffer, &rendering_info);
      ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd_buffer);
  context.vkRenderingKHR(cmd_buffer);
  RenderUtils::AddImageMemoryBarrier(cmd_buffer, img,
                                     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                     VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

  vkEndCommandBuffer(cmd_buffer);
  return cmd_buffer;
}

auto UIRenderer::create_descriptor_pool() -> void {
  auto& context = renderer_->get_context();
  // clang-format off
  VkDescriptorPoolSize pool_sizes[] = {
    { VK_DESCRIPTOR_TYPE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_SAMPLER_POOL_SIZE },
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
    { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, IMGUI_IMPL_VULKAN_MINIMUM_SAMPLED_IMAGE_POOL_SIZE }
  };
  // clang-format on
  VkDescriptorPoolCreateInfo pool_info {};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  pool_info.maxSets = 0;


  for (VkDescriptorPoolSize& pool_size : pool_sizes) {
    pool_info.maxSets += pool_size.descriptorCount;
  }

  pool_info.poolSizeCount = static_cast<uint32_t>(IM_COUNTOF(pool_sizes));
  pool_info.pPoolSizes = pool_sizes;

  vkCreateDescriptorPool(context.device, &pool_info, nullptr, &descriptor_pool_);

  command_buffers_.resize(renderer_->get_image_count());
  VkCommandBufferAllocateInfo alloc_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = context.command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = static_cast<uint32_t>(command_buffers_.size())
  };

  const auto res = vkAllocateCommandBuffers(context.device, &alloc_info,
                                            command_buffers_.data());
  if (res != VK_SUCCESS) {
    const auto msg_err = fmt::format("Failed to allocate ImGui command buffer");
    throw std::runtime_error(msg_err);
  }
}

auto UIRenderer::setup_imgui() -> void {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  io.DisplaySize.x = static_cast<float>(frame_buffer_size_.width);
  io.DisplaySize.y = static_cast<float>(frame_buffer_size_.height);

  ImGui::GetStyle().FontScaleMain = 1.0f;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForVulkan(renderer_->get_window());

  auto& context = renderer_->get_context();
  ImGui_ImplVulkan_InitInfo info {
    .ApiVersion = renderer_->get_api_version(),
    .Instance = renderer_->get_instance(),
    .PhysicalDevice = context.physical_device,
    .QueueFamily = context.present_queue_index,
    .Queue = context.present_queue,
    .PipelineCache = nullptr,
    .DescriptorPool = descriptor_pool_,
    .MinImageCount = renderer_->get_min_image_count(),
    .ImageCount = renderer_->get_image_count(),
    .Allocator = nullptr
  };

  color_format_ = renderer_->get_swap_chain_image_format();
  depth_format_ = RenderUtils::FindDepthFormat(context);

  info.UseDynamicRendering = true;
  info.PipelineInfoMain.RenderPass = nullptr;
  info.PipelineInfoMain.Subpass = 0;
  info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

  info.PipelineInfoMain.PipelineRenderingCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
  info.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;
  info.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats =
      &color_format_;
  info.PipelineInfoMain.PipelineRenderingCreateInfo.depthAttachmentFormat =
      depth_format_;
  info.PipelineInfoMain.PipelineRenderingCreateInfo.stencilAttachmentFormat =
      VK_FORMAT_UNDEFINED;

  ImGui_ImplVulkan_Init(&info);
}
