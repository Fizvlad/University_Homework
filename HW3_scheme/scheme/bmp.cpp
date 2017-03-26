#include <cstdio>
#include <cstdlib>

class pixel {
    unsigned char B; // (1B)
    unsigned char G; // (1B)
    unsigned char R; // (1B)

    void set(unsigned char* c) {
        R = c[0];
        G = c[1];
        B = c[2];
    }

    pixel() {
        R = 255;
        G = 255;
        B = 255;
    }
    pixel(const pixel& in) {
        R = in.R;
        G = in.G;
        B = in.B;
    }
    ~pixel() {
    }

    friend class BMP_Image;
};

class BMP_Image {
// BitMapFileHeader (14B)
    unsigned short signature; // File signature = "BM" (2B)
    unsigned long size; // File size (4B)
    unsigned short reserved_1; // Reserved field (2B)
    unsigned short reserved_2; // Reserved field (2B)
    unsigned long shift; // Shift of image from file beginning = 54 (4B)

// BitMapInfoHeader (40B)
    unsigned long header_size; // Size of BitMapFileHeader = 40 (4B)
    long width; // Picture width (4B)
    long height; // Picture height. Way of line counting depends on sign (4B)
    unsigned short layers_amount; // Layers amount = 1 (2B)
    unsigned short color_depth; // Color depth = 24; 8b for each color (2B)
    unsigned long compression; // Compression type = 0 (4B)
    unsigned long pixels_size; // = 0 (if we are storing pixels as array) or size of pixels array = width * abs(height) * 3 (4B)
    long horizontal_resolution; // Horizontal resolution (pixels per meter) or 0 (4B)
    long vertical_resolution; // Vertical resolution (pixels per meter) or 0 (4B)
    unsigned long color_table_size; // Color table size = 0 (4B)
    unsigned long used_colors_size; // Size of fields with used colors = 0 (4B)

// BitMapArray
    pixel* pixels;

//Custom fields
    const char* name;

public:
    void point (float x, float y, unsigned char* Color) {
        x = floor(x);
        y = floor(y);
        if (x < 0 || y < 0 || x >= width || y >= abs(height)) {
            // Pixel is out of borders
            return;
        }
        pixels[(int)(y * width + x)].set(Color);
    }

    void cut (float x1, float y1, float x2, float y2, unsigned char* Color) {
        x1 = floor(x1) + 0.5;
        x2 = floor(x2) + 0.5;
        y1 = floor(y1) + 0.5;
        y2 = floor(y2) + 0.5;

        if (x1 == x2 && y1 == y2) {
            point(x1, y1, Color);
            return;
        }

        if (x2 == x1) {
            if (y2 > y1) {
                float ty = y2;
                y2 = y1;
                y1 = ty;
            }
            // Now y2 < y1
            for (float y = y2; y <= y1; y += 1) {
                point(x1, y, Color);
            }
        } else {
            float k = (y2 - y1) / (x2 - x1);
            float b = y1 - k * x1;
            if (abs(k) > 1) {
                if (y1 > y2) {
                    float tx = x2;
                    float ty = y2;
                    x2 = x1;
                    y2 = y1;
                    x1 = tx;
                    y1 = ty;
                }
                // Now y1 < y2
                for (float y = y1; y <= y2; y += 1) {
                    float x = (y - b) / k;
                    point(x, y, Color);
                }
            } else {
                if (x1 > x2) {
                    float tx = x2;
                    float ty = y2;
                    x2 = x1;
                    y2 = y1;
                    x1 = tx;
                    y1 = ty;
                }
                // Now x1 < x2
                for (float x = x1; x <= x2; x += 1) {
                    float y = k * x + b;
                    point(x, y, Color);
                }
            }
        }
    }

    void circle (float x0, float y0, float Radius, unsigned char* Color) {
        x0 = floor(x0) + 0.5;
        y0 = floor(y0) + 0.5;
        float xLeft = x0 - Radius;
        float xRight = x0 + Radius;
        for (float x = xLeft; x <= xRight; x += 1) {
            float t = sqrt(pow(Radius, 2) - pow(x - x0, 2));
            point(x, y0 + t, Color);
            point(x, y0 - t, Color);
        }
        float yDown = y0 + Radius;
        float yUp = y0 - Radius;
        for (float y = yUp; y <= yDown; y += 1) {
            float t = sqrt(pow(Radius, 2) - pow(y - y0, 2));
            point(x0 - t, y, Color);
            point(x0 + t, y, Color);
        }
    }

    void print() {
        /*
            Really bad way of visualizing :\
        */
        for (unsigned y = 0; y < abs(height); y++) {
            for (unsigned x = 0; x < width; x++) {
                printf("%c%d%c%d%c%d%c", '(', (int)(pixels[y * abs(height) + x].R), ';', (int)(pixels[y * abs(height) + x].G), ';', (int)(pixels[y * abs(height) + x].B), ')');
            }
            printf("%c", '\n');
        }
    }

    void render() {
        FILE* out = fopen(name, "w");
        if (out == NULL) {
            fprintf(stderr, "%s", "Can not create or open a file\n");
            return;
        }
        unsigned saved_size = 0;

        // BitMapFileHeader (14B)
        saved_size += 2 * fwrite(&signature, 2, 1, out); // File signature = "BM" (2B)
        saved_size += 4 * fwrite(&size, 4, 1, out); // File size (4B)
        saved_size += 2 * fwrite(&reserved_1, 2, 1, out); // Reserved field (2B)
        saved_size += 2 * fwrite(&reserved_2, 2, 1, out); // Reserved field (2B)
        saved_size += 4 * fwrite(&shift, 4, 1, out); // Shift of image from file beginning = 54 (4B)

        // BitMapInfoHeader (40B)
        saved_size += 4 * fwrite(&header_size, 4, 1, out); // Size of BitMapFileHeader = 40 (4B)
        saved_size += 4 * fwrite(&width, 4, 1, out); // Picture width (4B)
        saved_size += 4 * fwrite(&height, 4, 1, out);; // Picture height. Way of line counting depends on sign (4B)
        saved_size += 2 * fwrite(&layers_amount, 2, 1, out); // Layers amount = 1 (2B)
        saved_size += 2 * fwrite(&color_depth, 2, 1, out); // Color depth = 24; 8b for each color (2B)
        saved_size += 4 * fwrite(&compression, 4, 1, out); // Compression type = 0 (4B)
        saved_size += 4 * fwrite(&pixels_size, 4, 1, out); // = 0 (if we are storing pixels as array) or size of pixels array = width * abs(height) * 3 (4B)
        saved_size += 4 * fwrite(&horizontal_resolution, 4, 1, out); // Horizontal resolution (pixels per meter) or 0 (4B)
        saved_size += 4 * fwrite(&vertical_resolution, 4, 1, out); // Vertical resolution (pixels per meter) or 0 (4B)
        saved_size += 4 * fwrite(&color_table_size, 4, 1, out); // Color table size = 0 (4B)
        saved_size += 4 * fwrite(&used_colors_size, 4, 1, out); // Size of fields with used colors = 0 (4B)

        // ColorTable (Optional)
        // SKIPPING => (0B)

        // BitMapArray
        for (unsigned y = 0; y < abs(height); y++) {
            for (unsigned x = 0; x < width; x++) {
                saved_size += 3 * fwrite(pixels + width * y + x, 3, 1, out);
            }
            // Need to make each line divisible by 4B
            unsigned char n = 0;
            for (short i = 0; i < 4 - (width * 3) % 4; i++) {
                saved_size += fwrite(&n, 1, 1, out);
            }
        }

        fclose(out);

        if (saved_size == size) {
            printf("%s", "Image saved successfully!\n");
        } else {
            fprintf(stderr, "%s%d%s%d%c", "Can not save image: \nSaved elements: ", saved_size, "\nHave to save: ", size, '\n');
        }
    }

    BMP_Image(unsigned Width, unsigned Height, const char* Name) {
        signature = 19778; // "BM"
        size = (3 * Width + 4 - (Width * 3) % 4) * Height + 54; // 4B for each pixel + 54B for header and info
        reserved_1 = 0;
        reserved_2 = 0;
        shift = 54; // 54B of header and info

        header_size = 40; // 40B of header
        width = Width;
        height = -1 * Height;
        layers_amount = 1;
        color_depth = 24;
        compression = 0;
        pixels_size = 0;
        horizontal_resolution = 0;
        vertical_resolution = 0;
        color_table_size = 0;
        used_colors_size = 0;

        pixels = new pixel[Width * Height];

        name = Name;
    }
    BMP_Image(const BMP_Image& in) {
        signature = in.signature;
        size = in.size;
        reserved_1 = in.reserved_1;
        reserved_2 = in.reserved_2;
        shift = in.shift;

        header_size = in.header_size;
        width = in.width;
        height = in.height;
        layers_amount = in.layers_amount;
        color_depth = in.color_depth;
        compression = in.compression;
        pixels_size = in.pixels_size;
        horizontal_resolution = in.horizontal_resolution;
        vertical_resolution = in.vertical_resolution;
        color_table_size = in.color_table_size;
        used_colors_size = in.used_colors_size;

        pixels = new pixel[width * height];
        for (unsigned i = 0; i < width * height; i++) {
            pixels[i] = in.pixels[i];
        }

        // Do not changing name
    }
    ~BMP_Image() {
        delete pixels;
        delete name;
    }
};
