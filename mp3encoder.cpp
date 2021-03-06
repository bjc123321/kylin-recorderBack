#include "mp3encoder.h"

Mp3Encoder::Mp3Encoder(int rate, int channels,QObject *parent) : QObject(parent)
{
    gfp = lame_init();//初始化编码参数

    this->channels = channels;
    lame_set_num_channels(gfp,channels);  //设置pcm数据的声道数
    lame_set_in_samplerate(gfp, rate); //采样率
    lame_set_brate(gfp,128);  // 128kbps 编码率

    lame_set_mode(gfp, channels>=2 ? STEREO : MONO);    //设置输出的mp3数据的声道, 0==STEREO, 3==MONO
    lame_set_quality(gfp,2);   /* 2=high  5 = medium  7=low */

    lame_init_params(gfp);
}
Mp3Encoder::~Mp3Encoder()
{
    lame_close(gfp);
}

int Mp3Encoder::getLastMp3Data(char *mp3_data, int mp3data_len)
{
    return lame_encode_flush(gfp, (quint8 *)mp3_data, mp3data_len);
}

int Mp3Encoder::encode(short *pcm_data, int pcmdata_len, char *mp3_data, int mp3data_len)
{
    int num_samples = pcmdata_len>>1;
    if (channels > 1)
        num_samples >>= 1; //双声道的处理

    return lame_encode_buffer_interleaved(gfp, pcm_data, num_samples, (unsigned char *)mp3_data, mp3data_len);
}
