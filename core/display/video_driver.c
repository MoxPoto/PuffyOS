#include <vendor/multiboot.h>
#include <drivers/video_driver.h>
#include <vendor/font8x8_basic.h>

multiboot_info_t* videoInfo;

static uint16_t pixelwidth = 0;

static inline void setPixel(uint8_t r, uint8_t g, uint8_t b, uint32_t index) {
	unsigned char* pixel = (unsigned char*)videoInfo->framebuffer_addr;
	pixel[index] = b;
	pixel[index + 1] = g;
	pixel[index + 2] = r;
}

static inline uint32_t getPixelLocation(uint32_t x, uint32_t y) { 
    uint32_t index = (y * videoInfo->framebuffer_pitch + x * pixelwidth);

    return index;
}

void video_initialize(multiboot_info_t* grubInfo) {
    videoInfo = grubInfo;
	pixelwidth = grubInfo->framebuffer_bpp / 8;   

	for(unsigned int x = 0; x < videoInfo->framebuffer_width; x++) {
		for(unsigned int y = 0; y < videoInfo->framebuffer_height; y++) {
			uint32_t index = getPixelLocation(x, y);

			setPixel(0, 0, 0, index);
		}
	}
}

struct FontSpaceInfo video_drawchar(uint32_t oX, uint32_t oY, struct Color3 col, char target) {
    struct FontSpaceInfo spacing;

    if (target == ' ') {
        spacing.width = 8;
        spacing.height = 8;
    }

    for (uint8_t y = 0; y < 8; y++) {
        char row = font8x8_basic[target][y];

        for (uint8_t x = 0; x < 8; x++) {
            if (row & (1 << x)) {
                // pixel is active in here

                uint32_t newX = (uint32_t)x + oX;
                uint32_t newY = (uint32_t)y + oY;

                if (spacing.width < x) {
                    spacing.width = (int)x;
                }

                if (spacing.height < y) {
                    spacing.height = (int)y;
                }

                uint32_t index = getPixelLocation(newX, newY);

                setPixel(col.r, col.g, col.b, index);
            }
        }
    }

    return spacing;
}