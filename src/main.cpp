#include <iostream>

#include "tgaimage.h"
#include "model.h"

Model* model = nullptr;

int width = 400;
int height = 400;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false; //是否斜率大于1
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) { //如果斜率大于1，交换x和y
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) { 
        std::swap(x0, x1);
        std::swap(y0, y1); 
    }
    float k = 1.0 * (y1 - y0) / (x1 - x0);
    for (int x = x0; x <= x1; x++) {
        int y = int(k * (x - x0) + y0);
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        
    }
}

Vec2i world2screen(Vec3f v){
  return Vec2i(int((v.x + 1.0) * width / 2.0), int((v.y + 1.0) * height / 2.0));
}

int main(int argc, char** argv) {
    if (argc == 1 || argc > 2) {
        model = new Model("/Users/atwbzhang/workspace/soft_renderer/obj/african_head/african_head.obj");
    } else {
        model = new Model(argv[1]);
    }

    TGAImage image(width, height, TGAImage::RGB);

    for (int i = 0; i < model->nverts(); i++) {
        Vec3f v = model->vert(i);
        Vec2i p = world2screen(v);
        image.set(p.x, p.y, white);
    }

    line(13, 20, 80, 40, image, white); //线段A
    line(20, 13, 40, 80, image, red); //线段B
    line(80, 40, 13, 20, image, red);//线段C

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}

