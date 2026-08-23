#ifndef _RENDERER_HH_
#define _RENDERER_HH_

#include <cstdint>

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

struct RenderContext {
    VkDevice device;
    VkPhysicalDevice physical_device;

VkQueue graphics_queue;
VkQueue present_queue;
std::uint32_t graphics_queue_index;
std::uint32_t present_queue_index;

VmAllocator allocator;
VkCommandPool command_pool;
VkDescriptorPool descriptor_pool;

VkDescriptorSetLayout global_descriptor_global_layout;
VkDescriptorSetLayout texture_descriptor_set_layout;
VkRenderPass render_pass;

PFN_vkCmdBeginRenderingKHR vk_cmd_begin_rendering_khr;
PFN_vkCmdEndRenderingKHR vk_cmd_end_rendering_khr;
};





class Renderer {
    public:

    auto cleanup() -> void;


    [[nodiscard]] auto get_window() const -> SDL_Window*;

    [[nodiscard]] auto get_context() const -> const RenderContext&;

    [[nodiscard]] auto get_api_version() const -> std::uint32_t;

    [[nodiscard]] auto get_frame_buffer_size() const -> VkExtent2D;

    [[nodiscard]] auto get_min_image_count() const noexcept -> std::uint32_t;

    [[nodiscard]] auto get_image_count() const noexcept -> std::uint32_t;

    [[nodiscard]] auto get_swap_chain_image(std::uint32_t img_index) const -> VkImage;

    [[nodiscard]] auto get_swap_chain_image_view(std::uint32_t img_index) const -> VkImageView;


    private:
    auto on_window_resize() -> void;



};


#endif // _RENDERER_HH_
