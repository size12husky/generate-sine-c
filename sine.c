#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define SAMPLERATE 44100
#define DURATION 3
#define TOTALSAMPLES (SAMPLERATE * DURATION)

struct RIFFheader
{
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
};

struct FormatChunk
{
    char chunk_id[4];
    uint32_t chunk_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

struct DataChunk
{
    char chunk_id[4];
    uint32_t chunk_size;
};

int main()
{

    struct RIFFheader riff_header;

    riff_header.chunk_id[0] = 'R';
    riff_header.chunk_id[1] = 'I';
    riff_header.chunk_id[2] = 'F';
    riff_header.chunk_id[3] = 'F';

    riff_header.format[0] = 'W';
    riff_header.format[1] = 'A';
    riff_header.format[2] = 'V';
    riff_header.format[3] = 'E';

    riff_header.chunk_size = 24 + 8 + (TOTALSAMPLES * sizeof(short));

    struct FormatChunk format_chunk;

    format_chunk.chunk_id[0] = 'f';
    format_chunk.chunk_id[1] = 'm';
    format_chunk.chunk_id[2] = 't';
    format_chunk.chunk_id[3] = ' ';
    format_chunk.chunk_size = 16;
    format_chunk.audio_format = 1;
    format_chunk.num_channels = 1;
    format_chunk.sample_rate = SAMPLERATE;
    format_chunk.bits_per_sample = 16;
    format_chunk.byte_rate = SAMPLERATE * format_chunk.num_channels * (format_chunk.bits_per_sample / 8);
    format_chunk.block_align = format_chunk.num_channels * (format_chunk.bits_per_sample / 8);

    struct DataChunk data_chunk;

    data_chunk.chunk_id[0] = 'd';
    data_chunk.chunk_id[1] = 'a';
    data_chunk.chunk_id[2] = 't';
    data_chunk.chunk_id[3] = 'a';
    data_chunk.chunk_size = TOTALSAMPLES * sizeof(short);

    // FILE LOADING
    FILE *file;
    file = fopen("mysine.wav", "wb");
    if (!file)
    {
        printf("Error opening file\n");
        return 1;
    };
    printf("File loaded successfully!");
    // END FILE LOADING

    fwrite(&riff_header, sizeof(struct RIFFheader), 1, file);
    fwrite(&format_chunk, sizeof(struct FormatChunk), 1, file);
    fwrite(&data_chunk, sizeof(struct DataChunk), 1, file);

    printf("Total samples: %d\n", TOTALSAMPLES);
    for (int i = 0; i < TOTALSAMPLES; i++)
    {
        double time = (double)i / SAMPLERATE;

        double current_freq;
        if (i < TOTALSAMPLES / 5)
        {
            current_freq = 293.66;
        }
        else if (i < (TOTALSAMPLES / 5) * 2)
        {
            current_freq = 196;
        }
        else if (i < (TOTALSAMPLES / 5) * 3)
        {
            current_freq = 220;
        }
        else if (i < (TOTALSAMPLES / 5) * 4)
        {
            current_freq = 329.63;
        }
        else
        {
            current_freq = 261.63;
        };

        double sine_input = 2 * M_PI * current_freq * time;
        double sine_result = sin(sine_input);
        double scaled_sine = sine_result * 32767;
        short audio_sample = (short)(scaled_sine);
        fwrite(&audio_sample, sizeof(short), 1, file);
    }

    fclose(file);
    return 0;
}