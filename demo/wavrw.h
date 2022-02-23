
#include <stdint.h>
#include <stdio.h>

struct wav_hdr {
    char w_riff[4];
    uint32_t w_fsize;
    char w_wave[4];
    char w_fmt[4];
    uint32_t w_fmtsize;
    uint16_t w_type;
    uint16_t w_nchannels;
    uint32_t w_srate;
    uint32_t w_brate;
    uint16_t w_align;
    uint16_t w_bitdepth;
    char w_dhead[4];
    uint32_t w_dsize;
};

typedef struct wav_file {
    struct wav_hdr hdr;
    FILE * file;
    _Static_assert(sizeof(struct wav_hdr) == 44, "Bad wav_hdr size");
} wav_file_t;

int wav16_read_head(wav_file_t*);
int wav16_read_sample(wav_file_t*,int16_t*);
int wav16_write_head(wav_file_t*);
int wav16_write_sample(wav_file_t*,int16_t*);

#define WAV_NUM_CHANNELS(w) (w->hdr.w_nchannels)
#define WAV_SAMPLE_RATE(w) (w->hdr.w_srate)

