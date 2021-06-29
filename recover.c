//Recover.c
//cs50
//Krishna Gaur
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Byte is defined to read one byte each time
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover forensic image\n");
        return 1;
    }
    //Stores the name of the file
    char filename[8];

    //Reading blocks of 512 bytes according to FAT system
    BYTE *Buffer = malloc(512 * sizeof(char));

    //Special Buffer to check EOF
    char *EOF_Check = malloc(512 * sizeof(char));

    //Opening file pointer to read the card
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Couldn't open file \n");
        fclose(inptr);
        return 1;
    }

    //Pointer to write images in the new jpg file
    FILE *img = NULL;

    //Counter to check no of images
    int i = -1;
    int position = 512;
    do
    {
        //Reading from card into Buffer
        fread(Buffer, 512 * sizeof(char), 1, inptr);
        if (feof(inptr))
        {
            break;
        }
        if (Buffer[0] == 0xff &&
            Buffer[1] == 0xd8 &&
            Buffer[2] == 0xff &&
            (Buffer[3] & 0xf0) == 0xe0)
        {
            ++i;
            if (i == 0)
            {
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Couldn't open the file \n");
                    return 1;
                }

                if (feof(inptr))
                {
                    fseek(inptr, -1 * 512, SEEK_CUR);
                    fread(EOF_Check, i * sizeof(char), 1, inptr);
                    for (int j = 0, flag = 0; i < 512 && flag != 1; ++i)
                    {
                        if (EOF_Check[i] == EOF)
                        {
                            fwrite(Buffer, i * sizeof(BYTE), 1, img);
                            flag = 1;
                        }
                    }
                }
                else
                {
                    //Writing into the file
                    fwrite(Buffer, position * sizeof(BYTE), 1, img);
                }
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Couldn't open the file \n");
                    return 1;
                }

                if (feof(inptr))
                {
                    fseek(inptr, -1 * 512, SEEK_CUR);
                    fread(EOF_Check, i * sizeof(char), 1, inptr);
                    for (int j = 0, flag = 0; i < 512 && flag != 1; ++i)
                    {
                        if (EOF_Check[i] == EOF)
                        {
                            fwrite(Buffer, i * sizeof(BYTE), 1, img);
                            flag = 1;
                        }
                    }
                }
                else
                {
                    //Writing into the file
                    fwrite(Buffer, position * sizeof(BYTE), 1, img);
                }
            }
        }
        else
        {
            if (i >= 0)
            {
                if (feof(inptr))
                {
                    fseek(inptr, -1 * 512, SEEK_CUR);
                    fread(EOF_Check, i * sizeof(char), 1, inptr);
                    for (int j = 0, flag = 0; i < 512 && flag != 1; ++i)
                    {
                        if (EOF_Check[i] == EOF)
                        {
                            fwrite(Buffer, i * sizeof(BYTE), 1, img);
                            flag = 1;
                        }
                    }
                }
                else
                {
                    //Writing into the file
                    fwrite(Buffer, position * sizeof(BYTE), 1, img);
                }
            }
        }
    }
    while (!feof(inptr)); //checking for end of file

    //closing the last image file stored
    fclose(inptr);

    //Dealocating memory
    free(Buffer);
    free(EOF_Check);

    return 0;
}
