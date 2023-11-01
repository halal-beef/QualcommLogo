#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
   printf("QualcommLogo\n");
   if(argc == 3)
   {
    // Variables
    char *filePath = argv[2];
    FILE *fp;
    unsigned char buffer[9];
    unsigned char bmpheader[3];
    int pos = 0x4008;
    int bmpOff[10]; // I know i shouldnt have the size fixed but what can i do :(
    int count = 0;

    // Actual stuff
    if(access(filePath, 0) == 0)
    {
        printf("file exists\n");
        fp = fopen(filePath, "rb");
        fseek(fp, 0x4000, SEEK_SET);
        fread(buffer, 1, 8, fp);
        if(strcmp(buffer, "LOGO!!!!") == 0)
        {
            printf("valid logo image\n");
            while (!feof(fp) && fread(bmpheader, 2, 1, fp) == 1)
            {
                if(strcmp(bmpheader, "BM") == 0)
                {
                    printf("Found a BMP header at %d\n", pos);
                    bmpOff[count] = pos;
                    count++;
                }
                pos += 2;
            }
            // Done with finding BMP headers
            
            int bmpSize[count];

            for (int i = 0; i < 10; i++)
            {
                if(bmpOff[i] != 0) // Offset of 0 shouldnt be possible
                {
                    int size;

                    fseek(fp, bmpOff[i] + 2, SEEK_SET); // Skip the initial "BM" header for now, we need to get our sizes
                    fread(&size, 1, 4, fp);
                    printf("%d\n",size);
                    bmpSize[i] = size;
                }
            }

            // Done finding sizes

            for (int j = 0; j < count; j++)
            {
                if(bmpSize[j] != 0)
                {
                    unsigned char image[bmpSize[j] + 1];
                    char filename[] = "image-";
                    char ext[] = ".bmp";

                    sprintf(filename, "%s%d%s", filename, j, ext);

                    fseek(fp, bmpOff[j], SEEK_SET);
                    fread(image, 1, bmpSize[j], fp);
                    FILE *file = fopen(filename, "wb");
                    fwrite(image, 1, bmpSize[j], file);
                    fclose(file);
                }
            }
            fclose(fp);
        }
    }
   }
   else
   {
    printf("Usage\n");
    printf("QualcommLogo -l <logo.img>\n");
   }
   return 0;
}
