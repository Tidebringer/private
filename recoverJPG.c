
/**
 * recoverJPEG.c
 * Recovers JPEGs from a forensic image.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
int main(int argc, char* argv[])
{
    // opens the forensic image.
    FILE* file = fopen("card.raw", "r"); 
    FILE* img;
    
    uint8_t block[512];
    bool found = false;
    char title[8];
    int counter = 0;
    int open = 0;
    
    // reads into the forensic image 512 bytes at a time.
    fread(&block, sizeof(block), 1, file);
     
    // stops if End of File is reached.
    while (fread(&block, sizeof(block), 1, file) > 0)
    {
        // checks for .jpg signature among all bytes in the block.
        for (int i = 0; i < sizeof(block); i++)
        {
   
            if (block[i] == 255 && block[i + 1] == 216 && block[i + 2] == 255 
            && (block[i + 3] == 225 || block[i + 3] == 224))
            {
                sprintf(title, "%03d.jpg", counter);
                counter++;
                found = true;
            }
            
        }   
    
        // begins to write the found .jpg in a new file.
        if (open == 0 && found == true)
        {
            img = fopen(title, "w");
            fwrite(&block, sizeof(block), 1, img);
            open = 1;  
        }
        
        // closes the current file and writes to a new when a new .jpg is found
        if (open == 1 && found == true)
        {
            fclose(img);
            img = fopen(title, "w");
            fwrite(&block, sizeof(block), 1, img);
        }
        
        // continues writing until a new .jpg signature is found.
        if (open == 1 && found == false)
        {
            fwrite(&block, sizeof(block),1, img);
        }
        
        found = false;
 
    }
    
    fclose(img);
    fclose(file);
	return 0; 
}