#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define SIZE 256
#define maxColors 255

// Reference
// http://netpbm.sourceforge.net/doc/ppm.html

//represents a single pixel object
typedef struct RGB {
    unsigned char r, g, b;
}RGBPix;

//this struct is used to store the entire image data, along with the width and height
typedef struct Image {
    int width, height;
    RGBPix *data;
}PPMImage;

typedef struct MONO {
    uint8_t b[3];
}MONOByte;

//this struct is used to store the entire image data, along with the width and height
typedef struct inImage {
    int width, height;
    MONOByte *data;
}PBMImage;

uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int main(int argc, char* argv[]) {

    char* infilename;
    char* outfilename;
    FILE* infile;
    FILE* outfile;
    unsigned short pixel; // 16-bits per pixel
    unsigned int invert; // invert bitmap
    int read,i,j;
    PPMImage image;
    PBMImage inimage;
    char buff[SIZE], *fh;
    uint32_t indata;
    // Parse Args
    if (argc < 4) {
        printf("Usage: %s infile invert outfile.\n", argv[0]);
        printf("EX: %s infile.pbm 0(1 enables) fb.ppm.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    infilename = argv[1];
    invert = atoi(argv[2]);
    outfilename = argv[3];

  //make sure they are providing a .ppm file
  if(strstr(infilename, ".pbm") == NULL) {
    perror("Please provide a .pbm file for conversion");
    return 0;
  }
  if(strstr(outfilename, ".ppm") == NULL) {
    perror("Please provide a valid .ppm file name to write to");
    return 0;
  }

    if (maxColors <= 0 || maxColors >= 256) {
        printf("Err: depth must be between 1 and 256");
        exit(EXIT_FAILURE);
    }

    // Open appropriate files
    infile = fopen(infilename, "r");
    outfile = fopen(outfilename, "wb");

    fh = fgets(buff, SIZE, infile);           //Get the magic number first
    printf("Infile:\n");
    printf("%s\n",buff );
    if ( (fh == NULL) || ( strncmp(buff, "P4\n", 3) != 0 ) ) perror("Please provide a P4 .pbm file for conversion\n");

    (void) fprintf(outfile, "P6\n");

    //handle any and all comments
    do
        {
            fh = fgets(buff, SIZE,  infile);
            if( strncmp(buff, "#", 1) == 0) fprintf(outfile, "%s", buff);
            printf("%s",buff);
            if ( fh == NULL ) return 1;
        } while ( strncmp(buff, "#", 1) == 0 );

    //read in the width and height
    read = sscanf(buff, "%u %u", &inimage.width, &inimage.height);
    if(read < 2) {
        perror("File Unreadable. Please check the file format\n");
        return 1;
    }
    image.width = inimage.width / 3;
    image.height = inimage.height;

    //allocate memory for the image buffer
    image.data = (RGBPix *)malloc(sizeof(RGBPix) * image.width * image.height);
    inimage.data = (MONOByte *)malloc(sizeof(MONOByte) * inimage.width * inimage.height / 24);


    // P3 - PPM "plain" header
    fprintf(outfile, "#converted with ppmmonodlp\n%d %d\n%d\n", image.width, image.height, maxColors);

    //read the image into the buffer
    fread(inimage.data, sizeof(MONOByte), inimage.width * inimage.height / 24, infile);

    for (i = 0; i < image.width * image.height; i=i+8) {
        indata = reverse(inimage.data[(i>>3)].b[0]) | (reverse(inimage.data[(i>>3)].b[1]) << 8) | (reverse(inimage.data[(i>>3)].b[2]) << 16);

        for (j = 0;j < 8;j++) {
            if (invert == 1) {
                image.data[i+j].r = (((indata >> ((j*3)+0)) & 0x01) * maxColors);
                image.data[i+j].g = (((indata >> ((j*3)+1)) & 0x01) * maxColors);
                image.data[i+j].b = (((indata >> ((j*3)+2)) & 0x01) * maxColors);
            } else {
                image.data[i+j].r = ~(((indata >> ((j*3)+0)) & 0x01) * maxColors);
                image.data[i+j].g = ~(((indata >> ((j*3)+1)) & 0x01) * maxColors);
                image.data[i+j].b = ~(((indata >> ((j*3)+2)) & 0x01) * maxColors);
            }
        }
    }

    //write the binary out
    fwrite(image.data, 3 * image.width, image.height, outfile);
    printf("\nOutfile:\nP6\n\n%s width: %d height: %d\n", outfilename, image.width, image.height);

    //clean up
    (void) fclose(outfile);
    (void) fclose( infile);
    return EXIT_SUCCESS;
}
