#include <iostream>
#include "header/vulkan_mini.h"
int main(){
	auto x = vkm::Window::builder{}.set_size({ 800,600 }).set_pos({100,100}).set_title("test").set_fullscreen_exclusive().build();
	while (x.is_valid()) {
		vkm::Window::update();
		std::cout<<x.get_window_size().x<<" "<<x.get_window_size().y<<"\n";
	}
}
