#include "SOIL.h"
#include <stdio.h>
#include <math.h>

enum
{
    COL_RED,
    COL_GREEN,
    COL_BLUE,
    COL_ALPHA,
};

enum
{
    SUCCESS,
    ERR_GENERIC
};

bool CharArrayToInt(char* char_array, int* output)
{
    *output = 0;
    int num_count = 0;
    while (char_array[++num_count] != NULL);
    for (int i = 0; i < num_count; i++)
    {
        if (char_array[i] < '0' || char_array[i] > '9')
        {
            return false;
        }
        int number = char_array[i] - '0';
        *output += number*(int)(pow(10.f, num_count - 1 - i) + 0.5);
    }

    return true;
}

int main(int argc, char* argv[])
{
    //#define DEBUG1
    //#define DEBUG2
    //#define DEBUG3

#ifdef DEBUG1
    argc = 15;
    argv = new char*[argc];
    argv[1] = "test.tga";
    argv[2] = "out.tga";
    argv[3] = "-width";
    argv[4] = "18";
    argv[5] = "-height";
    argv[6] = "32";
    argv[7] = "-r";
    argv[8] = "1r";
    argv[9] = "-g";
    argv[10] = "1g";
    argv[11] = "-b";
    argv[12] = "1ri";
    argv[13] = "-a";
    argv[14] = "1a";
#endif // DEBUG1
#ifdef DEBUG2
    argc = 12;
    argv = new char*[argc];
    argv[1] = "test.tga";
    argv[2] = "test2.tga";
    argv[3] = "out.tga";
    argv[4] = "-r";
    argv[5] = "1r";
    argv[6] = "-g";
    argv[7] = "1g";
    argv[8] = "-b";
    argv[9] = "2ri";
    argv[10] = "-a";
    argv[11] = "1a";
#endif // DEBUG2
#ifdef DEBUG3
    argc = 12;
    argv = new char*[argc];
    argv[0] = "test.tga";
    argv[1] = "test2.tga";
    argv[2] = "-r";
    argv[3] = "1r";
    argv[4] = "-g";
    argv[5] = "1g";
    argv[6] = "-b";
    argv[7] = "1ri";
    argv[8] = "-a";
    argv[9] = "1a";
    argv[10] = "test2.tga";
    argv[11] = "out.tga";
#endif // DEBUG3

    if (argc < 5)
    {
        fprintf(stderr, "ERROR: Not enough arguments supplied to be able to make a valid command\n");
        return ERR_GENERIC;
    }

    // NUMBER OF FILES
    int file_count = 0;
    for (int arg = 1; arg < argc; arg++)
    {
        if (argv[arg][0] != '-')
        {
            file_count++;
        }
        else
        {
            break;
        }

        if (file_count > 3)
        {
            fprintf(stderr, "ERROR: It seems like there are too many image inputs, max amount is 2\n");
            return ERR_GENERIC;
        }
    }

    int width[3], height[3], channels[3];
    unsigned char *image[3];

    // OUTPUT FILE FORMAT
    int output_path_length = 0;
    while (argv[file_count][++output_path_length] != NULL);
    if (output_path_length < 5)
    {
        fprintf(stderr, "ERROR: Output file name is too short to even contain a file ending\n");
        return ERR_GENERIC;
    }

    char file_extension[] =
    {
        argv[file_count][output_path_length - 4],
        argv[file_count][output_path_length - 3],
        argv[file_count][output_path_length - 2],
        argv[file_count][output_path_length - 1]
    };

    if (file_extension[0] != '.')
    {
        fprintf(stderr, "ERROR: Output file name does not have a valid file extension (.bmp, .tga or .dds)\n");
        return ERR_GENERIC;
    }

    int save_format = 0;
    if (file_extension[1] == 'b' && file_extension[2] == 'm' && file_extension[3] == 'p')
    {
        save_format = SOIL_SAVE_TYPE_BMP;
    }
    else if (file_extension[1] == 't' && file_extension[2] == 'g' && file_extension[3] == 'a')
    {
        save_format = SOIL_SAVE_TYPE_TGA;
    }
    else if (file_extension[1] == 'd' && file_extension[2] == 'd' && file_extension[3] == 's')
    {
        save_format = SOIL_SAVE_TYPE_DDS;
    }
    else
    {
        fprintf(stderr, "ERROR: Output file extension is not valid, must be .bmp, .tga or .dds (lower case)\n");
        return ERR_GENERIC;
    }

    // LOAD INPUT IMAGES
    image[0] = SOIL_load_image
        (
        argv[1],
        &width[0], &height[0], &channels[0],
        SOIL_LOAD_AUTO
        );
    if (!image[0])
    {
        fprintf(stderr, "ERROR: Could not load input image 1\n");
        return ERR_GENERIC;
    }

    if (file_count > 2)
    {
        image[1] = SOIL_load_image
            (
            argv[2],
            &width[1], &height[1], &channels[1],
            SOIL_LOAD_AUTO
            );
        if (!image[1])
        {
            fprintf(stderr, "ERROR: Could not load input image 2\n");
            return ERR_GENERIC;
        }
    }
    else
    {
        image[1] = 0;
        width[1] = 0;
        height[1] = 0;
        channels[1] = 0;
    }

    width[2] = width[0] > width[1] ? width[0] : width[1];
    height[2] = height[0] > height[1] ? height[0] : height[1];
    channels[2] = save_format == SOIL_SAVE_TYPE_BMP ? 3 : 4;
    int image_size;
    bool size_set = false;

    // Go through flags and create image.
    for (int flag = file_count + 1; flag < argc; flag++)
    {
        int flag_length = 0;
        while (argv[flag][++flag_length] != NULL);

        // FLAG
        if (argv[flag][0] == '-')
        {
            if (flag_length == 6 && argv[flag][1] == 'w' && argv[flag][2] == 'i' && argv[flag][3] == 'd' && argv[flag][4] == 't' && argv[flag][5] == 'h')
            {
                if (size_set)
                {
                    fprintf(stderr, "The -width flag needs to be located after the output image, but before the channel flags\n");
                    return ERR_GENERIC;
                }
                flag++;
                if (flag > argc)
                {
                    fprintf(stderr, "Supplied width flag without a number\n");
                    return ERR_GENERIC;
                }
                if (!CharArrayToInt(argv[flag], &width[2]))
                {
                    fprintf(stderr, "Width flag supplied with invalid number\n");
                    return ERR_GENERIC;
                }
            }
            else if (flag_length == 7 && argv[flag][1] == 'h' && argv[flag][2] == 'e' && argv[flag][3] == 'i' && argv[flag][4] == 'g' && argv[flag][5] == 'h' && argv[flag][6] == 't')
            {
                if (size_set)
                {
                    fprintf(stderr, "The -height flag needs to be located after the output image, but before the channel flags\n");
                    return ERR_GENERIC;
                }
                flag++;
                if (flag > argc)
                {
                    fprintf(stderr, "Supplied height flag without a number\n");
                    return ERR_GENERIC;
                }
                if (!CharArrayToInt(argv[flag], &height[2]))
                {
                    fprintf(stderr, "Height flag supplied with invalid number\n");
                    return ERR_GENERIC;
                }
            }
            else if (flag_length == 2)
            {
                if (!size_set)
                {
                    if (width[2] < 1)
                    {
                        fprintf(stderr, "The supplied width is too small, must be at least 1\n");
                        return ERR_GENERIC;
                    }
                    if (height[2] < 1)
                    {
                        fprintf(stderr, "The supplied height is too small, must be at least 1\n");
                        return ERR_GENERIC;
                    }

                    image_size = width[2] * height[2] * channels[2] * sizeof(char);
                    image[2] = new unsigned char[image_size]();
                    size_set = true;
                }
                // TO CHANNEL
                int to_channel = 0;
                switch (argv[flag][1])
                {
                case 'r':
                {
                    to_channel = COL_RED;
                    break;
                }
                case 'g':
                {
                    to_channel = COL_GREEN;
                    break;
                }
                case 'b':
                {
                    to_channel = COL_BLUE;
                    break;
                }
                case 'a':
                {
                    to_channel = COL_ALPHA;
                    break;
                }
                default:
                {
                    fprintf(stderr, "ERROR: Supplied non-exsistent destination channel in flag\n");
                    return ERR_GENERIC;
                }
                }

                if (channels[2] < to_channel + 1)
                {
					fprintf(stderr, "WARNING: A supplied target channel does not exsist\n");
					flag++;
					continue;
                }

                // ARGUMENT
                if (argc < flag + 1)
                {
                    fprintf(stderr, "ERROR: Missing argument after flag\n");
                    return ERR_GENERIC;
                }
                int argument_length = 0;
                while (argv[flag + 1][++argument_length] != NULL);

                if (argument_length < 2 || argument_length > 3)
                {
                    fprintf(stderr, "ERROR: Invalid length of argument after flag\n");
                    return ERR_GENERIC;
                }
                flag++;

                // FROM IMAGE
                unsigned char* from_image = 0;
                int from_image_index = 0;
                switch (argv[flag][0])
                {
                case '1':
                {
                    from_image_index = 0;
                    from_image = image[from_image_index];
                    break;
                }
                case '2':
                {
                    from_image_index = 1;
                    from_image = image[from_image_index];
                    break;
                }
                default:
                {
                    fprintf(stderr, "ERROR: Invalid argument after flag, must be 1 or 2\n");
                    return ERR_GENERIC;
                }
                }

                if (from_image_index == 1 && file_count < 3)
                {
                    fprintf(stderr, "ERROR: Tried using the second input image, but only one input image was found.\n");
                    return ERR_GENERIC;
                }

                // FROM CHANNEL
                int from_channel = 0;
                switch (argv[flag][1])
                {
                case 'r':
                {
                    from_channel = COL_RED;
                    break;
                }
                case 'g':
                {
                    from_channel = COL_GREEN;
                    break;
                }
                case 'b':
                {
                    from_channel = COL_BLUE;
                    break;
                }
                case 'a':
                {
                    from_channel = COL_ALPHA;
                    break;
                }
                default:
                {
                    fprintf(stderr, "ERROR: Invalid channel after image number in argument, must be r, g, b or a\n");
                    return ERR_GENERIC;
                }
                }

				bool from_channel_exists = true;
                if (channels[from_image_index] < from_channel + 1)
                {
                    fprintf(stderr, "WARNING: A supplied source channel does not exsist\n");
					from_channel_exists = false;
                }

                // INVERT
                bool invert = false;
                if (argument_length == 3)
                {
                    if (argv[flag][2] == 'i')
                    {
                        invert = true;
                    }
                    else
                    {
                        fprintf(stderr, "ERROR: Invalid letter after channnel letter in argument, must be i to invert the channel or nothing\n");
                        return ERR_GENERIC;
                    }
                }

                // PERFORM SHUFFLE
                int x_ratio = (width[from_image_index] << 16) / width[2] + 1;
                int y_ratio = (height[from_image_index] << 16) / height[2] + 1;
                int x2, y2;

                for (int i = 0; i < height[2]; i++)
                {
                    for (int k = 0; k < width[2]; k++)
                    {
                        x2 = (k*x_ratio) >> 16;
                        y2 = (i*y_ratio) >> 16;
						int right_side = from_channel_exists ? (from_image[y2*width[from_image_index] * channels[from_image_index] + from_channel + x2*channels[from_image_index]]) : 0;
						image[2][i*width[2] * channels[2] + k*channels[2] + to_channel] = abs(invert*255 - right_side);
                    }
                }
            }
            else
            {
                fprintf(stderr, "ERROR: Faulty argument\n");
                return ERR_GENERIC;
            }
        }
        else
        {
            fprintf(stderr, "ERROR: Faulty argument, expected a dash\n");
            return ERR_GENERIC;
        }
    }

    fprintf(stderr, "Writing file: ");
    fprintf(stderr, argv[file_count]);
    fprintf(stderr, " . . . ");
	if (!SOIL_save_image
		(
		argv[file_count],
		save_format,
		width[2], height[2], channels[2],
		image[2]
		)
		)
	{
		fprintf(stderr, "\nERROR: Could not save image, does the target directory exist?\n");
		return ERR_GENERIC;
	}

    fprintf(stderr, "Done\n");
    return(SUCCESS);
}