#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void stb()
{
    int w, h, n;

    //rgba
    //load image
    unsigned char *data = stbi_load("rgba.png", &w, &h, &n, 0);

    printf("%d, %d, %d\n", w, h, n);

    //change pixel

    //rgba,write 10 red pixel at line 11
    for (int dx = 0; dx < 10; ++dx)
    {
        data[n * w * 10 + dx * n + 0] = 255;
        data[n * w * 10 + dx * n + 1] = 0;
        data[n * w * 10 + dx * n + 2] = 0;
        data[n * w * 10 + dx * n + 3] = 255;
    }


    //write image
    stbi_write_png("write.png", w, h, n, data, w * n);

    stbi_image_free(data);

}
int main(){
	stb();
	return 0;
}

